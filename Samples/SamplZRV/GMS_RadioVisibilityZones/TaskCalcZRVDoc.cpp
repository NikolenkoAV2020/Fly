P//-------------------------------------------------------------------------------
// TaskCalcZRVDoc.cpp : 
// А. Николенко 30.11.2018
//
// Функции документирования расчитанных данных ЗРВ в файл
//-------------------------------------------------------------------------------
#include "stdafx.h"
#include <direct.h>

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>		// Собственоно сама модель движения
#include <FlyMMath.h>		// Математические функции

//---------------------------------------------------------------------------
#include "TaskCalcZRVDoc.h"
#include "TaskCalcZRVMod.h"		

//-------------------------------------------------------------------------------
// Архив для накопления рассчитываемых ЗРВ
vector<ZZRV> VzZRV ;	
// Исходные данные для решения задачи и рабочие массивы
extern IDTASKZRV IDZRV ;

//-------------------------------------------------------------------------------
void MakeZRVDir(char* LDir, char* SDir)
{
	mkdir(DIRRESULT);
	if (SDir) sprintf(SDir, "ЗРВ");
	sprintf(LDir, "%s\\ЗРВ", DIRRESULT);
	mkdir(LDir);
}

//-------------------------------------------------------------------------------
// Заголовок текстового документа программы расчёта ЗРВ
void DocHeaderZRV(FILE* fp, ZModZRV* KA, int CountKA, int* NipIndexes, int CountNip)
{
	if (!fp) return ;
	if (!KA && !NipIndexes) {
		fprintf(fp, "\n  Расчёт зон радиовидимости КА с НИП\n\n\0" ) ;
		return ;
	}

	int i ;
	if (!KA && NipIndexes) {
		fprintf(fp, "\n  Расчёт зон радиовидимости КА\n\0" ) ;
		fprintf(fp, "  с НИП:\0" ) ;
		for (i=0; i<CountNip; i++) {
			int ind = NipIndexes[i] ;
			fprintf(fp, i==CountNip-1 ? " %d.\0":" %d,\0", ALLNIPS[ind].Num) ;
		}
		fprintf(fp, "\n\n\0" ) ;
		return ;
	}

	if (KA && !NipIndexes) {
		fprintf(fp, "\n  Расчёт зон радиовидимости с НИП\n\n\0" ) ;
		fprintf(fp, "  для КА:\0" ) ;
		for (i=0; i<CountKA; i++) {		
			fprintf(fp, i==CountKA-1 ? " %d;\0":" %d,\0", KA[i].GetNumKA()) ;
		}
		fprintf(fp, "\n\n\0" ) ;
		return ;
	}

	if (KA && NipIndexes) {
		fprintf(fp, "\n  Расчёт зон радиовидимости\n\0" ) ;
		fprintf(fp, "  для КА:\0" ) ;
		for (i=0; i<CountKA; i++) {		
			fprintf(fp, i==CountKA-1 ? " %d;\0":" %d,\0", KA[i].GetNumKA()) ;
		}
		fprintf(fp, "\n\0" ) ;
		fprintf(fp, "  с  НИП:\0" ) ;
		for (i=0; i<CountNip; i++) {
			int ind = NipIndexes[i] ;
			fprintf(fp, i==CountNip-1 ? " %d.\0":" %d,\0", ALLNIPS[ind].Num) ;
		}
		fprintf(fp, "\n\n\0" ) ;
		return ;
	}
}

//-------------------------------------------------------------------------------
// Заголовок таблицы ЗРВ
void DocTablHeaderZRV(FILE* fp)
{
	if (!fp) return ;
	fprintf(fp, "\n\0" ) ;

	if (IDZRV.VarDocZRV==0) { 
		fprintf(fp, "______________________________________________________________\n\0" ) ;
		fprintf(fp, "             Дата-Время                    Азимут   Угол места\n\0" ) ;
		fprintf(fp, "                                             град         град\n\0" ) ;
		fprintf(fp, "______________________________________________________________\n\0" ) ;
	}
	if (IDZRV.VarDocZRV==1) { 
		fprintf(fp, "________________________________________________________________________________________________________________________________________________________\n\0" ) ;
		fprintf(fp, "    КА   НИП  Виток   Дата-Время                  Азимут Угол места Длительность Время            Азимут Угол места   Время            Азимут Угол места\n\0" ) ;
		fprintf(fp, "                      (Вход)                       град.      град.         сек. (Выход)           град.      град.   (Параметр)        град.      град.\n\0" ) ;
		fprintf(fp, "________________________________________________________________________________________________________________________________________________________\n\0" ) ;
	}
}

