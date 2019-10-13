//-------------------------------------------------------------------------------
// DemandIncrement.cpp : 
// А. Николенко 12.10.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции
#include <FlyCore.h>		// Математические функции
#include <FlyMagicLNS.h>	//

//-------------------------------------------------------------------------------
// Формирование структуры заданных параметров с учётом текущего помаха - 
// формирование ложной цели для обеспечения сходимости задачи
void DemandIncrement(ZCLnsNpmJob& MZ, ZSLnsNpmPar& MP, ZCLnsNpmClaim &PTN, ZSLoopFact& PR, ZCLnsNpmClaim &PTT)  
{
	bool isOrb_ew = (MZ.ORB()==SPO_ORB_EL || MZ.ORB()==SPO_ORB_OMIV || MZ.ORB()==SPO_ORB_DU) ;
	bool isOrb_hbh= MZ.ORB()==SPO_ORB_HBH ;

	if (MZ.DemandsKey&MKN_PARAM_ORB_T)		PTT.T = PTT.T+(PTN.T-PR.T) ;
	if (MZ.DemandsKey&MKN_PARAM_ORB_a)		PTT.a = PTT.a+(PTN.a-PR.a) ;    
	if (MZ.DemandsKey&MKN_PARAM_ORB_L)		PTT.L = ValiA(PTT.L+ValiDA(PTN.L-PR.L, 1), 1) ;
	if (MZ.DemandsKey&MKN_PARAM_ORB_dL)		PTT.dL= ValiDA(PTT.dL+ValiDA(PTN.dL-PR.dLs, 1), 1) ;   
	if (MZ.DemandsKey&MKN_PARAM_ORB_u)		PTT.u = ValiA(PTT.u + ValiDA(PTN.u - PR.up, 1), 1);
	if (MZ.DemandsKey&MKN_PARAM_ORB_hs)		PTT.Hs   = PTT.Hs+(PTN.Hs-PR.Hs) ;
	if (MZ.DemandsKey&MKN_PARAM_ORB_hmax || (MZ.SecondDemandsKey&MKN_PARAM_ORB_hmax &&
		MP.isEstimateOrb && MP.isEstimateOrb_h && isOrb_hbh))					
											PTT.Hmax = PTT.Hmin+(PTN.Hmin-PR.hmin) ; 
	if (MZ.DemandsKey&MKN_PARAM_ORB_hmin || (MZ.SecondDemandsKey&MKN_PARAM_ORB_hmin &&
		MP.isEstimateOrb && MP.isEstimateOrb_h && isOrb_hbh))				
											PTT.Hmin = PTT.Hmin+(PTN.Hmin-PR.hmin) ; 
	if (MZ.DemandsKey&MKN_PARAM_ORB_Bh || (MZ.SecondDemandsKey&MKN_PARAM_ORB_Bh &&
		MP.isEstimateOrb && MP.isEstimateOrb_Bh && isOrb_hbh)) {	
											PTT.BH = ValiDA(PTT.BH+ValiDA(PTN.BH-PR.Bhmin, 1), 1) ;   
											//if (fabs(PTT.BH-k_alfa0*Sin(PTT.BH))>= PTN.i) 
											if (fabs(PTT.BH)>= PTN.i) 
												PTT.BH = PTN.i*PTT.BH/fabs(PTT.BH);
#ifdef ESTIMATE_BH_AS_UH
											PTT.uH = PTT.uH+ValiDA(PTN.uH-PR.Uhmin, 1) ; 
#endif 
	}
	if (MZ.DemandsKey&MKN_PARAM_ORB_e || (MZ.SecondDemandsKey&MKN_PARAM_ORB_e &&
		MP.isEstimateOrb && MP.isEstimateOrb_e && isOrb_ew))		
											PTT.e  = PTT.e+(PTN.e-PR.e) ;
	if (MZ.DemandsKey&MKN_PARAM_ORB_w || (MZ.SecondDemandsKey&MKN_PARAM_ORB_w &&
		MP.isEstimateOrb && MP.isEstimateOrb_w && isOrb_ew))				
											PTT.w  = ValiA(PTT.w+ValiDA(PTN.w-PR.w, 1), 1) ;
	bool isNeedRecalc_ew = false ;
	if ((MZ.DemandsKey&MKN_PARAM_ORB_e1 || MZ.DemandsKey&MKN_PARAM_ORB_e12 || 
	    MZ.SecondDemandsKey&MKN_PARAM_ORB_e1 || MZ.SecondDemandsKey&MKN_PARAM_ORB_e12 || isOrb_ew ) &&
	    MP.isEstimateOrb && MP.isEstimateOrb_e12) {					
											PTT.e1 = PTT.e1+(PTN.e1-PR.e1) ;
											isNeedRecalc_ew = true ;
	}
	if ((MZ.DemandsKey&MKN_PARAM_ORB_e2 || MZ.DemandsKey&MKN_PARAM_ORB_e12 || 
	    MZ.SecondDemandsKey&MKN_PARAM_ORB_e2 || MZ.SecondDemandsKey&MKN_PARAM_ORB_e12 || isOrb_ew) &&
	    MP.isEstimateOrb && MP.isEstimateOrb_e12) {			
											PTT.e2 = PTT.e2+(PTN.e2-PR.e2) ;
											isNeedRecalc_ew = true ;
	}
	if (isNeedRecalc_ew) {
		PTT.e = sqrt(PTT.e1*PTT.e1+PTT.e2*PTT.e2) ;
		if (PTT.e>1e-14) PTT.w = AcosSS(PTT.e1/PTT.e, PTT.e2, 1) ;
		else PTT.w = 0.0 ;
	}
}

//-------------------------------------------------------------------------------