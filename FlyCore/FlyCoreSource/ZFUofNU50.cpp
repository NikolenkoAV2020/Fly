//---------------------------------------------------------------------------
// ZFUofNU50.cpp
// А. Николенко 26.04.2019
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
//---------------------------------------------------------------------------
#include <FlyMMath.h>
#include <FlyTime.h>

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZSK.h>
#include <FlyCoreSource\\ZSKNU.h>
#include <FlyCoreSource\\ZFUofNU50.h>
#include <FlyCoreSource\\ZCNumParser.h>

//---------------------------------------------------------------------------
// Расшифровка НУ из результатов прогнозирования движения ЦМ,
// формат "ЧЕ"
//
// Пример формата:
//
//----- НУ движения КА - 68 ---------------------------------
//Номер - 1201    Тип - 10  Мод - 0  БЦ - 0
//Дата  11.03.2014   Время  02.12.11.65899  Виток 3902
//Номер ЛШС  1015    Номер ПУ  0			Карта связи 0
//-----------------------------------------------------------
//			    АСК-2000          ГСК          АСК-ИстЭ
//X, км		 3547.509260   -897.659240      3528.579176
//Y, км		 5881.353276  -6809.505751      5892.731937
//Z, км		   -4.730527	 -0.000000         0.000000
//Vx, м / с -1709.775038   1492.303964	   -1723.355784
//Vy, м / с  1030.633684   -191.037238      1025.379421
//Vz, м / с  7354.209292   7351.773437       351.773437
//-----------------------------------------------------------
//Sб(м**2 / кг)              0.012720
//Kсв(каппа)               200.000000
//-----------------------------------------------------------
//
// Предполагается что УОФ находдится в текстовом файле FileName, либо
// может быть прочитана из открытого ранее файла pf, либо находится в 
// строке Text.
// Результат в виде структуры НУ в NU. 
//---------------------------------------------------------------------------
int LoadNU50(char* FileName, FILE* pf, char* Text, ZNU& NU, bool PrintToCout)
{
	FAST_SEMAPFORE_OPEN("ParserAndLoadNU50");

	// Форматированная дата и время
	DATETIMEDMB DT;
	std::string nameParam;
	double D;
	int    rc=0, i, I, N, IndI;

	// По умолчанию все поля НУ нулевые
	memset(&NU, 0, sizeof(ZNU));

	//-----------------------------------------------------------------------
	// Разбор полученных строк из файла
	//-----------------------------------------------------------------------

	// Объект для разбора строк на подстроки целые числа и разделители 
	ZNumericParser Parser;

	// Признак начала формы
	Parser.WhatToLookFor.lf1 = "НУ движения КА";
	Parser.WhatToLookFor.lf2 = "";

	// Непосредственно разбор строк
	rc = Parser.Execute(FileName, pf, Text, nullptr, PrintToCout);
	if (rc) { rc = 1; goto end; }

	//-----------------------------------------------------------------------
	// Анализ содержимого массива текстовых чтрок
	//-----------------------------------------------------------------------

	// Колличество найденных строк
	N = Parser.GetCountString();
	//-----------------------------------------------------------------------
	// Разбор первых строк
	// Проверка имени формы
	//-----------------------------------------------------------------------

	// Номер КА (1-я строка)
	IndI = 0;
	rc = Parser.FindInt(0, IndI, I); if (rc) return rc;
	NU.Key.ka = I;
	// Ключи НУ (2-я строка)
	IndI = 0;
	rc = Parser.FindInt(1, IndI, I); if (rc) return rc;
	NU.Key.nom = I;
	rc = Parser.FindInt(1, IndI, I); if (rc) return rc;
	NU.Key.type = I;
	rc = Parser.FindInt(1, IndI, I); if (rc) return rc;
	NU.Key.mod = I;
	rc = Parser.FindInt(1, IndI, I); if (rc) return rc;
	NU.Key.bc = I;
	// Дата
	IndI = 0;
	rc = Parser.FindInt(2, IndI, I); if (rc) return rc;
	DT.D = I;
	rc = Parser.FindInt(2, IndI, I); if (rc) return rc;
	DT.M = I;
	rc = Parser.FindInt(2, IndI, I); if (rc) return rc;
	DT.G = I;
	// Время
	rc = Parser.FindInt(2, IndI, I); if (rc) return rc;
	DT.h = I;
	rc = Parser.FindInt(2, IndI, I); if (rc) return rc;
	DT.m = I;
	rc = Parser.FindDbl(2, IndI, D); if (rc) return rc;
	DT.s = (int)D;
	DT.d = D - DT.s;
	NU.t << DT;

	// СК
	NU.sk = _SK_ASK2000;

	// Виток
	IndI = 0;
	rc = Parser.FindInt(6, IndI, I); if (rc) return rc;
	NU.vit = I;

	// Координаты
	for (i = 0; i < 3; ++i){
		IndI = 0;
		rc = Parser.FindDbl(7 + i, IndI, D); if (rc) return rc;
		NU.X[i] = TO_DTKM(D);
	}
	for (i = 0; i < 3; ++i){
		IndI = 0;
		rc = Parser.FindDbl(10 + i, IndI, D); if (rc) return rc;
		NU.X[i+3] = TO_DTKMSSS(D*1-3);
	}

	i = 14;
	IndI = 0;
	rc = Parser.FindStr(i, IndI, nameParam, "Sб");
	if (!rc) {
		rc = Parser.FindDbl(i, IndI, D); if (rc) return rc;
		// В данном формате Sб выражен в кг/м**2 поэтому на g делить не нужно  
		// NU.Sb = D / 9.80665;
		i++;
		IndI = 0;
	}
	rc = Parser.FindStr(i, IndI, nameParam, "Kсв");
	if (!rc) {
		rc = Parser.FindDbl(i, IndI, D); if (rc) return rc;
		NU.Kp = D;
	}

end:;
	FAST_SEMAPFORE_CLOSE();
	return 0;
}

//---------------------------------------------------------------------------
