#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Draw a core's front panel from cores/<core>/cfg/panel.yaml.

The static art (case, labels, button caps, unlit LEDs, key hints) becomes an 8 x 8 tile set with
one 16-colour palette, for a tile-map renderer in block RAM. Dynamic parts (LEDs, pressed
buttons) are listed with their rectangles; the HDL recolours them from the core's LED and key
bits. The character LCD and the status strip keep their own renderers; the panel leaves their
rectangles to them.

  panel_art.py PANEL.yaml --png OUT.png [--lit] [--pressed]   mock-up at the output resolution
  panel_art.py PANEL.yaml --stats                              tile and palette counts
"""
import argparse
import sys

import yaml
from PIL import Image, ImageDraw, ImageFont

FONT_DIRS = ['/opt/homebrew/Caskroom/font-inter/4.1/extras/ttf']
FONTS = {}


def font(size, weight='Medium'):
    key = (size, weight)
    if key not in FONTS:
        for d in FONT_DIRS:
            try:
                FONTS[key] = ImageFont.truetype(f'{d}/Inter-{weight}.ttf', size)
                break
            except OSError:
                pass
        else:
            FONTS[key] = ImageFont.load_default(size)
    return FONTS[key]


def rgb(c):
    c = c.lstrip('#')
    return tuple(int(c[i:i + 2], 16) for i in (0, 2, 4))


def shade(c, f):
    return tuple(max(0, min(255, int(v * f))) for v in c)


def text(d, xy, s, size, color, anchor='mm', weight='Medium', fit=None):
    f = font(size, weight)
    tw = d.textlength(s, font=f)
    if not fit or tw <= fit:
        d.text(xy, s, fill=color, font=f, anchor=anchor)
        return
    # squeeze horizontally to the box, as MAME's layout text does
    asc, desc = f.getmetrics()
    tmp = Image.new('RGBA', (int(tw) + 2, asc + desc + 2), (0, 0, 0, 0))
    ImageDraw.Draw(tmp).text((1, 1), s, fill=color + (255,), font=f)
    tmp = tmp.resize((max(1, int(fit)), tmp.height), Image.LANCZOS)
    x, y = xy
    x -= {'l': 0, 'm': tmp.width / 2, 'r': tmp.width}[anchor[0]]
    y -= tmp.height / 2 + 1
    d._image.paste(tmp, (round(x), round(y)), tmp)


def expand(spec):
    """Replace {type: lay} entries with the MAME layout's elements; attach key hints."""
    from lay2panel import convert
    out = []
    def norm(k):
        tag, mask = str(k).split(':')
        return f'{tag}:{int(mask, 0)}'.lower()
    keys = {norm(k): v for k, v in spec.get('keys', {}).items()}
    for e in spec['elements']:
        if e['type'] != 'lay':
            out.append(e)
            continue
        for el in convert(e['file'], e['view'], *e['rect']):
            if el['type'] == 'lcd' and e.get('lcd') == 'window':
                el = {'type': 'area', 'rect': el['rect'], 'color': '#0a0a0a', 'radius': 0}
            if el['type'] == 'button' and el.get('tag'):
                k = norm(f"{el['tag']}:{el['mask']}")
                if k in keys:
                    el['key'] = keys[k]
                    el['input'] = k
                el['label_style'] = 'none'
            if el['type'] == 'led' and el.get('name') in spec.get('leds', {}):
                el['bit'] = spec['leds'][el['name']]
            out.append(el)
    return out


