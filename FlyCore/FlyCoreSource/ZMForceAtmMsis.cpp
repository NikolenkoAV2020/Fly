//---------------------------------------------------------------------------
// ZMForceAtmMsis.cpp
// ������ ��������� ��������� �� ������ MSIS
// �. ��������� 31.10.2018
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ZCalc.h>
#include <FlyCoreSource\\ZSiderealTime.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ZCISAManager.h>
#include <FlyCoreSource\\ZMForceAtmMsis.h>
#include <FlyCoreSource\\ZMForceAtmMsis90Data.h>
#include <FlyCoreSource\\ZMForceAtmMsis2000Struct.h>
#include <FlyCoreSource\\ZMForceAtmMsis2000.h>	

//---------------------------------------------------------------------------
int CalcModAtm_DMA_MSIS(VersionATM vatm, double ta, double* HBL, ZCISAManager* ISAManager, double *pdPlotAtm) 
{
	if (!ISAManager) assert(0) ;
	if (vatm!=ATMMSIS1990 && vatm!= ATMMSIS2000) assert(0) ;
	int rc = 0 ;

	// ����������� �������� ��������� ���������� �� ������ ������� ta
	ISAManager->SelectFrom(ta) ;
	// ������ ��������� ����������
	double F107 = ISAManager->F107() ;
	// ���������� ������ ��������� ����������
	double F81  = ISAManager->F81() ;
	// ������������ ������
	double AP[7] ; 
	// ���������� ������
	AP[0] = ISAManager->AP() ;
	// ���� ������ SW[9]=-1.0 (static double SW[25] � ZMForceAtmMsis90Data.h), 
	// �� ����������� ���������� ��������� 
	// ������������ ���� ��������� ������ ����-������� � ���������� ��������
	// 
	// 3-� ������� ������ �� ������� �����
	AP[1] = 0 ;
	// 3-� ������� ������ �� 3 ���� �� �������� �������
	AP[2] = 0 ;
	// 3-� ������� ������ �� 6 ����� �� �������� �������
	AP[3] = 0 ;
	// 3-� ������� ������ �� 9 ����� �� �������� �������
	AP[4] = 0 ;
	// ���������� �� 8-� �������� 3-� ������� ��������
	// � 12 �� 33 ����� �� �������� �������
	AP[5] = 0 ;
	// ���������� �� 8-� �������� 3-� ������� ��������
	// � 36 �� 59 ����� �� �������� �������
	AP[6] = 0 ;

	// ������ ���������� ���������
	switch (vatm) {
		case ATMMSIS1990:
			rc = DMA_MSIS_1990(ta, HBL[0], HBL[1], HBL[2], F81, F107, AP, pdPlotAtm) ;
		break ;
		case ATMMSIS2000:
			rc = DMA_MSIS_2000(ta, HBL[0], HBL[1], HBL[2], F81, F107, AP, pdPlotAtm) ;
		break ;
		default: assert(0) ;
	}

	return rc ; 
}

//---------------------------------------------------------------------------
int DMA_MSIS_PRECALC(double AtomicTime, double& GLAT, double& GLONG, double& STL, int& IYD, double& SEC)
{
	int rc ;

	GLAT  = ValiDA(GLAT, 1) ;
	GLONG = ValiA(GLONG, 1) ;

	// ���������� ��������� ������
	rc = _CalcAstroKoord(AtomicTime, 0) ; RRC ;
	// ���������� ���������� ����
	double Sun[3] ;
	ZASCtoAscensionDecination(Wae->qso, Sun, 0) ;

	// ���������� �������� ������� � ��������
	double SiderialTime ;
	rc = ZTrueSiderealTime(AtomicTime, SiderialTime) ; if(rc) return rc ;
	// ������������ �������� ��������� ���������� ������� � ��������
	STL = SiderialTime + GLONG / M_GRARAD - Sun[0] + M_PI;
	STL = ValiA(STL, 0) ;
	// ������� � ������� ����  
	STL = STL * 24 / M_DPI;

	// ���������� ���� �� ������ ���� � ��������� ������� UT
	// �.�. ������� �� �������� � ��������
	// 
	// "-0.125" ��� ������� �� ���������� ������� � ������������� ������� �.�. UT
	double tag = (AtomicTime-0.125) ;
	DATETIMEDMB DT ; DT << tag ;
    DT.h=DT.m=DT.s=0 ; DT.d=0 ;
	// 0 ����� �� ��������
    double t1 ; t1 << DT ;
	DT.D=DT.M=1 ; 
	// 0 ����� �� �������� 01 ������
    double t0 ; t0 << DT ;
	// ���� �� ������ ����
	IYD = (int)(t1-t0) ;
	// ������ �� ������ ����� UT (�� ������ ����� �� ��������)
	SEC = (tag-t1)*k_cbc ;

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// For the most physically realistic calculation these three
	// variables should be consistent (STL=SEC/3600+GLONG/15).
	return 0 ;
}

