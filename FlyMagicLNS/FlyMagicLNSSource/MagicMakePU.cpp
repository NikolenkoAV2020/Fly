//-------------------------------------------------------------------------------
// MagicMakePU.cpp : 
// А. Николенко 23.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции
#include <FlyCore.h>		// Математические функции
#include <FlyMagicLNS.h>	//

//-------------------------------------------------------------------------------
int MagicMakePU(int NumPU, double m0, double* dms, ZSFlightRule& U, ZCDU& DU, ZPU& PU, int is, int ie)
{
	double t0, tm, dm ;
	int    i, j, rc = 0 ; 
	int    je = ie<0 ? (U.n-1):(ie>=U.n ? (U.n-1):ie) ;
	int    js = is<0 ? 0:(is>je? je:is) ;

	if (dms) *dms = 0 ; 
	for (j=js; j<=je; j++) {
		// Расчёт длительности основного участка работы ДУ
		rc = ZFPUCalcTimeMainFromTime(m0, DU, NULL, NULL, &U.dt[j], NULL, &dm, NULL, tm, NULL) ; RRC ;
		// Уменьшаем массу на суммарный расход топлива на включении с длительностью
		// основного участка tm
		m0-= dm ;
		if (dms) *dms+= dm ; 
		// Включение ДУ
		ZVPU VPU ;	
		t0 = U.t[j] ;
		for (i=0 ; i<DU.k ; i++) {
			// Элемент программы управления
			if (i>=DU.mu && DU.isOnlyMain) break ;
			ZPUItem IPU ;	
			IPU.var = varPU_pms ;
			IPU.t   = t0 ;			
			IPU.dt  = (i==DU.mu)?tm:DU.i[i].t ;  
			IPU.P   = DU.i[i].p ;
			IPU.ms  = DU.i[i].p/DU.i[i].pu ;
			IPU.dP  = 0 ;
			IPU.dms = 0 ; 
			IPU.a   = U.alf[j] ; 
			IPU.b   = U.bet[j] ; 
			IPU.da  = 0 ;
			IPU.db  = 0 ;
			// Добавляем участок во включение
			VPU << IPU ;
			t0+= IPU.dt/k_cbc ;
		}
		// Добавляем включение в программу управления
		PU << VPU ;
	}
	PU.NumPU = NumPU ;
	return 0 ;
}

//-------------------------------------------------------------------------------