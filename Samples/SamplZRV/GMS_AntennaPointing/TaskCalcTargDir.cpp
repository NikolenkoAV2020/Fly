//-------------------------------------------------------------------------------
// TaskTargDir.cpp : 
// �. ��������� 18.12.2018
//
// ������ � ���������������� �� ��� ������� � �������� ��� ��-���
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>		// ����������� ���� ������ ��������
#include <FlyMMath.h>		// �������������� �������

//---------------------------------------------------------------------------
// ���������������� ����������� ������� ���
#include "..\GMS_RadioVisibilityZones\TaskCalcZRVDoc.h"	
// ���������� ��� ������� �������� ��� ������� ���
#include "..\GMS_RadioVisibilityZones\TaskCalcZRVMod.h"		
// ������� ������� ������������ ��������� ���
#include "TaskCalcTargDir.h"

//-------------------------------------------------------------------------------
extern IDTASKZRV IDZRV ;			// �������� ������ ��� ������� ������ + ������� ������� 
extern PZCPrintManager PrintMng ;	// ��������� ����������������

char LineCalcTD[] = "_______________________________________________\0" ;

//-------------------------------------------------------------------------------
int CalcTargDir(int NumKA, int NipInd, long Vit, double* Xasc1975, double t1, double t2)
{
	// �������� ��� �������� ���������� ��������� ��� ���������������� ��
	FILE* pf = TargDirFileOpen(NumKA, NipInd, Vit, t1, t2) ;
	if (!pf) return-1 ;

	// ������ �������� ��� ������� �� 
	int    rc  = 0 ;
	double t   = t1 ;
	double dt  = IDZRV.StepTargDir / k_cbc ;
	double Xgsc[6] ;

	DATETIMEDMB DT ;
	ZSC_SI      SI ;
	ZMSC		KA ;
	ZNU         NU ;

	NU = IDZRV.NU[0] ;
	NU.t = t1 ;
	NU.sk = _SK_ASK1975 ;
	NU.Key.ka = NumKA ;
	memcpy(NU.X, Xasc1975, 6*sizeof(double)) ;

	if (KA.IsOkCreate()) {
	} else {
		// ��������� �������������
		rc = KA.Init(NU, &IDZRV.LSF) ; RRC ;
	}
	
	// ������� ���������� � ��� (���������, �� ������ ������ ���)
	KA.GetVector(Xgsc, nullptr, nullptr, nullptr, _SK_GSK);
	//---------------------------------------------------------------------------
	// ���� ������������� �������� �� � �������� ����� ������� ��
	//double percent = 0 ;
	//printf("TargDir for SC %d, GMS %d\0", NumKA, ALLNIPS[NipInd].Num) ;
	while (t<=t2) {
		//percent = 100.0*(t-t1)/(t2-t1) ;
		//putchar(177) ;
		// ������ ���������� ��������� ��������� ���-��
		double Params[7] ;
		CalcZRVParams(&ALLNIPS[NipInd], Xgsc, Params) ;
		double G = Params[0] ;
		double A = Params[4] ;
		DT << t ;
		fprintf(pf, "  \0") ; 
		pf << DT ;
		fprintf(pf, "   %9.4lf %9.4lf\n\0", G, A) ;
		// ���� ��������� ����� ���
		if (t==t2) {
			fprintf(pf, "%s\n", LineCalcTD) ;
			break ;
		}
		// ���������� ������� - ��������� ����� ��� ������� ��
		t+= dt ;
		// ����������� ������� ������� �������� ��������� ���
		// ��� ������� ������ �� ������ ���������� ��� 
		if (t>t2) t = t2 ;
		// ������ ��������� ����� ���������� �� 
		rc = KA.GoToTime(t, &SI) ; RRC ;   
		memcpy(Xgsc, SI.GSK, 6*sizeof(double)) ;
	}
	//printf("\n\0") ;

	return 0 ;
}

//-------------------------------------------------------------------------------
// ���������������� ������������ �������� ��
//-------------------------------------------------------------------------------
vector<FILE*> VPFTDfromKA ;
vector<int>   VKeyKAFile ;

