//--------------------------------------------------------------------------------
// ZMForceAtmDMAGost.CPP
// ���������� ��������� ������������ ��������� �� ������,
// ���������� �������.
// �. ��������� 07.11.2018.
//--------------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ZCISAManager.h>
#include <FlyCoreSource\\ZMForceAtmDMA1990_Gost25645115.h>
#include <FlyCoreSource\\ZMForceAtmDMA2001_Gost25645000.h>
#include <FlyCoreSource\\ZMForceAtmDMA2004_Gost25645166.h>
#include <FlyCoreSource\\ZCalc.h>

//---------------------------------------------------------------------------
int CalcModAtm_DMA_GOST(
	VersionATM    vatm,			// ������� ������������ ������ ��������� (����) 
	double        AtomicTime,	// ������� ����� �� J1975, [���]
	double*       XYZg,			// ���������� �� � ��������, [����]
	ZCISAManager* ISAManager,	// ��������� �������� ��
	double*       pdPlotAtm)	// ������� ��������� ���������, [��/�3]
{
	if (!ISAManager) assert(0) ;
	if (vatm!= ATMDMAGOST1990 && vatm!=ATMDMAGOST2001 && vatm!= ATMDMAGOST2004) assert(0) ;

	int rc = 0 ;

	// ����������� �������� ��������� ���������� �� ������ ������� ta
	ISAManager->UpdateToTime(AtomicTime, 1.7) ; 
	// ���������� ����������� ������� F107 - �� ���� ���������� F81
	ISAManager->AverageF107t0F81(AtomicTime, 81, 1.7) ; 

	// ������� ��������� ����������
	ISA_DMA ISA ;
	ISA.F107   = ISAManager->F107() ;
	ISA.F81    = ISAManager->F81() ;
	ISA.Kp     = ISAManager->KP() ;
	ISA.flag3H = false ;

	// ���������� ��������� ������
	rc = _CalcAstroKoord(AtomicTime, 0) ; RRC ;
	// ����������� ���������� ������ � ���
	double SUNg[3] ;
	rc = ZASC1975toGSC(AtomicTime, Wae->qso, SUNg, true) ; RRC ;

	// ��������� ���� 
	double JD = ZTime::AtomicTimetoMJD(AtomicTime)+ZMJD0; 

	// ���������� ��
	double Rg[3] ; 
	// �������������� ��������� �� � ������ �� ���� � ��,
	// ��� ��� ��������� ��� �-�� DMA_GOST_...
	for (int i=0; i<3; i++) { SUNg[i]*= 1e4 ; Rg[i] = XYZg[i]*1e4 ;	}
	 
	// ������ ���������� ���������
	switch (vatm) {
		case ATMDMAGOST1977:
		case ATMDMAGOST1984:
			assert(0) ;
		case ATMDMAGOST2001:
			// ������ ��������� ���-2000
			rc = DMA_GOST_25645_000_2001(JD, ISA, SUNg, Rg, *pdPlotAtm) ;
			break ;
		case ATMDMAGOST2004: 
		case ATMDMAGOST1990: {
			double SUNte[3] ;
			// ���������� ������ � ��� ������� �����
			rc = ZASC_1975toTE(AtomicTime, Wae->qso, SUNte, false) ; RRC ;
			// �������������� ��������� ������ �� ���� � ��
			for (int i=0; i<3; i++) SUNte[i]*= 1e4 ;
			if (vatm==ATMDMAGOST1990) {
				// ������ ��������� ���-1990
				rc = DMA_GOST_25645_115_1990(JD, ISA, SUNg, SUNte, Rg, *pdPlotAtm) ;
				break ;
			}
			// ������ ��������� ���-2004
			rc = DMA_GOST_25645_166_2004(JD, ISA, SUNg, SUNte, Rg, *pdPlotAtm) ;
			} break ;
		default: 
			assert(0) ;
	}

	return rc ; 
}

//---------------------------------------------------------------------------
