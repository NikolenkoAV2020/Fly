//////////////////////////////////////////////////////////////////////////////
////    MODEL_PV.C                                                        ////
////    ������� ������������� ���������� �������� �����                   ////
////    ���������: Time_PVZ - ����� ������� ���                           ////
////     �������: Xp - ���������� Xp (���� nullptr - �� ������������)        ////
////              Yp - ���������� Yp (���� nullptr - �� ������������)        ////
////              UT - UT1-UTC       (���� nullptr - �� ������������)        ////
////    ������������ ��������: !=0 - ������                               ////
//////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>
#pragma   hdrstop
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
//---------------------------------------------------------------------------
static bool FlagInitPVZ = 0 ; // ���� ������������� ������ ���

short Model_PVZ( double Time_PVZ, double  *Xp, double *Yp, double *UT )
{
	int ko = 0 ;

	// ��� ���������
	if (Xp != nullptr) *Xp = 0.0;
	if (Yp != nullptr) *Yp = 0.0;
	if (UT != nullptr) *UT = GetLeapSeconds(Time_PVZ);

	/*
	if( !k_Flag_Pvz ) {
		// ��� ���������
		if( Xp != nullptr ) *Xp = 0.0 ;
		if( Yp != nullptr ) *Yp = 0.0 ;
		if( UT != nullptr ) *UT = Get_Delta_UTC_IAT( Time_PVZ );
	}
	// ��� ��������
	if( !FlagInitPVZ ) {
		// ��� �� �������������������

		// �������� ������ ���������� �������� �����
		FlagInitPVZ = true ;
	}

	// ������ ���������� �������� ����� �� �����
	*/
	return 0 ;
}
//---------------------------------------------------------------------------
