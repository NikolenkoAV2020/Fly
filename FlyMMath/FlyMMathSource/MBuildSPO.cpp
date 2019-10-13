//---------------------------------------------------------------------------
// MBuildSPO.cpp
// А. Николенко 03.08.2018
//
// Модуль проектирования орбит - расчёт геометрических характеристик
// орбиты (p, e, e1 = e*cos(w), e2 = e*sin(w) ) по заданным :
// периоду, средней высоте или суточному смещению
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma hdrstop
#include <FlyMMathSource\\MMath.h>
#include <FlyMMathSource\\MMathDef.h>
#include "MBuildSPO.h"

//---------------------------------------------------------------------------
static double epsT  = 0.000001 ;   // сек
static double epsh  = 0.00001 ;    // км
static double epsdL = 0.000001 ;   // град в сутки
static double epsi  = 0.000001 ;   // град
static double epse  = 0.00000001 ; //

static double kb_eps = 26332713071.154318 ;			// = 1.5*1,31048433178429426  км**5/ сек**2
static double kb_mu  = k_b0*1e4/8.64/8.64 ;			// 
static double kb_omega = 4.1780746209654444e-3 ;	// град/сек (6.300388096980000)
static double kb_a0	 = 6378.136 ;					// км

/*
double  pi      = acos(-1 ) ;
double  k_g0    = 9.80665 ;              // м / cек**2
double  k_mu    = 398600.44 ;            // км**3/ cек**2
double  k_gnom  = 9.780328E-3 ;          // км / сек**2 Ускорение силы тяжести на экваторе
double  k_eps   = 26332713071.154318 ;   // км**5/ сек**2(1.5*1,31048433178429426)
double  kb_a0    = 6378.136 ;             // км
double  kb_omega = 4.1780746209654444e-3 ;// град / сек    (6.300388096980000)
double  k_wzw   = 7.2921157E-5 ;         // рад / сек     Угловая скорость вращения Земли
double  k_tzw   = 2 * pi / k_wzw ;       // Звездные сутки
double  k_alfa0 = 0.0033528244 ;         // коэффициент сжатия Земли
double  k_r0    = 6378.136 ;             // Экваториальный радиус Земли
double  k_pzw   = 42164.192 ;            // Фокальный параметр стационарной орбиты
double  k_DTmin = 0.1 ;
double  Tzw     = k_tzw ;
double  Tss     = 86400.0 ;
double  azw     = k_pzw ;
*/

//------------------------------------------------------------------------------
// Признак уточнения орбиты по согласованию периода h и Bh
bool isNeedEstimate_ThBh = 0 ;   
  
//------------------------------------------------------------------------------
// Вспомогательные функции
//------------------------------------------------------------------------------
// Драконический период в нормальном поле
// Заданные параметры 
//		i		- наклонение орбиты в градусах
//		e1, e2	- компоненты вектора эксцентриситета
//		pp		- указатель на фокальный параметр (м/б NULL)
//		pa		- указатель на большую полуось (м/б NULL)
// Необходимо задать либо pp либо pa. 	
// Результат	- драконический период в нормальном поле
//------------------------------------------------------------------------------
FLYMMATH_API double MTdrB20(double i, double e1, double e2, double* pp, double* pa)
{
    double p ;
    double a ;
    double T ;
	assert(pa || pp) ;
    if (pp) { p =*pp ; a = p/(1.0-e1*e1-e2*e2) ; }
    else    { a =*pa ; }
    double ski  = pow(Sin(i), 2) ;
	T = 2.*M_PI*a*sqrt(a / kb_mu)*(1. - kb_eps / kb_mu / a / a*(3. - 2.5*ski - e1*(1. - 5.*ski)));
    return T ;
}

//------------------------------------------------------------------------------
// Большaя полуось в центральном поле выраженная через период
//------------------------------------------------------------------------------
FLYMMATH_API double MATC00(double T)
{   
	return(pow(kb_mu*T*T / M_DPI / M_DPI, 1.0 / 3.0));
}

//------------------------------------------------------------------------------
// Большaя полуось в нормальном поле выраженная через период
// Заданные параметры 
//		i		- наклонение орбиты в градусах
//		e1, e2	- компоненты вектора эксцентриситета
//		T		- драконический период в нормальном поле в сек.
// Результат	- большая полуось в км.
//------------------------------------------------------------------------------
FLYMMATH_API int MATC20(double i, double e1, double e2, double T, double& a)
{
    int    count = 0 ;
    double dT, Tj, aj = MATC00( T ) ;
    while(1) {
		Tj = MTdrB20(i, e1, e2, 0, &aj) ;
		dT = T-Tj ; if ( fabs(dT) < epsT ) break ;
		aj = aj + MdadTC00(dT, aj) ;
		count++ ; if (count>100) return-22 ;
    }
    a = aj ;
    return 0 ;
}

//------------------------------------------------------------------------------
// Период в центральном поле выраженный через большую полуось
//------------------------------------------------------------------------------
FLYMMATH_API double MTAC00(double a)
{   
	return (2.*M_PI*a*sqrt(a / kb_mu));
}

//------------------------------------------------------------------------------
// Большaя полуось в центральном поле поле выраженная через суточное смещение трассы
//------------------------------------------------------------------------------
FLYMMATH_API double MAdLC00(double i, double dL, double N)
{
    double a ;
	a = (360.0 - dL) / M_DPI / N / kb_omega;
    a = a*a ;
    a = pow(kb_mu*a, 1.0/3.0) ;
	a = (360. - dL - N*(kb_omega + M_DPI / a / a*kb_eps / kb_mu*Cos(i))) / 2. / M_PI / N / kb_omega;
    a = a*a ;
    a = pow(kb_mu*a, 1.0/3.0) ;
    return a ;
}

//------------------------------------------------------------------------------
// E1 в нормальном поле выраженное через фокальный параметр
//------------------------------------------------------------------------------
FLYMMATH_API double ME1B20(double i, double p)
{   return( kb_eps / kb_mu  / p / p * ( 1.0 - 2.0 / 3.0 * pow( Sin( i ), 2 ) ) ) ; }
//------------------------------------------------------------------------------
// Суточный дрейф долготы в нормальном поле выраженный через период
//------------------------------------------------------------------------------
FLYMMATH_API double MdLTB20(double i, double T, double N, double p)
{   return( 360.0 - N * ( kb_omega * T + 360.0 * kb_eps / kb_mu / p / p * Cos( i ) ) ) ; }
//------------------------------------------------------------------------------
// Приращение большой полуоси выраженное через приращение суточного дрейфа долготы
//------------------------------------------------------------------------------
FLYMMATH_API double MdaddLC00(double a, double N, double ddL)
{
	return(-ddL / N / kb_omega / 3.0 / M_PI * sqrt(kb_mu / a));
}
//------------------------------------------------------------------------------
// Приращение большой полуоси выраженное через приращение косинуса наклонения для ССО
//------------------------------------------------------------------------------
FLYMMATH_API double MdadciC00(double a, double dci)
{
	return(-dci * 365.2422 * 86400.0 * kb_eps / 7.0 / M_PI / a / a / sqrt(a * kb_mu));
}
//------------------------------------------------------------------------------
// Приращение большой полуоси выраженное через приращение периода
//------------------------------------------------------------------------------
FLYMMATH_API double MdadTC00(double dT, double a)
{
	return(dT / 3.0 / M_PI * sqrt(kb_mu / a));
}
//------------------------------------------------------------------------------
// E1 и E2 выраженное через наклонение и большую полуось для ДУ с учётом аномалий до 7x0
//------------------------------------------------------------------------------
FLYMMATH_API int ME1E2iB70(double i, double a, double& e1, double& e2)
{
    double si   = Sin(i) ;
    double ski  = si*si ;
    double da, d, g2, g3, g5, g7 ;
    double C20  =-1.0826257e-3 ;
    double C30  = 2.5324070e-6 ;
    double C50  = 0.2279185e-6 ;
    double C70  = 0.350079e-6 ;
    //
    da= kb_a0/a ;
    g2 =-C20*pow(da, 2) ;
    g3 = C30*pow(da, 3) ;
    g5 = C50*pow(da, 5) ;
    g7 = C70*pow(da, 7) ;
    d  = 4.0-5.0*ski ;
    d = fabs(d)<0.0001 ? 0.0001*Sign1(d) : d ;
    e1 = g2*(1.5-ski) ;
    e2 = si/g2*(0.5*g3 - 5./8.0*g5*(8.0-28.0*ski+21.0*ski*ski)/d
        + 35.0/256.0*g7*(64.0-432.0*ski+792.*ski*ski -
        429.0*ski*ski*ski)/d) ;
    return 0 ;
}

