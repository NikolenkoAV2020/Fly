//-------------------------------------------------------------------------------
// MagicLNS.cpp: 
// �. ��������� 16.08.2018
//-------------------------------------------------------------------------------
// ������ ������� ����������
// �����������:
//   ���������� - ��
//   ��������   - ��/���
//   ����       - �������
//------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>		// �������������� �������
#include <FlyCore.h>		// �������������� �������
#include <FlyMagicLNS.h>	//

//---------------------------------------------------------------------------
ZSMagicLnsSolution* CreateMagicLnsSolution()
{
	ZSMagicLnsSolution* MS = new ZSMagicLnsSolution;
	return MS;
}

//---------------------------------------------------------------------------
void FreeMagicLnsSolution(ZSMagicLnsSolution* MS)
{
	if (MS) delete MS ;
}

//---------------------------------------------------------------------------
double  k_epsdrs= 0.01 ;	// 10 ������
double  k_stepu = 0.5 ;		// [����] ��� �������� ��������� ������
int     pru = 0 ;			// ������� ����� ������� ������ ���
int     kp  = 0 ;			// ������� ��������������� ���������
double  dtQ1, dtQ2 ;		// ��� ������� �������
double  PV1[3], PV2[3] ;	// ��� ��������� �,�,w  �� V1,V2

int MagicCalcDpDV(double a0, double e0, double i0, double w0, double u, int dN, double dV, ZSLnsNpmDiffDV& DP) ;

