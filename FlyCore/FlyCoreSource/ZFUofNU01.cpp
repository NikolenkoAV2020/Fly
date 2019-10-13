//---------------------------------------------------------------------------
// ZFUofNU01.cpp
// �. ��������� 15.03.2019
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
#include <FlyCoreSource\\ZFUofNU01.h>
#include <FlyCoreSource\\ZCNumParser.h>

//---------------------------------------------------------------------------
// ����������� ��� ��01
//
// ������ ����� ������ ��01:
//
//		��01:312,26,0,0,21;
//		1.312,1,352;
//		2.19.01.2019;
//		3.07.41.29.232;
//		4.-1.2493935104+03;
//		5.+6.7719309892+03;
//		6.+0.0000000000+00;
//		7.+1.4675401079+00;
//		8.+2.6111330851-01;
//		9.+7.5468935407+00;
//		10.+5.9506820-04;
//		12.242441311,10;
//		�.
//
// �������������� ��� ��� ���������� � ��������� ����� FileName, ����
// ����� ���� ��������� �� ��������� ����� ����� pf, ���� ��������� � 
// ������ Text.
// ��������� � ���� ��������� �� � NU. 
//---------------------------------------------------------------------------
int LoadNU01(char* shortFileName, FILE* pf, char* Text, 
			 ZNU& NU,
			 vector<string>* rows,
			 bool PrintToCout)
{
	int rc = 0 ;

	FAST_SEMAPFORE_OPEN("ParserAndLoadNU01") ;

	char FileName[512];
	// ������ ���� � ����� ��, ���� ��� ����������
	if (shortFileName) {
		sprintf(FileName, "%s\\NU01\\%s\0", DIRUOF, shortFileName);
	}
	
	// ��������������� ���� � �����
	DATETIMEDMB DT ;	

	// �� ��������� ��� ���� �� �������
	memset(&NU, 0, sizeof(ZNU)) ;

	//-----------------------------------------------------------------------
	// ������ ���������� ����� �� �����
	//-----------------------------------------------------------------------

	// ������ ��� ������� ����� �� ��������� ����� ����� � ����������� 
	ZNumericParser Parser ;	

	// ������� ������ �����
	Parser.WhatToLookFor.lf1 = "��01:";
	Parser.WhatToLookFor.lf2 = "NU01:";

	// ��������������� ������ �����
	rc = Parser.Execute(shortFileName? FileName : nullptr, 
						pf, Text, rows, PrintToCout);
	if (rc) { rc = 1; goto end; }

	//-----------------------------------------------------------------------
	// ������ ����������� ������� ��������� �����
	//-----------------------------------------------------------------------
	double D ;
	int    i, I, N, IndI ;
	// �����
	char S[512] ;						
	IndI = 0 ;
	// ����������� ��������� �����
	N = Parser.GetCountString() ;	
	//-----------------------------------------------------------------------
	// ������ ������ �����
	// �������� ����� �����
	//-----------------------------------------------------------------------
	rc = Parser.FindStr(0, IndI, S) ; 
	if (rc || (strcmp("��01", S) && strcmp("NU01", S))) {
		// �������� ������ ���
		rc = 1 ; goto end ; 
	}
	// ����� �� (������ ������)
	rc = Parser.FindInt(0, IndI, I) ; if (rc) { rc = 2 ; goto end ; }
	NU.Key.ka = I ;
	rc = Parser.FindInt(0, IndI, I) ; if (rc) { rc = 2 ; goto end ; }
	NU.Key.nom = I ;
	rc = Parser.FindInt(0, IndI, I) ; if (rc) { rc = 2 ; goto end ; }
	NU.Key.type = I ;
	rc = Parser.FindInt(0, IndI, I) ; if (rc) { rc = 2 ; goto end ; }
	NU.Key.mod = I ;
	rc = Parser.FindInt(0, IndI, I) ; if (rc) { rc = 2 ; goto end ; }
	NU.Key.bc = I ;
 	//-----------------------------------------------------------------------
	// ������ �� ������� ������� � �������, �� ���� ������ ���������
	unsigned long fullkey ;
	fullkey = 0 ;
	for (i=1; i<N; i++) {
		// ����� ����� �������� � ������
		int n = Parser.GetCountItems(i) ;
		// �������� �������� ���������� ���
		if (n==0) break ;
		if (n==1) {
			if ( Parser.IsText(i, 0, "C") || 
				 Parser.IsText(i, 0, "�")) break ;
		}
		// ������ ��������� ����� ��������...
		// � ������
		int ns ;
		IndI = 0 ;
		rc = Parser.FindInt(i, IndI, ns) ; if (rc) { rc = 3 ; goto end ; }
		// ������ � ������������ � ������� ������
		switch(ns) {
			case 1: // ����� �� � �����
				rc = Parser.FindInt(i, IndI, I) ; if (rc) break ;
				rc = Parser.FindInt(i, IndI, I) ; if (rc) break ;
				NU.sk = (SK_TYPE)I ;
				rc = Parser.FindInt(i, IndI, I) ; if (rc) break ;
				NU.vit = I ;
				fullkey+= 2 ;
				break ;
			case 2: // ����, ����� 
				rc = Parser.FindInt(i, IndI, I) ; if (rc) break ;
				DT.D = I ;
				rc = Parser.FindInt(i, IndI, I) ; if (rc) break ;
				DT.M = I ;
				rc = Parser.FindInt(i, IndI, I) ; if (rc) break ;
				DT.G = I ;
				// �����
				if (NU.sk != 1) {
					rc = Parser.FindInt(i, IndI, I); if (rc) break;
					switch (NU.sk){
						case 0: // � ��� ������ ���
							if (I == 0) NU.sk = _SK_ASKTE;
							else if (I == 1975) NU.sk = _SK_ASK1975;
								 else if (I == 2000) NU.sk = _SK_ASK2000;
									  else rc = 1;
							break;
						case 8: // � ��� ������ ��������� �������� ������
							if (I == 0) NU.sk = _SK_OSKTE;
							else if (I == 1975) NU.sk = _SK_OSK1975;
								 else if (I == 2000) NU.sk = _SK_OSK2000;
									  else rc = 1;
							break;
						default: 
							rc = 1;
					}
				}
				if (rc) break;
				fullkey+= 4 ;
				break ;
			case 3: // �����
				rc = Parser.FindInt(i, IndI, I) ; if (rc) break ;
				DT.h = I ;
				rc = Parser.FindInt(i, IndI, I) ; if (rc) break ;
				DT.m = I ;
				rc = Parser.FindDbl(i, IndI, D) ; if (rc) break ;
				DT.s = (int)D ;
				DT.d = D-DT.s ;
				fullkey+= 8 ;
				break ;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				rc = Parser.FindDbl(i, IndI, D) ; if (rc) break ;
				NU.X[ns-4] = D ;
				fullkey+= (unsigned long)pow(2.0, ns) ;
				break ;
			case 10:
				rc = Parser.FindDbl(i, IndI, D) ; if (rc) break ;
				NU.Sb = D ;
				break ;
			case 11:
				rc = Parser.FindDbl(i, IndI, D) ; if (rc) break ;
				NU.Kp = D ;
				break ;
			case 12:
				// ���������� ����� ���
				rc = Parser.FindInt(i, IndI, I) ; if (rc) break ;	
				// ����� ���������� ����� ���
				rc = Parser.FindInt(i, IndI, I) ; if (rc) break ;
				NU.numlsf = I ;
				// ��� ���������� ������������ �� ���������� ���������������� 
				// ������ ��� � ���������� �� ��� ������ ���, �����������
				// ������� � ������� ���� �������� ������ ��.
				// ������ ���
				// NNMMACFPD
				// ���: 
				//		NN - ����� ��������� �������� ���������� ���.
				//		MM - ����� ������������� ��������.
				//		A  - �������� ����� ������ ���������, 0,1,2,3...,
				//		     ���� 0, �� ��������� �� �����������.
				//		C  - ������� ����� ����-������������ �������������,
				//		     ���� ������ 0.
				//		F  - ������� ���� 1-������, 2-����, 
				//		     4-���� ��������� ��������
				//		P  - ������� ����� ��������������� ���������� ������.
				//		D  - ������� ����� ������ ������������ ���������.
				// 
				// � ����� ����������� ������������� ���� ���������� ������������
				// ������ ��� � ������� � ������� ���� ����� ���, �����������
				// ������ ������ ����������� �������. ����� ���� �������� 
				// ��������� ����� � ��������������� ���� ��������� ��.
				// ���� �������� ��� � ���� ���, �� � ���������� ������� 
				// �� ����������� � ������� ��������� � ��� ��01.
				break ;
		}
		// ����� ��
		if ((ns==2 || ns==3) && (fullkey & 14)) NU.t << DT ;
		if (rc) { rc =-ns ; goto end ; }
	}

	// ������� ���������� �������� � ������ ����������� 
	// ����, ����/���, �������,
	// � ������������ � �������� �������� ���������
	if (NU.sk==_SK_GSK    || NU.sk==_SK_GSKZ  || NU.sk==_SK_ASK1975 || 
		NU.sk==_SK_ASKTES || NU.sk==_SK_ASKTE || NU.sk==_SK_ASK2000) {
		int i ;
		for (i=0; i<3; i++) {
			NU.X[i]  /= 1e4 ; 
			NU.X[i+3]*= 8.64 ; 
		}
	} 
	if (NU.sk==_SK_OSK1975 || NU.sk==_SK_OSKTES || 
		NU.sk==_SK_OSKTE || NU.sk==_SK_OSK2000) {
		NU.X[0]/= 1e4 ; 
		int i ;
		for (i=2; i<6; i++) NU.X[i]*= k_pi/180.0 ; 
	}

	// ���������� ��������������� ������������ � ����������� ������
	// � ��01 ����������� �3 / (���2*��),
	// � ������ �������� ������� �2 / ��, �������
	NU.Sb /= 9.80665;

	// �������� ���������� ���� �������� ����� �����
	if (fullkey!=1022) rc =-1022 ;
end : ;

	FAST_SEMAPFORE_CLOSE() ;
	return rc ; 
}

