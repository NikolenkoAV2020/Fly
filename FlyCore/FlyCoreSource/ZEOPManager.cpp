//-----------------------------------------------------------------------------
// ZEOPManager.cpp
// ��������� ���������� �������� ����� (Eath orientation parameters)
//
//-----------------------------------------------------------------------------
// ��������� ��� ������ ������� �������� ������ ������� ������ ������� 
// ���������� ������ ��������. ��� ������� ������ ZMSC ���������� � ������ 
// � ���� �� ������� ZEOPManager.
// 
// ������ ������� ������ ��������, ��� �� ��� � ����� ���������� ��������� 
// �������� ���������� ���������� ZEOPManager EOPMng. 
//
//
//
//
//
// �. ��������� 06.03.2019
//-----------------------------------------------------------------------------
#include <stdafx.h>

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlySofa.h>		// ������� ��������������� ����������

//-----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <FlyCoreSource\\ZEOPManager.h>
#include <FlyCoreSource\\ZIndex.h>

//-----------------------------------------------------------------------------
ZEOPManager EOPMng;
//ZIndexes    EOPIndex;

//-----------------------------------------------------------------------------
ZEOPManager::ZEOPManager() 
{
	Clear();
	// ����� ������-��������
	PxModelHolder.SetStorageName(MOD_PX_STORAGE_FILE_NAME);
	PyModelHolder.SetStorageName(MOD_PY_STORAGE_FILE_NAME);
	DTModelHolder.SetStorageName(MOD_UT1UTC_STORAGE_FILE_NAME);
	// �������� ������� �� ������� �������� ������ ������� ������
	PxModelHolder.SetTimeFrameType(1);
	PyModelHolder.SetTimeFrameType(1);
	DTModelHolder.SetTimeFrameType(1);
}

//-----------------------------------------------------------------------------
ZEOPManager::~ZEOPManager()
{
	Clear() ;
}

//-----------------------------------------------------------------------------
void ZEOPManager::Clear()
{
	EOP.clear() ;
	memset(&EopLoadReport, 0, sizeof(EopLoadReport));

	IsForecastBuild    = false ;
	lastFindTableIndex =-1 ;
	lastFindMJDLeft    = 0 ;
	lastFindMJDRight   = 0 ;
	LastFindIAT        = 0 ;

	TimeTableFirst = 0 ; 
	TimeTableLast  = 0 ; 
	TimeForecastTo = 0 ; 

	MJDTableFirst = 0 ;
	MJDTableLast  = 0 ;

	// ��� ����� �������� ��������� ������ ��� - ����������� IERS
	IersFileName = EOP_IERS_FILE_NAME;
	// ��� �����-��������� ��� � �������� ���� 
	StorageFileName = EOP_STORAGE_FILE_NAME;
}

//-----------------------------------------------------------------------------
// ��������� ��� �� �������� ���������������� ��������� ����
// ���� �� �������� ���� ��� ��� �� ���������� ������� ��� � ��������� ����� -1
//-----------------------------------------------------------------------------
ZEOPItem EmptyItemEOP = {-1, 0, 0, 0 };
ZEOPItem& ZEOPManager::getItemEOP(long Mjd)
{
	if (!EopLoadReport.CountItems || 
		Mjd < EopLoadReport.fromMjd || 
		Mjd > EopLoadReport.toMjd) return EmptyItemEOP;
	long i = Mjd - EopLoadReport.fromMjd;
	if (i<0 || i >= (long)EOP.size()) return EmptyItemEOP;
	return EOP[i];
}

