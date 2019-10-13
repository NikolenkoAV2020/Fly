//-----------------------------------------------------------------------------
// ZCPrintManagerApeakText.cpp
// А. Николенко 03.08.2018
// Вертикальное документирование результатов прогноза
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZCPrintManager.h>
#include <FlyCoreSource\\ZCPrintFormat.h>

//-------------------------------------------------------------------------------
// "Вертикальное", то есть в столбик, документирование параметров движения
//		pKA - модель движения (КА) для которой выполняется документирование,
//		FS  - параметры движения на текущий момент времени,
//		FV  - параметры движения на текущий виток
//		fp  - текстовый файл в который выдаются результаты.
//-------------------------------------------------------------------------------
int ZCPrintManager::PrintApeakMSCtext(
	ZMSC   *pKA,	// модель движения(КА) для которой выполняется документирование,
	ZSC_SI *FS,		// параметры движения на текущий момент времени,
	ZSC_VI *FV,		// параметры движения на текущий виток
	FILE   *fp)		// текстовый файл в который выдаются результаты.
{
	char TimeString[SIZE_PSB][30];
	int i, N = countPSB;

	if (MPF.IsFirstLine()) 	PrintLine(fp);

	fprintf(fp, "  Виток     ");
	for (i = 0; i<countPSB; i++) fprintf(fp, "%18ld", PSB[i].VitTE); fprintf(fp, "\n");
	fprintf(fp, "  Дата      ");
	for (i = 0; i<countPSB; i++) {
		char s[25];
		MPF.FDataTime.SetValue(PSB[i].ta);
		MPF.FDataTime.GetTimeText(TimeString[i]);
		MPF.FDataTime.GetDataText(s);
		fprintf(fp, "%18s", s);
	}
	fprintf(fp, "\n");
	fprintf(fp, "  Время     ");
	for (i = 0; i<countPSB; i++) fprintf(fp, "%18s", TimeString[i]); fprintf(fp, "\n");
	PrintLine(fp);

	fprintf(fp, "  Масса КА  ");
	for (i = 0; i<countPSB; i++) MPF.PrintM(this, fp, nullptr, PSB[i].m); fprintf(fp, "\n");

	if (MPK.fromVit) {
		fprintf(fp, "  Tдр, сек  ");
		for (i = 0; i<countPSB; i++) MPF.PrintTdr(this, fp, nullptr, PVB[i].Td); fprintf(fp, "\n");
		fprintf(fp, "  Tоск,сек  ");
		for (i = 0; i<countPSB; i++) MPF.PrintTdr(this, fp, nullptr, PSB[i].THBLFR[0]); fprintf(fp, "\n");
		fprintf(fp, "  МССВ      ");
		for (i = 0; i<countPSB; i++) {
			MPF.Ftmss << PSB[i].tmss;
			fprintf(fp, "%18s", MPF.Ftmss.GetText());
		}
		fprintf(fp, "\n");
		fprintf(fp, "  Hср, км   ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", PVB[i].HAvr); fprintf(fp, "\n");
		fprintf(fp, "  Hву, км   ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", PSB[i].THBLFR[1]); fprintf(fp, "\n");
		fprintf(fp, "  Lву, град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, PVB[i].UN.L); fprintf(fp, "\n");
		fprintf(fp, "  Lну, град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, PVB[i].DN.L); fprintf(fp, "\n");
		fprintf(fp, "  Lмв, град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, PVB[i].dL); fprintf(fp, "\n");
		fprintf(fp, "  Lсут,град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, PVB[i].dLs); fprintf(fp, "\n");
		fprintf(fp, "  Витков/сут");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18d", PVB[i].Ks); fprintf(fp, "\n");

		ZSC_VP* Hmin[MAX_APEAK_COLUMN];
		ZSC_VP* Hmax[MAX_APEAK_COLUMN];
		ZSC_VP* Rmin[MAX_APEAK_COLUMN];
		ZSC_VP* Rmax[MAX_APEAK_COLUMN];

		for (i = 0; i<countPSB; i++) {
			Hmin[i] = HMIN_VI(PVB[i]);
			Hmax[i] = HMAX_VI(PVB[i]);
			Rmin[i] = RMIN_VI(PVB[i]);
			Rmax[i] = RMAX_VI(PVB[i]);
		}

		fprintf(fp, "  Hmin      "); fprintf(fp, "\n");
		fprintf(fp, "     Время  ");
		for (i = 0; i<countPSB; i++) {
			MPF.FDataTime << Hmin[i]->t;
			MPF.FDataTime.GetTimeText(TimeString[i]);
			fprintf(fp, "%18s", TimeString[i]);
		}
		fprintf(fp, "\n");
		fprintf(fp, "     u(т.е.)");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Hmin[i]->ute); fprintf(fp, "\n");
		fprintf(fp, "     H,км   ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", Hmin[i]->H); fprintf(fp, "\n");
		fprintf(fp, "     B,град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Hmin[i]->B); fprintf(fp, "\n");
		fprintf(fp, "     L,град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Hmin[i]->L); fprintf(fp, "\n");

		fprintf(fp, "  Hmax      "); fprintf(fp, "\n");
		fprintf(fp, "     Время  ");
		for (i = 0; i<countPSB; i++) {
			MPF.FDataTime << Hmax[i]->t;
			MPF.FDataTime.GetTimeText(TimeString[i]);
			fprintf(fp, "%18s", TimeString[i]);
		}
		fprintf(fp, "\n");
		fprintf(fp, "     u(т.е.)");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Hmax[i]->ute); fprintf(fp, "\n");
		fprintf(fp, "     H,км   ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", Hmax[i]->H); fprintf(fp, "\n");
		fprintf(fp, "     B,град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Hmax[i]->B); fprintf(fp, "\n");
		fprintf(fp, "     L,град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Hmax[i]->L); fprintf(fp, "\n");

		fprintf(fp, "  Rmin      "); fprintf(fp, "\n");
		fprintf(fp, "     Время  ");
		for (i = 0; i<countPSB; i++) {
			MPF.FDataTime << Rmin[i]->t;
			MPF.FDataTime.GetTimeText(TimeString[i]);
			fprintf(fp, "%18s", TimeString[i]);
		}
		fprintf(fp, "\n");
		fprintf(fp, "     u(т.е.)");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Rmin[i]->ute); fprintf(fp, "\n");
		fprintf(fp, "     R,км   ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", Rmin[i]->R); fprintf(fp, "\n");
		fprintf(fp, "     B,град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Rmin[i]->B); fprintf(fp, "\n");
		fprintf(fp, "     L,град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Rmin[i]->L); fprintf(fp, "\n");

		fprintf(fp, "  Rmax      "); fprintf(fp, "\n");
		fprintf(fp, "     Время  ");
		for (i = 0; i<countPSB; i++) {
			MPF.FDataTime << Rmax[i]->t;
			MPF.FDataTime.GetTimeText(TimeString[i]);
			fprintf(fp, "%18s", TimeString[i]);
		}
		fprintf(fp, "\n");
		fprintf(fp, "     u(т.е.)");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Rmax[i]->ute); fprintf(fp, "\n");
		fprintf(fp, "     R,км   ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", Rmax[i]->R); fprintf(fp, "\n");
		fprintf(fp, "     B,град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Rmax[i]->B); fprintf(fp, "\n");
		fprintf(fp, "     L,град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, Rmax[i]->L); fprintf(fp, "\n");

		PrintLine(fp);
	}
	else {
		fprintf(fp, "\n");
		fprintf(fp, " Трасса полёта\n");
		fprintf(fp, "  H,   км   ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.4lf", PSB[i].THBLFR[1]); fprintf(fp, "\n");
		fprintf(fp, "  L,   град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, PSB[i].THBLFR[3]); fprintf(fp, "\n");
		fprintf(fp, "  В,   град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, PSB[i].THBLFR[2]); fprintf(fp, "\n");
		fprintf(fp, "  f,   град ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, PSB[i].THBLFR[4]); fprintf(fp, "\n");
	}

	//-------------------------------------------------------------------------
	// Документирование декатровых координат и оскулирующих элементов 
	// в различных эпохах
	//-------------------------------------------------------------------------

	fprintf(fp, " АСК 1975\n");
	fprintf(fp, "  X, км     ");
	for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASK1975[0])); fprintf(fp, "\n");
	fprintf(fp, "  Y, км     ");
	for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASK1975[1])); fprintf(fp, "\n");
	fprintf(fp, "  Z, км     ");
	for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASK1975[2])); fprintf(fp, "\n");
	fprintf(fp, "  Vx,м/сек  ");
	for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASK1975[3])); fprintf(fp, "\n");
	fprintf(fp, "  Vy,м/сек  ");
	for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASK1975[4])); fprintf(fp, "\n");
	fprintf(fp, "  Vz,м/сек  ");
	for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASK1975[5])); fprintf(fp, "\n");

	bool aeIsPrint = false;

	if (1) {
		fprintf(fp, "  a, км     ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.8lf", TO_KM(PSB[i].OSK1975[0])); fprintf(fp, "\n");
		fprintf(fp, "  e         ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.9lf", PSB[i].OSK1975[1]); fprintf(fp, "\n");
		fprintf(fp, "  i, град   ");
		for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSK1975[2]); fprintf(fp, "\n");
		fprintf(fp, "  W, град   ");
		for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSK1975[3]); fprintf(fp, "\n");
		fprintf(fp, "  w, град   ");
		for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSK1975[4]); fprintf(fp, "\n");
		fprintf(fp, "  u, град   ");
		for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSK1975[5]); fprintf(fp, "\n");
		aeIsPrint = true;
	}

	if (1) {
		fprintf(fp, " АСК 2000\n");
		if (1) {
			fprintf(fp, "  X, км     ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASK2000[0])); fprintf(fp, "\n");
			fprintf(fp, "  Y, км     ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASK2000[1])); fprintf(fp, "\n");
			fprintf(fp, "  Z, км     ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASK2000[2])); fprintf(fp, "\n");
			fprintf(fp, "  Vx,м/сек  ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASK2000[3])); fprintf(fp, "\n");
			fprintf(fp, "  Vy,м/сек  ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASK2000[4])); fprintf(fp, "\n");
			fprintf(fp, "  Vz,м/сек  ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASK2000[5])); fprintf(fp, "\n");
		}
		if (1) {
			if (!aeIsPrint) {
				aeIsPrint = true;
				fprintf(fp, "  a, км     ");
				for (i = 0; i<countPSB; i++) fprintf(fp, "%18.8lf", TO_KM(PSB[i].OSK2000[0])); fprintf(fp, "\n");
				fprintf(fp, "  e         ");
				for (i = 0; i<countPSB; i++) fprintf(fp, "%18.9lf", PSB[i].OSK2000[1]); fprintf(fp, "\n");
			}
			fprintf(fp, "  i, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSK2000[2]); fprintf(fp, "\n");
			fprintf(fp, "  W, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSK2000[3]); fprintf(fp, "\n");
			fprintf(fp, "  w, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSK2000[4]); fprintf(fp, "\n");
			fprintf(fp, "  u, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSK2000[5]); fprintf(fp, "\n");
		}
	}

	if (1) {
		fprintf(fp, " АСК ТЭ\n");
		if (1) {
			fprintf(fp, "  X, км     ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASKTE[0])); fprintf(fp, "\n");
			fprintf(fp, "  Y, км     ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASKTE[1])); fprintf(fp, "\n");
			fprintf(fp, "  Z, км     ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASKTE[2])); fprintf(fp, "\n");
			fprintf(fp, "  Vx,м/сек  ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASKTE[3])); fprintf(fp, "\n");
			fprintf(fp, "  Vy,м/сек  ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASKTE[4])); fprintf(fp, "\n");
			fprintf(fp, "  Vz,м/сек  ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASKTE[5])); fprintf(fp, "\n");
		}
		if (1) {
			if (!aeIsPrint) {
				aeIsPrint = true;
				fprintf(fp, "  a, км     ");
				for (i = 0; i<countPSB; i++) fprintf(fp, "%18.8lf", TO_KM(PSB[i].OSKTE[0])); fprintf(fp, "\n");
				fprintf(fp, "  e         ");
				for (i = 0; i<countPSB; i++) fprintf(fp, "%18.9lf", PSB[i].OSKTE[1]); fprintf(fp, "\n");
			}
			fprintf(fp, "  i, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSKTE[2]); fprintf(fp, "\n");
			fprintf(fp, "  W, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSKTE[3]); fprintf(fp, "\n");
			fprintf(fp, "  w, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSKTE[4]); fprintf(fp, "\n");
			fprintf(fp, "  u, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSKTE[5]); fprintf(fp, "\n");
		}
	}

	if (1) {
		fprintf(fp, " АСК ТЭС\n");
		if (1) {
			fprintf(fp, "  X, км     ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASKTES[0])); fprintf(fp, "\n");
			fprintf(fp, "  Y, км     ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASKTES[1])); fprintf(fp, "\n");
			fprintf(fp, "  Z, км     ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].ASKTES[2])); fprintf(fp, "\n");
			fprintf(fp, "  Vx,м/сек  ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASKTES[3])); fprintf(fp, "\n");
			fprintf(fp, "  Vy,м/сек  ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASKTES[4])); fprintf(fp, "\n");
			fprintf(fp, "  Vz,м/сек  ");
			for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].ASKTES[5])); fprintf(fp, "\n");
		}
		if (1) {
			if (!aeIsPrint) {
				aeIsPrint = true;
				fprintf(fp, "  a, км     ");
				for (i = 0; i<countPSB; i++) fprintf(fp, "%18.8lf", TO_KM(PSB[i].OSKTES[0])); fprintf(fp, "\n");
				fprintf(fp, "  e         ");
				for (i = 0; i<countPSB; i++) fprintf(fp, "%18.9lf", PSB[i].OSKTES[1]); fprintf(fp, "\n");
			}
			fprintf(fp, "  i, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSKTES[2]); fprintf(fp, "\n");
			fprintf(fp, "  W, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSKTES[3]); fprintf(fp, "\n");
			fprintf(fp, "  w, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSKTES[4]); fprintf(fp, "\n");
			fprintf(fp, "  u, град   ");
			for (i = 0; i<countPSB; i++) MPF.PrintGrad(this, fp, nullptr, PSB[i].OSKTES[5]); fprintf(fp, "\n");
		}
	}

	if (1) {
		fprintf(fp, " ГСК\n");
		fprintf(fp, "  X, км     ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].GSK[0])); fprintf(fp, "\n");
		fprintf(fp, "  Y, км     ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].GSK[1])); fprintf(fp, "\n");
		fprintf(fp, "  Z, км     ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", TO_KM(PSB[i].GSK[2])); fprintf(fp, "\n");
		fprintf(fp, "  Vx,м/сек  ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].GSK[3])); fprintf(fp, "\n");
		fprintf(fp, "  Vy,м/сек  ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].GSK[4])); fprintf(fp, "\n");
		fprintf(fp, "  Vz,м/сек  ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", 1e3*TO_KMS(PSB[i].GSK[5])); fprintf(fp, "\n");
	}

	if (!MPK.fromVit) {
		fprintf(fp, "\n");
		fprintf(fp, "  Tоск,сек  ");
		for (i = 0; i<countPSB; i++) MPF.PrintTdr(this, fp, nullptr, PSB[i].THBLFR[0]); fprintf(fp, "\n");
		fprintf(fp, "  H,км      ");
		for (i = 0; i<countPSB; i++) fprintf(fp, "%18.4lf", PSB[i].THBLFR[1]); fprintf(fp, "\n");
		fprintf(fp, "  L,град    ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, PSB[i].THBLFR[3]); fprintf(fp, "\n");
		fprintf(fp, "  B,град    ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, PSB[i].THBLFR[2]); fprintf(fp, "\n");
		fprintf(fp, "  f,град    ");
		for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, PSB[i].THBLFR[4]); fprintf(fp, "\n");
	}

	PrintLine(fp);

	return 0;
}

//-------------------------------------------------------------------------------
void ZCPrintManager::PrintApeakLineMSC(FILE* fp)
{
	// Вычисление длинны Len горизонтальной черты в соответствии с заданным  
	// набором документируемых параметров вертикальной печати 
	// результатов
	int Len = MPF.CalcLineLenght(this);
	// Печать горизонтальной черты заданной длинны Len
	MPF.PrintLineAflet(fp, Len);
}

//-------------------------------------------------------------------------------
void ZCPrintManager::PrintLine(FILE* fp)
{
	fprintf(fp, "____________");
	for (int i = 0; i<countPSB; i++) fprintf(fp, "__________________");
	fprintf(fp, "\n");
}

//-------------------------------------------------------------------------------
void ZCPrintManager::PrintVitPoint(FILE* fp, ZSC_VP& VP)
{
	int  i;
	char TimeString[30];
	fprintf(fp, "     Время  ");
	for (i = 0; i<countPSB; i++) {
		MPF.FDataTime.SetValue(VP.t);
		MPF.FDataTime.GetTimeText(TimeString);
		fprintf(fp, "%18s", TimeString);
	}
	fprintf(fp, "\n");
	fprintf(fp, "     u(т.е.)");
	for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, VP.ute); fprintf(fp, "\n");
	fprintf(fp, "     H      ");
	for (i = 0; i<countPSB; i++) fprintf(fp, "%18.6lf", VP.H); fprintf(fp, "\n");
	fprintf(fp, "     B      ");
	for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, VP.B); fprintf(fp, "\n");
	fprintf(fp, "     L      ");
	for (i = 0; i<countPSB; i++) MPF.PrintBLF(this, fp, nullptr, VP.L); fprintf(fp, "\n");
}

//-------------------------------------------------------------------------------
