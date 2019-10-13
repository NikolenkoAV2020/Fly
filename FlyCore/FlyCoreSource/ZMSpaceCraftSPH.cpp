//-------------------------------------------------------------------------------
// ZMSpaceCraftSPH.cpp
// �. ��������� 03.08.2018
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>	// �������������� �������

//---------------------------------------------------------------------------
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZMForceSPD.h>
#include <FlyCoreSource\\ZCISAManager.h>
#include <FlyCoreSource\\ZMForceAtmSMA81.h>
#include <FlyCoreSource\\ZMForceAtmSMA62.h>
#include <FlyCoreSource\\ZMForceAtm.h>
#include <FlyCoreSource\\ZMForcePlanet.h>

//-------------------------------------------------------------------------------
int ZMSC::Sph( double* X, double* Y ) 
{
	int     i, rc = 0 ;
	double  M = X[7] ;
	double  t = X[0] ;
	double* RAsk = &X[1] ;
	double  Rg[6] ;
	double  A[3] ;

	//---------------------------------------------------------------------------
	// ������ ��������� ������������� ���
	rc = ZASC1975toGSC(t, &X[1], Rg, 0) ; if(rc) return rc;
	// ��������� ������������ ���
	double UG0GPZ[3]; 
	rc = GPZ.Model_Grav_Pole_Center(Rg, UG0GPZ); if (rc) return rc;
	// ��������� ������������� ���������� ���	
	double UGAGPZ[3]; 
	rc = GPZ.Model_Grav_Pole_Anomal(t, LSF.ngpz, LSF.mgpz, 
		 0, Rg, UGAGPZ, LSF.vTide);  if (rc) return rc;
	// ����� ���������
	double UGGPZ[3] ; 
	for(i = 0 ; i < 3 ; i++) UGGPZ[i]  = UG0GPZ[i]+UGAGPZ[i] ;
	// ������� ��������� �� ��� � ��� 1975 ����
	double UAGPZ[3];  
	rc = ZGSCtoASC1975(t, UGGPZ, UAGPZ, 1); if (rc) return rc;

	//---------------------------------------------------------------------------
	// ������������ ������ ������ ��� ��������
	Y[0] = 1 ;
	Y[1] = X[4] ;
	Y[2] = X[5] ;
	Y[3] = X[6] ;
	Y[4] = UAGPZ[0]  ;
	Y[5] = UAGPZ[1]  ;
	Y[6] = UAGPZ[2]  ;
	Y[7] = 0 ;

	//---------------------------------------------------------------------------
	// �������������� ������� ����, ������, ������
	rc = ZMAttractionPlanet(t, RAsk, LSF, A) ;
	for(i=0 ; i<3 ; i++) Y[i+4]+= A[i] ;

	//---------------------------------------------------------------------------
	// ���������
	if (LSF.vatm==ATMSMA81 || LSF.vatm==ATMSMA62 || LSF.vatm==ATMSMA440181) { 
		// ���� �����������
		ZMAtmSMA* pAtm = nullptr ;
		if (LSF.vatm==ATMSMA81) pAtm = (ZMAtmSMA*)m_AtmSMA81 ;
		else { if (LSF.vatm==ATMSMA62) pAtm = (ZMAtmSMA*)m_AtmSMA62 ; 
			   else if (LSF.vatm==ATMSMA440181) pAtm = (ZMAtmSMA*)m_AtmSMA440181 ; 
		}
		assert(pAtm) ;
		rc = ZFCalcSAtm(t, Rg, LSF, NU, pAtm, A) ; RRC ;
	} else {
		// ���� ������������
		if (LSF.vatm>=ATMDMAGOST1977 && LSF.vatm<=ATMMSIS2000) {
			rc = ZFCalcDAtm(t, Rg, LSF, NU, m_ISAManager, A) ; RRC ;
	}	}
	// ��������� ������������ ���������� � ������ ��������� ����������� ���������
	for(i=0 ; i<3 ; i++) Y[i+4]+= A[i] ;

	//---------------------------------------------------------------------------
	// �������� ��������
	if (LSF.vsvd) {
		memset(A, 0, 3*sizeof(double)) ;

		double dDSD = 1.52e-5 ;
		double dPSD = 1998314.0 ;
		double dKSD = 0.02 ;

		// ������ ��������� ������ �� ������ ������� t (IAT)  
		rc = CalcAstroKoord(t, 0) ; RRC ;
		// ������ ��������� ������������ ��������� �������� � ����������� �� ����������� �������� �������������
		switch(LSF.vsvd) {
			default:
			case 1:
				if (NU.Kp<=0) {
					// �� ������ �������� ������������ ��������� �������� � �� 
					break ;
				}
				dDSD = NU.Kp*1e-7 ;
				rc = ZMForceSPD_DSD(RAsk, Wae->qso, dDSD, A); RRC;
				break ;
			case 2 :
				if (NU.Kp<=0) {
					// �� ������ �������� ������������ ��������� �������� � �� 
					break ;
				}
				dPSD = NU.Kp*1e-7 ;
				rc = ZMForceSPD_PSD(RAsk, Wae->qso, dPSD, A); RRC;
				break ;
			case 3 :
				if (NU.Kp<=0) {
					// �� ������ �������� ������������ ��������� �������� � �� 
					break ;
				}
				dKSD = NU.Kp*1e-7 ;
				rc = ZMForceSPD_KSD(RAsk, Wae->qso, dKSD, A); RRC;
				break ;
		}
		// ��������� ������������ ���������� � ������ ��������� ����������� ���������
		for(i=0; i<3; i++) Y[i+4]+= A[i] ;
	}

	//---------------------------------------------------------------------------
	// ��������� ����������
	if (LSF.isDU && !MPU.isEmpty()) {
		// ��������� � ����������� ������� ���������
		double Ms ;
		double Aorb[3] ;
		if (!MPU.CalcRLN(t, M, Aorb, &Ms)) {
			// �������� �� ����������� �� � ������� ��������� ��������������
			ZRLNtoASC(RAsk, Aorb, A, 0) ;
			// ������������ ������ ������
			for(i=0; i<3; i++) Y[i+4]+= A[i] ;
			// ����������� �� ����� ����� ���������� ������� ������� � �������� ������
			Y[7] =-Ms*k_cbc ;
	}	}

	return 0  ;
}

//-------------------------------------------------------------------------------
int ZMSC::CalcAstroKoord( double t, int NP ) 
{
	return(_CalcAstroKoord(t, NP)) ; 
}

//-------------------------------------------------------------------------------