//-------------------------------------------------------------------------------
// FlightCalcHightSimplified.cpp
// ������� ��������� ������� ����������
// 
// �. ��������� 14.02.2019
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>			// ����������� ���� ������ ��������

//-------------------------------------------------------------------------------
// ��������������� ��������� ������������ � ����������� ����� �� �����
// �� �������� ��������� ������
//		NU		- �� �� ������� ����� ��������������
//		LSF		- ����� ��� � ������� ����������� �������
//		maxTime	- �������� ������������� � ������
//		SkipVit	- ���������� ������ ����� ������� �� ������� ��������������� 
//				  ������������� ������
// 
int FlightCalcHightSimplified(ZNU& NU, ZLSF& LSF, char* FilePrintSticker,
							  double maxTime, int SkipVit)
{
	printf("Height frofile Model forcing...\n") ;
	int i, rc = 0 ; 
	int CountStep = 720 ; 

	//---------------------------------------------------------------------------
	// ������ �������� ��
	ZMSC KA ;
	// ������������� ������ ��������
	rc = KA.Init(NU, &LSF) ; RRC ;

	//---------------------------------------------------------------------------
	// ��������� ���������������� ���������
	// ��������� ����������������
	ZCPrintManager PrintMng ;
	// ��������� ���������������� �������� �� ������
	PrintMng.MPK.FromVitOff() ;
	// ���������������� ����������� �������� �� ������� (�������������)
	PrintMng.MPK.AflatOn() ;
	// ���������� ��������������� ���������� ��������
	//PrintMng.MPK << MODPR_VIT ;
	// �������������� ����������������� ����������������� ����������� ��������.
	// ����������: ��� ���������, ��� �������� (����� ������ ����� �������),
	// ������������ ����������� �������� ��� ������������� � ��������.
	// �������� ��������������� ���������� �������� ����� ��� ������
	// ������� PMPK.UPP(0).value = ...
	// PrintMng.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "�����", "����") ;
	// PrintMng.MPK.UPP(0).value = 0 ;
	for (i=0 ; i<=CountStep ; i++) {  
		PrintMng.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "h", "��") ;
	}

	// ������������� ����������������

	// ��������� ������ � ����� ����� ���������
	PrintMng.SetPrintFileSticker(FilePrintSticker) ;
	// �������� ���������
	PrintMng.PrePrint(&KA, "�������", "_FlightHProf") ;
	// ��������� ���������
	PrintMng.PrintHeaderDoc("\n   �������� ������� (FlightCalcHightSimplified)\n\n") ;

	// ���������������� ��
	PrintMng.PrintNU(NU, NULL, 3) ;
	// ���������������� ���������� ����� ���
	PrintMng.PrintLSF(LSF, 1) ;

	//---------------------------------------------------------------------------
	// ��������� ���������������� �������� ����������� � ������������ �����
	ZCPrintManager PrintMngH ;
	PrintMngH.MPK.FromVitOff() ;
	PrintMngH.MPK.AflatOn() ;
	PrintMngH.MPK << ZSUserPrintParam(UPPint,    16, 6, "�����", "") ;
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmin1", "��") ;
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmin2", "��") ;
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmax1", "��") ;
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmax2", "��") ;
	// ��������� ������ � ����� ����� ���������
	PrintMng.SetPrintFileSticker(FilePrintSticker) ;
	// �������� ���������
	PrintMngH.PrePrint(&KA, "�������", "_FlightHminmax") ;
	// ��������� ���������
	PrintMngH.PrintHeaderDoc("\n   ����������� �����������/������������ ����� �� ������� �����\n   (FlightCalcHightSimplified)\n\n") ;
	// ���������������� ��
	PrintMngH.PrintNU(NU, NULL, 3) ;
	// ���������������� ���������� ����� ���
	PrintMngH.PrintLSF(LSF, 1) ;

	//---------------------------------------------------------------------------
	// ���� ������������� 
	ZSC_SI SI ;
	double U ;
	double du = 360.0/CountStep ; 
	// ������ ����� ������������� ����� ����� ��������� 
	// �� ������ ��������� �������
	int    Vit = NU.vit+1 ;
	// ���������������� �������� ��������� ������ ��� ������� �����
	U = 0 ;
	for(i=0 ; i<=CountStep ; i++) {
		PrintMng.MPK.UPP(i).value = U ;
		U+= du ;
	}
	PrintMng.PrintMSC(&KA, &SI, NULL, NULL) ;

	//---------------------------------------------------------------------------
	// ���� ������������� �������� �� �������� ���������
	while(1) {
		// ������������� �� ������ �������� �����
		rc = KA.GoToVit(Vit, &SI); if (rc) break;
		printf("\rTime %.1lf from %.1lf  ", SI.ta-NU.t, maxTime) ;
		// ������ ��� ������ ����� ����� (������ �����)
		PrintMng.MPK.UPP(0).value = SI.THBLFR[1] ;
		// ������������� ������������ ������� ����� (��� � ���� ������)
		KA.RunUpVit() ;
		//-----------------------------------------------------------------------
		// ������������� �������� � �������� �������� ����� 
		// � �������� ����� du �� ��������� ������
		U = du ;
		// ���� ������������� ����������� ����� ��� ���������������
		// ����������� 2-� �������� ����������� � 2-�
		// �������� ������������ �����
		// ������������� ����������� � ����� ������ � ����� �����
		// ������� ��� 4-� ������������� ������
		// ����������� ������������ ������������� ����� � �����������
		// � ����������� �������� �������� �� ��� �� ����� ����� ����
		// ������������ ��� ����������� ����� �� 1-� �� 3-�
		for (int i=1 ; i<=2*CountStep ; i++) {  
			rc = KA.GoToVitU(Vit, U, &SI); if (rc) break;
			if (i<=CountStep) {
				// ���������������� ����������������� ����������� ��������.
				// ������� �������� ��������������� ���������,
				// �������������� ����� ��� ������ ������ ��������� 
				// PrintMng.MPK << ZSUserPrintParam(UPPdouble...
				PrintMng.MPK.UPP(i).value = SI.THBLFR[1] ;
			}
			U += du ;
			// �������� ����������� ���� 4-� ����� 
			// � ������������ ������������� ����� �����
			// ���� ����� �������� � ���� �� ����� �� ������� ��
			// ����� ����������� 1-2-3 ���� �������������� � ������� ���������
			// ����������� �������� ������������� ������
			if (KA.FV.Hmin.isFind && KA.FV.Hmin2.isFind &&
				KA.FV.Hmax.isFind && KA.FV.Hmax2.isFind &&
				i>=CountStep) {
				break ;
			}
		}
		// ���������� ������������ ������� �����
		KA.CompletVit() ;
		// ���� � ����� ���� ��������� ������ �� ������� ���� 
		// ����� ������������
		if (rc) break ;

		// ���������������� ���������� ���������� ��������
		// ������������ � �������� ���� ������ ������� �����
		PrintMng.PrintMSC(&KA, &SI, NULL, NULL) ;

		//-----------------------------------------------------------------------
		// ���������������� ����������� �������� ������������� �����
		if (KA.FV.Hmin.ute>100.0 && KA.FV.Hmin.ute<260.0) {
			ZSC_VP HM   = KA.FV.Hmin ;
			KA.FV.Hmin  = KA.FV.Hmin2 ;
			KA.FV.Hmin2 = HM ;
		}
		PrintMngH.MPK.UPP(0).value = Vit ;
		PrintMngH.MPK.UPP(1).value = KA.FV.Hmin.H ; 
		PrintMngH.MPK.UPP(2).value = KA.FV.Hmin2.H ; 
		PrintMngH.MPK.UPP(3).value = KA.FV.Hmax.H ; 
		PrintMngH.MPK.UPP(4).value = KA.FV.Hmax2.H ; 
		PrintMngH.PrintMSC(&KA, &SI, NULL, NULL) ;

		//-----------------------------------------------------------------------
		// �������� ������� ���������� ���������� �������������� (��������)
		if (SI.ta > NU.t+maxTime) break ;

		//-----------------------------------------------------------------------
		// ���������� ������ �����
		Vit++ ;
		// ��������� SkipVit ������
		Vit+= SkipVit ;
	}
	printf("\n");

	//---------------------------------------------------------------------------
	// ���������� ����������������
	PrintMng.PostPrint(); 
	PrintMng.ClosePrint();
	PrintMngH.PostPrint(); 
	PrintMngH.ClosePrint();

	return rc ;
}

//-------------------------------------------------------------------------------
