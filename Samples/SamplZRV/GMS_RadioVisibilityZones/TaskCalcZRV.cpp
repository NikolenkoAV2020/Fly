//-------------------------------------------------------------------------------
// TaskCalcZRV.cpp : 
// А. Николенко 26.11.2018
//
// Расчёт ЗРВ одновременно для нескольких КА и несколькох НИПов
// !!!!
// Весь механизм фиксирования событий начала и конца зон, а также моментов
// параметра ЗРВ, реализован внутри модели движения. Отслеживание ЗРВ начинается 
// с момента передачи в модель движения массива контролируемых НИП посредством
// метода RunUpZRV.
// Найденые события транслируются в класс ZModZRV, наследуемый от класса 
// модели движения ZMSC, при помощи виртуального метода OnZRVEvent.
// В данном методе найденные ЗРВ архивируются.
//
// Таким образом, функционал настоящей программы сводится к формированию 
// исходных данных для решения, организации архивирования и документирования
// найденных ЗРВ и перехвату в методе OnZRVEvent найденных моделью движения
// событий ЗРВ.
// !!!!
// Большая часть кода настоящего модуля посвящена документированию результатов
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>	// Собственоно сама модель движения
#include <FlyMMath.h>	// Математические функции
#include <FlyZip.h>		// Создание ZIP-архива

//---------------------------------------------------------------------------
// Генерация структуры КА в виде набора НУ (Разбег)
#include <..\\RunUpSys\\RunUpSysBuild.h>	

#include "TaskCalcZRVDoc.h"	// Документирование результатов расчёта ЗРВ
#include "TaskCalcZRVMod.h"	// Надстройка над моделью движения для расчёта ЗРВ

// Расчёт целеуказаний для НКУ
#include "..\\GMS_AntennaPointing\\TaskCalcTargDir.h"	

//-------------------------------------------------------------------------------
// Исходные данные для решения задачи + рабочие массивы 
IDTASKZRV     IDZRV ;		
// Диспетчер документирования
PZCPrintManager PrintMng ;	

