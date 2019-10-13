//-----------------------------------------------------------------------------
// ZCPrintManagerAflatText.cpp
// �. ��������� 03.08.2018
// �������������� ���������������� ����������� ��������
//-----------------------------------------------------------------------------
#pragma once
#include <stdafx.h>
#include <algorithm>

#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZCPrintManager.h>
#include <FlyCoreSource\\ZCPrintFormat.h>

//-----------------------------------------------------------------------------
void ZCPrintManager::PrintAflatLineMSC(FILE* fp)
{
	PrintAflatLineMSCtext(fp);
	PrintAflatLineMSChtml();
}

//-----------------------------------------------------------------------------
void ZCPrintManager::PrintAflatLineMSCtext(FILE* fp)
{
	// ���������� ������ Len �������������� ����� � ������������ � ��������  
	// ������� ��������������� ���������� ������� (��������������) ������ 
	// �����������
	int Len = MPF.CalcLineLenght(this);
	// ������ �������������� ����� �������� ������ Len
	MPF.PrintLineAflet(fp, Len);
}

//-----------------------------------------------------------------------------
void ZCPrintManager::PrintAflatLineMSChtml()
{
	if (!HtmlCreator) return;

	// �������� ������ ��������� ������� 1 �������
	int j = HtmlCreator->CountItems();	if (--j < 0) return;

	Z�HtmlItem& I = (*HtmlCreator)(j);
	while (1) {
		// ����� �������� ��������� ���������� ����
		int k = I.CountItems();	if (--k < 0) return;
		//  ������ �� ��������� �������� �������
		Z�HtmlItem& J = I(k);
		// ��� ���������� ���������� ��������
		string t = J.n;
		transform(t.begin(), t.end(), t.begin(), tolower);
		// �������� ����� ���� - ������ ���� ��� ������ ������� <tr>
		if (!t.compare(0, 2, "tr")) {
			k = J.CountItems();
			int r = I.CountItems();
			I(r).n = "td";
			I(r).txt = "<hr>";
			char s[64]; sprintf(s, "colspan \n %d\0", k);
			I(r)[0] << s;
			return;
		}
		I = J;
	}
}

