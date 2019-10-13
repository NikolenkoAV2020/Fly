//-------------------------------------------------------------------------------
// TaskTurnsPredictionSys2.cpp : 
// А. Николенко 26.11.2018
//
// Формирование 8-и НУ для КА в 2-х плоскостях
//		W1 = 339 град
//		W2 = 39  град
// В качестве орбит выбираются ОМИВ ССО
//
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>		// Собственоно сама модель движения
#include <FlyMMath.h>		// Математические функции

//---------------------------------------------------------------------------
int RunUpSysEstimateNU(ZLSF& LSF, ZNU& NU0, ZNU& NUK, double uz, double e1z, double e2z, double az, double havrz=0) ; 

//-------------------------------------------------------------------------------
ZMSC_EXHV_BEG(ZModExtern2)
	double GSK[6] ;
	int rc = ZASC1975toGSC(t, Xmain, GSK, 0) ;	RRC ;
	double HBL[3] ;
	ZGRtoHBL(GSK, HBL, 0, 0) ;
	Yexe[0] = 1e4*HBL[0] ;
	return 0 ;
ZMSC_EXTFORCE(ZModExtern2)
	// Учёт дополнительных возмущающих факторов
	Y[0]  += 0.0 ;
	Y[1]  += 0.0 ;
	Y[2]  += 0.0 ;
	Y[N-1]+= 0.0 ;
ZMSC_END


//-------------------------------------------------------------------------------
int TaskTurnsPredictionSys2()
{
	int  j, rc = 0 ;
	long Vit ; 
	long Vitn = 100 ;	// Начальный виток моделирования 
	long Vitk = 117 ;	// Конечный виток моделирования

	ZCPrintManager PrintMng ;
	ZModExtern2	  KA[8] ;	// Модели движения
	ZNU			  NU[8] ;	// Начальные условия

	//----------------------------------------------------------
	// Шкала сил
	ZLSF LSF ; memset(&LSF, 0, sizeof(LSF)) ;
	LSF.num    = 1 ;
	LSF.vgpz   = GPZ90 ;
	LSF.ngpz   = 16 ;
	LSF.mgpz   = 16 ;
	LSF.vatm   = ATMSMA81 ; // ATMNULL ;
	LSF.vsvd   = 0 ;       
	LSF.varsp  = 0 ;
	LSF.vTide  = TIDENULL ;
	LSF.isDU   = 0 ;
	LSF.isSun  = 1 ;
	LSF.isMoon = 1 ;

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
		DATE dt = DATE(1, 4, 2018);
		TIME tm = TIME(9, 0, 0, 0) ;
		TA_DMB(1, &dt, &tm, &NU[j].t) ;
		NU[j].vit  = 100 ;
		// Модификация НУ - заполняем структуру НУ полученными параметрами движения для ССО
		NU[j].sk   = _SK_OSKTE ;
		NU[j].X[0] = TO_DTKM(az) ;
		NU[j].X[1] = ez ;
		NU[j].X[2] = TO_RAD(iz) ;
		if ((j/2)*2==j)
			NU[j].X[3] = TO_RAD(339.555) ;
		else 
			NU[j].X[3] = TO_RAD(39.551) ;
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
	PrintMng.MPK << MODPR_V_NUmssv ;
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

	// Инициализация документирования и создание документа. 
	PrintMng.PrePrint(&KA[0], "Проектирование орбит", "_TaskTurnsPredictionSys2") ;
	PrintMng.PrintHeaderDoc("\n   Проектирование орбит и проноз восьми КА \"Р\"\n\n") ;
	// Документирование НУ и ЛШС
	for (j=0 ; j<8 ; j++) PrintMng.PrintNU(NU[j], NULL, 3);
	PrintMng.PrintLSF(LSF, 1) ;
	// Подшегивание всех НУ до начала следующего витка
	for (j=7 ; j>=0 ; j--) { rc = KA[j].GoToVit(Vitn); RRC; }
	Vit = Vitn ;

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

		PrintMng.PrintLineMSC() ;
		Vit++ ;
		if (Vit>Vitk) break ;
	}

	PrintMng.PostPrint() ; 
	PrintMng.ClosePrint();
	printf( "\n\n" ) ;
	return rc ;
}

//-------------------------------------------------------------------------------
