//-----------------------------------------------------------------------------
// ZCPrintManagerApeakHtml.cpp
// �. ��������� 03.08.2018
// ������������ ���������������� ����������� ��������
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

#include <FlyMMath.h>		// �������������� �������
#include <FlyTime.h>		// �� ��� �����
#include <FlyHtmlWriter.h>	// ��� ���������������� � ������� Html

//-----------------------------------------------------------------------------
#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZCPrintManager.h>
#include <FlyCoreSource\\ZCPrintFormat.h>

//-------------------------------------------------------------------------------
void ZCPrintManager::TablApeakHtmlGroupHeader(Z�HtmlItem& I,
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
void ZCPrintManager::TablApeakHtmlGroupHBLRH(Z�HtmlItem& I,
	const int  j,
	const bool forH,
	const bool forMax,
	const int  countPSB)
{
	// ����� ������ 
	I(j).n = "tr";
	// ����� ����������
	I(j)(0).n = "th";
	I(j)(0).txt = forH ? "�����<br>u(�.�.)<br>H<br>B<br>L" : "�����<br>u(�.�.)<br>R<br>B<br>L";
	// �����������
	I(j)(1).n = "th";
	I(j)(1).txt = "<br>����<br>��<br>����<br>����";
	// �������� ����������
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
void ZCPrintManager::TablApeakHtmlGroupCoord(Z�HtmlItem& I,
	const int  j,
	const int  k,
	const int  countPSB)
{
	// ����� ������ 
	I(j).n = "tr";
	// ����� ����������
	I(j)(0).n = "th";
	I(j)(0).txt = "X<br>Y<br>Z<br>V<sub>x</sub><br>V<sub>y</sub><br>V<sub>z</sub>";
	if (k<4) {
		if (!k) I(j)(0).txt += "<br>a<br>e";
		I(j)(0).txt += "<br>i<br><font face=\"symbol\">W</font><br><font face=\"symbol\">w</font><br>u";
	}
	// �����������	
	I(j)(1).n = "th";
	I(j)(1).txt = "��<br>��<br>��<br>�/���<br>�/���<br>�/���";
	if (k<4) {
		if (!k) I(j)(1).txt += "<br>��<br>";
		I(j)(1).txt += "<br>����<br>����<br>����<br>����";
	}
	// �������� ����������
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
		// ���� ������ "��������" �� ������, �� ������� ��� ��������
		// ���������� � html-������� �� �������� - ��������� ���������,
		// �� ������ ��� �������, ��� ��� �������� �� ���������.
		return 0;

	// �������� ��������� �������
	int i, j = -1;
	if (!MPF.IsFirstLine()) {
		ZSMarkItem im = HtmlCreator->GetItemsMark("PredRez");
		j = im[0];
	}
	if (MPF.IsFirstLine() || j == -1) {
		// ������ ������ �������� ���������
		j = HtmlCreator->CountItems();
		(*HtmlCreator)(j).n = "br"; j++;
		(*HtmlCreator)(j).n = "br"; j++;
		// ������ ������ ������ �������� - ������� � ����������� ��������
		HtmlCreator->CreateItemsMark("PredRez", ZSMarkItem(j));
		// ���������� �������� ����� �������
		Z�HtmlItem& I = (*HtmlCreator)(j);
		I.n = "table";
		I[0] << "class\n predByVit";
	}

	Z�HtmlItem& I = (*HtmlCreator)(j);
	j = I.CountItems();

	//-------------------------------------------------------------------------
	I(j).n = "tr";
	I(j)(0).n = "th";
	I(j)(0).txt = "�����<br>����<br>�����<br>";
	I(j)(0)[0] << "colspan \n 2";

	char S[256], sd[64], st[64];
	for (i = 0; i < countPSB; i++) {
		// �������������� ������� ��������� � ��� �
		// ������ ����������������� ���� � �������
		MPF.FDataTime.SetValue(PSB[i].ta);
		MPF.FDataTime.GetTimeText(st);
		MPF.FDataTime.GetDataText(sd);
		// ����� ��� ������
		sprintf(S, "%18ld<br>%18s<br>%18s", PSB[i].VitTE, sd, st);
		// ����� ������
		I(j)(1 + i).n = "th";
		I(j)(1 + i).txt = S;
	}
	j++;

	//-------------------------------------------------------------------------
	I(j).n = "tr";
	I(j)(0).n = "th";
	I(j)(0).txt = "����� ��,";
	I(j)(1).n = "th";
	I(j)(1).txt = "��";

	for (i = 0; i < countPSB; i++) {
		MPF.PrintM(this, nullptr, S, PSB[i].m);
		I(j)(2 + i).n = "td";
		I(j)(2 + i).txt = S;
	}
	j++;

	//-------------------------------------------------------------------------
	string str = "";
	if (MPK.fromVit) {
		// ����� ������ 
		TablApeakHtmlGroupHeader(I, j, "��������� �����", countPSB);
		j++;

		// ����� ������ 
		I(j).n = "tr";
		// ����� ����������
		str = "T<sub>��</sub><br>T<sub>���</sub><br>����<br>H<sub>��</sub><br>H<sub>��</sub><br>L<sub>��</sub><br>L<sub>��</sub><br>L<sub>��</sub><br>L<sub>���</sub><br>������/���";
		I(j)(0).n = "th";
		I(j)(0).txt = str;
		// �����������
		str = "���<br>���<br> <br>��<br>��<br>����<br>����<br>����<br>����<br> <br>";
		I(j)(1).n = "th";
		I(j)(1).txt = str;
		// �������� ����������
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
			TablApeakHtmlGroupHeader(I, j, k ? "H<sub>����</sub>" : "H<sub>���</sub>", countPSB);
			j++;
			TablApeakHtmlGroupHBLRH(I, j, true, k ? true : false, countPSB);
			j++;
		}

		for (k = 0; k < 2; k++) {
			TablApeakHtmlGroupHeader(I, j, k ? "R<sub>����</sub>" : "R<sub>���</sub>", countPSB);
			j++;
			TablApeakHtmlGroupHBLRH(I, j, false, k ? true : false, countPSB);
			j++;
		}
	}
	else {
		// ����� ������ 
		TablApeakHtmlGroupHeader(I, j, "������ �����", countPSB);
		j++;

		// ����� ������ 
		I(j).n = "tr";
		// ����� ����������
		str = "T���<br>H<br>L<br>B<br><font face=\"symbol\">f</font>";
		I(j)(0).n = "th";
		I(j)(0).txt = str;
		// �����������
		str = "���<br>��<br>����<br>����<br>����";
		I(j)(1).n = "th";
		I(j)(1).txt = str;
		// �������� ����������
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
	TablApeakHtmlGroupHeader(I, j, "��� 1975", countPSB);
	j++;
	TablApeakHtmlGroupCoord(I, j, 0, countPSB);
	j++;

	//-------------------------------------------------------------------------
	TablApeakHtmlGroupHeader(I, j, "��� 2000", countPSB);
	j++;
	TablApeakHtmlGroupCoord(I, j, 1, countPSB);
	j++;

	//-------------------------------------------------------------------------
	TablApeakHtmlGroupHeader(I, j, "��� ��", countPSB);
	j++;
	TablApeakHtmlGroupCoord(I, j, 2, countPSB);
	j++;

	//-------------------------------------------------------------------------
	TablApeakHtmlGroupHeader(I, j, "��� ���", countPSB);
	j++;
	TablApeakHtmlGroupCoord(I, j, 3, countPSB);
	j++;

	//-------------------------------------------------------------------------
	TablApeakHtmlGroupHeader(I, j, "���", countPSB);
	j++;
	TablApeakHtmlGroupCoord(I, j, 4, countPSB);
	j++;

	return 0;
}

//-------------------------------------------------------------------------------