//-----------------------------------------------------------------------------
// �������� ������������� ������ �� ��������� ��������� - ���������� �����
// ����������� � ��������-������� ������������� ������ �������� ����� IERS.
// ��� �������������� ������� ���������� ������ �����������
// IERS (��������� ������ X,Y � ������� ������������ 
// � �������� UTC1-UTC � ������������)
//
// � ������ ��������� �������� ������� ��������� ��� ���������������
// ���������� �������� ����� ��������� � �������� ����������� ���������
// ������ � �������� ����� UT1 � UTC.
// ������ �������� �� �������� ��������� � ������� ������ � ���������������
// �������� �������� ����������� ���������������. ��� ���� ���������� 
// ����������� ����� ����� ������ ������������� ������������, �� ���� � ��������
// ������� ������ �������� ��-�� ���������� ��� ������������� �� �����.
//
// ������������ � ��������� ���������� ����� � ������� IERS ����������� �������� 
// ����������� ����, ���������� ������ ���������� ������ � ���������� UTC1-UTC, 
// ��� ����������� ������� �������� ���� ������.
//-----------------------------------------------------------------------------
int ZEOPManager::LoadFromIersFormat(bool isNeedSaveToBin)
{
	// ����� ������ ��������
	time_t startTIME = clock();

	// �������� ����� ����������� ���������� � ���
	Clear() ;
	// �������� ����� ��������������� ������� ���
	//EOPIndex.Clear();

	// �������� ���������� ��������� ��� ���������� ���
	ifstream FileTxt;
	FileTxt.open(GetIersFileName(), ios::in);
	if (FileTxt.fail() || FileTxt.eof()) return-1;

	// �������� ��������� ��������� 
	ofstream FileBin;	
	if (isNeedSaveToBin) {
		FileBin.open(GetBinaryFileName(), ios::out | ios::binary);
		if (FileBin.fail() || FileBin.eof()) return-2;
	}

	// ��������� ���������� ��� ����������� �������� �������� ���
	int  n, N, rc=0 ;
	char S[512] ;

	// ���������� ����������� ������� � �����
	FileTxt.getline(S, 512, '\n');
	n = sscanf(S, "%d", &N); if (n != 1) { rc = -1; goto end; }
	if (N<30)    { rc = -2; goto end; } // ������� ���� ������
	if (N>25000) { rc = -3; goto end; } // ������� ����� ������ 

	// ����������� � ������� ��� �������� ��� N �������
	EOP.reserve(N);

	{
	// ��������� ���������� ��� ����������� �������� �������� ���
	char     *s, C, text[64];
	float    F;
	ZEOPItem I;
	DATETIMEUTC UTC;

	// ���� ���������� ����� �� ������� 
	while (1) {
		// ���������� ������ �������
		FileTxt.getline(S, 512, '\n'); s = &S[0];

		// ���������� ����
		memcpy(text, s, 2) ; text[2] = '\0' ; s+=2 ;
		n = sscanf(text, "%d", &UTC.G) ; if (n!=1) { rc =-10 ; goto end ; }
		memcpy(text, s, 2) ; s+=2 ;
		// �������������� ���� �� ������� XX � ������ XXXX
		if (UTC.G>50) UTC.G+= 1900 ;
		else UTC.G+= 2000 ;
		// ���������� ������ � ���
		n = sscanf(text, "%d", &UTC.M) ; if (n!=1) { rc =-10 ; goto end ; }
		memcpy(text, s, 2) ; s+=2 ;
		n = sscanf(text, "%d", &UTC.D) ; if (n!=1) { rc =-10 ; goto end ; }	

		// ���������� MJ
		n = sscanf(s, "%f", &I.MJD) ; if (n!=1) { rc =-11 ; goto end ; }
		s = SkipSpace(s) ;
		s = SkipToSpace(s) ;

		// �������� ������ � ����
		{
		double D, D2 ;  
		rc = iauDtf2d("UTC", UTC.G, UTC.M, UTC.D, UTC.h, UTC.m, 
			 UTC.s+UTC.d, &D, &D2) ;
		// �� ������� JD ��������� MJD
		D = D+D2-ZMJD0 ;
		// ���������� ���� ��������  
		if (fabs(I.MJD-D)>0.001) { rc =-30 ; goto end ; }
		}

		// ���������� ��������� ������� ������� �����
		s = SkipSpace(s) ;
		n = sscanf(s, "%c", &C) ; if (n!=1) { rc =-12 ; goto end ; }
		if (C!='I' && C!='P') { rc =-13 ; goto end ; }
		s = SkipSpace(s) ; s++ ;

		// ���������� Xp
		n = sscanf(s, "%f", &I.Xp) ; if (n!=1) { rc =-14 ; goto end ; }
		// ������� �� ������ � ����������� (������� ����������)
		I.Xp*= 1e3 ; 
		s = SkipSpace(s) ;
		s = SkipToSpace(s) ;

		// �������� Xp - ��� �� �����
		n = sscanf(s, "%f", &F) ; if (n!=1) { rc =-15 ; goto end ; }
		s = SkipSpace(s) ;
		s = SkipToSpace(s) ;

		// ���������� Yp
		n = sscanf(s, "%f", &I.Yp) ; if (n!=1) { rc =-16 ; goto end ; }
		// ������� �� ������ � ����������� (������� ����������)
		I.Yp*= 1e3 ; 
		s = SkipSpace(s) ;
		s = SkipToSpace(s) ;

		// �������� Yp - ��� �� �����
		n = sscanf(s, "%f", &F) ; if (n!=1) { rc =-17 ; goto end ; }
		s = SkipSpace(s) ;
		s = SkipToSpace(s) ;

		// ���������� ��������� ������� ������� �����
		s = SkipSpace(s) ;
		n = sscanf(s, "%c", &C) ; if (n!=1) { rc =-18 ; goto end ; }
		if (C!='I' && C!='P') { rc =-19 ; goto end ; }
		s = SkipSpace(s) ; s++ ;

		// ���������� ���������� UT1 �� UTC
		n = sscanf(s, "%f", &I.UT1mUTC); if (n != 1) { rc = -20; goto end; }
		// ������� �� ������ � �����������
		I.UT1mUTC *= 1e3;

		// ���������� ������ � ������� ����, ���� ��� ���������� � ���� ������
		if (FileBin.is_open()) FileBin.write((const char*)(&I), sizeof(I));

		// ��������� ��������� ��� ��� ���� I.MJD � ����� �������		 
		EOP.push_back(I) ;
		// ��������� �������� ����� � ������
		//EOPIndex.AddValue(I.MJD);

		// ��������� ������ ���� ���� ��� � ���� �� ����� - ������� ��
		// ����� ������ ������������

		// �������� ���������� �������� �������
		if(--N==0) break ;
	}
	}
end: ;
	// ���� ��� �������� ������ ��������� ������, ��� ����� ���� ������ �����
	// ��-�� �� ����������� ������� �����, �� ������ ������� ����������
	// ���������� ���
	if (rc) Clear() ;
	else {
		// ����� ���������� ��������  
		time_t endTIME = clock();
		// ������������ �������� � ������������
		EopLoadReport.Duration  = (long)(endTIME - startTIME);
		// �������� �������� IERS
		EopLoadReport.source    = 0;
		EopLoadReport.CountItems= EOP.size();
		EopLoadReport.fromMjd   = (long)EOP[0].MJD;
		EopLoadReport.toMjd     = (long)EOP[EopLoadReport.CountItems - 1].MJD;
	}
	EopLoadReport.ExitCode = rc;

	//�������� ���������� �����, ���� ��� ����������
	if (FileTxt.is_open())	FileTxt.close();
	//�������� ��������� �����, ���� ��� ����������
	if (FileBin.is_open())	FileBin.close();

	return rc ;
}

