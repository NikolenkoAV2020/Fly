//-------------------------------------------------------------------------------
// DemandInit.cpp : 
// �. ��������� 12.10.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>		// �������������� �������
#include <FlyCore.h>		// �������������� �������
#include <FlyMagicLNS.h>	//

//-------------------------------------------------------------------------------
// ������������ ��������� ��������� �������� ����������.
// �������� ������������� ���������� � ���������� �� �������� �����.
//		PN	- ��������� ������. ��������� (��������) �� �������� �����.
//		PTN	- ��������� ��������� ���������.
void DemandInit(ZCLnsNpmJob& MZ, ZSLoopFact& PN, ZCLnsNpmClaim &PTN) 
{ 
    PTN.Clr( ) ;
    PTN.VarKor = MZ.TASK() ;	// �������� ����� ��������: 11,12,13,...,20,21,...,32,33
    PTN.VarOrb = MZ.ORB() ;		// ������: ������ (0), ���� (1), �� (2)
    PTN.Vit    = MZ.VIT() ;		// �������� ������
    PTN.VarH = 0 ;				// ������: ����������� (0), ������������ (1)
    PTN.VarB = 0 ;				// �����: ���������� (0), ���������� (1)
    if (MZ.ContL   ()) { PTN.L    = MZ.PARL() ; }
    if (MZ.ContT   ()) { PTN.T    = MZ.PART() ; }
    if (MZ.ContDL  ()) { PTN.dL   = MZ.PARdL() ; PTN.N = PN.Ns ; }
    if (MZ.ContHs  ()) { PTN.Hs   = MZ.PARhs() ; }
    if (MZ.ContHmin()) { PTN.Hmin = MZ.PARh() ; }
    if (MZ.ContHmax()) { PTN.Hmax = MZ.PARh() ; }
    //if (MZ.ContH   ()) { PTN.H  = MZ.PARh() ; ( MZ.ORB() == SPO_ORB_EL ? thmin : thmax ) = &PTN.H ; }
    if (MZ.ContBH  ()) { PTN.BH   = MZ.PARBh() ; PTN.VarB = MZ.PARBv( ) ; }
	if (MZ.Contu()) { 
		PTN.u = MZ.PARu(); 
		PTN.t = MZ.PARt();
	}
    if (MZ.ContA   ()) { PTN.a    = MZ.PARa() ; }
	if (MZ.ORB()==SPO_ORB_EL) {
		// ���� ������ ��������� - ������ ������ � ������� ��������� ������,
		// �� ���� �������������� � ��������� �������, ��...
		bool ise = 0, isw = 0 ;
		if (MZ.ContE()) { 
			PTN.e=MZ.PARe() ; ise=1 ; 
		}
		if (MZ.ContW()) { 
			PTN.w=MZ.PARw() ; isw=1 ; 
		}
		if (ise && isw) {
			// ...��� ������ ���������� ��������� � �������� �������������� ����������
			// ��������� ���������� ������� ���������������
			if (MZ.SecondDemandsKey&MKN_PARAM_ORB_e && MZ.SecondDemandsKey&MKN_PARAM_ORB_w) {
				MZ.SecondDemandsKey&=~(MKN_PARAM_ORB_e|MKN_PARAM_ORB_w) ;
				MZ.SecondDemandsKey|= MKN_PARAM_ORB_e1|MKN_PARAM_ORB_e2 ;
			} else {
				MZ.DemandsKey&=~(MKN_PARAM_ORB_e|MKN_PARAM_ORB_w) ;
				MZ.DemandsKey|= MKN_PARAM_ORB_e1|MKN_PARAM_ORB_e2 ;
			}
			PTN.e1 = PTN.e*Cos(PTN.w) ; PTN.e2 = PTN.e*Sin(PTN.w) ;
		}
	}
    // ���������� � ���������� ��������� �� ���������� � ������ �������� ����������
    // �������������� �� �������� �����
    PTN.i = PN.i ;
}

//-------------------------------------------------------------------------------