//------------------------------------------------------------------------------
// P1 (Pv1)	- ��������� �������� ������ �� ����� N1 (����� ������� ��������)
// P2		- ��������� �������� ������ �� ����� N2 (����� ������� ��������)
// PZ (PN)	- ��������� �������� ������ �� �������� �����
// PTN		- ��������� �������� ������ (�������)
// PTT		- ������� ��������� �������� ������ (� ������ �������� �� �������)
// U		- ������� ����������
//------------------------------------------------------------------------------
// ���� ������������� ��������� ��� ������ ���� ��� ��, �� ��������
// e � w ������������ � ������� ������� MknBuildSpo
//------------------------------------------------------------------------------
int LNSMagicBox(
	ZSMagicLnsSolution* MS,	// �������� �������� ������ 	
	ZSLoopFact    &P1,		// ��������� �������� �� ����� ������� ��������	
	ZSLoopFact    &P2,		// ��������� �������� �� ����� ������� ��������	
	ZSLoopFact    &PZ,		// ��������� �������� �� �������� �����
	ZCLnsNpmClaim &PT,		// ��������� ��������� ��������
	ZSFlightRule  &U)		// ������� ���������� - ��������� ����. ����������
{
    int rc = 0 ;

	// ���� ������ ��������� ������ ���������� ���������
	if (MS->GlobCount==1 && U.isFixedArg) {
	    if (U.u0[0] &&!U.u0[1]) U.plane=1 ; // ����� u1
		if (U.u0[1] &&!U.u0[0]) U.plane=2 ; // ����� u2
		if (U.u0[0] && U.u0[1]) U.plane=3 ; // ������ u1,u2
		if (U.u0[0] && U.u0[1] && U.u0[2]) U.plane=4; // ������ u1,u2,u3
	}
	// �������� ��������� �������� � ��������� ��������� ����� ������
	if (MS->GlobCount == 1 && PT.VarPar == SPO_VARIDPAR_dL) {
		if (MS->GlobCount == 1) {
			double  dLmv1 =(360.-PT.dL)/(PZ.Ns -1) ;
			double  dLmv2 =(360.-PT.dL)/(PZ.Ns +1) ;
			if (fabs(PT.dL)>0.5*dLmv1 || fabs(PT.dL)>0.5*dLmv2) {
				//MMessageBox("��������� �������� �������� > 1/2 ������������ ���������",2,0) ; 
				return 1 ;
			}
			if (PZ.dLs<0 && PT.dL>0 && (PZ.dLs*PT.dL)<0){
				PZ.Ns-=1;PT.N-=1;
				//MMessageBox( "�������� ����� ������ ��������� �� 1" , 2, 0) ;
			}
			if (PZ.dLs>0 && PT.dL<0){
				PZ.Ns+=1;PT.N+=1;
				//MMessageBox( "�������� ����� ������ ��������� �� 1", 2, 0) ;
	}	}	}

	// ������ ��� ��������� (h,Bh+...)
	if (PT.VarOrb==SPO_ORB_HBH) {
		if (MS->GlobCount == 1) {
#ifdef ESTIMATE_BH_AS_UH
			// ������� ������������ ����������� �������� ��������� ������
			// ����������� ������ � ��� ��������� �� ������ ��������
			// ������� �� ���������� �� �� �������� Bh � �� �������� uh
			PT.uH =-999.0 ;
#endif
	}	}

	// ������ ����������
	switch ( PT.VarKor ) {
		// ������ ���������� ���������� ��� 1-���������� ���������
		case 10:  rc = MagicCalcU10 (MS, P1, PZ, PT, U) ; break ;
		case 11:  rc = MagicCalcU11 (MS, P1, PZ, PT, U); break;
		case 12:  rc = MagicCalcU12 (MS, P1, PZ, PT, U); break;
		case 13:  rc = MagicCalcU13 (MS, P1, PZ, PT, U); break;
		case 14:  rc = MagicCalcU14 (MS, P1, PZ, PT, U); break;
		case 15:  rc = MagicCalcU15 (MS, P1, PZ, PT, U); break;
		case 16:  rc = MagicCalcU16 (MS, P1, PZ, PT, U); break;
		case 17:  rc = MagicCalcU17 (MS, P1, PZ, PT, U); break;
		case 18:  rc = MagicCalcU18 (MS, P1, PZ, PT, U); break;

		// ������ ���������� ���������� ��� 2-���������� ���������
		case 20:  rc = MagicCalcU20 (MS, P1, P2, PZ, PT, U); break;
		case 21:  rc = MagicCalcU21 (MS, P1, P2, PZ, PT, U); break;
		case 22:  rc = MagicCalcU22 (MS, P1, P2, PZ, PT, U); break;
		case 23:  rc = MagicCalcU23 (MS, P1, P2, PZ, PT, U); break;
		case 24:  rc = MagicCalcU24 (MS, P1, P2, PZ, PT, U); break;
		case 241: rc = MagicCalcU241(MS, P1, P2, PZ, PT, U); break;
		case 25:  rc = MagicCalcU25 (MS, P1, P2, PZ, PT, U); break;
		case 26:  rc = MagicCalcU26 (MS, P1, P2, PZ, PT, U); break;
		case 27:  rc = MagicCalcU27 (MS, P1, P2, PZ, PT, U); break;
		case 28:  rc = MagicCalcU28 (MS, P1, P2, PZ, PT, U); break;
		case 29:  rc = MagicCalcU29 (MS, P1, P2, PZ, PT, U); break;

		// ������ ���������� ���������� ��� 3-���������� ���������
		case 30:  rc = MagicCalcU30 (MS, P1, PZ, PT, U); break;
		case 31:  rc = MagicCalcU31 (MS, P1, PZ, PT, U); break;
		case 311: rc = MagicCalcU311(MS, P1, PZ, PT, U); break;
		case 32:  rc = MagicCalcU32 (MS, P1, PZ, PT, U); break;
		case 33:  rc = MagicCalcU33 (MS, P1, PZ, PT, U); break;

		default:	return-100 ;
	}

	// ����������� ������������ ������ �� � ������ �������� ������������� 
	if (!rc) { 
		double M = MS->MZ.M();
		for (int i=0 ; i<U.n ; i++ ) { 
			double dVS = fabs(U.dV[i]) ;
			rc = ZFPUCalcTimeMain(2, M, MS->DU, NULL, &dVS, &U.dt[i], &U.dts[i], NULL, &M, U.tm[i], NULL); RRC;
	}	}
	return rc ;
}

