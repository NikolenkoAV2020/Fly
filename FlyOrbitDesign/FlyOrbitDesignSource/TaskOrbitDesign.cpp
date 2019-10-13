//-------------------------------------------------------------------------------
// TaskOrbitDesign.cpp : 
// �. ��������� 28.12.2018
//-------------------------------------------------------------------------------
#include "stdafx.h"

//-------------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>		// ����������� ���� ������ ��������
#include <FlyMMath.h>		// �������������� �������
#include <FlyOrbitDesign.h>	//

//-------------------------------------------------------------------------------
static double k_eps_kms = 26332713071.154318 ;

// �������������� ������
double Re = 6378.136 ;
// ������� ������ �����
double Rz = 6371.0 ;
double mu = 398600.44 ;
double eps= k_eps_kms ; // 2.634e10 ;

// ������������ ��������� �������� ���������� ���
double C20 =-0.0010826257 ;
double C30 = 2.5324070e-6 ;
double C40 = 1.6305600e-6 ;
double C50 = 2.2791850e-7 ;
double C60 =-5.1721630e-7 ;  
	
double J2 = 1.0826257e-3 ;
double J3 =-2.532407e-6 ;
double J5 =-0.2279185e-6 ;
double J7 =-0.350079e-6 ;

// ���������� ����� ���
// �.�. �������� ������ ��������
ZLSF LSF ;

char FilePrintSticker[64] = "" ;

//-------------------------------------------------------------------------------
// ��� ������� ������
// 1-� ����. ������� ����������� ��� �������� ������� ������
int OrbitDesignSSOStage1h(double h0, double& a0, double& e0, double& w0, double& i0) ; 

//-------------------------------------------------------------------------------
// ��� �������������� �������
// 1-� ����. ������� ����������� ��� �������� ������������� �������
int OrbitDesignSSOStage1T(double Tdr0, double& a0, double& e0, double& w0, double& i0) ; 
// 2-� ���� ��� ��������� �������������� �������
// ��������� �������� ������� �� ��������� �������, �������� � ������ ���� ���
int OrbitDesignSSOStage2T(ZLSF* LSF, double Tdr, double a0, double e0, double i0, double W0, double w0, double u0, double& ak) ;

//-------------------------------------------------------------------------------
// ��� �������� ��������� � ��������� ������
// 1-� ����. ������� ����������� ��� �������� k-�������� ��������� � � ����������
// ������ ����� l ������ ������
int OrbitDesignSSOStage1kl(double k, double l, double& Tdr0, double& a0, double& e0, double& w0, double& i0) ; 

//-------------------------------------------------------------------------------
// ���������� ������������ ��������� �� ������ ����� �� ��������� ����������
// ��������� ������� - �������� ��������-���������� ������������� ���������� ������
// ���� isStage2 = false,	�� ������� �� �������� �����, �� ���� � ������� 
//							����������� ����������� ������ ������������
// ���� isStage2 = true,	�� ����������� ������ ���� ������� � �����������
//							�������������� ������� � ������ ���� ��� ��������
//					i0,	-	������� ��������� ������ �� ������ �����
//					a0,
//					e0, w0,
//					Tdr0
int OrbitDesignSSOStage12I(bool isStage2, double i0, double& a0, double& e0, double& w0, double& Tdr0) ;

// ������ ������� ������ �� ���������� � ������ 7-� ��������� ��������
int OrbitDesignStage3 (double a0, double e0, double w0, double i0, double Tdr0, double& ik) ; 
// ���������� ��������� ������ �� ������ �����
int OrbitDesignStage4 (double a0, double e0, double i0, double w0, double W0, double u0, 
					   double& es, double& ws, double& e1s, double& e2s) ; 
// ������ �� �������
int OrbitDesignStage5 (double a0, double e0, double i0, double w0, double W0, double u0, 
					   double* dL=NULL, double* dLs=NULL, 
					   double* Hmin=NULL, double* Hmax=NULL, double* Hs=NULL) ; 
// ������ �� ������ � ����������� ��������� �������
int OrbitDesignStage6 (double a0, double e0, double i0, double w0, double W0, double u0) ;
int OrbitDesignStage7 (double a0, double e0, double i0, double w0, double W0, double u0) ;

// ������ �������������� ������� � ������ ���� ���
int CalcPeriodInFullForce(ZLSF* LSF, double a0, double e0, double i0, double W0, double w0, double u0, double& Tdr) ;
// �������������� ������ ��� ��� ��������� �������
int OrbitDesignSSOfromPeriod(double Tdr0) ;
// �������������� ������ ��� ��� �������� ��������� ������ �����
int OrbitDesignSSOfromMultiplicity(int K, int L) ; 
// ���������� ���������� ������ ������������ ��������� ������������ ����� 
int CalcParamMCSHOfromA(double a, double& i, double& e, double& w) ;
// ��������� ���������� ������ ������������ ��������� ������������ �����
// �� ������� ��������� ����� � ������������ ����������������� ������ ������
int EstParamMCSHOfromA(double a, double i, double e, double w, double& ik) ;

//-------------------------------------------------------------------------------
// �������������� �����
// ������������ ��������� ������, � ��� ����� � ������������� ����������
//-------------------------------------------------------------------------------
int TaskOrbitDesign()
{
	// ���������� ����� ���
	// �.�. �������� ������ ��������
	LSF.num    = 1 ;
	LSF.vgpz   = GPZ90 ;
	LSF.ngpz   = 16 ;
	LSF.mgpz   = 16 ;
	LSF.vatm   = ATMNULL ;  // ATMSMA81 ; ATMNULL
	LSF.vsvd   = 0 ;       
	LSF.varsp  = 0 ;
	LSF.vTide  = TIDENULL ; // TIDENULL, TIDESTEP2, TIDEPOSEIDON
	LSF.isDU   = 0 ;
	LSF.isSun  = 0 ;
	LSF.isMoon = 0 ;
	memset(LSF.isPlanet, 0, sizeof(LSF.isPlanet )) ;

	// ������� ��������������

	//---------------------------------------------------------------------------
	//double a, i, e, w ;
	//a = 6879.872 ; 
	// ����� ���������� ��� �������������� ���� ������������ �����
	//CalcParamMCSHOfromA(a, i, e, w) ;
	// ��������������� ��� ��� ��������� ����������
	//double Tdr ;
	//OrbitDesignSSOStage12I(false, i, a, e, w, Tdr) ;
	//OrbitDesignSSOStage12I(true,  i, a, e, w, Tdr) ;
	// ���������� ��������� ������� �
	//OrbitDesignStage6  (a, e, i, w, 0, 0) ;
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	// ����� ���������� ��� �������������� ���� ������������ �����
	//CalcParamMCSHOfromA(a, i, e, w) ;
	// ���������� ��������� ������� �
	//sprintf(ZPrintManager::CommonFileSticker, "����_0700ATM\0") ; ; 
	//OrbitDesignStage7(a, e, i, w, 0, 0) ;
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	//i = 65.939 ;
	//OrbitDesignStage6  (a, e, i, w, 0, 0) ;
	//EstParamMCSHOfromA (a, i, e, w, ik) ;
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	// ��� ��������� ������� ����������� ���
	// OrbitDesignSSOfromPeriod(95.664*60.0) ; 
	//---------------------------------------------------------------------------

	FAST_SEMAPFORE_OPEN("TaskOrbitDesignBlock") ;

	// ��������� ��������� �������� ����������
	VKLTOrb VKL ;
	// ���������� ��������� ���� ��������� �������� ����������
	// �������� �������            �� 1/16 �� 1/14 �����
	// ��������� ����������� ����� �� 1    �� 14
	// ��������� ����� ������ 
	// ��������� ������            �� 14   �� 224 
	BuildLotsOfKLT(1.0/16.0, 1.0/14.0, 1, 21, 14, 400, VKL) ;

	// ��� ������������� ������ ����������
	int rc, N = VKL.size() ;
	N=5 ;
	for (int j=0; j<N; j++) {
		int K = VKL[j].K ;
		int L = VKL[j].L ;
		rc = OrbitDesignSSOfromMultiplicity(K, L) ;	RRC ;
	}

	FAST_SEMAPFORE_CLOSE() ;

	return 0 ;
}