//---------------------------------------------------------------------------
int DMA_MSIS_1990(double AtomicTime, double ALT,
				  double GLAT, double GLONG, double F107A,
				  double F107, double AP[], double *pdPlotAtm)
{
	int rc = 0 ;
	// ���������� �������� ������ ��� ������� ������� ������������� ���������
	double STL ;	// ������� �������� ��������� ������� � ��������
	double SEC ;	// ������ �� ������ ����� UT (�� ������ ����� �� ��������)
	int    IYD ;	// ���� �� ������ ����
	rc = DMA_MSIS_PRECALC(AtomicTime, GLAT, GLONG, STL, IYD, SEC) ; RRC ;
	for (int i=0; i<25; i++) SW[i] = 1.0 ;
	TSELEC(&SW[0], 0) ;

	// ������ ���������� ���������
	static double dParAtm[10], dTemp[3] ;
	GTD6(IYD, SEC, ALT, GLAT, GLONG, STL, F107A, F107, &AP[0], 48, dParAtm, dTemp) ;
	// ������� ��������� ���������
	if (pdPlotAtm) *pdPlotAtm = dParAtm[5]*1000.0 ;
	return S_OK ;
}

//---------------------------------------------------------------------------
int DMA_MSIS_2000(double AtomicTime, double ALT,
				  double GLAT, double GLONG, double F107A,
				  double F107, double AP[], double *pdPlotAtm)
{
	int rc ; 

	// ���������� �������� ������ ��� ������� ������� ������������� ���������
	double STL ;	// ������� �������� ��������� ������� � ��������
	double SEC ;	// ������ �� ������ ����� UT (�� ������ ����� �� ��������)
	int    IYD ;	// ���� �� ������ ����
	rc = DMA_MSIS_PRECALC(AtomicTime, GLAT, GLONG, STL, IYD, SEC) ; RRC ;

	// ��������� ��������� ����� ��������� ���������� ������� ��������� 	
	nrlmsise_flags flags ;
	for (int i=0; i<24; i++) {
		flags.switches[i] = 1 ;
		flags.sw[i]  = 1.0 ;
		flags.swc[i] = 1.0 ;
	}

	// ��������� � ��������� ������������ ����������
	ap_array ap_a ; memset(ap_a.a, 0, sizeof(ap_a.a)) ;

	// �������� ��������� � ��������� ������� ��� 
	// ������� ������ �������� ������������� ���������
	nrlmsise_input input ; 
	input.year = 0 ;		/* year, currently ignored */
	input.doy  = IYD ;		/* day of year */
	input.sec  = SEC ;		/* seconds in day (UT) */
	input.alt  = ALT ;		/* altitude in kilometers */
	input.g_lat= GLAT ;		/* geodetic latitude */
	input.g_long = GLONG ;	/* geodetic longitude */
	input.lst  = STL ;		/* local apparent solar time (hours), see note below */
	input.f107A= F107A ;	/* 81 day average of F10.7 flux (centered on doy) */
	input.f107 = F107 ;		/* daily F10.7 flux for previous day */
	input.ap   = AP[0] ;	/* magnetic index(daily) */
	input.ap_a = &ap_a ;	/* see above */

	// ���������� ��������
	nrlmsise_output output ;

	// ������ ���������� ���������
	gtd7(&input, &flags, &output);
	// ������� ��������� ���������
	if (pdPlotAtm) *pdPlotAtm = output.d[5] ;
	return S_OK ;
}

//---------------------------------------------------------------------------