def bevel_rect(d, r, face, radius=3, depth=2, pressed=False):
    x, y, w, h = r
    hi, lo = shade(face, 1.35), shade(face, 0.55)
    if pressed:
        hi, lo = lo, hi
    d.rounded_rectangle([x, y, x + w - 1, y + h - 1], radius, fill=lo)
    d.rounded_rectangle([x, y, x + w - 1 - depth, y + h - 1 - depth], radius, fill=hi)
    d.rounded_rectangle([x + depth // 2 + 1, y + depth // 2 + 1, x + w - 2 - depth // 2,
                         y + h - 2 - depth // 2], max(0, radius - 1), fill=face)


def draw(spec, lit=False, pressed=False):
    W, H = spec.get('size', [800, 600])
    th = spec.get('theme', {})
    bg = rgb(th.get('background', '#101828'))
    case = rgb(th.get('case', '#2b2b2f'))
    ink = rgb(th.get('ink', '#e8e8e8'))
    dim = rgb(th.get('dim', '#9a9aa2'))
    hint = rgb(th.get('hint', '#d8b040'))
    im = Image.new('RGB', (W, H), bg)
    d = ImageDraw.Draw(im)
    dyn = []
    for e in expand(spec):
        t = e['type']
        if t == 'case':
            x, y, w, h = e['rect']
            c = rgb(e.get('color', th.get('case', '#2b2b2f')))
            d.rectangle([x, y, x + w - 1, y + h - 1], fill=shade(c, 0.6))
            d.rectangle([x + 2, y + 2, x + w - 3, y + h - 3], fill=c)
            d.line([x + 2, y + 2, x + w - 3, y + 2], fill=shade(c, 1.4))
            for sx in e.get('screws', []):
                for sy in (y + 10, y + h - 11):
                    d.ellipse([sx - 5, sy - 5, sx + 5, sy + 5], fill=shade(c, 0.5))
                    d.ellipse([sx - 4, sy - 4, sx + 4, sy + 4], fill=shade(c, 1.6))
                    d.line([sx - 3, sy, sx + 3, sy], fill=shade(c, 0.5))
        elif t == 'area':
            x, y, w, h = e['rect']
            c = rgb(e['color'])
            d.rounded_rectangle([x, y, x + w - 1, y + h - 1], e.get('radius', 2), fill=c)
        elif t == 'line':
            d.line(e['xy'], fill=rgb(e.get('color', '#808080')), width=e.get('width', 1))
        elif t == 'box':
            x, y, w, h = e['rect']
            d.rounded_rectangle([x, y, x + w - 1, y + h - 1], e.get('radius', 3),
                                outline=rgb(e.get('color', '#808080')), width=e.get('width', 1))
            if 'title' in e:
                tw = d.textlength(e['title'], font=font(e.get('size', 10)))
                d.rectangle([x + w // 2 - tw / 2 - 4, y - 6, x + w // 2 + tw / 2 + 4, y + 6],
                            fill=rgb(e.get('bg', th.get('case', '#2b2b2f'))))
                text(d, (x + w // 2, y), e['title'], e.get('size', 10),
                     rgb(e.get('color', '#808080')))
        elif t == 'label':
            text(d, tuple(e['at']), e['text'], e.get('size', 10),
                 rgb(e['color']) if 'color' in e else ink, e.get('anchor', 'mm'),
                 e.get('weight', 'Medium'), e['fit'] * 1.1 if 'fit' in e else None)
        elif t == 'button':
            x, y, w, h = e['rect']
            face = rgb(e.get('color', th.get('button', '#5a5a60')))
            p = pressed and e.get('input') is not None
            bevel_rect(d, e['rect'], shade(face, 1.3) if p else face, e.get('radius', 3), 2, p)
            inside = th.get('hint_style') == 'inside' and 'key' in e
            if 'inner' in e:
                fx, fy, fw, fh = e['inner']['frac']
                d.rectangle([x + fx * w, y + fy * h, x + (fx + fw) * w - 1, y + (fy + fh) * h - 1],
                            fill=rgb(e['inner']['color']))
            lum = sum(rgb(e['inner']['color']) if 'inner' in e else face) / 3
            kcol = (70, 44, 0) if lum > 140 else hint
            corner = False
            if 'legend' in e:
                two = inside
                text(d, (x + w / 2, y + h * (0.34 if two else 0.5)), e['legend'],
                     max(8, int(h * (0.4 if two else 0.5))), rgb(e.get('lcolor', '#000000')),
                     fit=w * 0.85)
                if two:
                    text(d, (x + w / 2, y + h * 0.74), e['key'], max(8, int(h * 0.38)), kcol,
                         weight='Bold', fit=w * 0.85)
                    inside = False
                    e = dict(e, key=None)
            if 'glyph' in e:
                gx = x + w * (0.27 if inside else 0.5)
                text(d, (gx, y + h / 2), e['glyph'], e.get('gsize', max(9, h // 2)),
                     rgb(e.get('gcolor', th.get('glyph', '#f0f0f0'))))
            if inside:
                kx = x + w * (0.68 if 'glyph' in e else 0.5)
                text(d, (kx, y + h / 2 - 1), e['key'], max(9, h // 2), kcol, weight='Bold',
                     fit=w * (0.45 if 'glyph' in e else 0.9))
            if 'label' in e:
                pos = e.get('label_pos', 'above')
                ly = y - 8 if pos == 'above' else y + h + 8
                for i, ln in enumerate(e['label'].split('\n')):
                    off = (i - (e['label'].count('\n'))) * 11 if pos == 'above' else i * 11
                    text(d, (x + w / 2, ly + off), ln, e.get('size', 9), ink)
            if e.get('key') and not inside and not corner:
                kx, ky = x + w / 2, y + h + (20 if e.get('label_pos') == 'below' else 9)
                tw = d.textlength(e['key'], font=font(9, 'Bold'))
                d.rounded_rectangle([kx - tw / 2 - 3, ky - 6, kx + tw / 2 + 3, ky + 6], 2,
                                    outline=hint)
                text(d, (kx, ky), e['key'], 9, hint, weight='Bold')
            if e.get('input') is not None:
                dyn.append(('button', e['input'], e['rect']))
        elif t == 'led':
            cx, cy = e['at']
            r = e.get('r', 4)
            on = rgb(e.get('color', '#ff3020'))
            off = shade(on, 0.28)
            fill = on if lit else off
            if e.get('shape', 'round') == 'round':
                d.ellipse([cx - r - 1, cy - r - 1, cx + r + 1, cy + r + 1], fill=(20, 20, 20))
                d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=fill)
                if lit:
                    d.ellipse([cx - r // 2 - 1, cy - r // 2 - 1, cx - r // 2 + 1,
                               cy - r // 2 + 1], fill=shade(on, 1.6))
            else:
                w, h = e.get('size', [2 * r, r])
                d.rectangle([cx - w // 2 - 1, cy - h // 2 - 1, cx + w // 2, cy + h // 2],
                            fill=(20, 20, 20))
                d.rectangle([cx - w // 2, cy - h // 2, cx + w // 2 - 1, cy + h // 2 - 1],
                            fill=fill)
            if 'label' in e:
                lx, ly = e.get('label_at', [cx, cy + r + 9])
                text(d, (lx, ly), e['label'], e.get('lsize', 8), dim, e.get('anchor', 'mm'))
            dyn.append(('led', e.get('bit'), [cx - r, cy - r, 2 * r + 1, 2 * r + 1]))
        elif t == 'knob':
            cx, cy = e['at']
            r = e['r']
            d.ellipse([cx - r - 3, cy - r - 3, cx + r + 3, cy + r + 3], fill=(18, 18, 20))
            d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=rgb(e.get('color', '#3c3c40')))
            d.ellipse([cx - r + 4, cy - r + 4, cx + r - 4, cy + r - 4],
                      fill=shade(rgb(e.get('color', '#3c3c40')), 1.3))
            d.line([cx, cy - r + 4, cx, cy - r // 2], fill=(240, 240, 240), width=2)
            if 'label' in e:
                text(d, (cx, cy + r + 12), e['label'], e.get('size', 9), ink)
            if 'key' in e:
                text(d, (cx, cy + r + 26), e['key'], 9, hint, weight='Bold')
        elif t == 'slider':
            x, y, w, h = e['rect']
            d.rectangle([x + w // 2 - 2, y, x + w // 2 + 1, y + h], fill=(10, 10, 10))
            ky = y + int(h * (1 - e.get('pos', 0.7)))
            bevel_rect(d, [x, ky - 6, w, 12], rgb(e.get('color', '#c8c8c8')), 1, 2)
            if 'label' in e:
                text(d, (x + w / 2, y + h + 12), e['label'], e.get('size', 9), ink)
        elif t == 'lcd':
            x, y, w, h = e['rect']
            bz = e.get('bezel', 10)
            d.rounded_rectangle([x - bz, y - bz, x + w + bz - 1, y + h + bz - 1], 4,
                                fill=rgb(e.get('bezel_color', '#0c0c0e')))
            d.rectangle([x, y, x + w - 1, y + h - 1], fill=rgb(e.get('color', '#7a8a40')))
            if 'image' in e:
                bx, by, bw, bh = e['image']['box']
                src = Image.open(e['image']['file']).convert('RGB').crop((bx, by, bx + bw, by + bh))
                im.paste(src.resize((w, h), Image.LANCZOS), (x, y))
            else:
                text(d, (x + w / 2, y + h / 2), e.get('placeholder', 'LCD'), 14, (40, 40, 40))
            dyn.append(('lcd', None, e['rect']))
        elif t == 'strip':
            x, y = e['at']
            d.rectangle([x, y, x + 199, y + 19], fill=(0x20, 0xc0, 0x40))
            for i, c in enumerate([(255, 255, 255), (0x30, 0x38, 0x48), (0x30, 0x38, 0x48)]):
                d.rectangle([x + 220 + 60 * i, y, x + 259 + 60 * i, y + 19], fill=c)
            dyn.append(('strip', None, [x, y, 380, 20]))
        elif t == 'keys':
            x, y, w, h = e['rect']
            n = e['white']
            kw = w / n
            for i in range(n):
                d.rectangle([x + i * kw, y, x + (i + 1) * kw - 2, y + h], fill=(235, 235, 228))
            pat = e.get('black', [1, 1, 0, 1, 1, 1, 0])
            for i in range(n - 1):
                if pat[(i + e.get('start', 0)) % 7]:
                    bx = x + (i + 1) * kw - kw * 0.3
                    d.rectangle([bx, y, bx + kw * 0.6 - 1, y + h * 0.6], fill=(20, 20, 22))
        else:
            raise ValueError(f'unknown element type {t}')
    return im, dyn


def tile_stats(im):
    q = im.convert('RGB').quantize(16, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    W, H = q.size
    px = q.load()
    tiles = set()
    for ty in range(0, H, 8):
        for tx in range(0, W, 8):
            tiles.add(bytes(px[tx + i, ty + j] for j in range(8) for i in range(8)))
    return len(tiles), (W // 8) * (H // 8)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('panel')
    ap.add_argument('--png')
    ap.add_argument('--lit', action='store_true')
    ap.add_argument('--pressed', action='store_true')
    ap.add_argument('--stats', action='store_true')
    ap.add_argument('--lcd-image', nargs=5, metavar=('PNG', 'X', 'Y', 'W', 'H'),
                    help='mock-up only: paste this region of a screenshot into the LCD')
    a = ap.parse_args()
    spec = yaml.safe_load(open(a.panel))
    if a.lcd_image:
        for e in spec['elements']:
            if e['type'] == 'lcd':
                e['image'] = {'file': a.lcd_image[0], 'box': list(map(int, a.lcd_image[1:]))}
    im, dyn = draw(spec, a.lit, a.pressed)
    if a.png:
        im.save(a.png)
    if a.stats:
        static, _ = draw(spec)
        n, cells = tile_stats(static)
        tbits = n * 64 * 4
        mbits = cells * (10 + 6)
        print(f'{a.panel}: {n} unique tiles of {cells}; tile ROM {tbits} bits, map {mbits} bits, '
              f'~{-(-(tbits) // 10240) + -(-mbits // 10240)} M10K; '
              f'{sum(1 for k in dyn if k[0] == "led")} LEDs, '
              f'{sum(1 for k in dyn if k[0] == "button")} buttons')


if __name__ == '__main__':
    sys.exit(main())
