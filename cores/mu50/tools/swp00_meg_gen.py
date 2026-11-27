#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Compile MAME's SWP00 effects code (the MEG) into the program of hdl/swp00/swp00_meg.sv.

Usage: swp00_meg_gen.py MAME_DIR OUT_DIR

Reads src/devices/sound/swp00.cpp, takes sound_stream_update() from "// Variation block" to
"// 18-bits ADCs" (the code after the voice loop) and compiles it, statement by statement, into
micro-ops of the MEG executor. Writes OUT_DIR/swp00_meg_prog.svh (the program ROM contents and
the variant start addresses) and OUT_DIR/swp00_meg_prog.txt (a listing with the C it came from).

The code is straight-line C over s32 values, with MAME's helper calls (m7, m9, m9v, lfo_step,
filtered_lfo_step, alfo, the delay_block reads and writes ...). Every helper becomes one op, or a
few for filtered_lfo_step; '+', '-' and '& 0x7fffff' become ALU ops. The executor runs ops in
program order, so C's sequential semantics (a member read after it is assigned sees the new
value) hold by construction. The four `switch(m_meg_control & 0xc0)` variants become four
programs: common head, the variant, common tail. All values are 32-bit two's complement, as the
s32/u32 arithmetic of the C code (u16 m_const[] values zero-extended).

