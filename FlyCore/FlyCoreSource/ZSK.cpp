//---------------------------------------------------------------------------
// ZSK.cpp
// �. ��������� 03.08.2018
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
//---------------------------------------------------------------------------
#include <FlyMMath.h>
#include <FlyTime.h>

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\Z.h>

static double M_ASK2000to1975[9] ;
static bool   isCalc_M_ASK2000to1975 = false ; 

int ZCalcM_ASK2000to1975( )
{
	if ( isCalc_M_ASK2000to1975 ) return 0 ;

	double M[9] ; ZBuildMatrPrecessJ2000(J_ask0, M, TIMEJ2000JC) ;
	isCalc_M_ASK2000to1975 = true ; 
	memcpy(M_ASK2000to1975, M, 9*sizeof(double)) ;
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ���_1975 � ���_2000
//----------------------------------------------------------------------------
int ZASC_1975to2000(double *dASC_1975, double *dASC_2000, bool Speed)
{
	ZCalcM_ASK2000to1975( ) ;
	umobr(M_ASK2000to1975, dASC_1975, dASC_2000);
	if(Speed) umobr(M_ASK2000to1975, dASC_1975+3, dASC_2000+3);
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ���_2000 � ���_1975
//----------------------------------------------------------------------------
int ZASC_2000to1975(double *dASC_2000, double *dASC_1975, bool Speed)
{
	ZCalcM_ASK2000to1975( ) ;
	umpr(M_ASK2000to1975, dASC_2000, dASC_1975);
	if(Speed) umpr(M_ASK2000to1975, dASC_2000+3, dASC_1975+3);
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ���_TE � ���_2000
//----------------------------------------------------------------------------
int ZASC_TEto2000(double t, double *dASC_TE, double *dASC_2000, bool Speed )
{
	double MN[9] ; ZBuildMatrNutJ2000(t, MN, TIME1975ASD) ; 
	double MP[9] ; ZBuildMatrPrecessJ2000(t, MP, TIME1975ASD) ;
	double X[6] ;
	// �������� ���������
	Um_Mat(dASC_TE, MN, X, 1, 3, 3);
	umobr(MP, X, dASC_2000);
	// �������� ���������
	if (Speed) {
		Um_Mat(dASC_TE+3, MN, X+3, 1, 3, 3);
		umobr(MP, X+3, dASC_2000+3);
	}
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ���_2000 � ���_TE (�������� ������� �����)
//----------------------------------------------------------------------------
int ZASC_2000toTE(double t, double *dASC_2000, double *dASC_TE, bool Speed)
{
	double MN[9] ; ZBuildMatrNutJ2000(t, MN, TIME1975ASD) ; 
	double MP[9] ; ZBuildMatrPrecessJ2000(t, MP, TIME1975ASD) ;
	double X[6] ;
	umpr(MP, dASC_2000, X);
	Um_Mat(MN,X,dASC_TE,3,3,1);
	if (Speed) {
		umpr(MP, dASC_2000+3, X+3);
		Um_Mat(MN,X+3,dASC_TE+3,3,3,1);
	}
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ���_TES � ���_2000
//----------------------------------------------------------------------------
int ZASC_TESto2000(double t, double *dASC_TES, double *dASC_2000, bool Speed )
{
	double MP[9] ;
	ZBuildMatrPrecessJ2000(t, MP, TIME1975ASD) ;
	// �������� ��������� � ���������
	umobr(MP, dASC_TES, dASC_2000);
	if(Speed) umobr(MP, dASC_TES+3, dASC_2000+3);
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ���_2000 � ���_TES
//----------------------------------------------------------------------------
int ZASC_2000toTES(double t, double *dASC_2000, double *dASC_TES, bool Speed)
{
	double MP[9] ;
	ZBuildMatrPrecessJ2000(t, MP, TIME1975ASD) ;
	umpr(MP, dASC_2000, dASC_TES);
	if(Speed) umpr(MP, dASC_2000+3, dASC_TES+3);
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ���_1975 � ���_TE
//----------------------------------------------------------------------------
int ZASC_1975toTE(double t, double* dASC_1975, double* dASC_TE, bool Speed)
{
	double dASC_2000[7] ;
	ZASC_1975to2000(dASC_1975, dASC_2000, Speed) ;
	ZASC_2000toTE  (t, dASC_2000, dASC_TE, Speed) ;
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ���_TE � ���_1975
//----------------------------------------------------------------------------
int ZASC_TEto1975(double t, double* dASC_TE, double* dASC_1975, bool Speed)
{
	double dASC_2000[7] ;
	ZASC_TEto2000  (t, dASC_TE, dASC_2000, Speed) ;
	ZASC_2000to1975(dASC_2000, dASC_1975, Speed) ;
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ���_1975 � ���_TES
//----------------------------------------------------------------------------
int ZASC_1975toTES(double t, double* dASC_1975, double* dASC_TES, bool Speed)
{
	double dASC_2000[7] ;
	ZASC_1975to2000(dASC_1975, dASC_2000, Speed) ;
	ZASC_2000toTES (t, dASC_2000, dASC_TES, Speed) ;
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ���_TES � ���_1975
//----------------------------------------------------------------------------
int ZASC_TESto1975(double t, double* dASC_TES, double* dASC_1975, bool Speed)
{
	double dASC_2000[7] ;
	ZASC_TESto2000 (t, dASC_TES, dASC_2000, Speed) ;
	ZASC_2000to1975(dASC_2000, dASC_1975, Speed) ;
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ���_2000 � ���
//----------------------------------------------------------------------------
int ZASC2000toGSC(double t, double* dASK_2000, double* dGSK, bool Flag_R)
{
	double MN[9] ;		ZBuildMatrNutJ2000(t, MN, TIME1975ASD) ;		// ������� �������
	double MP[9] ;		ZBuildMatrPrecessJ2000(t, MP, TIME1975ASD) ;	// ������� ��������
	double MER[9] ; 
	double MDER[9] ;	ZMER(t, MER, MDER) ;				// ������ ������� �� ���

	double MNP[9] ;		Um_Mat(MN, MP, MNP, 3, 3, 3) ;		//
	double MG[9] ;		Um_Mat(MER, MNP, MG, 3, 3, 3) ;		// ������� �������� �� ��� 2000 � ��� 
	double DMG[9] ;		Um_Mat(MDER, MNP, DMG, 3, 3, 3) ;	// ����������� ������� �������� �� ��� 2000 � ��� 

	umpr(MG, dASK_2000, dGSK);			// ������� ��������� ���������
	if( !Flag_R ) {						// ������� �������� ��������
		umpr(MG, dASK_2000+3, dGSK+3) ;
		double X[6] ;
		umpr(DMG, dASK_2000, X) ;
		for (int i=0 ; i<3 ; i++) dGSK[i+3]+= X[i] ;   
	}
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �������� �� ��� � ���_2000
//----------------------------------------------------------------------------
int ZGSCtoASC2000(double t, double* dGSC, double* dASC_2000, bool Flag_R)
{
	double MN[9];		ZBuildMatrNutJ2000(t, MN, TIME1975ASD) ;		// ������� �������
	double MP[9] ;		ZBuildMatrPrecessJ2000(t, MP, TIME1975ASD) ;	// ������� ��������
	double MER[9] ; 
	double MDER[9] ;	ZMER(t, MER, MDER) ;				// ������ ������� �� ���

	double MNP[9] ;		Um_Mat(MN, MP, MNP, 3, 3, 3) ;		//
	double MG[9] ;		Um_Mat(MER, MNP, MG, 3, 3, 3) ;		// ������� �������� �� ��� 2000 � ��� 
	double DMG[9] ;		Um_Mat(MDER, MNP, DMG, 3, 3, 3) ;	// ����������� ������� �������� �� ��� 2000 � ��� 

	umobr(MG, dGSC, dASC_2000);			// ������� ��������� ���������
	if( !Flag_R ) {						// ������� �������� ��������
		umobr(MG, dGSC+3, dASC_2000+3) ;
		double X[6] ;
		umobr(DMG, dGSC, X) ;
		for (int i=0 ; i<3 ; i++) dASC_2000[i+3]+= X[i] ;   
	}
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �� ���_1975 � ��� � �������
//----------------------------------------------------------------------------
int ZASC1975toGSC(double t, double* dASC_1975, double* dGSC, bool Flag_R)
{
	double dASC_2000[7] ;
	ZASC_1975to2000(dASC_1975, dASC_2000, !Flag_R) ;
	ZASC2000toGSC(t, dASC_2000, dGSC, Flag_R) ;
	return 0 ;
}

int ZGSCtoASC1975(double t, double* dGSC, double* dASC_1975, bool Flag_R)
{
	double dASC_2000[7] ;
	ZGSCtoASC2000 (t, dGSC, dASC_2000, Flag_R) ;
	ZASC_2000to1975(dASC_2000, dASC_1975, !Flag_R) ;
	return 0 ;
}

//----------------------------------------------------------------------------
// ������� �� ��� � ������������ �������� ������ � �������
//---------------------------------------------------------------------------
void ZASCtoOSC(double *a, double *o)
{
	short delta = 0,i ;
	ask_nsk( a, o, 0, (short* )&delta, 7, k_b0 ) ;
	for (i=3 ; i<6 ; i++) o[i]+= -floor( o[i]/k_dwapi )*k_dwapi ;
	while (o[5]>=k_dwapi) o[5]-= k_dwapi ;
	while (o[5]< 0.)	  o[5]+= k_dwapi ;
}

//---------------------------------------------------------------------------
void ZOSCtoASC(double *o, double *a)
{
	short delta=0;
	ask_nsk(a, o, 1, (short *)&delta, 7, k_b0);
	//ask_nsk(a, o, 1, (short *)&delta, 7, 398600.44*8.64*8.64/1e4);
}

//---------------------------------------------------------------------------
// ������ ���������� ���������� ��������� ������ �� ���������� �������������� ����������
//---------------------------------------------------------------------------
int ZCalcKEO(double *A, double *KEO)
{
	double dP, dF, dUbl, dUol, dR[4], dC[4], dL[4], dB[3], dO[3], dQ[3];
	double dM[3]={0.,1.,0.}, dN[3]={0.,0.,1.}, dEps=1.e-14;

	// dR - ������-������ ��������� ��
	dR[3] = FR(A);
	dR[0] = A[0]/dR[3];
	dR[1] = A[1]/dR[3];
	dR[2] = A[2]/dR[3];
	// dP - ��������������� ��������
	dP = k_b0/dR[3];
	// dC - ������ ������� ���������� �������� (������� � ��������� ������)
	wek_pr(A,&A[3],dC);
	dC[3] = FR(dC);
	// dL - ������ ������� (����������� �� ������� ������)
	dL[0] = dC[2]*A[4]-dC[1]*A[5]-dP*A[0];
	dL[1] = dC[0]*A[5]-dC[2]*A[3]-dP*A[1];
	dL[2] = dC[1]*A[3]-dC[0]*A[4]-dP*A[2];
	dL[3] = FR(dL);
	dL[0] /= dL[3]; dL[1] /= dL[3]; dL[2] /= dL[3];
	// dB - ������ ����������� ��������� ������ � ���������� �������� ���������
	wek_pr_norm(dM,dC,dB);
	// dO - ������ ����������� ��������� ������ � ���������� ��������
	wek_pr_norm(dN,dC,dO);
	// ���� ����� �������� dB � �������� dL
	dP = scal_pr(dL,dB,3); dF = fabs(dP); if(dF>1.) dP /= dF;
	dUbl = acos(dP);
	wek_pr(dC,dB,dQ);
	dP = scal_pr(dL,dQ,3); dF = fabs(dP); if(dF>1.) dP /= dF;
	if(dP<0.) dUbl = k_dwapi-dUbl;
	// ���� ����� �������� dO � �������� dL
	dP = scal_pr(dL,dO,3); dF = fabs(dP); if(dF>1.) dP /= dF;
	dUol = acos(dP);
	wek_pr(dC,dO,dQ);
	dP = scal_pr(dL,dQ,3);
	if(dP<0.) dUol = k_dwapi-dUol;
	// ��������� �������
	KEO[1] = dC[3]*dC[3]/k_b0;
	// ��������������
	KEO[2] = dL[3]/k_b0;
	if(KEO[2]>=1.) return S_FALSE;
	// ������� �������
	KEO[0] = KEO[1]/(1-KEO[2]*KEO[2]);
	// ����������
	KEO[3] = acos(dC[2]/dC[3]);
	while(KEO[3]>=k_dwapi) KEO[3]-=k_dwapi;
	while(KEO[3]<0.) KEO[3]+=k_dwapi;
	dF = dC[3]*sin(KEO[3]);
	// ������ ����������� ����������� ����
	KEO[4] = dargum(-dC[1],dC[0]);
	while(KEO[4]>=k_dwapi) KEO[4]-=k_dwapi;
	while(KEO[4]<0.) KEO[4]+=k_dwapi;
	// �������� ������� �� ��������
	KEO[5] = dUol;
	while(KEO[5]>=k_dwapi) KEO[5]-=k_dwapi;
	while(KEO[5]<0.) KEO[5]+=k_dwapi;
	// �������� ������� �� ���������
	KEO[6] = dUbl;
	while(KEO[6]>=k_dwapi) KEO[6]-=k_dwapi;
	while(KEO[6]<0.) KEO[6]+=k_dwapi;
	// �������� ��������
	dP = scal_pr(dR,dL,3); dF = fabs(dP); if(dF>1.) dP /= dF;
	KEO[7] = acos(dP);
	wek_pr(dC,dL,dQ);
	dP = scal_pr(dR,dQ,3);
	if(dP<0.) KEO[7] = k_dwapi-KEO[7];
	while(KEO[7]>=k_dwapi) KEO[7]-=k_dwapi;
	while(KEO[7]<0.) KEO[7]+=k_dwapi;
	// ��������������� ��������
	KEO[8] = 2.*atan(sqrt((1-KEO[2])*(1+KEO[2]))*tan(KEO[7]/2.));
	while(KEO[8]>=k_dwapi) KEO[8]-=k_dwapi;
	while(KEO[8]<0.) KEO[8]+=k_dwapi;
	// ������� ��������
	KEO[9] = KEO[8]-KEO[2]*sin(KEO[8]);
	while(KEO[9]>=k_dwapi) KEO[9]-=k_dwapi;
	while(KEO[9]<0.) KEO[9]+=k_dwapi;
	// �������� ������ �� ��������
	KEO[10] = dUol + KEO[7];
	while(KEO[10]>=k_dwapi) KEO[10]-=k_dwapi;
	if(fabs(KEO[10]-k_dwapi)<dEps) KEO[10]=0.;
	if(fabs(KEO[10])<dEps) KEO[10]=0.;
	while(KEO[10]<0.) KEO[10]+=k_dwapi;
	// �������� ������ �� ���������
	KEO[11] = dUbl + KEO[7];
	while(KEO[11]>=k_dwapi) KEO[11]-=k_dwapi;
	if(fabs(KEO[11]-k_dwapi)<dEps) KEO[11]=0.;
	if(fabs(KEO[11])<dEps) KEO[11]=0.;
	while(KEO[11]<0.) KEO[11]+=k_dwapi;
	return S_OK ;
}

//---------------------------------------------------------------------------
// ������� ������� ������� �������� �������������� ����������
// �������� ������� �� ������������ �� � ���
//---------------------------------------------------------------------------
// ����������
//	   ������������ ��������� ������� �������� ������������
//	   ������� ��������� (x,y,z,Vx,Vy,Vz): ���-��, ����,
//	   �����-J1975, �����-J2000 � �.�. � ���������
//     �������� ������ (a,e,i,om,w,u).
// ������� ���������:
//	   ask[6] - ������ ������� ��������� � ������������ ��
//	   X,Y,Z,,Vx,Vy,Vz � ����������� ���� � ����/���,
// �������� ���������:
//	   m[36] - ������, �������� �������� ���������� �������� ������� ��������.
// ���������� �������� � ����������� ��������� ������� ��.
//---------------------------------------------------------------------------
void ZM_ASCtoOSC(double *ask, double *m)
{
	//������ ������� ��� <- ��� (�� ������� ���)
	double x, y, z, vx, vy, vz, e, q, qr, c_2, c_0, l_2, l, r, r_2, R, si, cw, tgi;
	double dC[18], dL[18], C[3], L[3], dC1[6], dL1[6], dr[6], dz[6], CC[18];
	short  i, j;
	double const l1e_7=1.0e-7, l1=1.0, l0=0.0;
	double keo[6];
	// ������� ���������
	ZASCtoOSC(ask,keo);
	// ������������ ������� ��������
	x=ask[0]; y=ask[1]; z=ask[2]; vx=ask[3]; vy=ask[4]; vz=ask[5];
	r_2=x*x+y*y+z*z;       r=sqrt(r_2);
	// dr/x,y,z,vx,vy,vz
	if(r<=0.0) return ;

	dr[0]=x/r; dr[1]=y/r; dr[2]=z/r;
	dr[3]=l0; dr[4]=l0; dr[5]=l0;
	// dz/x,y,z,vx,vy,vz
	dz[0]=l0; dz[1]=l0; dz[2]=l1; dz[3]=l0; dz[4]=l0; dz[5]=l0;
	//      dC/x                        dC/y
	dC[0]=l0; dC[1]=-vz; dC[2]=vy;    dC[3]=vz; dC[4]=l0; dC[5]=-vx;
	//      dC/z                        dC/vx
	dC[6]=-vy; dC[7]=vx; dC[8]=l0;    dC[9]=l0; dC[10]=z; dC[11]=-y;
	//      dC/vy                       dC/vz
	dC[12]=-z;dC[13]=l0;dC[14]=x;     dC[15]=y; dC[16]=-x; dC[17]=l0;

	C[0]=y*vz-z*vy;   C[1]=z*vx-x*vz;   C[2]=x*vy-y*vx;
	c_2=C[0]*C[0]+C[1]*C[1]+C[2]*C[2];  c_0=sqrt(c_2);
	q=C[0]*C[0]+C[1]*C[1];  qr=sqrt(q);

	// dL/x
	dL[0]=dC[2]*vy-dC[1]*vz+k_b0*(x*x/r_2-l1)/r;
	dL[1]=dC[0]*vz-dC[2]*vx+k_b0*x*y/r_2/r;
	dL[2]=dC[1]*vx-dC[0]*vy+k_b0*x*z/r_2/r;
	// dL/y
	dL[3]=dC[5]*vy-dC[4]*vz+k_b0*x*y/r_2/r;
	dL[4]=dC[3]*vz-dC[5]*vx+k_b0*(y*y/r_2-l1)/r;
	dL[5]=dC[4]*vx-dC[3]*vy+k_b0*y*z/r_2/r;
	// dL/z
	dL[6]=dC[8]*vy-dC[7]*vz+k_b0*x*z/r_2/r;
	dL[7]=dC[6]*vz-dC[8]*vx+k_b0*y*z/r_2/r;
	dL[8]=dC[7]*vx-dC[6]*vy+k_b0*(z*z/r_2-l1)/r;
    // dL/vx
	dL[9]=dC[11]*vy-dC[10]*vz;  dL[10]=dC[9]*vz-dC[11]*vx-C[2];
	dL[11]=dC[10]*vx-dC[9]*vy+C[1];
	// dL/vy
	dL[12]=dC[14]*vy-dC[13]*vz+C[2];  dL[13]=dC[12]*vz-dC[14]*vx;
    dL[14]=dC[13]*vx-dC[12]*vy-C[0];
	// dL/vz
	dL[15]=dC[17]*vy-dC[16]*vz-C[1];  dL[16]=dC[15]*vz-dC[17]*vx+C[0];
    dL[17]=dC[16]*vx-dC[15]*vy;

	L[0]=vy*C[2]-vz*C[1]-k_b0*x/r;
	L[1]=vz*C[0]-vx*C[2]-k_b0*y/r;
	L[2]=vx*C[1]-vy*C[0]-k_b0*z/r;
	l_2=L[0]*L[0]+L[1]*L[1]+L[2]*L[2]; l=sqrt(l_2);
	for(i=0;i<6;i++) {
		Um_Mat(C,&dC[i*3],&R,1,3,1);
		dC1[i]=R/c_0;
	}
	for(i=0;i<6;i++) {
		Um_Mat(L,&dL[i*3],&R,1,3,1);
		dL1[i]=R/l;
	}
	TRAM(dC,CC,6,3);
	for(i=0;i<18;i++) dC[i]=CC[i];
	TRAM(dL,CC,6,3);
	for(i=0;i<18;i++) dL[i]=CC[i];
    e=l1-keo[1]*keo[1];
    si=sin(keo[2]);
    cw=cos(keo[4]);
    tgi=tan(keo[2]);
    if(fabs(si)<l1e_7) if(si<l0) si=-l1e_7; else si=l1e_7;
    if(fabs(cw)<l1e_7) if(cw<l0) cw=-l1e_7; else cw=l1e_7;
    if(fabs(tgi)<l1e_7)if(tgi<l0) tgi=-l1e_7; else tgi=l1e_7;
    for(j=0; j<6; j++) {
		m[j]=(dC1[j]+dL1[j]*c_0*keo[1]/(e*k_b0))*(2.*c_0/(k_b0*e));
		m[j+6]=dL1[j]/k_b0;
		m[j+12]=-(dC[j+12]-C[2]*dC1[j]/c_0)/(c_0*si);
		m[j+18]=(C[0]*dC[j+6]/(C[1]*C[1])-dC[j]/C[1]) / (l1+tan(keo[3])*tan(keo[3]));
		m[j+24]=(dL[j+12]-L[2]*dL1[j]/l-L[2]*m[j+12]/tgi) / (cw*si*l);
		m[j+30]=(dC1[j]*z+dz[j]*c_0-z*c_0*(dr[j]*qr+r*(C[0]*dC[j]+C[1]*dC[j+6])/qr)/r/qr) / (y*C[0]-x*C[1]);
	}
}

//---------------------------------------------------------------------------
// ������� ������� ������� �������� ����������
// �������� ������� �� ��� � ������������ ��
//---------------------------------------------------------------------------
//	����������
//	   ������������ ��������� ������� �������� ����������
//	   ��������� ������ (a,e,i,om,w,u) � ������������
//	   ������� ���������� (x,y,z,Vx,Vy,Vz): ���-��, ����,
//	   �����-J1975, �����-J2000 � �.�.
//	������� ���������:
//     keo[6]- ������ ���������� ��������� ������
//     (a[����], e[�/�], i[���], om[���], w[���], u[���])
//	�������� ���������:
//     m[36] - ������, �������� �������� ���������� �������� ������� ��������.
//	���������� �������� � ����������� ��������� ������� ��.
//---------------------------------------------------------------------------
void ZM_OSCtoASC (double *keo,double *m)
{
	//������ ������� ��� <- ��� �� ������� ���
	double c_u, s_u, c_om, s_om, c_i, s_i, r_1, r_2, c_z, s_z, c_z0, r_a;
	double x_ckb, y_ckb, z_ckb, g_uw;
	double kor1, kor2, k0, k1, k2, k3, k4, k5, k6, k7, k8, k9, k10, k11;
	double const l1=1.0, l0=0.0;
	double ask[6];

	// ������� ���������
	ZOSCtoASC(keo,ask);
	// ������������ ������� ��������
	c_u=cos(keo[5]);  s_u=sin(keo[5]);
	c_om=cos(keo[3]); s_om=sin(keo[3]);
	c_i=cos(keo[2]);  s_i=sin(keo[2]);
	r_1=l1-keo[1]*keo[1]; r_2=keo[0]*r_1; g_uw=keo[5]-keo[4];
	c_z=cos(g_uw); s_z=sin(g_uw); c_z0=l1+keo[1]*c_z; r_a=r_2/c_z0;
	x_ckb=c_u*c_om-s_u*s_om*c_i;y_ckb=c_u*s_om+s_u*c_om*c_i;z_ckb=s_u*s_i;

	m[0]=ask[0]/keo[0];
	m[6]=ask[1]/keo[0];
	m[12]=ask[2]/keo[0];
	m[18]=-ask[3]/(2.*keo[0]);
	m[24]=-ask[4]/(2.*keo[0]);
	m[30]=-ask[5]/(2.*keo[0]);
	k0=(2.*keo[1]*c_z0+r_1*c_z)/(c_z0*c_z0);
	m[1]=-keo[0]*k0*x_ckb;
	m[7]=-keo[0]*k0*y_ckb;
	m[13]=-keo[0]*k0*z_ckb;
    kor1=sqrt(k_b0/(keo[0]*r_1));
    kor2=sqrt(k_b0/keo[0]);
    k7=sqrt(r_1*r_1*r_1);
    k1=kor2*keo[1]*keo[1]/k7+kor1;
    k2=kor2*keo[1]*c_z0/k7+kor1*c_z;
    k5=(s_u*c_om+c_u*s_om*c_i);
    k6=(s_u*s_om-c_u*c_om*c_i);
	m[19]=s_z*x_ckb*k1-k5*k2;
	m[25]=s_z*y_ckb*k1-k6*k2;
	m[31]=s_z*z_ckb*k1+(c_u*s_i)*k2;
	m[2]=r_a*s_u*s_om*s_i;
	m[8]=-r_a*s_u*c_om*s_i;
	m[14]=r_a*s_u*c_i;
	m[20]=kor1*(keo[1]*s_z*s_u*s_om*s_i+c_z0*c_u*s_om*s_i);
	m[26]=-kor1*(keo[1]*s_z*s_u*c_om*s_i+c_z0*c_u*c_om*s_i);
	m[32]=kor1*(keo[1]*s_z*s_u*c_i+c_z0*c_u*c_i);
    k8=c_u*s_om+s_u*c_om*c_i;
    k9=c_u*c_om-s_u*s_om*c_i;
	m[3]=-r_a*k8;
	m[9]=r_a*k9;
	m[15]=l0;
	m[21]=kor1*(c_z0*k6-keo[1]*s_z*k8);
	m[27]=kor1*(keo[1]*s_z*k9-c_z0*k5);
	m[33]=l0;
    k10=-(r_2*keo[1]*s_z)/(c_z0*c_z0);
	m[4]=k10*x_ckb;
	m[10]=k10*y_ckb;
	m[16]=k10*z_ckb;
	k3=kor1*keo[1]*c_z; k4=kor1*keo[1]*s_z;
	m[22]=-(k3*x_ckb+k4*k5);
	m[28]=-(k3*y_ckb+k4*k6);
	m[34]=-(k3*z_ckb-k4*c_u*s_i);

	m[5]=-m[4]-r_a*k5;
	m[11]=-m[10]-r_a*k6;
	m[17]=-m[16]+r_a*(c_u*s_i);

	m[23]=k3*x_ckb-k4*k5+kor1*(keo[1]*s_z*k5-c_z0*x_ckb);
	m[29]=k3*y_ckb-k4*k6+kor1*(keo[1]*s_z*k6-c_z0*y_ckb);
    k11=s_z*c_u;
	m[35]=kor1*s_i*(keo[1]*(c_z*s_u+k11)-(keo[1]*k11+c_z0*s_u));
}

//---------------------------------------------------------------------------
//  ������ cos(f-E) � sin(f-E) � y �� e*cosf e*sinf eta � x  
void anom( double *x,double *y )
{ 
	double a;
	a=1+x[2];
	y[0]=x[1]/((1+x[0])*a);
	y[1]=(a+x[0])*y[0];
	y[0]=1-x[1]*y[0];
}

//---------------------------------------------------------------------------
//	������� ��������� ������� � ����������� ����������
//	��� ����� 
//		x[0] - e*cos(l)
//		x[1] - e*sin(l)
//	�� ������ 
//		x[2] - e*cos(E)
//		x[3] - e*sin(E)
//---------------------------------------------------------------------------
void kep_rml(double* anom)
{
	static const double koef[10]= {
	0.500000588274841173,
	0.500002353106286046,
	0.500009412535887821,
	0.500037651915547721,
	0.500150636020651018,
	0.500602998235196273,
	0.502419286188155678,
	0.509795579104159180,
	0.541196100146197012,
	0.707106781186547462
	};
	static const double pi=3.14159265358979312;
	static const double tochnost=1.0e-15;
	//��� ����� anom[0]=e*cos(M)  anom[1]=e*sin(M)
	//�� ������ ������� � anom[2] ��������� ��������� ��������
	//e*cos(E) e*sin(E) ;
	//���������p����� e*cos(f) e*sin(f) e**2 sqrt(1-e*e)
	short indicator=0;
	double a=anom[0],b=anom[1];
	double xl=0,ft=0,xt=0,fp=a,fl,xp=pi/2;
	short cikl=10;
	fl=fabs(b);
	if (b<0) {indicator=1;b=-b;}
	for (;--cikl>=0;) { //����� � ������� ����������� 
		xt=(xl+xp)*0.5;
		ft=koef[cikl]*(fl+fp);
		if (ft<xt) {xp=xt;fp=ft;}
		else 	   {xl=xt;fl=ft;}
	}
	xt=(fp*xl-fl*xp)/(fp-fl-xp+xl);
	iteration:
	ft=sin(xt);
	fl=cos(xt);
	fp=b*ft-a*fl+1.0; /*������ ����������� */
	fl=a*ft+b*fl; /*������ ����������� */
	ft=xt-fl; /*�������� ������� */
	fl=fl/fp;
	fp=ft/fp;
	xl=fp*(1.0+0.5*(fp*fl)); /*������ xt*/
	xt=xt-xl;
	if (fabs(xl) > tochnost) goto iteration;
	if (indicator != 0) {xt=-xt;b=-b;} /*�������� E-M */
	fl=cos(xt);
	fp=sin(xt);
	xl=a*fl-b*fp; /* e*cos(E) */
	xp=b*fl+a*fp; /* e*sin(E) */
	anom[2]=xl;
	anom[3]=xp;
	//xt=1.0/(1.0-xl);
	//ft=a*a+b*b;  // e*e
	//fl=sqrt(1.0-ft); //  ���
	//anom[4]=(xl-ft)*xt; //  e*cos(f)
	//anom[5]=fl*xt*xp; //  e*sin(f)
	//anom[6]=ft;
	//anom[7]=fl;
}

//---------------------------------------------------------------------------
//  ������� �������� �� ������������ ��� ����������� �  ���������� � �������
//---------------------------------------------------------------------------
//    direct = 0   asks -> nsk
//	   = 1   nsk -> asks
//	   = 2   nsk -> asks  ������ ����������
//	sk = 0   ����������� �� ������� �������� � �������
//	     1   ������������ �� ������� ���������
//	     2   ������������ �� ������� ��������,�������� ������� � �������
//	     3   ������������ �� ������� ���������� ������
//	     4   ����������� � �������� �������� � �������
//	     5   ������������ � �������� ���������
//	     6   ������������ � �������� ��������,�������� ������� � �������
//	     7   ������������ � �������� ���������� ������
//
//    ������ ����� �� ���������� ��� �������� �� �������� ������������ �
// ���������� � ��� ��������� �� ����������. � ���� ������ ��� �����
// ��� ����� 0 � ����� ����������� �������������. � ��������� �������
// ������ ������ ���� 1 ��� -1 � �� ���������� �� ��������� ��������.
//---------------------------------------------------------------------------
void ask_nsk(double *ask,double *nsk,short direct, short *delta, short sk, double Mu )
{
	double psk[ 8 ] ;
	if ( !direct ) goto met1 ;
met2:
	psk_nsk( psk, nsk, direct, delta, sk, Mu ) ;
	if ( !direct ) goto met3;
met1:
	ask_psk( ask, psk, direct, delta ) ;
	if ( !direct ) goto met2 ;
met3:
	;
}

//---------------------------------------------------------------------------
//  ������� �� ������������ ��� ����������� � ������������� � ������� */
//
//    direct = 0   psk -> nsk
//	  != 0   nsk -> psk
//	sk = 0   ����������� �� ������� �������� � �������
//	     1   ������������ �� ������� ���������
//	     2   ������������ �� ������� ��������,�������� ������� � �������
//	     3   ������������ �� ������� ���������� ������
//	     4   ����������� � �������� �������� � �������
//	     5   ������������ � �������� ���������
//	     6   ������������ � �������� ��������,�������� ������� � �������
//	     7   ������������ � �������� ���������� ������
//    ������ ����� �� ���������� ��� �������� �� �������� ������������ �
// �������������.� ���� ������ ��� ����� ��� ����� 0 � ����� �����������
// �������������. � ��������� ������� ������ ������ ���� 1 ��� -1 � ��
// ���������� �� ��������� ��������.
//---------------------------------------------------------------------------
void psk_nsk(double *psk,double *nsk,short direct, short *delta,short sk,double Mu)
{ 
	double x[6];
	short r1,r2,r3;
	r1=sk&0x03;
	r2=sk&0x01;
	r3=sk&0x02;
	if ( !direct ){
		if ( r1 ) {
			nsk[3]=dargum(-psk[1],psk[0]);
			if ( !r2 ) nsk[3]*=(*delta);
			nsk[2]=dargum(psk[2],sqrt(psk[0]*psk[0]+psk[1]*psk[1]));
		} else {
			x[0]=1/sqrt((psk[3]+(*delta)*psk[2])*psk[3]*2);
			nsk[3]=-psk[1]*x[0];
			nsk[4]=(*delta)*psk[0]*x[0];
		}
		x[0]=psk[3]*psk[3]/(Mu*psk[4])-1;
		x[1]=psk[3]*psk[5]/Mu;
		x[2]=x[0]*x[0]+x[1]*x[1];
		nsk[5]=dargum(psk[6],psk[7]);
		if ( !r1 ) {
			nsk[1]=psk[6]*x[0]+psk[7]*x[1];
			nsk[2]=psk[7]*x[0]-psk[6]*x[1];
		} else {
			nsk[1]=sqrt(x[2]);
			nsk[4]=nsk[5]-dargum(x[0],x[1]);
		}
		x[4]=2*Mu/psk[4]-psk[5]*psk[5]-psk[3]*psk[3]/(psk[4]*psk[4]);
		nsk[0]=Mu/x[4];
		if ( sk < 4 ) {
			x[2]=sqrt(1-x[2]);
			anom(x,x+3);
			nsk[5]-=(dargum(x[3],x[4])+x[1]*x[3]-x[0]*x[4]);
		}
		if ( r2 ) {
			nsk[4]-=nsk[3]*(*delta);
			nsk[5]-=nsk[3]*(*delta);
			if ( !r3 ) nsk[5]-=nsk[4];
		}
		return ;
	} 

	if ( r1 ) x[0]=cos(nsk[2]);
	else  x[0]=(1-2*(nsk[3]*nsk[3]+nsk[4]*nsk[4]))*(*delta);
	if ( *delta==0 ) *delta = (short)Sign2(1,x[0]);
	x[2]=1-nsk[1]*nsk[1];
	if ( !r1 ) x[2]-=nsk[2]*nsk[2];
	x[3]=x[2]*Mu*nsk[0];
	psk[3]=sqrt(x[3]);
	psk[2]=psk[3]*x[0];
	if ( r1 ) {
		x[4]=cos(nsk[3]);
		x[5]=sin(nsk[3]);
		if ( !r2 ) x[5]*=(*delta);
		psk[0]=x[5]*psk[3]*(x[1]=sin(nsk[2]));
		psk[1]=-x[4]*x[1]*psk[3];
	} else {
		x[1]=sqrt((x[0]*(*delta)+1)*2)*psk[3];
		psk[0]=nsk[4]*(*delta)*x[1];
		psk[1]=-nsk[3]*x[1];
	}
	if ( r1 ) {
		x[0]=nsk[5];
		if ( r3 ) x[0]-=nsk[4];
		x[1]=nsk[1]*sin(x[0]);
		x[0]=nsk[1]*cos(x[0]);
	} else  {
		x[0]=(psk[6]=cos(nsk[5]))*nsk[1]+(psk[7]=sin(nsk[5]))*nsk[2];
		x[1]=psk[7]*nsk[1]-psk[6]*nsk[2];
	}
	x[4]=sqrt(x[2]);
	if ( sk < 4 ) {
		kep_rml(x);
		psk[4]=nsk[0]*(1-x[2]);
		psk[5]=x[3]/psk[4]*sqrt(Mu*nsk[0]);
		x[2]=-x[2];
		anom(x+2,x);
		x[2]=nsk[5]+dargum(x[0],x[1])+x[3];
	} else {
		psk[4]=x[3]/(Mu*(1+x[0]));
		psk[5]=Mu/psk[3]*x[1];
		x[2]=nsk[5];
	}
	if ( r2 ) {
		x[2]+=nsk[3]*(*delta);
		if ( !r3 ) x[2]+=nsk[4];
	}
	if ( sk != 4 ) {
		psk[6]=cos(x[2]);
		psk[7]=sin(x[2]);
	}
}

//---------------------------------------------------------------------------
//  ������� �� ���������� � ������������� � �������
//	direct = 0 ask -> psk
//	       = 1 psk -> ask
//	       = 2 psk -> ask ������ ����������
//    ������ ����� �� ���������� ��� �������� �� ����������. � ����
// ������ ��� ����� ��� ����� 0 � ����� ����������� �������������.
// ��� �������� �� ������������� ������ ������ ���� 1 ��� -1 � ��
// ���������� �� ��������� ��������.
//---------------------------------------------------------------------------
void ask_psk(double *ask,double *psk,short direct,short *delta)
{  
	short  i ;
	double x ;

	if ( !direct ) {
		wek_pr(ask,ask+3,psk);
		x=0;
		for (i=3;--i>=0;x+=ask[i]*ask[i]);
		psk[4]=sqrt(x);
		x=0;
		for (i=3;--i>=0;x+=ask[i]*ask[i+3]);
		psk[5]=x/psk[4];
		x=0;
		for (i=3;--i>=0;x+=psk[i]*psk[i]);
		psk[3]=sqrt(x);
		if (*delta==0) *delta=(short)(Sign2(1,psk[2]));
		psk[6]=(ask[0]-ask[2]*psk[0]/(psk[2]+(*delta)*psk[3]))/psk[4];
		psk[7]=(ask[1]-ask[2]*psk[1]/(psk[2]+(*delta)*psk[3]))/psk[4]*(*delta);
		return ;
	}
	
	ask[2]=-(psk[6]*psk[0]+(*delta)*psk[7]*psk[1])*psk[4]*(*delta)/psk[3];
	ask[0]=psk[4]*psk[6]+ask[2]*psk[0]/(psk[2]+(*delta)*psk[3]);
	ask[1]=(*delta)*psk[4]*psk[7]+ask[2]*psk[1]/(psk[2]+(*delta)*psk[3]);
	if ( direct !=2 ) {
		wek_pr(psk,ask,ask+3);
		for (i=3;--i>=0;ask[i+3]=(ask[i+3]/psk[4]+ask[i]*psk[5])/psk[4]);
	}
}

//---------------------------------------------------------------------------
// ���������� ������� ����������� � ��������� �� ���������� �����������
// ������ ����������� ������������� �� ��������� XOZ
//		x    - ���������� X, Y, Z 
//		ad[0]- ������ �����������
//		ad[1]- ��������� 
//		ad[2]- ������ ������-�������
//		dim  - ������������ (0-�������, 1-�������)
void ZASCtoAscensionDecination(double *x, double *ad, bool dim) 
{
	ad[2] = FR(x);
	ad[1] = asin(x[2]/ad[2]);
	ad[0] =	ValiA(dargum(x[0],x[1]), 0);
	if (dim) {
		ad[0] = GradA(ad[0]) ; 
		ad[1] = GradA(ad[1]) ; 
	}
}

//---------------------------------------------------------------------------