//-------------------------------------------------------------------------------
// ���������� ��������������� ������� gam2... gam7
void CalcGam2357(double a, double& gam2, double& gam3, double& gam5, double& gam7)
{
	// ��������� ��������������� ������� � ������� �������
	double Rea = Re/a ;
	// ������������ "�����"
	gam2= J2*pow(Rea,2) ;
	gam3=-J3*pow(Rea,3) ;
	gam5=-J5*pow(Rea,5) ;
	gam7=-J7*pow(Rea,7) ;
}

//-------------------------------------------------------------------------------
// ���������� ���������� ������ ������������ ��������� ������������ ����� 
// (��� �� ������������� ���������� ������) ��� ��������� �������� 
// ������� ������� a
// ����������
//		i	- ���������� 
//		e	- ��������������
//		w	- ��������� ������� ������
// ��������� ���������� i � ��������� �� 0 �� 90 ��������
// ������ ����� ������ ������� 180-i !!!!
//
// ������� �������� ������ ������ �������������� �����, 
// � ������ ������������� �������� �������� �����������. ����� ���� ���������� 
// ���������� �������� � ������ ���� ��� ������ �� ������� ��������� ����� 
// � ����������������� ������ ������, �������� � ������ � ����������� ������
// 90 � 270 ��������.
//-------------------------------------------------------------------------------
int CalcParamMCSHOfromA(double a, double& i, double& e, double& w)
{
	// ������������ gam
	double gam2, gam3, gam5, gam7 ;
	CalcGam2357(a, gam2, gam3, gam5, gam7) ;

	// ������������ ���-�� ��������� 3-�� ������� ��� ���������� ����������
	double c2 =-5.0/8.0 * gam5 ; 
	double c3 = 35.0/256.0 * gam7 ; 
	double K[4] ;
	K[0] =-429.0*c3 ;  
	K[1] = 792.0*c3+21.0*c2 ;  
	K[2] =-432.0*c3-28.0*c2-5.0/2.0*gam3 ;  
	K[3] =  64.0*c3+ 8.0*c2+2.0*gam3 ;  
	// ������� ����������� ���������
	// K[0]*s3+K[1]*s2+K[2]*s+K[3] = 0 ;
	
	double Xr[3], Xi[3] ;
	int    rc, CountX ;
	rc = MSolv3DegreeEquation(K, Xr, Xi, &CountX) ;	if (rc || CountX!=1) return 1 ;    
	// ������� ������ ����������
	double s2= Xr[0] ; if (s2<=0) return 2 ;
	// ����� ����������
	double s = sqrt(s2) ; 
	// ���������� ������ � ��������
	i = Asin(s, 1) ;
	// �������������� ������
	// ex = e
	// ey = 0 ===> w = 0 !!!! 
	e = gam2*(1.5-s2) ;
	// �������� ������� ������ 
	w = 0 ;

	return 0 ;
}

//-------------------------------------------------------------------------------
// ��������� ���������� ������ ������������ ��������� ������������ �����
// �� ������� ��������� ����� � ������������ ����������������� ������ ������
int EstParamMCSHOfromA(double a, double i, double e, double w, double& ik)
{
	int    rc ;

	// ��������� ������� ��������
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a) ;
	NU.X[1] = e ;
	NU.X[2] = TO_RAD(i+1) ;
	NU.X[3] = TO_RAD(0) ;
	NU.X[4] = TO_RAD(w) ;
	NU.X[5] = TO_RAD(0) ;
	NU.Sb   = 0.0005 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

	int Vit = NU.vit ;

	// ������ �������� ��
	ZMSC KA ;
	// ������������� ������ ��������
	rc = KA.Init(NU, &LSF); RRC;

	// ����� �� ������ ����� �� 
	rc = KA.GoToVit(Vit); RRC;
	// ���������� ��������� ������ �������� �� ������ ������ �����
	int HStory = 0;
	HStory = KA.Story(HStory); if (!HStory) return-1 ;

	//---------------------------------------------------------------------------
	// ��������� ������� ������������ i ��������� (�2-�1)(i) = 0 
	//---------------------------------------------------------------------------
	while (1) {
		double dH[2] ;
		double epsi = 1e-2 ; // ����
		for (int j=0; j<2; j++) {
			// ������������� �� ��������� ������ 90 ����
			ZSC_SI* FS ;
			rc = KA.GoToVitU(Vit, 90.0) ; RRC ;
			FS = &KA.FSR ; if (!FS || FS->E.ID!=ModEvent_ONARG) return-2 ;
			double H1 = FS->THBLFR[5] ; 
			// ������������� �� ��������� ������ 270 ����
			rc = KA.GoToVitU(Vit, 270.0) ; RRC ;
			FS = &KA.FSR ; if (!FS || FS->E.ID!=ModEvent_ONARG) return-3 ;
			double H2 = FS->THBLFR[5] ; 
			// ��������������� ��������� ������ �������� �� ������ ������ �����
			rc = KA.ReStory(HStory) ; RRC ;

			// ���������� �����
			dH[j] = H2-H1 ;
			//
			if (!j) {
				NU.X[2] = RadA(i+epsi) ;
				KA.SetVector(NU.X, NU.vit, NU.t, NU.M0+NU.MF, _SK_OSKTE) ;
		}	}
		// ����������� �������� ����� �� ���������� ������
		double ddH = (dH[1]-dH[0])/epsi ;
		// �������� � ���������, �������������� �������� �������� ����� � 
		// ����������������� ������ ������
		double di = dH[0]/ddH ;
		i+= di ;
		// �������� ������� ���������� ��������
		if (fabs(dH[0]) < 0.001) break ;
		// ����� �������� ���������� ������
		NU.X[2] = RadA(i) ;
		// ����� ������� ������
		KA.SetVector(NU.X, NU.vit, NU.t, NU.M0+NU.MF, _SK_OSKTE) ;
	}

	// ������������ ������, ���������� ����� ��� ���������� ��������� ������
	if (HStory) KA.FreeStory(&HStory) ;
	return rc ;
}

