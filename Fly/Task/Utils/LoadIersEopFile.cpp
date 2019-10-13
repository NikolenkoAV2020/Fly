//-----------------------------------------------------------------------------
// LoadIersEopFile.cpp : 
// �. ��������� 04.04.2019
//
// ���������� ��������� �������� � ����� IERS ������ � ���
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>		// ����������� ���� ������ ��������
#include <FlyMMath.h>		// �������������� �������
#include <FlySofa.h>		// ������� ��������������� ����������
#include <FlyChebyshov.h>	// ������������ ���������� ������� ����������
							// ��������
#include <FlyUrlLoader.h>	// 

//-----------------------------------------------------------------------------
int CallBackLoadFromNet(unsigned int ID, float p)
{
	cout << "\r     " << p << "%    ";
	return 0;
}

//-----------------------------------------------------------------------------
int LoadIersEopFile(
	// ��� ���������� ����� � ��� � ������� IERS. ���� ������ (�� nullptr), ��
	// ��������� duration, for2000, updateStorege �� ����� ������-���� ��������.
	// ��������� ��� ����������� ������� �� ����� IersFileName.
	const char* IersFileName,
	// ������ �� ������� � ����� IERS ����������� ������
	//	0 - ������ ��������� ������ (������� ���)
	//	1 - ������� �������� (������� � 1992 ����)
	//	2 - ���� �������� (������� � 1973 ����)
	char duration = 0, 
	// ������ ��������-������� ������������ ������� �������� ���
	//	0 - ������ 1980 ����
	//	1 - ������ 2000 ����
	bool for2000 = true,
	// ������� ������������� ���������� ������ ��������� ��������� ���
	bool updateStorege = false,
	// ������� ������������� ������������ ������ ������������� ������ 
	// �������� �������� ��� IERS. ����� ����� ������ ����
	// updateStorege = true. 
	bool updateApEst = true,
	// ������� ���������� ��������� ��������� �������.
	bool updateLocMod  = false)
{
	int rc;
	string error;

	//ZCEopManager M1;
	//rc = M1.LoadFromIersFormat("IERS_EOP2000A.txt",
	//	true, true, updateLocMod);
	// return 0;

	// ��������� ������ ������ �� ���
	char FtpSource[6][128] = {
		"https://datacenter.iers.org/data/latestVersion/12_FINALS.DAILY_IAU1980_V2013_0112.txt",
		"https://datacenter.iers.org/data/latestVersion/8_FINALS.DATA_IAU1980_V2013_018.txt",
	    "https://datacenter.iers.org/data/latestVersion/7_FINALS.ALL_IAU1980_V2013_017.txt",
		"ftp://ftp.iers.org/products/eop/rapid/daily/finals2000A.daily",
		"ftp://ftp.iers.org/products/eop/rapid/standard/finals2000A.data",
		"ftp://ftp.iers.org/products/eop/rapid/standard/finals2000A.all"};

	// ��� ���������� ����� � ������� ��� ����������� �� IERS
	string localFileName;

	if (!IersFileName) {
		// ����� ������ ������, ������ �� ���� ��� ������ ��� ��������
		int fi = duration + 3 * for2000;
		char* sourceFileName = FtpSource[fi];

		//---------------------------------------------------------------------
		// ������������ �����, ������������ �� ����������� ����������, 
		// ���������� ����� ���
		localFileName = "IERS_EOP_";
		// ���� �������� ������� �����
		char SDate[16];
		time_t rawtime; time(&rawtime);
		struct tm *ptm = gmtime(&rawtime);
		ptm->tm_year -= 100;
		sprintf(SDate, "%.2d%.2d%.2d", ptm->tm_year, 
				ptm->tm_mon + 1, ptm->tm_mday);
		localFileName += SDate;
		// ��� ������ (���������, ����������� ��� ������)
		switch (duration) {
		case 0: localFileName += "_DAILY"; break;
		case 1: localFileName += "_DATA"; break;
		case 2: localFileName += "_ALL"; break;
		}
		// ���� ����� ������ ������
		localFileName += ".txt";

		//---------------------------------------------------------------------
		cout << sourceFileName << endl;
		rc = LoadFromNetToFile(sourceFileName, (char*)localFileName.c_str(),
							   DIRIERSEOP, false, &error, 1, CallBackLoadFromNet);
		if (rc) {
			cout << "������ �� ���������." << endl << error;
		} else {
			cout << "������ ���������.";
		}
		cout << endl << endl;
		if (rc) return rc;
	} else {
		updateStorege = true;
		localFileName = IersFileName;
	}

	//-------------------------------------------------------------------------
	if (!updateStorege) return 0;
	rc = CEopMng.LoadFromIersFormat((char*)localFileName.c_str(),
									updateApEst, true, updateLocMod);
	return rc;
}

//-----------------------------------------------------------------------------


