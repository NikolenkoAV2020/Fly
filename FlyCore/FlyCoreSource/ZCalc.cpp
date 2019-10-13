//---------------------------------------------------------------------------
// ZCalc.cpp
// А. Николенко 03.08.2018
// Некоторые вычислительные процедуры общщего назначения
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
//---------------------------------------------------------------------------
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ZSiderealTime.h>
#include <FlyCoreSource\\ZSK.h>
#include <FlyCoreSource\\ZCalc.h>

//------------------------------------------------------------------------------
// Преобразование угловой величины в иную размерность
double AngleDimConv(double a, DIMANGL fromdim, DIMANGL todim)  
{
	double u = a ; if (fromdim==todim) return u ; 
	switch (fromdim) {
		case DimAnglRad:
			// перевод из радианов
			switch (todim) {
				case DimAnglGrad: u = a*M_GRARAD ; break ;
				case DimAnglSSS:  u = a/k_dwapi ; break ;
				case DimAnglH:    u = a/k_dwapi*24.0 ; break ;
				case DimAnglMin:  u = a/k_dwapi*1440.0 ; break ;
				case DimAnglSec:  u = a/k_dwapi*k_cbc ; break ;
				default: assert(0) ;
			} break ;
		case DimAnglGrad:
			// перевод из градусов
			switch (todim) {
			case DimAnglRad:  u = a / M_GRARAD; break;
				case DimAnglSSS:  u = a/360.0 ; break ;
				case DimAnglH:    u = a/360.0*24.0 ; break ;
				case DimAnglMin:  u = a/360.0*1440.0 ; break ;
				case DimAnglSec:  u = a/360.0*k_cbc ; break ;
				default: assert(0) ;
			} break ;
		case DimAnglSSS:
			// перевод из ССС
			switch (todim) {
				case DimAnglRad:  u = a*k_dwapi ; break ;
				case DimAnglGrad: u = a*360.0 ; break ;
				case DimAnglH:    u = a*24.0 ; break ;
				case DimAnglMin:  u = a*1440.0 ; break ;
				case DimAnglSec:  u = a*k_cbc ; break ;
				default: assert(0) ;
			} break ;
		case DimAnglH:
			// перевод из Часов
			switch (todim) {
				case DimAnglRad:  u = a/24.0*k_dwapi ; break ;
				case DimAnglGrad: u = a/24.0*360.0 ; break ;
				case DimAnglSSS:  u = a/24.0 ; break ;
				case DimAnglMin:  u = a/24.0*1440.0 ; break ;
				case DimAnglSec:  u = a/24.0*k_cbc ; break ;
				default: assert(0) ;
			} break ;
		case DimAnglMin:
			// перевод из Минут
			switch (todim) {
				case DimAnglRad:  u = a/1440.0*k_dwapi ; break ;
				case DimAnglGrad: u = a/1440.0*360.0 ; break ;
				case DimAnglSSS:  u = a/1440.0 ; break ;
				case DimAnglH:    u = a/1440.0*24.0 ; break ;
				case DimAnglSec:  u = a/1440.0*k_cbc ; break ;
				default: assert(0) ;
			} break ;
		case DimAnglSec:
			// перевод из Секунд
			switch (todim) {
				case DimAnglRad:  u = a/k_cbc*k_dwapi ; break ;
				case DimAnglGrad: u = a/k_cbc*360.0 ; break ;
				case DimAnglSSS:  u = a/k_cbc ; break ;
				case DimAnglH:    u = a/k_cbc*24.0 ; break ;
				case DimAnglMin:  u = a/k_cbc*1440.0 ; break ;
				default: assert(0) ;
			} break ;
		default: assert(0) ;
	}
	return u ;
}

//------------------------------------------------------------------------------
// Вычисление местного СРЕДНЕГО солнечного времени tmss 
// на долготе L по атомному ta, отсчитанному от J1975 
//		AtomicTime - атомное время от эпохи J1975
//		L          - гринвическая долгота в градусах
//		tmss       - искомое среднее солнечное время
//		dim        - размерность tmis:
//		             0 - в радианах
//                   1 - в градусах
//                   2 - в ссс
//                   3 - в часах
//------------------------------------------------------------------------------
int ZCalcMSSV(double AtomicTime, double L, double& tmss, DIMANGL dim)
{
	tmss = AtomicTime-0.125+ValiA(L, 1)/360.0 ;
	DATETIMEDMB DT ; DT << tmss ;
    DT.h=DT.m=DT.s=0 ; DT.d=0 ;
    double t0 ; t0 << DT ;
	// МССВ в ссс 
    tmss = tmss-t0 ;
	// Преобразование к нужной размерности если это требуется
	// в радианы  
	tmss = AngleDimConv(tmss, DimAnglSSS, dim) ;
    return 0 ;
}

//------------------------------------------------------------------------------
// Вычисление местного ИСТИННОГО солнечного времени tmss 
// на долготе L по атомному ta, отсчитанному от J1975 
//		AtomicTime - атомное время от эпохи J1975
//		L          - гринвическая долгота в градусах
//		tmis       - искомое истинное солнечное время
//		dim        - размерность tmis:
//		             0 - в радианах
//                   1 - в градусах
//                   2 - в ссс
//                   3 - в часах
//------------------------------------------------------------------------------
int ZCalcMISV(double AtomicTime, double L, double& tmis, DIMANGL dim)
{
	// Вычисление координат Солнца
	int rc ;
	rc = _CalcAstroKoord(AtomicTime, 0) ; RRC ;
	// Вычисление восхождения и склонения Солнца
	double Sun[3] ;	
	ZASCtoAscensionDecination(Wae->qso, Sun, 0) ;
	// Вычисление звёздного времени 
	double SiderialTime ; 
	rc = ZTrueSiderealTime(AtomicTime, SiderialTime) ; if(rc) return rc ;
	// Вычисление истинного солнечного времени в радианах
	tmis = SiderialTime + L / M_GRARAD - Sun[0] + k_pi;
	// Приведение к диапазону от 0 до pi
	tmis = ValiA(tmis, 0) ;
	// Преобразование к нужной размерности если это требуется
	// в градусы  
	tmis = AngleDimConv(tmis, DimAnglRad, dim) ;
	return 0 ;
}

//---------------------------------------------------------------------------
//    Расчет высоты объекта над ОЗЭ (или ЭК если fromEK=true) 
//    и производной по времени.
//---------------------------------------------------------------------------
//   Прототип: void ZHightDif (double *w, double *hht);
//   Входные параметры:
//         w - координаты в ГСК (x,y,z,Vx,Vy,Vz).
//   Выходные параметры:
//         hht[0] - высота над ОЗЭ,
//         hht[1] - радиус,
//         hht[2] - производная высоты по времени,
//         hht[3] - производная радиуса по времени.
//   Возвращаемое значение: нет.
//---------------------------------------------------------------------------
void ZCalcHightDif (double *x, double *hht, bool fromEK)
{
	double sin_fi ;
	double alfa = fromEK?k_alfak:k_alfa0 ; 
	double rekw = fromEK?k_ak:k_rekw ; 
	hht[1]=FR(x);
	sin_fi=x[2]/hht[1];
	hht[0]=hht[1]-rekw*(1-alfa*sin_fi*sin_fi);
	hht[3]=scal_pr(x,&x[3],3)/hht[1];
	hht[2]=hht[3]+2*rekw*alfa*(x[5]*hht[1]-x[2]*hht[3])*sin_fi/hht[1]/hht[1];
}

//---------------------------------------------------------------------------