//-------------------------------------------------------------------------------
// Завершение документирования массива расчитанных ЗРВ
void DocEndZRV(FILE* fp)
{
	if (!fp) return ;
	// Сброс всех насчитанных ЗРВ в текстовый документ
	ResetZRVData(fp) ;
	if (IDZRV.VarDocZRV==0) { 
		fprintf(fp, "______________________________________________________________\n\0" ) ;
	}
	if (IDZRV.VarDocZRV==1) { 
		fprintf(fp, "________________________________________________________________________________________________________________________________________________________\n\0" ) ;
	}
}

//-------------------------------------------------------------------------------
// Документирование одной ЗРВ
void DocZRVVar0(ZZRV& zrv, char* S, char Flag)
{
	if (!S) return ;
	char SN[6][24] ;
	if (Flag&1) {
		sprintf(SN[0], "SC") ;
		sprintf(SN[1], "GMS") ;
		sprintf(SN[2], "Turn ") ;
		sprintf(SN[3], "Entry:     ") ;
		sprintf(SN[4], "Vertex:    ") ;
		sprintf(SN[5], "OutFlow:   ") ;
	} else {
		sprintf(SN[0], "КА") ;
		sprintf(SN[1], "НИП") ;
		sprintf(SN[2], "Виток") ;
		sprintf(SN[3], "Вход:      ") ;
		sprintf(SN[4], "Параметр:  ") ;
		sprintf(SN[5], "Выход:     ") ;
	}

	sprintf(S, "  %s %d %s %d %s %d\n\0", SN[0], zrv.KA, SN[1], zrv.Nip, SN[2], zrv.Vit) ; 

	DATETIMEDMB DT ;
	DT.FromSSS(zrv.t1, 3) ;
	sprintf(&S[strlen(S)], "  %s%.2d:%.2d:%.4d %.2d:%.2d:%.2d,%.3d   %10.5lf   %10.5lf\n\0", 
	SN[3], DT.D, DT.M, DT.G, DT.h, DT.m, DT.s, (int)(1e3*DT.d), zrv.A1, zrv.G1) ;
	
	DT.FromSSS(zrv.tp, 3) ;
	sprintf(&S[strlen(S)], "  %s%.2d:%.2d:%.4d %.2d:%.2d:%.2d,%.3d   %10.5lf   %10.5lf\n\0", 
	SN[4], DT.D, DT.M, DT.G, DT.h, DT.m, DT.s, (int)(1e3*DT.d), zrv.Ap, zrv.Gmax) ;

	DT.FromSSS(zrv.t2, 3) ;
	sprintf(&S[strlen(S)], "  %s%.2d:%.2d:%.4d %.2d:%.2d:%.2d,%.3d   %10.5lf   %10.5lf\n\0", 
	SN[5], DT.D, DT.M, DT.G, DT.h, DT.m, DT.s, (int)(1e3*DT.d), zrv.A2, zrv.G2) ;
}

void DocZRVVar1(ZZRV& zrv, char* S)
{
	if (!S) return ;

	sprintf(S, " %5.0d %5.0d %6.0d\0", zrv.KA, zrv.Nip, zrv.Vit) ; 

	DATETIMEDMB DT ;
	DT.FromSSS(zrv.t1, 3) ;
	sprintf(&S[strlen(S)], "   %.2d:%.2d:%.4d %.2d:%.2d:%.2d,%.3d %10.5lf %10.5lf\0", 
	DT.D, DT.M, DT.G, DT.h, DT.m, DT.s, (int)(1e3*DT.d), zrv.A1, zrv.G1) ;

	sprintf(&S[strlen(S)], "% 12.3lf \0", 86400.0*(zrv.t2-zrv.t1)) ;

	DT.FromSSS(zrv.t2, 3) ;
	sprintf(&S[strlen(S)], " %.2d:%.2d:%.2d,%.3d %10.5lf %10.5lf\0", 
	DT.h, DT.m, DT.s, (int)(1e3*DT.d), zrv.A2, zrv.G2) ;

	DT.FromSSS(zrv.tp, 3) ;
	sprintf(&S[strlen(S)], "   %.2d:%.2d:%.2d,%.3d %10.5lf %10.5lf\n\0", 
	DT.h, DT.m, DT.s, (int)(1e3*DT.d), zrv.Ap, zrv.Gmax) ;
}