//-----------------------------------------------------------------------------
int ZEOPManager::LoadFromBin()
{
	// ����� ���������� ��������  
	time_t startTIME = clock();

	unsigned long FaleSize;
	int rc = 0;
	int CountItems, dCI;
	ZEOPItem I;

	// �������� ����� ����������� ���������� � ���
	Clear();
	// �������� ����� ��������������� ������� ���
	//EOPIndex.Clear();

	// ���� ��������� ��������� ��� ���������� ��� 
	ifstream FileBin;

	// �������������� ������������ ������ ����� � ����������� ������� � ��.
	// ��� ����� ��������� ���� � ���������� ������ ������������ � �����
	// �����. ��������� ������� �������� ��������� ������ � ��� ����� 
	// ��������� ������ �����.
	// ����� ������ ����� ����� �� ������ ����� ������ (��������� ZEOPItem).
	// ��������� ������� ������� ���� ����������� ������� � �����.
	FileBin.open(GetBinaryFileName(), ios::ate | ios::in | ios::binary);
	// ������ �� ����.
	if (FileBin.fail()) { 
		rc =-1; goto end; 
	}		
	// ������ ����� � ������ - ������� ��������� ������ ���������������
	// �� ���� streamoff � ���� unsigned long.
	FaleSize = static_cast<unsigned long>(FileBin.tellg());
	// ���� ���� ������
	if (!FaleSize) { 
		rc =-2; goto end; 
	}
	// ����� ������� � �����.
	CountItems = FaleSize / sizeof(ZEOPItem);	
	// �������� �������� ������� ����� ������� ����� ������
	dCI = FaleSize % sizeof(ZEOPItem); 
	if (dCI  || CountItems<30) {
		rc =-3; goto end; 
	}

	// ������������� ��������� ������ �� ����� � ������ �����.
	FileBin.seekg(0);

	// ����������� � ������� ��� �������� ��� N �������
	EOP.reserve(CountItems);

	// ���� ���������� ����� �� ������� 
	while (1) {
		// ������ ����� ������
		FileBin.read((char*)(&I), sizeof(I)).eof();
		// ��������� ��������� ��� ��� ���� I.MJD � ����� �������		 
		// �������������� ��� ���������� ������ ������ � ������� ������������,
		// ���������� UTC �� UTC1 - � ������������.
		if (0) {
			// ���������� �������� ��������� ������ �� ������������
			rc =-4;
			break;
		}
		// ���������� ������ ���������� � ������.
		EOP.push_back(I);
		// ��������� �������� ����� � ������
		//EOPIndex.AddValue(I.MJD);
		// �������� ���������� �������� �������, ��� ��������� �������
		// ���������
		if (--CountItems == 0) break;
		// ��������� ����� �����
		if (FileBin.eof()) {
			if (CountItems > 0) {
				// ������ ��� ���������� �������� ������ � 
				// ���������� ����� ������ ���� �� ������
				rc =-3;
			}
			break;
	}	}
end:;
	// ���� ��� �������� ������ ��������� ������, ��� ����� ���� ������ �����
	// ��-�� �� ����������� ������� �����, �� ������ ������� ����������
	// ���������� ���
	if (rc) Clear();
	else {
		// ����� ���������� ��������  
		time_t endTIME = clock();
		// ������������ �������� � ������������
		EopLoadReport.Duration  = (long)(endTIME - startTIME);
		// �������� �������� �������� ���������
		EopLoadReport.source    = 1;
		EopLoadReport.CountItems= EOP.size();
		EopLoadReport.fromMjd   = (long)EOP[0].MJD;
		EopLoadReport.toMjd     = (long)EOP[EopLoadReport.CountItems - 1].MJD;
	}
	EopLoadReport.ExitCode = rc;

	//�������� ��������� �����, ���� ��� ����������
	if (FileBin.is_open())	FileBin.close();

	return rc;
}

