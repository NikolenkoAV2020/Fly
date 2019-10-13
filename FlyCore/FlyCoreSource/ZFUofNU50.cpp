//---------------------------------------------------------------------------
// ZFUofNU50.cpp
// �. ��������� 26.04.2019
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
// ����������� �� �� ����������� ��������������� �������� ��,
// ������ "��"
//
// ������ �������:
//
//----- �� �������� �� - 68 ---------------------------------
//����� - 1201    ��� - 10  ��� - 0  �� - 0
//����  11.03.2014   �����  02.12.11.65899  ����� 3902
//����� ���  1015    ����� ��  0			����� ����� 0
//-----------------------------------------------------------
//			    ���-2000          ���          ���-����
//X, ��		 3547.509260   -897.659240      3528.579176
//Y, ��		 5881.353276  -6809.505751      5892.731937
//Z, ��		   -4.730527	 -0.000000         0.000000
//Vx, � / � -1709.775038   1492.303964	   -1723.355784
//Vy, � / �  1030.633684   -191.037238      1025.379421
//Vz, � / �  7354.209292   7351.773437       351.773437
//-----------------------------------------------------------
//S�(�**2 / ��)              0.012720
//K��(�����)               200.000000
//-----------------------------------------------------------
//
// �������������� ��� ��� ���������� � ��������� ����� FileName, ����
// ����� ���� ��������� �� ��������� ����� ����� pf, ���� ��������� � 
// ������ Text.
// ��������� � ���� ��������� �� � NU. 
//---------------------------------------------------------------------------
int LoadNU50(char* FileName, FILE* pf, char* Text, ZNU& NU, bool PrintToCout)
{
	FAST_SEMAPFORE_OPEN("ParserAndLoadNU50");

	// ��������������� ���� � �����
	DATETIMEDMB DT;
	std::string nameParam;
	double D;
	int    rc=0, i, I, N, IndI;

	// �� ��������� ��� ���� �� �������
	memset(&NU, 0, sizeof(ZNU));

	//-----------------------------------------------------------------------
	// ������ ���������� ����� �� �����
	//-----------------------------------------------------------------------

	// ������ ��� ������� ����� �� ��������� ����� ����� � ����������� 
	ZNumericParser Parser;

	// ������� ������ �����
	Parser.WhatToLookFor.lf1 = "�� �������� ��";
	Parser.WhatToLookFor.lf2 = "";

	// ��������������� ������ �����
	rc = Parser.Execute(FileName, pf, Text, nullptr, PrintToCout);
	if (rc) { rc = 1; goto end; }

	//-----------------------------------------------------------------------
	// ������ ����������� ������� ��������� �����
	//-----------------------------------------------------------------------

	// ����������� ��������� �����
	N = Parser.GetCountString();
	//-----------------------------------------------------------------------
	// ������ ������ �����
	// �������� ����� �����
	//-----------------------------------------------------------------------

	// ����� �� (1-� ������)
	IndI = 0;
	rc = Parser.FindInt(0, IndI, I); if (rc) return rc;
	NU.Key.ka = I;
	// ����� �� (2-� ������)
	IndI = 0;
	rc = Parser.FindInt(1, IndI, I); if (rc) return rc;
	NU.Key.nom = I;
	rc = Parser.FindInt(1, IndI, I); if (rc) return rc;
	NU.Key.type = I;
	rc = Parser.FindInt(1, IndI, I); if (rc) return rc;
	NU.Key.mod = I;
	rc = Parser.FindInt(1, IndI, I); if (rc) return rc;
	NU.Key.bc = I;
	// ����
	IndI = 0;
	rc = Parser.FindInt(2, IndI, I); if (rc) return rc;
	DT.D = I;
	rc = Parser.FindInt(2, IndI, I); if (rc) return rc;
	DT.M = I;
	rc = Parser.FindInt(2, IndI, I); if (rc) return rc;
	DT.G = I;
	// �����
	rc = Parser.FindInt(2, IndI, I); if (rc) return rc;
	DT.h = I;
	rc = Parser.FindInt(2, IndI, I); if (rc) return rc;
	DT.m = I;
	rc = Parser.FindDbl(2, IndI, D); if (rc) return rc;
	DT.s = (int)D;
	DT.d = D - DT.s;
	NU.t << DT;

	// ��
	NU.sk = _SK_ASK2000;

	// �����
	IndI = 0;
	rc = Parser.FindInt(6, IndI, I); if (rc) return rc;
	NU.vit = I;

	// ����������
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
	rc = Parser.FindStr(i, IndI, nameParam, "S�");
	if (!rc) {
		rc = Parser.FindDbl(i, IndI, D); if (rc) return rc;
		// � ������ ������� S� ������� � ��/�**2 ������� �� g ������ �� �����  
		// NU.Sb = D / 9.80665;
		i++;
		IndI = 0;
	}
	rc = Parser.FindStr(i, IndI, nameParam, "K��");
	if (!rc) {
		rc = Parser.FindDbl(i, IndI, D); if (rc) return rc;
		NU.Kp = D;
	}

end:;
	FAST_SEMAPFORE_CLOSE();
	return 0;
}

//---------------------------------------------------------------------------
