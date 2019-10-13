//-----------------------------------------------------------------------------
// TheDblTime.cpp
// �. ��������� 19.11.2018
// 
// ����� ������� ��������.
// ����������� ���� + ����� � �������� �������� ��� � ��������
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyTime.h>	// ������� ��������������� ����������
#include <FlySofa.h>	// ������� ��������������� ����������

//-----------------------------------------------------------------------------
#include <FlyTimeSource\\TheDblTime.h>

//-----------------------------------------------------------------------------
// ���� � ����� ���������� ��� �������� �������� ����� �� ����
// ������� ����������� ���� � ����������������� ����� UTC
// ��� ���� ���� ��� ������ ������ ������� �������� ��� ����� �� �������
// TheDblTime T ��������� ��������� �������� 0.125 ���
void operator << (DATETIME& DT, TheDblTime& T)
{
	// SOFA CALL
	double fd, h;
	int rc = iauJd2cal(T.days, T.sec / 86400.0, &DT.G, &DT.M, &DT.D, &fd);
	h = 24.0*fd;
	DT.h = (int)floor(h);
	h = (int)(60.0*(h - DT.h));
	DT.m = (int)floor(h);
	h = (int)(60.0*(h - DT.m));
	DT.s = (int)floor(h);
	DT.d = h - DT.s;
}
void operator >> (TheDblTime& T, DATETIME& DT)  { DT << T; }

//-----------------------------------------------------------------------------
void operator << (DATETIMEDMB& DMB, TheDblTime& T) { *((DATETIME*)(&DMB)) << (T + 0.125); }
void operator >> (TheDblTime& T, DATETIMEDMB& DMB) { DMB << T; }

//-----------------------------------------------------------------------------
void operator << (DATETIMEUTC& UTC, TheDblTime& T) { *((DATETIME*)(&UTC)) << T; }
void operator >> (TheDblTime& T, DATETIMEUTC& UTC) { UTC << T; }

//-----------------------------------------------------------------------------
TheDblTime operator - (TheDblTime& t1, TheDblTime& t2)
{
	TheDblTime at(t1);
	at -= t2;
	return at;
}
TheDblTime operator - (TheDblTime& t1, const double& t2)
{
	TheDblTime at(t1);
	TheDblTime at2(t2);
	at -= at2;
	return at;
}

//-----------------------------------------------------------------------------
TheDblTime operator + (TheDblTime& t1, TheDblTime& t2)
{
	TheDblTime at(t1);
	at += t2;
	return at;
}
TheDblTime operator + (TheDblTime& t1, const double& t2)
{
	TheDblTime at(t1);
	TheDblTime at2(t2);
	at += at2;
	return at;
}

//-----------------------------------------------------------------------------