//-----------------------------------------------------------------------------
int ZEOPManager::Load()
{
	// ������� �������� �� ��������� ������
	int rc = LoadFromBin(); if (!rc) return 0;
	// ���� ������ ������ ��� (-1), ������ �������� ����� (-2), 
	// ������������ ������ (-3), �������� ������ ������ (-4), 
	// �� ��������� ������ �� ���������� ����� �
	// ������� IERS. ������������ ������ ������ �������� ����� ������.
	if (rc ==-1 || rc ==-2 || rc ==-3 || rc ==-4) {
		rc = LoadFromIersFormat(true);
	}
	return rc;
}

//-----------------------------------------------------------------------------
// ��������� � ��������� � ��������� ��������� �������������� �������
// �� ������ ��� ����������� �� �������� ��������� � ���� ���������� �������
// ��������� ������� �������� ��� ��������� ��������� ������� ��������.
// ��������� ������� ������������ ����� �������� ���� � ������� ���������������
// �������� ������������ �������������� ���������. 
// ������ ��� ���������� ��� ��� ���� �������������� ������ 
// �������� MJD ������ ������� ����������� ������ ������
int ZEOPManager::MakeAllEopModels(double fromTa, double toTa)
{
	if (!IsLoadStorage()) {
		EopModelBuildReport.ExitCode =-1;
		return-1;
	}

	// ����� ���������� ���������� �������  
	time_t startTIME = clock();
	memset(&EopModelBuildReport, 0, sizeof(EopModelBuildReport));

	double fromMjd;  ZFTaToMJD(fromTa, &fromMjd);
	double toMjd;    ZFTaToMJD(toTa, &toMjd);
	// ������������ ����������
	double  frame = PxModelHolder.GetTimeFrame();
	// MJD ������ ����������
	double  mjdb  = PxModelHolder.FindTimeFrameBeg(fromTa, false);
	// MJD ����� ����������
	double  mjde;
	// MJD ������ ������� ����������� ������ ��������� ������ �
	// ����� ������� ����������� ��������� ��������� ������
	double  rmjdb = 999e99; 
	double  rmjde =-999e99;
	// ������� ���������� ����� ���������� �������
	bool    needBreak = false;
	// ����������� �������� �������� 
	int	    M = (int)frame + 2;
	// �������� �������: �������� � �������� ���������� ������� 
	double* T = new double[M];
	double* Y = new double[M];
	int     rc = 0;

	//-------------------------------------------------------------------------
	while (1) {
		mjde = mjdb + frame;
		// �������� ������ ������� ��������� ��� ������� ���������� ��������� 
		// ������.
		if (mjde > toMjd) {
			mjde = toMjd;
			needBreak = true;
		}

		//---------------------------------------------------------------------
		// �������� ������������ ��������� ����
		if (mjdb > EopLoadReport.toMjd) {
			// ���� ������ ���� �� ���������� � ��������� ���,
			// �� ���������� ������ ���������� ==> ���������� �����
			break;
		}
		if (mjde > EopLoadReport.toMjd) {
			mjde = EopLoadReport.toMjd;
			needBreak = true;
		}
		if (mjdb < EopLoadReport.fromMjd) {
			// ���� ������ ���� �� ���������� � ��������� ���,
			// �� ���������� ������� � ������ ����� �� ��������� ���
			mjdb = EopLoadReport.fromMjd;
			mjde = mjdb + frame;
			if (mjde > EopLoadReport.toMjd) {
				mjde = EopLoadReport.toMjd;
				needBreak = true;
		}	}

		//---------------------------------------------------------------------
		// ���������� ��������� �������������� ������
		int jn = (int)(mjdb - EopLoadReport.fromMjd);
		int jk = (int)(mjde - EopLoadReport.fromMjd);
		int i, j;
		int N = jk - jn + 1;
		assert(N <=M);
		// �������� ������������ ����������� ����� ��� ���������� 
		// �������������� ������
		if (N < 30) {
			break;
		}
		// ������������ ������� ��������� � ������� ���������� ������ X
		for (j = jn; j <= jk; j++) {
			i = j - jn;
			T[i] = EOP[j].MJD - mjdb;
			Y[i] = EOP[j].Xp;
		}
		// ���������� ������ ��� ���������� ������ X
		rc = PxModelHolder.BuildNewMod(T, Y, N); if (rc) break;

		// ������������ ������� ���������� ������ Y
		for (j = jn; j <= jk; j++) {
			i = j - jn;
			Y[i] = EOP[j].Yp;
		}
		// ���������� ������ ��� ���������� ������ Y
		//rc = PyModelHolder.BuildNewMod(T, Y, N); if (rc) break;

		// ������������ ������� �������� UT1-UTC
		for (j = jn; j <= jk; j++) {
			i = j - jn;
			Y[i] = EOP[j].UT1mUTC;
		}
		// ���������� ������ ��� �������� UT1-UTC
		//rc = DTModelHolder.BuildNewMod(T, Y, N); if (rc) break;

		// ����������� ������� ��������� �������
		EopModelBuildReport.Count++;
		if (rmjdb > mjdb) rmjdb = mjdb;
		if (rmjde < mjde) rmjde = mjde;

		//---------------------------------------------------------------------
		// ���������� ����� ��� ������� � ���������� ���������� ������� ���
		// � � ������������ ��������� �������������� ������
		if (needBreak) break;
		mjdb = PxModelHolder.FindTimeFrameBeg(mjde, true);
	}

	EopModelBuildReport.ExitCode = rc;
	if (!rc) {
		// ���������� ����������� ������� � ��������������� ���������
		//PxModelHolder.SaveToStorage();
		//PyModelHolder.SaveToStorage();
		//DTModelHolder.SaveToStorage();

		// ����� ���������� ���������� �������  
		time_t endTIME = clock();
		// ������������ �������� � ������������
		EopModelBuildReport.Duration   = (long)(endTIME - startTIME);
		// �������� �������� �� ������� ���������� ������� ����� 
		// �������
		EopModelBuildReport.fromTaNeed = fromTa;
		EopModelBuildReport.toTaNeed   = toTa;
		// ����������� �������� ������� �� ������� ������� ������� �����������
		// ����� �������������� �������
		ZFMJDToTa(rmjdb, &EopModelBuildReport.fromTa);
		ZFMJDToTa(rmjde, &EopModelBuildReport.toTa);
	}

	delete[] T;
	delete[] Y;
	return rc;
}

