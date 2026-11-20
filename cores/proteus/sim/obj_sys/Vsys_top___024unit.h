// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vsys_top.h for the primary calling header

#ifndef VERILATED_VSYS_TOP___024UNIT_H_
#define VERILATED_VSYS_TOP___024UNIT_H_  // guard

#include "verilated.h"
#include "Vsys_top___024unit.h"


class Vsys_top__Syms;
struct Vsys_top_s_clks__struct__0 {
    CData/*0:0*/ __PVT__clk;
    CData/*0:0*/ __PVT__extReset;
    CData/*0:0*/ __PVT__pwrUp;
    CData/*0:0*/ __PVT__enPhi1;
    CData/*0:0*/ __PVT__enPhi2;

    bool operator==(const Vsys_top_s_clks__struct__0& rhs) const {
        return __PVT__clk == rhs.__PVT__clk
            && __PVT__extReset == rhs.__PVT__extReset
            && __PVT__pwrUp == rhs.__PVT__pwrUp
            && __PVT__enPhi1 == rhs.__PVT__enPhi1
            && __PVT__enPhi2 == rhs.__PVT__enPhi2;
    }
    bool operator!=(const Vsys_top_s_clks__struct__0& rhs) const {
        return !(*this == rhs);
    }

    bool operator<(const Vsys_top_s_clks__struct__0& rhs) const {
        return std::tie(__PVT__clk, __PVT__extReset, __PVT__pwrUp, __PVT__enPhi1, __PVT__enPhi2)
            <  std::tie(rhs.__PVT__clk, rhs.__PVT__extReset, rhs.__PVT__pwrUp, rhs.__PVT__enPhi1, rhs.__PVT__enPhi2);
    }
};
template <>
struct VlIsCustomStruct<Vsys_top_s_clks__struct__0> : public std::true_type {};
struct Vsys_top_s_irdecod__struct__0 {
    CData/*0:0*/ __PVT__isPcRel;
    CData/*0:0*/ __PVT__isTas;
    CData/*0:0*/ __PVT__implicitSp;
    CData/*0:0*/ __PVT__toCcr;
    CData/*0:0*/ __PVT__rxIsDt;
    CData/*0:0*/ __PVT__ryIsDt;
    CData/*0:0*/ __PVT__rxIsUsp;
    CData/*0:0*/ __PVT__rxIsMovem;
    CData/*0:0*/ __PVT__movemPreDecr;
    CData/*0:0*/ __PVT__isByte;
    CData/*0:0*/ __PVT__isMovep;
    CData/*2:0*/ __PVT__rx;
    CData/*2:0*/ __PVT__ry;
    CData/*0:0*/ __PVT__rxIsAreg;
    CData/*0:0*/ __PVT__ryIsAreg;
    SData/*15:0*/ __PVT__ftuConst;
    CData/*5:0*/ __PVT__macroTvn;
    CData/*0:0*/ __PVT__inhibitCcr;

    bool operator==(const Vsys_top_s_irdecod__struct__0& rhs) const {
        return __PVT__isPcRel == rhs.__PVT__isPcRel
            && __PVT__isTas == rhs.__PVT__isTas
            && __PVT__implicitSp == rhs.__PVT__implicitSp
            && __PVT__toCcr == rhs.__PVT__toCcr
            && __PVT__rxIsDt == rhs.__PVT__rxIsDt
            && __PVT__ryIsDt == rhs.__PVT__ryIsDt
            && __PVT__rxIsUsp == rhs.__PVT__rxIsUsp
            && __PVT__rxIsMovem == rhs.__PVT__rxIsMovem
            && __PVT__movemPreDecr == rhs.__PVT__movemPreDecr
            && __PVT__isByte == rhs.__PVT__isByte
            && __PVT__isMovep == rhs.__PVT__isMovep
            && __PVT__rx == rhs.__PVT__rx
            && __PVT__ry == rhs.__PVT__ry
            && __PVT__rxIsAreg == rhs.__PVT__rxIsAreg
            && __PVT__ryIsAreg == rhs.__PVT__ryIsAreg
            && __PVT__ftuConst == rhs.__PVT__ftuConst
            && __PVT__macroTvn == rhs.__PVT__macroTvn
            && __PVT__inhibitCcr == rhs.__PVT__inhibitCcr;
    }
    bool operator!=(const Vsys_top_s_irdecod__struct__0& rhs) const {
        return !(*this == rhs);
    }