//-------------------------------------------------------------------------------
// �������������� ������ ��� ��� ��������� �������
int OrbitDesignSSOfromPeriod(double Tdr0) 
{
	double a0, e0, w0, i0, W0, u0 ;
	W0 = 0.0 ;
	u0 = 0.0 ;

	sprintf(FilePrintSticker, "T=%.0lf-%.2d", Tdr0, (int)(10*(Tdr0-(int)Tdr0))) ;

	// ������� ����������� ���������� ������
	OrbitDesignSSOStage1T(Tdr0, a0, e0, w0, i0) ; 
	// ��������� �������� ������� �� ��������� �������
	// �������� � ������ ���� ���
	OrbitDesignSSOStage2T(&LSF, Tdr0, a0, e0, i0, W0, w0, u0, a0) ;
	// ��������� ���������� ��� �� ������ �������� ������� �������
	OrbitDesignStage3 (a0, e0, w0, i0, Tdr0, i0) ; 
	//
	OrbitDesignSSOStage2T(&LSF, Tdr0, a0, e0, i0, W0, w0, u0, a0) ;
	OrbitDesignStage3 (a0, e0, w0, i0, Tdr0, i0) ; 
	// ������� � ����������
	double es, ws, e1s, e2s ;
	OrbitDesignStage4 (a0, e0, i0, w0, W0, u0, es, ws, e1s, e2s) ; 
	// � �������� ��������� ������������ ������ �� ������ ����� ���������
	// ���������� ���������
	e0 = es ;
	w0 = ws ;
	// ������ �������
	OrbitDesignStage5 (a0, e0, i0, w0, W0, u0) ; 
	// ������� �� ������ � ����������� ��������� �������
	OrbitDesignStage6 (a0, e0, i0, w0, W0, u0) ;
	printf("\n") ;

	return 0 ;
}

//-------------------------------------------------------------------------------
// �������������� ������ ��� ��� �������� ���������
int OrbitDesignSSOfromMultiplicity(int K, int L) 
{
	double Tdr0 ;
	double a0, e0, w0, i0, W0, u0 ;
	W0 = 0.0 ;
	u0 = 0.0 ;

	// ������� ����������� ���������� ������
	// ��������� ������ �� K ����� � L ������.
	OrbitDesignSSOStage1kl(K, L, Tdr0, a0, e0, w0, i0) ;

	sprintf(FilePrintSticker, "KL=%d-%d_T=%.0lf-%.2d", K, L, Tdr0, (int)(10*(Tdr0-(int)Tdr0))) ;

	// ��������� �������� ������� �� ��������� �������
	// �������� � ������ ���� ���
	OrbitDesignSSOStage2T(&LSF, Tdr0, a0, e0, i0, W0, w0, u0, a0) ;
	// ��������� ���������� ��� �� ������ �������� ������� �������
	OrbitDesignStage3 (a0, e0, w0, i0, Tdr0, i0) ; 
	//
	OrbitDesignSSOStage2T(&LSF, Tdr0, a0, e0, i0, W0, w0, u0, a0) ;
	OrbitDesignStage3 (a0, e0, w0, i0, Tdr0, i0) ; 
	// ������� � ����������
	double es, ws, e1s, e2s ;
	OrbitDesignStage4 (a0, e0, i0, w0, W0, u0, es, ws, e1s, e2s) ; 
	// � �������� ��������� ������������ ������ �� ������ ����� ���������
	// ���������� ���������
	// ������ �������
	double dL, dLs, Hmin, Hmax, Hs ; 
	OrbitDesignStage5 (a0, es, i0, ws, W0, u0, &dL, &dLs, &Hmin, &Hmax, &Hs) ; 
	// ������� �� ������ � ����������� ��������� �������
	OrbitDesignStage6 (a0, es, i0, ws, W0, u0) ;
	printf("\n") ;

	// ���������������� ���������� ������
	mkdir("_����������") ;
	mkdir("_����������\\�������������� �����") ;
	FILE* fp = fopen( "_����������\\�������������� �����\\_OrbitDesignTabl.txt", "a+" ) ;
	fprintf(fp, "\n %5d	%5d	%.3lf	%.5lf	%.3lf	%.7lf	%.3lf	%.7lf	%.7lf	%.3lf	%.3lf	%.3lf	%.3lf	%.3lf", 
		    K, L, Tdr0, i0, a0, es, ws, e1s, e2s, Hmin, Hmax, Hs, dL, dLs) ;
	fclose(fp) ; fp = NULL ; 
	//
	return 0 ;
}

//-------------------------------------------------------------------------------
// ������������� ������ � ������ ������ ��������� ���������
double CalcTdrC20(double a, double e1, double e2, double i)
{
	double si = Sin(i, 1) ;
	double ski= si*si ;
	double p  = a*(1-e1*e1-e2*e2) ;
	double T  = a*sqrt(a/mu)-eps/mu/sqrt(p*mu)*(3.0-2.5*ski-e1*(1.0-5.0*ski)) ;
	T*= k_pi*2.0 ;
	return T ;
}

//-------------------------------------------------------------------------------
// ���������� ��� � ������ �����������
double CalcISSO(double p, double Tdr)
{
	double i = 0 ;
	// ������������� ������ � ���
	Tdr/= 86400.0 ;
	// ������� ���������� ��� � ������ ������ ������ ��������� ���������
	i =-Tdr*p*p/365.2422*mu/eps ;
	// ���������� ��� � ��������
	i = Acos(i, 1) ;
	return i ;
}

//-------------------------------------------------------------------------------
// ���������� ��������� ������� ��������������� � ������ 7-� ��������� ��������
// ��� �������� ���������� �����������-�������� ������
// �� ��������� ���������� � ������� �������
//		i0		- �������� ���������� 
//		a		- �������� �������
//		e1, e2	- ���������� ���������� ������� ���������������
//		psi		- ���� ��������� �� �������, �� sin(i0)
void CalcE12SSOfromI(double i0, double a, double& e1, double& e2, double* psi)
{
	double si = Sin(i0, 1) ;
	double si2= si*si ;
	// ������������ "�����"
	double gam2, gam3, gam5, gam7 ;
	CalcGam2357(a, gam2, gam3, gam5, gam7) ;
	// ����� �������� ��������� ������� ���������������
	e1 = gam2*(1.5-si*si); 
	e2 = gam3 - 5.0/4.0*gam5*(8.0-28.0*si2+21.0*si2*si2)/
		 (4.0-5.0*si2) + 35.0/128.0*gam7*
		 (64.0-432.0*si2+792.0*si2*si2-429.0*si2*si2*si2)/(4.0-5.0*si2) ;
	e2*= si/(2.0*gam2) ;
	if (psi) *psi = si ;
}

