//-------------------------------------------------------------------------------
// TaskTargDir.cpp : 
// А. Николенко 18.12.2018
//
// Расчёт и документирование ЦУ для антенны в пределах ЗРВ КА-НИП
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>		// Собственоно сама модель движения
#include <FlyMMath.h>		// Математические функции

//---------------------------------------------------------------------------
// Документирование результатов расчёта ЗРВ
#include "..\GMS_RadioVisibilityZones\TaskCalcZRVDoc.h"	
// Надстройка над моделью движения для расчёта ЗРВ
#include "..\GMS_RadioVisibilityZones\TaskCalcZRVMod.h"		
// Функции расчёта целеуказаний средствам НКУ
#include "TaskCalcTargDir.h"

//-------------------------------------------------------------------------------
extern IDTASKZRV IDZRV ;			// Исходные данные для решения задачи + рабочие массивы 
extern PZCPrintManager PrintMng ;	// Диспетчер документирования

char LineCalcTD[] = "_______________________________________________\0" ;

//-------------------------------------------------------------------------------
int CalcTargDir(int NumKA, int NipInd, long Vit, double* Xasc1975, double t1, double t2)
{
	// Создание или открытие текстового документа для документирование ЦУ
	FILE* pf = TargDirFileOpen(NumKA, NipInd, Vit, t1, t2) ;
	if (!pf) return-1 ;

	// Модель движения для расчёта ЦУ 
	int    rc  = 0 ;
	double t   = t1 ;
	double dt  = IDZRV.StepTargDir / k_cbc ;
	double Xgsc[6] ;

	DATETIMEDMB DT ;
	ZSC_SI      SI ;
	ZMSC		KA ;
	ZNU         NU ;

	NU = IDZRV.NU[0] ;
	NU.t = t1 ;
	NU.sk = _SK_ASK1975 ;
	NU.Key.ka = NumKA ;
	memcpy(NU.X, Xasc1975, 6*sizeof(double)) ;

	if (KA.IsOkCreate()) {
	} else {
		// Первичная инициализация
		rc = KA.Init(NU, &IDZRV.LSF) ; RRC ;
	}
	
	// Текущие координаты в ГСК (начальные, на момент начала ЗРВ)
	KA.GetVector(Xgsc, nullptr, nullptr, nullptr, _SK_GSK);
	//---------------------------------------------------------------------------
	// Цыкл моделирования движения КА с заданным шагом расчёта ЦУ
	//double percent = 0 ;
	//printf("TargDir for SC %d, GMS %d\0", NumKA, ALLNIPS[NipInd].Num) ;
	while (t<=t2) {
		//percent = 100.0*(t-t1)/(t2-t1) ;
		//putchar(177) ;
		// Расчёт параметров взаимного положения НИП-КА
		double Params[7] ;
		CalcZRVParams(&ALLNIPS[NipInd], Xgsc, Params) ;
		double G = Params[0] ;
		double A = Params[4] ;
		DT << t ;
		fprintf(pf, "  \0") ; 
		pf << DT ;
		fprintf(pf, "   %9.4lf %9.4lf\n\0", G, A) ;
		// если достигнут конец ЗРВ
		if (t==t2) {
			fprintf(pf, "%s\n", LineCalcTD) ;
			break ;
		}
		// Приращение времени - очередная точка для расчёта ЦУ
		t+= dt ;
		// ограничение времени расчёта моментом окончания ЗРВ
		// для точного выхода на момент завершения ЗРВ 
		if (t>t2) t = t2 ;
		// Расчёт очередной точки траектории КА 
		rc = KA.GoToTime(t, &SI) ; RRC ;   
		memcpy(Xgsc, SI.GSK, 6*sizeof(double)) ;
	}
	//printf("\n\0") ;

	return 0 ;
}

//-------------------------------------------------------------------------------
// Документирование рассчитанных массивов ЦУ
//-------------------------------------------------------------------------------
vector<FILE*> VPFTDfromKA ;
vector<int>   VKeyKAFile ;