    bool operator<(const Vsys_top_s_irdecod__struct__0& rhs) const {
        return std::tie(__PVT__isPcRel, __PVT__isTas, __PVT__implicitSp, __PVT__toCcr, __PVT__rxIsDt, __PVT__ryIsDt, __PVT__rxIsUsp, __PVT__rxIsMovem, __PVT__movemPreDecr, __PVT__isByte, __PVT__isMovep, __PVT__rx, __PVT__ry, __PVT__rxIsAreg, __PVT__ryIsAreg, __PVT__ftuConst, __PVT__macroTvn, __PVT__inhibitCcr)
            <  std::tie(rhs.__PVT__isPcRel, rhs.__PVT__isTas, rhs.__PVT__implicitSp, rhs.__PVT__toCcr, rhs.__PVT__rxIsDt, rhs.__PVT__ryIsDt, rhs.__PVT__rxIsUsp, rhs.__PVT__rxIsMovem, rhs.__PVT__movemPreDecr, rhs.__PVT__isByte, rhs.__PVT__isMovep, rhs.__PVT__rx, rhs.__PVT__ry, rhs.__PVT__rxIsAreg, rhs.__PVT__ryIsAreg, rhs.__PVT__ftuConst, rhs.__PVT__macroTvn, rhs.__PVT__inhibitCcr);
    }
};
template <>
struct VlIsCustomStruct<Vsys_top_s_irdecod__struct__0> : public std::true_type {};
struct Vsys_top_s_nanod__struct__0 {
    CData/*0:0*/ __PVT__permStart;
    CData/*0:0*/ __PVT__waitBusFinish;
    CData/*0:0*/ __PVT__isWrite;
    CData/*0:0*/ __PVT__busByte;
    CData/*0:0*/ __PVT__isRmc;
    CData/*0:0*/ __PVT__noLowByte;
    CData/*0:0*/ __PVT__noHighByte;
    CData/*0:0*/ __PVT__updTpend;
    CData/*0:0*/ __PVT__clrTpend;
    CData/*0:0*/ __PVT__tvn2Ftu;
    CData/*0:0*/ __PVT__const2Ftu;
    CData/*0:0*/ __PVT__ftu2Dbl;
    CData/*0:0*/ __PVT__ftu2Abl;
    CData/*0:0*/ __PVT__abl2Pren;
    CData/*0:0*/ __PVT__updPren;
    CData/*0:0*/ __PVT__inl2psw;
    CData/*0:0*/ __PVT__ftu2Sr;
    CData/*0:0*/ __PVT__sr2Ftu;
    CData/*0:0*/ __PVT__ftu2Ccr;
    CData/*0:0*/ __PVT__pswIToFtu;
    CData/*0:0*/ __PVT__ird2Ftu;
    CData/*0:0*/ __PVT__ssw2Ftu;
    CData/*0:0*/ __PVT__initST;
    CData/*0:0*/ __PVT__Ir2Ird;
    CData/*0:0*/ __PVT__auClkEn;
    CData/*0:0*/ __PVT__noSpAlign;
    CData/*2:0*/ __PVT__auCntrl;
    CData/*0:0*/ __PVT__todbin;
    CData/*0:0*/ __PVT__toIrc;
    CData/*0:0*/ __PVT__dbl2Atl;
    CData/*0:0*/ __PVT__abl2Atl;
    CData/*0:0*/ __PVT__atl2Abl;
    CData/*0:0*/ __PVT__atl2Dbl;
    CData/*0:0*/ __PVT__abh2Ath;
    CData/*0:0*/ __PVT__dbh2Ath;
    CData/*0:0*/ __PVT__ath2Dbh;
    CData/*0:0*/ __PVT__ath2Abh;
    CData/*0:0*/ __PVT__db2Aob;
    CData/*0:0*/ __PVT__ab2Aob;
    CData/*0:0*/ __PVT__au2Aob;
    CData/*0:0*/ __PVT__aob2Ab;
    CData/*0:0*/ __PVT__updSsw;
    CData/*1:0*/ __PVT__dobCtrl;
    CData/*0:0*/ __PVT__abh2reg;
    CData/*0:0*/ __PVT__abl2reg;
    CData/*0:0*/ __PVT__reg2abl;
    CData/*0:0*/ __PVT__reg2abh;
    CData/*0:0*/ __PVT__dbh2reg;
    CData/*0:0*/ __PVT__dbl2reg;
    CData/*0:0*/ __PVT__reg2dbl;
    CData/*0:0*/ __PVT__reg2dbh;
    CData/*0:0*/ __PVT__ssp;
    CData/*0:0*/ __PVT__pchdbh;
    CData/*0:0*/ __PVT__pcldbl;
    CData/*0:0*/ __PVT__pclabl;
    CData/*0:0*/ __PVT__pchabh;
    CData/*0:0*/ __PVT__rxh2dbh;
    CData/*0:0*/ __PVT__rxh2abh;
    CData/*0:0*/ __PVT__dbl2rxl;
    CData/*0:0*/ __PVT__dbh2rxh;
    CData/*0:0*/ __PVT__rxl2db;
    CData/*0:0*/ __PVT__rxl2ab;
    CData/*0:0*/ __PVT__abl2rxl;
    CData/*0:0*/ __PVT__abh2rxh;
    CData/*0:0*/ __PVT__dbh2ryh;
    CData/*0:0*/ __PVT__abh2ryh;
    CData/*0:0*/ __PVT__ryl2db;
    CData/*0:0*/ __PVT__ryl2ab;
    CData/*0:0*/ __PVT__ryh2dbh;
    CData/*0:0*/ __PVT__ryh2abh;
    CData/*0:0*/ __PVT__dbl2ryl;
    CData/*0:0*/ __PVT__abl2ryl;
    CData/*0:0*/ __PVT__rz;
    CData/*0:0*/ __PVT__rxlDbl;
    CData/*2:0*/ __PVT__aluColumn;
    CData/*1:0*/ __PVT__aluDctrl;
    CData/*0:0*/ __PVT__aluActrl;
    CData/*0:0*/ __PVT__aluInit;
    CData/*0:0*/ __PVT__aluFinish;
    CData/*0:0*/ __PVT__abd2Dcr;
    CData/*0:0*/ __PVT__dcr2Dbd;
    CData/*0:0*/ __PVT__dbd2Alue;
    CData/*0:0*/ __PVT__alue2Dbd;
    CData/*0:0*/ __PVT__dbd2Alub;
    CData/*0:0*/ __PVT__abd2Alub;
    CData/*0:0*/ __PVT__alu2Dbd;
    CData/*0:0*/ __PVT__alu2Abd;
    CData/*0:0*/ __PVT__au2Db;
    CData/*0:0*/ __PVT__au2Ab;
    CData/*0:0*/ __PVT__au2Pc;
    CData/*0:0*/ __PVT__dbin2Abd;
    CData/*0:0*/ __PVT__dbin2Dbd;
    CData/*0:0*/ __PVT__extDbh;
    CData/*0:0*/ __PVT__extAbh;
    CData/*0:0*/ __PVT__ablAbd;
    CData/*0:0*/ __PVT__ablAbh;
    CData/*0:0*/ __PVT__dblDbd;
    CData/*0:0*/ __PVT__dblDbh;
    CData/*0:0*/ __PVT__abdIsByte;

