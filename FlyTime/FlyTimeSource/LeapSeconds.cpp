//---------------------------------------------------------------------------
// LeapSeconds.cpp
// А. Николенко 11.06.2019
//---------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyTime.h>
#include "LeapSeconds.h"

//---------------------------------------------------------------------------
static ZSLeapSeconds DifUtcIat;

static char LeapSecodsPath[512]   = ".\\FlyModInfo";
static char LeapSecodsSource[128] = "UTC_IAT.dat";

//---------------------------------------------------------------------------
// Определение каталога файла с массивовм секунд координации
void  SetLeapSecondsPath(const char* path)
{
	sprintf(LeapSecodsPath, "%s\0", path);
}
// Определение имени файла с массивовм секунд координации
void  SetLeapSecondsSource(const char* source)
{
	sprintf(LeapSecodsSource, "%s\0", source);
}

//---------------------------------------------------------------------------
// Текущие имя каталога и файла с данными о скачках секунды координации
char* GetLeapSecondsPath()
{
	return LeapSecodsPath;
}

char* GetLeapSecondsSource()
{
	return LeapSecodsSource;
}

//---------------------------------------------------------------------------
// Инициализация массива секунд координации
//---------------------------------------------------------------------------
int LeapSecondsInit(bool forSureLoad)
{
	if (forSureLoad) {
		// Принудительная инициализация
		if (DifUtcIat.isInit == true) DifUtcIat.Clear();
		DifUtcIat.isInit = false;
	} else { 
		// Инициализация при необходимости
		if (DifUtcIat.isInit == true) return 0;
	}

	// Инициализация
	ZSUtcIatItem D;
	ZSUtcIatItem Temp_DELTA_UTC_IAT;
	// Добавление фиктивного скачка времени До
	D.IatTime = -1.e20;
	D.dUtcIat = 0;
	DifUtcIat.IatList.push_back(D);
	// Чтение
	FILE* pf = 0;
	int rc = 0;
	char fileName[512];
	sprintf(fileName, "%s\\%s\0", LeapSecodsPath, LeapSecodsSource);
	pf = fopen(fileName, "r");
	if (pf) {
		while (1) {
			D.IatTime = 0;
			D.dUtcIat = 0;
			int rc = fscanf(pf, "%lf    %lf\n", &D.IatTime, &D.dUtcIat);
			if (rc != 2) break;
			D.dUtcIat /= 86400.0;
			DifUtcIat.IatList.push_back(D);
		}
		fclose(pf); pf = 0;
		DifUtcIat.isInit = true;
	}
	// Добавление фиктивного скачка времени После
	D.IatTime = 1.e20;
	DifUtcIat.IatList.push_back(D);

	// Заполнение оперативного массива (Последний участок)
	DifUtcIat.Cur  = DifUtcIat.IatList.at(DifUtcIat.IatList.size() - 2);
	DifUtcIat.Next = DifUtcIat.IatList.at(DifUtcIat.IatList.size() - 1);

#ifdef _DEBUG
	if (!DifUtcIat.isInit) {
		cout << endl << 
		"    ОШИБКА !!!! Не инициализирован массив секунд координации UTC-IAT" 
		<< endl;
	}
	//System.Diagnostics.Debug.WriteLine("    ОШИБКА !!!! Не инициализирован массив секунд координации UTC-IAT");
	//System.Diagnostics.Trace.WriteLine("    ОШИБКА !!!! Не инициализирован массив секунд координации UTC-IAT");
	//#if DifUtcIat.isInit
	//	#pragma message ("    Массив секунд координации UTC-IAT инициализирован успешно")
	//#else
	//	#pragma message ("    ОШИБКА !!!! Не инициализирован массив секунд координации UTC-IAT")
	//#endif
#else
	if (!DifUtcIat.isInit) {
		MessageBox(nullptr, 
		"ОШИБКА !!!!\nНе инициализирован массив секунд координации UTC-IAT", 
		"FlyTime", MB_ICONERROR | MB_OK);
	}
#endif

	return 0;
}

//---------------------------------------------------------------------------
// Определение величины скачка секнд координации на заданный момент IAT
// атомного времени - dTime, заданного в ссс
//---------------------------------------------------------------------------
double GetLeapSeconds(double dTime)
{
	UINT i;
	if (!DifUtcIat.isInit) {
		if (LeapSecondsInit(0)) return 0;
	}
	// Проверка соответствия оперативного массива
	if (DifUtcIat.Cur.IatTime  > dTime || DifUtcIat.Next.IatTime <= dTime) {
		// Поиск участка, соответствующего заданному моменту времени
		for (i = 0; i < DifUtcIat.IatList.size() - 1; i++) {
			if (DifUtcIat.IatList.at(i).IatTime <= dTime &&
				DifUtcIat.IatList.at(i + 1).IatTime > dTime) {
				// Формирование оперативного массива
				DifUtcIat.Cur = DifUtcIat.IatList.at(i);
				DifUtcIat.Next = DifUtcIat.IatList.at(i + 1);
				break;
	}	}	}
	// Возврат текущей поправки
	return DifUtcIat.Cur.dUtcIat;
}

//---------------------------------------------------------------------------
