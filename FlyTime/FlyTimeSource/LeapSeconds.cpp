//---------------------------------------------------------------------------
// LeapSeconds.cpp
// �. ��������� 11.06.2019
//---------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyTime.h>
#include "LeapSeconds.h"

//---------------------------------------------------------------------------
static ZSLeapSeconds DifUtcIat;

static char LeapSecodsPath[512]   = ".\\FlyModInfo";
static char LeapSecodsSource[128] = "UTC_IAT.dat";

//---------------------------------------------------------------------------
// ����������� �������� ����� � ��������� ������ �����������
void  SetLeapSecondsPath(const char* path)
{
	sprintf(LeapSecodsPath, "%s\0", path);
}
// ����������� ����� ����� � ��������� ������ �����������
void  SetLeapSecondsSource(const char* source)
{
	sprintf(LeapSecodsSource, "%s\0", source);
}

//---------------------------------------------------------------------------
// ������� ��� �������� � ����� � ������� � ������� ������� �����������
char* GetLeapSecondsPath()
{
	return LeapSecodsPath;
}

char* GetLeapSecondsSource()
{
	return LeapSecodsSource;
}

//---------------------------------------------------------------------------
// ������������� ������� ������ �����������
//---------------------------------------------------------------------------
int LeapSecondsInit(bool forSureLoad)
{
	if (forSureLoad) {
		// �������������� �������������
		if (DifUtcIat.isInit == true) DifUtcIat.Clear();
		DifUtcIat.isInit = false;
	} else { 
		// ������������� ��� �������������
		if (DifUtcIat.isInit == true) return 0;
	}

	// �������������
	ZSUtcIatItem D;
	ZSUtcIatItem Temp_DELTA_UTC_IAT;
	// ���������� ���������� ������ ������� ��
	D.IatTime = -1.e20;
	D.dUtcIat = 0;
	DifUtcIat.IatList.push_back(D);
	// ������
	FILE* pf = 0;
	int rc = 0;
	char fileName[512];
	sprintf(fileName, "%s\\%s\0", LeapSecodsPath, LeapSecodsSource);
	pf = fopen(fileName, "r");
	if (pf) {
		while (1) {
			D.IatTime = 0;
			D.dUtcIat = 0;
			int rc = fscanf(pf, "%lf    %lf\n", &D.IatTime, &D.dUtcIat);
			if (rc != 2) break;
			D.dUtcIat /= 86400.0;
			DifUtcIat.IatList.push_back(D);
		}
		fclose(pf); pf = 0;
		DifUtcIat.isInit = true;
	}
	// ���������� ���������� ������ ������� �����
	D.IatTime = 1.e20;
	DifUtcIat.IatList.push_back(D);

	// ���������� ������������ ������� (��������� �������)
	DifUtcIat.Cur  = DifUtcIat.IatList.at(DifUtcIat.IatList.size() - 2);
	DifUtcIat.Next = DifUtcIat.IatList.at(DifUtcIat.IatList.size() - 1);

#ifdef _DEBUG
	if (!DifUtcIat.isInit) {
		cout << endl << 
		"    ������ !!!! �� ��������������� ������ ������ ����������� UTC-IAT" 
		<< endl;
	}
	//System.Diagnostics.Debug.WriteLine("    ������ !!!! �� ��������������� ������ ������ ����������� UTC-IAT");
	//System.Diagnostics.Trace.WriteLine("    ������ !!!! �� ��������������� ������ ������ ����������� UTC-IAT");
	//#if DifUtcIat.isInit
	//	#pragma message ("    ������ ������ ����������� UTC-IAT ��������������� �������")
	//#else
	//	#pragma message ("    ������ !!!! �� ��������������� ������ ������ ����������� UTC-IAT")
	//#endif
#else
	if (!DifUtcIat.isInit) {
		MessageBox(nullptr, 
		"������ !!!!\n�� ��������������� ������ ������ ����������� UTC-IAT", 
		"FlyTime", MB_ICONERROR | MB_OK);
	}
#endif

	return 0;
}

//---------------------------------------------------------------------------
// ����������� �������� ������ ����� ����������� �� �������� ������ IAT
// �������� ������� - dTime, ��������� � ���
//---------------------------------------------------------------------------
double GetLeapSeconds(double dTime)
{
	UINT i;
	if (!DifUtcIat.isInit) {
		if (LeapSecondsInit(0)) return 0;
	}
	// �������� ������������ ������������ �������
	if (DifUtcIat.Cur.IatTime  > dTime || DifUtcIat.Next.IatTime <= dTime) {
		// ����� �������, ���������������� ��������� ������� �������
		for (i = 0; i < DifUtcIat.IatList.size() - 1; i++) {
			if (DifUtcIat.IatList.at(i).IatTime <= dTime &&
				DifUtcIat.IatList.at(i + 1).IatTime > dTime) {
				// ������������ ������������ �������
				DifUtcIat.Cur = DifUtcIat.IatList.at(i);
				DifUtcIat.Next = DifUtcIat.IatList.at(i + 1);
				break;
	}	}	}
	// ������� ������� ��������
	return DifUtcIat.Cur.dUtcIat;
}

//---------------------------------------------------------------------------
