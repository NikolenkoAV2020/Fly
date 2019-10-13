//-----------------------------------------------------------------------------
// TheJD.cpp
// А. Николенко 12.08.2018
// 
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyTimeSource\\TheJD.h>

TheDblTime DblTime_J2000toJ1975(TheT_JD_J2000toJ1975, -TheT_LEAP_UTC_1975to2000) ;

//-----------------------------------------------------------------------------
void ZFTaToMJD(double ta, double* mjd)
{
	TheDblTime dmjd; ZFTaToMJD(ta, &dmjd);
	*mjd = dmjd.LikeDouble();
}

//-----------------------------------------------------------------------------
void ZFTaToMJD(double ta, TheDblTime* mjd)
{
	DATETIMEUTC UTC; UTC << ta;
	ZFDMYHMStoMJD(UTC, mjd);
}

//-----------------------------------------------------------------------------
void ZFMJDToTa(double mjd, double* ta)
{
	TheDblTime MJD(mjd);
	ZFMJDToTa(MJD, ta);
}

//-----------------------------------------------------------------------------
void ZFMJDToTa(TheDblTime& mjd, double* ta)
{
	DATETIMEUTC UTC; ZFMJDtoDMYH(mjd, &UTC);
	UTC >> *ta;
}

//-----------------------------------------------------------------------------
TheDblTime ZFDMYHtoMJD(int d, int m, int g, double* h, 
				       TheDblTime* mjd, DATETIME* DT)
{
	// Модифицированная юлианская дата (вместе с дробной частью),
	// то есть в том числе часы
	TheDblTime MJD;
	// только дробная часть выраженная в часах
	double JDH;
	// Гражданская (календарная) дата и время
	DATETIMEUTC DTUTC;

	MJD = (g - 1) * 365l + (g - 1) / 4 - (g - 1) / 100 +
		(g - 1) / 400 + 1721425l + (m - 1) * 30 + d - 1;
	if (m>2) {
		MJD -= 2;
		if (g % 4 == 0) {
			MJD += 1;
			if (g % 100 == 0 && g % 400 != 0) MJD -= 1;
		}
	}
	MJD += 0.5;
	JDH = 0;
	if (h) {
		JDH = (*h) / 24.0;
		MJD += JDH;
	}
	else JDH = 0;
	if (m>8) MJD += (m + 1) / 2;
	else     MJD += m / 2;
	MJD -= TheT_MJD_ORIGIN;
	DTUTC.D = d;
	DTUTC.M = m;
	DTUTC.G = g;
	DTUTC.h = (int)floor(JDH);
	double a;
	a = (int)(60.0*(JDH - DTUTC.h));
	DTUTC.m = (int)floor(a);
	a = 60.0*(a - DTUTC.m);
	DTUTC.s = (int)floor(a);
	DTUTC.d = a - DTUTC.s;
	if (mjd) *mjd = MJD;
	if (DT)  *DT = DTUTC;
	return MJD;
}

//-----------------------------------------------------------------------------
TheDblTime ZFDMYHtoMJD(DATE& date, double* h, TheDblTime* mjd, DATETIME* DT)
{
	return (ZFDMYHtoMJD(date.d, date.m, date.g, h, mjd, DT));
}

//-----------------------------------------------------------------------------
TheDblTime ZFDMYHMStoMJD(DATETIME DTUTC, TheDblTime* mjd)
{
	double h = DTUTC.h + DTUTC.m / 60.0 + (DTUTC.s + DTUTC.d) / 3600.0;
	return (ZFDMYHtoMJD(DTUTC.D, DTUTC.M, DTUTC.G, &h, mjd));
}

