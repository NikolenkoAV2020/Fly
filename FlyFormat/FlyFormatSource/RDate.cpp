//-----------------------------------------------------------------------------
// RDate.cpp: implementation of the CRDate class.
// А. Николенко 03.08.2018
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyFormat.h>
#pragma hdrstop

//-----------------------------------------------------------------------------
// Construction/Destruction
CRDate::~CRDate()
{

}

//-----------------------------------------------------------------------------
void CRDate::Clear()
{
	// За ноль принимается 1.01.-4713 Юлианского календаря
	// Соответствует 1.01.2000 Григорианского календаря
    SetFlags( 0 ) ;
	m_JulDay  = 2451545l ;
	flag_Grig = 3 ;
}

//-----------------------------------------------------------------------------
CRDate::CRDate()
{
	Clear( ) ;
}

//-----------------------------------------------------------------------------
CRDate::CRDate(const long julDayNumber)
{
	Clear( ) ;
	m_JulDay = julDayNumber ;
}

//-----------------------------------------------------------------------------
void CRDate::FieldsException(char* txt)
{
	if (txt) throw (txt) ;
	else throw( "Набор полей для даты ошибочен" ) ;
}

//-----------------------------------------------------------------------------
int CRDate::TestFields( bool isExcept  )
{
	register int year = (int)m_Year;
	if ((int)m_Year<0) year++;
	flag_366 = 0;
	if (year % 4 == 0) flag_366 = 1;
	flag_afterGrig = 0;

	switch (flag_Grig) {
		case 0:
			break;
		case 1:
			if (((int)m_Year == 1582) && ((int)m_Month == 10)
			   && ((int)m_Day<15) && ((int)m_Day>4)) {
				if (isExcept) FieldsException();
				return 1;
			}
			if ((long)m_Year * 10000 + (int)m_Month * 100 +
			   (int)m_Day >= 15821015l) {
				flag_afterGrig = 1;
			}
			break;
		case 2:
			if (((int)m_Year == 1918) && ((int)m_Month == 2) && 
			   ((int)m_Day<14)) {
				if (isExcept) FieldsException();
				return 1;
			}
			if ((int)m_Year * 100 + (int)m_Month>191801l) flag_afterGrig = 1;
			break;
		case 3:
			flag_afterGrig = 1;
			break;
		default:
			if (isExcept) FieldsException();
			return 1;
	}

	if ((flag_afterGrig != 0) && (year % 100 == 0) && (year % 400 != 0))
		flag_366=0;
	if (((int)m_Month == 2) && ((int)m_Day>28 + (int)flag_366)) {
		if (isExcept) FieldsException();
		return 1;
    }

	// Считаем 1.01.m_Year по Юлианскому
	m_JulDay = (int)(floor((year + 4715) / 4.0)) + 
			   (year + 4712) * 365;
	// Добавляем Григорианскую поправку
	if (flag_afterGrig != 0) {
		m_JulDay -= (int)(floor((year - 1) / 100.) - 
					floor((year - 1) / 400.) - 2);
	}
	// Добавляем начало невисокосного месяца от начала года
	month_delta = -1;
	if ((int)m_Month<3) month_delta = 1;
	if (((int)m_Month == 9) || ((int)m_Month == 11)) month_delta = 0;
	m_JulDay += (int)month_delta + ((int)m_Month - 1) * 30 + 
				((int)m_Month / 2) - 1;
	// Если год високосный, то для марта и далее
	if ((int)m_Month>2) m_JulDay += (int)flag_366;
	m_JulDay += (int)m_Day - 1;
	flag_FieldsIsSet = 1;

    return 0 ;
}

//-----------------------------------------------------------------------------
int CRDate::IsValiFields( const int day,const int month,const int year )
{
	TestAccess();
	flag_FieldsIsSet = 0;
	if ((year<-9999) || (year>9999) || (year == 0)) return 3;
	m_Year = year;
	if ((month<1) || (month>12)) return 2;
	m_Month = month;
	if ((day<1) || (day>31 - (((int)m_Month - 1) % 7) % 2)) return 1;
	m_Day = day;
	return (TestFields(0));
}

