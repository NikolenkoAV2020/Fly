//-----------------------------------------------------------------------------
// TaskMagicFlySys3SC.cpp : 
// �. ��������� 15.04.2019
//
// ������ ������� ������ ��������� ������� �� 3-� �� � ����� ���������
// � ����� ������������ ������ ��������� ������� ����� �� ��������� � ���������
// ����� �� 60 ��������.
// � �������� ������ ��� ������� �� ����������� 241 ������� L-������ ���
// �������� ���������� ������ ������������ ����� (�����).
// ��������� ����� ��� ������� �� ������������� �� ��������� ��������.
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>		// �������������� �������
#include <FlyCore.h>		// �������������� �������
#include <FlyOrbitDesign.h>	// �������������� �����
#include <FlyMagicLNS.h>	//

//-----------------------------------------------------------------------------
#include <FlyMagicLNSSource/MagicSolution.h>

//-----------------------------------------------------------------------------
int TaskMagicFlySys3SC(int TypeOrb)
{
	// ������������� ������ (����� ������� ����� �������� ��������������)
	int indexTask = 311;

	// ������ ������� ������
	ZSMagicLnsSolution* MS = CreateMagicLnsSolution(); if (!MS) return 1;
	MS->MZ.ORB() = SPO_ORB_EL;	// ������� ������ ������� e+w.
	MS->MZ.ks    = 0;			// ��� ��.

	//-------------------------------------------------------------------------
	// ��������� ����������������
	ZCPrintManager PrintMng;

	//-------------------------------------------------------------------------
	// ������ ���������������� .... �������� ���������

	// ���� ��������� 
	FILE* fp = nullptr;

	char NameOrb[64] = "_";
	switch (MS->MZ.ORB()) {
	case SPO_ORB_OMIV: sprintf(NameOrb, "OMIV\0"); break;
	case SPO_ORB_DU:   sprintf(NameOrb, "DU\0"); break;
	case SPO_ORB_EL:   sprintf(NameOrb, "EL\0"); break;
	case SPO_ORB_HBH:  sprintf(NameOrb, "HBH\0"); break;
	}
	char NameFile[256]; sprintf(NameFile, "_TaskMagicFlySys%d_%s\0", indexTask, NameOrb);
	PrintMng.PrePrint(nullptr, " ������ ��������� ������� ��", NameFile, &fp);

	//-------------------------------------------------------------------------
	int i, j, rc = 0;

	SmartPrintfS(fp, "  ������� ������ ��������� ������� �� �� ������� ��\n", true);
	SmartPrintfS(fp, "  �������� ���������� ������� ������������ �����,\n", true);
	SmartPrintfS(fp, "  ������������ ���� ������������ ����� �� ��������� ������.\n\n", true);

	// �� �������� �� �������.
	ZNU   NU[3];	
	ZNU   NU2[3];

	// �������� �� �� ��� ��01 
	cout << "  ��1:" << endl;
	rc = LoadNU01("����� ��01 312.txt", NULL, NULL, NU[0], nullptr, true); 
	if (rc) return rc;
	NU[0].M0 = 420.0;
	NU[0].MF = 50.0;

	cout << "  ��2:" << endl;
	rc = LoadNU01("����� ��01 313.txt", NULL, NULL, NU[1], nullptr, true); 
	if (rc) return rc;
	NU[1].M0 = 420.0;
	NU[1].MF = 50.0;

	cout << "  ��3:" << endl;
	rc = LoadNU01("����� ��01 314.txt", NULL, NULL, NU[2], nullptr, true); 
	if (rc) return rc;
	NU[2].M0 = 420.0;
	NU[2].MF = 50.0;

	// �������������� �� � ������������ �������� ������� �����
	for (i = 0; i < 3; i++) {
		NU2[i] = NU[i];
		rc = ConvertSkNu(NU2[i], _SK_OSKTE); if (rc) return rc;
	}

	// ����� ����� ������� ��
	double tmax =-9e99;
	// ����������� ������� ����� ������� ��
	j = 0;
	for (i = 0; i < 3; i++)	{
		if (tmax < NU[i].t) { j = i; tmax = NU[i].t; }
	}

	// ������ ������ ��������������
	double t0   = tmax + 1.0;
	// ����� �� ������� �������� �������
	double tsys = tmax + 6.0;

	char docString[256];
	DATETIMEUTC DTUTC;
	DTUTC<< t0;
	sprintf(docString, "\n  ����� ������ ��������������: ");
	&docString[strlen(docString)] << DTUTC;
	SmartPrintfS(fp, docString, true);

	DTUTC<< tsys;
	sprintf(docString, "\n  ������ ���������� �������:   ");
	&docString[strlen(docString)] << DTUTC;
	SmartPrintfS(fp, docString, true);
	SmartPrintfS(fp, "\n", true);

	// ���������� ����� ��� ��� ������� ������
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
	ZMSC  *KA;		// ������ �������� ��
	ZSC_SI SI;		// ��������� ��� ��������� �� ������ ��������� �������� ��
	ZNU    NU0[3];	// ��������� ������� �� ������ ������ ��������������
	ZNU    NUT[3];	// ��������� ������� �� ������ ���������� �������
	double uT[3];	// �������� ���������� ������ �� � ����� �� 
	int	   vitT[3];	// ������ ���������� �������
	double TdrT[3]; // ������������� ������ ��� ������� �� ��

	// �������� � ���� ������ �������� ��
	KA = new ZMSC; if (!KA) return -1;

	//-------------------------------------------------------------------------
	// ������������ ���� �� �� ���� � ��� �� ������ �������, 
	// ������ �� ������ ������ ��������������, ����� �� ������ 
	// ���������� �������
	cout << endl 
		 << "  ������ ���������� �������� � ��������� �����" << endl
		 << "  �� ������� ������������ �������." << endl;
	for (i = 0; i < 3; i++) {
		DTUTC<< NU[i].t;
		cout << "  �� �" << NU[i].Key.ka << "  �� " << DTUTC << " . . ." << endl;
		// ������������� ������ �������� ��� ������� �� �������� �� (NU)
		// � �������� ���� ��� (LSF)
		rc = KA->Init(NU[i], &LSF); if (rc) return rc;

		// ������� �� ������ ������ ��������������
		rc = KA->GoToTime(t0, &SI); if (rc) return rc;
		// ������������ ��
		NU0[i]     = NU[i];
		NU0[i].sk  = _SK_OSKTE;
		NU0[i].t   = SI.ta;						// ����� � ���
		NU0[i].vit = SI.VitTE;					// �����
		memcpy(NU0[i].X, SI.OSKTE, Size6Dbl);	// ���������� (�� ��)

		// ������� �� ������ ������� ���������� �������
		rc = KA->GoToTime(tsys, &SI); if (rc) return rc;
		uT[i]   = TO_GRAD(SI.OSKTE[5]);			// �������� ������ � ��������
		vitT[i] = SI.VitTE;						// �����

		// ������� ���������� ����� - ����� ����������� ������
		// ������������ �������. ��� ������� �� ���� ����� ����� ����.
		rc = KA->GoAlongVit(SI.VitTE); if (rc) return rc;
		TdrT[i]    = KA->FV.Td;							// ������������ ������
		NUT[i]     = NU[i];
		NUT[i].sk  = _SK_OSKTE;							// ��� ����. ��
		NUT[i].t   = KA->FV.FSUN.ta;					// ����� � ���
		NUT[i].vit = SI.VitTE;							// �����
		memcpy(NUT[i].X, KA->FV.FSUN.OSKTE, Size6Dbl);	// ���������� (�� ��)

		sprintf(docString, "   �� %d, ������������� ������ �� ����� %d  %.3lf ���.\n", 
		KA->GetKeyNU().ka, NUT[i].vit, TdrT[i]);
		SmartPrintfS(fp, docString, true);
	}

	SmartPrintfS(fp, "\n\n", true);
	SmartPrintfS(fp, "   �������� �� ������� ������������ ��������\n", true);
	sprintf(docString, "   ���������� ��������� ��                             %d\n\n", NUT[2].Key.ka);
	SmartPrintfS(fp, docString, true);
	SmartPrintfS(fp, "   �� ���������� ������������ ��������� �� ������� ��  60 ����.\n\n", true);
	SmartPrintfS(fp, "   ��� ������� �� �������, � ��� ����� � ��� ���������,\n", true);
	SmartPrintfS(fp, "   ����������� ��������-���������� ������ ������������ �����.\n\n", true);
	SmartPrintfS(fp, "   ����� ������������ ������� �� �������� � ��������� ���������\n", true);
	SmartPrintfS(fp, "   �� ��������-���������� ������� ������������ ����� �� ����������\n", true);
	SmartPrintfS(fp, "   ������� ���������� ���� �� �����.\n", true);

	//-------------------------------------------------------------------------
	// �������� ������� ������ ��������.
	delete KA; KA = nullptr;

	//-------------------------------------------------------------------------
	// ���������� ��������� ������ ������� ���������� ���������
	// � ���������� ��������������
	double duT[3];
	duT[2] = 0;

	SmartPrintfS(fp, 
	"\n  ������� �� ��������� ������ �� ������ ������������ �������:\n", true);
	for (i = 0; i < 2; i++) {
		duT[i] = ValiDA(uT[i] - uT[2], 1);
		double s = Sign1(duT[i]);
		duT[i] = 60.0*s - duT[i];
		sprintf(docString, "       �� %d   %lf ����.\n", NU0[i].Key.ka, duT[i]);
		SmartPrintfS(fp, docString, true);
	}
	SmartPrintfS(fp, "\n", true);

	//-------------------------------------------------------------------------
	// ������ ���������� ����� �� ������ ���������� ������� 
	// ��� ������� �� ������� 
	int    vitz[3]; // ��������� ����� �� ������ ������������ �������.
	double uz[3],   // ��������� ���-�� ������ �� ������ ������������ �������.
		   az[3],   // ������� �������, �������������� � ��������� �������
		   ez[3],   // �� ������ ��������� ����� ...
		   wz[3],   // 
		   a0[3],   // �������, �������������� � ��������� ������� �� ������ 
		   e0[3],   // ������ �����, ����������� ������ 
		   w0[3];   // ������������ �������.

	SmartPrintfS(fp, "   ���������� ���������� ������� �� ���,\n", true);
	SmartPrintfS(fp, "     ��      ������       �����������    ����������\n", true);

	for (i = 0; i<3; i++) {
		double 
		i0    = TO_GRAD(NUT[i].X[2]),
		W0    = TO_GRAD(NUT[i].X[3]);
		a0[i] = TO_KM(NUT[i].X[0]);
		e0[i] = NUT[i].X[1];
		w0[i] = TO_GRAD(NUT[i].X[4]);
		// �������������� ����� �� �������� ������� ��� ��������� ���������� �
		// ���������� ������� �� ������
		rc = OrbitDesignOSHfromTi(&LSF, TdrT[2], i0, W0, az[i], ez[i], wz[i]); 
		if (rc) return rc;

		// ��������� �������� ������ �� ������ ������������ ������� �� 
		uz[i]   = uT[i] + duT[i];
		vitz[i] = vitT[i];
		// �������� ���������� �������� ��������� ������ ��� ���������
		// [0, 360] � ��������������� ������������� �����, ���� ��� ����������
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
	// ����� ���������� ���������
	int Vit[3][4];
	for (i = 0; i < 3; i++) {
		Vit[i][0] = NU0[i].vit + 2; 
		Vit[i][1] = NU0[i].vit + 20;
		Vit[i][2] = NUT[i].vit - 1; 
		Vit[i][3] = NUT[i].vit;
	}

	//-------------------------------------------------------------------------
	// ������� ������ ��������������
	//-------------------------------------------------------------------------

	// ������������ �� (� �����)
	MS->MZ.DU()  = 1;					
	// ����� ������ � ����� ������ �� �������.	
	MS->MZ.VAR() = MS->MZ.FINDVARTASK(indexTask);
	// ������������ ������ (��������� � ���������������) 
	// ��������� �� ������ ����� ���������� ���������� 
	MS->MZ.GetParKorFromVar(MS->MZ.DemandsKey);
	MS->MZ.GetAdvParKorFromVar(MS->MZ.SecondDemandsKey);
	// �������������� ������ ������� ���������� ��������� ��������� �� 
	DefaultParam(MS->MP);
	// �������� �� � �������� ������� 1 ��� �� 1
	LoadDU(1, 1, MS->DU);
	// ������� ���� ��������� ����� ������ ��� ���� �����
	// �� ��������� �������� ���������� �������� ����������� ������ � � ������
	MS->MP.isEstimateOrb = true;

	//-------------------------------------------------------------------------
	// �������������� ���������������� ����������� ��������
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
	// ���������������� ��
	PrintMng.PrintNU(NU0[0], NULL, 3, fp);
	PrintMng.PrintNU(NU0[1], NULL, 3, fp);
	PrintMng.PrintNU(NU0[2], NULL, 3, fp);
	// ���������������� ���
	PrintMng.PrintLSF(LSF, 1, fp);

	//-------------------------------------------------------------------------
	// �������� ������� ���������������� ���������
	ZCExternalEquationsDefinition EED;
	EED << EXTEQ_ID_avrT_ex;
	EED << EXTEQ_ID_avrT_ey;
	EED << EXTEQ_ID_avrT_H;

	//-------------------------------------------------------------------------
	// ���� ������� ����� �������������� ��� ������� �� �������
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// �������� ������ ��������
	KA = new ZMSC; if (!KA) return-1;
	
	for (i = 0; i < 3; i++) {
		//---------------------------------------------------------------------
		// � ��� ���� ���� �� ����������
		assert(LSF.isDU);
		// ��� ����������� ������ ���� �� ����� ��� "���������� ���"
		assert(LSF.ngpz >= 2);

		//---------------------------------------------------------------------
		// ������������� ������ ��������
		rc = KA->Init(NU0[i], &LSF, &EED); if (rc) break;

		//---------------------------------------------------------------------
		MS->MZ.KA() = NU0[i].Key.ka;		// �������� ����� ��
		MS->MZ.M()  = KA->WeightFullNU();	// ����� ��������� ����� ��

		MS->MZ.VITIMP(0) = Vit[i][0]; // ����� ������� ��������
		MS->MZ.VITIMP(1) = Vit[i][1]; // ����� ������� ��������
		MS->MZ.VITIMP(2) = Vit[i][2]; // ����� ������� ��������
		MS->MZ.VIT()     = Vit[i][3]; // �������� ����� �� ������� 
									  // �������������� ������� �������
		// ��������� ��������� ������������ �������� �� - �������� �
		// ���������� �������������� �����
		MS->MZ.PART() = TdrT[2];// ������������� ������
		MS->MZ.PARe() = ez[i];	// �������������� �����	
		MS->MZ.PARw() = wz[i];	// �������� ������ ������� �����		
		MS->MZ.PARu() = uz[i];	// �������� ���� ��
		MS->MZ.PARt() = tsys;	// ����� �� ������� ���������� ���������� 
								// �������� ���� ��

		//---------------------------------------------------------------------
		// ������� ������ ������� ���������� ��������� �� ������� MZ 
		ZSLoopFact  P1;	// ��������� �������� �� ����� ������� ��� 
		ZSLoopFact  P2;	// ��������� �������� �� ����� ������� ��� 
		ZSLoopFact  PN;	// ��������� ��������� �������� �� �������� �����
		ZSLoopFact  PR;	// ��������� �������� ��������� �������� �� �������� ���. 
		ZSFlightRule U;	// ���������� ����������

		// ��������� ������������� ������� ��������������� ������ ������� 
		// ������� ������� �����. ������ ��������� �������� ������ ��� ���� 
		// �������� ����� ����������� ��� TaskMagicFly__. 
		rc = MagicSolution(MS, &PrintMng, fp, LSF, *KA, NU0[i], &EED,
						   &P1, &P2, &PN, &PR, &U); if (rc) break;
	}
	// �������� ��������� � ������������ �������
	PrintMng.ClosePrint(&fp);

	//-------------------------------------------------------------------------
	// ������� ������ �������� ��
	if (KA) delete KA;
	// ������� ��������� ����� "������ �������"
	FreeMagicLnsSolution(MS);

	return rc;
}

//-----------------------------------------------------------------------------