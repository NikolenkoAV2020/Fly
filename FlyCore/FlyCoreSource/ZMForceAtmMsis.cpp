//---------------------------------------------------------------------------
// ZMForceAtmMsis.cpp
// Расчет плотности атмосферы по модели MSIS
// А. Николенко 31.10.2018
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

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

	// Определение индексов Солнечной активности на момент времени ta
	ISAManager->SelectFrom(ta) ;
	// Индекс солнечной активности
	double F107 = ISAManager->F107() ;
	// Осреднённый индекс солнечной активности
	double F81  = ISAManager->F81() ;
	// Геомагнитный индекс
	double AP[7] ; 
	// Ежедневный индекс
	AP[0] = ISAManager->AP() ;
	// Если задать SW[9]=-1.0 (static double SW[25] в ZMForceAtmMsis90Data.h), 
	// то прирассчёте параметров атмосферы 
	// ислользуется ниже следующий массив трех-часовых и осреднённых индексов
	// 
	// 3-х часовой индекс на текущее время
	AP[1] = 0 ;
	// 3-х часовой индекс на 3 часа до текущего времени
	AP[2] = 0 ;
	// 3-х часовой индекс на 6 часов до текущего времени
	AP[3] = 0 ;
	// 3-х часовой индекс на 9 часов до текущего времени
	AP[4] = 0 ;
	// Осреднённое из 8-и значений 3-х часовых индексов
	// с 12 до 33 часов до текущего времени
	AP[5] = 0 ;
	// Осреднённое из 8-и значений 3-х часовых индексов
	// с 36 до 59 часов до текущего времени
	AP[6] = 0 ;

	// Расчёт параметров атмосферы
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

	// Вычисление координат Солнца
	rc = _CalcAstroKoord(AtomicTime, 0) ; RRC ;
	// Собственно вычисление МССВ
	double Sun[3] ;
	ZASCtoAscensionDecination(Wae->qso, Sun, 0) ;

	// Вычисление звёздного времени в радианах
	double SiderialTime ;
	rc = ZTrueSiderealTime(AtomicTime, SiderialTime) ; if(rc) return rc ;
	// Формирование местного ИСТИННОГО солнечного времени в радианах
	STL = SiderialTime + GLONG / M_GRARAD - Sun[0] + M_PI;
	STL = ValiA(STL, 0) ;
	// переход в часовую меру  
	STL = STL * 24 / M_DPI;

	// Вычисление дней от начала года и всемирное времени UT
	// т.е. временя на гринвиче в секундах
	// 
	// "-0.125" это переход от зонального времени к гринвическому времени т.е. UT
	double tag = (AtomicTime-0.125) ;
	DATETIMEDMB DT ; DT << tag ;
    DT.h=DT.m=DT.s=0 ; DT.d=0 ;
	// 0 часов на гринвиче
    double t1 ; t1 << DT ;
	DT.D=DT.M=1 ; 
	// 0 часов на гринвиче 01 января
    double t0 ; t0 << DT ;
	// Дней от начала года
	IYD = (int)(t1-t0) ;
	// Секунд от начала суток UT (от начала суток на Гринвиче)
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
	// Подготовка исходных данных для функции расчёта характеристик атмосферы
	double STL ;	// Местное ИСТИННОЕ солнечное временя в радианах
	double SEC ;	// Секунд от начала суток UT (от начала суток на Гринвиче)
	int    IYD ;	// Дней от начала года
	rc = DMA_MSIS_PRECALC(AtomicTime, GLAT, GLONG, STL, IYD, SEC) ; RRC ;
	for (int i=0; i<25; i++) SW[i] = 1.0 ;
	TSELEC(&SW[0], 0) ;

	// Расчет параметров атмосферы
	static double dParAtm[10], dTemp[3] ;
	GTD6(IYD, SEC, ALT, GLAT, GLONG, STL, F107A, F107, &AP[0], 48, dParAtm, dTemp) ;
	// искомая плотность атмосферы
	if (pdPlotAtm) *pdPlotAtm = dParAtm[5]*1000.0 ;
	return S_OK ;
}

//---------------------------------------------------------------------------
int DMA_MSIS_2000(double AtomicTime, double ALT,
				  double GLAT, double GLONG, double F107A,
				  double F107, double AP[], double *pdPlotAtm)
{
	int rc ; 

	// Подготовка исходных данных для функции расчёта характеристик атмосферы
	double STL ;	// Местное ИСТИННОЕ солнечное временя в радианах
	double SEC ;	// Секунд от начала суток UT (от начала суток на Гринвиче)
	int    IYD ;	// Дней от начала года
	rc = DMA_MSIS_PRECALC(AtomicTime, GLAT, GLONG, STL, IYD, SEC) ; RRC ;

	// Структура признаков учёта различных физических сыойств атмосферы 	
	nrlmsise_flags flags ;
	for (int i=0; i<24; i++) {
		flags.switches[i] = 1 ;
		flags.sw[i]  = 1.0 ;
		flags.swc[i] = 1.0 ;
	}

	// Структура с индексами геомагнитной обстановки
	ap_array ap_a ; memset(ap_a.a, 0, sizeof(ap_a.a)) ;

	// Основная структура с исходными данными для 
	// решения задачи рассчёта характеристик атмосферы
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

	// Результаты рассчёта
	nrlmsise_output output ;

	// Расчет параметров атмосферы
	gtd7(&input, &flags, &output);
	// искомая плотность атмосферы
	if (pdPlotAtm) *pdPlotAtm = output.d[5] ;
	return S_OK ;
}

//---------------------------------------------------------------------------