//-----------------------------------------------------------------------------
char* ZEOPManager::SkipSpace(char* s) 
{
	while (*s==' ') {
		if (*s=='\n' || *s=='\r' || *s=='\0') break ;
		s++ ;
	}
	return s ;
}

//-----------------------------------------------------------------------------
char* ZEOPManager::SkipToSpace(char* s) 
{
	while (*s!=' ') {
		if (*s=='\n' || *s=='\r' || *s=='\0') break ;
		s++ ;
	}
	return s ;
}

//-----------------------------------------------------------------------------
// �������� ��������� �� ��������� ����������
bool ZEOPManager::IsLoadStorage()
{
	return (EOP.size()?true:false) ;
}

//-----------------------------------------------------------------------------
// ���������� ��� ���� ������� ������������ �� �����������
// ������������� ������, ���� �� ����������� ��������������� ������.
int ZEOPManager::Calc(double t, ZEOPItem& P, TIMESCALE Scale)
{
	int rc = 0;
	rc = PxModelHolder.Calc(t, P.Xp); 
	// �������� ������������� �������� ��������� ��� � ������������
	// ����� ��������� ������ ���������� ��� ������� t.
	if (rc == 100) {
		// �������� ��������� ���
		double *T, *Y;
		int     N;

		// �������� ����� ��������� �������
		rc = PxModelHolder.BuildNewMod(T, Y, N); if (rc) return rc;
		rc = PyModelHolder.BuildNewMod(T, Y, N); if (rc) return rc;
		rc = DTModelHolder.BuildNewMod(T, Y, N); if (rc) return rc;

		// ��������� ������� ����������
		rc = PxModelHolder.Calc(t, P.Xp);
		if (rc) {
			assert(0);
		}
	}
	if (rc) return rc;
	rc = PyModelHolder.Calc(t, P.Yp); if (rc) return rc;
	rc = DTModelHolder.Calc(t, P.UT1mUTC); if (rc) return rc;
	return 0 ;
}

