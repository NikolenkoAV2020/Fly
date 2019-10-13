//-------------------------------------------------------------------------------
// ZMForceAtm.cpp
// �. ��������� 03.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ZHBL.h>
#include <FlyCoreSource\\ZCISAManager.h>
#include <FlyCoreSource\\ZMForceAtmSMA81.h>
#include <FlyCoreSource\\ZMForceAtmSMA62.h>
#include <FlyCoreSource\\ZMForceAtmMsis.h>
#include <FlyCoreSource\\ZMForceAtmDMAGost.h>
//
// ���� 25645.101-83		"��������� ����� �������..."
// ���� 4401-81				"������������������ ��������. ��������� �����������"
// ���� � 25645.000-2001	"��������� ����� �������..." - 4���� 
// ���� � 25645.166-2004	"��������� ����� �������..." - 4���� 
// ���� � 25645.___-2010	"��������� ����� �������..." - 4���� 

//-------------------------------------------------------------------------------
// ������ ��������� �� ��������� ��������� 
int ZFCalcJAtm(double ta, double Ro, double Sb, double* Vg, double* UAsk) ;

//-------------------------------------------------------------------------------
// ��������� �����������
int ZFCalcSAtm(double t, double* Rgsk, ZLSF& LSF, ZNU& NU, ZMAtmSMA* pAtm, double* UAsk)
{
	// �������� ����������
	memset(UAsk, 0, 3*sizeof(double)) ;
	// ������ ��������� ������� ���� ������
	if (!LSF.vatm) return-21 ;

	double* Rg=&Rgsk[0] ;	// ���������� � ��������
	double* Vg=&Rgsk[3] ;	// ��������� � ��������
	int     rc=0 ;

	// �������������� ����������� ������ ���� ����� ����� ������
	// ������������ ����������� ��������� �� ����� ������
	if (NU.Sb<=0) {
		// �� ����� �������������� ����������� � �� 
		AddErrorMSG( "�� ����� �������������� ����������� � ��." ) ;
		return-22 ;
	}
	if (!pAtm) {
		AddErrorMSG( "������ ����� ������������� ���������. �� ������ ������ ��� �������� ��������� ���������" ) ;
		return 1 ;
	} 

	// ��������� � ��/�3 �� �������� ������ (LSF.vatm) 
	double Ro ;  
	rc = pAtm->Calc(Rg, Ro) ; RRC ;
/*
	switch(LSF.vatm) {
	case ATMSMA81: {
			// ���-81
			// ������ ��������� � ��/�3  
			// ��� ��������� ��������� ��������� �� ������ 270 �� 
			// �� ���� 25645.101-83	"��������� ����� �������..." 
			//		�� 1,2e-11 �� 3,7e-11 ��/�3 (��� F0=65)
			// �� ���� 4401-81		"������������������ ��������. ��������� �����������" 
			//		3.73422e-11 ��/�3
			// 
			// � ����� ���������� ��������� ������ �������� ��� ������ 270 ��
			//		3.5511e-11 ��/�3
			ZModAtmSMA81* AtmSMA81 = (ZModAtmSMA81*)pAtm ;
			rc = AtmSMA81->Calc(Rg, Ro) ; RRC ;
			} break ;
		case ATMSMA62: {
			// ��������� ��-62
			// ��� ���������� ���-81
			// � ����� ���������� ��������� ������ �������� ��� ������ 270 ��
			//		7.6436e-011 ��/�3
			//		7.6410e-011 ��/�3
			//		��� ���� ���������... � ��� ������� �� ��������
			ZModAtmSMA62* AtmSMA62 = (ZModAtmSMA62*)pAtm ;
			rc = AtmSMA62->Calc(Rg, Ro) ; RRC ;
			} break ;
		case ATMSMA440181: { 
			// ��������� ��� ���� 4401.81
			ZModAtmSMA440181* AtmSMA440181 = (ZModAtmSMA440181*)pAtm ;
			rc = AtmSMA440181->Calc(Rg, Ro) ; RRC ;
			} break ;
		default: assert(0) ;
	}
*/	
	rc = ZFCalcJAtm(t, Ro, NU.Sb, Vg, UAsk);
	return rc ;
}

//-------------------------------------------------------------------------------
// ��������� ������������
int ZFCalcDAtm(double t, double* Rgsk, ZLSF& LSF, ZNU& NU, ZCISAManager* ISAManager, double* UAsk, double* atmro)
{
	if (!LSF.vatm) return 0 ;
	if (UAsk) memset(UAsk, 0, 3*sizeof(double)) ;

	double* Rg=&Rgsk[0] ;
	double* Vg=&Rgsk[3] ;
	int     rc=0 ;

	if (NU.Sb <= 0) {
		// �� ����� �������������� ����������� � �� 
		AddErrorMSG( "�� ����� �������������� ����������� � ��." ) ;
		return 1 ;
	}
	if (!ISAManager) {
		AddErrorMSG( "������ ����� ������������� ���������. �� ������ ��������� �������� ��" ) ;
		return 1 ;
	} 

	// ��������� � ��/�3  
	double Ro ;  

	switch(LSF.vatm) {
		case ATMMSIS1990: 
		case ATMMSIS2000: {
			double HBL[3] ;
			ZGRtoHBL(Rgsk, HBL, 0, 0, nullptr, nullptr) ;
			HBL[0] = TO_KM(HBL[0]) ;
			HBL[1] = TO_GRAD(HBL[1]) ;
			HBL[2] = TO_GRAD(HBL[2]) ;
			rc = CalcModAtm_DMA_MSIS(LSF.vatm, t, HBL, ISAManager, &Ro) ; }
			break ;
		case ATMDMAGOST1990: 
		case ATMDMAGOST2001: 
		case ATMDMAGOST2004: 
			rc = CalcModAtm_DMA_GOST(LSF.vatm, t, Rgsk, ISAManager, &Ro) ; 
			break ;
		case ATMDMAGOST1977: 
		case ATMDMAGOST1984: 
		default: assert(0) ;
	}
	RRC ;

	if (atmro) *atmro = Ro ; 
	if (UAsk) rc = ZFCalcJAtm(t, Ro, NU.Sb, Vg, UAsk) ;

	return rc ;
}

//-------------------------------------------------------------------------------
int ZFCalcJAtm(double ta, double Ro, double Sb, double* Vg, double* UAsk)
{
	// ������ ������������ ���������
	double J ;
	// ������ �������� 
	double Vm = FR(Vg) ;

	// ������ ����������� ��������� � ���
	//   - �������� NU.Sb = Cx*Sm / (2*m) - ����������� �2/��
	//     ������ � ����� ������������ Sb ������ ��������� � ��
	//	 - ����� �������, ��� ���� �� ������� ��������� ����� ������� �� �����
	//	   (�������� ����� ��� ����������� �������� ��������� �������) ��
	//	   � Sb ��� �� ��� ���� ��������.....  
	//   - ��������� �� 1e7 ���� �������� ��������� � ����/���2  
	//	   (Ro � Sb ������ � ������ ������� ���������� ��������� 1e7)
	J = 1e7*Ro*Vm*Sb ;
	// ���������� ��������� � ���
	// !!!! �� �������� ��������� ����� ����� ��������� (-Vg[i]) 
	// ������ ��� ��������� ������������� ���� ���������� ������ �������� ��
	double Jg[3] ;
	for (int i=0 ; i<3 ; i++) Jg[i] =-Vg[i]*J ;
	int rc = ZGSCtoASC1975(ta, Jg, UAsk, 1) ; 
	return rc ;
}

//-------------------------------------------------------------------------------

