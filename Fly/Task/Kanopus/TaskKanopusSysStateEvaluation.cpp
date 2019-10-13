//-------------------------------------------------------------------------------
// TaskKanopusSysStateEvaluation.cpp
// �. ��������� 14.02.2019
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>			// ����������� ���� ������ ��������
#include <FlyMMath.h>			// �������������� �������
#include <FlyOrbitDesign.h>		// �������������� �����
#include <FlyMagicLNS.h>		// ����� ������� �����

//---------------------------------------------------------------------------
#include "FlightThisAndThat.h"
#include <FlyMagicLNSSource/MagicSolution.h>

int TaskMagicFlyBegin(ZSMagicLnsSolution* MS, int task, ZMSC& KA,
	ZNU& NU, ZLSF& LSF, bool ResetNU, ZCPrintManager* PrintMng,
	FILE** pfp);

//---------------------------------------------------------------------------
int TaskKanopusSysStateEvaluation()
{
	int rc = 0 ;
	// ���������� ����� ��� 
	ZLSF LSF ;
	LSF.num    = 1 ;
	LSF.vgpz   = GPZ90 ;
	LSF.ngpz   = 16 ;
	LSF.mgpz   = 16 ;
	LSF.vatm   = ATMSMA81 ;  // ATMSMA81 ; ATMNULL
	LSF.vsvd   = 0 ;       
	LSF.varsp  = 0 ;
	LSF.vTide  = TIDESTEP2 ; // TIDENULL, TIDESTEP2, TIDEPOSEIDON
	LSF.isDU   = 1 ;
	LSF.isSun  = 1 ;
	LSF.isMoon = 1 ;
	memset(LSF.isPlanet, 0, sizeof(LSF.isPlanet )) ;

	// ������ �� ��� 6-� �� �������
	ZNU NU[6] ;
	ZNU NUOSK[6] ;
	// ������ ����� ��� ������ � ��� ��01 
	char NU01FileName[6][64] = {
		"��01 312",
		"��01 313",
		"��01 314",
		"��01 315",
		"��01 316",
		"��01 806" } ;

	// �������� �� �� ����� � ���� ��� ��01 � ���������� ���������
	// ������� ��� ������� �� �� ��������� 1250 ������
	int i ; 
	for (i=0; i<6; i++) {
		// ��� ����� � ��� ��01
		char FN[512] ; sprintf(FN, "����� %s.txt", NU01FileName[i]) ;
		// �������� �� ��� i-�� ���� �������
		rc = LoadNU01(FN, NULL, NULL, NU[i]) ; RRC ;
		// ������������� ��������� �������
		//rc = FlightCalcHightContour(NU[i], LSF, 1250, NU01FileName[i]) ; RRC ;
	}

	// ������ �������� ����. � ��� ����� �� ��������� ������ ����
	// ��� ������� �� �������
	for (i=0; i<6; i++) {
		//rc = FlightCalcHightSimplified(NU[i], LSF, NU01FileName[i], 365, 15) ; RRC ;
	}

	// ������ ���������� ����� �� ������ �� ��� ������� �� �� ������� 
	double az[6], ez[6], wz[6], a0[6], e0[6], w0[6] ;
	printf( "���������� ���������� ������� �� ���\n") ; 
	for (i=0; i<6; i++) {
		NUOSK[i] = NU[i] ;
		ConvertSkNu(NUOSK[i], _SK_OSKTE) ;
		double Tdr0 = 5692 ;
		double i0 = TO_GRAD(NUOSK[i].X[2]) ;
		double W0 = TO_GRAD(NUOSK[i].X[3]) ;
		double a  = TO_KM  (NUOSK[i].X[0]) ;
		double e  = NUOSK[i].X[1] ;
		double w  = TO_GRAD(NUOSK[i].X[4]) ;
		a0[i] = a ;
		e0[i] = e ;
		w0[i] = w ;
		rc = OrbitDesignOSHfromTi(&LSF, Tdr0, i0, W0, a, e, w) ; if (rc) return rc;
		az[i] = a ;
		ez[i] = e ;
		wz[i] = w ;
		printf( "   da = %lf,  de = %lf,  dw = %lf\n", 
		az[i]-a0[i], ez[i]-e0[i], wz[i]-w0[i]) ; 		
	}

	FILE* pfs ; pfs = nullptr ;
	ZCPrintManager PrintSysMng ;	

	//---------------------------------------------------------------------------
	// ������ ���������� ������ ������� �� �������, ���������������
	// ������������ �����.

	//---------------------------------------------------------------------------
	ZSMagicLnsSolution* MS = CreateMagicLnsSolution();

	//---------------------------------------------------------------------------
	for (i=0; i<6; i++) {
		// ������������� ������
		int indexTask = 21 ;

		// TypeOrb - ��� ��������� ������
		//		SPO_ORB_EL	- ��������� ��������� ������ ������� ��� e, w 
		MS->MZ.ORB() = SPO_ORB_EL ;	

		//-----------------------------------------------------------------------
		// ������������� �� ����� � ������ ��������, ��������� ����������������
		ZMSC   KA ;		// ������ �������� ��
		FILE*  fp=NULL ;// ���� ���������������� �����������
		ZCPrintManager PrintMng ;	// ��������� ����������������
		PrintMng.SetPrintFileSticker(NU01FileName[i]) ;

		// �������� � ���������� ���� �������� ������
		rc = TaskMagicFlyBegin(MS, indexTask, KA, NU[i], LSF, false,
			 &PrintMng, &fp) ; RRC ;

		//----------------------------------------------------------------------
		// ������������ ������� �� ���� (����������� � ������������� ������ 
		// � �������������� GUI)
		MS->MZ.VITIMP(0) = NU[i].vit+1 ;	// ����� ������� ��������
		MS->MZ.VITIMP(1) = NU[i].vit + 2;	// ����� ������� ��������
		MS->MZ.VIT() = NU[i].vit + 3;	// �������� ����� �� ������� 
										// �������������� ������� �������
		// ��������� ��������� ������
		MS->MZ.PART() = 5692.8;	// ������������� ������
		MS->MZ.PARe() = ez[i];		//	
		MS->MZ.PARw() = wz[i];		//	

		// ������� ���� ��������� ����� ������ ��� ���� �����
		// �� ��������� �������� ���������� �������� ����������� 
		// ������ � � ������
		MS->MP.isEstimateOrb = true;

		//----------------------------------------------------------------------
		// ������� ������ ������� ���������� ��������� �� ������� MZ 
		ZSLoopFact P1 ;	// ��������� �������� �� ����� ������� ��� 
		ZSLoopFact P2 ;	// ��������� �������� �� ����� ������� ��� 
		ZSLoopFact PN ;	// ��������� ��������� �������� �� �������� �����
		ZSLoopFact PR ;	// ��������� �������� ��������� �������� �� �������� ����� 
		ZSFlightRule U ;	// ���������� ����������

		//----------------------------------------------------------------------
		// ���������������� ������� ������ ������� ���������� ��������� 
		// ���������� ��������� ������ ���� ��
		rc = MagicSolution(MS, &PrintMng, fp, LSF, KA, NU[i], nullptr, 
						   &P1, &P2, &PN, &PR, &U) ; 
		// �������� ��������� � ������������ �������
		PrintMng.ClosePrint(&fp);
		if (rc) return rc;

		if (!pfs) {
			PrintSysMng.PrePrint(&KA, "�������", "SystemState", &pfs) ;
			PrintSysMng.PrintHeaderDoc("\n   ������ ��������� ������� �������\n\n" ) ;
			if (pfs) {
				for (int j=0; j<6; j++) {
					PrintSysMng.PrintNU(NU[j], "") ;
				}
				PrintSysMng.PrintLSF(LSF) ;
				fprintf(pfs, "\n\n") ;
				fprintf(pfs, "_____________________________________________________________________________________\n") ;
				fprintf(pfs, "     |���������� �� ������������ �����     |  �������������� ��������� ���������� \n") ;
				fprintf(pfs, "__ __|_____________________________________|_________________________________________\n") ;
				fprintf(pfs, "  �� | �. �������   �������������� ������� | U 1-���  dV 1-���    U 2-���   dV 2-���\n") ;
				fprintf(pfs, "     |         ��                     ���� |    ����     �/���       ����      �/���\n") ;
				fprintf(pfs, "_____|_____________________________________|_________________________________________\n") ;
			}
		} 
		if (pfs) {
			fprintf(pfs,"%4.0d |%11.4lf  %13.7lf   %7.3lf | %7.3lf %8.3lf(%c)  %7.3lf %8.3lf(%c)\n", 
			NU[i].Key.ka, az[i]-a0[i], ez[i]-e0[i], wz[i]-w0[i], 
			U.us[0], 1e3*fabs(U.dV[0]), U.alf[0]>0? '-':'+',
			U.us[1], 1e3*fabs(U.dV[1]), U.alf[1]>0? '-':'+') ; 	
		}
		if (i==5) {
			fprintf(pfs, "_____|_____________________________________|_________________________________________\n") ;
			PrintSysMng.PostPrint(&pfs) ;
		}
	}

	FreeMagicLnsSolution(MS);

	return 0 ;
}

//-------------------------------------------------------------------------------




