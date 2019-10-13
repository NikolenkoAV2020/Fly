//-------------------------------------------------------------------------------
// MagicPrint.cpp : 
// �. ��������� 27.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>		// �������������� �������
#include <FlyCore.h>		// �������������� �������
#include <FlyMagicLNS.h>	//

//-------------------------------------------------------------------------------
void MagicPrint(
	ZSMagicLnsSolution *MS, 
	ZCPrintManager *PrintMng, 
	FILE *fp,
	ZSLoopFact   &P1, 
	ZSLoopFact   &P2, 
	ZSLoopFact   &PN, 
	ZSLoopFact   &PR, 
	ZSFlightRule &U, 
	int NumIter)
{
	int  i ; 
	char S[512];

	CFormatString FDataTime ;
	CFormatString FTdr ;

	FDataTime.FormatedDateTime(0) ;
	FDataTime.SetFixedLastField(true) ; 
	FDataTime.SetDec(4) ;

	FTdr.FormatedTimeHMS() ;
	FTdr.SetFixedLastField(true) ; 
	FTdr.SetFixedFirstField(false) ;
	FTdr.SetFormatIO(EDF_SEC) ;
	FTdr.SetInterval(true) ;
	FTdr.SetDec(4) ;

	bool isContE12 = MS->MZ.ContE12() ;

	if (NumIter<0) {
		//--------------------------------------------------------------------------------------------
		SmartPrintfS(fp, "\n������:   ") ; 
		char Dsk[256]; MS->MZ.GetVarDsk(Dsk);
		int  i0 = 0 ;
		while(1) {
			bool isEnd = false ;
			for (i=i0 ; i<256 ; i++) { 
				if (Dsk[i] =='\n') break ; 
				if (Dsk[i] =='\0') { isEnd = true ; break ; }
			}
			if (i<(256-2)) Dsk[i] = '\0' ;
			if (!i0) SmartPrintfS(fp, "%s\n", &Dsk[i0]) ; 
			else SmartPrintfS(fp, "          %s\n", &Dsk[i0]) ; 
			i0 = i+1 ;
			if (isEnd || i0>=256-2) break ; 
		}
		for (i=0 ; i<U.n ; i++) { 
			if (U.n>1)	{
				SmartPrintfI(fp, "����� %d-�� �������", i+1) ; 
				SmartPrintfL(fp, "          %d\n", MS->MZ.VITIMP(i));
			}
			else SmartPrintfL(fp, "����� �������               %d\n", MS->MZ.VITIMP(i));
		}
		SmartPrintfL(fp, "�������� �����              %d\n", MS->MZ.VIT());

		if (MS->MZ.ORB() == SPO_ORB_OMIV)
			SmartPrintfS(fp, "������                      ������������ ��������� ������\n" ) ; 
		if (MS->MZ.ORB() == SPO_ORB_DU)
			SmartPrintfS(fp, "������                      ����������� ����������\n" ) ; 
		if (MS->MZ.ContT()) {
			SmartPrintfS(fp, "������ ��.        [�.�.�.]  " ) ; 
			FTdr << MS->MZ.PART();
			SmartPrintfS(fp, "%s\n", FTdr.GetText()) ;
		}
		if (MS->MZ.ContA()) {
			SmartPrintfS(fp, "�. �������        [��]      ") ; 
			SmartPrintfD(fp, "%.4lf\n", MS->MZ.PARa());
		}
		if (MS->MZ.ContL()) {
			SmartPrintfS(fp, "�������           [����]    ") ; 
			SmartPrintfD(fp, "%.4lf\n", MS->MZ.PARL());
		}
		if (MS->MZ.ContDL()) {
			SmartPrintfS(fp, "���. �����        [����]    ") ; 
			SmartPrintfD(fp, "%.4lf\n", MS->MZ.PARdL());
		}
		if (MS->MZ.Contu()) {
			SmartPrintfS(fp, "���. ������       [����]    ");
			SmartPrintfD(fp, "%.4lf\n", MS->MZ.PARu());
		}
		if (MS->MZ.ContHs()) {
			SmartPrintfS(fp, "��. ������        [��]      ") ; 
			SmartPrintfD(fp, "%.4lf\n", MS->MZ.PARhs());
		}
		if (MS->MZ.ContHmin()) {
			SmartPrintfS(fp, "���.������        [��]      ") ; 
			SmartPrintfD(fp, "%.4lf\n", MS->MZ.PARh());
		}
		if (MS->MZ.ContBH()) {
			SmartPrintfS(fp, "������ ������ %s  [����]    ", MS->MZ.PARBv() ? "��" : "��");
			SmartPrintfD(fp, "%.4lf\n", MS->MZ.PARBh());
		}
		if (MS->MZ.ContE() || isContE12) {
			SmartPrintfS(fp, "��������������              ") ;
			SmartPrintfD(fp, "%.8lf\n", MS->MZ.PARe());
		}
		if (MS->MZ.ContW() || isContE12) {
			SmartPrintfS(fp, "�������           [����]    ") ; 
			SmartPrintfD(fp, "%.5lf\n", MS->MZ.PARw());
		}
		SmartPrintfD(fp, "���� ��           [���]     %.3lf\n", MS->DU.i[MS->DU.mu].p);
		SmartPrintfD(fp, "�������� ����     [���]     %.3lf\n", MS->DU.i[MS->DU.mu].pu);
		SmartPrintfD(fp, "��������� �����   [��]      %.3lf\n", MS->MZ.M());
	}

	if (NumIter>=0) {
		SmartPrintfI(fp, "  ���������� ������� �� �������� - %d\n\n", NumIter ) ; 
	}

	//--------------------------------------------------------------------------------------------
	if (U.n<=0) return ;
	SmartPrintfS(fp, "\n" ) ; 
	sprintf(S, "  2. ��������� ���������� ��������� ��\n");
	SmartPrintfS(fp, "%s", S);
	if (fp)	PrintMng->PrintText(S);

	SmartPrintfS(fp, "_______________________________________________________________________________\n" ) ; 
	SmartPrintfS(fp, "  ���. �����  ���. ������   ����-����� ���./����.    ������������    ������� Vx\n" ) ; 
	SmartPrintfS(fp, "                   [����]                                   [���]       [�/���]\n" ) ; 
	SmartPrintfS(fp, "_______________________________________________________________________________\n" ) ; 
	double dts = 0 ;
	double dvs = 0 ;
	for (i=0 ; i<U.n ; i++) { 
		FDataTime << U.t[i] ;
		SmartPrintfI(fp, "    %d", i+1) ; 
		SmartPrintfL(fp, "  %5d", (long)U.vs[i]) ; 
		SmartPrintfD(fp, " %12.4lf", U.us[i]) ; 
		SmartPrintfS(fp, "   %s", FDataTime.GetText()) ; 
		SmartPrintfD(fp, " %12.4lf", U.dts[i]) ; 
		SmartPrintfD(fp, "%12.4lf\n", 1e3*U.dV[i]) ; 
		SmartPrintfD(fp, "             %12.4lf", U.ue[i]) ; 
		FDataTime << U.t[i]+U.dts[i]/k_cbc ;
		SmartPrintfS(fp, "   %s\n", FDataTime.GetText()) ; 
		dts+= U.dts[i] ;
		dvs+= fabs(U.dV[i]) ;
	}
	if (U.n>1) {
		SmartPrintfS(fp, "                                              _________________________________\n" ) ; 
		SmartPrintfS(fp, "                                              �����:") ; 
		SmartPrintfD(fp, " %12.4lf", dts) ; 
		SmartPrintfD(fp, "%12.4lf \n", dvs*1e3) ; 
	}
	SmartPrintfS(fp, "_______________________________________________________________________________\n" ) ; 
	SmartPrintfS(fp, "\n" ) ; 

	//--------------------------------------------------------------------------------------------
	sprintf(S, "  3. ��������� �������� �� �������� �����\n");
	SmartPrintfS(fp, "%s", S);
	if (fp)	PrintMng->PrintText(S);

	int linelen = 12 ;
	if (MS->MZ.ContT())    linelen += 13;
	if (MS->MZ.ContA())    linelen += 13;
	if (MS->MZ.ContL())    linelen += 12;
	if (MS->MZ.ContDL())   linelen += 12;
	if (MS->MZ.Contu())    linelen += 13;
	if (MS->MZ.ContHs())   linelen += 12;
	if (MS->MZ.ContHmin()) linelen += 12;
	if (MS->MZ.ContBH())   linelen += 15;
	if (MS->MZ.ContE() || isContE12)
						   linelen+= 15 ; 
	if (MS->MZ.ContW() || isContE12)
					       linelen+= 13 ; 
	PrintMng->MPF.PrintLineAflet(fp, linelen) ;
	SmartPrintfS(fp, "            " ) ; 
	if (MS->MZ.ContT())    SmartPrintfS(fp, "   ������ ��.");
	if (MS->MZ.ContA())    SmartPrintfS(fp, "  �. �������");
	if (MS->MZ.ContL())    SmartPrintfS(fp, "     �������");
	if (MS->MZ.ContDL())   SmartPrintfS(fp, "  ���. �����");
	if (MS->MZ.Contu())    SmartPrintfS(fp, "  ���. ������");
	if (MS->MZ.ContHs())   SmartPrintfS(fp, "  ��. ������");
	if (MS->MZ.ContHmin()) SmartPrintfS(fp, "  ���.������");
	if (MS->MZ.ContBH())   SmartPrintfS(fp, "  ������ ������");
	if (MS->MZ.ContE() || isContE12)
					   SmartPrintfS(fp, "   ��������������") ; 
	if (MS->MZ.ContW() || isContE12)
					   SmartPrintfS(fp, "    �������") ; 
	SmartPrintfS(fp, "\n" ) ; 
	SmartPrintfS(fp, "            " ) ; 
	if (MS->MZ.ContT())    SmartPrintfS(fp, "     [�.�.�.]");
	if (MS->MZ.ContA())    SmartPrintfS(fp, "        [��]");
	if (MS->MZ.ContL())    SmartPrintfS(fp, "      [����]");
	if (MS->MZ.ContDL())   SmartPrintfS(fp, "      [����]");
	if (MS->MZ.Contu())    SmartPrintfS(fp, "       [����]");
	if (MS->MZ.ContHs())   SmartPrintfS(fp, "        [��]");
	if (MS->MZ.ContHmin()) SmartPrintfS(fp, "        [��]");
	if (MS->MZ.ContBH())   SmartPrintfS(fp, "         [����]");
	if (MS->MZ.ContE() || isContE12)
					   SmartPrintfS(fp, "               ") ; 
	if (MS->MZ.ContW() || isContE12)
					   SmartPrintfS(fp, "       [����]") ; 
	SmartPrintfS(fp, "\n" ) ; 
	PrintMng->MPF.PrintLineAflet(fp, linelen) ;
	SmartPrintfS(fp, "  ��������� " ) ; 
	if (MS->MZ.ContT()) { FTdr << PN.T; SmartPrintfS(fp, "%s", FTdr.GetText()); }
	if (MS->MZ.ContA())    SmartPrintfD(fp, "  %10.4lf", PN.a);
	if (MS->MZ.ContL())    SmartPrintfD(fp, "  %10.4lf", PN.L);
	if (MS->MZ.ContDL())   SmartPrintfD(fp, "  %10.4lf", PN.dLs);
	if (MS->MZ.Contu())    SmartPrintfD(fp, "  %11.5lf", PN.up);
	if (MS->MZ.ContHs())   SmartPrintfD(fp, "  %10.4lf", PN.Hs);
	if (MS->MZ.ContHmin()) SmartPrintfD(fp, "  %10.4lf", PN.hmin);
	if (MS->MZ.ContBH())   SmartPrintfD(fp, "  %13.4lf", PN.Bhmin);
	if (MS->MZ.ContE() || isContE12)
					   SmartPrintfD(fp, "  %14.8lf", PN.e) ; 
	if (MS->MZ.ContW() || isContE12)
					   SmartPrintfD(fp, "  %10.5lf", PN.w) ; 
	SmartPrintfS(fp, "\n" ) ; 	
	SmartPrintfS(fp, "  ��������� " ) ; 
	if (MS->MZ.ContT()) { FTdr << MS->MZ.PART(); SmartPrintfS(fp, "%s", FTdr.GetText()); }
	if (MS->MZ.ContA())    SmartPrintfD(fp, "  %10.4lf", MS->MZ.PARa());
	if (MS->MZ.ContL())    SmartPrintfD(fp, "  %10.4lf", MS->MZ.PARL());
	if (MS->MZ.ContDL())   SmartPrintfD(fp, "  %10.4lf", MS->MZ.PARdL());
	if (MS->MZ.Contu())    SmartPrintfD(fp, "  %11.5lf", MS->MZ.PARu());
	if (MS->MZ.ContHs())   SmartPrintfD(fp, "  %10.4lf", MS->MZ.PARhs());
	if (MS->MZ.ContHmin()) SmartPrintfD(fp, "  %10.4lf", MS->MZ.PARh());
	if (MS->MZ.ContBH())   SmartPrintfD(fp, "  %13.4lf", MS->MZ.PARBh());
	if (MS->MZ.ContE() || isContE12)
		SmartPrintfD(fp, "  %14.8lf", MS->MZ.PARe());
	if (MS->MZ.ContW() || isContE12)
		SmartPrintfD(fp, "  %10.5lf", MS->MZ.PARw());
	SmartPrintfS(fp, "\n" ) ; 	
	SmartPrintfS(fp, "  ��������� " ) ; 
	if (MS->MZ.ContT()) { FTdr << PR.T; SmartPrintfS(fp, "%s", FTdr.GetText()); }
	if (MS->MZ.ContA())    SmartPrintfD(fp, "  %10.4lf", PR.a);
	if (MS->MZ.ContL())    SmartPrintfD(fp, "  %10.4lf", PR.L);
	if (MS->MZ.ContDL())   SmartPrintfD(fp, "  %10.4lf", PR.dLs);
	if (MS->MZ.Contu())    SmartPrintfD(fp, "  %11.5lf", PR.up);
	if (MS->MZ.ContHs())   SmartPrintfD(fp, "  %10.4lf", PR.Hs);
	if (MS->MZ.ContHmin()) SmartPrintfD(fp, "  %10.4lf", PR.hmin);
	if (MS->MZ.ContBH())   SmartPrintfD(fp, "  %13.4lf", PR.Bhmin);
	if (MS->MZ.ContE() || isContE12)
					   SmartPrintfD(fp, "  %14.8lf", PR.e) ; 
	if (MS->MZ.ContW() || isContE12)
					   SmartPrintfD(fp, "  %10.5lf", PR.w) ; 
	SmartPrintfS(fp, "\n" ) ; 	
	SmartPrintfS(fp, "  �������   " ) ; 
	if (MS->MZ.ContT()) { FTdr << (PN.T - MS->MZ.PART()); SmartPrintfS(fp, "%s", FTdr.GetText()); }
	if (MS->MZ.ContA())    SmartPrintfD(fp, "  %10.4lf", PN.a - MS->MZ.PARa());
	if (MS->MZ.ContL())    SmartPrintfD(fp, "  %10.4lf", ValiDA(PN.L - MS->MZ.PARL(), 1));
	if (MS->MZ.ContDL())   SmartPrintfD(fp, "  %10.4lf", PN.dLs - MS->MZ.PARdL());
	if (MS->MZ.Contu())    SmartPrintfD(fp, "  %11.5lf", PN.up - MS->MZ.PARu());
	if (MS->MZ.ContHs())   SmartPrintfD(fp, "  %10.4lf", PN.Hs - MS->MZ.PARhs());
	if (MS->MZ.ContHmin()) SmartPrintfD(fp, "  %10.4lf", PN.hmin - MS->MZ.PARh());
	if (MS->MZ.ContBH())   SmartPrintfD(fp, "  %13.4lf", ValiDA(PN.Bhmin - MS->MZ.PARBh(), 1));
	if (MS->MZ.ContE() || isContE12)
		SmartPrintfD(fp, "  %14.8lf", PN.e - MS->MZ.PARe());
	if (MS->MZ.ContW() || isContE12)
		SmartPrintfD(fp, "  %10.5lf", ValiDA(PN.w - MS->MZ.PARw(), 1));
	SmartPrintfS(fp, "\n" ) ; 	
	SmartPrintfS(fp, "  ������    " ) ; 
	if (MS->MZ.ContT()) { FTdr << (PR.T - MS->MZ.PART()); SmartPrintfS(fp, "%s", FTdr.GetText()); }
	if (MS->MZ.ContA())    SmartPrintfD(fp, "  %10.4lf", PR.a - MS->MZ.PARa());
	if (MS->MZ.ContL())    SmartPrintfD(fp, "  %10.4lf", ValiDA(PR.L - MS->MZ.PARL(), 1));
	if (MS->MZ.ContDL())   SmartPrintfD(fp, "  %10.4lf", PR.dLs - MS->MZ.PARdL());
	if (MS->MZ.Contu())    SmartPrintfD(fp, "  %11.5lf", PR.up - MS->MZ.PARu());
	if (MS->MZ.ContHs())   SmartPrintfD(fp, "  %10.4lf", PR.Hs - MS->MZ.PARhs());
	if (MS->MZ.ContHmin()) SmartPrintfD(fp, "  %10.4lf", PR.hmin - MS->MZ.PARh());
	if (MS->MZ.ContBH())   SmartPrintfD(fp, "  %13.4lf", ValiDA(PR.Bhmin - MS->MZ.PARBh(), 1));
	if (MS->MZ.ContE() || isContE12)
		SmartPrintfD(fp, "  %14.8lf", PR.e - MS->MZ.PARe());
	if (MS->MZ.ContW() || isContE12)
		SmartPrintfD(fp, "  %10.5lf", ValiDA(PR.w - MS->MZ.PARw(), 1));
	SmartPrintfS(fp, "\n" ) ; 	
	PrintMng->MPF.PrintLineAflet(fp, linelen) ;
	SmartPrintfS(fp, "\n" ) ; 
}

//-------------------------------------------------------------------------------