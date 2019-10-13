//-------------------------------------------------------------------------------
// TaskTurnsPredictionSys.cpp : 
// А. Николенко 10.08.2018
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>		// Собственоно сама модель движения
#include <FlyMMath.h>		// Математические функции

//---------------------------------------------------------------------------
int RunUpSysEstimateNU(ZLSF& LSF, ZNU& NU0, ZNU& NUK, double uz, double e1z, double e2z, double az, double havrz=0) ; 

//-------------------------------------------------------------------------------
ZMSC_EXHV_BEG(ZModExtern)
	// Правые части внешних дифференциальных уравнений
	// "Вертикальное" расширение
	double GSK[6] ;
	int rc = ZASC1975toGSC(t, Xmain, GSK, 0) ;	RRC ;
	double HBL[3] ;
	ZGRtoHBL(GSK, HBL, 0, 0) ;
	Yexe[0] = 1e4*HBL[0] ;
ZMSC_EXTFORCE(ZModExtern)
	// Учёт дополнительных возмущающих факторов
	// "Горизонтальное" расширение
	Y[0]  += 0.0 ;
	Y[1]  += 0.0 ;
	Y[2]  += 0.0 ;
	Y[N-1]+= 0.0 ;
ZMSC_END

//-------------------------------------------------------------------------------
int TaskTurnsPredictionSys()
{
	int  j, rc = 0 ;
	long Vit ; 
	long Vitn = 100 ;	// Начальный виток моделирования 
	long Vitk = 117 ;	// Конечный виток моделирования

	ZCPrintManager PrintMng ;
	ZModExtern	  KA[8] ;	// Модели движения
	ZNU			  NU[8] ;	// Начальные условия

	//----------------------------------------------------------
	// Шкала сил
	ZLSF LSF ;	memset(&LSF, 0, sizeof(LSF)) ;
	LSF.num     = 1 ;
	LSF.vgpz    = GPZ90 ;
	LSF.ngpz    = 16 ;
	LSF.mgpz    = 16 ;
	LSF.vatm    = ATMSMA81 ;
	LSF.vsvd    = 0 ;       
	LSF.varsp   = 0 ;
	LSF.vTide   = TIDENULL ;
	LSF.isDU    = 0 ;
	LSF.isSun   = 1 ;
	LSF.isMoon  = 1 ;

	//----------------------------------------------------------	
	// Выбор параметров орбиты - ССО
	// Параметры проектируемой орбиты
	double Tz, iz, havrz, az, pz, ez, e1z, e2z ;
	// Расчёт по средней высоте
	double Hs = 270 ;
	char   Msg[512] ;
	rc = MBuildSSOMIV_hs(Hs, Tz, iz, pz, e1z, e2z, Msg) ; RRC ;
	ez = sqrt(e1z*e1z+e2z*e2z) ;
	az = pz/(1-ez*ez) ;
	havrz = 270.0 ;

	printf( "   Build Selection of 8 IC\n" ) ;
	//----------------------------------------------------------
	// Формирование набора из 8 НУ разнесённых в плоскости на 45 градусов
	for (j=0 ; j<8 ; j++) { 
		NU[j].Key.ka = j+1 ; NU[j].Key.nom = 15 ; NU[j].Key.mod = 0 ;
		NU[j].Key.type = 8 ; NU[j].Key.bc = 1 ;
		NU[j].numlsf = 1 ;
		DATE dt = DATE(18, 7, 2018);
		TIME tm = TIME(9, 30, 0, 0) ;
		TA_DMB(1, &dt, &tm, &NU[j].t) ;
		NU[j].vit  = 100 ;
		// Модификация НУ - заполняем структуру НУ полученными параметрами движения для ССО
		NU[j].sk   = _SK_OSKTE ;
		NU[j].X[0] = TO_DTKM(az) ;
		NU[j].X[1] = ez ;
		NU[j].X[2] = TO_RAD(iz) ;
		NU[j].X[3] = TO_RAD(120.0) ;
		NU[j].X[4] = TO_RAD(0) ;
		NU[j].X[5] = TO_RAD(1e-15+45.0*((double)j)) ;
		NU[j].Sb = 0.001 ;
		NU[j].Kp = 0 ;
		NU[j].M0 = 900 ;
		NU[j].MF = 50 ;

		// Уточняем параметры всех НУ с целью дибиться для всех НУ
		// одинаковых параметров (большая полуось, эксцентриситет и перигей) 
		// при "сносе" на начало витка 
		{
		ZNU NUK ;
		printf( "\r") ;
		printf( "      IC - %d", j+1) ;
		rc = RunUpSysEstimateNU(LSF, NU[j], NUK, 0, e1z, e2z, 0, havrz) ; RRC ;
		NU[j] = NUK ;
		}
/*
{
	DATA dt = DATA(19, 7, 2018) ;
	TIME tm = TIME(15, 0, 0, 0) ;
	TA_DMB(1, &dt, &tm, &NU[0].t) ;
	NU[0].sk   = _SK_GSK ;
	NU[0].X[0] = TO_DTKM(-6599.19286026398) ;
	NU[0].X[1] = TO_DTKM(868.80031692849) ;
	NU[0].X[2] = 0 ;
	NU[0].X[3] = TO_DTKMSSS(0.180032426629421) ;
	NU[0].X[4] = TO_DTKMSSS(1.36691456137583) ;
	NU[0].X[5] = TO_DTKMSSS(7.68922748487688) ;
	NU[0].Sb = 0.001 ;

	dt = DATA(19, 7, 2018) ;
	tm = TIME(15, 11, 15, 0) ;
	TA_DMB(1, &dt, &tm, &NU[1].t) ;
	NU[1].sk   = _SK_GSK ;
	NU[1].X[0] = TO_DTKM(-6548.55788221351) ;
	NU[1].X[1] = TO_DTKM(1191.86830688357) ;
	NU[1].X[2] = 0 ;
	NU[1].X[3] = TO_DTKMSSS(0.246950571663044) ;
	NU[1].X[4] = TO_DTKMSSS(1.35642268776906) ;
	NU[1].X[5] = TO_DTKMSSS(7.68922748487689) ;
	NU[1].Sb = 0.001 ;

	dt = DATA(19, 7, 2018) ;
	tm = TIME(15, 22, 31, 0) ;
	TA_DMB(1, &dt, &tm, &NU[2].t) ;
	NU[2].sk   = _SK_GSK ;
	NU[2].X[0] = TO_DTKM(-6482.11666845168) ;
	NU[2].X[1] = TO_DTKM(1512.05948766412) ;
	NU[2].X[2] = 0 ;
	NU[2].X[3] = TO_DTKMSSS(0.313272652797566) ;
	NU[2].X[4] = TO_DTKMSSS(1.34265682054221) ;
	NU[2].X[5] = TO_DTKMSSS(7.68922748487689) ;
	NU[2].Sb = 0.001 ;

	dt = DATA(19, 7, 2018) ;
	tm = TIME(15, 33, 46, 0) ;
	TA_DMB(1, &dt, &tm, &NU[3].t) ;
	NU[3].sk   = _SK_GSK ;
	NU[3].X[0] = TO_DTKM(-6400.02990709996) ;
	NU[3].X[1] = TO_DTKM(1828.59989770323) ;
	NU[3].X[2] = 0 ;
	NU[3].X[3] = TO_DTKMSSS(0.378838357242988) ;
	NU[3].X[4] = TO_DTKMSSS(1.32565025245001) ;
	NU[3].X[5] = TO_DTKMSSS(7.68922748487688) ;
	NU[3].Sb = 0.001 ;

	dt = DATA(19, 7, 2018) ;
	tm = TIME(15, 45, 02, 0) ;
	TA_DMB(1, &dt, &tm, &NU[4].t) ;
	NU[4].sk   = _SK_GSK ;
	NU[4].X[0] = TO_DTKM(-6302.49514768791) ;
	NU[4].X[1] = TO_DTKM(2140.72775266369) ;
	NU[4].X[2] = 0 ;
	NU[4].X[3] = TO_DTKMSSS(0.443489894950644) ;
	NU[4].X[4] = TO_DTKMSSS(1.3054439114018) ;
	NU[4].X[5] = TO_DTKMSSS(7.68922748487689) ;
	NU[4].Sb = 0.001 ;

	dt = DATA(19, 7, 2018) ;
	tm = TIME(15, 56, 18, 0) ;
	TA_DMB(1, &dt, &tm, &NU[5].t) ;
	NU[5].sk   = _SK_GSK ;
	NU[5].X[0] = TO_DTKM(-6189.74807349361) ;
	NU[5].X[1] = TO_DTKM(2447.68853907852) ;
	NU[5].X[2] = 0 ;
	NU[5].X[3] = TO_DTKMSSS(0.50707098234828) ;
	NU[5].X[4] = TO_DTKMSSS(1.28208662406239) ;
	NU[5].X[5] = TO_DTKMSSS(7.68922748487688) ;
	NU[5].Sb = 0.001 ;

	dt = DATA(19, 7, 2018) ;
	tm = TIME(16, 07, 33, 0) ;
	TA_DMB(1, &dt, &tm, &NU[6].t) ;
	NU[6].sk   = _SK_GSK ;
	NU[6].X[0] = TO_DTKM(-6062.0608218124) ;
	NU[6].X[1] = TO_DTKM(2748.74134656234) ;
	NU[6].X[2] = 0 ;
	NU[6].X[3] = TO_DTKMSSS(0.56942815394437) ;
	NU[6].X[4] = TO_DTKMSSS(1.25563476785045) ;
	NU[6].X[5] = TO_DTKMSSS(7.68922748487688) ;
	NU[6].Sb = 0.001 ;

	dt = DATA(19, 7, 2018) ;
	tm = TIME(16, 18, 49, 0) ;
	TA_DMB(1, &dt, &tm, &NU[7].t) ;
	NU[7].sk   = _SK_GSK ;
	NU[7].X[0] = TO_DTKM(-5919.74212213539) ;
	NU[7].X[1] = TO_DTKM(3043.15849161199) ;
	NU[7].X[2] = 0 ;
	NU[7].X[3] = TO_DTKMSSS(0.630410684405569) ;
	NU[7].X[4] = TO_DTKMSSS(1.22615229956423) ;
	NU[7].X[5] = TO_DTKMSSS(7.68922748487688) ;
	NU[7].Sb = 0.001 ;
}
*/
		// Перед инициализацией МД сообщаем что будет мнтегрироваться одно 
		// дополнительное дифференциальное уравнение
		// !!!! Вызывать можно только до вызова !!!!
		KA[j].SetExternalEquationsCount(1) ;
		// Инициализация моделей соответствующими НУ
		// Моделей тоже 8 - по числу КА
		rc = KA[j].Init(NU[j], &LSF) ; RRC ;
		// В процессе моделирования движения интегрирование дополнительных 
		// уравнений можно как "включить" так и "выключить"
		// В данном случае указываем на то что это делать все-таки необходимо.
		KA[j].ExternalEquationsOn() ;
		KA[j].ExternalEquationsOff() ;
	}
	printf( "\n") ;

	// Формирование набора документируемых параметров
	PrintMng.MPK.AflatOn() ;
	PrintMng.MPK.FromVitOn() ;
	PrintMng.MPK << MODPR_KA ;
	PrintMng.MPK << MODPR_VIT ;
	PrintMng.MPK << MODPR_V_Hmin ;
	PrintMng.MPK << MODPR_V_Hmax ;
	PrintMng.MPK << MODPR_V_VUdate ;
	PrintMng.MPK << MODPR_V_VUtime ;
	PrintMng.MPK << MODPR_V_VUmssv ;
	PrintMng.MPK << MODPR_V_VUL ;
	PrintMng.MPK << MODPR_V_NUtime ;
	PrintMng.MPK << MODPR_V_NUL ;	
	PrintMng.MPK << MODPR_V_Tdr ;
	PrintMng.MPK << MODPR_V_Hsr ;
	PrintMng.MPK << MODPR_V_Hmin ;
	PrintMng.MPK << MODPR_V_HminU ;
	PrintMng.MPK << MODPR_V_Hmax ;
	PrintMng.MPK << MODPR_V_HmaxU ;
	PrintMng.MPK << MODPR_Aosk ;
	PrintMng.MPK << MODPR_eosk ;
	PrintMng.MPK << MODPR_T_wTE ;
	PrintMng.MPK << MODPR_T_iTE ;
	PrintMng.MPK << MODPR_T_RTE ;
	PrintMng.MPK << MODPR_T_VTE ;

	PrintMng.PrePrint(&KA[0], NULL, "_TaskTurnsPredictionSys.txt") ;
	// Документирование НУ и ЛШС
	for (j=0 ; j<8 ; j++) PrintMng.PrintNU(NU[j], NULL, 3) ;
	PrintMng.PrintLSF(LSF, 1) ;
	// Подшегивание всех НУ до начала следующего витка
	for (j=7 ; j>=0 ; j--) { rc = KA[j].GoToVit(Vitn); RRC; }
	Vit = Vitn ;

	FILE* fp = fopen( "_TaskTurnsPredictionSys2.txt", "w" ) ;

	// Прогнозирование движения по виткам с расчетом всех характеристик витка
	double t00 =-999 ;
	double Xexe ;

	printf( "\n\n") ;
	printf( "   First turn........%d\n", Vitn) ;
	printf( "   Last turn.........%d\n", Vitk) ;

	while(1) {
		for (j=7 ; j>=0 ; j--) { 
			printf( "\r" ) ;
			printf( "   Turn..............%d  SC...%d", Vit, j+1) ;
			// Включаем внешние дифференциальные уравнения
 			KA[j].ExternalEquationsOn() ;
			// Моделирование движения вдоль заданного витка с расчётом всех характеристик
			// движения на витке
			rc = KA[j].GoAlongVit(Vit) ; if (rc) break ;
			// Выключаем внешние дифференциальные уравнения
			KA[j].ExternalEquationsOff(&Xexe) ;
			KA[j].FV.HAvr = Xexe/(KA[j].FV.Td/k_cbc) ; 
			//
			PrintMng.PrintMSC(&KA[j]) ;
		}

		double dt[8] ;
		double dts = 0 ;
		for (j=0 ; j<8 ; j++) { 
			if (j<7) {
				dt[j] = (KA[j].FV.UN.t - KA[j+1].FV.UN.t)*k_cbc ;  
				dts+= dt[j] ;
			} else {
				if (t00==-999) {
					t00 = KA[0].FV.UN.t ;
					continue ;  
				}
				dt[j] = (KA[j].FV.UN.t - t00)*k_cbc ;  
				dts+= dt[j] ;
				t00 = KA[0].FV.UN.t ;
			}
			fprintf(fp, "%12.5lf", dt[j]) ;
			if (j==7) fprintf(fp, "%15.5lf", dts) ;
		}
		fprintf( fp, "\n" ) ;
		PrintMng.PrintLineMSC() ;
		Vit++ ;
		if (Vit>Vitk) break ;
	}

	PrintMng.PostPrint() ; 
	PrintMng.ClosePrint();
	if (fp) fclose(fp) ; fp = NULL ;

	printf( "\n\n" ) ;
	return rc ;
}

//-------------------------------------------------------------------------------
