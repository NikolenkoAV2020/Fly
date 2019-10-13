//-----------------------------------------------------------------------------
// MagicCalcU1.cpp: 
// �. ��������� 23.08.2018
//
// ���������� ������� �������������� ��� ��������-�������������� �������
// ���������� ���������� ��������� ������ ���� �� ��� ��������� �������� 10+,
// (�� ���� 10, 11,..) � ������ �������� ������� �����. 
// ����� ���� ������� ������ ���������. 
// !!!! ������ ��������� ����� �� ��������� � ������������� !!!!
//-----------------------------------------------------------------------------
#include <stdafx.h>

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>		// �������������� �������
#include <FlyMagicLNS.h>	//

extern double k_epsdrs ;		// 10 ������
extern double k_stepu ;			// [����] ��� �������� ��������� ������
extern int    pru ;				// ������� ����� ������� ������ ���
extern int    kp ;				// ������� ��������������� ���������
extern double dtQ1, dtQ2 ;		// ��� ������� �������
extern double PV1[3], PV2[3] ;	// ��� ��������� �,�,w  �� V1,V2

//-----------------------------------------------------------------------------
// ������ ���������� �� ��������� 1-���������� ���������
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ���������� ��������� ������
int MagicBuildORB(ZSMagicLnsSolution* MS, ZCLnsNpmClaim& PT, bool reBuild)
{
    ZSSpoOrb SP ;
	int rc = MagicBuildSpo(PT, SP) ; RRC ;
	// �� ������ �������� ��������� ��������� ��������� ������
	if (reBuild || MS->GlobCount == 1) {
		PT.T = SP.T ; 
		PT.a = SP.a ; 
		PT.e = SP.e ; PT.w = SP.w ; 
		PT.e1= SP.e1; PT.e2= SP.e2 ; 
		if (MS->GlobCount == 1) {
			PT.uH= SP.uh ;
			MS->MZ.PARe() = SP.e;
			MS->MZ.PARw() = SP.w;
		}
	} else {
		// �� ����������� ��������� ���������� ��������� ������, 
		// ����������� �� ������ ��������
		SP.e1= PT.e1; SP.e2= PT.e2 ; 
		SP.e = PT.e ; SP.w = PT.w ; 
		PT.T = SP.T ; 
		PT.a = SP.a ; 
		PT.p = SP.p ; 
	}	
	PT.p = PT.a*(1-PT.e*PT.e) ;
	return 0 ; 
}

//-----------------------------------------------------------------------------
//������ ���������� �� �������� "10"("a+h,Bh; a+e,w; a+����; a+��")
int MagicCalcU10(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	int rc = 0 ; 
	// "�������" ����������
	memset(&U, sizeof(U), 0) ;
    
	// �������������� ��������� ������ �� �������
	PT.VarPar = SPO_VARIDPAR_a ;
	rc = MagicBuildORB(MS, PT, false) ; if(rc) return rc;
    // ���������� �������� � ������� ������ ��������� ������ � �������
    ZSLnsNpmFault P ;
    rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; 
	if(rc) return rc;
	// ���� �������� ����� �����
	// P.u0 += PZ.w-P1.w ;  

	// ���������� ������������ ����������
	// �������� ���������� ��������
    if (!U.isFixedArg) U.u0[0] = P.u0 ;        
	U.n = 1 ; // ����������� ��������� - 1 	
    // ����������� V1 � ����������  ������� ��������
	rc = MagicUT1(MS, PZ.Vit - P1.Vit, 4, PT.a - PZ.a, P1, U.u0[0], &U.dV[0], PV1); RRC;
    return 0 ;
}

//-----------------------------------------------------------------------------
//������ ���������� �� �������� "11"("T+h,Bh; T+e,w; T+����; T+��")
int MagicCalcU11(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	int rc = 0 ; 
	// "�������" ����������
	memset(&U, sizeof(U), 0) ;

	// �������������� ��������� ������ �� �������
	PT.VarPar = SPO_VARIDPAR_T ;
	rc = MagicBuildORB(MS, PT, false) ; RRC ;
    // ���������� �������� � ������� ������ ��������� ������ � �������
    ZSLnsNpmFault P ;
    rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; if(rc) return rc;
	//���� �������� ����� �����
	// P.u0 += PZ.w-P1.w ;   
	
	// ���������� ������������ ����������
	// �������� ���������� ��������
    if (!U.isFixedArg) U.u0[0] = P.u0 ;
	U.n = 1 ; // ����������� ��������� - 1 	

    // ����������� V1 � ����������  ������� ��������
	rc = MagicUT1(MS, PZ.Vit - P1.Vit, 1, PT.T - PZ.T, P1, U.u0[0], &U.dV[0], PV1); RRC;
    return 0 ;
}

