//--------------------------------------------------------------------------------
// ZMForceAtmDMAGost.CPP
// Вычисление плотности динамической атмосферы по ГОСТам,
// обобщённая функция.
// А. Николенко 07.11.2018.
//--------------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

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
	VersionATM    vatm,			// Вариант динамической модели атмосферы (ГОСТ) 
	double        AtomicTime,	// Атомное время от J1975, [ссс]
	double*       XYZg,			// Координаты КА в гринвиче, [ДТКМ]
	ZCISAManager* ISAManager,	// Диспетчер индексов СА
	double*       pdPlotAtm)	// Искомая плотность атмосферы, [кг/м3]
{
	if (!ISAManager) assert(0) ;
	if (vatm!= ATMDMAGOST1990 && vatm!=ATMDMAGOST2001 && vatm!= ATMDMAGOST2004) assert(0) ;

	int rc = 0 ;

	// Определение индексов Солнечной активности на момент времени ta
	ISAManager->UpdateToTime(AtomicTime, 1.7) ; 
	// Осреднение предыстории индекса F107 - то есть вычисление F81
	ISAManager->AverageF107t0F81(AtomicTime, 81, 1.7) ; 

	// Индексы солнечной активности
	ISA_DMA ISA ;
	ISA.F107   = ISAManager->F107() ;
	ISA.F81    = ISAManager->F81() ;
	ISA.Kp     = ISAManager->KP() ;
	ISA.flag3H = false ;

	// Вычисление координат Солнца
	rc = _CalcAstroKoord(AtomicTime, 0) ; RRC ;
	// Преобразуем координаты Солнца в ГСК
	double SUNg[3] ;
	rc = ZASC1975toGSC(AtomicTime, Wae->qso, SUNg, true) ; RRC ;

	// Юлианская дата 
	double JD = ZTime::AtomicTimetoMJD(AtomicTime)+ZMJD0; 

	// Координаты КА
	double Rg[3] ; 
	// Преобразование координат КА и Солнца из дткм в км,
	// как это требуется для ф-ии DMA_GOST_...
	for (int i=0; i<3; i++) { SUNg[i]*= 1e4 ; Rg[i] = XYZg[i]*1e4 ;	}
	 
	// Расчёт параметров атмосферы
	switch (vatm) {
		case ATMDMAGOST1977:
		case ATMDMAGOST1984:
			assert(0) ;
		case ATMDMAGOST2001:
			// Расчёт плотности ДМА-2000
			rc = DMA_GOST_25645_000_2001(JD, ISA, SUNg, Rg, *pdPlotAtm) ;
			break ;
		case ATMDMAGOST2004: 
		case ATMDMAGOST1990: {
			double SUNte[3] ;
			// Координаты Солнца в АСК текущей эпохи
			rc = ZASC_1975toTE(AtomicTime, Wae->qso, SUNte, false) ; RRC ;
			// Преобразование координат Солнца из дткм в км
			for (int i=0; i<3; i++) SUNte[i]*= 1e4 ;
			if (vatm==ATMDMAGOST1990) {
				// Расчёт плотности ДМА-1990
				rc = DMA_GOST_25645_115_1990(JD, ISA, SUNg, SUNte, Rg, *pdPlotAtm) ;
				break ;
			}
			// Расчёт плотности ДМА-2004
			rc = DMA_GOST_25645_166_2004(JD, ISA, SUNg, SUNte, Rg, *pdPlotAtm) ;
			} break ;
		default: 
			assert(0) ;
	}

	return rc ; 
}

//---------------------------------------------------------------------------
