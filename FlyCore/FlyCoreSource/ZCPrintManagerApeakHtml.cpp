//-----------------------------------------------------------------------------
// ZCPrintManagerApeakHtml.cpp
// А. Николенко 03.08.2018
// Вертикальное документирование результатов прогноза
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

#include <FlyMMath.h>		// Математические функции
#include <FlyTime.h>		// Всё про время
#include <FlyHtmlWriter.h>	// Для документирования в формате Html

//-----------------------------------------------------------------------------
#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZCPrintManager.h>
#include <FlyCoreSource\\ZCPrintFormat.h>

//-------------------------------------------------------------------------------
void ZCPrintManager::TablApeakHtmlGroupHeader(ZСHtmlItem& I,
	const int j,
	const char* caption,
	const int countPSB)
{
	I(j).n = "tr";
	I(j)(0).n = "td";
	I(j)(0)[0] << "colspan \n 2";
	I(j)(1).n = "th";
	I(j)(1).txt = caption;
	char S[128];  sprintf(S, "colspan \n %d", countPSB);
	I(j)(1)[0] << S;
	I(j)(1)[1] << "style \n text-align: center";
}

//-------------------------------------------------------------------------------
void ZCPrintManager::TablApeakHtmlGroupHBLRH(ZСHtmlItem& I,
	const int  j,
	const bool forH,
	const bool forMax,
	const int  countPSB)
{
	// Новая строка 
	I(j).n = "tr";
	// Имена переменных
	I(j)(0).n = "th";
	I(j)(0).txt = forH ? "Время<br>u(т.е.)<br>H<br>B<br>L" : "Время<br>u(т.е.)<br>R<br>B<br>L";
	// Размерности
	I(j)(1).n = "th";
	I(j)(1).txt = "<br>град<br>км<br>град<br>град";
	// Значения переменных
	string str;
	char   S[128];
	for (int i = 0; i < countPSB; i++) {
		ZSC_VP* HR;
		if (forH) HR = forMax ? HMAX_VI(PVB[i]) : HMIN_VI(PVB[i]);
		else      HR = forMax ? RMAX_VI(PVB[i]) : RMIN_VI(PVB[i]);
		MPF.FDataTime << HR->t;
		MPF.FDataTime.GetTimeText(S);
		str = S; str += "<br>";
		MPF.PrintBLF(this, nullptr, S, HR->ute);
		str += S; str += "<br>";
		sprintf(S, "%18.6lf", forH ? HR->H : HR->R);
		str += S; str += "<br>";
		MPF.PrintBLF(this, nullptr, S, HR->B);
		str += S; str += "<br>";
		MPF.PrintBLF(this, nullptr, S, HR->L);
		str += S;
		I(j)(i + 2).n = "td";
		I(j)(i + 2).txt = str;
	}
}

//-------------------------------------------------------------------------------
void ZCPrintManager::TablApeakHtmlGroupCoord(ZСHtmlItem& I,
	const int  j,
	const int  k,
	const int  countPSB)
{
	// Новая строка 
	I(j).n = "tr";
	// Имена переменных
	I(j)(0).n = "th";
	I(j)(0).txt = "X<br>Y<br>Z<br>V<sub>x</sub><br>V<sub>y</sub><br>V<sub>z</sub>";
	if (k<4) {
		if (!k) I(j)(0).txt += "<br>a<br>e";
		I(j)(0).txt += "<br>i<br><font face=\"symbol\">W</font><br><font face=\"symbol\">w</font><br>u";
	}
	// Размерности	
	I(j)(1).n = "th";
	I(j)(1).txt = "км<br>км<br>км<br>м/сек<br>м/сек<br>м/сек";
	if (k<4) {
		if (!k) I(j)(1).txt += "<br>км<br>";
		I(j)(1).txt += "<br>град<br>град<br>град<br>град";
	}
	// Значения переменных
	char S[128];
	string str;
	for (int i = 0; i < countPSB; i++) {
		double *ASK, *OSK;
		switch (k) {
		case 0: ASK = PSB[i].ASK1975; OSK = PSB[i].OSK1975; break;
		case 1: ASK = PSB[i].ASK2000; OSK = PSB[i].OSK2000; break;
		case 2: ASK = PSB[i].ASKTE;   OSK = PSB[i].OSKTE; break;
		case 3: ASK = PSB[i].ASKTES;  OSK = PSB[i].OSKTES; break;
		case 4: ASK = PSB[i].GSK;     OSK = nullptr; break;
		default: assert(0);
		}
		sprintf(S, "%18.6lf<br>", TO_KM(ASK[0])); str = S;
		sprintf(S, "%18.6lf<br>", TO_KM(ASK[1])); str += S;
		sprintf(S, "%18.6lf<br>", TO_KM(ASK[2])); str += S;
		sprintf(S, "%18.6lf<br>", 1e3*TO_KMS(ASK[3])); str += S;
		sprintf(S, "%18.6lf<br>", 1e3*TO_KMS(ASK[4])); str += S;
		sprintf(S, "%18.6lf<br>", 1e3*TO_KMS(ASK[5])); str += S;
		if (OSK) {
			if (!k) {
				sprintf(S, "%18.8lf<br>", TO_KM(OSK[0])); str += S;
				sprintf(S, "%18.9lf<br>", OSK[1]); str += S;
			}
			MPF.PrintGrad(this, nullptr, S, OSK[2]); str += S; str += "<br>";
			MPF.PrintGrad(this, nullptr, S, OSK[3]); str += S; str += "<br>";
			MPF.PrintGrad(this, nullptr, S, OSK[4]); str += S; str += "<br>";
			MPF.PrintGrad(this, nullptr, S, OSK[5]); str += S;
		}
		I(j)(i + 2).n = "td";
		I(j)(i + 2).txt = str;
	}
}

