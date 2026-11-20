// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsys_top.h for the primary calling header

#include "Vsys_top__pch.h"

std::string VL_TO_STRING(const Vsys_top_s_clks__struct__0& obj) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vsys_top___024unit::VL_TO_STRING\n"); );
    // Body
    std::string out;
    out += "'{clk:" + VL_TO_STRING(obj.__PVT__clk);
    out += ", extReset:" + VL_TO_STRING(obj.__PVT__extReset);
    out += ", pwrUp:" + VL_TO_STRING(obj.__PVT__pwrUp);
    out += ", enPhi1:" + VL_TO_STRING(obj.__PVT__enPhi1);
    out += ", enPhi2:" + VL_TO_STRING(obj.__PVT__enPhi2);
    out += "}";
    return (out);
}

std::string VL_TO_STRING(const Vsys_top_s_irdecod__struct__0& obj) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vsys_top___024unit::VL_TO_STRING\n"); );
    // Body
    std::string out;
    out += "'{isPcRel:" + VL_TO_STRING(obj.__PVT__isPcRel);
    out += ", isTas:" + VL_TO_STRING(obj.__PVT__isTas);
    out += ", implicitSp:" + VL_TO_STRING(obj.__PVT__implicitSp);
    out += ", toCcr:" + VL_TO_STRING(obj.__PVT__toCcr);
    out += ", rxIsDt:" + VL_TO_STRING(obj.__PVT__rxIsDt);
    out += ", ryIsDt:" + VL_TO_STRING(obj.__PVT__ryIsDt);
    out += ", rxIsUsp:" + VL_TO_STRING(obj.__PVT__rxIsUsp);
    out += ", rxIsMovem:" + VL_TO_STRING(obj.__PVT__rxIsMovem);
    out += ", movemPreDecr:" + VL_TO_STRING(obj.__PVT__movemPreDecr);
    out += ", isByte:" + VL_TO_STRING(obj.__PVT__isByte);
    out += ", isMovep:" + VL_TO_STRING(obj.__PVT__isMovep);
    out += ", rx:" + VL_TO_STRING(obj.__PVT__rx);
    out += ", ry:" + VL_TO_STRING(obj.__PVT__ry);
    out += ", rxIsAreg:" + VL_TO_STRING(obj.__PVT__rxIsAreg);
    out += ", ryIsAreg:" + VL_TO_STRING(obj.__PVT__ryIsAreg);
    out += ", ftuConst:" + VL_TO_STRING(obj.__PVT__ftuConst);
    out += ", macroTvn:" + VL_TO_STRING(obj.__PVT__macroTvn);
    out += ", inhibitCcr:" + VL_TO_STRING(obj.__PVT__inhibitCcr);
    out += "}";
    return (out);
}