//-------------------------------------------------------------------------------
// ���������� ������������� ������ ������������ ����� �� ���������
// ���������� � �������������� �������
// 
// ����������� ������������ ��������� �� ������ �����
// ������ �� ������ ������ �����: 
//		������� �����			t0, 
//		������������� ������	Tdr0, 
//		����������				i0, 
//		���������� ������� ��	W0,
//		������� �������			a0 - ��� ������� �����������.
// ��������� ������� �������� ������������� ���������� ������: 
//		������� �������			a, 
//		��������� ������		e, w.
// � ���� ������� �������������� ��� ��� �������� ������ ������ ������������ 
// �������� �������� ������� �����. ��������� ������ ����������� ��  
// ������������� ������������, � ������� ������� ���������� �������� ������ 
// �� �������� �������������� �������, ���������� ������� � ������� ������ �����.
int OrbitDesignOSHfromTi(ZLSF* LSF, double Tdr0, double i0, double W0, double& a0, double& e, double& w) 
{
	int rc ;
	double Tdr, e1=0, e2=0, e1r=999, e2r=999 ;
	MATC20(i0, e1, e2, Tdr0, a0);
	CalcE12SSOfromI(i0, a0, e1, e2, NULL) ;
	double a = a0 ;

	while(1) {
		CalcE12SSOfromI(i0, a, e1, e2, NULL) ;
		e = sqrt(e1*e1+e2*e2) ;
		w = Atan2(e2, e1, 1) ;
		// ������ ������� � ������ ���� ���
		rc = CalcPeriodInFullForce(LSF, a, e, i0,  W0, w, 0, Tdr) ; RRC ; 
		// ������� �� �������
		double dT = Tdr0 - Tdr ; 
		// �������� � �������� ������� �������, 
		// �������������� �������� ������� �� ������� dT
		double da = dT/3.0/k_pi*sqrt(k_mu_kms/a) ;
		// ����� �������� ������� �������
		a += da ;
		a0 = a ;
		// �������� ������� ���������� ������������� ��������
		double de = sqrt((e1r-e1)*(e1r-e1)+(e2r-e2)*(e2r-e2)) ;
		if (fabs(dT)<1e-4 && fabs(de)<1e-8) break ;
		e1r = e1 ;
		e2r = e2 ;
	}

	return 0 ;
}


//-------------------------------------------------------------------------------
// ���������� ������������ ��������� �� ������ �����
// ������ ������� ������
// ��������� ������� �������� ��������-���������� ������������� ���������� ������
int OrbitDesignSSOStage1h(double h0, double& a0, double& e0, double& w0, double& i0) 
{
	// ����� ��������
	int j = 0 ;
	// ������� �������
	double aj = Rz+h0 ;
	a0 = aj ;
	// ������� ������������� ����������
	double ij ; 
	// ������������� ������
	double Tdrj = 2*k_pi*aj*sqrt(aj/k_mu_kms) ;
	// ���������� ������� ��������������� e*cos(w), e*sin(w)   
	double e1j  = 0.0 ;
	double e2j  = 0.0 ;
	// ��������� �������� ������ 
	double pj   = aj*(1-e1j*e1j-e2j*e2j) ;
	while(1) {
		// ���������� ��������� ���
		ij = Tdrj/86400.0/365.2422*pj*pj*k_mu_kms/k_eps_kms ;
		ij = Acos(ij, 1) ;
		// ���������� ��������� ������� ��������������� � ������ 7-�
		// ��������� ��������, �� �������� ������� � ����������
		double e1j, e2j, si ;
		CalcE12SSOfromI(ij, aj, e1j, e2j, &si) ;
		// ����� �������� �������
		aj  = pj/(1.0-e1j*e1j-e2j*e2j) ;  
		// ����� �������� �������
		Tdrj= 2*k_pi*(aj*sqrt(aj/k_mu_kms) - k_eps_kms/k_mu_kms/sqrt(pj*k_mu_kms)* 
			  (3.0-2.5*si*si-e1j*(1.0-5.0*si*si)));
		// �������� ������� ����������
		if (fabs(aj-a0)<1e-5) break ;
		// ����� �������� �������
		a0 = aj ;
		// ���������� �������� ��������
		j++ ;
	}
	// ���������� �������
	a0 = aj ;
	e0 = sqrt(e1j*e1j+e2j*e2j) ;
	w0 = Atan2(e2j, e1j, 1) ;
	i0 = ij ;
	return 0 ;
}

//-------------------------------------------------------------------------------
// ���������� ������������ ��������� �� ������ �����
// ����� ������������� ������
// ��������� ������� �������� ��������-���������� ������������� ���������� ������
int OrbitDesignSSOStage1T(double Tdr0, double& a0, double& e0, double& w0, double& i0) 
{
	// ����� ��������
	int j = 0 ;
	// ������� ������������� ������
	double Tdrj = Tdr0 ;
	// ������� ������������� ����������
	double ij ; 
	// ������� �������
	double aj = pow(Tdrj*Tdrj*k_mu_kms/4.0/k_pi/k_pi, 1.0/3.0) ;
	// ���������� ������� ��������������� e*cos(w), e*sin(w)   
	double e1j = 0.0 ;
	double e2j = 0.0 ;
	a0 = aj ;
	while(1) {
		// ��������� �������� ������ 
		double pj = aj*(1-e1j*e1j-e2j*e2j) ;
		// ���������� ��������� ���
		ij =-Tdrj/86400.0/365.2422*aj*aj*k_mu_kms/k_eps_kms ;
		ij = Acos(ij, 1) ;
		// ���������� ��������� ������� ��������������� � ������ 7-�
		// ��������� ��������, �� �������� ������� � ����������
		double e1j, e2j, si ;
		CalcE12SSOfromI(ij, aj, e1j, e2j, &si) ;
		// ����� �������� �������
		Tdrj= 2*k_pi*(aj*sqrt(aj/k_mu_kms) - k_eps_kms/k_mu_kms/sqrt(pj*k_mu_kms)* 
			  (3.0-2.5*si*si-e1j*(1.0-5.0*si*si)));
		// �������� ������� ����������
		if (fabs(Tdrj-Tdr0)<1e-4) break ;
		// ����� �������� �������
		aj+= (Tdr0-Tdrj)/3.0/k_pi*sqrt(k_mu_kms/aj) ; ;
		// ���������� �������� ��������
		j++ ;
	}
	// ���������� �������
	a0 = aj ;
	e0 = sqrt(e1j*e1j+e2j*e2j) ;
	w0 = Atan2(e2j, e1j, 1) ;
	i0 = ij ;
	return 0 ;
}

//-------------------------------------------------------------------------------
// ���������� ������������ ��������� �� ������ �����
// ������ k-�������� ��������� � � ����������
// ������ ����� l ������ ������.
// ��������� ������� �������� ��������-���������� ������������� ���������� ������
int OrbitDesignSSOStage1kl(double k, double l, double& Tdr0, double& a0, double& e0, double& w0, double& i0) 
{
	Tdr0 = 86400.0*(double)k/(double)l ; 
	return(OrbitDesignSSOStage1T(Tdr0, a0, e0, w0, i0)) ; 
}

