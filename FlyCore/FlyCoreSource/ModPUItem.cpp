//------------------------------------------------------------------------
// ModPUItem.cpp
// А. Николенко 24.08.2018
//------------------------------------------------------------------------
#include <stdafx.h>
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ModPUManager.h>
//-------------------------------------------------------------------------------
// Элемент программы управления
//-------------------------------------------------------------------------------
ZPUItem::ZPUItem ( ) {
	var = varPU_ppu ;
	t   = 0 ;  // Время включения в ссс
	dt  = 0 ;  // Длительность включения в сек 
	P   = 0 ;  // Тяга в кгс
	Pu  = 0 ;  // Удельная тяга в сек
	dP  = 0 ;  // Произмодная от тяги по времени кгс / сек
	dPu = 0 ;  // производная от удельной тяги бр
	a   = 0 ;  // Ориентация импкльса
	b   = 0 ;  //
	da  = 0 ;
	db  = 0 ;
}
//-------------------------------------------------------------------------------
double ZPUItem::TS() { return(t) ; }

//-------------------------------------------------------------------------------
double ZPUItem::TE() { return(t+dt/k_cbc) ; }

//-------------------------------------------------------------------------------
// Расчёт ускорений в орбитальной системе координат
//	time			- время на которое расчитываются ускорения
//	m				- начальная масса КА в момент времени time 
//	Arln			- искомые ускорения в орбитальной системе координат
//	pMs, pP, pPu	- секундный расход, тяга, удельная тяга на данном участке в момент времени time
int ZPUItem::CalcARLN(double time, double m, double* Arln, double* pMs, double* pP, double* pPu) 
{	
	if (time<t || time>t+dt/k_cbc) {
		// Если заданное время time лежит вне данного участка
		// то есть до момента t или после t+dt,
		// тогда возвращаем нулевые ускорения
		memset(Arln, 0, 3*sizeof(double)) ;
		return 0 ;
	}
	double AM ;
	double Pdu ;
	double PUdu ;
	double MSdu ;

	// Время от начала участка в секундах
	double dtime = (time-t)*k_cbc ;

	Pdu  = P ;
	if (dP!= 0) Pdu+= dP*dtime ;
	if (Pdu<1e-13) {
		AddErrorMSG( "В программе управления задано пренебрежимо малое значение тяги" ) ; 
		return 1 ;
	}

	switch (var) {
		case varPU_ppu:
			PUdu = Pu ;
			if (dPu!=0) PUdu+= dPu*dtime ;
			if (PUdu<1e-13) {
				AddErrorMSG( "В программе управления задано пренебрежимо малое значение удельной тяги" ) ; 
				return 1 ;
			}
			MSdu = Pdu/PUdu ;
			break ;
		case varPU_pms:
			MSdu = ms ;
			if (dms!= 0) MSdu+= dms*dtime ;
			if (MSdu<1e-13) {
				AddErrorMSG( "В программе управления задано пренебрежимо малое значение секундного расхода топлива" ) ; 
				return 1 ;
			}
			PUdu = Pdu/MSdu ;
			break ;
		default:
			AddErrorMSG( "Задан неизвестный тип участка программы управления" ) ; 
			return 1 ;
	}

	if (pMs) *pMs = MSdu ;
	if (pPu) *pPu = PUdu ;
	if (pP ) *pP  = Pdu ;
	// модуль ускорения в м/сек2
	AM = Pdu*k_g0/m ;
	// Учёт разворота на АУ
	double at = a+da*dtime ; 
	double bt = b+db*dtime ; 
	// Расчёт проекций в орбитальной СК
	double ra = TO_RAD(at) ;
	double rb = TO_RAD(bt) ;
	double sb = sin(rb) ;
	double cb = cos(rb) ;
	double sa = sin(ra) ;
	double ca = cos(ra) ;
	Arln[0] = AM*sb ;		// Радиальное направление
	Arln[1] = AM*cb*ca ;	// Трансверсальное		
	Arln[2] = AM*cb*sa ;	// Бинормальное
	
	return 0 ;
}

//-------------------------------------------------------------------------------
// Оператор сравнения двух участков
// "Меньше" тот участок который раньше
bool operator < ( const ZPUItem &U1, const ZPUItem &U2 ) 
{ return( U1.t < U2.t ? true : false ) ; }

//-------------------------------------------------------------------------------