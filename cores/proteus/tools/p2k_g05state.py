#!/usr/bin/env python3
"""Rebuild the G05 register file (32 channels x 64 words) from a P2K_TRACE error.log and print the
EMU8000-style INIT arrays and the decoded reverb parameters.   p2k_g05state.py error.log"""
import re,sys
def state(path, tmax=25.5):
    regs=[[0]*64 for _ in range(32)]
    for l in open(path,errors='replace'):
        m=re.search(r'T ([\d.]+) ([0-9a-f]{8}) X (..) (..) = (....)',l)
        if m:
            if float(m.group(1))>tmax: break
            regs[int(m.group(3),16)][int(m.group(4),16)//2]=int(m.group(5),16)
    return regs
if __name__=='__main__':
    r=state(sys.argv[1])
    if '--regs' in sys.argv:
        print('\n'.join(' '.join(str(v) for v in row) for row in r)); sys.exit(0)
    I=lambda n,ch: r[ch][{1:0x28,2:0x2a,3:0x38,4:0x3a}[n]//2]
    for n in (1,2,3,4): print(f'INIT{n}', ' '.join('%04x'%I(n,c) for c in range(32)))
    print('out_mix',I(1,3)&0xff,'apfb',I(1,5)&0xff,'link',hex(I(1,7)),'damp',I(2,0x1d)&0xff,'refl_in',I(3,1)&0xff,'tail_ret',I(4,0x1f)&0xff)
    print('gains',[(I(*g)>>4)&15 for g in ((1,0xf),(1,0x17),(1,0x1f),(2,7),(2,0xf),(2,0x17))],'fbs',[I(*g)&15 for g in ((1,9),(1,0x11),(1,0x19),(2,1),(2,9),(2,0x11))])
    print('PTRX', '%04x %04x'%(r[0][0x14//2],r[0][0x16//2]), 'CSL', '%04x %04x'%(r[0][0x74//2],r[0][0x76//2]))