//------------------------------------------------------------------------------
// Численное определение минимальной и максимальной высот на витке
// !!!!!!!!!!!! Требуется дороботка !!!!!!!!!!!!!!!
//------------------------------------------------------------------------------
FLYMMATH_API int MHminmax(double i, double p, double e1, double e2, double& hmin, double& hmax, double* umin, double* umax)
{
	double hu, u, stepu, ro, su, cu, c2u ;
    double ski = Sin(i) ;
    double ski2= ski*ski ;
    u = 0, stepu = 0.001 ;
    hmin = hmax =-10001.0 ;
    if (umin) *umin = 0 ;
    if (umax) *umax = 0 ;
    while (u<360.0) {
		su = Sin(u) ; cu = Cos(u) ; c2u = Cos(2.0*u) ;
		ro = p/(1.0+e1*cu+e2*su) ;
		hu = ro-kb_eps/ro/kb_mu*(1.0-cu+1.0/6.0*ski2*(4.0*cu-c2u-3.0))-kb_a0*(1.0-k_alfa0*ski2*su*su) ;
		if (hmin<-10000.0) { hmin=hmax=hu ; }
		else {
			if (hu<hmin) { hmin = hu ; if (umin) *umin = u ; }
			if (hu>hmax) { hmax = hu ; if (umax) *umax = u ; }
		}
		u+= stepu ;
    }
    return 0 ;
}

//------------------------------------------------------------------------------
FLYMMATH_API double BGeoFromGSK(double* xyz)
{
	double f  = 1.0/k_alfa0 ;
	double a  = 1e4*k_a0 ;
	double k0 = pow((f-1.0)/f, 2) ; 
	double k1 = a*(2.0*f-1.0)/f/f ; 
	double R  = xyz[0]*sqrt(1.0+pow(xyz[1]/xyz[0], 2)) ;
	double c1 = k0*pow(xyz[2]/R, 2) ;
	double b0 = R*sqrt(1.0+c1) ;
	double c0 = b0/(b0-k1) ; 
	double D1 = R*sqrt(1.0+c1*c0*c0) ;
	double Z1 = xyz[2]/D1 ;
	double R1 = R/D1 ; 
	double B  = atan(xyz[2]/R*(1.0+(c0+(1.0-c0)*R1*R1)/(D1/k1-R1*R1))) ;
	B = GradA(B) ;
	return B ;
}

//------------------------------------------------------------------------------
//Вычисление геоцентрической широты по геодезической
FLYMMATH_API void BGeodToBCent(double B, double& f)
{
    //f = sin(RadA(B)-k_alfa0*Sin(2*B)) ;
	//f = Asin(f) ;
	f = Tan(B)*pow(1.0-k_alfa0, 2.0) ;
	f = Atan(f) ;
}

//------------------------------------------------------------------------------
//Вычисление геодезической широты по геоцентрической
FLYMMATH_API void BCentToBGeod(double f, double& B)
{
	B = Tan(f)/pow(1.0-k_alfa0, 2.0) ;
	B = Atan(B) ;
}

//------------------------------------------------------------------------------
// Вычисление аргумента широты минимальной высоты
// Дополнительно рассчитывчаются геоцентрическая широта минимальной высоты
// косинус и синус найденного аргумента широты
FLYMMATH_API void CalcUBH(double i, double Bh, int VarBh, double& uBh, double* bhe, double* cuh, double* suh)
{
	double si = Sin(i) ;
	// геоцентрическая широта
	double b ;
	BGeodToBCent(Bh, b) ;
	if (bhe) *bhe = b ;
	double sbh = Sin(b) ;
	// синус аргумента широты
    double d2 = sbh/si ;
	if (suh) *suh = d2 ;
    if (fabs(d2)>1.0) {
		d2 = Sign1(d2) ;
		//MMessageBox( "Процедура MD12K12: | d2 |  > 1 \r\nИсправлено на 1" , 2, 0 ) ;
		//return 1 ;
    }
	// косинус аргумента широты минимаьлной высоты
    double d1 = sqrt(1.0-d2*d2) ;
    // Если экстремальная высота расположена на нисходящей ветви
	// то косинус аргумента минимальной высоты отрицательный
    if (VarBh) d1 =-d1 ;
	if (cuh) *cuh = d1 ;
	// значение аргумента широты экстремальной высоты
	uBh= AcosSS(d1, d2) ;
}

//------------------------------------------------------------------------------
// Вычисление промежуточных величин d1 d2 k1 k2
//------------------------------------------------------------------------------
FLYMMATH_API int MD12K12(double i, double he, double Bhe, int VarBh, double& d1, double& d2,
            double& k1, double& k2, double& rhe, double& uhe)
{
	double bhe, sbh, si ;
	CalcUBH(i, Bhe, VarBh, uhe, &bhe, &d1, &d2) ;
	sbh = Sin(bhe) ;
	si  = Sin(i) ;
	// Радиус экстремальной высоты
    rhe= he+kb_a0*(1.0-k_alfa0*sbh*sbh) ;
    k1 = rhe+kb_eps/kb_mu/rhe*(1.0-d1+1.0/6.0*si*si*(4.0*d1-d1*d1+d2*d2-3.0)) ;
    k2 = kb_eps/kb_mu/rhe*(d2+2.0/3.0*si*si*d2*(d1-1.0))-2.0*kb_a0*k_alfa0*d1*d2*si*si ;
    return 0 ;
}

//------------------------------------------------------------------------------
// Вычисление промежуточных величин d1 d2 k1 k2
// не по геодезической широте минимальной высоты а по агрументу широты этой высоты
//------------------------------------------------------------------------------
FLYMMATH_API int MD12K12fromUH(double i, double he, double uhe, double& d1, double& d2, double& k1, double& k2, double& rhe)
{
	double sbh, si ;
	si = Sin(i) ;
	d1 = Cos(uhe) ;
	d2 = Sin(uhe) ;
	sbh= si*d2 ;
	// Радиус экстремальной высоты
    rhe= he+kb_a0*(1.0-k_alfa0*sbh*sbh) ;
    k1 = rhe+kb_eps/kb_mu/rhe*(1.0-d1+1.0/6.0*si*si*(4.0*d1-d1*d1+d2*d2-3.0)) ;
    k2 = kb_eps/kb_mu/rhe*(d2+2.0/3.0*si*si*d2*(d1-1.0))-2.0*kb_a0*k_alfa0*d1*d2*si*si ;
    return 0 ;
}

