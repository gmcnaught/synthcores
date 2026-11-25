#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Draw a core's front panel from cores/<core>/cfg/panel.yaml, and the ROMs that show it.

The static art (case, labels, button caps, unlit LEDs, key hints) becomes 8 x 8 tiles in one
palette of 2^bpp colours, for modules/synthframe/hdl/panel/synthframe_panel_art.sv. LEDs and
buttons with an HDL expression (`bit:` / `input:`) are dynamic: their unlit pixels use reserved
palette entries, and a second palette holds the lit (or pressed) colours, chosen per 8 x 8 cell by
the element that owns the cell. The character LCD and the status strip keep their own
renderers, which draw over the art.

  panel_art.py PANEL.yaml --png OUT.png [--lit] [--pressed]   picture (mock-up)
  panel_art.py PANEL.yaml --stats                              tile and block RAM counts
  panel_art.py PANEL.yaml --hdl DIR --name NAME                NAME_panel_map.hex, NAME_panel_tiles.hex,
                                                               NAME_panel.svh in DIR
"""
import os
import re
import argparse
import sys

import numpy as np
import yaml
from PIL import Image, ImageDraw, ImageFont

FONT_DIRS = [os.path.join(os.path.dirname(os.path.abspath(__file__)), 'fonts')]
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
            raise OSError(f'Inter-{weight}.ttf not found in {FONT_DIRS}')
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


def expand(spec, base='.'):
    """Replace {type: lay} entries with the MAME layout's elements, with key hints and the HDL
    expressions of their buttons (key_expr) and LEDs (led_map)."""
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
        for el in convert(os.path.join(base, e['file']), e['view'], *e['rect']):
            if el['type'] == 'lcd' and e.get('lcd') == 'window':
                el = {'type': 'area', 'rect': el['rect'], 'color': '#0a0a0a', 'radius': 0}
            if el['type'] == 'button' and el.get('tag'):
                k = norm(f"{el['tag']}:{el['mask']}")
                if k in keys:
                    el['key'] = keys[k]
                m = re.fullmatch(r'kc(\d+)', el['tag'])
                if m and 'key_expr' in spec:
                    el['input'] = spec['key_expr'].format(tag=int(m.group(1)),
                                                          bit=int(el['mask'], 0).bit_length() - 1)
            if el['type'] == 'led':
                for pat, tmpl in spec.get('led_map', []):
                    m = re.fullmatch(pat, el.get('name') or '')
                    if m:
                        el['bit'] = tmpl.format(*m.groups())
                        break
            out.append(el)
    return out


def pressed_face(face):
    """A pressed button's face: lighter, and never equal to the face (dark faces too)."""
    return tuple(min(255, int(v * 1.25) + 40) for v in face)