//-------------------------------------------------------------------------------
// ���������� ������������ ��������� �� ������ ����� �� ��������� ����������
// ��������� ������� - �������� ��������-���������� ������������� ���������� ������
int OrbitDesignSSOStage12I(bool isStage2, double i0, double& a0, double& e0, double& w0, double& Tdr0) 
{
	// ��� ��� ������ �������� ���������� 
	if (i0<90) i0 = 180.0-i0 ;
	//
	double ci = Cos(i0, 1) ;
	double aj  ;

	if (isStage2) {
		aj = a0 ;
	} else {
		aj = 365.2422*eps*fabs(ci)/2.0/k_dwapi/sqrt(mu)*86400.0 ;
		aj = pow(aj, 2.0/7.0) ;
	}

	while(1) {
		// ���������� ��������� ������� ��������������� � ������ 7-�
		// ��������� ��������, �� �������� ������� � ����������
		double e1j, e2j ;
		CalcE12SSOfromI(i0, aj, e1j, e2j, NULL) ;
		// ��������� �������� �� ������� � ���������������
		double pj = aj*(1-e1j*e1j-e2j*e2j) ;
		// ������������� ������ �� �������� ������� � ���������� ����
		double Tj ;
		if (isStage2) {
			double ej = sqrt(e1j*e1j+e2j*e2j) ;
			double wj = Atan2(e2j, e1j, 1) ;
			int rc = CalcPeriodInFullForce(&LSF, aj, ej, i0, 0, wj, 0, Tj) ; RRC ; 
		} else {
			Tj = CalcTdrC20(aj, e1j, e2j, i0) ;
		}
		// ������� ���������� �� �������������� �������
		double cij=-Tj/365.2422/86440.0*pj*pj*mu/eps ;
		// ���������� �������� ��������� ���������� 
		// �� �������� ������������ ����������
		double dci= ci-cij ;
		// ����� �������� �������
		aj-= dci*365.2422*86400.0*eps/7.0/k_pi/aj/aj/sqrt(aj*mu) ;
		// ����� �������� ����������� ����������
		double ij = Acos(cij, 1) ;
		// �������� ������� ���������� �����
		// ��������� ���������� ij ����� ���� ����� ��������� i0
		if (fabs(ij-i0) < 0.00001) {
			Tdr0 = Tj ;
			a0 = aj ;
			e0 = sqrt(e1j*e1j+e2j*e2j) ;
			w0 = Atan2(e2j, e1j, 1) ;
			break ; 
	}	}
	return 0 ;
}

//-------------------------------------------------------------------------------
// ���������� �������������� ������� � ������ ���� ���
int CalcPeriodInFullForce(ZLSF* LSF, double a0, double e0, double i0, double W0, double w0, double u0, double& Tdr) 
{
	int    rc ;

	// ��������� ������� ��������
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a0) ;
	NU.X[1] = e0 ;
	NU.X[2] = TO_RAD(i0) ;
	NU.X[3] = TO_RAD(W0) ;
	NU.X[4] = TO_RAD(w0) ;
	NU.X[5] = TO_RAD(u0) ;
	NU.Sb   = 0.0005 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

	// �������������� ������ �������������� �������   
	// ����� ��������������� �������� ��� � �������� ������ ��������.
	// ������ �������� ��
	ZMSC KA ;
	// ������������� ������ ��������
	rc = KA.Init(NU, LSF) ; RRC ;
	// ������������� �����
	rc = KA.GoAlongVit(NU.vit) ; RRC ;
	// �������� �������������� ������� �� ��������������� �����
	// ������� ������������� ������
	Tdr = KA.FV.Td ; 
	return 0 ;
}

//-------------------------------------------------------------------------------
// ��������� ������� ������� ��� ��������� �������
int OrbitDesignSSOStage2T(ZLSF* LSF, double Tdr0, double a0, double e0, double i0, double W0, double w0, double u0, double& ak) 
{
	while (1) {
		double Tdr ;
		// ������ ������� � ������ ���� ���
		int rc = CalcPeriodInFullForce(LSF, a0, e0, i0,  W0, w0, u0, Tdr) ; RRC ; 
		// ������� �� �������
		double dT = Tdr0 - Tdr ; 
		// �������� � �������� ������� �������, 
		// �������������� �������� ������� �� ������� dT
		double da = dT/3.0/k_pi*sqrt(k_mu_kms/a0) ;
		// ����� �������� ������� �������
		a0+= da ;
		// �������� ������� ���������� ������������� ��������
		if (fabs(dT) <1e-4) break ;
	}
	ak = a0 ;	
	printf("Period = %.6lf  major semi-axis = %.6lf  ", Tdr0, ak) ;
	return 0 ;
}

//-------------------------------------------------------------------------------
// ��������� ���������� ���
//-------------------------------------------------------------------------------
double KW(double ps, double i)
{
	double K = 0 ;
	double ps2 = ps*ps ;
	double ps4 = ps2*ps2 ;
	double si  = Sin(i, 1) ;
	double ski = si*si ;
	K = C20+(3.0-20.0*ski)/4.0/ps2*C20*C20+
		35.0*(7.0*ski-4.0)/56.0/ps2*C40+
		35.0*(8.0-36.0*ski+33.0*ski*ski)/64.0/ps4*C60 ;
	K*= 3.0*k_pi*Cos(i, 1)/ps2 ;
	return K ;
}

double HW(double ps, double i)
{
	double H = 0 ;
	double si  = Sin(i, 1) ;
	double ski = si*si ;
	double ci  = Cos(i, 1) ;
	H = (15.0*ski-4.0)*C30+
		(8.0-84.0*ski+105*ski*ski)*5.0/4.0/ps/ps*C50 ;
	H*= 3.0*k_pi/4.0/ps/ps/ps*ci/si ;
	return H ;
}

double LW(double ps, double i)
{
	double L = 0 ;
	double si  = Sin(i, 1) ;
	double ski = si*si ;
	double ci  = Cos(i, 1) ;
	L = 6.0*k_pi*(2.0-5.0*ski)*ci*C20*C20/ps/ps/ps/ps ; 
	return L ;
}

double FW(double a, double e1, double e2, double i, double Tdr)
{
	double ps = a/(1-e1*e1-e2*e2)/Re ;
	double K = KW(ps, i) ;
	double H = HW(ps, i) ;
	double L = LW(ps, i) ;
	double F ;
	// ������� �������� ��������� ����������� ���� ������ �� ����� ������ 
	// (���/�����) � ������ 6-�� ��������� �������� ��� ��� �������� 
	// ��������� ��������� ������
	double dW = K+L*e2+H*e1 ; 
	// ������������������� � ���
	Tdr/= 86400.0 ;
	// ���������� �������� ��������� ��������� ������ �� 2-� "��" � ��������
	// (�� ������ ������� ������ ������������ ����)
	F = dW*365.2422/Tdr-2*k_pi ;
	return F ;
}

