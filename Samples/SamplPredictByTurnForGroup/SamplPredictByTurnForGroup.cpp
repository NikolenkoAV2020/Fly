//-----------------------------------------------------------------------------
// SamplPredictByTurnForGroup.cpp
// �. ��������� 14.06.2019
//
// ������� �� ������ ������ �� 8-� ��
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>		// ����������� ���� ������ ��������
#include <FlyMMath.h>		// �������������� �������
#include <FlyCoreLink.h>	// ����������� ���� ���������

int TaskTurnsPredictionGroup();

void main()
{
	// ����� �������� � ���������� ���������� 
	//setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// ������������� ���� Fly, ���������� ��������� ����� � ����������� 
	// �������������� ��������, ������������� ���������� ����������,
	// � ����������� �������� ������������� ���������� � ��...
	int rc = FlyGlobalInit(); if (rc) return;

	// ������ ��������������� �������� �� �� ������ ��� ������ ��
	TaskTurnsPredictionGroup();
}

//-----------------------------------------------------------------------------
// ������� �� ������ ������ �� 8-� ��
//		�������� ��������������� ������ � ������: 
//		ZNU    NU			- ��������� ������� - ���������
//		ZSC_SI SI			- �������� �������� �� - ���������
//		ZMSC   KA			- ������ ��������, ����� �������� � �� ���� ����� 
//							  ������ ������������ �������� (��) - �����
//		KA.Init(...)		- ������������� ������ �������� - ����� 
//		KA.GoToVit(...)		- ������������� �������� �� �� ������ ��������� 
//							  ����� - �����
//		KA.GoAlongVit(...)	- ������� ����� ����� � �������� ������������ 
//							- ���������� ����� - �����
//		ZCPrintManager PrintMng - ��������� ���������������� - �����
//-----------------------------------------------------------------------------
int TaskTurnsPredictionGroup()
{
	cout << "\n   Prediction by turns for 8 SC group...\n";
	int  j, rc = 0;
	long Vit;
	long Vitn = 100;	// ����� ������ �������������
	long Vitk = 105;	// ����� ���������� �������������

	PZCPrintManager PrintMng;	// ��������� ����������������
	PZMSC		    KA[8];		// ������ �������� ��
	ZNU             NU[8];		// ��������� ������� ��������

	//-------------------------------------------------------------------------
	// ���������� ����� ���
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
	// ����� ���������� ������ - ���
	// ��������� ������������� ������
	double T, i, a, p, e, e1, e2;
	// ������ �� ������� ������
	double Hs = 270;
	char   Msg[512];

	rc = MBuildSSOMIV_hs(Hs, T, i, p, e1, e2, Msg); RRC;
	e = sqrt(e1*e1 + e2*e2);
	a = p / (1 - e*e);

	//-------------------------------------------------------------------------
	// ������������ ������� ��
	for (j = 0; j<8; j++) {
		NU[j].Key.ka = j + 1; NU[j].Key.nom = 15; NU[j].Key.mod = 0;
		NU[j].Key.type = 8; NU[j].Key.bc = 1;
		NU[j].numlsf = 1;
		DATE dt = DATE(18, 7, 2018);
		TIME tm = TIME(9, 30, 0, 0);
		TA_DMB(1, &dt, &tm, &NU[j].t);
		NU[j].vit = 99;
		// ����������� �� - ��������� ��������� �� ����������� 
		// ����������� �������� ��� ���
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

		// ������������� ������ �������� ��� ������� �� �������� �� (NU)
		// � �������� ���� ��� (LSF)
		rc = KA[j]->Init(NU[j], &LSF); if (rc) return rc;
	}

	//-------------------------------------------------------------------------
	// �������� ������� ���������������� ����������� ��������

	// �������������� ���������������� ���������� �������� �� ���� � ������
	PrintMng->MPK.AflatOn();
	// ���������������� �� ������
	PrintMng->MPK.FromVitOn();
	// ������ ��������������� ����������
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

	// �������� ���������� ��������� ��� ���������������� ����������� ��������
	PrintMng->PrePrint(&KA[0], "�������", "_TaskTurnsPredictionGroup");
	// ��������� ���������� ���������
	PrintMng->PrintHeaderDoc("\n   ������� ���������� �������� �� ������\n   ��� ������ ��\n\n");
	// ���������������� �� � ���������� ����� ���
	for (j = 0; j<8; j++) PrintMng->PrintNU(NU[j], NULL, 3);
	PrintMng->PrintLSF(LSF, 1);

	// ��������� ��� ��������� ���� ��������� �������� �� �� ������ ����
	ZSC_SI SI;

	// ��������������� ��������� ���� �� �� ������ ��������� ����� - 
	// ������ ��� ���� ��.
	for (j = 7; j >= 0; j--) { rc = KA[j]->GoToVit(Vitn, &SI); RRC; }

	Vit = Vitn;
	cout << "      Initial tern " << Vitn << "\n";
	cout << "      Final   tern " << Vitk << "\n";

	// �������� ���� ������������� 
	while (1) {
		for (j = 7; j >= 0; j--) {
			cout << "\r         Current tern  " << Vit << "  KA  " << j + 1;
			// ������������� �������� �� ����� ����� Vit � �����������
			// ������������ ������������� ����� � ��������� �������� �
			// ����������� ������ ����� (���������� � ���������� ����)
			rc = KA[j]->GoAlongVit(Vit); if (rc) break;
			// ���������������� �������� ��������� ��
			PrintMng->PrintMSC(&KA[j]);
		}
		PrintMng->PrintLineMSC();
		Vit++;
		if (Vit>Vitk) break;
	}
	cout << "\n";

	PrintMng->PostPrint();
	PrintMng->ClosePrint();

	// �������� ����������� ���������� ������������ �������
	PrintMng->ShowTextFile();
	PrintMng->ShowHtmlFile();

	return rc;
}

//-----------------------------------------------------------------------------