    bool operator==(const Vsys_top_s_nanod__struct__0& rhs) const {
        return __PVT__permStart == rhs.__PVT__permStart
            && __PVT__waitBusFinish == rhs.__PVT__waitBusFinish
            && __PVT__isWrite == rhs.__PVT__isWrite
            && __PVT__busByte == rhs.__PVT__busByte
            && __PVT__isRmc == rhs.__PVT__isRmc
            && __PVT__noLowByte == rhs.__PVT__noLowByte
            && __PVT__noHighByte == rhs.__PVT__noHighByte
            && __PVT__updTpend == rhs.__PVT__updTpend
            && __PVT__clrTpend == rhs.__PVT__clrTpend
            && __PVT__tvn2Ftu == rhs.__PVT__tvn2Ftu
            && __PVT__const2Ftu == rhs.__PVT__const2Ftu
            && __PVT__ftu2Dbl == rhs.__PVT__ftu2Dbl
            && __PVT__ftu2Abl == rhs.__PVT__ftu2Abl
            && __PVT__abl2Pren == rhs.__PVT__abl2Pren
            && __PVT__updPren == rhs.__PVT__updPren
            && __PVT__inl2psw == rhs.__PVT__inl2psw
            && __PVT__ftu2Sr == rhs.__PVT__ftu2Sr
            && __PVT__sr2Ftu == rhs.__PVT__sr2Ftu
            && __PVT__ftu2Ccr == rhs.__PVT__ftu2Ccr
            && __PVT__pswIToFtu == rhs.__PVT__pswIToFtu
            && __PVT__ird2Ftu == rhs.__PVT__ird2Ftu
            && __PVT__ssw2Ftu == rhs.__PVT__ssw2Ftu
            && __PVT__initST == rhs.__PVT__initST
            && __PVT__Ir2Ird == rhs.__PVT__Ir2Ird
            && __PVT__auClkEn == rhs.__PVT__auClkEn
            && __PVT__noSpAlign == rhs.__PVT__noSpAlign
            && __PVT__auCntrl == rhs.__PVT__auCntrl
            && __PVT__todbin == rhs.__PVT__todbin
            && __PVT__toIrc == rhs.__PVT__toIrc
            && __PVT__dbl2Atl == rhs.__PVT__dbl2Atl
            && __PVT__abl2Atl == rhs.__PVT__abl2Atl
            && __PVT__atl2Abl == rhs.__PVT__atl2Abl
            && __PVT__atl2Dbl == rhs.__PVT__atl2Dbl
            && __PVT__abh2Ath == rhs.__PVT__abh2Ath
            && __PVT__dbh2Ath == rhs.__PVT__dbh2Ath
            && __PVT__ath2Dbh == rhs.__PVT__ath2Dbh
            && __PVT__ath2Abh == rhs.__PVT__ath2Abh
            && __PVT__db2Aob == rhs.__PVT__db2Aob
            && __PVT__ab2Aob == rhs.__PVT__ab2Aob
            && __PVT__au2Aob == rhs.__PVT__au2Aob
            && __PVT__aob2Ab == rhs.__PVT__aob2Ab
            && __PVT__updSsw == rhs.__PVT__updSsw
            && __PVT__dobCtrl == rhs.__PVT__dobCtrl
            && __PVT__abh2reg == rhs.__PVT__abh2reg
            && __PVT__abl2reg == rhs.__PVT__abl2reg
            && __PVT__reg2abl == rhs.__PVT__reg2abl
            && __PVT__reg2abh == rhs.__PVT__reg2abh
            && __PVT__dbh2reg == rhs.__PVT__dbh2reg
            && __PVT__dbl2reg == rhs.__PVT__dbl2reg
            && __PVT__reg2dbl == rhs.__PVT__reg2dbl
            && __PVT__reg2dbh == rhs.__PVT__reg2dbh
            && __PVT__ssp == rhs.__PVT__ssp
            && __PVT__pchdbh == rhs.__PVT__pchdbh
            && __PVT__pcldbl == rhs.__PVT__pcldbl
            && __PVT__pclabl == rhs.__PVT__pclabl
            && __PVT__pchabh == rhs.__PVT__pchabh
            && __PVT__rxh2dbh == rhs.__PVT__rxh2dbh
            && __PVT__rxh2abh == rhs.__PVT__rxh2abh
            && __PVT__dbl2rxl == rhs.__PVT__dbl2rxl
            && __PVT__dbh2rxh == rhs.__PVT__dbh2rxh
            && __PVT__rxl2db == rhs.__PVT__rxl2db
            && __PVT__rxl2ab == rhs.__PVT__rxl2ab
            && __PVT__abl2rxl == rhs.__PVT__abl2rxl
            && __PVT__abh2rxh == rhs.__PVT__abh2rxh
            && __PVT__dbh2ryh == rhs.__PVT__dbh2ryh
            && __PVT__abh2ryh == rhs.__PVT__abh2ryh
            && __PVT__ryl2db == rhs.__PVT__ryl2db
            && __PVT__ryl2ab == rhs.__PVT__ryl2ab
            && __PVT__ryh2dbh == rhs.__PVT__ryh2dbh
            && __PVT__ryh2abh == rhs.__PVT__ryh2abh
            && __PVT__dbl2ryl == rhs.__PVT__dbl2ryl
            && __PVT__abl2ryl == rhs.__PVT__abl2ryl
            && __PVT__rz == rhs.__PVT__rz
            && __PVT__rxlDbl == rhs.__PVT__rxlDbl
            && __PVT__aluColumn == rhs.__PVT__aluColumn
            && __PVT__aluDctrl == rhs.__PVT__aluDctrl
            && __PVT__aluActrl == rhs.__PVT__aluActrl
            && __PVT__aluInit == rhs.__PVT__aluInit
            && __PVT__aluFinish == rhs.__PVT__aluFinish
            && __PVT__abd2Dcr == rhs.__PVT__abd2Dcr
            && __PVT__dcr2Dbd == rhs.__PVT__dcr2Dbd
            && __PVT__dbd2Alue == rhs.__PVT__dbd2Alue
            && __PVT__alue2Dbd == rhs.__PVT__alue2Dbd
            && __PVT__dbd2Alub == rhs.__PVT__dbd2Alub
            && __PVT__abd2Alub == rhs.__PVT__abd2Alub
            && __PVT__alu2Dbd == rhs.__PVT__alu2Dbd
            && __PVT__alu2Abd == rhs.__PVT__alu2Abd
            && __PVT__au2Db == rhs.__PVT__au2Db
            && __PVT__au2Ab == rhs.__PVT__au2Ab
            && __PVT__au2Pc == rhs.__PVT__au2Pc
            && __PVT__dbin2Abd == rhs.__PVT__dbin2Abd
            && __PVT__dbin2Dbd == rhs.__PVT__dbin2Dbd
            && __PVT__extDbh == rhs.__PVT__extDbh
            && __PVT__extAbh == rhs.__PVT__extAbh
            && __PVT__ablAbd == rhs.__PVT__ablAbd
            && __PVT__ablAbh == rhs.__PVT__ablAbh
            && __PVT__dblDbd == rhs.__PVT__dblDbd
            && __PVT__dblDbh == rhs.__PVT__dblDbh
            && __PVT__abdIsByte == rhs.__PVT__abdIsByte;
    }
    bool operator!=(const Vsys_top_s_nanod__struct__0& rhs) const {
        return !(*this == rhs);
    }