//-------------------------------------------------------------------------------
// Создание имени файла для документирования ЦУ для заданного КА
void TargDirFileName(int NumKA, int NipInd, long Vit, char* FN)
{
	char Dir[256] ;	MakeZRVDir(Dir, nullptr) ;
	sprintf(FN, "%s\\_TaskCalcTD_SC%d.txt\0", Dir, NumKA) ;
}

//-------------------------------------------------------------------------------
// Создание или открытие текстового документа для документирование ЦУ
FILE* TargDirFileOpen(int NumKA, int NipInd, long Vit, double t1, double t2)
{
	FILE* pf = NULL ;
	char  FN[128] ; 
	int   i, j, N = VKeyKAFile.size() ;

	// Поиск открытого файла для КА с номером NumKA
	for (i=0; i<N; i++) {
		if (VKeyKAFile[i]==NumKA) break ;
	}
	// Если такого файла нет то создаём его
	if (i==N) {
		// Генерация имени файла
		TargDirFileName(NumKA, NipInd, Vit, FN) ; 
		// Сохраняем в архиве имён для дальнейшего сжатия данного файла
		// или помещения его в архивный файл
		IDZRV.VFN.push_back(FN) ;
		// Создание и открытие нового документа
		pf = fopen(FN, "w" ) ;

		// Архивируем указатель на созданный файл и номер КА
		VPFTDfromKA.push_back(pf) ;
		VKeyKAFile.push_back(NumKA) ;

		// Заголовок документа			
		fprintf(pf, "\n  Расчёт ЦУ для КА %d\n", NumKA) ;
		fprintf(pf, "  НИП:\0" ) ;
		for (j=0; j<IDZRV.CountNip; j++) {
			int ind = IDZRV.NipIndexes[j] ;
			fprintf(pf, j==IDZRV.CountNip-1 ? " %d.\0":" %d,\0", ALLNIPS[ind].Num) ;
		}
		fprintf(pf, "\n\n\0" ) ;

		// Поиск НУ для КА с номером NumKA
		for (j=0; j<IDZRV.CountKA; j++) {
			if (IDZRV.NU[j].Key.ka == NumKA) break ;
		}
		// Документирование НУ, если они конечно найдены
		if (j<IDZRV.CountKA) {
			PrintMng->PrintNU(IDZRV.NU[j], NULL, 3, pf);
		}
		// Документирование ЛШС
		PrintMng->PrintLSF(IDZRV.LSF, 1, pf);
	}
	pf = VPFTDfromKA[i] ;

	if (pf) {
		fprintf(pf, "\n  НИП   %d\n  Виток %d\n", ALLNIPS[NipInd].Num, Vit) ;
		DATETIMEDMB DT1, DT2 ;
		DT1 << t1 ;
		DT2 << t2 ;
		fprintf(pf, "  ЗРВ   начало: %.2d:%.2d:%.2d,%.3d  %.2d.%.2d.%.4d\n        конец : %.2d:%.2d:%.2d,%.3d\n", 
		DT1.h, DT1.m, DT1.s, (int)(DT1.d*1e3), DT1.D, DT1.M, DT1.G, 
		DT2.h, DT2.m, DT2.s, (int)(DT2.d*1e3)) ;
		fprintf(pf, "%s\n", LineCalcTD) ;
		fprintf(pf, "  Дата       Время         Угол места    Азимут\n") ;
		fprintf(pf, "                                град.     град.\n") ;
		fprintf(pf, "%s\n", LineCalcTD) ;
	}

	return pf ;
}

//-------------------------------------------------------------------------------
// Закрытие всех текстовых документов созданных для документирования ЦУ
void TargDirFileCloseAll()
{
	int i, N = VPFTDfromKA.size() ;
	for (i=0; i<N; i++) {
		FILE* pf = VPFTDfromKA[i] ;
		if (pf) {
			fclose(pf) ;
			pf = nullptr ;
	}	}
	VPFTDfromKA.clear() ;
	VKeyKAFile.clear() ;
}

//-------------------------------------------------------------------------------