//-------------------------------------------------------------------------------
// ��������� ���������� ��� ������ �� ������� ������������ � ���� �� 7-�
// ��������� ��������
int OrbitDesignStage3(double a0, double e0, double w0, double i0, double Tdr0, double& ik) 
{
	double e10 = e0*Cos(w0, 1) ;
	double e20 = e0*Sin(w0, 1) ;
	double p   = a0/(1-e10*e10-e20*e20) ;
	double F ;
	double si ; 
	double ij = i0 ;
	while (1) {
		// ���������� �������� ��������� ��������� ������ �� 2-� "��" � ��������
		F = FW(a0, e10, e20, ij, Tdr0) ;
		si = Sin(ij, 1) ;
		// ����� �������� ���������� � ��������
		ij = RadA(ij) - F*mu*p*p*Tdr0/86400.0/2.0/k_pi/eps/365.2422/si ;
		// ����� �������� ���������� � ��������
		ij = GradA(ij) ;
		// �������� ������� ���������� ��������
		if (fabs(F)<1e-6) break ;
	}
	ik = ij ;

	printf("Inclination = %.9lf\n", ik) ;

	return 0 ;
}

//-------------------------------------------------------------------------------
// ���������� ������� ���������������
int OrbitDesignStage4(double a0, double e0, double i0, double w0, double W0, double u0, double& es, double& ws, double& e1s, double& e2s)
{
	printf("Averaging of the orbital parametrs at the turns beginning...\n") ;
	int rc = 0 ; 

	// ��������� ������� ��������
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a0) ;
	NU.X[1] = e0 ;
	NU.X[2] = TO_RAD(i0) ;
	NU.X[3] = TO_RAD(W0) ;
	NU.X[4] = TO_RAD(w0) ;
	NU.X[5] = TO_RAD(u0) ;
	NU.Sb   = 0.0005 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

	// �������������� ������ �������������� �������   
	// ����� ��������������� �������� ��� � �������� ������ ��������.
	// ������ �������� ��
	ZMSC KA ;
	// ������������� ������ ��������
	rc = KA.Init(NU, &LSF) ; RRC ;

	// ��������� ����������������
	ZCPrintManager PrintMng ;
	// ��������� ������ � ����� ����� ���������
	PrintMng.SetPrintFileSticker(FilePrintSticker) ;
	// ��������� ���������������� �������� �� ������
	PrintMng.MPK.FromVitOff() ;
	// ���������������� ����������� �������� �� ������� (�������������)
	PrintMng.MPK.AflatOn() ;
	// �������������� ����������������� ����������������� ����������� ��������.
	// ����������: ��� ���������, ��� �������� (����� ������ ����� �������),
	// ������������ ����������� �������� ��� ������������� � ��������.
	// �������� ��������������� ���������� �������� ����� ��� ������
	// ������� PMPK.UPP(0).value = ...
	// PrintMng.MPK << UserPrintParam(UPPdouble, 16, 6, "�����", "����") ;
	// PrintMng.MPK.UPP(0).value = 0 ;
	// ���������� ��������������� ���������� ��������
	PrintMng.MPK << MODPRDATETIME ;
	PrintMng.MPK << MODPR_VIT ;
	PrintMng.MPK << MODPR_MSSV ;
	PrintMng.MPK << MODPR_T_iTE ;
	PrintMng.MPK << MODPR_T_WTE ;
	PrintMng.MPK << MODPR_T_wTE ;
	PrintMng.MPK << MODPR_T_uTE ;
	PrintMng.MPK << MODPR_H ;
	PrintMng.MPK << MODPR_Aosk ;
	PrintMng.MPK << MODPR_eosk ;
	PrintMng.MPK << MODPR_exosk ;
	PrintMng.MPK << MODPR_eyosk ;

	// ������������� ����������������
	PrintMng.PrePrint(&KA, "�������������� �����", "_OrbitDesign_") ;
	PrintMng.PrintHeaderDoc("\n   ������� ���������� �������� �� (TaskOrbitDesign)\n\n") ;
	// ���������������� ��
	PrintMng.PrintNU(NU, NULL, 3) ;
	// ���������������� ���������� ����� ���
	PrintMng.PrintLSF(LSF, 1) ;

	// ���������������� ���������� ��������
	PrintMng.PrintMSC(&KA, NULL, NULL, NULL);

	int dVit = 100 ;
	int Vitn = NU.vit+1 ;
	int Vit  = Vitn ;
	int Vitk = Vitn + dVit ;
	// ���� ������������� 
	ZSC_SI SI ;
	double e1, e2, e1max=-999, e1min=999, e2max=-999, e2min=999 ;
	while(1) {
		printf("\rCompleted %.2lf%s    ", (double)(Vit-Vitn)/(double)dVit*100.0, "%") ;
		// ������� �� ������ �������� �����
		rc = KA.GoToVit(Vit, &SI); if (rc) break;
		e1 = SI.OSKTE[1]*cos(SI.OSKTE[4]) ;
		e2 = SI.OSKTE[1]*sin(SI.OSKTE[4]) ;
		if (e1>e1max) e1max = e1 ;
		if (e1<e1min) e1min = e1 ;
		if (e2>e2max) e2max = e2 ;
		if (e2<e2min) e2min = e2 ;
		// ���������������� ���������� ���������� ��������
		PrintMng.PrintMSC(&KA, &SI, NULL, NULL);
		// �������� ������� ���������� ���������� �������������� (��������)
		if (Vit==Vitk) break ;
		Vit++ ;
	}
	printf("\n") ;

	if (!rc) {
		e1s = (e1max+e1min)/2.0 ;
		e2s = (e2max+e2min)/2.0 ;
		es = sqrt(e1s*e1s+e2s*e2s) ;
		ws = Atan2(e2s, e1s, 1) ;
	}

	// ���������� ����������������
	PrintMng.PostPrint() ; 
	PrintMng.ClosePrint();
	return rc ;
}

//-------------------------------------------------------------------------------
// ��������� �� ��� ���������� ������ ������� ������� ���������������� ���������
ZMSC_EXV_BEG(ZModHAvr)
	// ������ ����� ������� ���������������� ���������
	double GSK[6] ;
	int rc = ZASC1975toGSC(t, Xmain, GSK, 0) ;	RRC ;
	double HBL[3] ;
	ZGRtoHBL(GSK, HBL, 0, 0) ;
	Yexe[0] = 1e4*HBL[0] ;
ZMSC_END

