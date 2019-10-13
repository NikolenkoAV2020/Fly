P//-------------------------------------------------------------------------------
// TaskCalcZRVDoc.cpp : 
// �. ��������� 30.11.2018
//
// ������� ���������������� ����������� ������ ��� � ����
//-------------------------------------------------------------------------------
#include "stdafx.h"
#include <direct.h>

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>		// ����������� ���� ������ ��������
#include <FlyMMath.h>		// �������������� �������

//---------------------------------------------------------------------------
#include "TaskCalcZRVDoc.h"
#include "TaskCalcZRVMod.h"		

//-------------------------------------------------------------------------------
// ����� ��� ���������� �������������� ���
vector<ZZRV> VzZRV ;	
// �������� ������ ��� ������� ������ � ������� �������
extern IDTASKZRV IDZRV ;

//-------------------------------------------------------------------------------
void MakeZRVDir(char* LDir, char* SDir)
{
	mkdir(DIRRESULT);
	if (SDir) sprintf(SDir, "���");
	sprintf(LDir, "%s\\���", DIRRESULT);
	mkdir(LDir);
}

//-------------------------------------------------------------------------------
// ��������� ���������� ��������� ��������� ������� ���
void DocHeaderZRV(FILE* fp, ZModZRV* KA, int CountKA, int* NipIndexes, int CountNip)
{
	if (!fp) return ;
	if (!KA && !NipIndexes) {
		fprintf(fp, "\n  ������ ��� �������������� �� � ���\n\n\0" ) ;
		return ;
	}

	int i ;
	if (!KA && NipIndexes) {
		fprintf(fp, "\n  ������ ��� �������������� ��\n\0" ) ;
		fprintf(fp, "  � ���:\0" ) ;
		for (i=0; i<CountNip; i++) {
			int ind = NipIndexes[i] ;
			fprintf(fp, i==CountNip-1 ? " %d.\0":" %d,\0", ALLNIPS[ind].Num) ;
		}
		fprintf(fp, "\n\n\0" ) ;
		return ;
	}

	if (KA && !NipIndexes) {
		fprintf(fp, "\n  ������ ��� �������������� � ���\n\n\0" ) ;
		fprintf(fp, "  ��� ��:\0" ) ;
		for (i=0; i<CountKA; i++) {		
			fprintf(fp, i==CountKA-1 ? " %d;\0":" %d,\0", KA[i].GetNumKA()) ;
		}
		fprintf(fp, "\n\n\0" ) ;
		return ;
	}

	if (KA && NipIndexes) {
		fprintf(fp, "\n  ������ ��� ��������������\n\0" ) ;
		fprintf(fp, "  ��� ��:\0" ) ;
		for (i=0; i<CountKA; i++) {		
			fprintf(fp, i==CountKA-1 ? " %d;\0":" %d,\0", KA[i].GetNumKA()) ;
		}
		fprintf(fp, "\n\0" ) ;
		fprintf(fp, "  �  ���:\0" ) ;
		for (i=0; i<CountNip; i++) {
			int ind = NipIndexes[i] ;
			fprintf(fp, i==CountNip-1 ? " %d.\0":" %d,\0", ALLNIPS[ind].Num) ;
		}
		fprintf(fp, "\n\n\0" ) ;
		return ;
	}
}

//-------------------------------------------------------------------------------
// ��������� ������� ���
void DocTablHeaderZRV(FILE* fp)
{
	if (!fp) return ;
	fprintf(fp, "\n\0" ) ;

	if (IDZRV.VarDocZRV==0) { 
		fprintf(fp, "______________________________________________________________\n\0" ) ;
		fprintf(fp, "             ����-�����                    ������   ���� �����\n\0" ) ;
		fprintf(fp, "                                             ����         ����\n\0" ) ;
		fprintf(fp, "______________________________________________________________\n\0" ) ;
	}
	if (IDZRV.VarDocZRV==1) { 
		fprintf(fp, "________________________________________________________________________________________________________________________________________________________\n\0" ) ;
		fprintf(fp, "    ��   ���  �����   ����-�����                  ������ ���� ����� ������������ �����            ������ ���� �����   �����            ������ ���� �����\n\0" ) ;
		fprintf(fp, "                      (����)                       ����.      ����.         ���. (�����)           ����.      ����.   (��������)        ����.      ����.\n\0" ) ;
		fprintf(fp, "________________________________________________________________________________________________________________________________________________________\n\0" ) ;
	}
}