//---------------------------------------------------------------------------
static void Pmant10_p(double d, char* s)
{
	sprintf(s, "%+.10e\0", d);
	int j = strcspn(&s[0], "e");
	s[j] = 0;
	strcat(&s[j], &s[j + 1]);
}

static void Pmant7_p(double d, char* s)
{
	sprintf(s, "%+.7e", d);
	int j = strcspn(&s[0], "e");
	s[j] = 0;
	strcat(&s[j], &s[j + 1]);
}

//---------------------------------------------------------------------------
int SaveNU01(ZNU& NU, ZLSF* LSF, char* shortFileName, FILE* pf, char* Text)
{
	//-----------------------------------------------------------------------
	// ������ ���� ������ ���� �� ���������� ������ �����
	if (!shortFileName && !pf && !Text) return 1;
	// ���� ������ ��� �����, �� �������� ������ ����. ��� ���������� 
	// ������������ � ������ �����
	if (shortFileName) {
		// ������ ���� � ����� ��
		char FileName[512];
		sprintf(FileName, "%s\\NU01\\%s\0", DIRUOF, shortFileName);
		pf = fopen(FileName, "r");
	}
	// ������ ��� ����� �� ���� ������ �� ���
	if (shortFileName && !pf) return 2;
	if (!pf && !Text) return 3;

	DATETIMEDMB DT;
	short cur = 0,
		  NumSK = 0,
		  Age;

	switch (NU.sk){
		case _SK_ASK1975: Age = 1975;	break;
		case _SK_ASK2000: Age = 2000;	break;
		case _SK_ASKTE:   Age = 0;		break;
		case _SK_GSK:     Age = 0;		NumSK = 1;	break;
		case _SK_OSK1975: Age = 1975;	NumSK = 8;	break;
		case _SK_OSK2000: Age = 2000;	NumSK = 8;	break;
		case _SK_OSKTE:	  Age = 0;		NumSK = 8;	break;
		default: assert(0);
	}

	int  Size = 0;
	char S[600], SP[64];
	char fl_Sb = 0, fl_Sd = 0;

	cur = sprintf(S, "��01:%i,%i,%i,%i,%i;\n", 
	NU.Key.ka, NU.Key.nom, NU.Key.mod, NU.Key.type, NU.Key.bc);

	cur += sprintf(S + cur, "1.%i,%i,%i;\n", 
	NU.Key.ka, NumSK, NU.vit);

	DT << NU.t;
	cur += sprintf(S + cur, "2.%02i.%02i.%04i,%04i;\n", 
	DT.D, DT.M, DT.G, Age);

	cur += sprintf(S + cur, "3.%02i.%02i.%02i.%03i;\n", 
	DT.h, DT.m, DT.s, (int)(DT.d * 1000 + 0.0005) % 1000);

	if (NumSK <= 1) {
		Pmant10_p(NU.X[0] * 10000.0, SP);
		cur += sprintf(S + cur, "4.%s;\n", SP);
		Pmant10_p(NU.X[1] * 10000.0, SP);
		cur += sprintf(S + cur, "5.%s;\n", SP);
		Pmant10_p(NU.X[2] * 10000.0, SP);
		cur += sprintf(S + cur, "6.%s;\n", SP);
		Pmant10_p(NU.X[3] / 8.6400, SP);
		cur += sprintf(S + cur, "7.%s;\n", SP);
		Pmant10_p(NU.X[4] / 8.6400, SP);
		cur += sprintf(S + cur, "8.%s;\n", SP);
		Pmant10_p(NU.X[5] / 8.6400, SP);
		cur += sprintf(S + cur, "9.%s;\n", SP);
	}
	else {
		Pmant10_p(NU.X[0] * 10000.0, SP);
		cur += sprintf(S + cur, "4.%s;\n", SP);
		Pmant10_p(NU.X[1], SP);
		cur += sprintf(S + cur, "5.%s;\n", SP);
		Pmant10_p(TO_GRAD(NU.X[2]), SP);
		cur += sprintf(S + cur, "6.%s;\n", SP);
		Pmant10_p(TO_GRAD(NU.X[3]), SP);
		cur += sprintf(S + cur, "7.%s;\n", SP);
		Pmant10_p(TO_GRAD(NU.X[4]), SP);
		cur += sprintf(S + cur, "8.%s;\n", SP);
		Pmant10_p(TO_GRAD(NU.X[5]), SP);
		cur += sprintf(S + cur, "9.%s;\n", SP);
	}

	// ���������� ��������������� ������������ � ����������� ��� ��01
	// � ��01 ����������� �3 / (���2*��),
	// � ������ �������� ������� �2 / ��, �������
	double Sb = NU.Sb * 9.80665;
	Pmant7_p(Sb, SP);
	cur += sprintf(S + cur, "10.%s;\n", SP);

	Pmant7_p(NU.Kp, SP);
	cur += sprintf(S + cur, "11.%s;\n", SP);

	if (LSF) {
		sprintf(S + cur, "12.%02d%02d%d%d%d%d%d,%d;\n",
		LSF->ngpz, LSF->mgpz,		// ������� ���������� ���.
		LSF->vatm,					// ������� ������ ���������.
		0,							// ������� ����� ����-������������ �������������.
		(LSF->isSun ? 1:0) +		// ����+
		(LSF->isMoon ? 2:0) +		// ������+
		(LSF->vsvd ? 4:0),			// �������� �������� �������.
		(LSF->IsPlanet()? 1:0),		// ���� ���������� ������.
		LSF->isDU,					// ���� ���� ��.
		LSF->num);					// �������� ����� ���.
	} else {
		// ������ ��� �� ���������. 
		sprintf(S + cur, "12.020010700,1;\n");
	}

	return 0;
}

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
//------------------------------------------------------------
//			    ���-2000          ���          ���-����
//X, ��		 3547.509260   -897.659240      3528.579176
//Y, ��		 5881.353276  -6809.505751      5892.731937
//Z, ��		   -4.730527	 -0.000000         0.000000
//Vx, � / � -1709.775038   1492.303964	   -1723.355784
//Vy, � / �  1030.633684   -191.037238      1025.379421
//Vz, � / �  7354.209292   7351.773437       351.773437
//------------------------------------------------------------
//S�(�**2 / ��)              0.012720
//K��(�����)               200.000000
//------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
