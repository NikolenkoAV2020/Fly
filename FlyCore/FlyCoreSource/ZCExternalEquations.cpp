//-------------------------------------------------------------------------------
// ZCExternalEquations.cpp
// �. ��������� 22.10.2018
// 
// ���������� ������, ����������� �������� ����� � ������������ ������ ��������
// �� ����� ���������� ��������� �������� �������������� (�������) ���������.
// ������ ������� ��������� ����� ��������������� ������ � ��������� �����������
// �������� ��, � �������� ������� ��������� ���������������� ��������� ���
// ��������� �� (x, y, z), ��� ��������� (Vx, Vy, Vz) � ����� ��.
//
// �������� ����� ������� "�����������" ��� �������� ����� ������������ � 
// ����������� �������� ������ ���� ������������� � ��������� �������� ��,
// ���� ���������� �� ��� ���� ���������� �������� �� ��������� ����������
// � ������������������ ��������.
// 
// ��� ���������� ����� ������������� ��������� ���������� ����������� �����������
// ������� ExternalSph.
//
// ����� �����������, ����� ��������� �� ����� ���������� ���������
// ��������� ������ �������� "�������������" - ��������� ���� �������������� 
// ��������, ����������� �������� ��. 
// �������� ������������ ������ ��� ��� ���� ������������ ���������, ������... 
// ��������� ����� ����, �������������� �������� �� ������������ � ����� 
// �������������� � ������ �������� ��������� ����� ����������� ���������.
//
// ����� ����� ��������� ����� �������� ��� ��������� ������������������ ��
// ��� �� ������������� ���� ���������� �������������� �������
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>
#include <FlyCoreSource\\ZCExternalEquations.h>

//-------------------------------------------------------------------------------
void operator << (ZCExternalEquationsDefinition& EED, unsigned int itemID) 
{
	EED.ID.push_back(itemID) ;
}

//-------------------------------------------------------------------------------
ZCExternalEquations::ZCExternalEquations() 
{
	CountExtEquations = 0 ;
	IsOn = false ;
	Xexe = nullptr ;
	Yexe = nullptr ;
	NUexe= nullptr ;
}

ZCExternalEquations::~ZCExternalEquations() 
{
	if (NUexe) delete[] NUexe ; NUexe = nullptr ;
}

int ZCExternalEquations::ExternalSph(double t, double* mainX, double* X, double* Y) 
{ 
	return 0 ; 
}   

int ZCExternalEquations::ExternalForces(double t, double* X, double* Y, int N) 
{ 
	return 0 ; 
}   

void ZCExternalEquations::ExternalEquationsOn (double* NU) 
{ 
	if (CountExtEquations<=0) {
		IsOn = false ; 
		return ;
	}
	IsOn = true ; 
	if (NUexe && CountExtEquations) {
		if (NU)	memcpy(NUexe, NU, CountExtEquations*sizeof(double)) ; 
		else memset(NUexe, 0, CountExtEquations*sizeof(double)) ; 
	}	
}

void ZCExternalEquations::ExternalEquationsOff(double* X) 
{ 
	if (X&&Xexe) memcpy(X, Xexe, GetExternalEquationsCount()*sizeof(double)) ;
	IsOn = false ; 
}

bool ZCExternalEquations::IsExtEqOn() 
{ 
	return IsOn ; 
}

void ZCExternalEquations::SetExternalEquationsCount(int CountExt) 
{ 
	assert(CountExt<=EXTEQ_MAXCOUNT) ;
	CountExtEquations = CountExt ;
	if (NUexe) delete[] NUexe ;
	if (CountExt>0) { 
		NUexe = new double[CountExt] ;
		memset(NUexe, 0, CountExt*sizeof(double)) ;
	} else ExternalEquationsOff() ;
}

int ZCExternalEquations::GetExternalEquationsCount() 
{ 
	return(IsOn ? CountExtEquations:0) ; 
}

//-------------------------------------------------------------------------------