//-----------------------------------------------------------------------------
bool CRDate::SetFields( const int day,const int month,const int year )
{
	TestAccess();
	flag_FieldsIsSet = 0;
	if ((year < -9999) || (year > 9999) || (year == 0)) FieldsException();
	m_Year = year;
	if ((month<1) || (month>12)) FieldsException();
	m_Month = month;
	if ((day<1) || (day>31 - (((int)m_Month - 1) % 7) % 2)) FieldsException();
	m_Day = day;
	TestFields(1);
	if (flag_FieldsIsSet == 1) return true;
	else return false;
}

//-----------------------------------------------------------------------------
CRDate::CRDate(const int& day,const int& month,const int& year)
{
	Clear();
	SetFields(day, month, year);
}

//-----------------------------------------------------------------------------
CRDate::CRDate(const char* stroka)
{
	int x, y, z;
	Clear();
	if (sscanf(stroka, "%d.%d.%d", &x, &y, &z) != 3)  
		FieldsException("Не могу сосчитать поля из строки");
	SetFields(x, y, z);
}

//-----------------------------------------------------------------------------
CRDate CRDate::operator- (const long HowMuch) const
{
	return CRDate((long)m_JulDay - HowMuch);
}

//-----------------------------------------------------------------------------
CRDate CRDate::operator+ (const long HowMuch) const
{
	return CRDate((long)m_JulDay + HowMuch);
}

//-----------------------------------------------------------------------------
CRDate& CRDate::operator+= (const long HowMuch)
{
	TestAccess();
	m_JulDay += HowMuch;
	flag_FieldsIsSet = 0;
	return *this;
}

//-----------------------------------------------------------------------------
CRDate& CRDate::operator-= (const long HowMuch)
{
	TestAccess();
	m_JulDay -= HowMuch;
	flag_FieldsIsSet = 0;
	return *this;
}

//-----------------------------------------------------------------------------
CRDate& CRDate::operator= (const CRDate & other)
{
	TestAccess();
	m_JulDay = (long)other.m_JulDay;
	flag_FieldsIsSet = 0;
	return *this;
}

//-----------------------------------------------------------------------------
bool CRDate::PrepareFields()
{
	// пытаемся угадать год
	register int year;
	register int x;
	flag_FieldsIsSet = 0;
	switch (flag_Grig) {
		case 0:
			flag_afterGrig = 0;
			break;
		case 1:
			if ((long)m_JulDay>2299160l) flag_afterGrig = 1;
			else flag_afterGrig = 0;
			break;
	case 2:
			if ((long)m_JulDay<2421639l) flag_afterGrig = 0;
			else flag_afterGrig = 1;
			break;
	case 3:
			flag_afterGrig = 1;
			break;
	default:
			return false;
	}

	if ((flag_afterGrig == 0) && (((long)m_JulDay<-1930711l) || 
	   ((long)m_JulDay>5373557l)))
		return false;
	if ((flag_afterGrig == 1) && (((long)m_JulDay<-1930634l) || 
	   ((long)m_JulDay>5373484l)))
		return false;
	year = (int)floor((long)m_JulDay / 365.25 - 4711.5);
	if (year<-9998) year =-9998;
	if (year> 9999) year = 9999;
	year++;
	do {
		year--;
		// Считаем 1.01.year по Юлианскому 
		x = (int)(floor((year + 4715) / 4.0)) + (year + 4712) * 365;
		// Добавляем Григорианскую поправку 
		if (flag_afterGrig != 0) {
			x -= (int)(floor((year - 1) / 100.) - floor((year - 1) / 400.) - 2);
		}
	} while (x>(long)m_JulDay);
	// Год найден, теперь флаг високосности
	flag_366 = 0;
	if (year % 4 == 0) flag_366 = 1;
	if ((flag_afterGrig != 0) && (year % 100 == 0) && (year % 400 != 0))
		flag_366 = 0;
	// Устанавливаем год. 0-го года нет.
	if (year <= 0) year--;
	m_Year = year;
	// Число дней от 1.01.m_Year по стилю !! результата !!
	// берем и рассчитываем месяц
	x = (long)m_JulDay - x;
	if ((x - 58)>(int)flag_366) year = 2 - flag_366;
	else year = 0;
	year += 31 + x;
	m_Month = (int)floor(year / 30.57);
	// Считаем 1.m_Month.m_Year-1.01.m_Year
	month_delta = -1;
	if ((int)m_Month<3) month_delta = 1;
	if (((int)m_Month == 9) || ((int)m_Month == 11)) month_delta = 0;
	x -= month_delta + ((int)m_Month - 1) * 30 + ((int)m_Month / 2) - 1;
	// Если год високосный, то для марта и далее 
	if ((int)m_Month>2) x-= (int)flag_366;
	m_Day = x + 1;
	flag_FieldsIsSet = 1;
	return true;
}

