//-------------------------------------------------------------------------------
// RGradTime.cpp: implementation of the CRGradTime class.
// А. Николенко 03.08.2018
//-------------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyFormat.h>
#pragma hdrstop

//-------------------------------------------------------------------------------
// Construction/Destruction
CRGradTime::CRGradTime( const double Value,const int measure )
{
	fromDouble(Value,measure);
}

CRGradTime::~CRGradTime()
{
}

bool CRGradTime::GetFields( int * h, int * m, int * s,double* millis) const
{
	bool scode=true;
	double x,y;
	if (((x=(y=fabs(m_Value)))>86400000.)|| (y<0))scode=false;
	//_ASSERT((h!=nullptr)&&(m!=nullptr)&&(s!=nullptr));
	*h = ( int )floor( x / 3600000. ) ; x = x - (*h)*3600000. ;
	*m = ( int )floor( x / 60000. ) ;   x = x - (*m)*60000. ;
	*s = ( int )floor( x  / 1000. ) ;
	if (m_Value<0) {*h=-*h;*m=-*m;*s=-*s;}
	if (millis!=0) *millis = x - *s * 1000.0 ;
	return scode;
}

bool CRGradTime::SetFields(const int h,const int m, const int s,const double millis)
{
	m_Value=(60.*(60.*h+m)+s)*1000.+millis;
	if ((fabs(m_Value)<86400000.)&&(abs(m)<=59)
		&&(abs(s)<=59)&&(fabs(millis)<1))
		return true;else return false;
}

bool CRGradTime::toString( char* stroka) const
{
int h,m,s;
double millis;
char buffer[20];
	if(GetFields(&h,&m,&s,&millis))
	{
		if	(14!=sprintf(buffer,"%4d.%02d.%02d.%3.0lf",
			h,abs(m),abs(s),fabs(millis)))	return false;
//		stroka=buffer;
		return true;
	}
	else return false;
}

bool CRGradTime::fromString( const char* stroka)
{
int h,m,s;
double millis;
	if (sscanf(stroka,"%d.%d.%lf",&h,&m,&millis)!=3)
		return false;
	s=(int) floor(millis);
	millis-=s;
	millis*=1000.;
	if (h<0) {m=-m;s=-s;millis=-millis;}
	return SetFields(h,m,s,millis);
}

double CRGradTime::toDouble(const int measure) const
{
double x=m_Value;
	switch(measure)
	{
	case EDF_RAD:
		x*=(acos(-1.0)/180.0);goto metka;
	case EDF_SSS:
		//x/=24;
        x = x/86400000.0 ;
        break ;
	case EDF_GRAD:
	case EDF_HOUR:
		metka:x/=60;
	case EDF_MIN:
		x/=60;
	default:
	case EDF_SEC:
		x/=1000;
	case EDF_MILLIS:
		break;
	}
	return x;
}

void CRGradTime::fromDouble(const double Value, const int measure)
{
	m_Value = Value;
	switch(measure)
	{
	case EDF_RAD:
		m_Value/=(acos(-1.0)/180.0);goto metka;
	case EDF_SSS:
		//m_Value*=24;
        m_Value *= 86400000.0 ;
        break ;
	case EDF_GRAD:
	case EDF_HOUR:
		metka:m_Value*=60;
	case EDF_MIN:
		m_Value*=60;
	default:
	case EDF_SEC:
		m_Value*=1000;
	case EDF_MILLIS:
		break;
	}
}

CRGradTime::operator double()
{
   return toDouble();
}

//------------------------------------------------------------------------------
bool IsValiTime( int h, int m, double s, int* errfield, bool fromInterval )
{
	int rc = 0 ;
    if ( s < 0 || s >=60 ) rc = 3 ;
    if ( m < 0 || m > 59 ) rc = 2 ; 
	if ( !fromInterval && ( h < 0 || h > 23 ) ) rc = 1 ;
	if ( rc && errfield ) *errfield = rc - 1 ;
    return ( !rc ? 1 : 0 ) ;
}

//------------------------------------------------------------------------------
bool IsValiAngle( int g, int m, double s, int* errfield, bool fromInterval )
{
	int rc = 0 ;
    if ( s < 0 || s >=60 ) rc = 3 ; 
    if ( m < 0 || m > 59 ) rc = 2 ; 
	if ( !fromInterval && ( g < 0 || g > 359 ) ) rc = 1 ;
	if ( rc && errfield ) *errfield = rc - 1 ;
    return ( !rc ? 1 : 0 ) ;
}

//------------------------------------------------------------------------------