//------------------------------------------------------------------------------
void DefaultParam(ZSLnsNpmPar& P) {
  P.maxIter = 50;
  P.epsT    = 0.001;   // ���
  P.epsL    = 0.001;   // ����
  P.epsdL   = 0.0001;  // ���� / ���
  P.epsu    = 0.001;   // ����
  P.epsH    = 0.0001;  // ��
  P.epsBH   = 0.01;    // ����
  P.epsa    = 0.00001; // ��
  P.epse    = 0.0001;  //
  P.epsw    = 0.1;     // ����
  P.isEstimateOrb = 0;
  P.isEstimateOrb_e12 = 1;
  P.isEstimateOrb_e   = 1;
  P.isEstimateOrb_w   = 1;
  P.isEstimateOrb_h   = 1;
  P.isEstimateOrb_Bh  = 1;
  //
  P.Epoha     = 2 ;
  //
  P.ViewDimT  = 0 ;
  P.ViewDimL  = 1 ;
  P.ViewDimdL = 1 ;
  P.ViewDimw  = 1 ;
  P.ViewDimBh = 1 ;
  P.ViewDimu  = 1 ;
  P.ViewDimdV = 0 ;
  P.ViewDimdt = 0 ;
  //
  P.RezSaveNU          = 2 ;
  P.RezSaveNUNom       = 1 ;
  P.RezSaveNUMod       = 0 ;
  P.RezSavePULast      = 1 ;
  P.RezUseLHS          = 0 ;
  P.RezSaveFOFrom      = 1 ;
  P.RezSaveFODbl       = 0 ;
  P.RezSaveDocFull     = 1 ;
  P.RezSaveDocDsk      = 1 ;
  P.RezSaveDocDU       = 0 ;
  P.RezSaveDocLHS      = 1 ;
  P.RezSaveDocLHSShort = 0 ;
  P.RezSaveDocINU      = 1 ;
  P.RezSaveDocINUShort = 0 ;
  P.RezSaveDocNUR      = 1 ;
  P.RezSaveDocNURShort = 0 ;
  P.RezSaveDocRV       = 1 ;
  P.RezSaveDocPU       = 1 ;
  P.RezSaveDocFO       = 1 ;
  P.RezSaveDocSVER     = 1 ;
  //
  P.RezUseLHSNum       = 0 ;
} ;

//------------------------------------------------------------------------------
void operator << (ZSLoopFact& P, ZSC_VI& FV) 
{
	ZSC_VP* Hmin = HMIN_VI(FV) ;
	ZSC_VP* Hmax = HMAX_VI(FV) ;

	P.Vit   = FV.FSUN.VitTE ;
	P.t     = FV.FSUN.ta ;
	P.Dt    = FV.FSUN.Data ;
	P.Tt    = FV.FSUN.Time ;
	P.T		= FV.Td ;			// ������ ������������� [���]
	P.Tss	= FV.Td ;			// ������ ������������� �������������� [���]
	P.dTv	= 0  ;				// ������� �������������� ������� �� ����� [���]
	P.Hs	= FV.HAvr ;         // ������� ������ ������ �� ����� [��]
	P.dLs	= FV.dLs ;			// �������� �������� ������ ����� [����]
	P.dLv	= FV.dL ;			// ����������� ���������� [����]
	P.L		= FV.UN.L ;			// ������� ������ ����� [����]
	P.Ns	= FV.Ks ;			// ��������� ������
	P.Nvs	= 1 ;				// ����� ��������� �����
	P.hmin	= Hmin->H ;			// ����������� ������ [��]
	P.Bhmin = Hmin->B ;			// ������ ����������� ������ [����]
	P.Uhmin = Hmin->ute ;		// �������� ������ ����������� ������ [����]
	P.hmax  = Hmax->H ;			// ����������� ������ [��]
	P.Bhmax = Hmax->B ;			// ������ ����������� ������ [����]
	P.Uhmax = Hmax->ute ;		// �������� ������ ������������ ������ [����]
	P.a		= TO_KM(FV.FSUN.OSKTE[0]) ;		// ������� ������� �� ������ ������ ����� [��]
	P.e		= FV.FSUN.OSKTE[1] ;			// �������������� �� ������ ������ �����
	P.w		= GradA(FV.FSUN.OSKTE[4]) ;		// ��������� ������� �� ������ ������ ����� [����]
	P.e1	= P.e*cos(FV.FSUN.OSKTE[4]) ;	//
	P.e2	= P.e*sin(FV.FSUN.OSKTE[4]) ;	//
	P.i		= GradA(FV.FSUN.OSKTE[2]) ;		// ���������� �� ������ ������ �����
	P.u		= 0 ;

	// ������� � ������� ������
    MagicCalcR(P.i, P.a*(1-P.e*P.e), P.e1, P.e2, P.Ro) ; 
}

void operator << (ZSLoopFact& P, ZSC_VI* FV) { P << *FV ; }

//------------------------------------------------------------------------------