//-----------------------------------------------------------------------------
//������ ���������� �� �������� "11" ��� ������( T+ e, w )
int MagicCalcKondorU11(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
					   ZCLnsNpmClaim& PT, ZSLnsNpmFault& P, ZSFlightRule& U)
{
	int    rc = 0 ; 
    double a ;
	memset(&U, sizeof(U), 0) ;
     
    rc = MATC20(PT.i, PT.e1, PT.e2, PT.T, a) ;

	// ����������� ��������� ������ ��������
    rc = MagicCalcMishit(PT.i, a*(1-PT.e*PT.e), PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; RRC ;
	U.u0[0] = P.u0 ;
	U.n    = 1 ;

    // ����������� V1 � ����������  ������� ��������
    dtQ1 = (PT.T-PZ.T); kp = 1 ;
	rc = MagicUT1(MS, PZ.Vit - P1.Vit, kp, dtQ1, P1, U.u0[0], &U.dV[0], PV1); RRC;

    return rc ;
}

//-----------------------------------------------------------------------------
//������ ���������� �� �������� "12"(L+h,Bh ��� ��. ��� L+e,w ��� ����, ��)
int MagicCalcU12(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
    int      rc = 0, count=0;
	int      dN = PZ.Vit-P1.Vit ;
    ZSLnsNpmFault  P ;
    ZSLnsNpmDiffDV DP ;
	// ��������� ��������� ������� �� �������� �����
    dtQ1 = ValiDA(PT.L-PZ.L, 1) ;
	// "�������" ���������� 
	memset(&U, sizeof(U), 0) ;
	//
	if (!U.isFixedArg) {
		//�������� ����. �������� �� �����
		if (MS->GlobCount == 1) U.u0[0] = P1.w;
		// ��������� ���. ������ ��������
		while ( 1 ) {
			// ����������� V1 � ����������  ������� ��������
			rc = MagicUT1(MS, dN, 2, dtQ1, P1, U.u0[0], &U.dV[0], PV1); if (rc) return rc;
			// ������ ���������� �� ���������������� ��������
			rc = MagicIZM(MS, P1.a, P1.e, P1.i, P1.w, dN, U.u0[0], U.dV[0], DP); RRC;
			// ������ ���������� ������
			PT.T = P1.T+DP.dT ;
			// �������������� ��������� ������ �� �������
			PT.VarPar = SPO_VARIDPAR_T ;
			rc = MagicBuildORB(MS, PT, false); RRC;
			// ���������� �������� � ������� ������ ��������� ������ � �������
			rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; RRC ;
			U.u0[0] = P.u0 ;
			// ���� �������� ����� �����
			//U.u0[0]+= PZ.w - P1.w ;                
			if (count==3) break ; count++ ;
		}
		// ������ ��������� ������ ���������� ��������
		rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; RRC ;
		U.u0[0] = P.u0 ;	 
		U.n    = 1 ;	// ����������� ��������� - 1 	
	}
	// ������ �������� �������� ��������
	rc = MagicUT1(MS, dN, 2, dtQ1, P1, U.u0[0], &U.dV[0], PV1); RRC;
    return 0 ;
}

//-----------------------------------------------------------------------------
//������ ���������� �� �������� "13"(dL+h,Bh; dL+e,w; dL+����; dL+��;)
int MagicCalcU13(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	int rc = 0 ; 
	// "�������" ����������
	memset(&U, sizeof(U), 0) ;

	// �������������� ��������� ������ �� �������
	PT.VarPar = SPO_VARIDPAR_dL ;
	rc = MagicBuildORB(MS, PT, true); RRC;

    // ���������� �������� � ������� ������ ��������� ������ � �������
	ZSLnsNpmFault P ;
    rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; RRC ;
	// P.u0 += PZ.w-P1.w ;

	// ���������� ������������ ����������
	// �������� ���������� ��������
    if (!U.isFixedArg) U.u0[0] = P.u0 ;	
	U.n = 1 ;

    // �����������V1 � ����������  ������� ��������
	rc = MagicUT1(MS, PZ.Vit - P1.Vit, 1, PT.T - PZ.T, P1, U.u0[0], &U.dV[0], PV1); RRC;
    return rc ;
}

//-----------------------------------------------------------------------------
// ������� ��������� h,Bh
int MagicAD12FromHBh(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
					 ZCLnsNpmClaim& PT, double& A1, double& A2, double& D1, double& D2)
{
    double rhe, sb, si, d1, d2, k1, k2 ;
    double p = P1.a*(1.-P1.e*P1.e) ;
    sb = sin(RadA(PT.BH)-k_alfa0*Sin(2.*PT.BH)) ;
    si = Sin(PT.i) ;
    d2 = sb/si ;
    d1 = (PT.VarB ? -1.0:1.0)*sqrt(1.0-d2*d2) ;
    rhe= PT.Hmin+1e4*k_a0*(1.0-k_alfa0*sb*sb) ;
    k1 = rhe+k_eps/k_mu_kms/rhe*(1.0-d1+1.0/6.0*si*si*(4.0*d1-d1*d1+d2*d2-3.0)) ;
    k2 = k_eps/k_mu_kms/rhe*(d2+2.0/3.0*si*si*(d1*d2-d2))-2.0*k_a0*k_alfa0*d1*d2*si*si ;
    A1 = p*(k2/k1*d2+d1)/k1 ;
    A2 = p*(d2-k2/k1*d1)/k1 ;
    D1 = sqrt(k_mu_kms/p)*(A1-d1-P1.e*Cos(P1.w))/2.0 ;
    D2 = sqrt(k_mu_kms/p)*(A2-d2-P1.e*Sin(P1.w))/2.0 ;
    return 0 ;
}

//-----------------------------------------------------------------------------
// ������ ���������� �� �������� "14"("hmin + Bh ��� ��." 
// ��� "h�� + e, w ��� ����, ��")
int MagicCalcU14(
	ZSMagicLnsSolution* MS,	// �������� ������� ������.
	ZSLoopFact&    P1,		// ��-� ����. �� ����� ������� ��������.
	ZSLoopFact&	   PZ,		// ��-� ����. �� �������� ����� � ��������� �����
	ZCLnsNpmClaim& PT,		// ��������� �������� ��������
	ZSFlightRule&  U)		// ������� ���������� 
{
	ZSLnsNpmFault  P ;
    ZSLnsNpmDiffDV D ;
    int    dN = PZ.Vit - P1.Vit, rc = 0 ;
	double up = MS->MZ.PARu();
	memset(&U, sizeof(U), 0) ;
	U.n = 1 ;

	if (PT.VarOrb == SPO_ORB_HBH) {
		// ��� �������������������
		// hmin + Bhmin
		// �������������� ������ �� ���������
		if (U.isFixedArg) {            
			//�������� ���������� �����
			rc = MagicCalcDifDV(P1.a, P1.e, P1.i, P1.w, U.u0[0], up, dN, D) ; RRC ;
			U.dV[0] = (PT.Hmin-PZ.hmin)/D.dhp ;
		}
		if (!U.isFixedArg) {           
			//�������� ���������� �� �����
			double A1, A2, D1, D2 ;
			// �������� h,Bh
			MagicAD12FromHBh(MS, P1, PZ, PT, A1, A2, D1, D2) ;       
			double R1 = A1*D1+A2*D2 ;
			double R2 = sqrt(D1*D1+D2*D2-pow(A1*D2-A2*D1, 2)) ;
			double R3 = A1*A1+A2*A2-1 ;
			double dV1=-(R1-R2)/R3 ;
			double dV2=-(R1+R2)/R3 ;
			double dhe= (PT.VarH ? PT.Hmax-PZ.hmax : PT.Hmin-PZ.hmin) ;
			if (dV1*dhe>0 && dV2*dhe<=0) U.dV[0] = dV1 ;
			else {
				if (dV2*dhe>0 && dV1*dhe<=0) U.dV[0] = dV2 ;
				else U.dV[0] = fabs(dV1)<fabs(dV2) ? dV1:dV2 ;
			}
			U.u0[0] = AtanSC((A2*U.dV[0]+D2)/(A1*U.dV[0]+D1), A1+D1/U.dV[0]) ;
		}
	} else {
		// ��� ���� ��� ��
		// h�� + e, w`
		// �������������� ��������� ������ �� ������� ������ ��� ���� ��� ���
		PT.VarPar = SPO_VARIDPAR_H ; 
		PT.VarH   = SPO_VARH_MID ;
		rc = MagicBuildORB(MS, PT, true); RRC;
		// ���������� �������� � ������� ������ ��������� ������ � �������
		rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P); RRC ;
		// P.u0+= (PZ.w-P1.w) ; 
		//�������� ��  �����
		if (!U.isFixedArg) U.u0[0] = P.u0 ; 
		U.dV[0] = 0.5*P.l*P.dRs ;
	}
	return rc ;
}

