//-------------------------------------------------------------------------------
// TaskCalcZRV.cpp : 
// �. ��������� 26.11.2018
//
// ������ ��� ������������ ��� ���������� �� � ���������� �����
// !!!!
// ���� �������� ������������ ������� ������ � ����� ���, � ����� ��������
// ��������� ���, ���������� ������ ������ ��������. ������������ ��� ���������� 
// � ������� �������� � ������ �������� ������� �������������� ��� �����������
// ������ RunUpZRV.
// �������� ������� ������������� � ����� ZModZRV, ����������� �� ������ 
// ������ �������� ZMSC, ��� ������ ������������ ������ OnZRVEvent.
// � ������ ������ ��������� ��� ������������.
//
// ����� �������, ���������� ��������� ��������� �������� � ������������ 
// �������� ������ ��� �������, ����������� ������������� � ����������������
// ��������� ��� � ��������� � ������ OnZRVEvent ��������� ������� ��������
// ������� ���.
// !!!!
// ������� ����� ���� ���������� ������ ��������� ���������������� �����������
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>	// ����������� ���� ������ ��������
#include <FlyMMath.h>	// �������������� �������
#include <FlyZip.h>		// �������� ZIP-������

//---------------------------------------------------------------------------
// ��������� ��������� �� � ���� ������ �� (������)
#include <..\\RunUpSys\\RunUpSysBuild.h>	

#include "TaskCalcZRVDoc.h"	// ���������������� ����������� ������� ���
#include "TaskCalcZRVMod.h"	// ���������� ��� ������� �������� ��� ������� ���

// ������ ������������ ��� ���
#include "..\\GMS_AntennaPointing\\TaskCalcTargDir.h"	

//-------------------------------------------------------------------------------
// �������� ������ ��� ������� ������ + ������� ������� 
IDTASKZRV     IDZRV ;		
// ��������� ����������������
PZCPrintManager PrintMng ;	