std::string VL_TO_STRING(const Vsys_top_s_nanod__struct__0& obj) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vsys_top___024unit::VL_TO_STRING\n"); );
    // Body
    std::string out;
    out += "'{permStart:" + VL_TO_STRING(obj.__PVT__permStart);
    out += ", waitBusFinish:" + VL_TO_STRING(obj.__PVT__waitBusFinish);
    out += ", isWrite:" + VL_TO_STRING(obj.__PVT__isWrite);
    out += ", busByte:" + VL_TO_STRING(obj.__PVT__busByte);
    out += ", isRmc:" + VL_TO_STRING(obj.__PVT__isRmc);
    out += ", noLowByte:" + VL_TO_STRING(obj.__PVT__noLowByte);
    out += ", noHighByte:" + VL_TO_STRING(obj.__PVT__noHighByte);
    out += ", updTpend:" + VL_TO_STRING(obj.__PVT__updTpend);
    out += ", clrTpend:" + VL_TO_STRING(obj.__PVT__clrTpend);
    out += ", tvn2Ftu:" + VL_TO_STRING(obj.__PVT__tvn2Ftu);
    out += ", const2Ftu:" + VL_TO_STRING(obj.__PVT__const2Ftu);
    out += ", ftu2Dbl:" + VL_TO_STRING(obj.__PVT__ftu2Dbl);
    out += ", ftu2Abl:" + VL_TO_STRING(obj.__PVT__ftu2Abl);
    out += ", abl2Pren:" + VL_TO_STRING(obj.__PVT__abl2Pren);
    out += ", updPren:" + VL_TO_STRING(obj.__PVT__updPren);
    out += ", inl2psw:" + VL_TO_STRING(obj.__PVT__inl2psw);
    out += ", ftu2Sr:" + VL_TO_STRING(obj.__PVT__ftu2Sr);
    out += ", sr2Ftu:" + VL_TO_STRING(obj.__PVT__sr2Ftu);
    out += ", ftu2Ccr:" + VL_TO_STRING(obj.__PVT__ftu2Ccr);
    out += ", pswIToFtu:" + VL_TO_STRING(obj.__PVT__pswIToFtu);
    out += ", ird2Ftu:" + VL_TO_STRING(obj.__PVT__ird2Ftu);
    out += ", ssw2Ftu:" + VL_TO_STRING(obj.__PVT__ssw2Ftu);
    out += ", initST:" + VL_TO_STRING(obj.__PVT__initST);
    out += ", Ir2Ird:" + VL_TO_STRING(obj.__PVT__Ir2Ird);
    out += ", auClkEn:" + VL_TO_STRING(obj.__PVT__auClkEn);
    out += ", noSpAlign:" + VL_TO_STRING(obj.__PVT__noSpAlign);
    out += ", auCntrl:" + VL_TO_STRING(obj.__PVT__auCntrl);
    out += ", todbin:" + VL_TO_STRING(obj.__PVT__todbin);
    out += ", toIrc:" + VL_TO_STRING(obj.__PVT__toIrc);
    out += ", dbl2Atl:" + VL_TO_STRING(obj.__PVT__dbl2Atl);
    out += ", abl2Atl:" + VL_TO_STRING(obj.__PVT__abl2Atl);
    out += ", atl2Abl:" + VL_TO_STRING(obj.__PVT__atl2Abl);
    out += ", atl2Dbl:" + VL_TO_STRING(obj.__PVT__atl2Dbl);
    out += ", abh2Ath:" + VL_TO_STRING(obj.__PVT__abh2Ath);
    out += ", dbh2Ath:" + VL_TO_STRING(obj.__PVT__dbh2Ath);
    out += ", ath2Dbh:" + VL_TO_STRING(obj.__PVT__ath2Dbh);
    out += ", ath2Abh:" + VL_TO_STRING(obj.__PVT__ath2Abh);
    out += ", db2Aob:" + VL_TO_STRING(obj.__PVT__db2Aob);
    out += ", ab2Aob:" + VL_TO_STRING(obj.__PVT__ab2Aob);
    out += ", au2Aob:" + VL_TO_STRING(obj.__PVT__au2Aob);
    out += ", aob2Ab:" + VL_TO_STRING(obj.__PVT__aob2Ab);
    out += ", updSsw:" + VL_TO_STRING(obj.__PVT__updSsw);
    out += ", dobCtrl:" + VL_TO_STRING(obj.__PVT__dobCtrl);
    out += ", abh2reg:" + VL_TO_STRING(obj.__PVT__abh2reg);
    out += ", abl2reg:" + VL_TO_STRING(obj.__PVT__abl2reg);
    out += ", reg2abl:" + VL_TO_STRING(obj.__PVT__reg2abl);
    out += ", reg2abh:" + VL_TO_STRING(obj.__PVT__reg2abh);
    out += ", dbh2reg:" + VL_TO_STRING(obj.__PVT__dbh2reg);
    out += ", dbl2reg:" + VL_TO_STRING(obj.__PVT__dbl2reg);
    out += ", reg2dbl:" + VL_TO_STRING(obj.__PVT__reg2dbl);
    out += ", reg2dbh:" + VL_TO_STRING(obj.__PVT__reg2dbh);
    out += ", ssp:" + VL_TO_STRING(obj.__PVT__ssp);
    out += ", pchdbh:" + VL_TO_STRING(obj.__PVT__pchdbh);
    out += ", pcldbl:" + VL_TO_STRING(obj.__PVT__pcldbl);
    out += ", pclabl:" + VL_TO_STRING(obj.__PVT__pclabl);
    out += ", pchabh:" + VL_TO_STRING(obj.__PVT__pchabh);
    out += ", rxh2dbh:" + VL_TO_STRING(obj.__PVT__rxh2dbh);
    out += ", rxh2abh:" + VL_TO_STRING(obj.__PVT__rxh2abh);
    out += ", dbl2rxl:" + VL_TO_STRING(obj.__PVT__dbl2rxl);
    out += ", dbh2rxh:" + VL_TO_STRING(obj.__PVT__dbh2rxh);
    out += ", rxl2db:" + VL_TO_STRING(obj.__PVT__rxl2db);
    out += ", rxl2ab:" + VL_TO_STRING(obj.__PVT__rxl2ab);
    out += ", abl2rxl:" + VL_TO_STRING(obj.__PVT__abl2rxl);
    out += ", abh2rxh:" + VL_TO_STRING(obj.__PVT__abh2rxh);
    out += ", dbh2ryh:" + VL_TO_STRING(obj.__PVT__dbh2ryh);
    out += ", abh2ryh:" + VL_TO_STRING(obj.__PVT__abh2ryh);
    out += ", ryl2db:" + VL_TO_STRING(obj.__PVT__ryl2db);
    out += ", ryl2ab:" + VL_TO_STRING(obj.__PVT__ryl2ab);
    out += ", ryh2dbh:" + VL_TO_STRING(obj.__PVT__ryh2dbh);
    out += ", ryh2abh:" + VL_TO_STRING(obj.__PVT__ryh2abh);
    out += ", dbl2ryl:" + VL_TO_STRING(obj.__PVT__dbl2ryl);
    out += ", abl2ryl:" + VL_TO_STRING(obj.__PVT__abl2ryl);
    out += ", rz:" + VL_TO_STRING(obj.__PVT__rz);
    out += ", rxlDbl:" + VL_TO_STRING(obj.__PVT__rxlDbl);
    out += ", aluColumn:" + VL_TO_STRING(obj.__PVT__aluColumn);
    out += ", aluDctrl:" + VL_TO_STRING(obj.__PVT__aluDctrl);
    out += ", aluActrl:" + VL_TO_STRING(obj.__PVT__aluActrl);
    out += ", aluInit:" + VL_TO_STRING(obj.__PVT__aluInit);
    out += ", aluFinish:" + VL_TO_STRING(obj.__PVT__aluFinish);
    out += ", abd2Dcr:" + VL_TO_STRING(obj.__PVT__abd2Dcr);
    out += ", dcr2Dbd:" + VL_TO_STRING(obj.__PVT__dcr2Dbd);
    out += ", dbd2Alue:" + VL_TO_STRING(obj.__PVT__dbd2Alue);
    out += ", alue2Dbd:" + VL_TO_STRING(obj.__PVT__alue2Dbd);
    out += ", dbd2Alub:" + VL_TO_STRING(obj.__PVT__dbd2Alub);
    out += ", abd2Alub:" + VL_TO_STRING(obj.__PVT__abd2Alub);
    out += ", alu2Dbd:" + VL_TO_STRING(obj.__PVT__alu2Dbd);
    out += ", alu2Abd:" + VL_TO_STRING(obj.__PVT__alu2Abd);
    out += ", au2Db:" + VL_TO_STRING(obj.__PVT__au2Db);
    out += ", au2Ab:" + VL_TO_STRING(obj.__PVT__au2Ab);
    out += ", au2Pc:" + VL_TO_STRING(obj.__PVT__au2Pc);
    out += ", dbin2Abd:" + VL_TO_STRING(obj.__PVT__dbin2Abd);
    out += ", dbin2Dbd:" + VL_TO_STRING(obj.__PVT__dbin2Dbd);
    out += ", extDbh:" + VL_TO_STRING(obj.__PVT__extDbh);
    out += ", extAbh:" + VL_TO_STRING(obj.__PVT__extAbh);
    out += ", ablAbd:" + VL_TO_STRING(obj.__PVT__ablAbd);
    out += ", ablAbh:" + VL_TO_STRING(obj.__PVT__ablAbh);
    out += ", dblDbd:" + VL_TO_STRING(obj.__PVT__dblDbd);
    out += ", dblDbh:" + VL_TO_STRING(obj.__PVT__dblDbh);
    out += ", abdIsByte:" + VL_TO_STRING(obj.__PVT__abdIsByte);
    out += "}";
    return (out);
}