//-----------------------------------------------------------------------------
// ������ ���������� �� �������� "15" (T+w ��� ����� ������)
int MagicCalcU15(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	// !!!! ��� ���������� ������ ���������� ������� ��� ������ SPO_ORB_EL
	assert(MS->MZ.ORB() == SPO_ORB_EL);

    int    i, k, rc = 0 ;
    double dw, du, dsw[2] ;
    double u1t, V1t = 0 ;
    int    dN = PZ.Vit-P1.Vit ;

	memset(&U, sizeof(U), 0) ;
	U.n = 1 ;
	k_stepu = 1.0 ;
    dtQ1 = PT.T-PZ.T ;    
	if (!U.isFixedArg) {
		//�������� ���������� �� �����
		dw = ValiDA(PT.w-PZ.w, 1) ;
		k  = (int)(180.0/k_stepu) ;
		dsw[0] = 360.0 ;
		// ��������� �������� ��������� ������
		u1t = P1.w ;
		du  = k_stepu*Sign1(dw*dtQ1) ;
		// ������ ��������
		for (i=0 ; i<k ; i++) {
			// ������� �������� ��������� ������
			u1t = ValiDA(u1t+du, 1) ;
			if (u1t<0) u1t+= 360.0 ;
			// ����������� V1 � ����������  ������� ��������
			V1t = 0 ;
			rc = MagicUT1(MS, dN, 1, dtQ1, P1, u1t, &V1t, PV1); RRC;
			dsw[1] = ValiDA(PT.w-PV1[2], 1) ;
			// �������� �������  ��������
			if (fabs(dsw[1])<fabs(dsw[0])) {
				U.u0[0]  = u1t ; 
				U.dV[0] = V1t ;   
				dsw[0]  = dsw[1] ; 
	}	}	}
	rc = MagicUT1(MS, dN, 1, dtQ1, P1, U.u0[0], &U.dV[0], PV1);
	return rc ;
}

