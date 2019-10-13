//-------------------------------------------------------------------------------
// MagicUT.cpp: 
// �. ��������� 23.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>		// �������������� �������
#include <FlyCore.h>		// �������������� �������
#include <FlyMagicLNS.h>	//

#include <FlyMagicLNSSource\\MagicLNS.h>
//------------------------------------------------------------------------------
// ������� ��������� 1-�� ����������������  ��������
int MagicUT1(ZSMagicLnsSolution* MS, int dN, int kp, double dtQ1, ZSLoopFact& P1, 
			 double u1, double *V1, double *PV1)
{
	// ��������� ����������� ������������ ���������� �� �������� ��������
	ZSLnsNpmDiffDV D ;
	// ��������� �������� ������ �� �������� ������ �������, ��������� ������
	// � ��������� ��������� ������� ������ 
	double up   = MS->MZ.PARu();
	// ���� ��
	double pdu  = MS->DU.i[MS->DU.mu].p;
	// �������� ���� ��
	double pudu = MS->DU.i[MS->DU.mu].pu;
    int    rc = 0, j, n1, nn1 ;
    double g = 9.806e-3;
    double minv = 0.000001 ;
    double Q1, dtu , uj ,v1j ;
    double aj, ej, i, wj ;

    aj = P1.a ; ej = P1.e ; i= P1.i ; wj = P1.w ;  
	// ���� ������� �� �����
    if (fabs(*V1)<(0.1*minv)) {         
		// ��� �����������
		rc = MagicCalcDifDV(aj, ej, i, wj, u1, up, dN, D) ; RRC ;            
		// ���������� ���.����.��������
		switch(kp) {
			case 1: *V1 = dtQ1/D.dT ; break ;
			case 2: *V1 = dtQ1/D.dL ; break ;
			case 3: *V1 = dtQ1/D.dw ; break ;
			case 4: *V1 = dtQ1/D.da ; break ; 
			default: assert(0) ;
		}
    }
    if (fabs(*V1)<minv) goto end ;

	// ��������  ������ ��� � ��������
	dtu = MS->MZ.M()*pudu / pdu*(1. - exp(-fabs(*V1) / pudu / g)) / aj*sqrt(k_mu_kms / aj);
    dtu = GradA(dtu) ;

    uj  = u1-0.5*dtu ;                         // �������� ������ ������ ������ ���
    n1  = nn1 = (int)floor(fabs(*V1)/minv)+1 ; // ����� ����� �������
    Q1  = dtQ1/nn1 ;                           // ��� ���� �������
    *V1 = 0 ;                                  // �������� ������� ����� �������
    for (j=0 ; j<=n1 ; j++) {
		// ��� �����������
		rc = MagicCalcDifDV(aj, ej, i, wj, uj, up, dN, D) ; RRC ; 
		// ���������� j-�� ������ ��������
		switch(kp) {
			case 1: v1j = Q1/D.dT ; break ;
			case 2: v1j = Q1/D.dL ; break ;
			case 3: v1j = Q1/D.dw ; break ;
			case 4: v1j = Q1/D.da ; break ; 
			default: assert(0) ;
		}
		aj+= v1j * D.da ;
		ej+= v1j * D.de ;
		wj+= v1j * D.dw ;                // ����� �� ��� ������� ����������
		if (wj<0) wj += 360. ;
		uj+= dtu / nn1 ;
		if (j==0 || j==n1) *V1+= v1j/2.; // ��������� � ��������� �����
		if (j!=0 && j< n1) *V1+= v1j ;   // ���������� �����
    }
end : ;
    PV1[0] = aj ; PV1[1] = ej ; PV1[2] = wj ;
    return rc ;
}