//-----------------------------------------------------------------------------
// ���������� ���������� ��������������� ������ �� ����������� ������.
int ZEOPManager::BuildForecast(double toTime, TIMESCALE Scale)
{

	return 0 ;
}

//-----------------------------------------------------------------------------
// ���� ��������� ������ �� ���������, �� ��� 
// ��� ������� GetTimeXXX ���������� ������� ��������
// ����� ������ ������� ��� - ����� ������ ������� ��������� ������
double ZEOPManager::GetTimeFirst(TIMESCALE Scale)
{
	return (double)TimeTableFirst ;
}

//-----------------------------------------------------------------------------
// ����� ����� ������� ��� - ����� �� �������� ��������� ���������������
// ������ ��� ����� ����� ������� ��������� ������, ���� ���������������
// ������ �� ���������
double ZEOPManager::GetTimeLast(TIMESCALE Scale)
{
	return (double)(IsForecastBuild ? TimeForecastTo:TimeTableLast) ;
}

//-----------------------------------------------------------------------------
// ����� ����� ������� ��������� ������
double ZEOPManager::GetTimeTable(TIMESCALE Scale)
{
	return (double)TimeTableLast ;
}

//-----------------------------------------------------------------------------
// ��������������� ���, ���������� ��������������� ������.
int ZEOPManager::Forecast(double t, TIMESCALE Scale) 
{



	return 0 ;
}

//-----------------------------------------------------------------------------