//------------------------------------------------------------------------------
// Вычисление E1, E2, a и Е по промежуточным величинам d1 d2 k1 k2
//------------------------------------------------------------------------------
FLYMMATH_API int ME1E2TpD12K12(double i, double p, double d1, double d2, double k1, double k2,
                 double& e1, double& e2, double& a, double& T )
{
    e1 = p/k1*(k2/k1*d2+d1)-d1 ;
    e2 = p/k1*(d2-k2/k1*d1)-d2 ;
    if ( e1*e1 + e2*e2 >= 1 ) {
		// Слишком большой эксцентриситет и как следствие формулы для вычисления
		// k1 k2 d1 d2, полученные для околокругового движения не работают
		// т.е. e1 и e2 не достоверны
		return 1 ;
    }
    a  = p/(1.-e1*e1-e2*e2) ;
    T  = MTdrB20(i, e1, e2, 0, &a) ;
    return 0 ;
}
//------------------------------------------------------------------------------
// Вычисление E1, E2, a и Е по промежуточным величинам d1 d2 k1 k2
//------------------------------------------------------------------------------
FLYMMATH_API double MissTp(double p, double T)
{   
	return(Acos(-T*p*p*kb_mu/365.2422/86400.0/kb_eps)) ; 
}
//------------------------------------------------------------------------------
// Вычисление a по наклонению для ССО
//------------------------------------------------------------------------------
FLYMMATH_API double Massi(double i)
{
	double a = Cos(i)*365.2422*86400.0*kb_eps / M_DPI;
    a = pow(a*a/kb_mu, 1.0/7.0) ;
    return a ;
}

