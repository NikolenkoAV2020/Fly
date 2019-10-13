//-------------------------------------------------------------------------------
// ZMSpaceCraftSPH.cpp
// А. Николенко 03.08.2018
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>	// Математические функции

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
	// Расчёт ускорений обусловленных ГПЗ
	rc = ZASC1975toGSC(t, &X[1], Rg, 0) ; if(rc) return rc;
	// Ускорения центрального ГПЗ
	double UG0GPZ[3]; 
	rc = GPZ.Model_Grav_Pole_Center(Rg, UG0GPZ); if (rc) return rc;
	// Ускорения обусловленные аномалиями ГПЗ	
	double UGAGPZ[3]; 
	rc = GPZ.Model_Grav_Pole_Anomal(t, LSF.ngpz, LSF.mgpz, 
		 0, Rg, UGAGPZ, LSF.vTide);  if (rc) return rc;
	// Сумма ускорений
	double UGGPZ[3] ; 
	for(i = 0 ; i < 3 ; i++) UGGPZ[i]  = UG0GPZ[i]+UGAGPZ[i] ;
	// Перевод ускорений из ГСК в АСК 1975 года
	double UAGPZ[3];  
	rc = ZGSCtoASC1975(t, UGGPZ, UAGPZ, 1); if (rc) return rc;

	//---------------------------------------------------------------------------
	// Формирование правых частей СДУ движения
	Y[0] = 1 ;
	Y[1] = X[4] ;
	Y[2] = X[5] ;
	Y[3] = X[6] ;
	Y[4] = UAGPZ[0]  ;
	Y[5] = UAGPZ[1]  ;
	Y[6] = UAGPZ[2]  ;
	Y[7] = 0 ;

	//---------------------------------------------------------------------------
	// Гравитационное влияние Луны, Солнца, планет
	rc = ZMAttractionPlanet(t, RAsk, LSF, A) ;
	for(i=0 ; i<3 ; i++) Y[i+4]+= A[i] ;

	//---------------------------------------------------------------------------
	// Атмосфера
	if (LSF.vatm==ATMSMA81 || LSF.vatm==ATMSMA62 || LSF.vatm==ATMSMA440181) { 
		// Если статическая
		ZMAtmSMA* pAtm = nullptr ;
		if (LSF.vatm==ATMSMA81) pAtm = (ZMAtmSMA*)m_AtmSMA81 ;
		else { if (LSF.vatm==ATMSMA62) pAtm = (ZMAtmSMA*)m_AtmSMA62 ; 
			   else if (LSF.vatm==ATMSMA440181) pAtm = (ZMAtmSMA*)m_AtmSMA440181 ; 
		}
		assert(pAtm) ;
		rc = ZFCalcSAtm(t, Rg, LSF, NU, pAtm, A) ; RRC ;
	} else {
		// Если динамическая
		if (LSF.vatm>=ATMDMAGOST1977 && LSF.vatm<=ATMMSIS2000) {
			rc = ZFCalcDAtm(t, Rg, LSF, NU, m_ISAManager, A) ; RRC ;
	}	}
	// Добавляем рассчитанные возмущения в вектор суммарных возмущающих ускорений
	for(i=0 ; i<3 ; i++) Y[i+4]+= A[i] ;

	//---------------------------------------------------------------------------
	// Световое давление
	if (LSF.vsvd) {
		memset(A, 0, 3*sizeof(double)) ;

		double dDSD = 1.52e-5 ;
		double dPSD = 1998314.0 ;
		double dKSD = 0.02 ;

		// Расчет координат солнца на момент времени t (IAT)  
		rc = CalcAstroKoord(t, 0) ; RRC ;
		// Расчет ускорений возмущающего светового давления в зависимости от выбрканного варианта моделирования
		switch(LSF.vsvd) {
			default:
			case 1:
				if (NU.Kp<=0) {
					// Не задано значение коэффициента светового давления в НУ 
					break ;
				}
				dDSD = NU.Kp*1e-7 ;
				rc = ZMForceSPD_DSD(RAsk, Wae->qso, dDSD, A); RRC;
				break ;
			case 2 :
				if (NU.Kp<=0) {
					// Не задано значение коэффициента светового давления в НУ 
					break ;
				}
				dPSD = NU.Kp*1e-7 ;
				rc = ZMForceSPD_PSD(RAsk, Wae->qso, dPSD, A); RRC;
				break ;
			case 3 :
				if (NU.Kp<=0) {
					// Не задано значение коэффициента светового давления в НУ 
					break ;
				}
				dKSD = NU.Kp*1e-7 ;
				rc = ZMForceSPD_KSD(RAsk, Wae->qso, dKSD, A); RRC;
				break ;
		}
		// Добавляем рассчитанные возмущения в вектор суммарных возмущающих ускорений
		for(i=0; i<3; i++) Y[i+4]+= A[i] ;
	}

	//---------------------------------------------------------------------------
	// Программа управления
	if (LSF.isDU && !MPU.isEmpty()) {
		// Ускорения в орбитальной системе координат
		double Ms ;
		double Aorb[3] ;
		if (!MPU.CalcRLN(t, M, Aorb, &Ms)) {
			// Пересчёт из орбитальной СК в систему координат интегрирования
			ZRLNtoASC(RAsk, Aorb, A, 0) ;
			// Суммирование правых частей
			for(i=0; i<3; i++) Y[i+4]+= A[i] ;
			// Производная от массы равна секундному расходу топлива с обратным знаком
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