//-----------------------------------------------------------------------------
// ������ ���������� �� �������� "16" (L+w ��� ����� ������)
int MagicCalcU16(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	// !!!! ��� ���������� ������ ���������� ������� ��� ������ SPO_ORB_EL
	assert(MS->MZ.ORB() == SPO_ORB_EL);

    int    dN, k , i, rc = 0 ;
    double dw, du, dsw[ 2 ] ;
    double u1t, V1t = 0   ;
    dN = PZ.Vit-P1.Vit ;

	memset(&U, sizeof(U), 0) ;
	U.n = 1 ;
	k_stepu = 1.0 ;

    dtQ2 =-ValiDA(PZ.L-PT.L, 1) ;                    
	if (!U.isFixedArg) {
		//�������� ���������� �� �����
		dw = ValiDA(PT.w-PZ.w, 1) ;
		dsw[0] = 360. ;
		du =-k_stepu*Sign1(dw*dtQ2) ;
		k  = (int)(180.0/k_stepu) ;
		// ��������� �������� ��������� ������
		u1t = P1.w ;    
		for (i=0 ; i<k ; i++) {
			// ������� �������� ��������� ������
			u1t = ValiDA(u1t+du, 1) ;
			if (u1t<0) u1t+= 360.0 ;
			// ����������� V1 � ����������  ������� ��������
			V1t = 0   ;
			rc = MagicUT1(MS, dN, 2, dtQ2, P1, u1t, &V1t, PV1); RRC;
			dsw[1] = ValiDA(PT.w-PV1[2], 1) ;
			// �������� �������  ��������
			if (fabs(dsw[1])<fabs(dsw[0])) {
				U.u0[0]  = u1t ; 
				U.dV[0] = V1t ; 
				dsw[0]  = dsw[1] ;
	}	}	}
	rc = MagicUT1(MS, dN, 2, dtQ2, P1, U.u0[0], &U.dV[0], PV1); RRC;
	return rc ;
}

//-----------------------------------------------------------------------------
// ������ ���������� �� �������� "17" (T ��� ����� ������)
int MagicCalcU17(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	int rc = 0 ;
	int dN = PZ.Vit-P1.Vit ;
	dtQ1 = PT.T-PZ.T ;
	
	memset(&U, sizeof(U), 0) ;
	U.n = 1 ;
	U.uw[0] = false ;
	// ����  �������� ���������� �� �����
	if (!U.isFixedArg) { 
		U.u0[0] = P1.w ; 
		U.uw[0]= 1 ; 
	}
	// ����������� V1 � ����������  ������� ��������
	rc = MagicUT1(MS, dN, 1, dtQ1, P1, U.u0[0], &U.dV[0], PV1);
	return rc ;
}

//-----------------------------------------------------------------------------
// ������ ���������� �� �������� "18" (L ��� ����� ������)
int MagicCalcU18(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	int rc = 0 ;
	int dN = PZ.Vit-P1.Vit ;
    dtQ2 =-ValiDA(PZ.L-PT.L, 1) ;                    

	memset(&U, sizeof(U), 0) ;
	U.n = 1 ;
	U.uw[0] = false ; 
	// ����  �������� ���������� �� �����
	if (!U.isFixedArg) { 
		U.u0[0] = P1.w ; 
		U.uw[0] = true ; 
	}
	// ����������� V1 � ����������  ������� ��������
	rc = MagicUT1(MS, dN, 2, dtQ2, P1, U.u0[0], &U.dV[0], PV1) ;
	return rc ;
}

//-----------------------------------------------------------------------------
