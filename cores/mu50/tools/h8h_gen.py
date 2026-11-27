#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Generate the H8/300H core's microcode from MAME's h8.lst and h8.cpp.

Usage: h8h_gen.py MAME_DIR OUT_DIR

MAME_DIR is a MAME checkout (cores/mu50/mame/patches/BASE). Writes OUT_DIR/h8h_gen.svh, included
by h8h_core.sv, and OUT_DIR/h8h_gen.txt, a listing of every state with the C source it came from.

The core is MAME's instruction handlers compiled to a state machine, statement by statement:

- h8.lst is read with MAME's own parser (h8make.py, imported from MAME_DIR), for the H8/300H
  (type "h"): 264 opcodes, the dispatch steps for prefixed opcodes, reset and the interrupt entry.
- Every C statement is translated to SystemVerilog by a small C front end that keeps C's types:
  every value is held as a 32-bit vector equal to the C value after integer promotion, and the
  conversions, signed comparisons, signed shifts and divisions are written out explicitly.
- A handler is split into states at its bus accesses (read8, read16, read16i, write8, write16)
  and internal(n) calls. Each of these is an operation with MAME's cost (2 states for an access,
  n+1 for internal), so the access order and the state counts are MAME's by construction.
- The ALU helpers (do_add8, do_shal32, set_nzv16 ...) are translated from h8.cpp into SV
  functions that take and return the CCR.
- The opcode decoder mirrors h8make.py's save_exec switch.

The generator fails on any statement it does not know, so a MAME change that adds a construct
shows up here, not as a silent mismatch.
"""
import re
import sys
from pathlib import Path

# ----------------------------------------------------------------------------------------------
# C front end: tokens, expressions, statements

TOK = re.compile(r"""
    (?P<ws>\s+)
  | (?P<num>(?:0[xX][0-9a-fA-F]+|\d+)[uUlL]*)
  | (?P<id>[A-Za-z_][A-Za-z_0-9]*(?:::[A-Za-z_][A-Za-z_0-9]*)*)
  | (?P<op><<=|>>=|\+\+|--|->|<<|>>|<=|>=|==|!=|&&|\|\||\+=|-=|\*=|/=|%=|&=|\|=|\^=|[-+*/%&|^~!<>=?:;,(){}\[\]])
