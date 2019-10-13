//-----------------------------------------------------------------------------
// Fly.cpp : 
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>		// �������������� �������
#include <FlySofa.h>		// ������� ��������������� ����������
#include <FlySsp.h>			// ��������� ��������� "������ SSP"
#include <FlyChebyshov.h>	// ������������ ���������� ������� ����������
							// ��������
#include <FlyCore.h>		// ����������� ���� ������ ��������
#include <FlyCoreLink.h>	// ����������� ���� ����������� ���������
#include <FlyMagicLNS.h>	// ����� ������� �����

#include <FlyUrlLoader.h>		// 
#include <FlyUrlLoaderLink.h>	//

//-----------------------------------------------------------------------------
// ������� �������� ������������ ������ �������� ��
//-----------------------------------------------------------------------------
int TaskFlightTrack();			// ������ �����
int TaskCalcZRV();				// ������ ��� � �� ��� ���
int TaskTimePrediction();		// ������� � �������� ����� �� �������
int TaskTurnsPredictionSimpl();	// ����� ������� ������� �� ������
int TaskTurnsPredictionGroup();	// ������� �� ������ ������ �� 8-� ��
int TaskTurnsPredictionHAvr();	// ��������������� �� ������ � ����������� 
								// ���������� �� ����� ��������� ��������
//-----------------------------------------------------------------------------
// ������ ���������� ������������ �������������� ��� 1-3 ����������
// ������� ������� ������� ����� ��� ������� ����� ���������� ����
// SPO_ORB_OMIV
// SPO_ORB_DU
// SPO_ORB_EL
// SPO_ORB_HBH
// ������� �� ��������� ���������: 11, 12, 13, ... 20, 21, ..., 30, 31, 32
//-----------------------------------------------------------------------------
int TaskMagicFly11 (int TypeOrb);
int TaskMagicFly21 (int TypeOrb);
int TaskMagicFly24 (int TypeOrb);
int TaskMagicFly241(int TypeOrb);
int TaskMagicFly31 (int TypeOrb);
int TaskMagicFly311(int TypeOrb);

int TaskMagicFlySys3SC(int TypeOrb);

//-----------------------------------------------------------------------------
// ������
int MatchingPrediction();
int TestAgeValidation();

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// ����� �������� � ���������� ���������� 
	//setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// ������������� ���������� ����������
	// �������������� ��������, ��� ��� ������ ������� ����������
	// ��� �������� ������� v����� �������� �� (ZMSC)
	FlyGlobalInit();

	// ����� ������� ������� �� ������
	//TaskTurnsPredictionSimpl();
	//return 0;

	ZNU NU;
	//LoadNU01("NU01_prognoz_000.txt", nullptr, nullptr, NU, true);
	//LoadNU01("NU01_268_Sverka_4_0005.txt", nullptr, nullptr, NU, true);
	//LoadNU50(".\\������ 4\\prognoz_000.txt", nullptr, nullptr, NU, true);

	// ������������ ��������� � ������� ����� ��
	TestAgeValidation();
	//return 0;
	// ������ ����������� ���������������
	//MatchingPrediction();

	// �������������� �����
	TaskMagicFly11(SPO_ORB_HBH);

	// �������������� �����
	TaskMagicFly21(SPO_ORB_HBH);

	// �������������� �����
	// ��� �������
	TaskMagicFly24(SPO_ORB_HBH);

	// �������������� �����
	// ��� ��������� ������
	TaskMagicFly241(SPO_ORB_HBH);

	// ������������� �����
	// ��� �������
	TaskMagicFly31(SPO_ORB_HBH);

	// ������������� �����
	// ��� ��������� ������
	TaskMagicFly311(SPO_ORB_HBH);

	// ��������� ������� ��
	// ��� ��������� ������
	TaskMagicFlySys3SC(SPO_ORB_HBH);

	// �������� ��������� ��� 
	//BuildEOPStorage();
	// ���� ���������� ��� 
	//TestEOPCalc();

	// ������ �����
	TaskFlightTrack();

	// ������� � �������� ����� �� �������
	TaskTimePrediction();

	// ����� ������� ������� �� ������
	TaskTurnsPredictionSimpl();

	// ������� �� ������ ������ �� 8-� ��
	TaskTurnsPredictionGroup();

	// ��������������� �� ������ � ����������� 
	// ���������� �� ����� ��������� ��������
	TaskTurnsPredictionHAvr();

	// ������ ��� � �� ��� ���
	TaskCalcZRV();

	return 0;
}

//-----------------------------------------------------------------------------
