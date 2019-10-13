//-----------------------------------------------------------------------------
// TheDblTime.cpp
// А. Николенко 19.11.2018
// 
// Время двойной точности.
// Колличество дней + время в пределах текущего дня в секундах
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Компоненты Fly
#include <FlyTime.h>	// Функции фундаментальной астрономии
#include <FlySofa.h>	// Функции фундаментальной астрономии

//-----------------------------------------------------------------------------
#include <FlyTimeSource\\TheDblTime.h>

//-----------------------------------------------------------------------------
// Дата и время получаются для нулевого часового пояса то есть
// получим календарную дату и структурированное время UTC
// Для того чтоб при помощи данной функции получить ДМВ нужно ко времени
// TheDblTime T прибавить зональную поправку 0.125 ССС
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