//-----------------------------------------------------------------------------
int ZCPrintManager::PrintAflatMSCtext(ZMSC* pKA, ZSC_SI* FS, ZSC_VI* FV, FILE* fp)
{
	// ���� �� ����� ��, �� ������ ��������� �������� �� ����� FS � ��
	// ������ �������������� �����, �� ������ ��������������� �� ����,
	// ������ ����� �� �������
	if (!pKA && !FS && !FV) return 0;

	int i, j, K;
	int N = MPK.Size();
	int NF = MPK.SizeF();
	// ��������� ������� ��������������� ���������������� ����������� ��������
	// ���������� ��������
	if (MPF.IsFirstLine()) {
		// ������ �������������� �����
		PrintAflatLineMSCtext(fp);
		string Name;
		int    Len;
		// ������ ����, ��������������� ���������� ��������
		for (i = 0; i<NF; i++) {
			MPF.GetItemName(this, Name, i);
			Len = MPF.CalcItemLenght(this, i);
			if (Len>0 && Name.length()) fprintf(fp, "%*s\0", Len, Name.c_str());
		}
		// ������ ������������, ��������������� ���������� ��������
		fprintf(fp, "\n");
		for (i = 0; i<NF; i++) {
			MPF.GetItemDim(this, Name, i);
			Len = MPF.CalcItemLenght(this, i);
			int N = 1;
			// ���� ������ ���������������� ��������� ������� ��� ������� 
			// ��������, �� ����������� ���������������� ��������� ���������� 
			// ������ - �� ����� ��������� �������.
			int ID = MPF.GetItemID(this, i);
			if (ID == MODPR_T_V75 || ID == MODPR_T_V00 || ID == MODPR_T_VTE ||
				ID == MODPR_T_VTES || ID == MODPR_T_VGSC ||
				ID == MODPR_T_R75 || ID == MODPR_T_R00 || ID == MODPR_T_RTE ||
				ID == MODPR_T_RTES || ID == MODPR_T_RGSC) N = 3;
			Len /= N;
			if (Len>0) {
				if (Name.length()) {
					for (int j = 0; j<N; j++) fprintf(fp, "%*s\0", Len, Name.c_str());
				} else {
					int j;
					char SpaceStr[256] = "                                ";
					for (j = 32; j<Len; j++) SpaceStr[i] = ' ';
					SpaceStr[Len] = '\0';
					for (j = 0; j<N; j++) fprintf(fp, "%s", SpaceStr);
		}	}	}
		fprintf(fp, "\n");

		// � ������, ����� � ��������� ������� ������������ ���������
		// �������� ��, ��� �� ������ ��� � �� �������, ���������������
		// ������� ����������� ������ ���������� � ���������.
		// � ������������� ���������������� ��������� ���������������
		// MPF.IsFirstLine() == true.
		// ������� ����� ������ ������� ���������������� � ��������� ���� �
		// � ���� html ���������� ��������� ������� ������� ����� � ���� ��
		// ����� true - ��������� ��� ���-��� ��������������� ��������� 
		// � ���� ������ � ���� ����� ��� ������� �������������� �������� 
		// PrintApeakMSCtext � ��������� �������� � PrintApeakMSChtml �
		// �������� html.
		MPF.FirstLineOff();
		// ���� ����, ��������������� � ������������� ������ ��������� 
		// ������� ��������, ��� ����������� ������� ���������� ������
		// � ��������� � ��������� ������� ��������� ������ ���������
		// ��������.

		// ������ �������������� �����
		PrintAflatLineMSCtext(fp);
	}

	// ���� ������ FS ���������� �������� "�� �����" �� �����, ��  
	// � �������� �������� ������� ���������� ���������� ������������� 
	// ��������, �� ���� ������ FSR ������ ZMSC. ������ FSR ����������� 
	// ��� ��������� ���������� ������ "Go..." ������ ������ ��������, 
	// �������� GoToTime � ��� �����.
	if (!FS) FS = &pKA->FSR;
	// ���� ������ FV ������������� ����� �� �����, �� � �������� �������� 
	// ���������� ������ ������ ������ �������� pKA.
	if (!FV) FV = &pKA->FV;
	// ���� ������ ������� ���������������� �� ������, �� �������� 
	// �� �� ��� � �������� ������� ���������� �������� "�� �����"
	// ���������� ������ FSUN ���������� �������� � ���������� ���� ������
	// �� ������� �������� ������������� ����� FV.
	FS = (MPK.fromVit ? &FV->FSUN : FS);

	// ����� ����������� � ������������ ����� � �������� �� �����
	// �� ����� �������� ����������� � ������������ ����� � 
	// �������� �� �����.
	// �������� ��� ���� ����� ����� ����������� �� 1-� �� 3-�
	// �������� ����������� �����, �� � ����������� ������� ���  
	// ������������� ����� - 2, ��� ������������������� - 1. 
	// ����������� �������� ����� ����� ���� ��� �������� 
	// ����������� ������ ��������� �������� ������������ � 
	// �������� �������������
	ZSC_VP* Hmin = HMIN_VI(*FV);
	ZSC_VP* Hmax = HMAX_VI(*FV);
	ZSC_VP* Rmin = RMIN_VI(*FV);
	ZSC_VP* Rmax = RMAX_VI(*FV);

	// ���������������� ��������� ������ ��������� �������� MPK,
	// ����������� ������ N = MPK.Size();
	for (i = 0; i<N; i++) {
		K = MPK[i];
		switch (K) {
		case MODPRDATETIME:
						MPF.PrintDateTime(this, fp, nullptr, FS->ta, 0); break;
		case MODPRDATE:	MPF.PrintDateTime(this, fp, nullptr, FS->ta, 1); break;
		case MODPRTIME:	MPF.PrintDateTime(this, fp, nullptr, FS->ta, 2); break;
		case MODPR_KA:		  MPF.PrintKA(this, fp, nullptr, pKA->GetNumKA()); break;
		case MODPR_VIT:		 MPF.PrintVit(this, fp, nullptr, FS->VitTE); break;
		case MODPR_M:		   MPF.PrintM(this, fp, nullptr, FS->m); break;
		case MODPR_MSSV:	MPF.PrintMSSV(this, fp, nullptr, FS->tmss); break;
		case MODPR_Tosk:	 MPF.PrintTdr(this, fp, nullptr, FS->THBLFR[0]); break;
		case MODPR_R:		 MPF.PrintRkm(this, fp, nullptr, FS->THBLFR[5]); break;
		case MODPR_H:		 MPF.PrintHkm(this, fp, nullptr, FS->THBLFR[1]); break;
		case MODPR_B:		 MPF.PrintBLF(this, fp, nullptr, FS->THBLFR[2]); break;
		case MODPR_L:		 MPF.PrintBLF(this, fp, nullptr, FS->THBLFR[3]); break;
			break;
		case MODPR_Aosk:	MPF.PrintAkm(this, fp, nullptr, TO_KM(FS->OSK1975[0])); break;
		case MODPR_eosk:	  MPF.PrintE(this, fp, nullptr, FS->OSK1975[1]); break;
		case MODPR_exosk:	  MPF.PrintE(this, fp, nullptr, FS->OSKTE[1] * Cos(FS->OSKTE[4], 0)); break;
		case MODPR_eyosk:	  MPF.PrintE(this, fp, nullptr, FS->OSKTE[1] * Sin(FS->OSKTE[4], 0)); break;
		case MODPR_V_Tdr:	MPF.PrintTdr(this, fp, nullptr, FV->Td); break;
		case MODPR_V_Hsr:	MPF.PrintHkm(this, fp, nullptr, FV->HAvr); break;
		case MODPR_V_DL:	MPF.PrintBLF(this, fp, nullptr, FV->dL); break;
		case MODPR_V_DLS:	MPF.PrintBLF(this, fp, nullptr, FV->dLs); break;
		case MODPR_V_KR:	 MPF.PrintKR(this, fp, nullptr, FV->Ks); break;
			// ���������� ����
		case MODPR_V_VUdatetime:
							MPF.PrintDateTime(this, fp, nullptr, FV->FSUN.ta, 0); break;
		case MODPR_V_VUdate:MPF.PrintDateTime(this, fp, nullptr, FV->FSUN.ta, 1); break;
		case MODPR_V_VUtime:MPF.PrintDateTime(this, fp, nullptr, FV->FSUN.ta, 2); break;
		case MODPR_V_VUR:		   MPF.PrintR(this, fp, nullptr, FV->FSUN.THBLFR[5]); break;
		case MODPR_V_VUH:		 MPF.PrintHkm(this, fp, nullptr, FV->FSUN.THBLFR[1]); break;
		case MODPR_V_VUL:		 MPF.PrintBLF(this, fp, nullptr, FV->FSUN.THBLFR[3]); break;
		case MODPR_V_VUmssv:	MPF.PrintMSSV(this, fp, nullptr, FV->FSUN.tmss); break;
			// ���������� ����
		case MODPR_V_NUdatetime:
							MPF.PrintDateTime(this, fp, nullptr, FV->FSDN.ta, 0); break;
		case MODPR_V_NUdate:MPF.PrintDateTime(this, fp, nullptr, FV->FSDN.ta, 1); break;
		case MODPR_V_NUtime:MPF.PrintDateTime(this, fp, nullptr, FV->FSDN.ta, 2); break;
		case MODPR_V_NUR:		   MPF.PrintR(this, fp, nullptr, FV->FSDN.THBLFR[5]); break;
		case MODPR_V_NUH:		 MPF.PrintHkm(this, fp, nullptr, FV->FSDN.THBLFR[1]); break;
		case MODPR_V_NUL:		 MPF.PrintBLF(this, fp, nullptr, FV->FSDN.THBLFR[3]); break;
		case MODPR_V_NUmssv:	MPF.PrintMSSV(this, fp, nullptr, FV->FSDN.tmss); break;
			// ����������� ������
		case MODPR_V_Hmint:	MPF.PrintDateTime(this, fp, nullptr, Hmin->t, 2); break;
		case MODPR_V_HminU:		 MPF.PrintBLF(this, fp, nullptr, Hmin->ute); break;
		case MODPR_V_Hmin:		 MPF.PrintHkm(this, fp, nullptr, Hmin->H); break;
		case MODPR_V_HminB:		 MPF.PrintBLF(this, fp, nullptr, Hmin->B); break;
		case MODPR_V_HminL:		 MPF.PrintBLF(this, fp, nullptr, Hmin->L); break;
			// ������������ ������
		case MODPR_V_Hmaxt: MPF.PrintDateTime(this, fp, nullptr, Hmax->t, 2); break;
		case MODPR_V_HmaxU:		 MPF.PrintBLF(this, fp, nullptr, Hmax->ute); break;
		case MODPR_V_Hmax:		 MPF.PrintHkm(this, fp, nullptr, Hmax->H); break;
		case MODPR_V_HmaxB:		 MPF.PrintBLF(this, fp, nullptr, Hmax->B); break;
		case MODPR_V_HmaxL:		 MPF.PrintBLF(this, fp, nullptr, Hmax->L); break;
			// ����������� ������
		case MODPR_V_Rmint: MPF.PrintDateTime(this, fp, nullptr, Rmin->t, 2); break;
		case MODPR_V_RminU:		 MPF.PrintBLF(this, fp, nullptr, Rmin->ute); break;
		case MODPR_V_Rmin:		   MPF.PrintR(this, fp, nullptr, Rmin->R);	break;
		case MODPR_V_RminB:		 MPF.PrintBLF(this, fp, nullptr, Rmin->B); break;
		case MODPR_V_RminL:		 MPF.PrintBLF(this, fp, nullptr, Rmin->L); break;
			// ������������ ������
		case MODPR_V_Rmaxt: MPF.PrintDateTime(this, fp, nullptr, Rmax->t, 2); break;
		case MODPR_V_RmaxU:		 MPF.PrintBLF(this, fp, nullptr, Rmax->ute); break;
		case MODPR_V_Rmax:		   MPF.PrintR(this, fp, nullptr, Rmax->R);	break;
		case MODPR_V_RmaxB:		 MPF.PrintBLF(this, fp, nullptr, Rmax->B); break;
		case MODPR_V_RmaxL:		 MPF.PrintBLF(this, fp, nullptr, Rmax->L); break;
			// ������ � �������� � ������ ������
		case MODPR_T_R75:	for (j = 0; j<3; j++) MPF.PrintR(this, fp, nullptr, TO_KM(FS->ASK1975[j])); break;
		case MODPR_T_V75:	for (j = 0; j<3; j++) MPF.PrintV(this, fp, nullptr, TO_KMS(FS->ASK1975[3 + j])); break;
		case MODPR_T_R00:	for (j = 0; j<3; j++) MPF.PrintR(this, fp, nullptr, TO_KM(FS->ASK2000[j])); break;
		case MODPR_T_V00:	for (j = 0; j<3; j++) MPF.PrintV(this, fp, nullptr, TO_KMS(FS->ASK2000[3 + j])); break;
		case MODPR_T_RTE:	for (j = 0; j<3; j++) MPF.PrintR(this, fp, nullptr, TO_KM(FS->ASKTE[j])); break;
		case MODPR_T_VTE:	for (j = 0; j<3; j++) MPF.PrintV(this, fp, nullptr, TO_KMS(FS->ASKTE[3 + j]));	break;
		case MODPR_T_RTES:	for (j = 0; j<3; j++) MPF.PrintR(this, fp, nullptr, TO_KM(FS->ASKTES[j])); break;
		case MODPR_T_VTES:	for (j = 0; j<3; j++) MPF.PrintV(this, fp, nullptr, TO_KMS(FS->ASKTES[3 + j])); break;
		case MODPR_T_RGSC:	for (j = 0; j<3; j++) MPF.PrintR(this, fp, nullptr, TO_KM(FS->GSK[j])); break;
		case MODPR_T_VGSC:	for (j = 0; j<3; j++) MPF.PrintV(this, fp, nullptr, TO_KMS(FS->GSK[3 + j])); break;
			// ������� ������������ �������� ������ � ������ ������
		case MODPR_T_i75:	MPF.PrintGrad(this, fp, nullptr, FS->OSK1975[2]); break;
		case MODPR_T_W75:	MPF.PrintGrad(this, fp, nullptr, FS->OSK1975[3]); break;
		case MODPR_T_w75:	MPF.PrintGrad(this, fp, nullptr, FS->OSK1975[4]); break;
		case MODPR_T_u75:	MPF.PrintGrad(this, fp, nullptr, FS->OSK1975[5]); break;
		case MODPR_T_i00:	MPF.PrintGrad(this, fp, nullptr, FS->OSK2000[2]); break;
		case MODPR_T_W00:	MPF.PrintGrad(this, fp, nullptr, FS->OSK2000[3]); break;
		case MODPR_T_w00:	MPF.PrintGrad(this, fp, nullptr, FS->OSK2000[4]); break;
		case MODPR_T_u00:	MPF.PrintGrad(this, fp, nullptr, FS->OSK2000[5]); break;
		case MODPR_T_iTE:	MPF.PrintGrad(this, fp, nullptr, FS->OSKTE[2]); break;
		case MODPR_T_WTE:	MPF.PrintGrad(this, fp, nullptr, FS->OSKTE[3]); break;
		case MODPR_T_wTE:	MPF.PrintGrad(this, fp, nullptr, FS->OSKTE[4]); break;
		case MODPR_T_uTE:	MPF.PrintGrad(this, fp, nullptr, FS->OSKTE[5]); break;
		case MODPR_T_iTES:	MPF.PrintGrad(this, fp, nullptr, FS->OSKTES[2]); break;
		case MODPR_T_WTES:	MPF.PrintGrad(this, fp, nullptr, FS->OSKTES[3]); break;
		case MODPR_T_wTES:	MPF.PrintGrad(this, fp, nullptr, FS->OSKTES[4]); break;
		case MODPR_T_uTES:	MPF.PrintGrad(this, fp, nullptr, FS->OSKTES[5]); break;
		default: assert(0);
		}
	}

	// ���������������� �������������� "����������������" ���������� MPK.UPP.
	// ����������� ������� ����� ���������� NF=MPK.SizeF()
	for (i = N; i<NF; i++) {
		int j = i - N;
		int Len = MPK.UPP(j).len;	// ������ ���������
		int Dec = MPK.UPP(j).dec;	// �������� ��������� - ����� ������ ����� �������
		switch (MPK.UPP(j).type) {
			// ����� �����
		case UPPint:    fprintf(fp, "%*d", Len, (int)MPK.UPP(j).value); break;
			// ����� � ������������� ������
		case UPPdouble: fprintf(fp, "%*.*lf", Len, Dec, MPK.UPP(j).value); break;
			// ����� � ��������� ������
		case UPPexp:    fprintf(fp, "%*.*e", Len, Dec, MPK.UPP(j).value); break;
		default:;
		}
	}

	fprintf(fp, "\n");
	return 0;
}

//-----------------------------------------------------------------------------