def bevel_rect(d, r, face, radius=3, depth=2, pressed=False):
    x, y, w, h = r
    hi, lo = shade(face, 1.35), shade(face, 0.55)
    if pressed:
        hi, lo = lo, hi
    d.rounded_rectangle([x, y, x + w - 1, y + h - 1], radius, fill=lo)
    d.rounded_rectangle([x, y, x + w - 1 - depth, y + h - 1 - depth], radius, fill=hi)
    d.rounded_rectangle([x + depth // 2 + 1, y + depth // 2 + 1, x + w - 2 - depth // 2,
                         y + h - 2 - depth // 2], max(0, radius - 1), fill=face)


def draw(spec, lit=False, pressed=False, hdl=False, base='.'):
    """hdl: the art the ROMs hold (no LCD contents, no status strip, no LED highlight)."""
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
    for e in expand(spec, base):
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
            bevel_rect(d, e['rect'], face, e.get('radius', 3), 2)
            if p:   # pressed: the face lights (the HDL recolours the face pixels only)
                x0, y0 = x + 2, y + 2
                d.rounded_rectangle([x0, y0, x + w - 4, y + h - 4], max(0, e.get('radius', 3) - 1),
                                    fill=pressed_face(face))
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
                dyn.append(('button', e['input'], [x, y, w, h], face, pressed_face(face)))
        elif t == 'led':
            cx, cy = e['at']
            r = e.get('r', 4)
            on = rgb(e.get('color', '#ff3020'))
            off = shade(on, 0.28)
            fill = on if lit and (e.get('bit') is not None or not hdl) else off
            if e.get('shape', 'round') == 'round':
                d.ellipse([cx - r - 1, cy - r - 1, cx + r + 1, cy + r + 1], fill=(20, 20, 20))
                d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=fill)
                if lit and not hdl:
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
            if e.get('bit') is not None:
                rr = max(r, *e.get('size', [0, 0])) + 1
                dyn.append(('led', e['bit'], [cx - rr, cy - rr, 2 * rr + 1, 2 * rr + 1], off, on))
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
            if hdl:
                pass
            elif 'image' in e:
                bx, by, bw, bh = e['image']['box']
                src = Image.open(e['image']['file']).convert('RGB').crop((bx, by, bx + bw, by + bh))
                im.paste(src.resize((w, h), Image.LANCZOS), (x, y))
            else:
                text(d, (x + w / 2, y + h / 2), e.get('placeholder', 'LCD'), 14, (40, 40, 40))
        elif t == 'strip':
            if hdl:
                continue
            x, y = e['at']
            d.rectangle([x, y, x + 199, y + 19], fill=(0x20, 0xc0, 0x40))
            for i, c in enumerate([(255, 255, 255), (0x30, 0x38, 0x48), (0x30, 0x38, 0x48)]):
                d.rectangle([x + 220 + 60 * i, y, x + 259 + 60 * i, y + 19], fill=c)
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


def build_rom(spec, bpp, base='.'):
    """Tile map, tiles, palettes and element list for synthframe_panel_art."""
    im, dyn = draw(spec, hdl=True, base=base)
    W, H = im.size
    a = np.asarray(im, dtype=np.int32)
    cols, rows = W // 8, H // 8
    lit_of = {}
    for k in dyn:
        u, l = tuple(k[3]), tuple(k[4])
        if lit_of.setdefault(u, l) != l:
            raise ValueError(f'unlit colour {u} lights to two colours')
    reserved = list(lit_of)
    nstat = 2 ** bpp - len(reserved)
    if nstat < 4:
        raise ValueError(f'{len(reserved)} dynamic colours leave {nstat} for the art')
    # each dynamic pixel belongs to the first element whose rectangle holds it and whose unlit
    # colour it has
    owner = np.zeros((H, W), np.int32)
    for i, (kind, expr, (x, y, w, h), u, l) in enumerate(dyn, 1):
        sub = a[y:y + h, x:x + w]
        o = owner[y:y + h, x:x + w]
        o[np.all(sub == u, axis=2) & (o == 0)] = i
    # each cell belongs to one element: LEDs before buttons, then the most pixels
    cell = np.zeros((rows, cols), np.int32)
    shared = 0
    for cy in range(rows):
        for cx in range(cols):
            o = owner[cy * 8:cy * 8 + 8, cx * 8:cx * 8 + 8]
            ids, cnt = np.unique(o[o > 0], return_counts=True)
            if len(ids):
                best = max(zip(ids, cnt), key=lambda t: (dyn[t[0] - 1][0] == 'led', t[1]))
                cell[cy, cx] = best[0]
                shared += len(ids) > 1
    mine = (owner > 0) & (owner == np.kron(cell, np.ones((8, 8), np.int32)))
    q = im.quantize(nstat, method=Image.Quantize.MEDIANCUT, kmeans=4, dither=Image.Dither.NONE)
    idx = np.asarray(q, dtype=np.int32).copy()
    qp = q.getpalette()[:3 * nstat]
    pal = [tuple(qp[3 * i:3 * i + 3]) if 3 * i + 2 < len(qp) else (0, 0, 0) for i in range(nstat)]
    for r, u in enumerate(reserved):
        sel = mine & np.all(a == u, axis=2)
        idx[sel] = nstat + r
    pal += reserved
    lit = pal[:nstat] + [lit_of[u] for u in reserved]
    tiles, tmap = {}, []
    for cy in range(rows):
        for cx in range(cols):
            t = idx[cy * 8:cy * 8 + 8, cx * 8:cx * 8 + 8].astype(np.uint8).tobytes()
            tmap.append((tiles.setdefault(t, len(tiles)), int(cell[cy, cx])))
    out = {'W': W, 'H': H, 'cols': cols, 'rows': rows, 'bpp': bpp, 'pal': pal, 'lit': lit,
           'tiles': list(tiles), 'map': tmap, 'dyn': dyn, 'shared': shared,
           'tb': max(1, (len(tiles) - 1).bit_length()), 'eb': max(1, len(dyn).bit_length())}
    out['image'] = Image.fromarray(np.array(pal, np.uint8)[idx], 'RGB')
    return out


def rom_bits(r):
    m10k = lambda depth, width: -(-depth * width // 10240)
    mb = len(r['map']) * (r['tb'] + r['eb'])
    tb = len(r['tiles']) * 8 * 8 * r['bpp']
    return mb, tb, m10k(len(r['map']), r['tb'] + r['eb']) + m10k(len(r['tiles']) * 8, 8 * r['bpp'])


def emit_hdl(r, outdir, name, src):
    os.makedirs(outdir, exist_ok=True)
    tb, eb, bpp = r['tb'], r['eb'], r['bpp']
    with open(f'{outdir}/{name}_panel_map.hex', 'w') as f:
        for t, e in r['map']:
            f.write(f'{(e << tb) | t:0{-(-(tb + eb) // 4)}x}\n')
    with open(f'{outdir}/{name}_panel_tiles.hex', 'w') as f:
        for t in r['tiles']:
            for row in range(8):
                v = 0
                for x in range(8):
                    v |= t[row * 8 + x] << (x * bpp)
                f.write(f'{v:0{2 * bpp}x}\n')
    n = 2 ** bpp

    def packed(p):
        return ',\n        '.join(f"24'h{c[0]:02x}{c[1]:02x}{c[2]:02x}" for c in reversed(p))
    with open(f'{outdir}/{name}_panel.svh', 'w') as f:
        f.write(f"""// Generated by modules/synthframe/tools/panel_art.py from {src}. Do not edit.
// {r['W']} x {r['H']}: {r['cols']} x {r['rows']} cells, {len(r['tiles'])} tiles of {bpp} bits a pixel,
// {len(r['dyn'])} dynamic elements ({r['shared']} cells shared by two elements).
localparam int PANEL_W = {r['W']}, PANEL_H = {r['H']}, PANEL_COLS = {r['cols']}, PANEL_ROWS = {r['rows']};
localparam int PANEL_NT = {len(r['tiles'])}, PANEL_TB = {tb}, PANEL_EB = {eb}, PANEL_BPP = {bpp};
localparam int PANEL_NE = {len(r['dyn'])};
// palette entry 0 in the low bits; PANEL_LIT differs only in the dynamic entries
localparam logic [24*{n}-1:0] PANEL_PAL = {{
        {packed(r['pal'])}}};
localparam logic [24*{n}-1:0] PANEL_LIT = {{
        {packed(r['lit'])}}};
""")
    with open(f'{outdir}/{name}_panel_state.svh', 'w') as f:
        f.write(f'// Generated by modules/synthframe/tools/panel_art.py from {src}. Do not edit.\n'
                f'// Element i lights (LED) or shows pressed (button) while its expression is 1.\n')
        for i, k in enumerate(r['dyn'], 1):
            f.write(f'assign panel_state[{i}] = {k[1]};   // {k[0]} at {k[2][0]},{k[2][1]}\n')
        f.write(f"assign panel_state[0] = 1'b0;\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('panel')
    ap.add_argument('--png')
    ap.add_argument('--lit', action='store_true')
    ap.add_argument('--pressed', action='store_true')
    ap.add_argument('--stats', action='store_true')
    ap.add_argument('--hdl', metavar='DIR')
    ap.add_argument('--name')
    ap.add_argument('--bpp', type=int)
    ap.add_argument('--lcd-image', nargs=5, metavar=('PNG', 'X', 'Y', 'W', 'H'),
                    help='mock-up only: paste this region of a screenshot into the LCD')
    a = ap.parse_args()
    spec = yaml.safe_load(open(a.panel))
    if a.lcd_image:
        for e in spec['elements']:
            if e['type'] == 'lcd':
                e['image'] = {'file': a.lcd_image[0], 'box': list(map(int, a.lcd_image[1:]))}
    im, dyn = draw(spec, a.lit, a.pressed, base=os.path.dirname(os.path.abspath(a.panel)))
    if a.png:
        im.save(a.png)
    base = os.path.dirname(os.path.abspath(a.panel))
    if a.stats or a.hdl:
        r = build_rom(spec, a.bpp or spec.get('bpp', 4), base)
        mb, tb, m = rom_bits(r)
        print(f'{a.panel}: {len(r["tiles"])} tiles, {r["bpp"]} bpp, {len(r["dyn"])} dynamic '
              f'elements, {r["shared"]} shared cells; map {mb} + tiles {tb} bits, ~{m} M10K')
        if a.hdl:
            emit_hdl(r, a.hdl, a.name, os.path.relpath(os.path.abspath(a.panel),
                                                        os.path.join(base, '../../..')))
            if a.png:
                r['image'].save(a.png.replace('.png', '_rom.png'))


if __name__ == '__main__':
    sys.exit(main())