//-------------------------------------------------------------------------------
// ������� �������
int OrbitDesignStage5(double a0, double e0, double i0, double w0, double W0, double u0, double* dL, double* dLs, double* Hmin, double* Hmax, double* Hs)
{
	printf("Motion prediction...\n") ;
	int rc = 0 ; 

	// ��������� ������� ��������
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a0) ;
	NU.X[1] = e0 ;
	NU.X[2] = TO_RAD(i0) ;
	NU.X[3] = TO_RAD(W0) ;
	NU.X[4] = TO_RAD(w0) ;
	NU.X[5] = TO_RAD(u0) ;
	NU.Sb   = 0.0005 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

	// �������������� ������ �������������� �������   
	// ����� ��������������� �������� ��� � �������� ������ ��������.
	// ������ �������� ��
	ZMSC KA ;
	// ������������� ������ ��������
	rc = KA.Init(NU, &LSF) ; RRC ;

	// ���� ��������� ������ ��� ��� � ������� ������, 
	// �� ���������� ����� � ������ ������ �������� ���������
	// ��� ������� ���������� �� ����� ������ (�������������������
	// ��������).
	if (Hmin || Hmax || Hs || dLs || dL) {
		ZModHAvr KA ;
		// ����� ����� ������� ���������
		KA.SetExternalEquationsCount(1) ;
		// �������������� ������
		rc = KA.Init(NU, &LSF) ; RRC ;
		// �������� �������������� �������� ���������,
		// ��� ���� �������� ������� �� ��� ������� ���������
 		KA.ExternalEquationsOn(NULL) ;
		// ������������� ���������� ����� ����� ����� NU.vit
		KA.GoAlongVit(NU.vit) ;
		// �������� ������������������� �������� ���������
		double HAvr ;
		KA.ExternalEquationsOff(&HAvr) ;
		// ����� �� ������������� ������ ����� �������� 
		// ������������������ �������� ���������
		HAvr/= (KA.FV.Td/k_cbc) ;
		// �������� �������� � ������� ������������ ���������� �����
		KA.FV.HAvr = HAvr ;

		// ����������� ����� � ��������
		if (dL ) *dL = KA.FV.dL ;	
		// �������� �������� ����� � ��������
		if (dLs) *dLs= KA.FV.dLs ;	

		// ����� �� ��������� ��������� ��������� ����������� ������ �� �����
		ZSC_VP* HM1 = HMIN_VI(KA.FV) ;
		// ����� �� ��������� ���������� ��������� ������������ ������ �� �����
		ZSC_VP* HM2 = HMAX_VI(KA.FV) ;
		if (Hmin) *Hmin = HM1->H ; 
		if (Hmax) *Hmax = HM2->H ; 
		if (Hs  ) *Hs   = KA.FV.HAvr ; 
	}

	// ��������� ����������������
	ZCPrintManager PrintMng ;
	// ��������� ������ � ����� ����� ���������
	PrintMng.SetPrintFileSticker(FilePrintSticker) ;
	// ��������� ���������������� �������� �� ������
	PrintMng.MPK.FromVitOff() ;
	// ���������������� ����������� �������� �� ������� (�������������)
	PrintMng.MPK.AflatOn() ;
	// �������������� ����������������� ����������������� ����������� ��������.
	// ����������: ��� ���������, ��� �������� (����� ������ ����� �������),
	// ������������ ����������� �������� ��� ������������� � ��������.
	// �������� ��������������� ���������� �������� ����� ��� ������
	// ������� PMPK.UPP(0).value = ...
	PrintMng.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "�����", "����") ;
	PrintMng.MPK.UPP(0).value = 0 ;
	// ���������� ��������������� ���������� ��������
	PrintMng.MPK << MODPRDATETIME ;
	PrintMng.MPK << MODPR_MSSV ;
	PrintMng.MPK << MODPR_T_iTE ;
	PrintMng.MPK << MODPR_T_WTE ;
	PrintMng.MPK << MODPR_T_wTE ;
	PrintMng.MPK << MODPR_T_uTE ;
	PrintMng.MPK << MODPR_H ;
	PrintMng.MPK << MODPR_Aosk ;
	PrintMng.MPK << MODPR_eosk ;
	PrintMng.MPK << MODPR_exosk ;
	PrintMng.MPK << MODPR_eyosk ;

	// ������������� ����������������
	PrintMng.PrePrint(&KA, "�������������� �����", "_OrbitDesign") ;
	PrintMng.PrintHeaderDoc("\n   ������� ���������� �������� �� (TaskOrbitDesign)\n\n") ;
	// ���������������� ��
	PrintMng.PrintNU(NU, NULL, 3) ;
	// ���������������� ���������� ����� ���
	PrintMng.PrintLSF(LSF, 1) ;

	// ���������������� ���������� ��������
	PrintMng.PrintMSC(&KA, NULL, NULL, NULL);
	// �������� ��������������� (7 �����)
	double dt= 3 ;
	// ������� �����
	double t = KA.GetTime() ;
	// ��������� �����
	double tn = t ;
	// �������� ����� �����
	double tk= t+dt ;
	int   dir= tk>=t ? 1:-1 ;
	// ���� ������������� 
	ZSC_SI SI ;
	while(1) {
		printf("\rCompleted %.2lf%s    ", (t-tn)/dt*100.0, "%") ;
		// �������� ������� ���������� ���������� �������������� (��������)
		if (t>=tk) break ;
		// ���� ��� ����������� � �������������� ������� ��� ��������, 
		// �� �� ����� ��� ������ ������� tk. �� ���� h <= tk-t, ���
		// t ���� ������� �����
		rc = KA.GoStepA(&SI, dir, &tk) ; if (rc) break ;
		// ����� ������� ����� 
		t = SI.ta ;
		// ���������������� ����������������� ����������� ��������.
		// ������� �������� ��������������� ���������,
		// �������������� ����� ��� ������ ������ ��������� 
		// PrintMng.MPK << UserPrintParam(UPPdouble...
		PrintMng.MPK.UPP(0).value = (t-tn)*24.0;
		// ���������������� ���������� ���������� ��������
		PrintMng.PrintMSC(&KA, &SI, NULL, NULL);
	}
	printf("\n") ;
	// ���������� ����������������
	PrintMng.PostPrint() ; 
	PrintMng.ClosePrint();
	return 0 ;
}

