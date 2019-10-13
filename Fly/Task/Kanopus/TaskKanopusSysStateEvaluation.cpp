//-------------------------------------------------------------------------------
// TaskKanopusSysStateEvaluation.cpp
// А. Николенко 14.02.2019
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>			// Собственоно сама модель движения
#include <FlyMMath.h>			// Математические функции
#include <FlyOrbitDesign.h>		// Проектирование орбит
#include <FlyMagicLNS.h>		// Метод узловых точек

//---------------------------------------------------------------------------
#include "FlightThisAndThat.h"
#include <FlyMagicLNSSource/MagicSolution.h>

int TaskMagicFlyBegin(ZSMagicLnsSolution* MS, int task, ZMSC& KA,
	ZNU& NU, ZLSF& LSF, bool ResetNU, ZCPrintManager* PrintMng,
	FILE** pfp);

//---------------------------------------------------------------------------
int TaskKanopusSysStateEvaluation()
{
	int rc = 0 ;
	// Логическая шкала сил 
	ZLSF LSF ;
	LSF.num    = 1 ;
	LSF.vgpz   = GPZ90 ;
	LSF.ngpz   = 16 ;
	LSF.mgpz   = 16 ;
	LSF.vatm   = ATMSMA81 ;  // ATMSMA81 ; ATMNULL
	LSF.vsvd   = 0 ;       
	LSF.varsp  = 0 ;
	LSF.vTide  = TIDESTEP2 ; // TIDENULL, TIDESTEP2, TIDEPOSEIDON
	LSF.isDU   = 1 ;
	LSF.isSun  = 1 ;
	LSF.isMoon = 1 ;
	memset(LSF.isPlanet, 0, sizeof(LSF.isPlanet )) ;

	// Массив НУ для 6-и КА системы
	ZNU NU[6] ;
	ZNU NUOSK[6] ;
	// Правая часть имён файлов с УОФ НУ01 
	char NU01FileName[6][64] = {
		"НУ01 312",
		"НУ01 313",
		"НУ01 314",
		"НУ01 315",
		"НУ01 316",
		"НУ01 806" } ;

	// Загрузка НУ из файла в виде УОФ НУ01 и построение высотного
	// профиля для каждого КА на дистанции 1250 витков
	int i ; 
	for (i=0; i<6; i++) {
		// Имя файла с УОВ НУ01
		char FN[512] ; sprintf(FN, "Форма %s.txt", NU01FileName[i]) ;
		// Загрузка НУ для i-го всем системы
		rc = LoadNU01(FN, NULL, NULL, NU[i]) ; RRC ;
		// Моделирование высотного профиля
		//rc = FlightCalcHightContour(NU[i], LSF, 1250, NU01FileName[i]) ; RRC ;
	}

	// Проноз эволюции макс. и мин высот на интервале одного года
	// для каждого КА системы
	for (i=0; i<6; i++) {
		//rc = FlightCalcHightSimplified(NU[i], LSF, NU01FileName[i], 365, 15) ; RRC ;
	}

	// Расчёт параметров ОСВСС на момент НУ для каждого из КА системы 
	double az[6], ez[6], wz[6], a0[6], e0[6], w0[6] ;
	printf( "Отклонения параметров онрбиты от ОСВ\n") ; 
	for (i=0; i<6; i++) {
		NUOSK[i] = NU[i] ;
		ConvertSkNu(NUOSK[i], _SK_OSKTE) ;
		double Tdr0 = 5692 ;
		double i0 = TO_GRAD(NUOSK[i].X[2]) ;
		double W0 = TO_GRAD(NUOSK[i].X[3]) ;
		double a  = TO_KM  (NUOSK[i].X[0]) ;
		double e  = NUOSK[i].X[1] ;
		double w  = TO_GRAD(NUOSK[i].X[4]) ;
		a0[i] = a ;
		e0[i] = e ;
		w0[i] = w ;
		rc = OrbitDesignOSHfromTi(&LSF, Tdr0, i0, W0, a, e, w) ; if (rc) return rc;
		az[i] = a ;
		ez[i] = e ;
		wz[i] = w ;
		printf( "   da = %lf,  de = %lf,  dw = %lf\n", 
		az[i]-a0[i], ez[i]-e0[i], wz[i]-w0[i]) ; 		
	}

	FILE* pfs ; pfs = nullptr ;
	ZCPrintManager PrintSysMng ;	

	//---------------------------------------------------------------------------
	// Расчёт параметров манёвра каждого КА системы, обеспечивающего
	// формирование ОСВСС.

	//---------------------------------------------------------------------------
	ZSMagicLnsSolution* MS = CreateMagicLnsSolution();

	//---------------------------------------------------------------------------
	for (i=0; i<6; i++) {
		// идентификатор задачи
		int indexTask = 21 ;

		// TypeOrb - Тип требуемой орбиты
		//		SPO_ORB_EL	- геометрия требуемой орбиты задаётся как e, w 
		MS->MZ.ORB() = SPO_ORB_EL ;	

		//-----------------------------------------------------------------------
		// Инициализация НУ шкалы и модели движения, настройка документирования
		ZMSC   KA ;		// Модель движения КА
		FILE*  fp=NULL ;// Файл документирования результатов
		ZCPrintManager PrintMng ;	// Диспетчер документирования
		PrintMng.SetPrintFileSticker(NU01FileName[i]) ;

		// Загрузка и подготовка всех исходных данных
		rc = TaskMagicFlyBegin(MS, indexTask, KA, NU[i], LSF, false,
			 &PrintMng, &fp) ; RRC ;

		//----------------------------------------------------------------------
		// Формирование задание на расёт (формируется в интерактивном режиме 
		// с использованием GUI)
		MS->MZ.VITIMP(0) = NU[i].vit+1 ;	// Виток первого импульса
		MS->MZ.VITIMP(1) = NU[i].vit + 2;	// Виток второго импульса
		MS->MZ.VIT() = NU[i].vit + 3;	// Заданный виток на котором 
										// контролируются краевые условия
		// Требуемые параметры орбиты
		MS->MZ.PART() = 5692.8;	// Драконический период
		MS->MZ.PARe() = ez[i];		//	
		MS->MZ.PARw() = wz[i];		//	

		// Взводим флаг уточнения формы орбиты для того чтобы
		// На итерациях изменять прицельные значения минимальной 
		// высоты и её широты
		MS->MP.isEstimateOrb = true;

		//----------------------------------------------------------------------
		// Решение задачи расчёта параметров коррекции по заданию MZ 
		ZSLoopFact P1 ;	// Параметры движения на виток первого имп 
		ZSLoopFact P2 ;	// Параметры движения на виток второго имп 
		ZSLoopFact PN ;	// Начальные параметры движения на заданный виток
		ZSLoopFact PR ;	// Расчётные конечные параметры движения на заданный виток 
		ZSFlightRule U ;	// Прогрмамма управления

		//----------------------------------------------------------------------
		// Непосредственное решение задачи расчёта параметров программы 
		// управления движением центра масс КА
		rc = MagicSolution(MS, &PrintMng, fp, LSF, KA, NU[i], nullptr, 
						   &P1, &P2, &PN, &PR, &U) ; 
		// Закрытие документа с результатами решения
		PrintMng.ClosePrint(&fp);
		if (rc) return rc;

		if (!pfs) {
			PrintSysMng.PrePrint(&KA, "Канопус", "SystemState", &pfs) ;
			PrintSysMng.PrintHeaderDoc("\n   Оценка состояния системы Канопус\n\n" ) ;
			if (pfs) {
				for (int j=0; j<6; j++) {
					PrintSysMng.PrintNU(NU[j], "") ;
				}
				PrintSysMng.PrintLSF(LSF) ;
				fprintf(pfs, "\n\n") ;
				fprintf(pfs, "_____________________________________________________________________________________\n") ;
				fprintf(pfs, "     |Отклонения от стационарных высот     |  Корректирующая программа управления \n") ;
				fprintf(pfs, "__ __|_____________________________________|_________________________________________\n") ;
				fprintf(pfs, "  КА | Б. полуось   Эксцентриситет Перигей | U 1-имп  dV 1-имп    U 2-имп   dV 2-имп\n") ;
				fprintf(pfs, "     |         км                     град |    град     м/сек       град      м/сек\n") ;
				fprintf(pfs, "_____|_____________________________________|_________________________________________\n") ;
			}
		} 
		if (pfs) {
			fprintf(pfs,"%4.0d |%11.4lf  %13.7lf   %7.3lf | %7.3lf %8.3lf(%c)  %7.3lf %8.3lf(%c)\n", 
			NU[i].Key.ka, az[i]-a0[i], ez[i]-e0[i], wz[i]-w0[i], 
			U.us[0], 1e3*fabs(U.dV[0]), U.alf[0]>0? '-':'+',
			U.us[1], 1e3*fabs(U.dV[1]), U.alf[1]>0? '-':'+') ; 	
		}
		if (i==5) {
			fprintf(pfs, "_____|_____________________________________|_________________________________________\n") ;
			PrintSysMng.PostPrint(&pfs) ;
		}
	}

	FreeMagicLnsSolution(MS);

	return 0 ;
}

//-------------------------------------------------------------------------------




