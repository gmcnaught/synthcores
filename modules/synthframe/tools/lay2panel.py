#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Turn one view of a MAME layout (.lay) into panel_art.py elements.

  lay2panel.py LAYOUT.lay VIEW X Y W H > elements.yaml

The view's bounds are scaled into the rectangle (X, Y, W, H) of the output, keeping the aspect
ratio. Buttons keep MAME's inputtag/inputmask, LEDs their output name, so the core's panel.yaml
can map them to its key and LED bits. Buttons are `button*` elements or, with an inputtag, any
element drawn as disks (MU50 `round_button`); a `led*` element of disks is a round LED, and
with an inputtag a lit button. An element drawn by an SVG image becomes a `svg` element;
elements blended onto the screen (contrast, dot mask) are left to the LCD renderer.
"""
import re
import sys
import xml.etree.ElementTree as ET

import yaml


def bounds(b):
    a = {k: float(v) for k, v in b.attrib.items()}

    def axis(lo, hi, pos, c, size):
        if lo in a:
            return a[lo], a[hi] - a[lo]
        s = a.get(size, 1.0)
        return (a[c] - s / 2 if c in a else a.get(pos, 0.0)), s

    x, w = axis('left', 'right', 'x', 'xc', 'width')
    y, h = axis('top', 'bottom', 'y', 'yc', 'height')
    return x, y, w, h


def colour(node, default='#808080'):
    c = node.find('color') if node is not None else None
    if c is None:
        return default
    return '#' + ''.join(f'{round(float(c.get(k, 1)) * 255):02x}' for k in ('red', 'green', 'blue'))


def expand_repeats(parent):
    """Expand <repeat> blocks of element definitions (params used as ~name~)."""
    out = []
    for ch in list(parent):
        if ch.tag != 'repeat':
            out.append(ch)
            continue
        n = int(ch.get('count'))
        params = [(p.get('name'), int(p.get('start')), int(p.get('increment', '1')))
                  for p in ch.findall('param')]
        body = [c for c in ch if c.tag != 'param']
        for i in range(n):
            for c in body:
                s = ET.tostring(c, encoding='unicode')
                for name, start, inc in params:
                    s = s.replace(f'~{name}~', str(start + i * inc))
                out.append(ET.fromstring(s))
    return out


def convert(lay, view_name, X, Y, W, H):
    root = ET.parse(lay).getroot()
    top = expand_repeats(root)
    elems = {e.get('name'): e for e in top if e.tag == 'element'}
    groups = {g.get('name'): g for g in top if g.tag == 'group'}

    def natural(g):
        xs = []
        for ch in g:
            b = ch.find('bounds')
            if b is not None:
                x, y, w, h = bounds(b)
                xs.append((x, y, x + w, y + h))
        return (min(a[0] for a in xs), min(a[1] for a in xs),
                max(a[2] for a in xs) - min(a[0] for a in xs),
                max(a[3] for a in xs) - min(a[1] for a in xs))

    out = []

    def walk(node, tf):
        sx, sy, ox, oy = tf
        for ch in node:
            b = ch.find('bounds')
            if b is None:
                continue
            x, y, w, h = bounds(b)
            r = [round(ox + x * sx), round(oy + y * sy), max(1, round(w * sx)),
                 max(1, round(h * sy))]
            if ch.tag == 'group':
                g = groups[ch.get('ref')]
                nx, ny, nw, nh = natural(g)
                walk(g, (sx * w / nw, sy * h / nh, ox + (x - nx * w / nw) * sx,
                         oy + (y - ny * h / nh) * sy))
            elif ch.tag == 'screen':
                out.append({'type': 'lcd', 'rect': r})
            elif ch.tag == 'element':
                if ch.get('blend'):     # LCD overlays (contrast, dot mask): the LCD renderer's
                    continue
                ref = ch.get('ref')
                e = elems[ref]
                img = e.find('image')
                if (img is not None and img.find('data') is not None and not ch.get('inputtag')
                        and not ref.startswith('button')):
                    out.append({'type': 'svg', 'rect': r, 'svg': img.find('data').text.strip()})
                    continue
                disks = {c.get('state'): c for c in e.findall('disk')}
                if not ref.startswith(('button', 'led')) and ch.get('inputtag') and '0' in disks:
                    out.append({'type': 'button', 'rect': r, 'radius': min(r[2], r[3]) // 2,
                                'color': colour(disks['0']), 'tag': ch.get('inputtag'),
                                'mask': ch.get('inputmask')})
                elif ref.startswith('button'):
                    st0 = [c for c in e.findall('rect') if c.get('state') == '0']
                    face = colour(st0[0]) if st0 else '#303030'
                    inner = [c for c in e.findall('rect') if c.get('state') == '0'][1:]
                    el = {'type': 'button', 'rect': r, 'color': face,
                          'tag': ch.get('inputtag'), 'mask': ch.get('inputmask')}
                    if inner:
                        _, _, iw, ih = bounds(inner[0].find('bounds'))
                        _, _, bw, bh = bounds(e.find('rect').find('bounds'))
                        ib = bounds(inner[0].find('bounds'))
                        el['inner'] = {'color': colour(inner[0]),
                                       'frac': [ib[0] / bw, ib[1] / bh, iw / bw, ih / bh]}
                    lt = e.find('text')
                    if lt is not None:
                        el['legend'] = lt.get('string')
                        el['lcolor'] = colour(lt, '#ffffff')
                    svg = ''.join(e.itertext())
                    m = re.search(r'points="([^"]+)"', svg)
                    if m:
                        pts = [tuple(map(float, p.split(','))) for p in m.group(1).split()]
                        ys = [p[1] for p in pts]
                        xs_ = [p[0] for p in pts]
                        el['glyph'] = ('▼' if ys.count(2) == 2 else '▲' if ys.count(8) == 2
                                       else '◀' if xs_.count(8) == 2 else '▶')
                    out.append(el)
                elif ref.startswith('led') and '1' in disks:
                    # a round LED; with inputtag it is also a button (MU50: lit PLAY, EDIT...)
                    el = {'type': 'led', 'shape': 'round',
                          'at': [r[0] + r[2] // 2, r[1] + r[3] // 2], 'r': min(r[2], r[3]) // 2,
                          'color': colour(disks['1']), 'name': ch.get('name')}
                    if '0' in disks:
                        el['off'] = colour(disks['0'])
                    if ch.get('inputtag'):
                        el['tag'], el['mask'] = ch.get('inputtag'), ch.get('inputmask')
                    out.append(el)
                elif ref.startswith('led'):
                    on = [c for c in e.findall('rect') if c.get('state') == '1']
                    out.append({'type': 'led', 'shape': 'rect',
                                'at': [r[0] + r[2] // 2, r[1] + r[3] // 2],
                                'size': [r[2], r[3]], 'r': max(r[2], r[3]) // 2,
                                'color': colour(on[0]) if on else '#ff2000',
                                'name': ch.get('name')})
                elif ref in ('whitekey', 'blackkey', 'keyfill'):
                    out.append({'type': 'area', 'rect': r, 'radius': 0,
                                'color': {'whitekey': '#ebebe4', 'blackkey': '#18181a',
                                          'keyfill': '#000000'}[ref]})
                else:
                    t = e.find('text')
                    rc = e.find('rect')
                    if rc is not None:
                        out.append({'type': 'area', 'rect': r, 'radius': 0,
                                    'color': colour(rc)})
                    if t is not None:
                        al = t.get('align', '0')
                        anchor = {'0': 'mm', '1': 'lm', '2': 'rm'}[al]
                        ax = r[0] + (r[2] // 2 if al == '0' else 0 if al == '1' else r[2])
                        out.append({'type': 'label', 'text': t.get('string'),
                                    'at': [ax, r[1] + r[3] // 2],
                                    'size': max(7, round(r[3] * 0.95)), 'anchor': anchor,
                                    'fit': r[2],
                                    'color': colour(t, '#ffffff')})

    view = next(v for v in top if v.tag == 'view' and v.get('name') == view_name)
    vx, vy, vw, vh = bounds(view.find('bounds'))
    s = min(W / vw, H / vh)
    walk(view, (s, s, X + (W - vw * s) / 2 - vx * s, Y + (H - vh * s) / 2 - vy * s))
    return out


def main():
    out = convert(sys.argv[1], sys.argv[2], *map(float, sys.argv[3:7]))
    yaml.safe_dump(out, sys.stdout, default_flow_style=None, sort_keys=False, width=200,
                   allow_unicode=True)


if __name__ == '__main__':
    main()