//-------------------------------------------------------------------------------
// ���������� ���������������� ������� ����������� ���
void DocEndZRV(FILE* fp)
{
	if (!fp) return ;
	// ����� ���� ����������� ��� � ��������� ��������
	ResetZRVData(fp) ;
	if (IDZRV.VarDocZRV==0) { 
		fprintf(fp, "______________________________________________________________\n\0" ) ;
	}
	if (IDZRV.VarDocZRV==1) { 
		fprintf(fp, "________________________________________________________________________________________________________________________________________________________\n\0" ) ;
	}
}

//-------------------------------------------------------------------------------
// ���������������� ����� ���
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
		sprintf(SN[0], "��") ;
		sprintf(SN[1], "���") ;
		sprintf(SN[2], "�����") ;
		sprintf(SN[3], "����:      ") ;
		sprintf(SN[4], "��������:  ") ;
		sprintf(SN[5], "�����:     ") ;
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
// ���������� ������� ��� - ��������� � ������ ����� ������������ ����
void AccumulationZRVData(ZZRV& zrv)
{
	// ���������� ������ � ��� ����������� ������ ���� ���������� 
	// ����, ����� � ��������
	if (zrv.isFound != 7) return ;
	// ���������� ������ � ���
	VzZRV.push_back(zrv) ;
}

//-------------------------------------------------------------------------------
// ����� ������������ ������� ��� � ��������� ��������
void ResetZRVData(FILE* fp, ZModZRV* KA, int CountKA)
{
	// ������ ������������ �������
	int N = VzZRV.size();
	// ���� ��� ������������ ���, �� ������ ������� �� ���������
	if (!N) return;
	// ���������� ������� �� ������� ������ ���
	sort(VzZRV.begin(), VzZRV.end());
	// ��������������� ������������ ������� �� ����� ����
	if (fp) {
		int i;
		for (i = 0; i<N; i++) {
			//---------------------------------------------------------------
			// ����� ��� ��� �������� ������� ��� VzZRV[i]
			int indNip = -1;
			int numNip = -1;
			int jNip;
			for (jNip = 0; jNip<IDZRV.CountNip; jNip++) {
				indNip = IDZRV.NipIndexes[jNip];
				numNip = ALLNIPS[indNip].Num;
				if (VzZRV[i].Nip == numNip) break;
			}
			if (jNip == IDZRV.CountNip) continue;

			// ���������������� 1-� ��� � ��������� ������
			char S[512], Sscr[512];
			DocZRV(VzZRV[i], S, Sscr);
			// ��������� ������� � ���������������� �� 
			CalcTargDir(VzZRV[i].KA, indNip, VzZRV[i].Vit,
			VzZRV[i].X, VzZRV[i].t1, VzZRV[i].t2);

			// ����� ��� ������ "������������" �� ����������� ����������

			if (IDZRV.needForKAFile) {
				//---------------------------------------------------------------
				// ����� �� ��� �������� ������� ��� VzZRV[i]
				int j;
				for (j = 0; j<CountKA; j++) {
					if (VzZRV[i].KA == KA[j].GetNumKA()) break;
				}
				// ������ ������������� ��������� ��� � ��������
				// ��� j-�� �� 
				if (j<CountKA) fprintf(KA[j].fp, "%s", S);
			}

			if (IDZRV.needForNipFile) {
				// ������ ������������� ��������� ��� � ��������
				// ��� j-�� ���
				fprintf(IDZRV.pfn[jNip], "%s", S);
			}

			//-------------------------------------------------------------------
			// ������ ������������� ��������� ��� � ��������
			// ��� ���� ������������ ��, ��������� � 
			// ������� �� ������ 
			fprintf(fp, "%s", S);
			// ���������������� �� �����, 
			// ���� ���������� ����������
			cout << Sscr;
	}	}
	// ������������ ������������ ������� ������ � ���
	VzZRV.clear();
}

//-------------------------------------------------------------------------------