//-------------------------------------------------------------------------------
int ZCPrintManager::PrintApeakMSChtml(ZMSC* pKA, ZSC_SI* FS, ZSC_VI* FV)
{
	if (!HtmlCreator)
		// Если объект "писатель" не создан, то понятно что выводить
		// информацию в html-формате не возможно - завершаем процедулу,
		// не считая это ошибкой, так как ситуация не фатальная.
		return 0;

	// Создание заголовка таблицы
	int i, j = -1;
	if (!MPF.IsFirstLine()) {
		ZSMarkItem im = HtmlCreator->GetItemsMark("PredRez");
		j = im[0];
	}
	if (MPF.IsFirstLine() || j == -1) {
		// Индекс нового элемента документа
		j = HtmlCreator->CountItems();
		(*HtmlCreator)(j).n = "br"; j++;
		(*HtmlCreator)(j).n = "br"; j++;
		// Создаём маркер нового элемента - таблицы с параметрами движения
		HtmlCreator->CreateItemsMark("PredRez", ZSMarkItem(j));
		// Собственно создание самой таблицы
		ZСHtmlItem& I = (*HtmlCreator)(j);
		I.n = "table";
		I[0] << "class\n predByVit";
	}

	ZСHtmlItem& I = (*HtmlCreator)(j);
	j = I.CountItems();

	//-------------------------------------------------------------------------
	I(j).n = "tr";
	I(j)(0).n = "th";
	I(j)(0).txt = "Виток<br>Дата<br>Время<br>";
	I(j)(0)[0] << "colspan \n 2";

	char S[256], sd[64], st[64];
	for (i = 0; i < countPSB; i++) {
		// Преобразование времени заданного в ссс в
		// строки структурированных даты и времени
		MPF.FDataTime.SetValue(PSB[i].ta);
		MPF.FDataTime.GetTimeText(st);
		MPF.FDataTime.GetDataText(sd);
		// Текст для ячейка
		sprintf(S, "%18ld<br>%18s<br>%18s", PSB[i].VitTE, sd, st);
		// Новая ячейка
		I(j)(1 + i).n = "th";
		I(j)(1 + i).txt = S;
	}
	j++;

	//-------------------------------------------------------------------------
	I(j).n = "tr";
	I(j)(0).n = "th";
	I(j)(0).txt = "Масса КА,";
	I(j)(1).n = "th";
	I(j)(1).txt = "кг";

	for (i = 0; i < countPSB; i++) {
		MPF.PrintM(this, nullptr, S, PSB[i].m);
		I(j)(2 + i).n = "td";
		I(j)(2 + i).txt = S;
	}
	j++;

	//-------------------------------------------------------------------------
	string str = "";
	if (MPK.fromVit) {
		// Новая строка 
		TablApeakHtmlGroupHeader(I, j, "Параметры витка", countPSB);
		j++;

		// Новая строка 
		I(j).n = "tr";
		// Имена переменных
		str = "T<sub>др</sub><br>T<sub>оск</sub><br>МССВ<br>H<sub>ср</sub><br>H<sub>ву</sub><br>L<sub>ву</sub><br>L<sub>ну</sub><br>L<sub>мв</sub><br>L<sub>сут</sub><br>Витков/сут";
		I(j)(0).n = "th";
		I(j)(0).txt = str;
		// Размерности
		str = "сек<br>сек<br> <br>км<br>км<br>град<br>град<br>град<br>град<br> <br>";
		I(j)(1).n = "th";
		I(j)(1).txt = str;
		// Значения переменных
		for (i = 0; i < countPSB; i++) {
			MPF.PrintTdr(this, nullptr, S, PVB[i].Td);
			str = S; str += "<br>";
			MPF.PrintTdr(this, nullptr, S, PSB[i].THBLFR[0]);
			str += S; str += "<br>";
			MPF.Ftmss << PSB[i].tmss;
			sprintf(S, "%18s", MPF.Ftmss.GetText());
			str += S; str += "<br>";
			sprintf(S, "%18.6lf", PVB[i].HAvr);
			str += S; str += "<br>";
			sprintf(S, "%18.6lf", PSB[i].THBLFR[1]);
			str += S; str += "<br>";
			MPF.PrintBLF(this, nullptr, S, PVB[i].UN.L);
			str += S; str += "<br>";
			MPF.PrintBLF(this, nullptr, S, PVB[i].DN.L);
			str += S; str += "<br>";
			MPF.PrintBLF(this, nullptr, S, PVB[i].dL);
			str += S; str += "<br>";
			MPF.PrintBLF(this, nullptr, S, PVB[i].dLs);
			str += S; str += "<br>";
			sprintf(S, "%18d", PVB[i].Ks);
			str += S;

			I(j)(2 + i).n = "td";
			I(j)(2 + i).txt = str;
		}
		j++;

		int k;
		for (k = 0; k < 2; k++) {
			TablApeakHtmlGroupHeader(I, j, k ? "H<sub>макс</sub>" : "H<sub>мин</sub>", countPSB);
			j++;
			TablApeakHtmlGroupHBLRH(I, j, true, k ? true : false, countPSB);
			j++;
		}

		for (k = 0; k < 2; k++) {
			TablApeakHtmlGroupHeader(I, j, k ? "R<sub>макс</sub>" : "R<sub>мин</sub>", countPSB);
			j++;
			TablApeakHtmlGroupHBLRH(I, j, false, k ? true : false, countPSB);
			j++;
		}
	}
	else {
		// Новая строка 
		TablApeakHtmlGroupHeader(I, j, "Трасса полёта", countPSB);
		j++;

		// Новая строка 
		I(j).n = "tr";
		// Имена переменных
		str = "Tоск<br>H<br>L<br>B<br><font face=\"symbol\">f</font>";
		I(j)(0).n = "th";
		I(j)(0).txt = str;
		// Размерности
		str = "сек<br>км<br>град<br>град<br>град";
		I(j)(1).n = "th";
		I(j)(1).txt = str;
		// Значения переменных
		for (i = 0; i < countPSB; i++) {
			MPF.PrintTdr(this, nullptr, S, PSB[i].THBLFR[0]); str = S; str += "<br>";
			sprintf(S, "%18.4lf<br>", PSB[i].THBLFR[1]); str += S;
			MPF.PrintBLF(this, nullptr, S, PSB[i].THBLFR[3]); str += S; str += "<br>";
			MPF.PrintBLF(this, nullptr, S, PSB[i].THBLFR[2]); str += S; str += "<br>";
			MPF.PrintBLF(this, nullptr, S, PSB[i].THBLFR[4]); str += S;
			I(j)(i + 2).n = "td";
			I(j)(i + 2).txt = str;
		}
		j++;
	}

	//-------------------------------------------------------------------------
	TablApeakHtmlGroupHeader(I, j, "АСК 1975", countPSB);
	j++;
	TablApeakHtmlGroupCoord(I, j, 0, countPSB);
	j++;

	//-------------------------------------------------------------------------
	TablApeakHtmlGroupHeader(I, j, "АСК 2000", countPSB);
	j++;
	TablApeakHtmlGroupCoord(I, j, 1, countPSB);
	j++;

	//-------------------------------------------------------------------------
	TablApeakHtmlGroupHeader(I, j, "АСК ТЭ", countPSB);
	j++;
	TablApeakHtmlGroupCoord(I, j, 2, countPSB);
	j++;

	//-------------------------------------------------------------------------
	TablApeakHtmlGroupHeader(I, j, "АСК ТЭС", countPSB);
	j++;
	TablApeakHtmlGroupCoord(I, j, 3, countPSB);
	j++;

	//-------------------------------------------------------------------------
	TablApeakHtmlGroupHeader(I, j, "ГСК", countPSB);
	j++;
	TablApeakHtmlGroupCoord(I, j, 4, countPSB);
	j++;

	return 0;
}

//-------------------------------------------------------------------------------
