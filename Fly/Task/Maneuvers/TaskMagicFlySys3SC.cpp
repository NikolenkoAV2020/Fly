//-----------------------------------------------------------------------------
// TaskMagicFlySys3SC.cpp : 
// А. Николенко 15.04.2019
//
// Пример решения задачи коррекции системы из 3-х КА в одной плоскости
// с целью формирования такого состояния системы когда КА разнесены в плоскости
// ровно на 60 градусов.
// В качестве манёвра для каждого КА применяется 241 вариант L-манёвра для
// солнечно синхронной орбиты стационарных высот (ССОСВ).
// Требуемая ССОСВ для каждого КА проектируются по известной методике.
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции
#include <FlyCore.h>		// Математические функции
#include <FlyOrbitDesign.h>	// Проектирование орбит
#include <FlyMagicLNS.h>	//

//-----------------------------------------------------------------------------
#include <FlyMagicLNSSource/MagicSolution.h>

//-----------------------------------------------------------------------------
int TaskMagicFlySys3SC(int TypeOrb)
{
	// идентификатор задачи (иными словами номер варианта маневрирования)
	int indexTask = 311;

	// Модель решения задачи
	ZSMagicLnsSolution* MS = CreateMagicLnsSolution(); if (!MS) return 1;
	MS->MZ.ORB() = SPO_ORB_EL;	// Целевая орбита задаётся e+w.
	MS->MZ.ks    = 0;			// Тип КА.

	//-------------------------------------------------------------------------
	// Диспетчер документирования
	ZCPrintManager PrintMng;

	//-------------------------------------------------------------------------
	// Начало документирования .... Создание документа

	// Файл документа 
	FILE* fp = nullptr;

	char NameOrb[64] = "_";
	switch (MS->MZ.ORB()) {
	case SPO_ORB_OMIV: sprintf(NameOrb, "OMIV\0"); break;
	case SPO_ORB_DU:   sprintf(NameOrb, "DU\0"); break;
	case SPO_ORB_EL:   sprintf(NameOrb, "EL\0"); break;
	case SPO_ORB_HBH:  sprintf(NameOrb, "HBH\0"); break;
	}
	char NameFile[256]; sprintf(NameFile, "_TaskMagicFlySys%d_%s\0", indexTask, NameOrb);
	PrintMng.PrePrint(nullptr, " Расчёт коррекции системы КА", NameFile, &fp);

	//-------------------------------------------------------------------------
	int i, j, rc = 0;

	SmartPrintfS(fp, "  Решение задачи коррекции системы КА до системы на\n", true);
	SmartPrintfS(fp, "  солнечно синхронных орбитах стационарных высот,\n", true);
	SmartPrintfS(fp, "  фазированных друг относительно друга по аргументу широты.\n\n", true);

	// НУ движения КА системы.
	ZNU   NU[3];	
	ZNU   NU2[3];

	// Загрузка НУ из УОФ НУ01 
	cout << "  НУ1:" << endl;
	rc = LoadNU01("Форма НУ01 312.txt", NULL, NULL, NU[0], nullptr, true); 
	if (rc) return rc;
	NU[0].M0 = 420.0;
	NU[0].MF = 50.0;

	cout << "  НУ2:" << endl;
	rc = LoadNU01("Форма НУ01 313.txt", NULL, NULL, NU[1], nullptr, true); 
	if (rc) return rc;
	NU[1].M0 = 420.0;
	NU[1].MF = 50.0;

	cout << "  НУ3:" << endl;
	rc = LoadNU01("Форма НУ01 314.txt", NULL, NULL, NU[2], nullptr, true); 
	if (rc) return rc;
	NU[2].M0 = 420.0;
	NU[2].MF = 50.0;

	// Преобразование НУ в оскулирующие элементы текущей эпохи
	for (i = 0; i < 3; i++) {
		NU2[i] = NU[i];
		rc = ConvertSkNu(NU2[i], _SK_OSKTE); if (rc) return rc;
	}

	// Время самых поздних НУ
	double tmax =-9e99;
	// Определение времени самых поздних НУ
	j = 0;
	for (i = 0; i < 3; i++)	{
		if (tmax < NU[i].t) { j = i; tmax = NU[i].t; }
	}

	// момент начала маневрирования
	double t0   = tmax + 1.0;
	// Время на которое строится система
	double tsys = tmax + 6.0;

	char docString[256];
	DATETIMEUTC DTUTC;
	DTUTC<< t0;
	sprintf(docString, "\n  Время начала маневрирования: ");
	&docString[strlen(docString)] << DTUTC;
	SmartPrintfS(fp, docString, true);

	DTUTC<< tsys;
	sprintf(docString, "\n  Момент построенич системы:   ");
	&docString[strlen(docString)] << DTUTC;
	SmartPrintfS(fp, docString, true);
	SmartPrintfS(fp, "\n", true);

	// Логическая шкала сил для решения задачи
	ZLSF LSF;
	LSF.num   = 1;
	LSF.vgpz  = GPZ90;
	LSF.ngpz  = 4;
	LSF.mgpz  = 4;
	LSF.vatm  = ATMSMA81;  
	LSF.vsvd  = 0;
	LSF.varsp = 0;
	LSF.vTide = TIDESTEP2; 
	LSF.isDU  = 1;
	LSF.isSun = 1;
	LSF.isMoon= 1;
	memset(LSF.isPlanet, 0, sizeof(LSF.isPlanet));

	//---------------------------------------------------------------------------
	ZMSC  *KA;		// Модель движения КА
	ZSC_SI SI;		// Структура для получения от модели элементов движения КА
	ZNU    NU0[3];	// Начальные условия на момент начала маневрирования
	ZNU    NUT[3];	// Начальные условия на момент построения системы
	double uT[3];	// Значения аргументов широты КА и витка на 
	int	   vitT[3];	// момент построения системы
	double TdrT[3]; // Драконический период для каждого из КА

	// Создание в куче модели движения КА
	KA = new ZMSC; if (!KA) return -1;

	//-------------------------------------------------------------------------
	// Подшагивание всех НУ на один и тот же момент времени, 
	// спрева на момент начала маневрирования, потом на момент 
	// построения системы
	cout << endl 
		 << "  Проноз параметров движения в пассивном полёте" << endl
		 << "  до момента формирования системы." << endl;
	for (i = 0; i < 3; i++) {
		DTUTC<< NU[i].t;
		cout << "  КА №" << NU[i].Key.ka << "  от " << DTUTC << " . . ." << endl;
		// Инициализация модели движения для расчёта от заданных НУ (NU)
		// в заданном поле сил (LSF)
		rc = KA->Init(NU[i], &LSF); if (rc) return rc;

		// Прогноз на момент начала маневрирования
		rc = KA->GoToTime(t0, &SI); if (rc) return rc;
		// Формирование НУ
		NU0[i]     = NU[i];
		NU0[i].sk  = _SK_OSKTE;
		NU0[i].t   = SI.ta;						// Время в ССС
		NU0[i].vit = SI.VitTE;					// Виток
		memcpy(NU0[i].X, SI.OSKTE, Size6Dbl);	// Координаты (ОЭ ТЭ)

		// Прогноз на момент времени построения системы
		rc = KA->GoToTime(tsys, &SI); if (rc) return rc;
		uT[i]   = TO_GRAD(SI.OSKTE[5]);			// Аргумент широты в градусах
		vitT[i] = SI.VitTE;						// Виток

		// Прогноз последнего витка - витка содержащего момент
		// формирования системы. Для каждого КА этот номер витка свой.
		rc = KA->GoAlongVit(SI.VitTE); if (rc) return rc;
		TdrT[i]    = KA->FV.Td;							// Драконически период
		NUT[i]     = NU[i];
		NUT[i].sk  = _SK_OSKTE;							// Оск элем. ТЭ
		NUT[i].t   = KA->FV.FSUN.ta;					// Время в ССС
		NUT[i].vit = SI.VitTE;							// Виток
		memcpy(NUT[i].X, KA->FV.FSUN.OSKTE, Size6Dbl);	// Координаты (ОЭ ТЭ)

		sprintf(docString, "   КА %d, драконический период на витке %d  %.3lf сек.\n", 
		KA->GetKeyNU().ka, NUT[i].vit, TdrT[i]);
		SmartPrintfS(fp, docString, true);
	}

	SmartPrintfS(fp, "\n\n", true);
	SmartPrintfS(fp, "   Основной КА системы относительно которого\n", true);
	sprintf(docString, "   фазируются остальные КА                             %d\n\n", NUT[2].Key.ka);
	SmartPrintfS(fp, docString, true);
	SmartPrintfS(fp, "   КА фазируются относительно основного КА системы на  60 град.\n\n", true);
	SmartPrintfS(fp, "   Для каждого КА системы, в том числе и для основного,\n", true);
	SmartPrintfS(fp, "   формируется солнечно-сонхронная орбита стационарных высот.\n\n", true);
	SmartPrintfS(fp, "   После формирования системы КА движутся в плоскости синхронно\n", true);
	SmartPrintfS(fp, "   по солнечно-синхронным орбитам стационарных высот на постоянном\n", true);
	SmartPrintfS(fp, "   угловом расстоянии друг от друга.\n", true);

	//-------------------------------------------------------------------------
	// Удаление объекта модель движения.
	delete KA; KA = nullptr;

	//-------------------------------------------------------------------------
	// Отклонения аргумента широты которые необходимо устранить
	// в результате маневрирования
	double duT[3];
	duT[2] = 0;

	SmartPrintfS(fp, 
	"\n  Невязки по аргументу широты на момент формирования системы:\n", true);
	for (i = 0; i < 2; i++) {
		duT[i] = ValiDA(uT[i] - uT[2], 1);
		double s = Sign1(duT[i]);
		duT[i] = 60.0*s - duT[i];
		sprintf(docString, "       КА %d   %lf град.\n", NU0[i].Key.ka, duT[i]);
		SmartPrintfS(fp, docString, true);
	}
	SmartPrintfS(fp, "\n", true);

	//-------------------------------------------------------------------------
	// Расчёт параметров ССОСВ на момент построения системы 
	// для каждого КА системы 
	int    vitz[3]; // Требуемые витки на момент формирования системы.
	double uz[3],   // Требуемые арг-ты широты на момент формирования системы.
		   az[3],   // Большая полуось, эксцентриситет и положение преигея
		   ez[3],   // на начало заданного витка ...
		   wz[3],   // 
		   a0[3],   // Полуось, эксцентриситет и положение перигея на момент 
		   e0[3],   // начала витка, содержащего момент 
		   w0[3];   // формирования системы.

	SmartPrintfS(fp, "   Отклонения параметров онрбиты от ОСВ,\n", true);
	SmartPrintfS(fp, "     КА      Задано       Наблюдается    Отклонение\n", true);

	for (i = 0; i<3; i++) {
		double 
		i0    = TO_GRAD(NUT[i].X[2]),
		W0    = TO_GRAD(NUT[i].X[3]);
		a0[i] = TO_KM(NUT[i].X[0]);
		e0[i] = NUT[i].X[1];
		w0[i] = TO_GRAD(NUT[i].X[4]);
		// Проектирование ССОСВ по значению периода для заданного наклонения и
		// абсолютной долготы ВУ орбиты
		rc = OrbitDesignOSHfromTi(&LSF, TdrT[2], i0, W0, az[i], ez[i], wz[i]); 
		if (rc) return rc;

		// Требуемый аргумент широты на момент формирования системы КА 
		uz[i]   = uT[i] + duT[i];
		vitz[i] = vitT[i];
		// Проверка возможного значения аргумента широты вне диапазона
		// [0, 360] и соответствующая корректировка витка, если это необходимо
		SuitabilityTurnArg(uz[i], vitz[i]);

		sprintf(docString, "   %5d  %11.3lf  %11.3lf  %11.3lf\n", 
		NU0[i].Key.ka, TdrT[2], TdrT[i], TdrT[2] - TdrT[i]);
		SmartPrintfS(fp, docString, true);

		sprintf(docString, "          %11.3lf  %11.3lf  %11.3lf\n", 
		uz[i], uT[i], duT[i]);
		SmartPrintfS(fp, docString, true);

		sprintf(docString, "          %11.8lf  %11.8lf  %11.8lf\n", 
		ez[i], e0[i], ez[i] - e0[i]);
		SmartPrintfS(fp, docString, true);

		sprintf(docString, "          %11.3lf  %11.3lf  %11.3lf\n\n", 
		wz[i], w0[i], wz[i] - w0[i]);
		SmartPrintfS(fp, docString, true);
	}
	SmartPrintfS(fp, "\n");

	//-------------------------------------------------------------------------
	// Витки приложения импульсов
	int Vit[3][4];
	for (i = 0; i < 3; i++) {
		Vit[i][0] = NU0[i].vit + 2; 
		Vit[i][1] = NU0[i].vit + 20;
		Vit[i][2] = NUT[i].vit - 1; 
		Vit[i][3] = NUT[i].vit;
	}

	//-------------------------------------------------------------------------
	// Решение задачи маневрирования
	//-------------------------------------------------------------------------

	// Используемая ДУ (её номер)
	MS->MZ.DU()  = 1;					
	// Номер задачи в общем списке по порядку.	
	MS->MZ.VAR() = MS->MZ.FINDVARTASK(indexTask);
	// Формирование ключей (основного и дополнительного) 
	// требуемых на правом конце траектории параметров 
	MS->MZ.GetParKorFromVar(MS->MZ.DemandsKey);
	MS->MZ.GetAdvParKorFromVar(MS->MZ.SecondDemandsKey);
	// Параметризация задачи расчета параметров управлени движением ЦМ 
	DefaultParam(MS->MP);
	// Загрузка ДУ с условным номером 1 для КА 1
	LoadDU(1, 1, MS->DU);
	// Взводим флаг уточнения формы орбиты для того чтобы
	// На итерациях изменять прицельные значения минимальной высоты и её широты
	MS->MP.isEstimateOrb = true;

	//-------------------------------------------------------------------------
	// Параметризация документирования результатов прогноза
	PrintMng.MPK.ApeakOn();
	PrintMng.MPK.FromVitOn();
	PrintMng.MPK << MODPRDATE;
	PrintMng.MPK << MODPRTIME;
	PrintMng.MPK << MODPR_VIT;
	PrintMng.MPK << MODPR_MSSV;
	PrintMng.MPK << MODPR_M;
	PrintMng.MPK << MODPR_V_Tdr;
	PrintMng.MPK << MODPR_L;
	PrintMng.MPK << MODPR_V_DL;
	PrintMng.MPK << MODPR_V_DLS;
	PrintMng.MPK << MODPR_Aosk;
	PrintMng.MPK << MODPR_eosk;
	PrintMng.MPK << MODPR_V_Hsr;
	PrintMng.MPK << MODPR_V_Hmaxt;
	PrintMng.MPK << MODPR_V_HmaxU;
	PrintMng.MPK << MODPR_V_Hmax;
	PrintMng.MPK << MODPR_V_Hmint;
	PrintMng.MPK << MODPR_V_HminU;
	PrintMng.MPK << MODPR_V_Hmin;

	//-------------------------------------------------------------------------
	// Документирование НУ
	PrintMng.PrintNU(NU0[0], NULL, 3, fp);
	PrintMng.PrintNU(NU0[1], NULL, 3, fp);
	PrintMng.PrintNU(NU0[2], NULL, 3, fp);
	// Документирование ЛШС
	PrintMng.PrintLSF(LSF, 1, fp);

	//-------------------------------------------------------------------------
	// Описание внешних дифференциальных уравнений
	ZCExternalEquationsDefinition EED;
	EED << EXTEQ_ID_avrT_ex;
	EED << EXTEQ_ID_avrT_ey;
	EED << EXTEQ_ID_avrT_H;

	//-------------------------------------------------------------------------
	// Цикл решения задач маневрирования для каждого КА системы
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Создание модели движения
	KA = new ZMSC; if (!KA) return-1;
	
	for (i = 0; i < 3; i++) {
		//---------------------------------------------------------------------
		// В ЛШС учёт тяги ДУ обязвтелен
		assert(LSF.isDU);
		// ГПЗ обязательно должно быть не проще чем "нормальное ГПЗ"
		assert(LSF.ngpz >= 2);

		//---------------------------------------------------------------------
		// Инициализация модели движения
		rc = KA->Init(NU0[i], &LSF, &EED); if (rc) break;

		//---------------------------------------------------------------------
		MS->MZ.KA() = NU0[i].Key.ka;		// Условный номер КА
		MS->MZ.M()  = KA->WeightFullNU();	// Плная начальная масса КА

		MS->MZ.VITIMP(0) = Vit[i][0]; // Виток первого импульса
		MS->MZ.VITIMP(1) = Vit[i][1]; // Виток второго импульса
		MS->MZ.VITIMP(2) = Vit[i][2]; // Виток второго импульса
		MS->MZ.VIT()     = Vit[i][3]; // Заданный виток на котором 
									  // контролируются краевые условия
		// Требуемые параметры программного движения КА - получены в
		// результате проектирования ССОСВ
		MS->MZ.PART() = TdrT[2];// Драконический период
		MS->MZ.PARe() = ez[i];	// Эксцентриситет ССОСВ	
		MS->MZ.PARw() = wz[i];	// Аргумент широты перигея ССОСВ		
		MS->MZ.PARu() = uz[i];	// Заданная фаза КА
		MS->MZ.PARt() = tsys;	// Время на которое необходимо обеспечить 
								// заданную фазу КА

		//---------------------------------------------------------------------
		// Решение задачи расчёта параметров коррекции по заданию MZ 
		ZSLoopFact  P1;	// Параметры движения на виток первого имп 
		ZSLoopFact  P2;	// Параметры движения на виток второго имп 
		ZSLoopFact  PN;	// Начальные параметры движения на заданный виток
		ZSLoopFact  PR;	// Расчётные конечные параметры движения на заданный вит. 
		ZSFlightRule U;	// Прогрмамма управления

		// Процедура итерационного решения формализованной задачи расчёта 
		// методом узловых точек. Данная процедура является единой для всех 
		// тестовых задач именованных как TaskMagicFly__. 
		rc = MagicSolution(MS, &PrintMng, fp, LSF, *KA, NU0[i], &EED,
						   &P1, &P2, &PN, &PR, &U); if (rc) break;
	}
	// Закрытие документа с результатами решения
	PrintMng.ClosePrint(&fp);

	//-------------------------------------------------------------------------
	// Удаляем модель движения КА
	if (KA) delete KA;
	// Удаляем созданную ранее "Модель решения"
	FreeMagicLnsSolution(MS);

	return rc;
}

//-----------------------------------------------------------------------------