//-----------------------------------------------------------------------
// ModGlobal.cpp
// �. ��������� 03.08.2018
//-----------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop

//-----------------------------------------------------------------------
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
//------------------------------------------------------------------------------
//FLYCORE_API char DIRFLY[512]        = ".\0";
//FLYCORE_API char DIRFLYINFO[512]    = ".\\ModInfo\0";
//FLYCORE_API char DIREGF[512]        = ".\\ModInfo\\EGF\0";
//FLYCORE_API char DIRIERSEOP[512]    = ".\\ModInfo\\EopIers\0";
//FLYCORE_API char DIREOPSTORAGE[512] = ".\\ModInfo\\EopStorage\0";
//FLYCORE_API char DIRRESULT[512]     = ".\\_����������\0";

FLYCORE_API char DIRFLY[512]        = "";
FLYCORE_API char DIRFLYINFO[512]    = "";
FLYCORE_API char DIREGF[512]        = "";
FLYCORE_API char DIRIERSEOP[512]    = "";
FLYCORE_API char DIREOPSTORAGE[512] = "";
FLYCORE_API char DIRRESULT[512]     = "";
FLYCORE_API char DIRUOF[512]        = "";

int FlyGlobalInit()
{
	char Buf[1024];
	int  rc;

	if (!strlen(DIRFLY)) {
		rc = GetEnvironmentVariable("FLY_PATH", Buf, sizeof(Buf));
		if (!rc) sprintf(DIRFLY, ".\0");
		else	 sprintf(DIRFLY, "%s\0", Buf);
	}

	if (!strlen(DIRRESULT)) {
		//rc = ExpandEnvironmentStrings("%FLY_RESULT1%\\���", Buf, sizeof(Buf));
		rc = GetEnvironmentVariable("FLY_RESULT", Buf, sizeof(Buf));
		if (!rc) sprintf(DIRRESULT, ".\\_����������\0");
		else	 sprintf(DIRRESULT, "%s\0", Buf);
	}

	if (!strlen(DIRUOF)) {
		rc = GetEnvironmentVariable("FLY_UOF", Buf, sizeof(Buf));
		if (!rc) sprintf(DIRUOF, ".\\FlyUof\0");
		else	 sprintf(DIRUOF, "%s\0", Buf);
	}

	if (!strlen(DIRFLYINFO)) {
		rc = GetEnvironmentVariable("FLY_MODINFO", Buf, sizeof(Buf));
		if (!rc) sprintf(DIRFLYINFO, ".\\FlyModInfo\0");
		else	 sprintf(DIRFLYINFO, "%s\0", Buf);
		sprintf(DIREGF,        "%s\\EGF\0", DIRFLYINFO);
		sprintf(DIRIERSEOP,    "%s\\EopIers\0", DIRFLYINFO);
		sprintf(DIREOPSTORAGE, "%s\\EopStorage\0", DIRFLYINFO);
	}

	// ������������� �������� ��� ���������� ������� ������
	// ����������� UTC-IAT
	// C:\A\CPP\FlyProject\_FlyExecut\FlyModInfo 
	SetLeapSecondsPath(DIRFLYINFO);
	rc = LeapSecondsInit(true); if (rc) return 1;

	return 0;
}