//-------------------------------------------------------------------------------
int TaskCalcZRV()
{
	cout << "\n   Calculation radio visibility zones...\n";

	unsigned int j;
	int  rc = 0;
	long Vit[8];
	char FullName[512];

	//----------------------------------------------------------
	// Формирование массива исходных данных для решения задачи. 
	// В прикладной программе имеющей GUI, эти исходные данные
	// формируются в той области программного кода, который реализует
	// интерактивное взаимодействие пользователя с программой.
	// В настоящем же примере, просто присвоим адекватные значения 
	// необходимым для расчёта структурам и переменным.

	// Начальный виток моделирования
	IDZRV.Vitn = 101;
	// Конечный виток моделирования
	IDZRV.Vitk = 103;
	// Колличество КА
	IDZRV.CountKA = 8;
	// Колличество НИП
	IDZRV.CountNip = 17;

	// Шаг расчёта целеуказаний, сек
	IDZRV.StepTargDir = 5;
	// Необходимость целеуказаний для каздой найденной ЗРВ
	IDZRV.needCalcTargDir = true;
	// Необходимость формирования отдельного документа для каждого НИП
	IDZRV.needForNipFile = IDZRV.CountNip > 1 ? true : false;
	// Необходимость формирования отдельного документа для каждого КА
	IDZRV.needForKAFile = IDZRV.CountKA > 1 ? true : false;
	// Необходимость сжимать файлы с результатами 
	// (исходные не сжвтые файлы удаляются)
	IDZRV.needZipRezults = true;

	// Индексы НИПов в таблице НИПов
	IDZRV.NipIndexes = new int[IDZRV.CountNip];
	memset(IDZRV.NipIndexes, 0, sizeof(int)*IDZRV.CountNip);

	// Индексы НИПов в массиве координат и характеристик 
	// НИПов (NIPS в файле ZNIP.cpp), для которых необходимо
	// выполнить рассчёт ЗРВ
	IDZRV.NipIndexes[0] = 1;
	IDZRV.NipIndexes[1] = 2;
	IDZRV.NipIndexes[2] = 3;
	IDZRV.NipIndexes[3] = 4;
	IDZRV.NipIndexes[4] = 5;
	IDZRV.NipIndexes[5] = 6;
	IDZRV.NipIndexes[6] = 7;
	IDZRV.NipIndexes[7] = 8;
	IDZRV.NipIndexes[8] = 9;
	IDZRV.NipIndexes[9] = 10;
	IDZRV.NipIndexes[10] = 11;
	IDZRV.NipIndexes[11] = 12;
	IDZRV.NipIndexes[12] = 13;
	IDZRV.NipIndexes[13] = 14;
	IDZRV.NipIndexes[14] = 15;
	IDZRV.NipIndexes[15] = 17;
	IDZRV.NipIndexes[16] = 16;

	IDZRV.VFN.clear();

	//----------------------------------------------------------
	// Выделение рабочих массивов

	// Номера КА для которы рассчитываются ЗРВ
	IDZRV.NumKA = new int[IDZRV.CountKA];
	memset(IDZRV.NumKA, 0, sizeof(int)*IDZRV.CountKA);
	// Модели движения
	IDZRV.KA = new ZModZRV[IDZRV.CountKA];
	// Начальные условия
	IDZRV.NU = new ZNU[IDZRV.CountKA];
	// Документы для отдельных НИП
	IDZRV.pfn = new FILE*[IDZRV.CountNip];
	memset(IDZRV.pfn, 0, sizeof(FILE*)*IDZRV.CountNip);
	// Вариант документирования ЗРВ
	IDZRV.VarDocZRV = 1;

	//----------------------------------------------------------
	// Каталог с результатами
	// Документирование результатов выполняется в каталог
	// _Результаты\\ЗРВ, создаваемый при его отсутствии автоматически
	// в текущем каталоге задачи
	char LDir[256], SDir[256];	MakeZRVDir(LDir, SDir);

	//----------------------------------------------------------
	// Шкала сил
	ZLSF LSF; memset(&LSF, 0, sizeof(LSF));
	LSF.num = 1;
	LSF.vgpz = GPZ90;
	LSF.ngpz = 8;
	LSF.mgpz = 8;
	LSF.vatm = ATMNULL;  // ATMSMA81 ; ATMNULL
	LSF.vsvd = 0;
	LSF.varsp = 0;
	LSF.vTide = TIDESTEP2; // TIDENULL, TIDESTEP2, TIDEPOSEIDON
	LSF.isDU = 0;
	LSF.isSun = 1;
	LSF.isMoon = 1;

	memcpy(&IDZRV.LSF, &LSF, sizeof(ZLSF));

	// Формирование набора из 8 НУ в двух плоскостях, 
	// разнесённых на 90 рад в плоскости и фазированных на 45 градусов
	// между плоскостями
	rc = RunUpSysBuild_4_4(IDZRV.NU, LSF, IDZRV.CountKA); if (rc) return rc;;

	//----------------------------------------------------------
	// Инициализация моделей соответствующими НУ
	// Моделей тоже 8 - по числу КА
	for (j = 0; j < (unsigned int)IDZRV.CountKA; j++) {
		rc = IDZRV.KA[j].Init(IDZRV.NU[j], &LSF); if (rc) return rc;;
		IDZRV.NumKA[j] = IDZRV.NU[j].Key.ka;
	}

	//----------------------------------------------------------
	// Начало документирования результатов для каждого КА отдельно
	if (IDZRV.needForKAFile) {
		for (j = 0; j < (unsigned int)IDZRV.CountKA; j++) {
			FILE** pfp = &IDZRV.KA[j].fp;
			PrintMng->PrePrint(&IDZRV.KA[j], SDir, "_TaskCalcZRV", pfp, FullName);
			if (pfp && *pfp) {
				// Если документ успешно создан, то...				
				// Сохраняем в архиве имён для дальнейшего сжатия данного файла
				// или помещения его в архивный файл
				IDZRV.VFN.push_back(FullName);
				// Заголовок создаваемого текстового документа
				DocHeaderZRV(*pfp, &IDZRV.KA[j], 1, IDZRV.NipIndexes, IDZRV.CountNip);
				// Документирование НУ и ЛШС 
				PrintMng->PrintNU(IDZRV.NU[j], nullptr, 3, *pfp);
				PrintMng->PrintLSF(LSF, 1, *pfp);
				// Документирование заголовка таблицы с ЗРВ
				DocTablHeaderZRV(*pfp);
	}	}	}

	//----------------------------------------------------------
	// Создание отдельного документа для каждого НИП
	if (IDZRV.needForNipFile) {
		for (j = 0; j < (unsigned int)IDZRV.CountNip; j++) {
			FILE**  pfp = &IDZRV.pfn[j];
			int  NumNip = ALLNIPS[IDZRV.NipIndexes[j]].Num;
			char FN[64]; sprintf(FN, "_TaskCalcZRV_NIP_%d\0", NumNip);

			PrintMng->PrePrint(nullptr, SDir, FN, pfp, FullName);
			if (pfp && *pfp) {
				// Если документ успешно создан, то...				
				// Сохраняем в архиве имён для дальнейшего сжатия данного файла
				// или помещения его в архивный файл
				IDZRV.VFN.push_back(FullName);
				// Заголовок создаваемого текстового документа
				DocHeaderZRV(*pfp, IDZRV.KA, IDZRV.CountKA, &IDZRV.NipIndexes[j], 1);
				// Документирование всех НУ
				for (int i = 0; i < IDZRV.CountKA; i++) PrintMng->PrintNU(IDZRV.NU[i], nullptr, 3, *pfp);
				// Документирование ЛШС
				PrintMng->PrintLSF(LSF, 1, *pfp);
				// Документирование заголовка таблицы с ЗРВ
				DocTablHeaderZRV(*pfp);
	}	}	}

	// Создание единого документа для всех рассчитываемых ЗРВ 
	// для всех КА
	rc = PrintMng->PrePrint(nullptr, SDir, "_TaskCalcZRV", nullptr, FullName);
	if (!rc) {
		// Если документ успешно создан...
		// Сохраняем в архиве имён для дальнейшего сжатия данного файла
		// или помещения его в архивный файл
		IDZRV.VFN.push_back(FullName);
		// Заголовок создаваемого текстового документа
		DocHeaderZRV(PrintMng->TextFile(), IDZRV.KA, IDZRV.CountKA, IDZRV.NipIndexes, IDZRV.CountNip);
		// Документирование НУ и ЛШС
		for (j = 0; j < (unsigned int)IDZRV.CountKA; j++) PrintMng->PrintNU(IDZRV.NU[j], nullptr, 3);
		PrintMng->PrintLSF(LSF, 1);
	}
	 
	//----------------------------------------------------------
	// Подшегивание всех НУ до начала первого витка интервала
	for (j=0; j<(unsigned int)IDZRV.CountKA; j++) { 
		rc = IDZRV.KA[j].GoToVit(IDZRV.Vitn) ; if (rc) return rc; ; 
		Vit[j] = IDZRV.Vitn ;
		// "Выкладываем" в модель движения НИПы для отслеживания ЗРВ
		// !!!! Именно вызовом этой функции прогрпмма расчёта ЗРВ
		// отличается от простого прогноза параметров движения !!!!
		IDZRV.KA[j].RunUpZRV(IDZRV.CountNip, IDZRV.NipIndexes) ;
		// Далее модель движения автоматически фиксирует моменты времени
		// наступленния событиз ЗРВ (вход, выход, параметр), вплоть до
		// вызова функции CompletZRV() 
	}

	// Документирование заголовка таблицы с ЗРВ
	DocTablHeaderZRV(PrintMng->TextFile());

	//----------------------------------------------------------
	// Цыкл моделирования движения КА на заданном интервале
	// с расчётом ЗРВ 
	while(1) {
		for (j=0; j<(unsigned int)IDZRV.CountKA; j++) { 
			// Только если не достигнут заданый виток
			if (Vit[j]>=IDZRV.Vitk) continue ;
			// Шаг интегрирования
			rc = IDZRV.KA[j].GoStepA() ; if (rc) break ;
			// Проверка значения витка
			Vit[j] = IDZRV.KA[j].GetVitok(_SK_ASKTE) ;
		}
		if (rc) break ;

		// Проверка, находится ли на текущем шаге хотя бы один КА
		// хотя бы в одной ЗРВ 
		for (j=0; j<(unsigned int)IDZRV.CountKA; j++) { 
			int j1 ;
			for (j1=0; j1<IDZRV.CountNip; j1++) { 
				if (IDZRV.KA[j].EventMng.ZRV[j1].Nip>0) break ;
			}
			if (j1<IDZRV.CountNip) break ;
		}		
		// При необходимости и возможности сбрасываем накопленный 
		// массив в текстовыый документ
		// Если нет ни одной "открытой" зоны (j=IDZRV.CountKA), 
		// то выполняем сброс накопленного массива в текстовый файл
		if (j==IDZRV.CountKA) {
			ResetZRVData(PrintMng->TextFile(), IDZRV.KA, IDZRV.CountKA);
		}

		// Проверка достижения для всех выбранных КА заданного витка
		int CountEnd = 0 ;
		for (j=0; j<(unsigned int)IDZRV.CountKA; j++) { 
			if (Vit[j]<IDZRV.Vitk) break ;
			CountEnd++ ;
		}
		// Если все КА достигли заданного витка, то завершаем цыкл моделирования
		if (CountEnd==IDZRV.CountKA) break ;
	}

	// Заверншение отслеживания событий ЗРВ
	for (j=0; j<(unsigned int)IDZRV.CountKA; j++) { 
		// Завершение автоматического фиксирования моментов времени
		// наступленния событий ЗРВ (вход, выход, параметр), 
		// освобождение выделенной ранее для этого памяти...
		IDZRV.KA[j].CompletZRV() ;
	}

	//----------------------------------------------------------
	// Завершение документирования массива расчитанных ЗРВ
	// для всех КА по всем НИПам
	DocEndZRV(PrintMng->TextFile());

	// Завершение документирования задачи
	PrintMng->PostPrint();
	PrintMng->ClosePrint();

	// Просмотр результатов средствами операционной системы
	PrintMng->ShowTextFile();
	PrintMng->ShowHtmlFile();

	// Закрытие всех текстовых документов созданных для документирования ЦУ
	TargDirFileCloseAll() ;

	//----------------------------------------------------------
	if (IDZRV.needForKAFile) {
		for (j=0 ; j<(unsigned int)IDZRV.CountKA ; j++) { 
			FILE** pfp = &IDZRV.KA[j].fp ;

			// Завершение документирования массива ЗРВ расчитанных 
			// для каждого КА отдельно
			DocEndZRV(*pfp) ;

			// Завершение документирования
			PrintMng->PostPrint(pfp);
			PrintMng->ClosePrint(pfp);
			IDZRV.KA[j].fp = nullptr ;
	}	}

	//----------------------------------------------------------
	if (IDZRV.needForNipFile) {
		for (j=0 ; j<(unsigned int)IDZRV.CountNip ; j++) { 
			FILE** pfp = &IDZRV.pfn[j] ;

			// Завершение документирования массива ЗРВ расчитанных 
			// для каждого НИП отдельно
			DocEndZRV(*pfp) ;

			// Завершение документирования
			PrintMng->PostPrint(pfp);
			PrintMng->ClosePrint(pfp);
			IDZRV.pfn[j] = nullptr ;
	}	}

	delete[] IDZRV.NumKA ;      IDZRV.NumKA = nullptr ;
	delete[] IDZRV.KA ;         IDZRV.KA = nullptr ;
	delete[] IDZRV.NU ;         IDZRV.NU = nullptr ;
	delete[] IDZRV.NipIndexes ; IDZRV.NipIndexes = nullptr ;
	delete[] IDZRV.pfn ;        IDZRV.pfn = nullptr ; 

	//----------------------------------------------------------
	// Сжатие файлов с результатами рассчёта, если это необходимо
	if (IDZRV.needZipRezults) {
		Zip  FZ;
		char cod_return[512];
		for (j = 0; j < IDZRV.VFN.size(); j++) {
			// NameFile   - Название файла и путь к нему
			// DirOut     - Путь выходного файла
			// flag_zip   - true - зазиповать, false - раззиповать
			// over       - true - перезапись, false - пропустить
			// cod_return - Текст ощибки возврата
			int rc = FZ.zip_unzip((char*)IDZRV.VFN[j].c_str(), LDir, true, true, cod_return);
			// Исходный файл удаляется только если он был корректно сжат
			if (!rc) rc = remove((char*)IDZRV.VFN[j].c_str());
	}	}

	return rc ;
}

//-------------------------------------------------------------------------------