Anything outside this subset stops the generator.
"""
import re
import sys
from pathlib import Path

# ---------------------------------------------------------------------------------------------- ops
OPS = ["END", "MUL7", "MUL9", "MUL9V", "ADD", "SUB", "AND23", "CONST", "SAT", "LDI", "LDMIX", "OUT",
       "LFOSTEP", "LFOWRAPR", "LFOSCALE", "LFOMOD", "WRAPI", "SUBSAT", "FMIX", "ALFO",
       "RD", "WR", "RLFO", "RLFO2"]
OPC = {n: i for i, n in enumerate(OPS)}
BUF = {"brev": 0, "bcho": 1, "bvar": 2}
MIX = ["dry_l", "dry_r", "rev", "cho_l", "cho_r", "var_l", "var_r"]
R_ZERO = 0          # register 0 holds 0
VIRT = 1 << 20      # virtual temporaries are numbered from here
MEM_OPS = {"RD", "WR", "RLFO", "RLFO2"}
NO_WRITE = {"WR", "OUT", "END"}


def allocate_temps(ops, named):
    """Map the virtual temporaries of a program to the registers no C variable uses, least
    recently freed first, so a register is reused as late as possible (fewer false dependencies
    for schedule())."""
    last = {}
    for i, op in enumerate(ops):
        for r in (op[1], op[2], op[3]):
            if r >= VIRT:
                last[r] = i
    free = [r for r in range(1, 256) if r not in named]
    m = {}
    out = []
    for i, op in enumerate(ops):
        f = list(op)
        for k in (2, 3):                         # reads
            if f[k] >= VIRT:
                f[k] = m[f[k]]
        if f[1] >= VIRT:                         # write
            if not free:
                raise GenError("out of registers")
            m[op[1]] = free.pop(0)
            f[1] = m[op[1]]
        for r in {op[2], op[3]}:
            if r >= VIRT and last[r] == i:
                free.append(m[r])
        if op[1] >= VIRT and last.get(op[1], -1) <= i:
            free.append(m[op[1]])                # written, never read
        out.append(tuple(f))
    return out


def schedule(ops):
    """Reorder a program so that an op rarely reads the register the op just before it writes
    (the executor waits a clock then). Keeps every register dependency (read after write, write
    after read, write after write), the order of the delay line accesses, and END last."""
    n = len(ops)
    preds = [set() for _ in range(n)]
    last_w, reads_since = {}, {}
    last_mem = None
    for j, op in enumerate(ops):
        name = OPS[op[0]]
        rd = {r for r in (op[2], op[3]) if r != R_ZERO}
        wr = {op[1]} if name not in NO_WRITE and op[1] != R_ZERO else set()
        for r in rd:
            if r in last_w:
                preds[j].add(last_w[r])
        for r in wr:
            if r in last_w:
                preds[j].add(last_w[r])
            preds[j] |= reads_since.get(r, set())
        if name in MEM_OPS:
            if last_mem is not None:
                preds[j].add(last_mem)
            last_mem = j
        if name == "END":
            preds[j] |= set(range(j))
        for r in rd:
            reads_since.setdefault(r, set()).add(j)
        for r in wr:
            last_w[r] = j
            reads_since[r] = set()
    done, order = set(), []
    left = [len(p) for p in preds]
    succ = [[] for _ in range(n)]
    for j, p in enumerate(preds):
        for i in p:
            succ[i].append(j)
    ready = sorted(j for j in range(n) if left[j] == 0)
    prev_w = None
    while ready:
        pick = None
        for j in ready:
            op = ops[j]
            if prev_w is None or prev_w not in (op[2], op[3]):
                pick = j
                break
        if pick is None:
            pick = ready[0]
        ready.remove(pick)
        order.append(pick)
        done.add(pick)
        op = ops[pick]
        prev_w = op[1] if OPS[op[0]] not in NO_WRITE and op[1] != R_ZERO else None
        for k in succ[pick]:
            left[k] -= 1
            if left[k] == 0:
                ready.append(k)
        ready.sort()
    if len(order) != n:
        raise GenError("schedule: dependency cycle")
    return [ops[j] for j in order]


def stalls(ops):
    """Clocks the executor waits for a result (an op reading the previous op's register)."""
    s = 0
    for p, q in zip(ops, ops[1:]):
        w = p[1] if OPS[p[0]] not in NO_WRITE and p[1] != R_ZERO else None
        if w is not None and w in (q[2], q[3]):
            s += 1
    return s


class GenError(Exception):
    pass


# ---------------------------------------------------------------------------------------------- parse
TOK = re.compile(r"\s*(?:(0[xX][0-9a-fA-F]+|\d+)|([A-Za-z_]\w*)|(\+\+|--|\+=|-=|[-+&*(){}\[\],;.=:]))")


def tokenize(s):
    s = re.sub(r"//[^\n]*", "", s)
    out, i = [], 0
    while i < len(s):
        if s[i].isspace():
            i += 1
            continue
        m = TOK.match(s, i)
        if not m:
            raise GenError("bad text %r" % s[i:i + 30])
        out.append(m.group(m.lastindex))
        i = m.end()
    return out


class P:
    def __init__(self, toks):
        self.t, self.i = toks, 0

    def peek(self, k=0):
        return self.t[self.i + k] if self.i + k < len(self.t) else None

    def take(self, v=None):
        x = self.t[self.i]
        if v is not None and x != v:
            raise GenError("expected %s got %s near %s" % (v, x, " ".join(self.t[self.i - 5:self.i + 5])))
        self.i += 1
        return x

    def stmts(self, end=None):
        out = []
        while self.i < len(self.t) and self.peek() != end and self.peek() not in ("case", "break"):
            out.append(self.stmt())
        return out

    def stmt(self):
        p = self.peek()
        if p == "{":
            self.take()
            s = []
            while self.peek() != "}":
                if self.peek() == "break":      # a case's { ... break; }
                    self.take()
                    self.take(";")
                    continue
                s += self.stmts("}")
            self.take("}")
            return ("block", s)
        if p == "switch":
            self.take()
            self.take("(")
            sel = self.expr()
            self.take(")")
            self.take("{")
            cases = {}
            while self.peek() == "case":
                self.take()
                v = int(self.take(), 0)
                self.take(":")
                body = self.stmts("}")
                if self.peek() == "break":
                    self.take()
                    self.take(";")
                cases[v] = body
                # a case body may be a { ... break; } block
                if len(body) == 1 and body[0][0] == "block":
                    cases[v] = body[0][1]
                while self.peek() == "break":
                    self.take()
                    self.take(";")
            self.take("}")
            return ("switch", sel, cases)
        if p in ("s32", "u32"):
            self.take()
            decls = []
            while True:
                n = self.take()
                init = None
                if self.peek() == "=":
                    self.take()
                    init = self.expr()
                decls.append((n, init))
                if self.peek() == ",":
                    self.take()
                    continue
                break
            self.take(";")
            return ("decl", decls)
        e = self.expr()
        if self.peek() in ("=", "+=", "-="):
            op = self.take()
            r = self.expr()
            self.take(";")
            return ("assign", op, e, r)
        if self.peek() in ("++", "--"):
            op = self.take()
            self.take(";")
            return ("incdec", op, e)
        self.take(";")
        return ("expr", e)

    # expr: additive with & at the lowest level used here
    def expr(self):
        e = self.add()
        while self.peek() == "&":
            self.take()
            e = ("&", e, self.add())
        return e

    def add(self):
        e = self.unary()
        while self.peek() in ("+", "-"):
            op = self.take()
            e = (op, e, self.unary())
        return e

    def unary(self):
        if self.peek() == "-":
            self.take()
            return ("neg", self.unary())
        return self.primary()

    def primary(self):
        x = self.take()
        if x == "(":
            e = self.expr()
            self.take(")")
            return e
        if re.match(r"^(0[xX][0-9a-fA-F]+|\d+)$", x):
            return ("num", int(x, 0))
        if self.peek() == ".":
            self.take()
            meth = self.take()
            return ("call", x + "." + meth, self.args())
        if self.peek() == "(":
            return ("call", x, self.args())
        if self.peek() == "[":
            self.take()
            idx = self.expr()
            self.take("]")
            return ("index", x, idx)
        return ("var", x)

    def args(self):
        self.take("(")
        out = []
        if self.peek() != ")":
            while True:
                out.append(self.expr())
                if self.peek() == ",":
                    self.take()
                    continue
                break
        self.take(")")
        return out


# ---------------------------------------------------------------------------------------------- compile
class Prog:
    def __init__(self, regs):
        self.ops = []           # (op, d, a, b, c, c2, c3, x, comment)
        self.regs = regs        # shared register map name -> index

    def emit(self, op, d=0, a=0, b=0, c=0, c2=0, c3=0, x=0, cm=""):
        self.ops.append((op, d, a, b, c, c2, c3, x, cm))


class Compiler:
    def __init__(self):
        self.regs = {"__zero": R_ZERO}
        self.nreg = 1
        self.nvirt = 0
        self.consts = {}        # immediate value -> register (loaded by LDI at the program start)

    def reg(self, name):
        if name not in self.regs:
            self.regs[name] = self.nreg
            self.nreg += 1
        return self.regs[name]

    def temp(self):
        # virtual temporaries, one per value; allocate_temps() maps them to registers
        self.nvirt += 1
        return VIRT + self.nvirt

    def free(self, r):
        self.temps_owned.discard(r)

    def imm(self, v):
        if v == 0:
            return R_ZERO
        if v not in self.consts:
            self.consts[v] = self.reg("__imm_%x" % v)
        return self.consts[v]

    def const_arg(self, e):
        if e[0] != "num":
            raise GenError("register argument must be a number: %r" % (e,))
        return e[1]

    # value of an expression in a register; dst: preferred destination
    def ev(self, p, e, dst=None):
        k = e[0]
        if k == "num":
            return self.imm(e[1])
        if k == "var":
            n = e[1]
            if n in self.locals or n.startswith("m_") or n in MIX:
                return self.reg(n)
            raise GenError("unknown variable %s" % n)
        if k == "index":
            if e[1] != "m_const":
                raise GenError("index of %s" % e[1])
            d = dst if dst is not None else self.newtemp()
            p.emit(OPC["CONST"], d=d, c=self.const_arg(e[2]), cm="m_const[%x]" % self.const_arg(e[2]))
            return d
        if k in ("+", "-"):
            a = self.ev(p, e[1])
            b = self.ev(p, e[2])
            d = dst if dst is not None else self.newtemp()
            p.emit(OPC["ADD" if k == "+" else "SUB"], d=d, a=a, b=b)
            self.free(a)
            self.free(b)
            return d
        if k == "neg":
            a = self.ev(p, e[1])
            d = dst if dst is not None else self.newtemp()
            p.emit(OPC["SUB"], d=d, a=R_ZERO, b=a)
            self.free(a)
            return d
        if k == "&":
            if e[2] != ("num", 0x7fffff):
                raise GenError("only & 0x7fffff is supported")
            a = self.ev(p, e[1])
            d = dst if dst is not None else self.newtemp()
            p.emit(OPC["AND23"], d=d, a=a)
            self.free(a)
            return d
        if k == "call":
            return self.call(p, e[1], e[2], dst)
        raise GenError("expression %r" % (e,))

    def newtemp(self):
        r = self.temp()
        self.temps_owned.add(r)
        return r

    def call(self, p, f, args, dst):
        d = dst if dst is not None else self.newtemp()

        def val(i):
            return self.ev(p, args[i])

        def cst(i):
            return self.const_arg(args[i])

        if f in ("m7", "m9", "lfo_wrap_reg", "lfo_scale", "lfo_mod"):
            a = val(0)
            op = {"m7": "MUL7", "m9": "MUL9", "lfo_wrap_reg": "LFOWRAPR", "lfo_scale": "LFOSCALE",
                  "lfo_mod": "LFOMOD"}[f]
            p.emit(OPC[op], d=d, a=a, c=cst(1), cm="%s(.., %x)" % (f, cst(1)))
            self.free(a)
        elif f == "m9v":
            a, b = val(0), val(1)
            p.emit(OPC["MUL9V"], d=d, a=a, b=b, cm="m9v")
            self.free(a)
            self.free(b)
        elif f == "saturate":
            a = val(0)
            p.emit(OPC["SAT"], d=d, a=a, cm="saturate")
            self.free(a)
        elif f == "alfo":
            # alfo(phase, delta_phase, levelreg, offsetreg, sub)
            a, b = val(0), val(1)
            sub = args[4]
            if sub not in (("var", "true"), ("var", "false")):
                raise GenError("alfo sub must be true/false")
            p.emit(OPC["ALFO"], d=d, a=a, b=b, c=cst(2), c2=cst(3), x=1 if sub == ("var", "true") else 0,
                   cm="alfo(.., %x, %x, %s)" % (cst(2), cst(3), sub[1]))
            self.free(a)
            self.free(b)
        elif re.match(r"^b(rev|cho|var)\.(r|rlfo|rlfo2)$", f):
            buf, meth = f.split(".")
            if meth == "r":
                p.emit(OPC["RD"], d=d, c=cst(0) // 3, c2=BUF[buf], cm="%s.r(%x)" % (buf, cst(0)))
            elif meth == "rlfo":
                a, b = val(1), val(2)
                p.emit(OPC["RLFO"], d=d, a=a, b=b, c=cst(0) // 3, c2=BUF[buf], c3=cst(3),
                       cm="%s.rlfo(%x, .., %x)" % (buf, cst(0), cst(3)))
                self.free(a)
                self.free(b)
            else:
                a = val(1)
                p.emit(OPC["RLFO2"], d=d, a=a, c=cst(0) // 3, c2=BUF[buf], cm="%s.rlfo2(%x)" % (buf, cst(0)))
                self.free(a)
        else:
            raise GenError("call %s" % f)
        return d

    def stmt(self, p, s):
        self.temps_owned = set()
        k = s[0]
        if k == "decl":
            for n, init in s[1]:
                self.locals.add(n)
                r = self.reg(n)
                if init is None:
                    raise GenError("declaration without a value: %s" % n)
                self.assign_to(p, r, init, n)
        elif k == "assign":
            _, op, lhs, rhs = s
            if lhs[0] != "var":
                raise GenError("assignment to %r" % (lhs,))
            r = self.reg(lhs[1])
            if op == "=":
                self.assign_to(p, r, rhs, lhs[1])
            else:
                v = self.ev(p, rhs)
                p.emit(OPC["ADD" if op == "+=" else "SUB"], d=r, a=r, b=v, cm=lhs[1] + " " + op)
                self.free(v)
        elif k == "expr":
            e = s[1]
            if e[0] != "call":
                raise GenError("statement %r" % (e,))
            f, args = e[1], e[2]
            if f == "lfo_step":
                r = self.reg(args[0][1])
                p.emit(OPC["LFOSTEP"], d=r, a=r, c=self.const_arg(args[1]), cm="lfo_step(%s)" % args[0][1])
            elif f == "filtered_lfo_step":
                # position = (step(scale) * lfo_saturate(wrap(phase, deltareg) - (rext(post) << 13))
                #             + (0x400000 - step(feedback)) * position) >> 22
                pos = self.reg(args[0][1])
                ph = self.ev(p, args[1])
                delta = self.const_arg(args[2]) if args[2][0] == "num" else None
                if args[2] == ("neg", ("num", 1)):
                    delta = -1
                t = self.newtemp()
                if delta == -1:
                    p.emit(OPC["SUBSAT"], d=t, a=ph, c=self.const_arg(args[3]), cm="lfo_saturate(phase - post)")
                else:
                    p.emit(OPC["WRAPI"], d=t, a=ph, x=delta, cm="lfo_wrap(phase, %x)" % delta)
                    p.emit(OPC["SUBSAT"], d=t, a=t, c=self.const_arg(args[3]), cm="lfo_saturate(.. - post)")
                p.emit(OPC["FMIX"], d=pos, a=t, b=pos, c=self.const_arg(args[4]), c2=self.const_arg(args[5]),
                       cm="filtered_lfo_step(%s)" % args[0][1])
                self.free(ph)
                self.free(t)
            elif re.match(r"^b(rev|cho|var)\.w$", f):
                buf = f.split(".")[0]
                v = self.ev(p, args[1])
                off = self.const_arg(args[0])
                p.emit(OPC["WR"], a=v, c=off // 3, c2=BUF[buf], cm="%s.w(%x)" % (buf, off))
                self.free(v)
            else:
                raise GenError("call statement %s" % f)
        elif k == "incdec":
            pass   # m_buffer_offset-- / m_sample_counter++: done by swp00.sv
        elif k == "block":
            for x in s[1]:
                self.stmt(p, x)
        else:
            raise GenError("statement %r" % (s,))
        for r in list(self.temps_owned):
            self.free(r)

    def assign_to(self, p, r, e, name):
        if e[0] in ("var", "num") or (e[0] == "index"):
            if e[0] == "index":
                self.ev(p, e, dst=r)
                return
            src = self.ev(p, e)
            p.emit(OPC["ADD"], d=r, a=src, b=R_ZERO, cm="%s = .." % name)
            return
        self.ev(p, e, dst=r)
        # tag the last op with the C variable
        op = p.ops[-1]
        p.ops[-1] = op[:8] + ((op[8] + " -> " if op[8] else "") + name,)


def extract(cpp):
    m = re.search(r"void swp00_device::sound_stream_update\(sound_stream &stream\)\n\{\n(.*?)\n\}\n", cpp, re.S)
    if not m:
        raise GenError("sound_stream_update not found")
    body = m.group(1)
    a = body.index("\t\t// Variation block")
    b = body.index("\t\t// 18-bits ADCs")
    return body[a:b]


def main():
    mame, outdir = Path(sys.argv[1]), Path(sys.argv[2])
    cpp = (mame / "src/devices/sound/swp00.cpp").read_text()
    code = extract(cpp)
    p = P(tokenize(code))
    stmts = p.stmts()
    if p.i != len(p.t):
        raise GenError("trailing text near %s" % " ".join(p.t[p.i:p.i + 10]))
    sw = [i for i, s in enumerate(stmts) if s[0] == "switch"]
    if len(sw) != 1 or set(stmts[sw[0]][2]) != {0x00, 0x40, 0x80, 0xc0}:
        raise GenError("expected one switch over the four variants")
    head, cases, tail = stmts[:sw[0]], stmts[sw[0]][2], stmts[sw[0] + 1:]

    comp = Compiler()
    for n in MIX:
        comp.reg(n)
    progs = []
    for v in (0x00, 0x40, 0x80, 0xc0):
        comp.locals = set()
        pr = Prog(comp.regs)
        for n in MIX:
            pr.emit(OPC["LDMIX"], d=comp.reg(n), c=MIX.index(n), cm="%s = voice sum" % n)
        for s in head + cases[v] + tail:
            comp.stmt(pr, s)
        pr.emit(OPC["OUT"], a=comp.reg("dry_l"), c=0, cm="out dry_l")
        pr.emit(OPC["OUT"], a=comp.reg("dry_r"), c=1, cm="out dry_r")
        pr.emit(OPC["END"])
        progs.append(pr)
    # immediates, loaded at the start of every program
    for pr in progs:
        pre = [(OPC["LDI"], r, 0, 0, 0, 0, 0, val, "imm %x" % val) for val, r in sorted(comp.consts.items())]
        pr.ops = pre + pr.ops
    if comp.nreg > 256:
        raise GenError("%d registers, more than 256" % comp.nreg)
    named = set(comp.regs.values())
    waits = []
    for pr in progs:
        pr.ops = allocate_temps(pr.ops, named)
        w0 = stalls(pr.ops)
        pr.ops = schedule(pr.ops)
        waits.append("%d->%d" % (w0, stalls(pr.ops)))

    words, starts, lst = [], [], []
    for v, pr in zip((0x00, 0x40, 0x80, 0xc0), progs):
        starts.append(len(words))
        lst.append("variant %02x at %d" % (v, len(words)))
        for (op, d, a, b, c, c2, c3, x, cm) in pr.ops:
            w = (op << 59) | (d << 51) | (a << 43) | (b << 35) | (c << 27) | ((c2 & 0xff) << 19) | ((c3 & 0xff) << 11) \
                | (x & 0x7ff)
            if x >= 0x800 or c >= 0x100:
                raise GenError("field out of range in %s" % cm)
            lst.append("%4d %-8s d%-3d a%-3d b%-3d c%02x c2 %02x c3 %02x x%03x  %s"
                       % (len(words), OPS[op], d, a, b, c, c2 & 0xff, c3 & 0xff, x, cm))
            words.append(w)
    aw = max(1, (len(words) - 1).bit_length())
    out = ["// Generated by cores/mu50/tools/swp00_meg_gen.py from MAME's swp00.cpp",
           "// (BSD-3-Clause, Olivier Galibert): the MEG code after the voice loop. Do not edit.",
           "localparam int PROG_LEN = %d;" % len(words),
           "localparam int PAW = %d;" % aw,
           "localparam int NREG = %d;" % comp.nreg,
           "localparam logic [PAW-1:0] VAR_START [4] = '{%s};" % ", ".join("%d'd%d" % (aw, s) for s in starts)]
    for i, n in enumerate(OPS):
        out.append("localparam logic [4:0] OP_%s = 5'd%d;" % (n, i))
    # the program ROM as a case statement in a clocked block (a ROM Quartus maps to block RAM);
    # included in swp00_meg.sv where `pc`, `stall` and `ins_q` exist
    rom = ["// Generated by cores/mu50/tools/swp00_meg_gen.py: the MEG program ROM. Do not edit.",
           "always_ff @(posedge clk) if (!stall) case (pc)"]
    rom += ["    %d'd%d: ins_q <= 64'h%016x;" % (aw, i, w) for i, w in enumerate(words)]
    rom += ["    default: ins_q <= 64'h0;", "endcase"]
    outdir.mkdir(parents=True, exist_ok=True)
    (outdir / "swp00_meg_prog.svh").write_text("\n".join(out) + "\n")
    (outdir / "swp00_meg_rom.svh").write_text("\n".join(rom) + "\n")
    (outdir / "swp00_meg_prog.txt").write_text("\n".join(lst) + "\n")
    # the MEG state (MAME's m_ members) in the register file, for the bench: R(member, register)
    regs = ["// Generated by cores/mu50/tools/swp00_meg_gen.py: MEG state registers. Do not edit."]
    regs += ["R(%s, %d)" % (n, i) for n, i in sorted(comp.regs.items(), key=lambda x: x[1]) if n.startswith("m_")]
    (outdir / "swp00_meg_regs.inc").write_text("\n".join(regs) + "\n")
    print("%d ops (%s), %d named registers, %d immediates; result waits per variant %s" % (
        len(words), ", ".join(str(len(p.ops)) for p in progs), comp.nreg, len(comp.consts), ", ".join(waits)))


if __name__ == "__main__":
    main()
