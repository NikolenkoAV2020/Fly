//-------------------------------------------------------------------------------
// MagicIZM.cpp: 
// �. ��������� 23.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>		// �������������� �������
#include <FlyCore.h>		// �������������� �������
#include <FlyMagicLNS.h>	//

//------------------------------------------------------------------------------
// ������ ��������� ��������� ������ �� ���������������� ��������
int MagicIZM(ZSMagicLnsSolution* MS, double a0, double e0, double i0, double w0, 
			 int dN, double u1, double V1, ZSLnsNpmDiffDV& DP)
{
    ZSLnsNpmDiffDV D ;
    double up  = MS->MZ.PARu() ;
	double pdu = MS->DU.i[MS->DU.mu].p;
	double pudu= MS->DU.i[MS->DU.mu].pu;
    double g =9.806e-3;
    double minv = 0.5e-3 ;                 // 0.5 �/���
    double dtu, uj, v, mn ;
    double aj = a0, ej =e0, i = i0, wj = w0 ;
    int    rc = 0, j, n1 ;
    memset( &DP, 0, sizeof( ZSLnsNpmDiffDV ) ) ;
    if ( fabs( V1 ) <= minv ) { v = V1 ; n1 = 1 ; uj = u1 ; }
    else {
		// ��������  ������ ���
		dtu = MS->MZ.M()*pudu / pdu*(1. - exp(-fabs(V1) / pudu / g)) / a0*sqrt(k_mu_kms / a0);
		dtu= GradA( dtu ) ;
		uj = u1- 0.5 * dtu ;                  // �������� ������ ������ ������ ���
		n1 = (int)floor(fabs(V1)/minv )+1 ;   // ����� ������� ��������
		v  = V1/n1 ;                          // � ��� �����   ��������
	}
    for (j=0 ; j<=n1 ; j++) {
		rc = MagicCalcDifDV(aj, ej, i, wj, uj, up, dN, D) ; RRC ; // ��� �����������
		if (j==0 || j==n1)  mn = 0.5 ;    // ��������� � ��������� �����
		if (j!=0 && j< n1)  mn = 1.  ;    // ���������� �����
		DP.da += D.da  * mn * v ;
		DP.de1+= D.de1 * mn * v ;
		DP.de2+= D.de2 * mn * v ;
		DP.dhp+= D.dhp * mn * v ;
		DP.dha+= D.dha * mn * v ;
		DP.dT += D.dT  * mn * v ;
		DP.dL += D.dL  * mn * v ;
		DP.de += D.de  * mn * v ;
		DP.dw = ValiDA( DP.dw + D.dw * mn * v , 1 ) ;
		aj += v * D.da ;
		ej += v * D.de ;
		wj += v * D.dw ;
		if (n1>1) uj+= dtu/n1 ;
	}
    return 0 ;
}

//------------------------------------------------------------------------------