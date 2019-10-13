//-----------------------------------------------------------------------------
// TheDataTime.cpp
// А. Николенко 03.08.2018
//
// Структуры Дата, Время, Дата-Время. Гражданская календарная дата и время
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyTimeSource\\ModTime.h>
#include <FlyTimeSource\\TheDataTime.h>

// Колличество знаков после запятой в секундах для докуметирования
// DATETIME, DATETIMEUTC, DATETIMEDMB в текстовую строку
int DATETIME::strdec = 3 ;

//-----------------------------------------------------------------------------
// Вывод DATETIME в поток
//-----------------------------------------------------------------------------
ostream& operator << (ostream& out, DATETIMEUTC& D)
{
	out.fill('0');
	out.width(2); out << right << D.D;
	out.width(1); out << '.';
	out.width(2); out << D.M;
	out.width(1); out << '.';
	out.width(4); out << D.G;
	out.width(1); out << ' ';

	out.width(2); out << D.h;
	out.width(1); out << ':';
	out.width(2); out << D.m;
	out.width(1); out << ':';
	out.fill('0');
	out.precision(5);
	out.width(2); out << (double)(D.s + D.d);
	out.width(4); out << " utc";

	out.width(0);
	out.precision(0);
	out.fill(' ');

	return out;
}

ostream& operator << (ostream& out, DATETIMEDMB& D)
{
	out.fill('0');
	out.width(2); out << right << D.D;
	out.width(1); out << '.';
	out.width(2); out << D.M;
	out.width(1); out << '.';
	out.width(4); out << D.G;
	out.width(1); out << ' ';

	out.width(2); out << D.h;
	out.width(1); out << ':';
	out.width(2); out << D.m;
	out.width(1); out << ':';
	out.precision(5);
	out.width(2); out << (double)(D.s + D.d);
	out.width(4); out << " дмв";

	out.width(0);
	out.precision(0);
	out.fill(' ');

	return out;
}

//-----------------------------------------------------------------------------
ostream& operator << (ostream& out, DATE& D)
{
	out.fill('0');
	out.width(2); out << right << D.d;
	out.width(1); out << '.';
	out.width(2); out << D.m;
	out.width(1); out << '.';
	out.width(4); out << D.g;
	out.fill(' ');
	return out;
}

ostream& operator << (ostream& out, TIME& T)
{
	out.fill('0');
	out.width(2); out << T.h;
	out.width(1); out << ':';
	out.width(2); out << T.m;
	out.width(1); out << ':';
	out.precision(5);
	out.width(2); out << (double)(T.s + T.d);
	out.fill(' ');
	return out;
}

//-----------------------------------------------------------------------------
// Абстрактные структурированные дата+время (т.е не известно ДМВ или UTC)
//-----------------------------------------------------------------------------
void operator << (DATE& D, DATETIME& DT)
{
	D.d = DT.D; D.m = DT.M; D.g = DT.G;
}

void operator << (DATETIME& DT, DATE& D)
{
	DT.h = DT.m = DT.s = 0; DT.d = 0;
	DT.D = D.d; DT.M = D.m; DT.G = D.g;
}

void operator << (TIME& T, DATETIME& DT)
{
	T.h = DT.h; T.m = DT.m; T.s = DT.s; T.d = DT.d;
}

void operator << (DATETIME& DT, TIME& T)
{
	DT.h = T.h; DT.m = T.m; DT.s = T.s; DT.d = T.d;
}

void operator >> (DATE& D, DATETIME& DT) { DT << D; }
void operator >> (DATETIME& DT, DATE& D) { D << DT; }
void operator >> (TIME& T, DATETIME& DT) { DT << T; }
void operator >> (DATETIME& DT, TIME& T) { T << DT; }

//-----------------------------------------------------------------------------
// ДМВ
//-----------------------------------------------------------------------------
void operator << (double& t, DATETIMEDMB& DMB)
{
	DATE dt = DATE(DMB.D, DMB.M, DMB.G);
	TIME tm = TIME(DMB.h, DMB.m, DMB.s, DMB.d) ;
	TA_DMB(1, &dt, &tm, &t) ;
}

void operator << (DATETIMEDMB& DMB, double& t)
{
	DATE dt;
	TIME tm ;
	TA_DMB(0, &dt, &tm, &t) ;
	DMB.Set(dt.d, dt.m, dt.g, tm.h, tm.m, tm.s, tm.d) ;
}

void operator << (DATE& D, DATETIMEDMB& DMB)
{
	D.d = DMB.D ; D.m = DMB.M ; D.g = DMB.G ;
}

void operator << (DATETIMEDMB& DMB, DATE& D)
{
	DMB.h = DMB.m = DMB.s = 0 ; DMB.d = 0 ;
	DMB.D = D.d ; DMB.M = D.m ; DMB.G = D.g ;
}

void operator << (TIME& T, DATETIMEDMB& DMB) 
{
	T.h = DMB.h ; T.m = DMB.m ; T.s = DMB.s ; T.d = DMB.d ;
}

void operator << (DATETIMEDMB& DMB, TIME& T) 
{
	DMB.h = T.h ; DMB.m = T.m ; DMB.s = T.s ; DMB.d = T.d ;
}

void operator >> (double& t, DATETIMEDMB& DMB) { DMB << t ; }
void operator >> (DATETIMEDMB& DMB, double& t){ t << DMB ; }
void operator >> (DATE& D, DATETIMEDMB& DMB) { DMB << D; }
void operator >> (DATETIMEDMB& DMB, DATE& D) { D << DMB; }
void operator >> (TIME& T, DATETIMEDMB& DMB) { DMB << T ; }
void operator >> (DATETIMEDMB& DMB, TIME& T) { T << DMB ; }