//------------------------------------------------------------------------------
// Проверка корректности эллиптической орбиты
//------------------------------------------------------------------------------
FLYMMATH_API bool isNotValiOrb(double i, double p, double e1, double e2, double he, int Varh, double uhe, double* dhe = 0, double* duhe = 0)
{
    double hmin, hmax, umin, umax, h, u ;
    MHminmax(i, p, e1, e2, hmin, hmax, &umin, &umax) ;
    h = Varh ? hmax:hmin ;
    u = Varh ? umax:umin ;
    if (dhe ) *dhe = h-he ;
    if (duhe) *duhe= ValiDA(u-uhe, 1) ;
    //if (fabs(h-he)>= 140) return 1 ;
	//if (fabs(h-he)>=3) return 1 ;
	//if (fabs(ValiDA(u-uhe, 1))>=10) return 1 ;
    return 0 ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int NotVali_i(double i, char* Msg)
{   
	if ( i <= 0 || i >= 180 ) { if ( Msg ) sprintf( Msg, "Задайте наклонение орбиты" ) ; return 1 ; } return 0 ; 
}
FLYMMATH_API int NotVali_a(double a, char* Msg)
{
	if (a <= 0) { if (Msg) sprintf(Msg, "Задайте большую полуось орбиты"); return 1; } return 0;
}
FLYMMATH_API int NotVali_e(double e, char* Msg)
{   if ( e < 0 || e >= 1 ) { if ( Msg ) sprintf( Msg, "Задайте эксцентриситет орбиты" ) ; return 1 ; } return 0 ; }
FLYMMATH_API int NotVali_w(double w, char* Msg)
{   if ( w < 0 || w >= 360.0 ) { if ( Msg ) sprintf( Msg, "Задайте положение перигея" ) ; return 1 ; } return 0 ; }
FLYMMATH_API int NotVali_T(double T, char* Msg)
{   if ( T <= 0 ) { if ( Msg ) sprintf( Msg, "Задайте драконический период" ) ; return 1 ; } return 0 ; }
FLYMMATH_API int NotVali_H(double H, char* Msg)
{   if ( H <= 0 ) { if ( Msg ) sprintf( Msg, "Задайте средную высоту" ) ; return 1 ; } return 0 ; }
FLYMMATH_API int NotVali_h(double h, char* Msg)
{   if ( h <= 0 ) { if ( Msg ) sprintf( Msg, "Задайте минимальную высоту" ) ; return 1 ; } return 0 ; }
FLYMMATH_API int NotVali_Bh(double Bh, char* Msg)
{   if ( Bh <-90 || Bh > 90 ) { if ( Msg ) sprintf( Msg, "Задайте широту минимальной высоты" ) ; return 1 ; } return 0 ; }
FLYMMATH_API int NotVali_N(double N, char* Msg)
{   if ( N <= 0 ) { if ( Msg ) sprintf( Msg, "Задайте кратность отбиты" ) ; return 1 ; } return 0 ; }
FLYMMATH_API int NotVali_dL(double dL, char* Msg)
{
    //if ( dL <= 0 ) { if (Msg) sprintf( Msg, "Задайте суточное смещение" ) ; return 1 ; }
    return 0 ;
}
FLYMMATH_API int NotVali_Ns(double Ns, char* Msg)
{   
	if (Ns<=0) { if (Msg) sprintf( Msg, "Задайте интервал замыкания трассы" ) ; return 1 ; }
    return 0 ; 
}
FLYMMATH_API int NotVali_Nv(double Nv, char* Msg)
{   
	if (Nv<=0) { if (Msg) sprintf( Msg, "Задайте число витков замыкания трассы" ) ; return 1 ; }
    return 0 ; 
}

//------------------------------------------------------------------------------
// Проектирование ОМИВ
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildOMIV_a(double i, double a, double& T, double& p, double& e1, double& e2, char* Msg)
{
    if ( NotVali_a( a, Msg ) ) return 1 ;
    if ( NotVali_i( i, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, ej ;
    ej = 0 ; aj = a ;
    pj = aj * ( 1.0 - ej*ej ) ;
    while ( 1 ) {
      ej = ME1B20( i, pj ) ;
      if ( ej < 0 || ej > 1 ) {
        if ( Msg ) sprintf( Msg, "Такой орбиты не существует\0" ) ;
        return-22 ;
      }
      pj = aj * ( 1.0 - ej*ej ) ;
      aj = pj / ( 1.0 - ej*ej ) ;
      if ( fabs( a - aj ) < epsh ) break ;
      //
      count++ ; if ( count > 100 ) return-22 ;
    }
    T = MTdrB20( i, ej, 0, 0, &a) ;
    p = pj ; e1 = ej ; e2 = 0 ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildOMIV_T(double i, double  T, double& p, double& e1, double& e2, char* Msg)
{
    if ( NotVali_i( i, Msg ) ) return 1 ;
    if ( NotVali_T( T, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, ej, Tj, dT ;
    ej = 0 ;
    aj = MATC00( T ) ;
    while( 1 ) {
      pj = aj * ( 1.0 - ej*ej ) ;
      ej = ME1B20( i, pj ) ;
      Tj = MTdrB20( i, ej, 0, 0, &aj) ;
      dT = T - Tj ; if ( fabs( dT ) < epsT ) break ;
      aj = aj + MdadTC00( dT, aj ) ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    p = pj ; e1 = ej ; e2 = 0 ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildOMIV_NsNv(double i, double Ns, double Nv, double& T, double& p,
                     double& e1, double& e2, char* Msg )
{
    if (NotVali_i (i,  Msg)) return 1 ;
    if (NotVali_Ns(Ns, Msg)) return 1 ;
    if (NotVali_Nv(Nv, Msg)) return 1 ;
    //
    int    count = 0, rc = 0 ;
    double Ti, d = 0 ;
    double ci = Cos(i, 1) * kb_eps/kb_mu ;
    //
    while(1) {
		if (count == 100) return-22 ;
		if (count) Ti = T ;
		T  = 360.0/kb_omega*(Ns/Nv-d)*86400.0 ;
		rc = MBuildOMIV_T(i, T, p, e1, e2, Msg); if (rc) return rc;
		if (count && fabs(Ti-T)<epsT) break ;
		d = ci/p/p ;
		count++ ;
    }
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildOMIV_hs(double i, double hs, double&  T, double& p,
                   double& e1, double& e2, char* Msg )
{
    if ( NotVali_i( i, Msg ) ) return 1 ;
    if ( NotVali_H( hs, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double pj, dpj, rs ;
    double ski  = pow( Sin( i ), 2 ) ;
    double k_em = kb_eps / kb_mu ;
    rs = hs + kb_a0 * ( 1.0 - 0.5 * k_alfa0 * ski ) ;
    pj = rs ;
    while( 1 ) {
      dpj = rs + k_em / pj * ( 1.0 - 0.5 * ski ) - pj ;
      if ( fabs( dpj ) < epsh ) { pj = pj + dpj ; break ; }
      pj = pj + dpj ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    p = pj ;
    e1= ME1B20( i, p ) ; e2 = 0 ;
    T = MTdrB20( i, e1, e2, &p, 0 ) ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildOMIV_dL(double i, double N, double dL, double&  T, double& p,
                  double& e1, double& e2, char* Msg )
{
    if ( NotVali_i ( i, Msg ) ) return 1 ;
    if ( NotVali_N ( N, Msg ) ) return 1 ;
    if ( NotVali_dL( dL,Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, ej, Tj, dLj, ddL ;
    ej = 0 ;
//    aj = MAdLC00( dL, N ) ;
    aj = MAdLC00(i, dL, N ) ;
    while ( 1 ) {
      pj = aj * ( 1.0 - ej*ej ) ;
      ej = ME1B20( i, pj ) ;
      Tj = MTdrB20( i, ej, 0, &pj, 0 ) ;
      pj = aj * ( 1.0 - ej*ej ) ;
      dLj= MdLTB20( i, Tj, N, pj ) ;
      ddL= dL - dLj ; if ( fabs( ddL ) < epsdL ) break ;
      aj = aj + MdaddLC00( aj, N, ddL ) ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    T = Tj ; p = pj ; e1 = ej ; e2 = 0 ;
    return 0 ;
}
//------------------------------------------------------------------------------
// Проектирование ДУ
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildDU_a(double i, double a, double&  T, double& p,
               double& e1, double& e2, char* Msg )
{
    if ( NotVali_a( a, Msg ) ) return 1 ;
    if ( NotVali_i( i, Msg ) ) return 1 ;
     //
    ME1E2iB70( i, a, e1, e2 ) ;
    T = MTdrB20( i, e1, e2, 0, &a ) ;
    p = a * ( 1.0 - e1*e1 - e2*e2 ) ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildDU_T(double i, double  T, double& p, double& e1, double& e2, char* Msg)
{
    if ( NotVali_i( i, Msg ) ) return 1 ;
    if ( NotVali_T( T, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, e1j, e2j, Tj, dT ;
    aj = MATC00( T ) ;
    while( 1 ) {
      ME1E2iB70( i, aj, e1j, e2j ) ;
      Tj = MTdrB20( i, e1j, e2j, 0, &aj ) ;
      dT = T - Tj ; if ( fabs( dT ) < epsT ) break ;
      aj = aj + MdadTC00( dT, aj ) ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    e1= e1j ; e2= e2j ;
    p = aj * ( 1.0 - e1*e1 - e2*e2 ) ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildDU_NsNv(double i, double Ns, double Nv, double& T, double& p,
                 double& e1, double& e2, char* Msg )
{
    if ( NotVali_i ( i, Msg  ) ) return 1 ;
    if ( NotVali_Ns( Ns, Msg ) ) return 1 ;
    if ( NotVali_Nv( Nv, Msg ) ) return 1 ;
    //
    int    count = 0, rc = 0 ;
    double Ti, d = 0 ;
    double ci = Cos( i, 1 ) * kb_eps / kb_mu ;
    //
    while ( 1 ) {
      if ( count == 100 ) return-22 ;
      if ( count ) Ti = T ;
      T = 360.0 / kb_omega * ( Ns / Nv - d ) * 86400.0 ;
	  rc = MBuildDU_T(i, T, p, e1, e2, Msg); if (rc) return rc;
      if ( count && fabs( Ti - T ) < epsT ) break ;
      d = ci / p / p ;
      count++ ;
    }
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildDU_hs(double i, double  hs, double& T, double& p,
               double& e1, double& e2, char* Msg )
{
    if ( NotVali_i(i, Msg )) return 1 ;
    if ( NotVali_H(hs, Msg)) return 1 ;
    //
    int    count = 0 ;
    double aj, e1j, e2j, pj, hmin, hmax, hj, dhj ;
    aj = hs+6371.0 ;
	while (1) {
		ME1E2iB70(i, aj, e1j, e2j) ;
		pj = aj*(1.0-e1j*e1j-e2j*e2j) ;
		MHminmax(i, pj, e1j, e2j, hmin, hmax, 0, 0) ;
		hj = 0.5*(hmin+hmax) ;
		dhj= hs-hj ; if (fabs(dhj)<epsh) break ;
		aj = aj+dhj ;
		count++ ; if (count>100) return-22 ;
    }
    p = pj ; e1 = e1j ; e2 = e2j ;
    T = MTdrB20(i, e1, e2, &p, 0) ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildDU_dL(double i, double N, double dL, double&  T, double& p,
               double& e1, double& e2, char* Msg )
{
    if ( NotVali_i ( i, Msg ) ) return 1 ;
    if ( NotVali_N ( N, Msg ) ) return 1 ;
    if ( NotVali_dL( dL, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, e1j, e2j, Tj, dLj, ddL ;
    aj = MAdLC00( i, dL, N ) ;
    while ( 1 ) {
      ME1E2iB70( i, aj, e1j, e2j ) ;
      Tj = MTdrB20( i, e1j, 0, 0, &aj ) ;
      pj = aj * ( 1.0 - e1j*e1j - e2j*e2j ) ;
      dLj= MdLTB20( i, Tj, N, pj ) ;
      ddL= dL - dLj ; if ( fabs( ddL ) < epsdL ) break ;
      aj = aj + MdaddLC00( aj, N, ddL ) ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    T = Tj ; p = pj ; e1 = e1j ; e2 = e2j ;
    return 0 ;
}
//------------------------------------------------------------------------------
// Проектирование эллиптической орбиты
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildEL_a(double i, double a, double e, double w, double& T, double& p,
                double& e1, double& e2, char* Msg )
{
    if ( NotVali_a ( a, Msg ) ) return 1 ;
    if ( NotVali_e ( e, Msg ) ) return 1 ;
    if ( NotVali_w ( w, Msg ) ) return 1 ;
    if ( NotVali_i ( i, Msg ) ) return 1 ;
    //
    p  = a * ( 1.0 - e*e ) ;
    e1 = e * Cos( w ) ;
    e2 = e * Sin( w ) ;
    T  = MTdrB20( i, e1, e2, &p, 0 ) ;
    return 0 ;
}

//------------------------------------------------------------------------------
// Решение квадратного уравнения
FLYMMATH_API int SolveSqrtEquation(double A, double B, double C, char VarRange, double* X1, double* X2)
{
	int ValiX = 0 ;
	double D = B*B-4*A*C ;
	if (D<0) return-1 ;
	double x1= (-B+sqrt(D))/2.0/A ;  
	double x2= (-B-sqrt(D))/2.0/A ;  
	if (VarRange==1) {
		if (x1>=0) ValiX+= 1 ;   
		if (x2>=0) ValiX+= 2 ;   
	}
	if (VarRange==-1) {
		if (x1<=0) ValiX+= 1 ;   
		if (x2<=0) ValiX+= 2 ;   
	}
	if ((ValiX&2) && !(ValiX&1)) {
		x1 = x2 ; 
		ValiX = 1 ;
	}
	if (X1) *X1 = x1 ;
	if (X2) *X2 = x2 ;
	return ValiX ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int MBuildEL_ahBh(double i, double a, double he, int Varh, double Bhe, int VarBh,
                   double& p, double& e1, double& e2, double& uhe, char* Msg)
{
    if (Msg) Msg[0] = '\0' ;
    if (NotVali_i (i, Msg))   return 1 ;
    if (NotVali_a (a, Msg))   return 1 ;
    if (NotVali_h (he, Msg))  return 1 ;
    if (NotVali_Bh(Bhe, Msg)) return 1 ;
    //
    int    count = 0 ;
    double d1, d2, k1, k2, rhe ;
	MD12K12( i, he, Bhe, VarBh, d1, d2, k1, k2, rhe, uhe ) ;

	int rc = 0 ; 
	double aj, Tj ;
	p = a ;
	while ( 1 ) {
		rc = ME1E2TpD12K12( i, p, d1, d2, k1, k2, e1, e2, aj, Tj ) ;
		if (rc) { rc =-22 ; return rc ; }
		p+= a-aj ;
		if (fabs(a-aj)<1e-10) break ;
		count++ ; if (count>50) { rc =-22 ; return rc ; }
    }
    if ( e1*e1 + e2*e2 >= 1 ) {
		// Слишком большой эксцентриситет и как следствие формулы для вычисления
		// k1 k2 d1 d2, полученные для околокругового движения не работают
		// т.е. e1 и e2 не достоверны
		if (Msg) sprintf(Msg, "Данная орбита не существует") ;
		return -22 ;
    }
    return 0 ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int MBuildEL_ThBh(double i, double T, double he, int Varh, double Bhe, int VarBh,
                   double& p, double& e1, double& e2, double& uhe, char* Msg )
{
    if (Msg) Msg[0] = '\0' ;
    if (NotVali_i (i, Msg))   return 1 ;
    if (NotVali_T (T, Msg))   return 1 ;
    if (NotVali_h (he, Msg))  return 1 ;
    if (NotVali_Bh(Bhe, Msg)) return 1 ;
    //
    int    rc, count = 0 ;
    double d1, d2, k1, k2, rhe ;
    double aj, e1j, e2j, pj, Tj, dT ;

	MD12K12( i, he, Bhe, VarBh, d1, d2, k1, k2, rhe, uhe ) ;
/*
	if (uhe==-999.0) {
		MD12K12( i, he, Bhe, VarBh, d1, d2, k1, k2, rhe, uhe ) ;
	} else {
		MD12K12fromUH(i, he, uhe, d1, d2, k1, k2, rhe) ;
	}
*/
    aj = MATC00( T ) ;
    pj = aj ;
	while ( 1 ) {
		rc = ME1E2TpD12K12( i, pj, d1, d2, k1, k2, e1j, e2j, aj, Tj ) ;
		if (rc) { rc =-22 ; goto error ; }
		pj = aj*(1.0-e1j*e1j-e2j*e2j) ;
		dT = T-Tj ; if (fabs(dT)<epsT) break ;
		pj = pj+MdadTC00(dT, pj) ;
		count++ ; if (count>50) { rc =-22 ; goto error ; }
    }
    if (isNotValiOrb(i, pj, e1j, e2j, he, Varh, uhe)) {
		rc =-10 ;
error : ;
		if (Msg) sprintf(Msg, "Данная орбита не существует") ;
		return rc ;
    }
    p = pj ; e1 = e1j ; e2 = e2j ;
    return 0 ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int MBuildEL_HshBh(double i, double hs, double he, int Varh, double Bhe, int VarBh,
                    double& p, double& e1, double& e2, double& uhe, char* Msg )
{
    if (Msg) Msg[0] = '\0' ;
    if (NotVali_i (i, Msg))   return 1 ;
    if (NotVali_H (hs, Msg))  return 1 ;
    if (NotVali_h (he, Msg))  return 1 ;
    if (NotVali_Bh(Bhe, Msg)) return 1 ;
    //
    int    count = 0 ;
    double d1, d2, k1, k2, rhe, ek, hmin, hmax ;
	MD12K12( i, he, Bhe, VarBh, d1, d2, k1, k2, rhe, uhe ) ;

	int rc = 0 ; 
	double hsj, aj, Tj ;
	p = hs+6371.0 ;
	while ( 1 ) {
		rc = ME1E2TpD12K12(i, p, d1, d2, k1, k2, e1, e2, aj, Tj) ; if (rc) { rc =-22 ; return rc ; }
		ek = e1*e1+e2*e2 ; 
		MHminmax(i, p, e1, e2, hmin, hmax, 0, 0) ;
		hsj= (hmin+hmax)/2.0 ;
		if (fabs(hs-hsj)<1e-10) break ;
		count++ ; if (count>50) { rc =-22 ; return rc ; }
		//
		aj+= hs-hsj ;
		p  = aj*(1-ek) ; 
    }
    if (ek>= 1 ) {
		// Слишком большой эксцентриситет и как следствие формулы для вычисления
		// k1 k2 d1 d2, полученные для околокругового движения не работают
		// т.е. e1 и e2 не достоверны
		if (Msg) sprintf(Msg, "Данная орбита не существует") ;
		return -22 ;
    }
    return 0 ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int MBuildEL_dLhBh(double i, double N, double dL, double he, int Varh, double Bhe,
                   int VarBh, double&  T, double& p, double& e1, double& e2, double& uhe, char* Msg)
{
    int  countEst_hBh = 0 ;
    if ( Msg ) Msg[ 0 ] = '\0' ;
    if ( NotVali_i ( i, Msg ) ) return 1 ;
    if ( NotVali_N ( N, Msg ) ) return 1 ;
    if ( NotVali_dL( dL, Msg ) ) return 1 ;
    if ( NotVali_h ( he, Msg ) ) return 1 ;
    if ( NotVali_Bh( Bhe, Msg ) ) return 1 ;
    //
	int    rc, count = 0 ;
    double d1, d2, k1, k2, rhe ;
    double aj, ej =0, e1j, e2j, pj, Tj, dLj, ddL ;
    double het = he ;
    double Bhet = Bhe ;
    int    VarBht = VarBh ;
met_MD12K12 :
    MD12K12(i, het, Bhet, VarBht, d1, d2, k1, k2, rhe, uhe) ;
    aj = MAdLC00(i ,dL, N) ;
    while (1) {
		pj = aj*(1.0-ej*ej) ;
		rc = ME1E2TpD12K12(i, pj, d1, d2, k1, k2, e1j, e2j, aj, Tj) ; if (rc) { rc =-22 ; goto error ; }
		ej = sqrt(e1j*e1j+e2j*e2j) ;
		pj = aj*(1.0-ej*ej) ;
		dLj= MdLTB20(i, Tj, N, pj) ;
		ddL= dL-dLj ; if (fabs(ddL)<epsdL) break ;
		aj = aj+MdaddLC00(aj, N, ddL) ;
		count++ ; if (count>100) { rc =-22 ; goto error ; }
    }
    double dhe ;
    if (isNotValiOrb(i, pj, e1j, e2j, he, Varh, uhe, &dhe)) {
		if (!isNeedEstimate_ThBh || countEst_hBh>5) {
			rc =-10 ;
error : ;
			if (Msg) sprintf(Msg, "Данная орбита не существует") ;
			return rc ;
		}
		het-= dhe ;
		countEst_hBh++ ;
		goto met_MD12K12 ;
	}
    //
    T = Tj ; p = pj ; e1 = e1j ; e2 = e2j ;
	return 0 ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int MBuildEL_NsNv(double i, double Ns, double Nv, double he, int Varh,
                   double Bhe, int VarBh, double& T, double& p, double& e1,
                   double& e2, double& uhe, char* Msg )
{
    if ( NotVali_i( i, Msg ) ) return 1 ;
    if ( NotVali_Ns( Ns, Msg ) ) return 1 ;
    if ( NotVali_Nv( Nv, Msg ) ) return 1 ;
    //
    int    count = 0, rc = 0 ;
    double Ti, d = 0 ;
    double ci = Cos( i, 1 ) * kb_eps / kb_mu ;
    //
	while ( 1 ) {
		if ( count == 100 ) return-22 ;
		if ( count ) Ti = T ;
		T = 360.0 / kb_omega * ( Ns / Nv - d ) * 86400.0 ;
		p = MATC00( T ) ;
		if ( count && fabs( Ti - T ) < epsT ) break ;
		d = ci / p / p ;
		count++ ;
	}
	rc = MBuildEL_ThBh(i, T, he, Varh, Bhe, VarBh, p, e1, e2, uhe, Msg ) ;
	return rc ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int MBuildEL_dLew(double i, double N, double dL, double e1, double e2, double& T, double& p, double& uhe, char* Msg)
{
//    int  countEst_hBh = 0 ;
    if (Msg) Msg[0] = '\0' ;
    if (NotVali_i (i, Msg)) return 1 ;
    if (NotVali_N (N, Msg)) return 1 ;
    if (NotVali_dL(dL, Msg)) return 1 ;
    //
    int    rc=0, count=0 ;
    double aj, e, pj=0, Tj=0, dLj, ddL ;
	e  = sqrt(e1*e1+e2*e2) ;
    aj = MAdLC00(i, dL, N) ;
    while ( 1 ) {
		pj = aj*(1.0-e*e) ;
		Tj = MTdrB20(i, e1, e2, NULL, &aj) ;
		dLj= MdLTB20(i, Tj, N, pj) ;
		ddL= dL-dLj ; if (fabs(ddL)<epsdL) break ;
		aj = aj+MdaddLC00(aj, N, ddL) ;
		count++ ; if (count>100) { rc =-22 ; goto error ; }
    }
error: ;
    T = Tj ; p = pj ; 
	uhe = 0 ;
    return rc ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int MBuildEL_hs(double i, double  hs, double e1, double e2, double& T, double& p, char* Msg)
{
    if ( NotVali_i(i, Msg )) return 1 ;
    if ( NotVali_H(hs, Msg)) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, hmin, hmax, hj, dhj ;
    aj = hs+6371.0 ;
    while (1) {
		pj = aj*(1.0-e1*e1-e2*e2) ;
		MHminmax(i, pj, e1, e2, hmin, hmax, 0, 0) ;
		hj = 0.5*(hmin+hmax) ;
		dhj= hs-hj ; if (fabs(dhj)<epsh) break ;
		aj = aj+dhj ;
		count++ ; if (count>100) return-22 ;
    }
    p = pj ; 
    T = MTdrB20(i, e1, e2, &p, 0) ;
    return 0 ;
}

//------------------------------------------------------------------------------
// Проектирование СС ОМИВ
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSOMIV_a(double a, double& i, double& p, double& e1, double& e2, char* Msg)
{
    if ( NotVali_a ( a, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, ej, ij, Tj ;//, dT ;
    ej = 0 ; pj = aj = a ;
    Tj = MTAC00(  a );
    while ( 1 ) {
      ij = MissTp( pj, Tj ) ;
      ej = ME1B20( ij, pj ) ;
      pj = aj * ( 1.0 - ej*ej ) ;
      Tj = MTdrB20( ij, ej, 0, &pj, 0 ) ;
      aj = pj / ( 1.0 - ej*ej ) ;
      if ( fabs( a - aj ) < epsh ) break ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    i = ij ; p = pj ; e1 = ej ; e2 = 0 ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSOMIV_T(double T, double& i, double& p, double& e1, double& e2, char* Msg)
{
    if ( NotVali_T ( T, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, ej, ij, Tj, dT ;
    ej = 0 ;
    aj = MATC00( T ) ;
    while ( 1 ) {
      pj = aj * ( 1.0 - ej*ej ) ;
      ij = MissTp( pj, T ) ;
      ej = ME1B20( ij, pj ) ;
      Tj = MTdrB20( ij, ej, 0, &pj, 0 ) ;
      dT = T - Tj ; if ( fabs( dT ) < epsT ) break ;
      aj = aj + MdadTC00( dT, aj ) ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    i = ij ; p = pj ; e1 = ej ; e2 = 0 ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSOMIV_NsNv(double Ns, double Nv, double& T, double& i,
                       double& p, double& e1, double& e2, char* Msg )
{
    if ( NotVali_Ns( Ns, Msg ) ) return 1 ;
    if ( NotVali_Nv( Nv, Msg ) ) return 1 ;
    //
    int    count = 0, rc = 0 ;
    double Ti, d = 0 ;
    //
    while ( 1 ) {
      if ( count == 100 ) return-22 ;
      if ( count ) Ti = T ;
      T = 360.0 / kb_omega * ( Ns / Nv - d ) * 86400.0 ;
	  rc = MBuildSSOMIV_T(T, i, p, e1, e2, Msg); if (rc) return rc;
      if ( count && fabs( Ti - T ) < epsT ) break ;
      d = Cos( i, 1 ) * kb_eps / kb_mu / p / p ;
      count++ ;
    }
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSOMIV_hs(double  hs, double& T, double& i, double& p,
                     double& e1, double& e2, char* Msg )
{
    if ( NotVali_H ( hs, Msg ) ) return 1 ;
    //
    int    k, count = 0 ;
    double si, ski ;
    double aj, pj, ej, ij, Tj, hj, dh ;
    double k_em = kb_eps / kb_mu ;
    ej = 0 ;
    aj = hs + 6371.0 ;
    Tj = MTAC00( aj ) ;
    while ( 1 ) {
      for ( k = 0 ; k < 10 ; k++ ) {
        pj = aj * ( 1.0 - ej*ej ) ;
        ij = MissTp( pj, Tj ) ;
        ej = ME1B20( ij, pj ) ;
        Tj = MTdrB20( ij, ej, 0, &pj, 0 ) ;
      }
      si = Sin( ij ) ; ski = si*si ;
      hj = pj - k_em / pj * ( 1.- 0.5 * ski )- kb_a0 *( 1.- 0.5 * k_alfa0 * ski ) ;
      dh = hs - hj ; if ( fabs( dh ) < epsh ) break ;
      aj = aj + dh ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    T = Tj, i = ij ; p = pj ; e1 = ej ; e2 = 0 ;
    return 0 ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSOMIV_dL(double N, double dL, double& T, double& i, double& p,
                    double& e1, double& e2, char* Msg )
{
    if ( NotVali_N ( N, Msg ) ) return 1 ;
    if ( NotVali_dL( dL, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, ej, ij, dLj, ddL, Tj ;
    ej = 0 ;
    aj = MAdLC00( i, dL, N ) ;
//    aj = MAdLC00( dL, N ) ;
    Tj = MTAC00( aj ) ;
    while ( 1 ) {
      pj = aj * ( 1.0 - ej*ej ) ;
      ij = MissTp( pj, Tj ) ;
      ej = ME1B20( ij, pj ) ;
      Tj = MTdrB20( ij, ej, 0, &pj, 0 ) ;
      dLj= MdLTB20( ij, Tj, N, pj ) ;
      ddL= dL - dLj ; if ( fabs( ddL ) < epsdL ) break ;
      aj = aj + MdaddLC00( aj, N, ddL ) ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    T = Tj, i = ij ; p = pj ; e1 = ej ; e2 = 0 ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSOMIV_i(double i, double& T, double& p, double& e1, double& e2, char* Msg)
{
    if ( NotVali_i ( i, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, ej, ij, dij, Tj ;
    ej = 0 ;
    aj = Massi( i ) ;
    while ( 1 ) {
      pj = aj * ( 1.0 - ej*ej ) ;
      ej = ME1B20( i, pj ) ;
      Tj = MTdrB20( i, ej, 0, &pj, 0 ) ;
      ij = MissTp( pj, Tj ) ;
      dij= i - ij ; if ( fabs( dij ) < epsi ) break ;
      aj = aj + MdadciC00( aj, Cos( i ) - Cos( ij ) ) ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    T = Tj, p = pj ; e1 = ej ; e2 = 0 ;
    return 0 ;
}
//------------------------------------------------------------------------------
// Проектирование СС ДУО
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSDU_a(double a, double& T, double& i, double& p, double& e1, double& e2, char* Msg)
{
    if ( NotVali_a ( a, Msg ) ) return 1 ;
//    int    count = 0 ;
    double aj, pj, e1j, e2j, ej, ij, Tj ;//, da ;
    aj = a ;
    e1j= 0 ; e2j= 0 ;
    Tj = MTAC00( aj ) ;
//    while ( 1 ) {
     pj = aj * ( 1.0 - e1j*e1j - e2j*e2j ) ;
     ij = MissTp( pj, Tj ) ;
     ME1E2iB70( ij, aj, e1j, e2j ) ;
     ej = sqrt( e1j*e1j + e2j*e2j ) ;
     pj = aj * ( 1.0 - e1j*e1j - e2j*e2j ) ;
     aj = pj / ( 1.0 - ej*ej ) ;
     Tj = MTdrB20( ij, e1j, e2j, &pj, 0 ) ;
//     da = a - aj ; if ( fabs( da ) < epsh ) break ;
//     count++ ; if ( count > 100 ) return-22 ;
//    }
    //
    T = Tj, i = ij ;p = pj ; e1 = e1j ; e2 = e2j ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSDU_T(double T, double& i, double& p, double& e1, double& e2, char* Msg)
{
    if ( NotVali_T ( T, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, e1j, e2j, ej, ij, Tj, dT ;
    e1j= 0 ; e2j= 0 ;
    aj = MATC00( T ) ;
    while ( 1 ) {
      pj = aj * ( 1.0 - e1j*e1j - e2j*e2j ) ;
      ij = MissTp( pj, T ) ;
      ME1E2iB70( ij, aj, e1j, e2j ) ;
      ej = sqrt( e1j*e1j + e2j*e2j ) ;
      pj = aj * ( 1.0 - ej*ej ) ;
      Tj = MTdrB20( ij, e1j, e2j, &pj, 0 ) ;
//      Tj = MTdrB20( ij, ej, 0, &pj, 0 ) ;
      dT = T - Tj ; if ( fabs( dT ) < epsT ) break ;
      aj = aj + MdadTC00( dT, aj ) ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    i = ij ; p = pj ; e1 = e1j ; e2 = e2j ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSDU_NsNv(double Ns, double Nv, double& T, double& i, double& p,
                     double& e1, double& e2, char* Msg )
{
    if ( NotVali_Ns( Ns, Msg ) ) return 1 ;
    if ( NotVali_Nv( Nv, Msg ) ) return 1 ;
    //
    int    count = 0, rc = 0 ;
    double Ti, d = 0 ;
    //
    while ( 1 ) {
      if ( count == 100 ) return-22 ;
      if ( count ) Ti = T ;
      T = 360.0 / kb_omega * ( Ns / Nv - d ) * 86400.0 ;
	  rc = MBuildSSDU_T(T, i, p, e1, e2, Msg); if (rc) return rc;
      if ( count && fabs( Ti - T ) < epsT ) break ;
      d = Cos( i, 1 ) * kb_eps / kb_mu / p / p ;
      count++ ;
    }
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSDU_hs(double hs, double& T, double& i, double& p,
                   double& e1, double& e2, char* Msg )
{
    if ( NotVali_H ( hs, Msg ) ) return 1 ;
    //
    int    k, count = 0 ;
    double hmin, hmax ;
    double aj, pj, ej, e1j, e2j, hj, dh, Tj, ij ;
    ej = 0 ; e1j = 0 ; e2j = 0 ;
    aj = hs + 6371.0 ;
    Tj = MTAC00( aj ) ;
    while ( 1 ) {
      for ( k = 0 ; k < 10 ; k++ ) {
        pj = aj * ( 1.0 - ej*ej ) ;
        ij = MissTp( pj, Tj ) ;
        ME1E2iB70( ij, aj, e1j, e2j ) ;
        ej = sqrt( e1j*e1j + e2j*e2j ) ;
        pj = aj * ( 1.0 - ej*ej ) ;
        Tj = MTdrB20( ij, e1j, e2j, &pj, 0 ) ;
//        Tj = MTdrB20( ij, ej, 0, &pj, 0 ) ;
      }
      MHminmax( ij, pj, e1j, e2j, hmin, hmax, 0, 0 ) ;
      hj = 0.5 * ( hmin + hmax ) ;
      dh = hs - hj ; if ( fabs( dh ) < epsh ) break ;
      aj = aj + dh ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    T = Tj, i = ij ; p = pj ; e1 = e1j ; e2 = e2j ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSDU_dL(double N, double dL, double& T, double& i, double& p,
                double& e1, double& e2, char* Msg )
{
    if ( NotVali_N ( N, Msg ) ) return 1 ;
    if ( NotVali_dL( dL, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, ej, e1j, e2j, ij, dLj, ddL, Tj ;
    ej = 0 ; e1j = 0 ; e2j = 0 ;
    aj = MAdLC00( i, dL, N ) ;
//    aj = MAdLC00( dL, N ) ;
    Tj = MTAC00( aj ) ;
    while ( 1 ) {
      pj = aj * ( 1.0 - ej*ej ) ;
      ij = MissTp( pj, Tj ) ;
      ME1E2iB70( ij, aj, e1j, e2j ) ;
      ej = sqrt( e1j*e1j + e2j*e2j ) ;
      pj = aj * ( 1.0 - ej*ej ) ;
      Tj = MTdrB20( ij, e1j, e2j, &pj, 0 ) ;
//      Tj = MTdrB20( ij, ej, 0, &pj, 0 ) ;
      ij = MissTp( pj, Tj ) ;
      dLj= MdLTB20( ij, Tj, N, pj ) ;
      ddL= dL - dLj ; if ( fabs( ddL ) < epsdL ) break ;
      aj = aj + MdaddLC00( aj, N, ddL ) ;
      count++ ; if ( count > 500 ) return-22 ;
    }
    T = Tj, i = ij ; p = pj ; e1 = e1j ; e2 = e2j ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSDU_i(double i, double& T, double& p, double& e1, double& e2, char* Msg)
{
    if ( NotVali_i( i, Msg ) ) return 1 ;
    //
    int    count = 0 ;
    double aj, pj, ej, e1j, e2j, ij, dij, Tj ;
    ej = 0 ; e1j = 0 ; e1j = 0 ;
    aj = Massi( i ) ;
    ij = i ;
    while ( 1 ) {
      ME1E2iB70( ij, aj, e1j, e2j ) ;
      ej = sqrt( e1j*e1j + e2j*e2j ) ;
      pj = aj * ( 1.0 - ej*ej ) ;
      Tj = MTdrB20( ij, e1j, e2j, &pj, 0 ) ;
//      Tj = MTdrB20( i, ej, 0, &pj, 0 ) ;
      pj = aj * ( 1.0 - ej*ej ) ;
      ij = MissTp( pj, Tj ) ;
      dij= i - ij ; if ( fabs( dij ) < epsi ) break ;
      aj = aj + MdadciC00( aj, Cos( i ) - Cos( ij ) ) ;
      count++ ; if ( count > 100 ) return-22 ;
    }
    T = Tj, p = pj ; e1 = e1j ; e2 = e2j ;
    return 0 ;
}
//------------------------------------------------------------------------------
// Проектирование СС элиптической орбиты
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSEL_T(double T, double he, int Varh, double Bhe, int VarBh,
                  double& i, double& p, double& e1, double& e2, double& uhe, char* Msg )
{
    if ( Msg ) Msg[ 0 ] = '\0' ;
    if ( NotVali_T ( T, Msg ) ) return 1 ;
    if ( NotVali_h ( he, Msg ) ) return 1 ;
    if ( NotVali_Bh( Bhe, Msg ) ) return 1 ;
    //
    int    rc, count = 0 ;
    double d1, d2, k1, k2, rhe ;
    double aj, ej , e1j, e2j, ij, pj, Tj, dT ;
    double sbh= sin( RadA( Bhe ) - k_alfa0 * Sin( 2 * Bhe ) ) ;
    rhe= he + kb_a0 * ( 1.0 - k_alfa0 * sbh*sbh ) ;
    pj = aj = MATC00( T ) ;
    while ( 1 ) {
      ij = MissTp( pj, T ) ;
      MD12K12( ij, he, Bhe, VarBh, d1, d2, k1, k2, rhe, uhe ) ;
      rc = ME1E2TpD12K12( ij, pj, d1, d2, k1, k2, e1j, e2j, aj, Tj ) ;
      if ( rc ) { rc =-22 ; goto error ; }
      dT = T - Tj ; if ( fabs( dT ) < epsT ) break ;
      aj = aj + MdadTC00( dT, aj ) ;
      ej = sqrt( e1j*e1j + e2j*e2j ) ;
      pj = aj * ( 1.0 - ej*ej ) ;
      count++ ; if ( count > 50 ) { rc =-22 ; goto error ; }
    }
    if ( isNotValiOrb( ij, pj, e1j, e2j, he, Varh, uhe ) ) {
      rc=-10 ;
error : ;
      if ( Msg ) sprintf( Msg, "Данная орбита не существует" ) ;
      return rc ;
    }
    i = ij, p = pj ; e1 = e1j ; e2 = e2j ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSEL_NsNv(double Ns, double Nv, double he, bool Varh, double Bhe,
                     bool VarBh, double& T, double& i, double& p, double& e1,
                     double& e2, double& uhe, char* Msg )
{
    if ( NotVali_Ns( Ns, Msg ) ) return 1 ;
    if ( NotVali_Nv( Nv, Msg ) ) return 1 ;
    //
    int    count = 0, rc = 0 ;
    double Ti, d = 0 ;
    //
    while ( 1 ) {
      if ( count == 100 ) return-22 ;
      if ( count ) Ti = T ;
      T = 360.0 / kb_omega * ( Ns / Nv - d ) * 86400.0 ;
      p = MATC00( T ) ;
      if ( count && fabs( Ti - T ) < epsT ) break ;
      d = Cos( i, 1 ) * kb_eps / kb_mu / p / p ;
      count++ ;
    }
      rc = MBuildSSEL_T( T, he, Varh, Bhe, VarBh, i, p, e1, e2, uhe, Msg ) ;
	  if (rc) return rc;
      rc = MBuildSSEL_T( T, he, Varh, Bhe, VarBh, i, p, e1, e2, uhe, Msg ) ;
	  if (rc) return rc;
    return 0 ;

}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSEL_dL(double N, double dL, double he, int Varh, double Bhe,
                   int VarBh, double& T, double& i, double& p, double& e1,
                   double& e2, double& uhe, char* Msg )
{
    if ( Msg ) Msg[ 0 ] = '\0' ;
    if ( NotVali_N ( N, Msg ) ) return 1 ;
    if ( NotVali_dL( dL, Msg ) ) return 1 ;
    if ( NotVali_h ( he, Msg ) ) return 1 ;
    if ( NotVali_Bh( Bhe, Msg ) ) return 1 ;
    //
    int    rc, count = 0 ;
    double d1, d2, k1, k2, rhe ;
    double aj, pj, Tj, ij, dLj, ddL ;
    double ej=0, e1j=0, e2j=0 ;
    aj = MAdLC00( i, dL, N ) ;
//    aj = MAdLC00( dL, N ) ;
    Tj = MTAC00( aj ) ;
    while ( 1 ) {
      pj = aj * ( 1.0 - ej*ej ) ;
      ij = MissTp( pj, Tj ) ;
      MD12K12( ij, he, Bhe, VarBh, d1, d2, k1, k2, rhe, uhe ) ;
      rc = ME1E2TpD12K12( ij, pj, d1, d2, k1, k2, e1j, e2j, aj, Tj ) ;
      if ( rc ) { rc =-22 ; goto error ; }
      ej = sqrt( e1j*e1j + e2j*e2j ) ;
      dLj= MdLTB20( ij, Tj, N, pj ) ;
      ddL= dL - dLj ; if ( fabs( ddL ) < epsdL ) break ;
      aj = aj + MdaddLC00( aj, N, ddL ) ;
      count++ ; if ( count > 500 ) { rc =-22 ; goto error ; }
    }
    if ( isNotValiOrb( ij, pj, e1j, e2j, he, Varh, uhe ) ) {
      rc =-10 ;
error : ;
      if ( Msg ) sprintf( Msg, "Данная орбита не существует" ) ;
      return rc ;
    }
    T = Tj ; i = ij ; p = pj ; e1 = e1j ; e2 = e2j ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSEL_i(double i, double he, int Varh, double Bhe, int VarBh,
                  double& T, double& p, double& e1, double& e2, double& uhe, char* Msg )
{
    if ( Msg ) Msg[ 0 ] = '\0' ;
    if ( NotVali_i ( i, Msg ) ) return 1 ;
    if ( NotVali_h ( he, Msg ) ) return 1 ;
    if ( NotVali_Bh( Bhe, Msg ) ) return 1 ;
    //
    int    rc, count = 0 ;
    double d1, d2, k1, k2, rhe ;
    double aj, pj, ej, e1j, e2j, ij, dij, Tj ;
    pj = aj = he + 6371.0 ;
    MD12K12( i, he, Bhe, VarBh, d1, d2, k1, k2, rhe, uhe ) ;
    while ( 1 ) {
      rc = ME1E2TpD12K12( i, pj, d1, d2, k1, k2, e1j, e2j, aj, Tj ) ;
      if ( rc ) { rc =-22 ; goto error ; }
      ej = sqrt( e1j*e1j + e2j*e2j ) ;
      aj = pj / ( 1.0 - ej*ej ) ;
      Tj = MTdrB20( i, ej, 0, &pj, 0 ) ;
      ij = MissTp( pj, Tj ) ;
      dij= i - ij ; if ( fabs( dij ) < epsi ) break ;
      pj = pj + MdadciC00( pj, Cos( i ) - Cos( ij ) ) ;
      count++ ; if ( count > 500 ) { rc =-22 ; goto error ; }
    }
    if ( isNotValiOrb( i, pj, e1j, e2j, he, Varh, uhe ) ) {
      rc =-10 ;
error : ;
      if ( Msg ) sprintf( Msg, "Данная орбита не существует" ) ;
      return rc ;
    }
    T = Tj, p = pj ; e1 = e1j ; e2 = e2j ;
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSEL_a(double a, double  e, double w, double& T, double& p,
                  double& e1, double& e2, double& uhe, char* Msg )
{
    if ( NotVali_a ( a, Msg ) ) return 1 ;
    if ( NotVali_e ( e, Msg ) ) return 1 ;
    if ( NotVali_w ( w, Msg ) ) return 1 ;
    //
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MBuildSSEL_iew(double i, double  e, double w, double& T, double& p,
                    double& e1, double& e2, double& uhe, char* Msg )
{
    if ( NotVali_e ( e, Msg ) ) return 1 ;
    if ( NotVali_w ( w, Msg ) ) return 1 ;
    if ( NotVali_i ( i, Msg ) ) return 1 ;
    return 0 ;
}
//------------------------------------------------------------------------------