//-----------------------------------------------------------------------------
void CRDate::TestAccess()
{
	if (flag_Access) 
	  FieldsException ( "Попытка внесения изменений при флаге защиты" ) ;
}

//-----------------------------------------------------------------------------
bool CRDate::GetFields( int* day, int* month, int* year)
{
	if (flag_FieldsIsSet==0) PrepareFields();
	*day=(int)m_Day;*month=(int)m_Month;*year=(int)m_Year;
	if (flag_FieldsIsSet==0) return false;
	return true;
}

//-----------------------------------------------------------------------------
bool CRDate::GetFlags(int & Flags)
{
	if (flag_FieldsIsSet==0) PrepareFields();
	Flags=(*((long*)(this)))%256;
	if (flag_FieldsIsSet==0) return false;
		else return true;
}

//-----------------------------------------------------------------------------
void CRDate::SetFlags(const int Flags)
{
	*((long*)(this))=(Flags%8);
	if (flag_Access) PrepareFields();
}

//-----------------------------------------------------------------------------
bool CRDate::toString( char* str )
{
	char bufer[ 40 ] ;
	int  x,y,z ;
	if (flag_FieldsIsSet==0) PrepareFields();
	x=(int)m_Day;y=(int)m_Month;z=(int)m_Year;
	if(10!=sprintf(bufer,"%2d.%2d.%04d",x,y,z))	return false;
	sprintf( str, "%s", bufer );
	return true ;
}

//-----------------------------------------------------------------------------
int CRDate::YearDay()
{
	CRDate YearBegining;
	if (flag_FieldsIsSet==0)PrepareFields();
	if (flag_FieldsIsSet==0)FieldsException();
	YearBegining.flag_Grig=flag_Grig;
	YearBegining.m_Year=m_Year;
	YearBegining.m_Month=1;
	YearBegining.m_Day=1;
	YearBegining.TestFields( 1 ) ;
	return (long)m_JulDay-(long)YearBegining.m_JulDay;
}

//-----------------------------------------------------------------------------
bool CRDate::fromString( const char* stroka )
{
	int x,y,z;
	if ( sscanf( stroka,"%d.%d.%d",&x,&y,&z)!=3 ) {
	  FieldsException( "Не могу сосчитать поля из строки" ) ;
	  return false ;
	}
	return SetFields( x, y, z ) ;
}

//-----------------------------------------------------------------------------
void CRDate::fromLong(const long julDay)
{
	TestAccess();
	m_JulDay=julDay;
	flag_FieldsIsSet=0;
}

//-----------------------------------------------------------------------------
bool IsValiDate( int d, int m, int g, int* errfield )
{
    CRDate dmy ;
    int rc = dmy.IsValiFields( d, m, g ) ;
    if ( rc && errfield ) *errfield = rc - 1 ;
    return( rc ? 0 : 1 ) ;
}

//-----------------------------------------------------------------------------
