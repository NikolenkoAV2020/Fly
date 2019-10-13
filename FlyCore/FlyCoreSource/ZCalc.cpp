//---------------------------------------------------------------------------
// ZCalc.cpp
// �. ��������� 03.08.2018
// ��������� �������������� ��������� ������� ����������
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
//---------------------------------------------------------------------------
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ZSiderealTime.h>
#include <FlyCoreSource\\ZSK.h>
#include <FlyCoreSource\\ZCalc.h>

//------------------------------------------------------------------------------
// �������������� ������� �������� � ���� �����������
double AngleDimConv(double a, DIMANGL fromdim, DIMANGL todim)  
{
	double u = a ; if (fromdim==todim) return u ; 
	switch (fromdim) {
		case DimAnglRad:
			// ������� �� ��������
			switch (todim) {
				case DimAnglGrad: u = a*M_GRARAD ; break ;
				case DimAnglSSS:  u = a/k_dwapi ; break ;
				case DimAnglH:    u = a/k_dwapi*24.0 ; break ;
				case DimAnglMin:  u = a/k_dwapi*1440.0 ; break ;
				case DimAnglSec:  u = a/k_dwapi*k_cbc ; break ;
				default: assert(0) ;
			} break ;
		case DimAnglGrad:
			// ������� �� ��������
			switch (todim) {
			case DimAnglRad:  u = a / M_GRARAD; break;
				case DimAnglSSS:  u = a/360.0 ; break ;
				case DimAnglH:    u = a/360.0*24.0 ; break ;
				case DimAnglMin:  u = a/360.0*1440.0 ; break ;
				case DimAnglSec:  u = a/360.0*k_cbc ; break ;
				default: assert(0) ;
			} break ;
		case DimAnglSSS:
			// ������� �� ���
			switch (todim) {
				case DimAnglRad:  u = a*k_dwapi ; break ;
				case DimAnglGrad: u = a*360.0 ; break ;
				case DimAnglH:    u = a*24.0 ; break ;
				case DimAnglMin:  u = a*1440.0 ; break ;
				case DimAnglSec:  u = a*k_cbc ; break ;
				default: assert(0) ;
			} break ;
		case DimAnglH:
			// ������� �� �����
			switch (todim) {
				case DimAnglRad:  u = a/24.0*k_dwapi ; break ;
				case DimAnglGrad: u = a/24.0*360.0 ; break ;
				case DimAnglSSS:  u = a/24.0 ; break ;
				case DimAnglMin:  u = a/24.0*1440.0 ; break ;
				case DimAnglSec:  u = a/24.0*k_cbc ; break ;
				default: assert(0) ;
			} break ;
		case DimAnglMin:
			// ������� �� �����
			switch (todim) {
				case DimAnglRad:  u = a/1440.0*k_dwapi ; break ;
				case DimAnglGrad: u = a/1440.0*360.0 ; break ;
				case DimAnglSSS:  u = a/1440.0 ; break ;
				case DimAnglH:    u = a/1440.0*24.0 ; break ;
				case DimAnglSec:  u = a/1440.0*k_cbc ; break ;
				default: assert(0) ;
			} break ;
		case DimAnglSec:
			// ������� �� ������
			switch (todim) {
				case DimAnglRad:  u = a/k_cbc*k_dwapi ; break ;
				case DimAnglGrad: u = a/k_cbc*360.0 ; break ;
				case DimAnglSSS:  u = a/k_cbc ; break ;
				case DimAnglH:    u = a/k_cbc*24.0 ; break ;
				case DimAnglMin:  u = a/k_cbc*1440.0 ; break ;
				default: assert(0) ;
			} break ;
		default: assert(0) ;
	}
	return u ;
}

//------------------------------------------------------------------------------
// ���������� �������� �������� ���������� ������� tmss 
// �� ������� L �� �������� ta, ������������ �� J1975 
//		AtomicTime - ������� ����� �� ����� J1975
//		L          - ������������ ������� � ��������
//		tmss       - ������� ������� ��������� �����
//		dim        - ����������� tmis:
//		             0 - � ��������
//                   1 - � ��������
//                   2 - � ���
//                   3 - � �����
//------------------------------------------------------------------------------
int ZCalcMSSV(double AtomicTime, double L, double& tmss, DIMANGL dim)
{
	tmss = AtomicTime-0.125+ValiA(L, 1)/360.0 ;
	DATETIMEDMB DT ; DT << tmss ;
    DT.h=DT.m=DT.s=0 ; DT.d=0 ;
    double t0 ; t0 << DT ;
	// ���� � ��� 
    tmss = tmss-t0 ;
	// �������������� � ������ ����������� ���� ��� ���������
	// � �������  
	tmss = AngleDimConv(tmss, DimAnglSSS, dim) ;
    return 0 ;
}

//------------------------------------------------------------------------------
// ���������� �������� ��������� ���������� ������� tmss 
// �� ������� L �� �������� ta, ������������ �� J1975 
//		AtomicTime - ������� ����� �� ����� J1975
//		L          - ������������ ������� � ��������
//		tmis       - ������� �������� ��������� �����
//		dim        - ����������� tmis:
//		             0 - � ��������
//                   1 - � ��������
//                   2 - � ���
//                   3 - � �����
//------------------------------------------------------------------------------
int ZCalcMISV(double AtomicTime, double L, double& tmis, DIMANGL dim)
{
	// ���������� ��������� ������
	int rc ;
	rc = _CalcAstroKoord(AtomicTime, 0) ; RRC ;
	// ���������� ����������� � ��������� ������
	double Sun[3] ;	
	ZASCtoAscensionDecination(Wae->qso, Sun, 0) ;
	// ���������� �������� ������� 
	double SiderialTime ; 
	rc = ZTrueSiderealTime(AtomicTime, SiderialTime) ; if(rc) return rc ;
	// ���������� ��������� ���������� ������� � ��������
	tmis = SiderialTime + L / M_GRARAD - Sun[0] + k_pi;
	// ���������� � ��������� �� 0 �� pi
	tmis = ValiA(tmis, 0) ;
	// �������������� � ������ ����������� ���� ��� ���������
	// � �������  
	tmis = AngleDimConv(tmis, DimAnglRad, dim) ;
	return 0 ;
}

//---------------------------------------------------------------------------
//    ������ ������ ������� ��� ��� (��� �� ���� fromEK=true) 
//    � ����������� �� �������.
//---------------------------------------------------------------------------
//   ��������: void ZHightDif (double *w, double *hht);
//   ������� ���������:
//         w - ���������� � ��� (x,y,z,Vx,Vy,Vz).
//   �������� ���������:
//         hht[0] - ������ ��� ���,
//         hht[1] - ������,
//         hht[2] - ����������� ������ �� �������,
//         hht[3] - ����������� ������� �� �������.
//   ������������ ��������: ���.
//---------------------------------------------------------------------------
void ZCalcHightDif (double *x, double *hht, bool fromEK)
{
	double sin_fi ;
	double alfa = fromEK?k_alfak:k_alfa0 ; 
	double rekw = fromEK?k_ak:k_rekw ; 
	hht[1]=FR(x);
	sin_fi=x[2]/hht[1];
	hht[0]=hht[1]-rekw*(1-alfa*sin_fi*sin_fi);
	hht[3]=scal_pr(x,&x[3],3)/hht[1];
	hht[2]=hht[3]+2*rekw*alfa*(x[5]*hht[1]-x[2]*hht[3])*sin_fi/hht[1]/hht[1];
}

//---------------------------------------------------------------------------