//-------------------------------------------------------------------------------
int TaskCalcZRV()
{
	cout << "\n   Calculation radio visibility zones...\n";

	unsigned int j;
	int  rc = 0;
	long Vit[8];
	char FullName[512];

	//----------------------------------------------------------
	// ������������ ������� �������� ������ ��� ������� ������. 
	// � ���������� ��������� ������� GUI, ��� �������� ������
	// ����������� � ��� ������� ������������ ����, ������� ���������
	// ������������� �������������� ������������ � ����������.
	// � ��������� �� �������, ������ �������� ���������� �������� 
	// ����������� ��� ������� ���������� � ����������.

	// ��������� ����� �������������
	IDZRV.Vitn = 101;
	// �������� ����� �������������
	IDZRV.Vitk = 103;
	// ����������� ��
	IDZRV.CountKA = 8;
	// ����������� ���
	IDZRV.CountNip = 17;

	// ��� ������� ������������, ���
	IDZRV.StepTargDir = 5;
	// ������������� ������������ ��� ������ ��������� ���
	IDZRV.needCalcTargDir = true;
	// ������������� ������������ ���������� ��������� ��� ������� ���
	IDZRV.needForNipFile = IDZRV.CountNip > 1 ? true : false;
	// ������������� ������������ ���������� ��������� ��� ������� ��
	IDZRV.needForKAFile = IDZRV.CountKA > 1 ? true : false;
	// ������������� ������� ����� � ������������ 
	// (�������� �� ������ ����� ���������)
	IDZRV.needZipRezults = true;

	// ������� ����� � ������� �����
	IDZRV.NipIndexes = new int[IDZRV.CountNip];
	memset(IDZRV.NipIndexes, 0, sizeof(int)*IDZRV.CountNip);

	// ������� ����� � ������� ��������� � ������������� 
	// ����� (NIPS � ����� ZNIP.cpp), ��� ������� ����������
	// ��������� ������� ���
	IDZRV.NipIndexes[0] = 1;
	IDZRV.NipIndexes[1] = 2;
	IDZRV.NipIndexes[2] = 3;
	IDZRV.NipIndexes[3] = 4;
	IDZRV.NipIndexes[4] = 5;
	IDZRV.NipIndexes[5] = 6;
	IDZRV.NipIndexes[6] = 7;
	IDZRV.NipIndexes[7] = 8;
	IDZRV.NipIndexes[8] = 9;
	IDZRV.NipIndexes[9] = 10;
	IDZRV.NipIndexes[10] = 11;
	IDZRV.NipIndexes[11] = 12;
	IDZRV.NipIndexes[12] = 13;
	IDZRV.NipIndexes[13] = 14;
	IDZRV.NipIndexes[14] = 15;
	IDZRV.NipIndexes[15] = 17;
	IDZRV.NipIndexes[16] = 16;

	IDZRV.VFN.clear();

	//----------------------------------------------------------
	// ��������� ������� ��������

	// ������ �� ��� ������ �������������� ���
	IDZRV.NumKA = new int[IDZRV.CountKA];
	memset(IDZRV.NumKA, 0, sizeof(int)*IDZRV.CountKA);
	// ������ ��������
	IDZRV.KA = new ZModZRV[IDZRV.CountKA];
	// ��������� �������
	IDZRV.NU = new ZNU[IDZRV.CountKA];
	// ��������� ��� ��������� ���
	IDZRV.pfn = new FILE*[IDZRV.CountNip];
	memset(IDZRV.pfn, 0, sizeof(FILE*)*IDZRV.CountNip);
	// ������� ���������������� ���
	IDZRV.VarDocZRV = 1;

	//----------------------------------------------------------
	// ������� � ������������
	// ���������������� ����������� ����������� � �������
	// _����������\\���, ����������� ��� ��� ���������� �������������
	// � ������� �������� ������
	char LDir[256], SDir[256];	MakeZRVDir(LDir, SDir);

	//----------------------------------------------------------
	// ����� ���
	ZLSF LSF; memset(&LSF, 0, sizeof(LSF));
	LSF.num = 1;
	LSF.vgpz = GPZ90;
	LSF.ngpz = 8;
	LSF.mgpz = 8;
	LSF.vatm = ATMNULL;  // ATMSMA81 ; ATMNULL
	LSF.vsvd = 0;
	LSF.varsp = 0;
	LSF.vTide = TIDESTEP2; // TIDENULL, TIDESTEP2, TIDEPOSEIDON
	LSF.isDU = 0;
	LSF.isSun = 1;
	LSF.isMoon = 1;

	memcpy(&IDZRV.LSF, &LSF, sizeof(ZLSF));

	// ������������ ������ �� 8 �� � ���� ����������, 
	// ���������� �� 90 ��� � ��������� � ������������ �� 45 ��������
	// ����� �����������
	rc = RunUpSysBuild_4_4(IDZRV.NU, LSF, IDZRV.CountKA); if (rc) return rc;;

	//----------------------------------------------------------
	// ������������� ������� ���������������� ��
	// ������� ���� 8 - �� ����� ��
	for (j = 0; j < (unsigned int)IDZRV.CountKA; j++) {
		rc = IDZRV.KA[j].Init(IDZRV.NU[j], &LSF); if (rc) return rc;;
		IDZRV.NumKA[j] = IDZRV.NU[j].Key.ka;
	}

	//----------------------------------------------------------
	// ������ ���������������� ����������� ��� ������� �� ��������
	if (IDZRV.needForKAFile) {
		for (j = 0; j < (unsigned int)IDZRV.CountKA; j++) {
			FILE** pfp = &IDZRV.KA[j].fp;
			PrintMng->PrePrint(&IDZRV.KA[j], SDir, "_TaskCalcZRV", pfp, FullName);
			if (pfp && *pfp) {
				// ���� �������� ������� ������, ��...				
				// ��������� � ������ ��� ��� ����������� ������ ������� �����
				// ��� ��������� ��� � �������� ����
				IDZRV.VFN.push_back(FullName);
				// ��������� ������������ ���������� ���������
				DocHeaderZRV(*pfp, &IDZRV.KA[j], 1, IDZRV.NipIndexes, IDZRV.CountNip);
				// ���������������� �� � ��� 
				PrintMng->PrintNU(IDZRV.NU[j], nullptr, 3, *pfp);
				PrintMng->PrintLSF(LSF, 1, *pfp);
				// ���������������� ��������� ������� � ���
				DocTablHeaderZRV(*pfp);
	}	}	}

	//----------------------------------------------------------
	// �������� ���������� ��������� ��� ������� ���
	if (IDZRV.needForNipFile) {
		for (j = 0; j < (unsigned int)IDZRV.CountNip; j++) {
			FILE**  pfp = &IDZRV.pfn[j];
			int  NumNip = ALLNIPS[IDZRV.NipIndexes[j]].Num;
			char FN[64]; sprintf(FN, "_TaskCalcZRV_NIP_%d\0", NumNip);

			PrintMng->PrePrint(nullptr, SDir, FN, pfp, FullName);
			if (pfp && *pfp) {
				// ���� �������� ������� ������, ��...				
				// ��������� � ������ ��� ��� ����������� ������ ������� �����
				// ��� ��������� ��� � �������� ����
				IDZRV.VFN.push_back(FullName);
				// ��������� ������������ ���������� ���������
				DocHeaderZRV(*pfp, IDZRV.KA, IDZRV.CountKA, &IDZRV.NipIndexes[j], 1);
				// ���������������� ���� ��
				for (int i = 0; i < IDZRV.CountKA; i++) PrintMng->PrintNU(IDZRV.NU[i], nullptr, 3, *pfp);
				// ���������������� ���
				PrintMng->PrintLSF(LSF, 1, *pfp);
				// ���������������� ��������� ������� � ���
				DocTablHeaderZRV(*pfp);
	}	}	}

	// �������� ������� ��������� ��� ���� �������������� ��� 
	// ��� ���� ��
	rc = PrintMng->PrePrint(nullptr, SDir, "_TaskCalcZRV", nullptr, FullName);
	if (!rc) {
		// ���� �������� ������� ������...
		// ��������� � ������ ��� ��� ����������� ������ ������� �����
		// ��� ��������� ��� � �������� ����
		IDZRV.VFN.push_back(FullName);
		// ��������� ������������ ���������� ���������
		DocHeaderZRV(PrintMng->TextFile(), IDZRV.KA, IDZRV.CountKA, IDZRV.NipIndexes, IDZRV.CountNip);
		// ���������������� �� � ���
		for (j = 0; j < (unsigned int)IDZRV.CountKA; j++) PrintMng->PrintNU(IDZRV.NU[j], nullptr, 3);
		PrintMng->PrintLSF(LSF, 1);
	}
	 
	//----------------------------------------------------------
	// ������������ ���� �� �� ������ ������� ����� ���������
	for (j=0; j<(unsigned int)IDZRV.CountKA; j++) { 
		rc = IDZRV.KA[j].GoToVit(IDZRV.Vitn) ; if (rc) return rc; ; 
		Vit[j] = IDZRV.Vitn ;
		// "�����������" � ������ �������� ���� ��� ������������ ���
		// !!!! ������ ������� ���� ������� ��������� ������� ���
		// ���������� �� �������� �������� ���������� �������� !!!!
		IDZRV.KA[j].RunUpZRV(IDZRV.CountNip, IDZRV.NipIndexes) ;
		// ����� ������ �������� ������������� ��������� ������� �������
		// ������������ ������� ��� (����, �����, ��������), ������ ��
		// ������ ������� CompletZRV() 
	}

	// ���������������� ��������� ������� � ���
	DocTablHeaderZRV(PrintMng->TextFile());

	//----------------------------------------------------------
	// ���� ������������� �������� �� �� �������� ���������
	// � �������� ��� 
	while(1) {
		for (j=0; j<(unsigned int)IDZRV.CountKA; j++) { 
			// ������ ���� �� ��������� ������� �����
			if (Vit[j]>=IDZRV.Vitk) continue ;
			// ��� ��������������
			rc = IDZRV.KA[j].GoStepA() ; if (rc) break ;
			// �������� �������� �����
			Vit[j] = IDZRV.KA[j].GetVitok(_SK_ASKTE) ;
		}
		if (rc) break ;

		// ��������, ��������� �� �� ������� ���� ���� �� ���� ��
		// ���� �� � ����� ��� 
		for (j=0; j<(unsigned int)IDZRV.CountKA; j++) { 
			int j1 ;
			for (j1=0; j1<IDZRV.CountNip; j1++) { 
				if (IDZRV.KA[j].EventMng.ZRV[j1].Nip>0) break ;
			}
			if (j1<IDZRV.CountNip) break ;
		}		
		// ��� ������������� � ����������� ���������� ����������� 
		// ������ � ���������� ��������
		// ���� ��� �� ����� "��������" ���� (j=IDZRV.CountKA), 
		// �� ��������� ����� ������������ ������� � ��������� ����
		if (j==IDZRV.CountKA) {
			ResetZRVData(PrintMng->TextFile(), IDZRV.KA, IDZRV.CountKA);
		}

		// �������� ���������� ��� ���� ��������� �� ��������� �����
		int CountEnd = 0 ;
		for (j=0; j<(unsigned int)IDZRV.CountKA; j++) { 
			if (Vit[j]<IDZRV.Vitk) break ;
			CountEnd++ ;
		}
		// ���� ��� �� �������� ��������� �����, �� ��������� ���� �������������
		if (CountEnd==IDZRV.CountKA) break ;
	}

	// ����������� ������������ ������� ���
	for (j=0; j<(unsigned int)IDZRV.CountKA; j++) { 
		// ���������� ��������������� ������������ �������� �������
		// ������������ ������� ��� (����, �����, ��������), 
		// ������������ ���������� ����� ��� ����� ������...
		IDZRV.KA[j].CompletZRV() ;
	}

	//----------------------------------------------------------
	// ���������� ���������������� ������� ����������� ���
	// ��� ���� �� �� ���� �����
	DocEndZRV(PrintMng->TextFile());

	// ���������� ���������������� ������
	PrintMng->PostPrint();
	PrintMng->ClosePrint();

	// �������� ����������� ���������� ������������ �������
	PrintMng->ShowTextFile();
	PrintMng->ShowHtmlFile();

	// �������� ���� ��������� ���������� ��������� ��� ���������������� ��
	TargDirFileCloseAll() ;

	//----------------------------------------------------------
	if (IDZRV.needForKAFile) {
		for (j=0 ; j<(unsigned int)IDZRV.CountKA ; j++) { 
			FILE** pfp = &IDZRV.KA[j].fp ;

			// ���������� ���������������� ������� ��� ����������� 
			// ��� ������� �� ��������
			DocEndZRV(*pfp) ;

			// ���������� ����������������
			PrintMng->PostPrint(pfp);
			PrintMng->ClosePrint(pfp);
			IDZRV.KA[j].fp = nullptr ;
	}	}

	//----------------------------------------------------------
	if (IDZRV.needForNipFile) {
		for (j=0 ; j<(unsigned int)IDZRV.CountNip ; j++) { 
			FILE** pfp = &IDZRV.pfn[j] ;

			// ���������� ���������������� ������� ��� ����������� 
			// ��� ������� ��� ��������
			DocEndZRV(*pfp) ;

			// ���������� ����������������
			PrintMng->PostPrint(pfp);
			PrintMng->ClosePrint(pfp);
			IDZRV.pfn[j] = nullptr ;
	}	}

	delete[] IDZRV.NumKA ;      IDZRV.NumKA = nullptr ;
	delete[] IDZRV.KA ;         IDZRV.KA = nullptr ;
	delete[] IDZRV.NU ;         IDZRV.NU = nullptr ;
	delete[] IDZRV.NipIndexes ; IDZRV.NipIndexes = nullptr ;
	delete[] IDZRV.pfn ;        IDZRV.pfn = nullptr ; 

	//----------------------------------------------------------
	// ������ ������ � ������������ ��������, ���� ��� ����������
	if (IDZRV.needZipRezults) {
		Zip  FZ;
		char cod_return[512];
		for (j = 0; j < IDZRV.VFN.size(); j++) {
			// NameFile   - �������� ����� � ���� � ����
			// DirOut     - ���� ��������� �����
			// flag_zip   - true - ����������, false - �����������
			// over       - true - ����������, false - ����������
			// cod_return - ����� ������ ��������
			int rc = FZ.zip_unzip((char*)IDZRV.VFN[j].c_str(), LDir, true, true, cod_return);
			// �������� ���� ��������� ������ ���� �� ��� ��������� ����
			if (!rc) rc = remove((char*)IDZRV.VFN[j].c_str());
	}	}

	return rc ;
}

//-------------------------------------------------------------------------------
