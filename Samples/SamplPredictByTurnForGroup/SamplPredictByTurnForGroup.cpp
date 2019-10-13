//-----------------------------------------------------------------------------
// SamplPredictByTurnForGroup.cpp
// А. Николенко 14.06.2019
//
// Прогноз по виткам группы из 8-и КА
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>		// Собственоно сама модель движения
#include <FlyMMath.h>		// Математические функции
#include <FlyCoreLink.h>	// Подключение всех библиотек

int TaskTurnsPredictionGroup();

void main()
{
	// Вывод кирилицы в консольном приложении 
	//setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// Инициализация ядра Fly, фактически настройка путей к необходимым 
	// информационным ресурсам, инициализация глобальных переменных,
	// в последствии возможно инициализация соединения с БД...
	int rc = FlyGlobalInit(); if (rc) return;

	// Пример прогнозирования движения КА по виткам для группы КА
	TaskTurnsPredictionGroup();
}

//-----------------------------------------------------------------------------
// Прогноз по виткам группы из 8-и КА
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
//-----------------------------------------------------------------------------
int TaskTurnsPredictionGroup()
{
	cout << "\n   Prediction by turns for 8 SC group...\n";
	int  j, rc = 0;
	long Vit;
	long Vitn = 100;	// Виток начала моделирование
	long Vitk = 105;	// Виток завершения моделирования

	PZCPrintManager PrintMng;	// Диспетчер документирования
	PZMSC		    KA[8];		// Модели движения КА
	ZNU             NU[8];		// Начальные условия движения

	//-------------------------------------------------------------------------
	// Логическая шкала сил
	ZLSF LSF; memset(&LSF, 0, sizeof(LSF));
	LSF.num = 1;
	LSF.vgpz = GPZ90;
	LSF.ngpz = 16;
	LSF.mgpz = 16;
	LSF.vatm = ATMSMA81;
	LSF.vsvd = 0;
	LSF.varsp = 0;
	LSF.vTide = TIDENULL;
	LSF.isDU = 0;
	LSF.isSun = 1;
	LSF.isMoon = 1;

	//-------------------------------------------------------------------------
	// Выбор параметров орбиты - ССО
	// Параметры проектируемой орбиты
	double T, i, a, p, e, e1, e2;
	// Расчёт по средней высоте
	double Hs = 270;
	char   Msg[512];

	rc = MBuildSSOMIV_hs(Hs, T, i, p, e1, e2, Msg); RRC;
	e = sqrt(e1*e1 + e2*e2);
	a = p / (1 - e*e);

	//-------------------------------------------------------------------------
	// Формирование массива НУ
	for (j = 0; j<8; j++) {
		NU[j].Key.ka = j + 1; NU[j].Key.nom = 15; NU[j].Key.mod = 0;
		NU[j].Key.type = 8; NU[j].Key.bc = 1;
		NU[j].numlsf = 1;
		DATE dt = DATE(18, 7, 2018);
		TIME tm = TIME(9, 30, 0, 0);
		TA_DMB(1, &dt, &tm, &NU[j].t);
		NU[j].vit = 99;
		// Модификация НУ - заполняем структуру НУ полученными 
		// параметрами движения для ССО
		NU[j].sk = _SK_OSKTE;
		NU[j].X[0] = TO_DTKM(a);
		NU[j].X[1] = e;
		NU[j].X[2] = TO_RAD(i);
		NU[j].X[3] = TO_RAD(120.0);
		NU[j].X[4] = TO_RAD(0);
		NU[j].X[5] = TO_RAD(1e-15 + 45.0*((double)j));
		NU[j].Sb = 0.001;
		NU[j].Kp = 0;
		NU[j].M0 = 900;
		NU[j].MF = 50;

		// Инициализация модели движения для расчёта от заданных НУ (NU)
		// в заданном поле сил (LSF)
		rc = KA[j]->Init(NU[j], &LSF); if (rc) return rc;
	}

	//-------------------------------------------------------------------------
	// Описание формата документирования результатов прогноза

	// Горизонтальное документирование параметров движения то есть в строку
	PrintMng->MPK.AflatOn();
	// Документирование по виткам
	PrintMng->MPK.FromVitOn();
	// Состав документируемых параметров
	PrintMng->MPK << MODPR_KA;
	PrintMng->MPK << MODPR_VIT;
	PrintMng->MPK << MODPR_V_Hmin;
	PrintMng->MPK << MODPR_V_Hmax;
	PrintMng->MPK << MODPR_V_VUdate;
	PrintMng->MPK << MODPR_V_VUtime;
	PrintMng->MPK << MODPR_V_VUmssv;
	PrintMng->MPK << MODPR_V_VUL;
	PrintMng->MPK << MODPR_V_NUtime;
	PrintMng->MPK << MODPR_V_NUL;
	PrintMng->MPK << MODPR_V_Tdr;
	PrintMng->MPK << MODPR_V_Hsr;
	PrintMng->MPK << MODPR_Aosk;
	PrintMng->MPK << MODPR_eosk;
	PrintMng->MPK << MODPR_T_RTE;
	PrintMng->MPK << MODPR_T_VTE;

	// Создание текстового документа для документирования результатов прогноза
	PrintMng->PrePrint(&KA[0], "Прогноз", "_TaskTurnsPredictionGroup");
	// Заголовок текстового документа
	PrintMng->PrintHeaderDoc("\n   Прогноз параметров движения по виткам\n   для восьми КА\n\n");
	// Документирование НУ и логической шкалы сил
	for (j = 0; j<8; j++) PrintMng->PrintNU(NU[j], NULL, 3);
	PrintMng->PrintLSF(LSF, 1);

	// Структура для получения всех элементов движения КА на каждом шаге
	ZSC_SI SI;

	// Прогнозирование состояния всех КА на начало заданного витка - 
	// одного для всех КА.
	for (j = 7; j >= 0; j--) { rc = KA[j]->GoToVit(Vitn, &SI); RRC; }

	Vit = Vitn;
	cout << "      Initial tern " << Vitn << "\n";
	cout << "      Final   tern " << Vitk << "\n";

	// Основной цыкл моделирования 
	while (1) {
		for (j = 7; j >= 0; j--) {
			cout << "\r         Current tern  " << Vit << "  KA  " << j + 1;
			// Моделирование движения КА вдоль витка Vit с вычислением
			// интегральных характеристик витка и элементов движения в
			// характерных точках витка (восходящий и нисходящий узел)
			rc = KA[j]->GoAlongVit(Vit); if (rc) break;
			// Документирование текущего состояния КА
			PrintMng->PrintMSC(&KA[j]);
		}
		PrintMng->PrintLineMSC();
		Vit++;
		if (Vit>Vitk) break;
	}
	cout << "\n";

	PrintMng->PostPrint();
	PrintMng->ClosePrint();

	// Просмотр результатов средствами операционной системы
	PrintMng->ShowTextFile();
	PrintMng->ShowHtmlFile();

	return rc;
}

//-----------------------------------------------------------------------------