//------------------------------------------------------------------------------
// ������� �������� 2-� ��������������� ���������
int MagicUT2(ZSMagicLnsSolution* MS, int dN1, int dN2, int kp, int pru, 
			 double dtQ1, double dtQ2, double u1, double u2, 
			 ZSLoopFact& P1, ZSLoopFact& P2, double*V1, double*V2, double *PV1, 
			 double *PV2)
{
	double up  = MS->MZ.PARu();
	double pdu = MS->DU.i[MS->DU.mu].p;
	double pudu= MS->DU.i[MS->DU.mu].pu;
    ZSLnsNpmDiffDV D ;
    int    n1, nn1, n2, nn2 ;
    double g = 9.806e-3;
	// ����� ��������
    double minv = 0.001 ;                        
    double dtu [2], u1j, u2j, v1j, v2j ;
    double A1, B1 ;
    double A2, B2 ;
    double a1j = P1.a ;double e1j = P1.e ; double w1j = P1.w ; double i = P1.i ;
    double a2j = P2.a ;double e2j = P2.e ; double w2j = P2.w ;

	// ���� ���. �� ������
	if (*V1<=(.1*minv) || *V2<= (.1*minv)) {           
		//PV1[0] = a2j ; PV1[1] = e2j ; PV1[2] = w2j ;
		MagicCalcDifDV(a1j, e1j, i, w1j, u1, up, dN1, D) ;
		if (kp ==1) { A1 = D.dT ; B1 = D.dL ; }		// L,T
		if (kp ==2) { A1 = D.dL ; B1 = D.dw ; }		// L,w
		if (kp ==3) { A1 = D.dT ; B1 = D.dw ; }		// T,w
		MagicCalcDifDV(a2j, e2j, i, w2j, u2, up, dN2, D);
		if (kp ==1) { A2 = D.dT ; B2 = D.dL ; }		// L,T
		if (kp ==2) { A2 = D.dL ; B2 = D.dw ; }		// L,w
		if (kp ==3) { A2 = D.dT ; B2 = D.dw ; }		// T,w
		*V1 =(dtQ2*A2-dtQ1*B2)/(B1*A2-A1*B2) ;		//���������� V1
		*V2 =(dtQ1*B1-dtQ2*A1)/(B1*A2-A1*B2) ;		//           V2
	}
    u1j = u1 ;    
	u2j = u2 ;
    if (!pru) { 
		// pru = 0 - � ������ ������� ������ ���
	    // �������� ���.��� ��� V1
		dtu[0] = MS->MZ.M()*pudu / pdu*(1.0 - exp(-fabs(*V1) / pudu / g)) / a1j*sqrt(k_mu_kms / a1j);
		dtu[0] = GradA(dtu [0]) ;
	    // ��������  ���.��� ��� V2
		dtu[1] = (MS->MZ.M() - pdu / pudu*dtu[0] / a1j*sqrt(a1j / k_mu_kms))*pudu / pdu*(1. - exp(-fabs(*V2) / pudu / g)) / a2j*sqrt(k_mu_kms / a2j);
	    dtu[1] = GradA(dtu[1]) ;

	    u1j = u1-0.5*dtu[0] ; // �������� ������ ������ ������ ��� ��� V1
		u2j = u2-0.5*dtu[1] ; // �������� ������ ������ ������ ��� ��� V2
	}
    n1 = nn1 = (int)floor(fabs(*V1)/minv)+1 ; // ����� ����� ������� ��� V1
    n2 = nn2 = (int)floor(fabs(*V2)/minv)+1 ; // ����� ����� ������� ��� V2
    v1j = v2j = 0 ;
    *V1 =*V2  = 0 ;                           // ������� ������� �����

	// ������ ������������
	while (n1>=1 || n2>=1) {
		if (n1>=0) {
			// ��� ����������� �� V1
			MagicCalcDifDV(a1j, e1j, i, w1j, u1j, up, dN1, D); 
			if (kp ==1) { A1 = D.dT ; B1 = D.dL ; }
			if (kp ==2) { A1 = D.dL ; B1 = D.dw ; }
			if (kp ==3) { A1 = D.dT ; B1 = D.dw ; }		// ��������� �����������
			a1j+= v1j*D.da ;
			e1j+= v1j*D.de ;	// �������� ������ �� N1 ����� ������ v1j
			w1j+= v1j*D.dw ;
			a2j+= v1j*D.da ;
			e2j+= v1j*D.de ;	// �������� ������ �� N2 ����� ������ v1j
			w2j+= v1j*D.dw ;
			if (!pru) u1j+= dtu[0] /nn1 ;
			n1-= 1 ;
		}
		//--------------------------------------
		if (n2>=0) {
			// ��� �����������  �� V2
			MagicCalcDifDV(a2j, e2j, i, w2j, u2j, up, dN2, D); 
			if (kp ==1) { A2 = D.dT ; B2 = D.dL ; } // L,T
			if (kp ==2) { A2 = D.dL ; B2 = D.dw ; } // L,w
			if (kp ==3) { A2 = D.dT ; B2 = D.dw ; } // T,w  ��������� �����������
			// �������� ������ �� N2 ����� ������ v2j
			a2j+= v2j*D.da ;
			e2j+= v2j*D.de ;              
			w2j+= v2j*D.dw ;
			if (!pru) u2j+= dtu[1]/nn2 ;
			n2 -= 1 ;
		}
		//=====================================
		v1j =(dtQ2*A2-dtQ1*B2)/(B1*A2-A1*B2)/nn1 ;      //����� v1j
		v2j =(dtQ1*B1-dtQ2*A1)/(B1*A2-A1*B2)/nn2 ;      //����� v2j
		// ����� ������� ��������� � ��������� �����
		if (n1==0 || n1==(nn1-1)) *V1+= v1j/2.0 ;
		if (n2==0 || n2==(nn2-1)) *V2+= v2j/2.0 ;
		// ����� ������� ���������� �����
		if (n1>0 && n1<(nn1-1)) *V1+= v1j ;
		if (n2>0 && n2<(nn2-1)) *V2+= v2j  ;
	}
	// ����� ������������
    PV1[0] = a1j ; PV1[1] = e1j ; PV1[2] = w1j ;    //--�������� �,�,w �����
    PV2[0] = a2j ; PV2[1] = e2j ; PV2[2] = w2j ;    //--���������
	return 0 ;
}

//------------------------------------------------------------------------------