//-----------------------------------------------------------------------------
DATETIME ZFMJDtoDMYH(TheDblTime mjd, DATETIME* DT, double* H)
{
	// Модифицированная юлианская дата (вместе с дробной частью),
	// то есть в том числе часы
	TheDblTime MJD;
	// только дробная часть выраженная в часах
	double JDH;
	// Гражданская (календарная) дата и время
	DATETIMEUTC DTUTC;

	long        jdt;
	double      jdd, d;
	TheDblTime  jd;
	int         id, i;
	static int  daym[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	MJD = mjd;
	jd = mjd;
	jd += TheT_MJD_ORIGIN;
	jdd = jd.days;

	jdt = (long)((jdd - 1721425.5) / 365.242198781);
	d = jdd - 1721425.5 - jdt * 365 - jdt / 4 + jdt / 100 - jdt / 400;
	if (d == 365l && (jdt % 4) != 0) {
		d -= 365;
		jdt += 1;
	}
	id = (int)d;
	d = d - id;
	jdt += 1;

	if (jdt % 4 == 0) daym[1] += 1;
	if (jdt % 100 == 0 && jdt % 400 != 0) daym[1] -= 1;
	for (i = 0; i<12; i++) {
		id -= daym[i];
		if (id<0) break;
	}
	id += daym[i] + 1;
	daym[1] = 28;
	if (id>31) {
		id = 1;
		i = 0;
		jdt += 1;
	}
	DTUTC.D = id;
	DTUTC.M = i + 1;
	DTUTC.G = jdt;
	// Часы
	JDH = MJD.sec / 3600.0;
	if (H) *H = JDH;
	DTUTC.h = (int)floor(JDH);
	// Минуты
	double a;
	a = 60.0*(JDH - DTUTC.h);
	DTUTC.m = (int)floor(a);
	// Секунды
	a = 60.0*(a - DTUTC.m);
	DTUTC.s = (int)floor(a);
	DTUTC.d = a - DTUTC.s;
	if (DT) *DT = DTUTC;
	return DTUTC;
}

//-----------------------------------------------------------------------------
TheDblTime ZFDaysBetweenDates(DATE& date1, DATE& date2)
{
	TheJD TJ;
	TheDblTime Day1 = TJ.DMYHtoMJD(date1);
	TheDblTime Day2 = TJ.DMYHtoMJD(date2);
	TheDblTime Days = Day2 - Day1;
	return Days;
}

//-----------------------------------------------------------------------------
TheJD::TheJD()
{
	MJD = 0 ;
	JDH = 0 ;
	memset(&DTUTC, 0, sizeof(DTUTC)) ;
}

//-----------------------------------------------------------------------------
TheJD::~TheJD()
{

}

//-----------------------------------------------------------------------------
void operator << (TheJD& TJ, TheDblTime mjd) { TJ.MJDtoDMYH(mjd) ; }
void operator >> (TheDblTime mjd, TheJD& TJ) { TJ.MJDtoDMYH(mjd) ; }

//-----------------------------------------------------------------------------
TheJD& TheJD::operator = (TheDblTime mjd) 
{
	MJDtoDMYH(mjd);
	return (*this);
}

//-----------------------------------------------------------------------------
TheDblTime TheJD::DMYHtoMJD(int d, int m, int g, double* h, TheDblTime* mjd) 
{

	ZFDMYHtoMJD(d, m, g, &JDH, &MJD, &DTUTC);
	if (mjd) *mjd = MJD;
	if (h  ) *h   = JDH;
	return MJD;
}

//-----------------------------------------------------------------------------
TheDblTime TheJD::DMYHtoMJD(DATE& date, double* h, TheDblTime* mjd)
{
	return (DMYHtoMJD(date.d, date.m, date.g, h, mjd));
}

//-----------------------------------------------------------------------------
TheDblTime TheJD::DMYHMStoMJD(DATETIME DTUTC, TheDblTime* mjd)
{
	double h = DTUTC.h + DTUTC.m / 60.0 + (DTUTC.s + DTUTC.d) / 3600.0;
	return (DMYHtoMJD(DTUTC.D, DTUTC.M, DTUTC.G, &h, mjd));
}

//-----------------------------------------------------------------------------
DATETIME TheJD::MJDtoDMYH(TheDblTime mjd, DATETIME* DT, double* H)  
{
	MJD = mjd ;
	ZFMJDtoDMYH(mjd, &DTUTC, &JDH);
	if (H) *H = JDH;
	if (DT) *DT = DTUTC;
	return DTUTC;
}

//-----------------------------------------------------------------------------
TheDblTime TheJD::DaysBetweenDates(DATE& date1, DATE& date2)
{
	TheJD TJ;
	TheDblTime Day1 = TJ.DMYHtoMJD(date1);
	TheDblTime Day2 = TJ.DMYHtoMJD(date2);
	TheDblTime Days = Day2 - Day1;
	return Days;
}

//-----------------------------------------------------------------------------