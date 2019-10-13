// ZCAlgebraicLocalModFactory.cpp
// �. ��������� 05.04.2019
//
//-----------------------------------------------------------------------------
#include <stdafx.h>

#include <math.h>
#include <iostream>
#include <fstream>

#include <FlyCoreSource\\ZCStorageBinaryFile.h>

#define MaxArrayKeySizeL 250

class ZCArrayKey {
public:
	ZCArrayKey();
	~ZCArrayKey();

	void Clear ();
	void Init  (int size);
	void SetOn (int i);
	void SetOff(int i);
	bool Get   (int i) const;
	int  GetMaxSizeArrey() const;

protected:
	int SizeArray;
	unsigned long K[MaxArrayKeySizeL];
};

ZCArrayKey::ZCArrayKey()
{

}

ZCArrayKey::~ZCArrayKey()
{

}

void ZCArrayKey::Clear()
{

}

void ZCArrayKey::Init(int size)
{
	memset(K, 0, sizeof(unsigned long)*MaxArrayKeySizeL);
	unsigned long I = 0;
	I = 1;
	I = 1 << 2;
}

void ZCArrayKey::SetOn(int i)
{

}

void ZCArrayKey::SetOff(int i)
{

}

bool ZCArrayKey::Get(int i) const
{
	return true;
}

int ZCArrayKey::GetMaxSizeArrey() const
{
	return 8*sizeof(unsigned long)*MaxArrayKeySizeL;
}

//-----------------------------------------------------------------------------
class ZCAlgebraicLocalModFactory {
public:
	ZCAlgebraicLocalModFactory();
	~ZCAlgebraicLocalModFactory();

	int    ReStartFromTime(double t, bool forMjd);
	int    PushValue(double y, double t, bool forMjd);

	void   SetTF(int month);
	unsigned long GetTFSize() const;
	unsigned long GetTFBeg() const;
	unsigned long GetTFEnd() const;

protected:
	double dFindTFBeg(double t, bool forMjd);
	unsigned long lFindTFBeg(double t, bool forMjd);

	// ���������� ������������ ����-������
	// 0 - �����
	// 1 - �������
	// 2 - ���������
	// 3 - ���
	char TimeFrameType;

	unsigned long fromMjd;
	unsigned long toMjd;

	vector<double> T;
	vector<double> Y;
};

//-----------------------------------------------------------------------------
ZCAlgebraicLocalModFactory::ZCAlgebraicLocalModFactory()
{
	TimeFrameType = 1;
	fromMjd = 0;
	toMjd = 0;
}

//-----------------------------------------------------------------------------
ZCAlgebraicLocalModFactory::~ZCAlgebraicLocalModFactory() 
{
	T.clear();
	Y.clear();
}

//-----------------------------------------------------------------------------
void ZCAlgebraicLocalModFactory::SetTF(int month)
{
	switch (month) {
		case 1: TimeFrameType = 0; break;
		case 3: TimeFrameType = 1; break;
		case 6: TimeFrameType = 2; break;
		case 12:TimeFrameType = 3; break;
		default: assert(0);
	}
}

//-----------------------------------------------------------------------------
unsigned long ZCAlgebraicLocalModFactory::GetTFSize() const
{
	switch (TimeFrameType) {
		case 0: return 32;
		case 1: return 93;
		case 2: return 184;
		case 3: return 367;
		default: assert(0);
	}
	return 32;
}

unsigned long ZCAlgebraicLocalModFactory::GetTFBeg() const
{
	return fromMjd;
}

unsigned long ZCAlgebraicLocalModFactory::GetTFEnd() const
{
	return toMjd;
}

//-----------------------------------------------------------------------------
// ���������� ������� ������ ���� ������, �������� ������������ (TimeFrameType),
// ����������� ������ ������� t, �������� ���� ���������������� ��������� 
// ����� (fromMjd=true), ���� �������� � ��� (fromMjd=false).
// ��������� - �����. ��������� ���� ������ ����-������.
double ZCAlgebraicLocalModFactory::dFindTFBeg(double t, bool forMjd)
{
	// MJD � ������� �������� ����� 
	TheDblTime  dMjd;
	// ����������� ���� � ����� � UTC ��� ��������� ������� ������� t
	DATETIMEUTC UTC;
	if (forMjd) {
		// ����� ������ ��� MJD � ������� ������ �����
		dMjd = t;
		ZFMJDtoDMYH(dMjd, &UTC);
	}
	else {
		// ����� t ������ ��� ������� ����� - ����� ���������� � ���,
		// ���������� ������� � ������������� �� ����� ������ �������
		// ������� (0h 0m 0s 01.01.1975)
		UTC << t;
	}
	// � ����������� �� ���� � �������� ������ ������ ��������� ����
	// 01 ������ �������� ����, ���� 01 ���� �������� ����.

	if (TimeFrameType == 0) {
		// �� �������
	} else {
		if (TimeFrameType == 1) {
			// �� ���������
			if (UTC.M > 9) UTC.M = 10;
			else {
				if (UTC.M > 6) UTC.M = 7;
				else {
					if (UTC.M > 3) UTC.M = 4;
					else {
						UTC.M = 1;
		}	}	}	}
		else {
			if (TimeFrameType == 2) {
				// ���������
				UTC.M = UTC.M < 7 ? 1 : 7;
			} else {
				if (TimeFrameType == 3) {
					// ���
					UTC.M = 1;
				} else {
					assert(0);
	}	}	}	}
	UTC.D = 1;
	UTC.h = UTC.m = UTC.s = 0; UTC.d = 0.0;
	// ����������� ������� � MJ
	ZFDMYHMStoMJD(UTC, &dMjd);
	return dMjd.LikeDouble();
}

unsigned long ZCAlgebraicLocalModFactory::lFindTFBeg(double t, bool forMjd)
{
	double Td = dFindTFBeg(t, forMjd);
	unsigned long Tl = (unsigned long)Td;
	// �������� ��������� ������ ����������
	double dT = Td - (double)Tl;
	if (1.0 - dT < 1e-12) ++Tl;
	// ������ ������ ����-������
	return Tl;
}

//-----------------------------------------------------------------------------
int ZCAlgebraicLocalModFactory::ReStartFromTime(double t, bool forMjd)
{
	unsigned long Days = GetTFSize();
	fromMjd = lFindTFBeg(t, forMjd);
	toMjd = fromMjd + Days;

	T.resize(0);
	Y.resize(0);
	T.reserve(Days + 1);
	Y.reserve(Days + 1);

	return 0;
}

//-----------------------------------------------------------------------------
int ZCAlgebraicLocalModFactory::PushValue(double y, double t, bool forMjd)
{
	double Mjd; 
	if (forMjd) Mjd = t;
	else {


	}
	int j = (int)Mjd - (int)fromMjd;
	Y[j] = y;
	T[j] = Mjd - (double)fromMjd;

	return 0;
}
