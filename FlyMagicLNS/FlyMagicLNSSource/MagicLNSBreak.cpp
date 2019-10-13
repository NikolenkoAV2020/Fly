//-------------------------------------------------------------------------------
// MagicLNSBreak.cpp : 
// А. Николенко 22.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции
#include <FlyCore.h>		// Математические функции
#include <FlyMagicLNS.h>	//

//-------------------------------------------------------------------------------
// Проверка условия завершения численного решения задачи
bool MagicIsNeedBreak(ZSMagicLnsSolution* MS, ZCLnsNpmClaim& PTN, ZSLoopFact& PR)
{
    bool   isBreak = 1 ;
    int    Count = 0 ;
    double dP[ 4 ] ;
    double epsP[ 4 ] ;
    double dL, dT, du, da, ddL, dh, dBh, de, dw ;

	MS->MZ.DemandsKeyBreak = 0;
	MS->MZ.SecondDemandsKeyBreak = 0;

	if (MS->MZ.DemandsKey & MKN_PARAM_ORB_T) {
		dT = PTN.T - PR.T;
		if (fabs(dT)>MS->MP.epsT) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_T;
		dP[Count] = dT; epsP[Count] = MS->MP.epsT; Count++;
	}
	if (MS->MZ.DemandsKey & MKN_PARAM_ORB_a) {
		da = PTN.a - PR.a;
		if (fabs(da) > MS->MP.epsa) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_a;
		dP[Count] = da; epsP[Count] = MS->MP.epsa; Count++;
    }
	if (MS->MZ.DemandsKey & MKN_PARAM_ORB_L) {
		dL = ValiDA(PTN.L - PR.L, 1);
		if (fabs(dL) > MS->MP.epsL) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_L;
		dP[Count] = dL; epsP[Count] = MS->MP.epsL; Count++;
    }
	if (MS->MZ.DemandsKey & MKN_PARAM_ORB_dL) {
		ddL = ValiDA(PTN.dL - PR.dLs, 1);
		if (fabs(ddL) > MS->MP.epsdL) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_dL;
		dP[Count] = ddL; epsP[Count] = MS->MP.epsdL; Count++;
    }
	if (MS->MZ.DemandsKey & MKN_PARAM_ORB_u) {
		du = ValiDA(PTN.u - PR.up, 1);
		if (fabs(du) > MS->MP.epsu) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_u;
		dP[Count] = du; epsP[Count] = MS->MP.epsu; Count++;
	}	
	if (MS->MZ.ContHs(1)) {
		dh = PTN.Hs - PR.Hs ;
		if (fabs(dh)>MS->MP.epsH) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_hs;
		dP[Count] = dh; epsP[Count] = MS->MP.epsH; Count++;
    }
	if (MS->MZ.ContHmin(1)) {
		dh = PTN.Hmin - PR.hmin ;
		if (fabs(dh)>MS->MP.epsH) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_hmin;
		dP[Count] = dh; epsP[Count] = MS->MP.epsH; Count++;
    }
	if (MS->MZ.DemandsKey & MKN_PARAM_ORB_Bh) {
		dBh = ValiDA(PTN.BH - PR.Bhmin, 1);
		if (fabs(dBh) > MS->MP.epsBH) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_Bh;
		dP[Count] = dBh; epsP[Count] = MS->MP.epsBH; Count++;
    }
	if (MS->MZ.DemandsKey & MKN_PARAM_ORB_e) {
		de = PTN.e - PR.e;
		if (fabs(de) > MS->MP.epse) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_e;
		dP[Count] = de; epsP[Count] = MS->MP.epse; Count++;
    }
	if (MS->MZ.DemandsKey & MKN_PARAM_ORB_w) {
		dw = ValiDA(PTN.w - PR.w, 1);
		if (fabs(dw) > MS->MP.epsw) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_w;
		dP[Count] = dw; epsP[Count] = MS->MP.epsw; Count++;
    }
	if (MS->MZ.DemandsKey & MKN_PARAM_ORB_e1) {
		de = PTN.e1 - PR.e1;
		if (fabs(de) > MS->MP.epse / 1000) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_e1;
		dP[Count] = de; epsP[Count] = MS->MP.epse / 1000; Count++;
    }
	if (MS->MZ.DemandsKey & MKN_PARAM_ORB_e2) {
		de = PTN.e2 - PR.e2;
		if (fabs(de) > MS->MP.epse / 1000) isBreak = 0;
		else MS->MZ.DemandsKeyBreak |= MKN_PARAM_ORB_e2;
		dP[Count] = de; epsP[Count] = MS->MP.epse / 1000; Count++;
    }
    //
	if (MS->MZ.ContHs(2) || MS->MZ.ContHmin(2)) {
		dh = (PTN.VarOrb==SPO_ORB_EL ? PTN.Hmin-PR.hmin:PTN.Hs-PR.Hs) ;
		if (fabs(dh)<MS->MP.epsH) MS->MZ.SecondDemandsKeyBreak |= (MS->MZ.ContHs(2) ? MKN_PARAM_ORB_hs : MKN_PARAM_ORB_hmin);
		//thmin = PTN.VarOrb == SPO_ORB_EL ? &dh : 0 ;
		//ths   = PTN.VarOrb != SPO_ORB_EL ? &dh : 0 ;
    }
	if (MS->MZ.SecondDemandsKey & MKN_PARAM_ORB_Bh) {
		dBh = ValiDA(PTN.BH - PR.Bhmin, 1);
		if (fabs(dBh) < MS->MP.epsBH) MS->MZ.SecondDemandsKeyBreak |= MKN_PARAM_ORB_Bh;
    }
	if (MS->MZ.SecondDemandsKey & MKN_PARAM_ORB_e) {
		de = PTN.e - PR.e;
		if (fabs(de) < MS->MP.epse) MS->MZ.SecondDemandsKeyBreak |= MKN_PARAM_ORB_e;
    }
	if (MS->MZ.SecondDemandsKey & MKN_PARAM_ORB_w) {
		dw = ValiDA(PTN.w - PR.w, 1);
		if (fabs(dw) < MS->MP.epsw) MS->MZ.SecondDemandsKeyBreak |= MKN_PARAM_ORB_w;
    }

    return isBreak ;
}

//-------------------------------------------------------------------------------