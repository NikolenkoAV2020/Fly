//-------------------------------------------------------------------------------
// TaskTurnsPredictionSimpl.cpp : 
// А. Николенко 10.08.2018
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>		// Собственоно сама модель движения
#include <FlyMMath.h>		// Математические функции

//-------------------------------------------------------------------------------
// Прогноз движения КА по виткам.
// Демонстрация моделирования движения до начала витка в заданной эпохе
//		Основные демонстрируемые методы и классы: 
//		ZNU    NU			- Начальные условия - структура
//		ZSC_SI SI			- Элементы движения КА - структура
//		ZMSC   KA			- Модель движения, можно полагать её по сути своей 
//							  равной Космическому аппарату (КА) - класс
//		KA.Init(...)		- Инициализация модели движения - метод 
//		KA.GoToVit(...)		- Моделирование движения КА до начала заданного 
//							  витка - метод
//		KA.GoAlongVit(...)	- Прогноз вдоль витка с расчётом интегральных 
//							- параметров витка - метод
//		ZCPrintManager PrintMng - Диспетчер документирования - класс
//-------------------------------------------------------------------------------
int TaskTurnsPredictionSimpl()
{
	cout << "\n   Simple prediction by Turns...\n";

	int  rc = 0 ; 
	long Vit = 105 ;

	ZCPrintManager PrintMng;	// Диспетчер документирования
	ZMSC		  KA;		// Модели движения КА
	ZNU           NU;		// Начальные условия движения

	//----------------------------------------------------------
	// НУ
	NU.Key.ka = 1 ; NU.Key.nom = 15 ; NU.Key.mod = 0 ;
	NU.Key.type = 8 ; NU.Key.bc = 1 ;

	NU.numlsf = 1 ;
	DATE dt = DATE(10, 12, 2011);
	TIME tm = TIME(12, 15, 54, 0.5432) ;
	TA_DMB( 1, &dt, &tm, &NU.t ) ;
	NU.vit  = 101 ;

	NU.sk   = _SK_OSK1975 ;
	NU.X[0] = TO_DTKM( 15000.0 ) ;
	NU.X[1] = 0.0012 ;
	NU.X[2] = TO_RAD( 69.0 ) ;
	NU.X[3] = TO_RAD( 250.0 ) ;
	NU.X[4] = TO_RAD( 125.0 ) ;
	NU.X[5] = TO_RAD( 15 ) ;
	NU.Sb   = 0 ;
	NU.Kp   = 0 ;
	NU.M0   = 1503 ;
	NU.MF   = 197 ;
	
	//---------------------------------------------------------------------------
	// Логическая шкала сил
	ZLSF LSF ;	memset(&LSF, 0, sizeof(LSF)) ;
	LSF.num     = 1 ;
	LSF.vgpz    = GPZ90 ;
	LSF.ngpz    = 4 ;
	LSF.mgpz    = 4 ;
	LSF.vatm    = ATMNULL ;
	LSF.vsvd    = 0 ;       
	LSF.varsp   = 0 ;
	LSF.vTide   = TIDENULL ;
	LSF.isDU    = 0 ;
	LSF.isSun   = 1 ;
	LSF.isMoon  = 1 ;

	// Инициализация модели движения для расчёта от заданных НУ (NU)
	// в заданном поле сил (LSF)
	rc = KA.Init(NU, &LSF) ; if (rc) return rc;

	// Создание текстового документа для документирования результатов прогноза
	PrintMng.PrePrint(&KA, "Прогноз", "_TaskTurnsPredictionSimpl") ;
	PrintMng.PrintHeaderDoc("Прогноз движения КА");
	PrintMng.PrintText("Прогноз параметров движения КА на заданные витки");
	// Документирование НУ и логической шкалы сил
	PrintMng.PrintNU(NU, NULL, 3) ;
	PrintMng.PrintLSF(LSF, 1) ;

	// Моделирование движения КА вдоль витка Vit с вычислением
	// интегральных характеристик витка и элементов движения в
	// характерных точках витка (восходящий и нисходящий узел)
	// Предполагается что начало и конец витка определяются моментом 
	// пересечения экватора ТЕКУЩЕЙ ЭПОХИ (_SK_ASKTE) в восходящем узле
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	// Документирование результатов прогноза
	PrintMng.PrintMSC(&KA) ;
	Vit++ ;

	// Повторение для нескольких витков
	cout << "\r	  Turn  " << Vit << "....     " ;
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	PrintMng.PrintMSC(&KA) ;
	Vit++ ;

	cout << "\r	  Turn  " << Vit << "....     ";
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	PrintMng.PrintMSC(&KA) ;
	Vit+=2 ;

	cout << "\r	  Turn  " << Vit << "....     ";
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	PrintMng.PrintMSC(&KA) ;
	Vit++ ;

	cout << "\r	  Turn  " << Vit << "....     ";
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	PrintMng.PrintMSC(&KA) ;
	Vit+=2 ;

	cout << "\r	  Turn  " << Vit << "....     \n\n";
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	PrintMng.PrintMSC(&KA) ;

	// Завершение документирования
	PrintMng.PostPrint(); 
	PrintMng.ClosePrint();
	// Просмотр результатов средствами операционной системы
	PrintMng.ShowTextFile();
	PrintMng.ShowHtmlFile();


	return rc ;
}

//-------------------------------------------------------------------------------