""", re.X)


def tokenize(src):
    out, pos = [], 0
    while pos < len(src):
        m = TOK.match(src, pos)
        if not m:
            raise SyntaxError("bad character at %r" % src[pos:pos + 20])
        pos = m.end()
        if m.lastgroup != "ws":
            out.append((m.lastgroup, m.group()))
    return out


BINPREC = {"||": 4, "&&": 5, "|": 6, "^": 7, "&": 8, "==": 9, "!=": 9, "<": 10, "<=": 10,
           ">": 10, ">=": 10, "<<": 11, ">>": 11, "+": 12, "-": 12, "*": 13, "/": 13, "%": 13}
ASSIGN = {"=", "+=", "-=", "|=", "&=", "^=", "<<=", ">>=", "*=", "/=", "%="}
TYPES = {"u8", "s8", "u16", "s16", "u32", "s32", "u64", "int", "bool"}


class Parser:
    def __init__(self, toks):
        self.t, self.i = toks, 0

    def peek(self, k=0):
        return self.t[self.i + k][1] if self.i + k < len(self.t) else None

    def take(self, v=None):
        tok = self.t[self.i][1]
        if v is not None and tok != v:
            raise SyntaxError("expected %s, got %s near %s" % (v, tok, self.ctx()))
        self.i += 1
        return tok

    def ctx(self):
        return " ".join(x[1] for x in self.t[max(0, self.i - 6):self.i + 6])

    # statements --------------------------------------------------------------------------
    def stmts(self):
        out = []
        while self.i < len(self.t) and self.peek() != "}":
            out.append(self.stmt())
        return out

    def stmt(self):
        p = self.peek()
        if p == "{":
            self.take("{")
            s = self.stmts()
            self.take("}")
            return ("block", s)
        if p == "if":
            self.take()
            self.take("(")
            c = self.expr()
            self.take(")")
            a = self.stmt()
            b = None
            if self.peek() == "else":
                self.take()
                b = self.stmt()
            return ("if", c, a, b)
        if p == "while":
            self.take()
            self.take("(")
            c = self.expr()
            self.take(")")
            return ("while", c, self.stmt())
        if p == "return":
            self.take()
            e = self.expr()
            self.take(";")
            return ("return", e)
        if p == "eat_all_cycles":
            self.take()
            self.take(";")
            return ("eat",)
        if p in TYPES and self.t[self.i + 1][0] == "id":
            ty = self.take()
            decls = []
            while True:
                name = self.take()
                init = None
                if self.peek() == "=":
                    self.take()
                    init = self.expr()
                decls.append((name, init))
                if self.peek() == ",":
                    self.take()
                    continue
                break
            self.take(";")
            return ("decl", ty, decls)
        e = self.expr()
        self.take(";")
        return ("expr", e)

    # expressions ---------------------------------------------------------------------------
    def expr(self):
        lhs = self.cond()
        if self.peek() in ASSIGN:
            op = self.take()
            return ("assign", op, lhs, self.expr())
        return lhs

    def cond(self):
        c = self.binary(4)
        if self.peek() == "?":
            self.take()
            a = self.expr()
            self.take(":")
            b = self.cond()
            return ("tern", c, a, b)
        return c

    def binary(self, minp):
        lhs = self.unary()
        while self.peek() in BINPREC and BINPREC[self.peek()] >= minp:
            op = self.take()
            rhs = self.binary(BINPREC[op] + 1)
            lhs = ("bin", op, lhs, rhs)
        return lhs

    def unary(self):
        p = self.peek()
        if p in ("-", "~", "!", "+"):
            self.take()
            return ("un", p, self.unary())
        return self.postfix()

    def postfix(self):
        e = self.primary()
        while True:
            p = self.peek()
            if p == "[":
                self.take()
                i = self.expr()
                self.take("]")
                e = ("idx", e, i)
            elif p == "(":
                self.take()
                args = []
                if self.peek() != ")":
                    while True:
                        args.append(self.expr())
                        if self.peek() == ",":
                            self.take()
                            continue
                        break
                self.take(")")
                e = ("call", e[1], args)
            elif p in ("++", "--"):
                self.take()
                e = ("post", p, e)
            elif p == "->":
                self.take()
                e = ("arrow", e, self.take())
            else:
                return e

    def primary(self):
        kind, v = self.t[self.i]
        self.i += 1
        if v == "(":
            e = self.expr()
            self.take(")")
            return e
        if kind == "num":
            n = int(v.rstrip("uUlL"), 0)
            return ("num", n, "u64") if v[-1] in "lL" or n > 0xffffffff else ("num", n)
        if kind == "id":
            return ("var", v)
        raise SyntaxError("unexpected %s near %s" % (v, self.ctx()))


def parse(src):
    src = re.sub(r"//[^\n]*", "", src).replace("eat-all-cycles", "eat_all_cycles")
    p = Parser(tokenize(src))
    s = p.stmts()
    if p.i != len(p.t):
        raise SyntaxError("trailing tokens: %s" % p.ctx())
    return s


# ----------------------------------------------------------------------------------------------
# C types. A value of any type is a 32-bit SV vector holding the C value after promotion.

def promote(t):
    if t == "u64":
        return "u64"
    return "uint" if t in ("u32", "uint") else "int"


def arith(a, b):
    pa, pb = promote(a), promote(b)
    if "u64" in (pa, pb):
        return "u64"
    return "uint" if "uint" in (pa, pb) else "int"


SIGNED = {"s8", "s16", "s32", "int"}


# conversion from type a to type b is free when the 32-bit promoted value does not change
FREE = {
    ("u8", "u16"), ("u8", "s16"), ("u8", "u32"), ("u8", "int"), ("u8", "uint"), ("u8", "s32"),
    ("s8", "s16"), ("s8", "int"), ("s8", "s32"), ("s8", "u32"), ("s8", "uint"),
    ("u16", "u32"), ("u16", "int"), ("u16", "uint"), ("u16", "s32"),
    ("s16", "int"), ("s16", "s32"), ("s16", "u32"), ("s16", "uint"),
    ("bool", "u8"), ("bool", "s8"), ("bool", "u16"), ("bool", "s16"), ("bool", "u32"),
    ("bool", "int"), ("bool", "uint"), ("bool", "s32"),
}
for _a in ("u32", "uint", "s32", "int"):
    for _b in ("u32", "uint", "s32", "int"):
        FREE.add((_a, _b))


def conv(sv, frm, to):
    if frm == to or (frm, to) in FREE:
        return sv
    if to == "u64":
        return ("c_sx64(%s)" if frm in SIGNED else "c_zx64(%s)") % sv
    if frm == "u64":
        return conv("c_lo(%s)" % sv, "uint", to)
    return {"u8": "c_u8", "s8": "c_s8", "u16": "c_u16", "s16": "c_s16", "bool": "c_b"}[to] + "(%s)" % sv


def lit(v):
    return "32'h%08x" % (v & 0xffffffff)


# ----------------------------------------------------------------------------------------------
# Expression emitter. Context decides what variables mean (handler state machine or helper).

class GenError(Exception):
    pass


class Emitter:
    """Emits SV for one statement list. `pre` collects lines that must run before the statement
    (hoisted helper calls)."""

    CONSTS = {"F_I": 0x80, "F_UI": 0x40, "F_H": 0x20, "F_U": 0x10, "F_N": 0x08, "F_Z": 0x04,
              "F_V": 0x02, "F_C": 0x01, "EXR_I": 0x07, "EXR_NC": 0x78, "EXR_T": 0x80,
              "true": 1, "false": 0}
    # configuration of the H8/300H in advanced mode (h8.cpp, h8h.cpp)
    CONFIG = {"m_mode_advanced": 1, "m_has_hc": 1, "m_has_mac": 0, "m_has_exr": 0,
              "m_has_trace": 0}

    def local_name(self, n):
        return "l_" + n

    def __init__(self, helpers):
        self.helpers = helpers   # name -> (ret type, [(param type, name)])
        self.pre = []
        self.ntmp = 0
        self.locals = {}         # C local -> type

    def const(self, e):
        """Value of a constant expression, or None."""
        k = e[0]
        if k == "num":
            return e[1]
        if k == "var" and e[1] in self.CONSTS:
            return self.CONSTS[e[1]]
        if k == "var" and e[1] in self.CONFIG:
            return self.CONFIG[e[1]]
        if k == "call" and e[1] == "exr_in_stack":
            return 0
        if k == "un" and e[1] == "!":
            v = self.const(e[2])
            return None if v is None else int(not v)
        if k == "un" and e[1] == "~":
            v = self.const(e[2])
            return None if v is None else ~v
        if k == "un" and e[1] == "-":
            v = self.const(e[2])
            return None if v is None else -v
        if k == "bin":
            a, b = self.const(e[2]), self.const(e[3])
            if a is None or b is None:
                return None
            return {"|": a | b, "&": a & b, "^": a ^ b, "+": a + b, "-": a - b, "*": a * b,
                    "<<": a << b, ">>": a >> b, "&&": int(bool(a and b)),
                    "||": int(bool(a or b)), "==": int(a == b), "!=": int(a != b)}.get(e[1])
        if k == "tern":
            c = self.const(e[1])
            if c is None:
                return None
            return self.const(e[2] if c else e[3])
        return None

    # variables are provided by the subclass
    def var(self, name):
        raise GenError("unknown variable %s" % name)

    def store(self, lhs, sv, ty):
        raise GenError("cannot assign %r" % (lhs,))

    def call(self, name, args):
        raise GenError("unknown call %s" % name)

    def hoist(self, name, args):
        """Helper with a CCR side effect: hoisted to its own line before the statement."""
        rty, params = self.helpers[name]
        if len(args) != len(params):
            raise GenError("%s: %d args" % (name, len(args)))
        av = []
        for (pty, _), a in zip(params, args):
            s, t = self.ex(a)
            av.append(conv(s, t, pty))
        t = "f%d" % self.ntmp
        self.ntmp += 1
        self.pre.append("{%s, %s} = %s(%s);" % (self.ccr_name(), t, "h_" + name,
                                               ", ".join([self.ccr_arg()] + av)))
        return t, rty

    def ex(self, e):
        """(sv, ctype) of an rvalue."""
        if e[0] == "num" and len(e) == 3:
            return "64'h%016x" % e[1], "u64"
        c = self.const(e)
        if c is not None:
            if -0x80000000 <= c <= 0x7fffffff:
                return lit(c), "int"
            if 0 <= c <= 0xffffffff:
                return lit(c), "uint"
            raise GenError("constant out of range %x" % c)
        k = e[0]
        if k == "var":
            if e[1] in self.locals:
                return self.local_name(e[1]), self.locals[e[1]]
            return self.var(e[1])
        if k == "idx":
            if e[1] != ("var", "m_IR"):
                raise GenError("index of %r" % (e[1],))
            i = self.const(e[2])
            return "{16'h0, n_ir[%d]}" % i, "u16"
        if k == "call":
            name, args = e[1], e[2]
            if name in ("s8", "u8", "s16", "u16", "s32", "u32", "u64"):
                s, t = self.ex(args[0])
                return conv(s, t, name), name
            if name in self.helpers:
                return self.hoist(name, args)
            return self.call(name, args)
        if k == "un":
            s, t = self.ex(e[2])
            if e[1] == "!":
                return "{31'h0, (%s) == '0}" % s, "int"
            if e[1] == "~":
                return "(~(%s))" % s, promote(t)
            if e[1] == "-":
                return "('0 - (%s))" % s, promote(t)
            return s, promote(t)
        if k == "bin":
            op = e[1]
            if op in ("&&", "||"):
                a, _ = self.ex(e[2])
                b, _ = self.ex(e[3])
                return "{31'h0, ((%s) != 32'h0) %s ((%s) != 32'h0)}" % (a, op, b), "int"
            a, ta = self.ex(e[2])
            b, tb = self.ex(e[3])
            if "u64" in (ta, tb) and op not in ("&", "|", "^", "+", "-", "==", "!=", "<<", ">>"):
                raise GenError("u64 operator %s" % op)
            if op in ("<<", ">>"):
                t = promote(ta)
                if op == ">>" and t == "int" and ta in SIGNED:
                    return "c_sra(%s, %s)" % (a, b), t
                return "((%s) %s (%s))" % (a, op, b), t
            t = arith(ta, tb)
            if t == "u64":
                a, b = conv(a, ta, "u64"), conv(b, tb, "u64")
            if op in ("<", "<=", ">", ">="):
                if t == "int":
                    return "{31'h0, $signed(%s) %s $signed(%s)}" % (a, op, b), "int"
                return "{31'h0, (%s) %s (%s)}" % (a, op, b), "int"
            if op in ("==", "!="):
                return "{31'h0, (%s) %s (%s)}" % (a, op, b), "int"
            if op in ("/", "%"):
                f = {("/", "int"): "c_sdiv", ("%", "int"): "c_smod",
                     ("/", "uint"): "c_udiv", ("%", "uint"): "c_umod"}[(op, t)]
                return "%s(%s, %s)" % (f, a, b), t
            return "((%s) %s (%s))" % (a, op, b), t
        if k == "tern":
            c, _ = self.ex(e[1])
            a, ta = self.ex(e[2])
            b, tb = self.ex(e[3])
            return "(((%s) != 32'h0) ? (%s) : (%s))" % (c, a, b), arith(ta, tb)
        raise GenError("expression %r" % (e,))

    def cond(self, e):
        s, _ = self.ex(e)
        return "(%s) != '0" % s

    def assign(self, e):
        """Assignment / compound assignment / post-increment expression statement."""
        if e[0] == "post":
            op = "+=" if e[1] == "++" else "-="
            e = ("assign", op, e[2], ("num", 1))
        _, op, lhs, rhs = e
        if op == "=":
            s, t = self.ex(rhs)
        else:
            s, t = self.ex(("bin", op[:-1], lhs, rhs))
        self.store(lhs, s, t)


# ----------------------------------------------------------------------------------------------
# Helpers from h8.cpp, translated to SV functions:
#   function automatic logic [39:0] h_<name>(logic [7:0] ccr_in, <params>);  -> {ccr, value}

HELPER_RE = re.compile(r"^(u8|u16|u32|void) h8_device::(do_\w+|set_nzv?\d+)\(([^)]*)\)\n\{\n(.*?)\n\}",
                       re.M | re.S)


class HelperEmitter(Emitter):
    def ccr_arg(self):
        return "ccr"

    def var(self, name):
        if name == "m_CCR":
            return "ccr", "u8"
        raise GenError("helper variable %s" % name)

    def ccr_name(self):
        return "ccr"

    def store(self, lhs, sv, ty):
        if lhs == ("var", "m_CCR"):
            self.pre.append("ccr = %s;" % conv(sv, ty, "u8"))
        elif lhs[0] == "var" and lhs[1] in self.locals:
            self.pre.append("l_%s = %s;" % (lhs[1], conv(sv, ty, self.locals[lhs[1]])))
        else:
            raise GenError("helper store %r" % (lhs,))


def helper_sv(name, rty, params, body, out):
    em = HelperEmitter({})
    for pty, pn in params:
        em.locals[pn] = pty
    ast = parse(body)
    lines = []

    def collect(st):
        for s in st:
            if s[0] == "decl":
                for n, _ in s[2]:
                    em.locals[n] = s[1]
            elif s[0] == "block":
                collect(s[1])
            elif s[0] == "if":
                collect([s[2]] + ([s[3]] if s[3] else []))
    collect(ast)

    def emit(st, ind):
        for i, s in enumerate(st):
            k = s[0]
            if k == "decl":
                for n, init in s[2]:
                    if init is not None:
                        em.store(("var", n), *em.ex(init))
                        lines.extend(ind + x for x in em.pre)
                        em.pre = []
            elif k == "expr":
                em.assign(s[1])
                lines.extend(ind + x for x in em.pre)
                em.pre = []
            elif k == "if" and em.const(s[1]) is not None:
                br = s[2] if em.const(s[1]) else s[3]
                if br is not None:
                    emit(br[1] if br[0] == "block" else [br], ind)
            elif k == "if":
                c = em.cond(s[1])
                assert not em.pre
                lines.append(ind + "if (%s) begin" % c)
                emit(s[2][1] if s[2][0] == "block" else [s[2]], ind + "  ")
                if s[3] is not None:
                    lines.append(ind + "end else begin")
                    emit(s[3][1] if s[3][0] == "block" else [s[3]], ind + "  ")
                lines.append(ind + "end")
            elif k == "return":
                if i != len(st) - 1 or ind != "    ":
                    raise GenError("%s: return not last" % name)
                v, t = em.ex(s[1])
                assert not em.pre
                lines.append(ind + "ret = %s;" % conv(v, t, rty))
            else:
                raise GenError("%s: statement %r" % (name, s))
    emit(ast, "    ")
    out.append("function automatic logic [39:0] h_%s(input logic [31:0] ccr_in%s);" % (
        name, "".join(", input logic [31:0] p_%s" % pn for _, pn in params)))
    decl = sorted(set(["ccr", "ret"] + ["l_" + n for n in em.locals if em.locals[n] != "u64"]))
    out.append("    logic [31:0] %s;" % ", ".join(decl))
    d64 = sorted("l_" + n for n in em.locals if em.locals[n] == "u64")
    if d64:
        out.append("    logic [63:0] %s;" % ", ".join(d64))
    out.append("    ccr = ccr_in;")
    out.append("    ret = 32'h0;")
    for _, pn in params:
        out.append("    l_%s = p_%s;" % (pn, pn))
    out.extend(lines)
    out.append("    return {ccr[7:0], ret};")
    out.append("endfunction")
    out.append("")


def load_helpers(cpp):
    helpers, bodies = {}, {}
    for m in HELPER_RE.finditer(cpp):
        rty, name, params, body = m.groups()
        ps = []
        for p in params.split(","):
            pty, pn = p.split()
            ps.append((pty, pn))
        helpers[name] = (rty if rty != "void" else "int", ps)
        bodies[name] = body
    return helpers, bodies


# ----------------------------------------------------------------------------------------------
# Handlers: the state machine.
#
# Variables in the core (h8h_core.sv) next-state copies: n_tmp1, n_tmp2, n_pc, n_npc (32-bit),
# n_pir, n_ir[0:4] (16-bit), n_ccr, n_exr (8-bit), n_r (16 x 16-bit), n_inst_state (24-bit),
# n_tvec, n_tlvl (taken interrupt), rd (the last read's data, registered).
#
# A state is: list of SV lines, then a terminator:
#   ("goto", s) | ("op", kind, addr_sv, data_sv, cost_sv, s) | ("end",) | ("if", cond, s1, s2)

OPK = {"read16i": "OP_F16", "read8": "OP_R8", "read16": "OP_R16", "write8": "OP_W8",
       "write16": "OP_W16"}


class HandlerEmitter(Emitter):
    VARS = {"m_TMP1": ("n_tmp1", "u32", 32), "m_TMP2": ("n_tmp2", "u32", 32),
            "m_PC": ("n_pc", "u32", 32), "m_NPC": ("n_npc", "u32", 32),
            "m_PIR": ("n_pir", "u16", 16), "m_CCR": ("n_ccr", "u8", 8),
            "m_EXR": ("n_exr", "u8", 8), "m_taken_irq_vector": ("n_tvec", "int", 8),
            "m_inst_state": ("n_inst_state", "int", 24)}

    def ccr_name(self):
        return "n_ccr"

    def ccr_arg(self):
        return "{24'h0, n_ccr}"

    def hoist(self, name, args):
        """A helper call runs on the shared ALU: a marker ends the state with the inputs, and
        the next state starts with the result (Compiler.flush)."""
        rty, params = self.helpers[name]
        av = []
        for (pty, _), a in zip(params, args):
            s, t = self.ex(a)
            av.append(conv(s, t, pty))
        t = "f%d" % self.ntmp
        self.ntmp += 1
        self.pre.append(("alu", name, av, t))
        return t, rty

    def var(self, name):
        if name in self.VARS:
            sv, t, w = self.VARS[name]
            return (sv if w == 32 else "{%d'h0, %s}" % (32 - w, sv)), t
        if name == "m_irq_vector":
            return "{24'h0, irq_vector}", "int"
        if name == "m_standby_pending":
            return "{31'h0, standby_pending}", "int"
        raise GenError("variable %s" % name)

    def store(self, lhs, sv, ty):
        if lhs[0] == "var" and lhs[1] in self.VARS:
            n, t, w = self.VARS[lhs[1]]
            v = conv(sv, ty, t)
            self.pre.append("%s = %s;" % (n, v if w == 32 else "t%d(%s)" % (w, v)))
        elif lhs[0] == "idx" and lhs[1] == ("var", "m_IR"):
            self.pre.append("n_ir[%d] = t16(%s);" % (self.const(lhs[2]), conv(sv, ty, "u16")))
        elif lhs[0] == "var" and lhs[1] in self.locals:
            self.pre.append("n_l_%s = %s;" % (lhs[1], conv(sv, ty, self.locals[lhs[1]])))
        else:
            raise GenError("store %r" % (lhs,))

    def local_name(self, n):
        # handler locals (q, r of DIVXS/DIVXU) live across states: registers l_q, l_r
        return "n_l_" + n

    def call(self, name, args):
        if name in ("r8_r", "r16_r", "r32_r"):
            s, t = self.ex(args[0])
            return "%s(r, %s)" % (name, s), {"r8_r": "u8", "r16_r": "u16", "r32_r": "u32"}[name]
        if name in ("read8", "read16", "read16i"):
            raise GenError("second bus read in one statement")
        raise GenError("call %s" % name)


def find_read(e):
    """The first read8/read16/read16i call in an expression (evaluation order is irrelevant: one
    per statement)."""
    if isinstance(e, tuple):
        if e[0] == "call" and e[1] in ("read8", "read16", "read16i"):
            return e
        for x in e[1:]:
            if isinstance(x, (tuple, list)):
                r = find_read(x) if isinstance(x, tuple) else next(
                    (find_read(y) for y in x if find_read(y)), None)
                if r:
                    return r
    return None


def replace_node(e, node, new):
    if e is node:
        return new
    if isinstance(e, tuple):
        return tuple(replace_node(x, node, new) if isinstance(x, (tuple, list)) else x for x in e)
    if isinstance(e, list):
        return [replace_node(x, node, new) for x in e]
    return e


STATE_CALLS = {"read8", "read16", "read16i", "write8", "write16", "internal", "r8_w", "r16_w",
               "r32_w"}


def has_op(s, helpers=()):
    """Does a statement (or list) need state boundaries: a bus access, internal(), eat, a
    register write, a helper (shared ALU) or a division?"""
    if isinstance(s, list):
        return any(has_op(x, helpers) for x in s)
    if isinstance(s, tuple):
        if s and s[0] == "call" and (s[1] in STATE_CALLS or s[1] in helpers):
            return True
        if s and s[0] in ("eat", "while"):
            return True
        if s and s[0] == "bin" and s[1] in ("/", "%"):
            return True
        return any(has_op(x, helpers) for x in s[1:] if isinstance(x, (tuple, list)))
    return False


def uses_regs(s):
    """Does a statement read or write the register file?"""
    if isinstance(s, list):
        return any(uses_regs(x) for x in s)
    if isinstance(s, tuple):
        if s and s[0] == "call" and s[1] in ("r8_r", "r16_r", "r32_r", "r8_w", "r16_w", "r32_w"):
            return True
        return any(uses_regs(x) for x in s[1:] if isinstance(x, (tuple, list)))
    return False


class Compiler:
    def __init__(self, helpers):
        self.helpers = helpers
        self.states = []       # [name, comment, lines, term]
        self.listing = []

    def new(self, comment):
        # [name, comment, lines, terminator, register write pending]
        st = ["S_%d" % len(self.states), comment, [], None, False]
        self.states.append(st)
        return st

    def split(self, cur, comment):
        nxt = self.new(comment)
        cur[3] = ("goto", nxt[0])
        return nxt

    def compile_handler(self, title, src):
        em = HandlerEmitter(self.helpers)
        ast = parse("\n".join(src))
        self.locals_of(ast, em)
        first = self.new(title)
        cur = self.block(ast, first, em)
        if cur is not None:
            cur[3] = ("end",)
        return first[0]

    def locals_of(self, st, em):
        for s in st:
            if s[0] == "decl":
                for n, _ in s[2]:
                    em.locals[n] = s[1]
            elif s[0] == "block":
                self.locals_of(s[1], em)
            elif s[0] == "if":
                self.locals_of([s[2]] + ([s[3]] if s[3] else []), em)
            elif s[0] == "while":
                self.locals_of([s[2]], em)

    def flush(self, em, cur):
        """Move the pending lines into cur. A helper or division marker ends the state; the
        next state starts with its result. Returns the state to continue in."""
        pre, em.pre = em.pre, []
        markers = 0
        for x in pre:
            if isinstance(x, str):
                cur[2].append(x)
                continue
            markers += 1
            if markers > 1:
                raise GenError("two helper calls or divisions in one statement")
            nxt = self.new("after " + x[0])
            if x[0] == "alu":
                _, name, av, f = x
                cur[3] = ("alu", "ALU_" + name.upper(), "{24'h0, n_ccr}", av, nxt[0])
                nxt[2].append("{n_ccr, %s} = {alu_q[39:32], alu_q[31:0]};" % f)
            else:
                _, signed, rem, a, b, f = x
                cur[3] = ("op", "OP_DIV", "32'h0", "16'h0", "6'd0", nxt[0],
                          (a, b, "1'b1" if signed else "1'b0"))
                nxt[2].append("%s = %s;" % (f, "div_r" if rem else "div_q"))
            cur = nxt
        return cur

    def block(self, st, cur, em):
        """Compile statements into cur (and new states). Returns the state where control
        continues, or None if control ended."""
        for s in st:
            if cur is None:
                raise GenError("statement after the end of a handler: %r" % (s,))
            if cur[4] and uses_regs(s):
                cur = self.split(cur, "after register write")
            cur = self.stmt(s, cur, em)
        return cur

    def sub(self, s):
        return s[1] if s[0] == "block" else [s]

    def stmt(self, s, cur, em):
        k = s[0]
        if k == "block":
            return self.block(s[1], cur, em)
        if k == "decl":
            for n, init in s[2]:
                if init is not None:
                    cur = self.expr_stmt(("assign", "=", ("var", n), init), cur, em)
            return cur
        if k == "if":
            c = em.const(s[1])
            if c is not None:
                br = s[2] if c else s[3]
                return cur if br is None else self.stmt(br, cur, em)
            if not has_op(s, self.helpers):
                cond = em.cond(s[1])
                cur = self.flush(em, cur)
                cur[2].append("if (%s) begin" % cond)
                cur = self.inline(self.sub(s[2]), cur, em, "  ")
                if s[3] is not None:
                    cur[2].append("end else begin")
                    cur = self.inline(self.sub(s[3]), cur, em, "  ")
                cur[2].append("end")
                return cur
            cond = em.cond(s[1])
            cur = self.flush(em, cur)
            a = self.new("then")
            join = self.new("join")
            b = self.new("else") if s[3] is not None else join
            cur[3] = ("if", cond, a[0], b[0])
            ea = self.block(self.sub(s[2]), a, em)
            if ea is not None:
                ea[3] = ("goto", join[0])
            if s[3] is not None:
                eb = self.block(self.sub(s[3]), b, em)
                if eb is not None:
                    eb[3] = ("goto", join[0])
            return join
        if k == "while":
            test = self.new("while")
            cur[3] = ("goto", test[0])
            body = self.new("do")
            done = self.new("done")
            cond = em.cond(s[1])
            if self.flush(em, test) is not test:
                raise GenError("helper call in a while condition")
            test[3] = ("if", cond, body[0], done[0])
            eb = self.block(self.sub(s[2]), body, em)
            if eb is not None:
                eb[3] = ("goto", test[0])
            return done
        if k == "eat":
            nxt = self.new("eat")
            cur[3] = ("op", "OP_EAT", "24'h0", "16'h0", "6'd0", nxt[0])
            return nxt
        if k == "expr":
            return self.expr_stmt(s[1], cur, em)
        raise GenError("statement %r" % (s,))

    def inline(self, st, cur, em, ind):
        """Statements with no operation, inline in the current state."""
        for s in st:
            k = s[0]
            if k == "if":
                c = em.const(s[1])
                if c is not None:
                    br = s[2] if c else s[3]
                    if br is not None:
                        self.inline(self.sub(br), cur, em, ind)
                    continue
                cond = em.cond(s[1])
                if self.flush(em, cur) is not cur:
                    raise GenError("helper call in an inline condition")
                cur[2].append(ind + "if (%s) begin" % cond)
                self.inline(self.sub(s[2]), cur, em, ind + "  ")
                if s[3] is not None:
                    cur[2].append(ind + "end else begin")
                    self.inline(self.sub(s[3]), cur, em, ind + "  ")
                cur[2].append(ind + "end")
            elif k == "block":
                self.inline(s[1], cur, em, ind)
            elif k in ("expr", "decl"):
                n0 = len(cur[2])
                r = self.stmt(s, cur, em)
                if r is not cur:
                    raise GenError("operation inside an inline block")
                for i in range(n0, len(cur[2])):
                    cur[2][i] = ind + cur[2][i]
            else:
                raise GenError("inline %r" % (s,))
        return cur

    def expr_stmt(self, e, cur, em):
        # bus read anywhere in the statement: split
        rd = find_read(e)
        if rd is not None:
            name, args = rd[1], rd[2]
            a, t = em.ex(args[0])
            addr = conv(a, t, "u32")
            cur = self.flush(em, cur)
            nxt = self.new("after " + name)
            cur[3] = ("op", OPK[name], addr, "16'h0", "6'd2", nxt[0])
            ty = {"read8": "u8", "read16": "u16", "read16i": "u16"}[name]
            e = replace_node(e, rd, ("rdval", ty))
            cur = nxt
        if e[0] == "call":
            return self.call_stmt(e[1], e[2], cur, em)
        if e[0] in ("assign", "post"):
            em.assign(e)
            return self.flush(em, cur)
        raise GenError("expression statement %r" % (e,))

    def call_stmt(self, name, args, cur, em):
        if name in ("write8", "write16"):
            a, ta = em.ex(args[0])
            d, td = em.ex(args[1])
            cur = self.flush(em, cur)
            nxt = self.new("after " + name)
            cur[3] = ("op", OPK[name], conv(a, ta, "u32"),
                      "t16(%s)" % conv(d, td, "u16" if name == "write16" else "u8"),
                      "6'd2", nxt[0])
            return nxt
        if name == "internal":
            n = em.const(args[0])
            nxt = self.new("after internal")
            cur[3] = ("op", "OP_INT", "24'h0", "16'h0", "6'd%d" % (n + 1), nxt[0])
            return nxt
        if name in self.helpers:
            em.hoist(name, args)
            return self.flush(em, cur)
        if name in ("r8_w", "r16_w", "r32_w"):
            # the register file's one write port, applied at the end of the state
            r, tr = em.ex(args[0])
            v, tv = em.ex(args[1])
            cur = self.flush(em, cur)
            if cur[4]:
                raise GenError("two register writes in one state")
            vt = {"r8_w": "u8", "r16_w": "u16", "r32_w": "u32"}[name]
            cur[2].append("rf_we = %s;" % {"r8_w": "RF_W8", "r16_w": "RF_W16", "r32_w": "RF_W32"}[name])
            cur[2].append("rf_idx = t4(%s);" % r)
            cur[2].append("rf_val = %s;" % conv(v, tv, vt))
            cur[4] = True
            return cur
        if name in ("prefetch_done", "prefetch_done_notrace", "prefetch_done_noirq"):
            # h8.cpp prefetch_done(): no DMA, DTC or trace on this core
            if name != "prefetch_done_noirq":
                cur[2].append("if (irq_vector != 8'h0) begin")
                cur[2].append("  n_inst_state = 24'h010001;")
                cur[2].append("  n_tvec = irq_vector;")
                cur[2].append("  n_tlvl = irq_level;")
                cur[2].append("end else begin")
                cur[2].append("  n_inst_state = {8'h0, n_pir};")
                cur[2].append("  n_ir[0] = n_pir;")
                cur[2].append("end")
            else:
                cur[2].append("n_inst_state = {8'h0, n_pir};")
                cur[2].append("n_ir[0] = n_pir;")
            return cur
        if name == "prefetch_switch":
            p, tp = em.ex(args[0])
            i, ti = em.ex(args[1])
            cur = self.flush(em, cur)
            cur[2].append("n_npc = %s & 32'h00ffffff;" % p)
            cur[2].append("n_pc = %s + 32'h2;" % p)
            cur[2].append("n_pir = t16(%s);" % conv(i, ti, "u16"))
            return cur
        if name == "update_irq_filter":
            # h83003.cpp update_irq_filter()
            cur[2].append("n_filter = filter_of(n_ccr, syscr_ue);")
            cur[2].append("n_filter_we = 1'b1;")
            # the strobe must reach the peripherals before the next bus access (MAME calls
            # set_filter() there and then): end the state here
            return self.split(cur, "after update_irq_filter")
        if name == "irq_setup":
            cur[2].append(IRQ_SETUP)
            return cur
        if name == "interrupt_taken":
            cur[2].append("n_irq_taken = 1'b1;")
            return self.split(cur, "after interrupt_taken")
        if name in ("debugger_exception_hook",):
            return cur
        if name in ("notify_standby", "suspend", "m_standby_cb"):
            cur[2].append("n_standby = 1'b1;")
            return cur
        raise GenError("call statement %s" % name)


# h83003.cpp irq_setup() / trapa_setup(): set I, and UI unless SYSCR.UE
IRQ_SETUP = "n_ccr = n_ccr | (syscr_ue ? 8'h80 : 8'hc0);"

# rdval node: the registered read data
_orig_ex = HandlerEmitter.ex


def _ex(self, e):
    if e[0] == "bin" and e[1] in ("/", "%"):
        a, ta = self.ex(e[2])
        b, tb = self.ex(e[3])
        t = arith(ta, tb)
        if t == "u64":
            raise GenError("u64 division")
        f = "f%d" % self.ntmp
        self.ntmp += 1
        self.pre.append(("div", t == "int", e[1] == "%", a, b, f))
        return f, t
    if e[0] == "rdval":
        return ("{24'h0, rd[7:0]}" if e[1] == "u8" else "{16'h0, rd}"), e[1]
    if e[0] == "call" and e[1] == "trapa_setup":
        self.pre.append(IRQ_SETUP)
        return lit(8), "int"
    if e[0] == "call" and e[1] == "total_cycles":
        return "32'h0", "int"
    return _orig_ex(self, e)


HandlerEmitter.ex = _ex


def _store(self, lhs, sv, ty, _orig=HandlerEmitter.store):
    if lhs == ("var", "m_standby_time"):
        return
    return _orig(self, lhs, sv, ty)


HandlerEmitter.store = _store


# ----------------------------------------------------------------------------------------------
# Decoder: mirrors h8make.py OpcodeList.save_exec

def case_values(val, fmask):
    """The byte values of one case label of h8make.py save_exec."""
    c, s = [], 0
    while s < 0x100:
        c.append(val | s)
        s += 1
        while s & fmask:
            s += s & fmask
    return c


def dispatch_tables(ol):
    """h8make.py save_exec as data: {level: [(hi values, target | (lo mask, [(lo values,
    target)]))]}; a target is an Opcode or a DispatchStep."""
    tabs = {}
    for i in range(0, len(ol.dispatch_info) + 2):
        if i == 1 or not (i == 0 or ol.dispatch_info[i - 2].enabled):
            continue
        h = ol.get(i)
        rows = []
        for val, h2 in sorted(h.d.items()):
            if not h2.enabled:
                continue
            hi = case_values(val, h2.premask | (h.mask ^ 0xff))
            if h2.mask == 0x00:
                rows.append((hi, h2.d[0]))
                continue
            mpos = 1 if i == 0 else ol.dispatch_info[i - 2].pos + 1
            sub = []
            for val2, n in sorted(h2.d.items()):
                if not n.enabled:
                    continue
                fm = h2.mask ^ 0xff
                fm |= n.mask if n.is_dispatch() else n.mask[mpos]
                sub.append((case_values(val2, fm), n))
            rows.append((hi, (h2.mask, sub)))
        tabs[i] = (h.mask, rows)
    return tabs


def decode_words(ol, tabs, words):
    """The Opcode MAME executes for an instruction starting with `words` (None if illegal)."""
    level, w = 0, words[0]
    while True:
        mask, rows = tabs[level]
        x, n = (w >> 8) & mask, None
        for hi, t in rows:
            if x in hi:
                if isinstance(t, tuple):
                    m2, sub = t
                    for lo, t2 in sub:
                        if (w & m2) in lo:
                            n = t2
                            break
                else:
                    n = t
                break
        if n is None:
            return None
        if not n.is_dispatch():
            return n
        end = n.pos // 2 + n.skip
        level, w = n.id, words[end]


def decoder(ol, handler_state, dispatch_state):
    out = ["function automatic logic [SW-1:0] decode(input logic [23:0] is);",
           "    logic [SW-1:0] s;",
           "    s = S_ILLEGAL;",
           "    case (is[23:16])"]

    def tgt(n):
        return dispatch_state[n.name] if n.is_dispatch() else handler_state[n.function_name()]

    def vals(v):
        return ", ".join("8'h%02x" % x for x in v)

    out.append("    8'h01: case (is[15:0])")
    for sta in ol.states_info:
        if sta.enabled and sta.name in handler_state:
            out.append("        16'h%04x: s = %s;" % (sta.val & 0xffff, handler_state[sta.name]))
    out.append("        default: s = S_ILLEGAL;")
    out.append("        endcase")
    for i, (mask, rows) in sorted(dispatch_tables(ol).items()):
        out.append("    8'h%02x: case (is[15:8] & 8'h%02x)" % (i, mask))
        for hi, t in rows:
            out.append("        %s:" % vals(hi))
            if isinstance(t, tuple):
                m2, sub = t
                out.append("            case (is[7:0] & 8'h%02x)" % m2)
                for lo, t2 in sub:
                    out.append("            %s: s = %s;" % (vals(lo), tgt(t2)))
                out.append("            default: s = S_ILLEGAL;")
                out.append("            endcase")
            else:
                out.append("            s = %s;" % tgt(t))
        out.append("        default: s = S_ILLEGAL;")
        out.append("        endcase")
    out.append("    default: s = S_ILLEGAL;")
    out.append("    endcase")
    out.append("    return s;")
    out.append("endfunction")
    return out


def load_opcodes(mame):
    """MAME's opcode list for the H8/300H (h8make.py from MAME_DIR)."""
    h8dir = Path(mame) / "src/devices/cpu/h8"
    sys.path.insert(0, str(h8dir))
    import h8make  # noqa: E402  (MAME, BSD-3-Clause)
    ol = h8make.OpcodeList(str(h8dir / "h8.lst"), 1)
    ol.build_dispatch()
    return ol