//vector<string>	ErrorsMsg ;
//vector<string>	WarningsMsg ;
/*
const double DTPCor1975 = 9131.;				 // ���� 1 ������ 1975 ���� �� ��������� � 1 ������ 2000 ����

const double k_mu_kms = 398600.44;				 //  �������������� ����������,           [��**3/c��**2]												 
const double k_b0     = +2.97553634058240e+03;   //  �������������� ����������,           [����**3/ccc**2]
const double k_c20    = -1.08262570000000e-03;   //  �����.���������� ���������� �����,   [����**2/ccc**2]
const double k_omega  = +6.30038809698000e+00;   //  ������� �������� �������� �����,     [���/ccc]
const double k_rcp    = +6.37100000000000e-01;   //  ������� ������ �����,                [����]
const double k_a0     = +6.37813600000000e-01;   //  ������� ������� ��� ,                [����]
const double k_alfa0  = +3.35282440000000e-03;   //  �������� ������ ��� ,
const double k_alfak  = +3.35232986926000e-03;   //  �������� ������ �� ,
const double k_ak     = +6.37824500000000e-01;   //  ������� ������� �� ,                 [����]
const double k_ekb    = +6.69342162180000e-03;   //  ������� ��������������� ��,
const double k_gekw   = +7.32062499840000e+03;   //  ��������� ���� ������� �� ��������,  [����/ccc**2]
const double k_g0     = 9.80665;                 //  ��������� ���� ������� �� ��������,  [�/c**2]
const double k_eps    = 1.96572649767644139 ;	 //	 = 1.5*1,31048433178429426  ��**5/ ���**2

const double k_dx     = +2.50000000000000e-06;   //  �������� ������ �� ���.������ ��� ,  [����]
const double k_dy     = -1.41000000000000e-05;   //              --
const double k_dz     = -8.00000000000000e-06;   //              --
const double k_cfak   = +2.59020683712400e+06;   //  �������� ����� �����������,          [����/ccc]
const double k_cnom   = +2.59200000000000e+06;   //  �������� ����� �����������,          [����/ccc]
const double k_tmt    = +2.73790926300000e-03;   //  ������������ �������� �� ���������� ������� �
const double k_tmf    = +2.73781190600000e-03;   //    ��������� � ������ ������������� ����� �����
const double k_rekw   = +6.37816000000000e-01;   //  �������������� ������ �����,         [����]
const double k_threeh = +1.25000000000000e-01;   //  3 ����,                              [ccc]
const double k_pi     = M_PI;        // +3.14159265358979e+00;   //  ����� ��
const double k_polpi  = M_PI/2.0;    // +1.57079632679000e+00;   //  ����� ��/2
const double k_dwapi  = M_PI*2.0;    // +6.28318530717000e+00;   //  ����� ��*2
const double k_t2pi   = M_PI*1.5;    // +4.71238897037000e+00;   //  ����� ��*3/2
const double k_cbc    = +8.64000000000000e+04;   //  ������ � ������                      [���]
const double k_cbm    = +1.44000000000000e+03;   //  ����� � ������                       [���]
const double k_e0     = +1.36825000000000e+03;   //  ����. ����. ���.�� ���-�� ����� [(�� ����**2)/(m**2 ccc**3]
const double k_gs     = +9.90693071599657e+08;   //  ����������������� ������. ����.      [����**3/���**2]
const double k_gl     = +3.65992110957681e+01;   //  ������������������ ������. ����.     [����**3/���**2]
const double k_qk     = +8.13005600000000e+01;   //  ��������� ����� ����� � ����� ����   [ - ]
const double k_qk2    = +3.00000000000000e-01;   //  ����� ����                           [ - ]
const double k_ql     = +4.36332312998585e-02;   //  ���� ������������ ��������� �����    [������]
const double k_ae     = +1.49597900000000e+04;   //  ��������������� �������              [����]
const double k_rcp_c  = +6.95980000000000e+01;   //  ������� ������ ������,               [����]
const double k_rcp_l  = +1.73800000000000e-01;   //  ������� ������ ����,                 [����]
const double k_zona   = +1.25000000000000e-01;   //  ������� ���������� ������� �� �������� [���]
const double k_vek    =   21;                    //  ������� ��� (�������  =  20)
const double k_vekcme =   50;                    //  ��� �������� ����� ����
const double k_dwx    = +0.00000000000000e+00;   //  ����������� ��� X �� ������������ ��� [���]
const double k_dwy    = +1.69684788388338E-06;   //  ����������� ��� Y �� ������������ ��� [���]
const double k_dwz    = +3.19977029532295E-06;   //  ����������� ��� Z �� ������������ ��� [���]

const bool k_Flag_Pril =        0;             //  ���� ����� �������� � ����������� ��
const bool k_Flag_Tekt =        0;             //  ���� ����� �������� ������������� ���� � ����������� ��
const bool k_Flag_Pvz  =        0;             //  ���� ������� ��� ��� ��������� ���-���
const bool k_Flag_Pril_Pvz  =   0;             //  ���� ����� ����������� ��������� �������� ���
const double k_g_jupiter = 945906.3002000;     //  ������. ����. �������  [����**3/���**2]
const double k_g_saturn  = 283225.4230000;     //  ������. ����. �������  [����**3/���**2]
const double k_g_uran    =  43256.1020000;     //  ������. ����. �����    [����**3/���**2]
const double k_g_venera  =   2425.0579000;     //  ������. ����. ������   [����**3/���**2]
const double k_g_mars    =    319.7118400;     //  ������. ����. �����    [����**3/���**2]
const double k_g_neptun  =  51034.4830000;     //  ������. ����. �������  [����**3/���**2]
const double k_g_merkurj =    164.4687000;     //  ������. ����. �������� [����**3/���**2]
const double k_g_pluton  =      7.3276157;     //  ������. ����. �������  [����**3/���**2]
// ������� � ��� ����� ����� ������ ������� ������� � ������ 2000 (01.01.2000)
const long   DELTA_2000  = -9131L ;
*/
//---------------------------------------------------------------------------
// ���� ������ ������� �������
//DATE   D1975( 1, 1, 1975 ) ;
//---------------------------------------------------------------------------
// ������� �������� �� J2000 � B1975
static double  Gl_mJ2000_B1975[9] ;
double*        mJ2000_B1975 = Gl_mJ2000_B1975 ;
//---------------------------------------------------------------------------
// M������ �������� �� ����������� ������������ �� � ���
static double  Gl_mgrzab[9] ;
double*        mgrzab = Gl_mgrzab ;
//---------------------------------------------------------------------------
POLE_AE POLEAE ;
POLE_AE* Wae = &POLEAE ;
//---------------------------------------------------------------------------
void ClearErrorsMSG( ) 
{
	//ErrorsMsg.clear( ) ;
	//WarningsMsg.clear( ) ;
}
//---------------------------------------------------------------------------
void AddErrorMSG( char* Msg ) 
{
	//ErrorsMsg.push_back( Msg ) ;
}
//---------------------------------------------------------------------------
void AddWarningMSG( char* Msg ) 
{
	//WarningsMsg.push_back( Msg ) ;
}
//---------------------------------------------------------------------------
int CountErrorsMSG( ) 
{
	//return ( ( int )ErrorsMsg.size( ) ) ;
	return 0 ;
}
//---------------------------------------------------------------------------
int CountWarningsMSG( ) 
{
	//return ( ( int )WarningsMsg.size( ) ) ;
	return 0 ;
}
//---------------------------------------------------------------------------
const char* GetErrorMsg( int i ) 
{
	//if ( i < 0 || i >= ( int )ErrorsMsg.size( ) ) return 0 ;
	//return ( ErrorsMsg[ i ].c_str( ) ) ;
	return "" ;
}
//---------------------------------------------------------------------------
const char* GetWarningMsg( int i ) 
{
	//if ( i < 0 || i >= ( int )WarningsMsg.size( ) ) return 0 ;
	//return ( WarningsMsg[ i ].c_str( ) ) ;
	return "" ;
}
//---------------------------------------------------------------------------