//-----------------------------------------------------------------------------
// UTC
//-----------------------------------------------------------------------------
void operator << (DATETIMEUTC& UTC, double& t)
{
	DATE dt;
	TIME tm;
	double _t = t - 0.125;
	TA_DMB(0, &dt, &tm, &_t);
	UTC.Set(dt.d, dt.m, dt.g, tm.h, tm.m, tm.s, tm.d);
}

void operator << (double& t, DATETIMEUTC& UTC)
{
	DATE dt = DATE(UTC.D, UTC.M, UTC.G);
	TIME tm = TIME(UTC.h, UTC.m, UTC.s, UTC.d);
	TA_DMB(1, &dt, &tm, &t);
	t += 0.125;
}

void operator << (DATETIMEUTC& DMB, DATE& D)
{
	DMB.h = DMB.m = DMB.s = 0; DMB.d = 0;
	DMB.D = D.d; DMB.M = D.m; DMB.G = D.g;
}

void operator << (DATE& D, DATETIMEUTC& UTC)
{
	D.d = UTC.D; D.m = UTC.M; D.g = UTC.G;
}

void operator << (DATETIMEUTC& UTC, TIME& T)
{
	UTC.h = T.h; UTC.m = T.m; UTC.s = T.s; UTC.d = T.d;
}

void operator << (TIME& T, DATETIMEUTC& UTC) 
{
	T.h = UTC.h ; T.m = UTC.m ; T.s = UTC.s ; T.d = UTC.d ;
}

void operator >> (double& t, DATETIMEUTC& UTC) { UTC << t; }
void operator >> (DATETIMEUTC& UTC, double& t) { t << UTC; }
void operator >> (DATE& D, DATETIMEUTC& UTC) { UTC << D; }
void operator >> (DATETIMEUTC& UTC, DATE& D) { D << UTC; }
void operator >> (TIME& T, DATETIMEUTC& UTC) { UTC << T; }
void operator >> (DATETIMEUTC& UTC, TIME& T) { T << UTC ; }

//-----------------------------------------------------------------------------
inline bool operator < (const DATETIMEDMB& d1, const DATETIMEDMB& d2)
{
	return (d1.G<d2.G || (d1.G == d2.G && (d1.M<d2.M ||
		(d1.M == d2.M && d1.D<d2.D || (d1.D == d2.D && d1.h<d2.h ||
		(d1.h == d2.h && d1.m<d2.m || (d1.m == d2.m && (d1.s + d1.d)<(d2.s + d2.d))))))));
}

inline bool operator < (const DATETIMEUTC& d1, const DATETIMEUTC& d2)
{
	return (d1.G<d2.G || (d1.G == d2.G && (d1.M<d2.M ||
		(d1.M == d2.M && d1.D<d2.D || (d1.D == d2.D && d1.h<d2.h ||
		(d1.h == d2.h && d1.m<d2.m || (d1.m == d2.m && (d1.s + d1.d)<(d2.s + d2.d))))))));
}

inline bool operator < (const DATETIME& d1, const DATETIME& d2)
{
	return (d1.G<d2.G || (d1.G == d2.G && (d1.M<d2.M ||
		(d1.M == d2.M && d1.D<d2.D || (d1.D == d2.D && d1.h<d2.h ||
		(d1.h == d2.h && d1.m<d2.m || (d1.m == d2.m && (d1.s + d1.d)<(d2.s + d2.d))))))));
}

//-----------------------------------------------------------------------
FLYTIME_API inline void operator << (char* S, DATETIME& D)
{
	if (DATETIME::strdec>0) {
		sprintf(S, "%.2d.%.2d.%.4d %.2d:%.2d:%.2d,%.*d\0",
			D.D, D.M, D.G, D.h, D.m, D.s, DATETIME::strdec, (int)(D.d*pow(10.0, DATETIME::strdec)));
	}
	else {
		sprintf(S, "%.2d.%.2d.%.4d %.2d:%.2d:%.2d\0",
			D.D, D.M, D.G, D.h, D.m, D.s);
	}
}

FLYTIME_API inline void operator << (FILE* pf, DATETIME& D)
{
	if (DATETIME::strdec>0) {
		fprintf(pf, "%.2d.%.2d.%.4d %.2d:%.2d:%.2d,%.*d\0",
			D.D, D.M, D.G, D.h, D.m, D.s, DATETIME::strdec, (int)(D.d*pow(10.0, DATETIME::strdec)));
	}
	else {
		fprintf(pf, "%.2d.%.2d.%.4d %.2d:%.2d:%.2d\0",
			D.D, D.M, D.G, D.h, D.m, D.s);
	}
}

FLYTIME_API inline void operator << (char* S, DATETIMEDMB& DMB)
{
	DATETIME* D = &DMB;
	S << *D;
}

FLYTIME_API inline void operator << (FILE* pf, DATETIMEDMB& DMB)
{
	DATETIME* D = &DMB;
	pf << *D;
}

FLYTIME_API inline void operator << (char* S, DATETIMEUTC& UTC)
{
	DATETIME* D = &UTC;
	S << *D;
}

FLYTIME_API inline void operator << (FILE* pf, DATETIMEUTC& UTC)
{
	DATETIME* D = &UTC;
	pf << *D;
}

FLYTIME_API inline void operator << (ofstream& F, DATETIMEDMB& DMB)
{
	//F.binary;
	//F.write("");
}

//-----------------------------------------------------------------------------