# ----------------------------------------------------------------------------------------------

def main():
    mame, outdir = Path(sys.argv[1]), Path(sys.argv[2])
    h8dir = mame / "src/devices/cpu/h8"
    ol = load_opcodes(mame)

    helpers, bodies = load_helpers((h8dir / "h8.cpp").read_text())
    used = set()
    srcs = []
    ops = [o for o in ol.opcode_info if o.enabled]
    specials = [s for s in ol.states_info if s.enabled and s.name in ("reset", "irq")]
    dsp = [d for d in ol.dispatch_info if d.enabled]
    for o in ops + specials:
        srcs.append("\n".join(o.source))
    for d in dsp:
        srcs.append("\n".join(d.source()))
    for s in srcs:
        for n in re.findall(r"\b(do_\w+|set_nzv?\d+)\(", s):
            used.add(n)

    fn = []
    for n in sorted(used):
        rty, ps = helpers[n]
        helper_sv(n, rty, ps, bodies[n], fn)

    comp = Compiler({n: helpers[n] for n in used})
    handler_state, dispatch_state = {}, {}
    listing = []
    for o in ops:
        name = o.function_name()
        try:
            handler_state[name] = comp.compile_handler(name, o.source)
        except (GenError, SyntaxError) as err:
            raise SystemExit("%s: %s" % (name, err))
        listing.append((handler_state[name], name, o.source))
    for s in specials:
        handler_state[s.name] = comp.compile_handler("state_" + s.name, s.source)
        listing.append((handler_state[s.name], "state_" + s.name, s.source))
    for d in dsp:
        dispatch_state[d.name] = comp.compile_handler("dispatch_" + d.name, d.source())
        listing.append((dispatch_state[d.name], "dispatch_" + d.name, d.source()))

    nstates = len(comp.states) + 3
    sw = max(1, (nstates - 1).bit_length())
    out = ["// Generated by cores/mu50/tools/h8h_gen.py from MAME's h8.lst and h8.cpp",
           "// (BSD-3-Clause, Olivier Galibert). Do not edit.", "",
           "localparam int SW = %d;" % sw,
           "localparam logic [SW-1:0] S_EXEC = %d'd0, S_ILLEGAL = %d'd1, S_HALT = %d'd2;" % (sw, sw, sw)]
    for i, st in enumerate(comp.states):
        out.append("localparam logic [SW-1:0] %s = %d'd%d;" % (st[0], sw, i + 3))
    out.append("")
    out.extend(fn)
    # the shared ALU: one instance of every helper, selected by alu_sel
    names = sorted(used)
    out.append("localparam int ALU_SW = %d;" % max(1, (len(names) - 1).bit_length()))
    for i, n in enumerate(names):
        out.append("localparam logic [ALU_SW-1:0] ALU_%s = %d;" % (n.upper(), i))
    out.append("function automatic logic [39:0] alu_eval(input logic [ALU_SW-1:0] sel, "
               "input logic [31:0] c, input logic [31:0] a, input logic [31:0] b);")
    out.append("    case (sel)")
    for n in names:
        args = ["c", "a", "b"][:1 + len(helpers[n][1])]
        out.append("    ALU_%s: return h_%s(%s);" % (n.upper(), n, ", ".join(args)))
    out.append("    default: return 40'h0;")
    out.append("    endcase")
    out.append("endfunction")
    out.append("")
    out.extend(decoder(ol, handler_state, dispatch_state))
    body = ["// Generated by cores/mu50/tools/h8h_gen.py. Do not edit. Case items of h8h_core's state",
            "// machine, included inside its case (st).", ""]
    for name, comment, lines, term, _ in comp.states:
        body.append("%s: begin // %s" % (name, comment))
        body.extend("    " + x for x in lines)
        if term is None:
            raise SystemExit("state %s has no terminator" % name)
        if term[0] == "goto":
            body.append("    n_st = %s;" % term[1])
        elif term[0] == "end":
            body.append("    n_st = S_EXEC;")
        elif term[0] == "if":
            body.append("    n_st = (%s) ? %s : %s;" % (term[1], term[2], term[3]))
        elif term[0] == "alu":
            _, sel, ccr, av, nxt = term
            body.append("    alu_ld = 1'b1;")
            body.append("    alu_sel_n = %s;" % sel)
            body.append("    alu_c_n = %s;" % ccr)
            body.append("    alu_a_n = %s;" % av[0])
            if len(av) > 1:
                body.append("    alu_b_n = %s;" % av[1])
            body.append("    n_st = %s;" % nxt)
        elif term[0] == "op":
            _, kind, addr, data, cost, nxt = term[:6]
            if len(term) > 6:
                a, b, sg = term[6]
                body.append("    div_a_n = %s;" % a)
                body.append("    div_b_n = %s;" % b)
                body.append("    div_signed_n = %s;" % sg)
            body.append("    op_kind = %s;" % kind)
            if addr != "24'h0":
                body.append("    op_addr = %s;" % addr)
            if data != "16'h0":
                body.append("    op_data = %s;" % data)
            body.append("    op_cost = %s;" % cost)
            body.append("    n_st = %s;" % nxt)
        body.append("end")
    outdir.mkdir(parents=True, exist_ok=True)
    (outdir / "h8h_gen.svh").write_text("\n".join(out) + "\n")
    (outdir / "h8h_gen_states.svh").write_text("\n".join(body) + "\n")

    lst = []
    for st, name, src in listing:
        lst.append("%s %s" % (st, name))
        lst.extend("    " + x.strip() for x in src)
    (outdir / "h8h_gen.txt").write_text("\n".join(lst) + "\n")
    print("%d opcodes, %d dispatch steps, %d states, %d helpers" % (
        len(ops), len(dsp), nstates, len(used)))


if __name__ == "__main__":
    main()