//-------------------------------------------------------------------------------
// �������� ����� ����� ��� ���������������� �� ��� ��������� ��
void TargDirFileName(int NumKA, int NipInd, long Vit, char* FN)
{
	char Dir[256] ;	MakeZRVDir(Dir, nullptr) ;
	sprintf(FN, "%s\\_TaskCalcTD_SC%d.txt\0", Dir, NumKA) ;
}

//-------------------------------------------------------------------------------
// �������� ��� �������� ���������� ��������� ��� ���������������� ��
FILE* TargDirFileOpen(int NumKA, int NipInd, long Vit, double t1, double t2)
{
	FILE* pf = NULL ;
	char  FN[128] ; 
	int   i, j, N = VKeyKAFile.size() ;

	// ����� ��������� ����� ��� �� � ������� NumKA
	for (i=0; i<N; i++) {
		if (VKeyKAFile[i]==NumKA) break ;
	}
	// ���� ������ ����� ��� �� ������ ���
	if (i==N) {
		// ��������� ����� �����
		TargDirFileName(NumKA, NipInd, Vit, FN) ; 
		// ��������� � ������ ��� ��� ����������� ������ ������� �����
		// ��� ��������� ��� � �������� ����
		IDZRV.VFN.push_back(FN) ;
		// �������� � �������� ������ ���������
		pf = fopen(FN, "w" ) ;

		// ���������� ��������� �� ��������� ���� � ����� ��
		VPFTDfromKA.push_back(pf) ;
		VKeyKAFile.push_back(NumKA) ;

		// ��������� ���������			
		fprintf(pf, "\n  ������ �� ��� �� %d\n", NumKA) ;
		fprintf(pf, "  ���:\0" ) ;
		for (j=0; j<IDZRV.CountNip; j++) {
			int ind = IDZRV.NipIndexes[j] ;
			fprintf(pf, j==IDZRV.CountNip-1 ? " %d.\0":" %d,\0", ALLNIPS[ind].Num) ;
		}
		fprintf(pf, "\n\n\0" ) ;

		// ����� �� ��� �� � ������� NumKA
		for (j=0; j<IDZRV.CountKA; j++) {
			if (IDZRV.NU[j].Key.ka == NumKA) break ;
		}
		// ���������������� ��, ���� ��� ������� �������
		if (j<IDZRV.CountKA) {
			PrintMng->PrintNU(IDZRV.NU[j], NULL, 3, pf);
		}
		// ���������������� ���
		PrintMng->PrintLSF(IDZRV.LSF, 1, pf);
	}
	pf = VPFTDfromKA[i] ;

	if (pf) {
		fprintf(pf, "\n  ���   %d\n  ����� %d\n", ALLNIPS[NipInd].Num, Vit) ;
		DATETIMEDMB DT1, DT2 ;
		DT1 << t1 ;
		DT2 << t2 ;
		fprintf(pf, "  ���   ������: %.2d:%.2d:%.2d,%.3d  %.2d.%.2d.%.4d\n        ����� : %.2d:%.2d:%.2d,%.3d\n", 
		DT1.h, DT1.m, DT1.s, (int)(DT1.d*1e3), DT1.D, DT1.M, DT1.G, 
		DT2.h, DT2.m, DT2.s, (int)(DT2.d*1e3)) ;
		fprintf(pf, "%s\n", LineCalcTD) ;
		fprintf(pf, "  ����       �����         ���� �����    ������\n") ;
		fprintf(pf, "                                ����.     ����.\n") ;
		fprintf(pf, "%s\n", LineCalcTD) ;
	}

	return pf ;
}

//-------------------------------------------------------------------------------
// �������� ���� ��������� ���������� ��������� ��� ���������������� ��
void TargDirFileCloseAll()
{
	int i, N = VPFTDfromKA.size() ;
	for (i=0; i<N; i++) {
		FILE* pf = VPFTDfromKA[i] ;
		if (pf) {
			fclose(pf) ;
			pf = nullptr ;
	}	}
	VPFTDfromKA.clear() ;
	VKeyKAFile.clear() ;
}

//-------------------------------------------------------------------------------