//-------------------------------------------------------------------------------
// ������� ��������� �������
int OrbitDesignStage6(double a0, double e0, double i0, double w0, double W0, double u0)
{
	printf("Height frofile Model forcing...\n") ;
	int rc = 0 ; 

	// ��������� ������� ��������
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a0) ;
	NU.X[1] = e0 ;
	NU.X[2] = TO_RAD(i0) ;
	NU.X[3] = TO_RAD(W0) ;
	NU.X[4] = TO_RAD(w0) ;
	NU.X[5] = TO_RAD(u0) ;
	NU.Sb   = 0.001 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

	// �������������� ������ �������������� �������   
	// ����� ��������������� �������� ��� � �������� ������ ��������.
	// ������ �������� ��
	ZMSC KA ;
	// ������������� ������ ��������
	rc = KA.Init(NU, &LSF) ; RRC ;

	int CountStep = 180 ; 

	// ��������� ����������������
	ZCPrintManager PrintMng ;
	// ��������� ������ � ����� ����� ���������
	PrintMng.SetPrintFileSticker(FilePrintSticker) ;
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
	// PrintMng.MPK << UserPrintParam(UPPdouble, 16, 6, "�����", "����") ;
	// PrintMng.MPK.UPP(0).value = 0 ;
	for (int i=0 ; i<CountStep+1 ; i++) {  
		PrintMng.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "h", "��") ;
	}

	// ������������� ����������������
	PrintMng.PrePrint(&KA, "�������������� �����", "_OrbitDesignHProf") ;
	PrintMng.PrintHeaderDoc("\n   ������� ���������� �������� �� (TaskOrbitDesign)\n\n") ;
	// ���������������� ��
	PrintMng.PrintNU(NU, NULL, 3) ;
	// ���������������� ���������� ����� ���
	PrintMng.PrintLSF(LSF, 1) ;

	// ���� ������������� 
	ZSC_SI SI ;
	double U ;
	double du = 360.0/CountStep ; 
	int    dVit = 2500 ;
	int    Vitn = NU.vit+1 ;
	int    Vit  = Vitn ;
	int    Vitk = Vitn + dVit ;
	// ������� �� ���������� �����
	rc = KA.GoToVit(Vitn, &SI); RRC;
	// ���� ������������� �������� �� ������
	while(1) {
		printf("\rTurn %d from %d   ", Vit-Vitn+1, dVit) ;
		// ������ ��� ������ ����� ����� (������ �����)
		PrintMng.MPK.UPP(0).value = SI.THBLFR[1] ;
		U = du ;
		// ������������� �������� � �������� ����� � �������� ����� du
		// �� ��������� ������
		for (int i=1 ; i<CountStep ; i++) {  
			rc = KA.GoToVitU(Vit, U, &SI); if (rc) break;
			// ���������������� ����������������� ����������� ��������.
			// ������� �������� ��������������� ���������,
			// �������������� ����� ��� ������ ������ ��������� 
			// PrintMng.MPK << UserPrintParam(UPPdouble...
			PrintMng.MPK.UPP(i).value = SI.THBLFR[1] ;
			U += du ;
		}
		// ���� � ����� ���� ��������� ������ �� ������� ���� 
		// ����� ������������
		if (rc) break ;
		// ���������� ������ �����
		Vit++ ;
		// ������������� �� ������ ���������� (����� ��������) �����
		rc = KA.GoToVit(Vit, &SI); if (rc) break;
		PrintMng.MPK.UPP(CountStep).value = SI.THBLFR[1] ;
		// ���������������� ���������� ���������� ��������
		// ������ ��� ��������� ����� ����� (����� �����)
		PrintMng.PrintMSC(&KA, &SI, NULL, NULL);
		// �������� ������� ���������� ���������� �������������� (��������)
		if (Vit==Vitk) break ;
	}
	printf("\n") ;
	// ���������� ����������������
	PrintMng.PostPrint() ; 
	PrintMng.ClosePrint();
	return rc ;
}

//-------------------------------------------------------------------------------
// ������� ��������� ������� ����������
int OrbitDesignStage7(double a0, double e0, double i0, double w0, double W0, double u0)
{
	printf("Height frofile Model forcing...\n") ;
	int i, rc = 0 ; 
	int CountStep = 720 ; 

	//---------------------------------------------------------------------------
	// ��������� ������� ��������
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a0) ;
	NU.X[1] = e0 ;
	NU.X[2] = TO_RAD(i0) ;
	NU.X[3] = TO_RAD(W0) ;
	NU.X[4] = TO_RAD(w0) ;
	NU.X[5] = TO_RAD(u0) ;
	NU.Sb   = 0.001 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

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
	// PrintMng.MPK << UserPrintParam(UPPdouble, 16, 6, "�����", "����") ;
	// PrintMng.MPK.UPP(0).value = 0 ;
	for (i=0 ; i<=CountStep ; i++) {  
		PrintMng.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "h", "��");
	}

	// ������������� ����������������
	PrintMng.PrePrint(&KA, "�������������� �����", "_OrbitDesignHProf") ;
	PrintMng.PrintHeaderDoc("\n   �������� ������� (TaskOrbitDesign)\n\n") ;
	// ���������������� ��
	PrintMng.PrintNU(NU, NULL, 3) ;
	// ���������������� ���������� ����� ���
	PrintMng.PrintLSF(LSF, 1) ;

	//---------------------------------------------------------------------------
	// ��������� ���������������� �������� ����������� � ������������ �����
	ZCPrintManager PrintMngH ;
	PrintMngH.MPK.FromVitOff() ;
	PrintMngH.MPK.AflatOn() ;
	PrintMngH.MPK << ZSUserPrintParam(UPPint, 16, 6, "�����", "");
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmin1", "��");
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmin2", "��");
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmax1", "��");
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmax2", "��");
	PrintMngH.PrePrint(&KA, "�������������� �����", "_OrbitDesignHminmax") ;
	PrintMngH.PrintHeaderDoc("\n   ����������� �����������/������������ ����� �� ������� ����� (TaskOrbitDesign)\n\n") ;
	// ���������������� ��
	PrintMngH.PrintNU(NU, NULL, 3) ;
	// ���������������� ���������� ����� ���
	PrintMngH.PrintLSF(LSF, 1) ;

	//---------------------------------------------------------------------------
	// ���� ������������� 
	ZSC_SI SI ;
	double U ;
	double du = 360.0/CountStep ; 
	// �������� ������������� � ������
	double maxTime = 366 ; 
	// ���������� ������ ����� ������� �� ������� ��������������� 
	// ������������� ������
	int    SkipVit = 15 ;
	// ������ ����� ������������� ����� ����� ��������� 
	// �� ������ ��������� �������
	int    Vit = NU.vit+1 ;
	// ���������������� �������� ��������� ������ ��� ������� �����
	U = 0 ;
	for(i=0 ; i<=CountStep ; i++) {
		PrintMng.MPK.UPP(i).value = U ;
		U+= du ;
	}
	PrintMng.PrintMSC(&KA, &SI, NULL, NULL);

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
				// PrintMng.MPK << UserPrintParam(UPPdouble...
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
		PrintMng.PrintMSC(&KA, &SI, NULL, NULL);

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
		PrintMngH.PrintMSC(&KA, &SI, NULL, NULL);

		//-----------------------------------------------------------------------
		// �������� ������� ���������� ���������� �������������� (��������)
		if (SI.ta > NU.t+maxTime) break ;

		//-----------------------------------------------------------------------
		// ���������� ������ �����
		Vit++ ;
		// ��������� 15 ������
		Vit+= SkipVit ;
	}
	printf("\n") ;

	//---------------------------------------------------------------------------
	// ���������� ����������������
	PrintMng.PostPrint(); 
	PrintMng.ClosePrint();
	PrintMngH.PostPrint(); 
	PrintMngH.ClosePrint();

	return rc ;
}

//-------------------------------------------------------------------------------