    bool operator<(const Vsys_top_s_nanod__struct__0& rhs) const {
        return std::tie(__PVT__permStart, __PVT__waitBusFinish, __PVT__isWrite, __PVT__busByte, __PVT__isRmc, __PVT__noLowByte, __PVT__noHighByte, __PVT__updTpend, __PVT__clrTpend, __PVT__tvn2Ftu, __PVT__const2Ftu, __PVT__ftu2Dbl, __PVT__ftu2Abl, __PVT__abl2Pren, __PVT__updPren, __PVT__inl2psw, __PVT__ftu2Sr, __PVT__sr2Ftu, __PVT__ftu2Ccr, __PVT__pswIToFtu, __PVT__ird2Ftu, __PVT__ssw2Ftu, __PVT__initST, __PVT__Ir2Ird, __PVT__auClkEn, __PVT__noSpAlign, __PVT__auCntrl, __PVT__todbin, __PVT__toIrc, __PVT__dbl2Atl, __PVT__abl2Atl, __PVT__atl2Abl, __PVT__atl2Dbl, __PVT__abh2Ath, __PVT__dbh2Ath, __PVT__ath2Dbh, __PVT__ath2Abh, __PVT__db2Aob, __PVT__ab2Aob, __PVT__au2Aob, __PVT__aob2Ab, __PVT__updSsw, __PVT__dobCtrl, __PVT__abh2reg, __PVT__abl2reg, __PVT__reg2abl, __PVT__reg2abh, __PVT__dbh2reg, __PVT__dbl2reg, __PVT__reg2dbl, __PVT__reg2dbh, __PVT__ssp, __PVT__pchdbh, __PVT__pcldbl, __PVT__pclabl, __PVT__pchabh, __PVT__rxh2dbh, __PVT__rxh2abh, __PVT__dbl2rxl, __PVT__dbh2rxh, __PVT__rxl2db, __PVT__rxl2ab, __PVT__abl2rxl, __PVT__abh2rxh, __PVT__dbh2ryh, __PVT__abh2ryh, __PVT__ryl2db, __PVT__ryl2ab, __PVT__ryh2dbh, __PVT__ryh2abh, __PVT__dbl2ryl, __PVT__abl2ryl, __PVT__rz, __PVT__rxlDbl, __PVT__aluColumn, __PVT__aluDctrl, __PVT__aluActrl, __PVT__aluInit, __PVT__aluFinish, __PVT__abd2Dcr, __PVT__dcr2Dbd, __PVT__dbd2Alue, __PVT__alue2Dbd, __PVT__dbd2Alub, __PVT__abd2Alub, __PVT__alu2Dbd, __PVT__alu2Abd, __PVT__au2Db, __PVT__au2Ab, __PVT__au2Pc, __PVT__dbin2Abd, __PVT__dbin2Dbd, __PVT__extDbh, __PVT__extAbh, __PVT__ablAbd, __PVT__ablAbh, __PVT__dblDbd, __PVT__dblDbh, __PVT__abdIsByte)
            <  std::tie(rhs.__PVT__permStart, rhs.__PVT__waitBusFinish, rhs.__PVT__isWrite, rhs.__PVT__busByte, rhs.__PVT__isRmc, rhs.__PVT__noLowByte, rhs.__PVT__noHighByte, rhs.__PVT__updTpend, rhs.__PVT__clrTpend, rhs.__PVT__tvn2Ftu, rhs.__PVT__const2Ftu, rhs.__PVT__ftu2Dbl, rhs.__PVT__ftu2Abl, rhs.__PVT__abl2Pren, rhs.__PVT__updPren, rhs.__PVT__inl2psw, rhs.__PVT__ftu2Sr, rhs.__PVT__sr2Ftu, rhs.__PVT__ftu2Ccr, rhs.__PVT__pswIToFtu, rhs.__PVT__ird2Ftu, rhs.__PVT__ssw2Ftu, rhs.__PVT__initST, rhs.__PVT__Ir2Ird, rhs.__PVT__auClkEn, rhs.__PVT__noSpAlign, rhs.__PVT__auCntrl, rhs.__PVT__todbin, rhs.__PVT__toIrc, rhs.__PVT__dbl2Atl, rhs.__PVT__abl2Atl, rhs.__PVT__atl2Abl, rhs.__PVT__atl2Dbl, rhs.__PVT__abh2Ath, rhs.__PVT__dbh2Ath, rhs.__PVT__ath2Dbh, rhs.__PVT__ath2Abh, rhs.__PVT__db2Aob, rhs.__PVT__ab2Aob, rhs.__PVT__au2Aob, rhs.__PVT__aob2Ab, rhs.__PVT__updSsw, rhs.__PVT__dobCtrl, rhs.__PVT__abh2reg, rhs.__PVT__abl2reg, rhs.__PVT__reg2abl, rhs.__PVT__reg2abh, rhs.__PVT__dbh2reg, rhs.__PVT__dbl2reg, rhs.__PVT__reg2dbl, rhs.__PVT__reg2dbh, rhs.__PVT__ssp, rhs.__PVT__pchdbh, rhs.__PVT__pcldbl, rhs.__PVT__pclabl, rhs.__PVT__pchabh, rhs.__PVT__rxh2dbh, rhs.__PVT__rxh2abh, rhs.__PVT__dbl2rxl, rhs.__PVT__dbh2rxh, rhs.__PVT__rxl2db, rhs.__PVT__rxl2ab, rhs.__PVT__abl2rxl, rhs.__PVT__abh2rxh, rhs.__PVT__dbh2ryh, rhs.__PVT__abh2ryh, rhs.__PVT__ryl2db, rhs.__PVT__ryl2ab, rhs.__PVT__ryh2dbh, rhs.__PVT__ryh2abh, rhs.__PVT__dbl2ryl, rhs.__PVT__abl2ryl, rhs.__PVT__rz, rhs.__PVT__rxlDbl, rhs.__PVT__aluColumn, rhs.__PVT__aluDctrl, rhs.__PVT__aluActrl, rhs.__PVT__aluInit, rhs.__PVT__aluFinish, rhs.__PVT__abd2Dcr, rhs.__PVT__dcr2Dbd, rhs.__PVT__dbd2Alue, rhs.__PVT__alue2Dbd, rhs.__PVT__dbd2Alub, rhs.__PVT__abd2Alub, rhs.__PVT__alu2Dbd, rhs.__PVT__alu2Abd, rhs.__PVT__au2Db, rhs.__PVT__au2Ab, rhs.__PVT__au2Pc, rhs.__PVT__dbin2Abd, rhs.__PVT__dbin2Dbd, rhs.__PVT__extDbh, rhs.__PVT__extAbh, rhs.__PVT__ablAbd, rhs.__PVT__ablAbh, rhs.__PVT__dblDbd, rhs.__PVT__dblDbh, rhs.__PVT__abdIsByte);
    }
};
template <>
struct VlIsCustomStruct<Vsys_top_s_nanod__struct__0> : public std::true_type {};

class alignas(VL_CACHE_LINE_BYTES) Vsys_top___024unit final {
  public:

    // INTERNAL VARIABLES
    Vsys_top__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vsys_top___024unit();
    ~Vsys_top___024unit();
    void ctor(Vsys_top__Syms* symsp, const char* namep);
    void dtor();
    VL_UNCOPYABLE(Vsys_top___024unit);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};

std::string VL_TO_STRING(const Vsys_top_s_clks__struct__0& obj);
std::string VL_TO_STRING(const Vsys_top_s_irdecod__struct__0& obj);
std::string VL_TO_STRING(const Vsys_top_s_nanod__struct__0& obj);

#endif  // guard
