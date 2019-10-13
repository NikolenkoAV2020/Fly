//-------------------------------------------------------------------------------
// RMoment.cpp: implementation of the CRMoment class.
// А. Николенко 03.08.2018
//-------------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyFormat.h>
#pragma hdrstop

//-------------------------------------------------------------------------------
recordPopIAT* CRMoment::tblPopIAT = 0 ;

CRMoment::CRMoment(const int zn)
{
	zona=zn;
}

CRMoment::~CRMoment()
{
}

long CRMoment::SetFields(int Day, int Month, int Year, int Hour, int Min,int Sec, double Millis, int Type)
{
	long scode=0;
	int x;
	try {
	  if ( !date.SetFields( Day, Month, Year ) ) return 1 ;
	  if( ( Hour<0)||(Hour>23)||(Min<0)||(Min>59)||(Sec<0)||(Millis<0)||(Millis>=1000.0)) return 1 ;
	}
	catch(...)
	{
	  return 1 ;
	}
	if(Type==TIME_LT) /* Переводим в UTC */
	{
		x=(int)floor((Min-=zona)/60.);
		Min-=x*60;
		x=(int)floor((Hour+=x)/24.);
		Hour-=x*24;
		date+=x;
		Type=TIME_UTC;
	}
	time.SetFields(Hour,Min,Sec,Millis);
    /* Теперь что-то всегда возвратится */
	if (Type==TIME_UTC)	scode=fromUTC();
	else if(time.m_Value>=86400000.0) scode= 1 ;
	switch(Type)
	{
	case TIME_TDB:
	case TIME_TDT:
		time.m_Value-=32184.0;
	case TIME_UT1:
	case TIME_UTC:
	case TIME_IAT:
	default:
		x=(int)floor(time.m_Value/86400000.);
		if (x!=0)
		{
			time.m_Value-=86400000.*x;
			date+=x;
		}
	}
	return scode;
}

long CRMoment::GetFields(int * Day, int * Month, int * Year,
		int* Hour, int* Min,int* Sec, double* Millis, int Type)
{
	long scode=0;
	CRGradTime x=time;
	CRDate y=date;
	long k;
	switch(Type)
	{
	case TIME_TDT:
	case TIME_TDB:
		if((x.m_Value+=32184.)>=86400000.)
		{x.m_Value-=86400000.;y+=1;}
	case TIME_IAT:
		break;
	case TIME_UT1:
	case TIME_UTC:
	case TIME_LT:
		if ( ( scode = toUTC( &y, &x ) ) < 0 )  return scode ;
	}
	x.GetFields(Hour,Min,Sec,Millis);
	if (*Hour>23) {*Min+=(*Hour-23)*60;*Hour=23;}
	if (*Min>59) {*Sec+=(*Min-59)*60;*Min=59;}
	if (Type==TIME_LT)
	{
		(*Min)+=zona;
		k=(long)floor((*Min)/60.);
		(*Min)-=60*k;
		k=(long)floor((*Hour+=k)/24.);
		(*Hour)-=k*24;
		y+=k;
	}
	if(!y.GetFields(Day,Month,Year))scode = 1 ;
	return scode;
}

long CRMoment::SetEpoha(const double Value, const int Type)
{
	long k=(long)floor(Value);
	date.fromLong(k+2451545);
	time.m_Value=(Value-k)*86400000.;
	return 0;
}

long CRMoment::GetEpoha(double* Value, const int EpohType)
{
	*Value=(date.julDayNumber()-2451545)+
		time.m_Value/86400000.;
return 0;
}

long CRMoment::Put_Zona(const int newValue)
{
	zona=newValue;
	return 0;
}

long CRMoment::Get_Zona(int * Value)
{
	*Value=zona;
	return 0 ;
}

CRMoment CRMoment::operator +(const CRGradTime tm)const
{
return CRMoment(date.julDayNumber(),tm.m_Value+time.m_Value);
}
CRMoment CRMoment::operator -(const CRGradTime tm)const
{
return CRMoment(date.julDayNumber(),time.m_Value-tm.m_Value);
}

CRGradTime CRMoment::operator -(const CRMoment other)const
{
return CRGradTime((date.julDayNumber()-other.date.julDayNumber())*
		86400000.+time.m_Value-other.time.m_Value,EDF_MILLIS);
}

long CRMoment::fromUTC()
{
	long scode;
	register int i;
	double x,y=0;
	scode=loadPopIAT();
	if( scode < 0 ) return scode;
	for(i=0,x=10000;i<scode;i++)
	{
		if(date<tblPopIAT[i].date) break;
		x=tblPopIAT[i].Millis;
	}
	if (i<scode) if((date-tblPopIAT[i].date)==-1)
	/*  корректируем величину суток */	y=tblPopIAT[i].Millis-x;
	if (time.m_Value>=86400000.+y) scode= 1 ;
		else scode=0;
	time.m_Value+=x;
return scode;
}

long CRMoment::loadPopIAT()
{
	static short DataArray[][4]={
		{1,7,1972,11},
		{1,1,1973,12},
		{1,1,1974,13},
		{1,1,1975,14},
		{1,1,1976,15},
		{1,1,1977,16},
		{1,1,1978,17},
		{1,1,1979,18},
		{1,1,1980,19},
		{1,7,1981,20},
		{1,7,1982,21},
		{1,7,1983,22},
		{1,7,1985,23},
		{1,1,1988,24},
		{1,1,1990,25},
		{1,1,1991,26},
		{1,7,1992,27},
		{1,7,1993,28},
		{1,7,1994,29},
		{1,1,1996,30},
		{1,7,1997,31},
		{1,1,1999,32}
	};
	const static int j=sizeof(DataArray)/sizeof(short)/4;
	int i;
	if ( tblPopIAT!=0) return j;
	else if( ( tblPopIAT = new recordPopIAT[ j ] )==0 )	return 1 ;
	for(i=j;--i>=0;)
	{
		short* k=&DataArray[i][0];
		tblPopIAT[i].date=CRDate(k[0],k[1],k[2]);
		tblPopIAT[i].Millis=k[3]*1000.;
	}
	return j;
}

CRMoment::CRMoment(const long Day, const double Millis,
					   const int zn)
{
long x=(long)floor(Millis/86400000.);
time.m_Value=Millis-86400000.*x;
date.fromLong(Day+x);
zona=zn;
}

void CRMoment::ClearTbls()
{
if (tblPopIAT!=0) delete[] tblPopIAT;
tblPopIAT=0;
}

long CRMoment::toUTC(CRDate* dt, CRGradTime* tm)
{
	long scode;
	register int i;
	double x = 0 ;
	scode=loadPopIAT();
	if( scode < 0 ) return scode;
    x = 10000 ;
	for( i = 0 ; i < scode ; i++ )
	{
		if((tblPopIAT[i].date-date)*86400000.>
			(time.m_Value-tblPopIAT[i].Millis)) break;
		x=tblPopIAT[i].Millis;
	}
	(*dt).fromLong(date.julDayNumber()+
		(long)floor((time.m_Value-x)/86400000.));
	if (i<scode)
		if((*dt)>=tblPopIAT[i].date)((*dt)=tblPopIAT[i].date-1);
	/*  корректируем время */
	(*tm).m_Value=86400000.*(date-(*dt))+time.m_Value-x;
return 0;
}