void DocZRV(ZZRV& zrv, char* S, char* Sscr)
{
	if (!S) return ;
	if (IDZRV.VarDocZRV==0) { 
		DocZRVVar0(zrv, S, 0) ;
		DocZRVVar0(zrv, Sscr, 1) ;
	} else {
		DocZRVVar0(zrv, Sscr, 1) ;
	}
	if (IDZRV.VarDocZRV==1) { 
		DocZRVVar1(zrv, S) ;
	}
}

//-------------------------------------------------------------------------------
// Накапление массива ЗРВ - добаление в массив вновь рассчитанной зоны
void AccumulationZRVData(ZZRV& zrv)
{
	// Накопление данных о ЗРВ выполняется только если обнаружены 
	// вход, выход и параметр
	if (zrv.isFound != 7) return ;
	// Накопление данных о ЗРВ
	VzZRV.push_back(zrv) ;
}

//-------------------------------------------------------------------------------
// Сброс накопленного массива ЗРВ в текстовый документ
void ResetZRVData(FILE* fp, ZModZRV* KA, int CountKA)
{
	// Разиер накопленного массива
	int N = VzZRV.size();
	// Если нет рассчитанных ЗРВ, то просто выходим из процедуры
	if (!N) return;
	// Сортировка массива по времени начала ЗРВ
	sort(VzZRV.begin(), VzZRV.end());
	// Докуметирование накопленного массива по одной зоне
	if (fp) {
		int i;
		for (i = 0; i<N; i++) {
			//---------------------------------------------------------------
			// Выбор НИП для которого найдена ЗРВ VzZRV[i]
			int indNip = -1;
			int numNip = -1;
			int jNip;
			for (jNip = 0; jNip<IDZRV.CountNip; jNip++) {
				indNip = IDZRV.NipIndexes[jNip];
				numNip = ALLNIPS[indNip].Num;
				if (VzZRV[i].Nip == numNip) break;
			}
			if (jNip == IDZRV.CountNip) continue;

			// Документирование 1-й ЗРВ в текстовую строку
			char S[512], Sscr[512];
			DocZRV(VzZRV[i], S, Sscr);
			// Инициация расчёта и документирования ЦУ 
			CalcTargDir(VzZRV[i].KA, indNip, VzZRV[i].Vit,
			VzZRV[i].X, VzZRV[i].t1, VzZRV[i].t2);

			// Далее эту строку "разбрасываем" по необходимым документам

			if (IDZRV.needForKAFile) {
				//---------------------------------------------------------------
				// Выбор КА для которого найдена ЗРВ VzZRV[i]
				int j;
				for (j = 0; j<CountKA; j++) {
					if (VzZRV[i].KA == KA[j].GetNumKA()) break;
				}
				// Запись характеристик найденной ЗРВ в документ
				// для j-го КА 
				if (j<CountKA) fprintf(KA[j].fp, "%s", S);
			}

			if (IDZRV.needForNipFile) {
				// Запись характеристик найденной ЗРВ в документ
				// для j-го НИП
				fprintf(IDZRV.pfn[jNip], "%s", S);
			}

			//-------------------------------------------------------------------
			// Запись характеристик найденной ЗРВ в документ
			// для всей совокупности КА, указанных в 
			// задании на расчёт 
			fprintf(fp, "%s", S);
			// Документирование на экран, 
			// если приложение консольное
			cout << Sscr;
	}	}
	// Освобождение накопленного массива данных о ЗРВ
	VzZRV.clear();
}

//-------------------------------------------------------------------------------