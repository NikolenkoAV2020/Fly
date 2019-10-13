//-------------------------------------------------------------------------------
// ZFDUCalc.cpp
// А. Николенко 16.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZCDU.h>
#include <FlyCoreSource\\ZFDUCalc.h>

//------------------------------------------------------------------------------
// Расчёт длительности основного участка на включении
// var :											in
//   1  - по времени работы ДУ до отсечки тяги
//   2  - по dV с учётом импульса помледействия
//   3  - по dV без учёта импульса последействия
//   4  - По расходу топлива
//   5  - по сумарному времени работы ДУ
// m0   - начальная масса КА						in
// du   - характеристики ДУ							in
// pdV1 - Vx без импульса последействия             in-out
// pdV2 - Vx с импульса последействия				in-out
// pdt1 - длительность работы до отсечки тяги		in-out
// pdt2 - суммарная длительность работы    	     	in-out
// pdms - суммарный расход топлива					in-out
// pmk  - конечная масса КА							out
// tm   - длительность  основного участка			out
//------------------------------------------------------------------------------
int ZFPUCalcTimeMain(int var, double m0, ZCDU& du, double* pdV1, double* pdV2, 
					 double* pdt1, double* pdt2, double* pdms, double* pmk, 
					 double& tm, char* errmsg)
{
    double dV1 = pdV1?*pdV1:0 ;  // Vx без учёта импульса последействия
	double dV2 = pdV2?*pdV2:0 ;  // Vx с учётом импульса последействия
	double dts1= pdt1?*pdt1:0 ;  // длительность работы ДУ без импульса последействия
	double dts2= pdt2?*pdt2:0 ;  // длительность работы ДУ с импульсом последействия
	double dms = pdms?*pdms:0 ;  // суммарный расход топлива
	double mk  = pmk?*pmk:0 ;    // конечная масса
    double gekv ;
	double epsdV ;
	int    errCode = 0 ;
    //
    if (du.k==0) { errCode = 9 ; goto end ; }
    if (du.k==1 && du.i[0].ms==0 && du.i[0].p==0 && du.i[0].pu==0) { errCode = 9 ; goto end ; }
    if (du.isNotValiDU) { errCode = 8 ; goto end ; }
	// Для расчётов в секундах и метрах
	gekv = 9.80665 ;
	epsdV= 1e-12 ;
	// Для расчётов в нужной системе единиц
    if (du.isDimKmS) {
      // Для расчётов в сек и км
	  epsdV = epsdV*1e4 ;
      gekv  = gekv/1e3  ;
    } else {
      // Для расчётов в ссс и дткм
	  epsdV = epsdV*0.00864 ;
      gekv  = gekv*8.64*86.4 ;
    }
	//
	if (m0<=0) { errCode = 4 ; goto end ; }
	//-------------------------------------------------------------------------
	// по времени работы ДУ
	if (var==1 || var==5) {
	  int i ;
      double tv, ts, dV, m, dm, ms ;
	  if (var==1 && dts1<=0) { errCode = 7 ; goto end ; }
	  if (var==5 && dts2<=0) { errCode = 7 ; goto end ; }
	  tv = 0 ;
      ts = 0 ;
	  tm = 0 ;
	  dV = 0 ;
	  m  = m0 ;
	  dm = 0 ;
      if (du.isOnlyMain) {
        i    = du.mu ;							// Индекс основного участка
        tm   = (var==1?dts1:dts2) ;				// Время работы ДУ	
        dts1 = dts2 = tm ;						// Нет ни выхода на режим ни импульса последействия
		ms   = du.i[i].ms ;			
	    dm   = ms*tm ;							// Расход топлива вдоль активного участка
	    dV   =-du.i[i].pu*gekv*log(1-ms*tm/m) ;	// Характеристическая скорость импульса
	    m    = m0-dm ;
	    if (m<=0) { errCode = 1 ; goto end ; }
	    dV1  = dV2 = dV ;
        dms  = dm ;
        mk   = m ;
        goto end ;
      }
	  for (i=0 ; i<du.mu ; i++) tv+= du.i[i].t ;
	  for (i=du.mu+1 ; i<du.k ; i++) ts+= du.i[i].t ;
	  if (var==1 ) {
		// По времени работы до отсечки тяги ДУ 
        tm   = dts1-tv ;	// Длительность основного участка
        dts2 = dts1+ts ;
      } else {
		// По суммарному времени с учётом импульса последействия
        tm   = dts2-tv-ts ;	// Длительность основного участка
        dts1 = dts2-ts ;
      }
	  if (tm<0) { errCode = 2 ; goto end ; }
	  if (tm>du.i[du.mu].t) { errCode = 3 ; goto end ; }
	  // Выход на режим
	  for (i=0 ; i<du.mu ; i++) {
		ms = du.i[i].ms ;
		dm+= ms*du.i[i].t ;
		dV+=-du.i[i].pu*gekv*log(1-ms*du.i[i].t/m) ;
		m  = m0-dm ;
	    if (m<=0) { errCode = 1 ; goto end ; }
	  }
	  // Основной участок
	  ms = du.i[ i ].ms ;
	  dm+= ms*tm ;
	  dV+=-du.i[i].pu*gekv*log(1-ms*tm/m) ;
	  m  = m0-dm ;
	  if (m<=0) { errCode = 1 ; goto end ; }
	  dV1 = dV ;
	  // Импульс последейчтвия
      for (i=du.mu+1 ; i<du.k ; i++) {
	    ms = du.i[i].ms ;
	    dm+= ms*du.i[i].t ;
	    dV+=-du.i[i].pu*gekv*log(1-ms*du.i[i].t/m) ;
	    m  = m0-dm ;
	    if (m<=0) { errCode = 1 ; goto end ; }
	  }
	  dV2 = dV ; mk = m ; dms = dm ; goto end ;
	}
	//-------------------------------------------------------------------------
	// по dV с учётом импульса помледействия
	if ( var == 2 ) {
	  if ( dV2 <= 0 ) { errCode = 6 ; goto end ; }
	  if ( du.ip == 0 || du.isOnlyMain ) {
		dV1 = dV2 ;	var = 3 ; goto var3 ;
	  }
	  int i ;
      double dV, dVv, dVip, dVip2, tv, m, dm, ms ;
	  dVip= 0 ;
var2 : ;
	  m   = m0 ;
	  dm  = 0 ;
	  tv  = 0 ;
	  dVv = 0 ;
	  //
	  for ( i = 0 ; i < du.mu ; i++ ) {
		ms  = du.i[ i ].ms ;
		dm += ms * du.i[ i ].t ;
		tv += du.i[ i ].t ;
		dVv+=-du.i[ i ].pu * gekv * log( 1 - ms * du.i[ i ].t / m ) ;
		m   = m0 - dm ;
	    if ( m <= 0 ) { errCode = 1 ; goto end ; }
	  }
	  dV = dV2 - dVv - dVip ;
	  if ( dV < 0 ) { errCode = 2 ; goto end ; }
	  //
	  ms = du.i[ i ].ms ;
	  tm = m / ms * ( 1 - exp( -dV * ms / du.i[ i ].p / gekv ) ) ;
	  if ( tm > du.i[ du.mu ].t ) { errCode = 3 ; goto end ; }
	  dm += ms * tm ;
	  dts1= tv + tm ;
	  m   = m0 - dm ;
	  if ( m <= 0 ) { errCode = 1 ; goto end ; }
	  //
      i++ ;
      dVip2 = 0 ;
      dts2  = dts1 ;
	  for ( i = du.mu + 1 ; i < du.k ; i++ ) {
	    ms    = du.i[ i ].ms ;
        dVip2+= -du.i[ i ].pu * gekv * log( 1 - ms * du.i[ i ].t / m ) ;
        dts2 += du.i[ i ].t ;
        dm   += ms * du.i[ i ].t ;
        m  = m0 - dm ;
        if ( m <= 0 ) { errCode = 1 ; goto end ; }
      }
	  dV1 = dV2 - dVip2 ;
	  if ( fabs( dVip2 - dVip ) > epsdV ) { dVip = dVip2 ; goto var2 ; }
	  //
	  mk  = m ; dms = dm ;  goto end ;
	}
var3 : ;
	//-------------------------------------------------------------------------
	// по dV без учёта импульса последействия
	if ( var == 3 ) {
	  int i ;
      double tv, dV, dVv, m, dm, ms ;
	  if ( dV1 <= 0 ) { errCode = 6 ; goto end ; }
	  dVv = 0 ;
	  dm  = 0 ;
	  tv  = 0 ;
	  m   = m0 ;
      if ( du.isOnlyMain ) {
        i = du.mu ;
	    ms = du.i[ i ].ms ;
	    tm = m / ms * ( 1 - exp( -dV1 * ms / du.i[ i ].p / gekv ) ) ;
        dts2 = dts1 = tm ;
        dV2  = dV1 ;
	    dm   = ms * tm ;
	    m    = m0 - dm ;
	    if ( m <= 0 ) { errCode = 1 ; goto end ; }
        dms  = dm ;
        mk   = m ;
        goto end ;
      }
	  for ( i = 0 ; i < du.mu ; i++ ) {
		ms  = du.i[ i ].ms ;
		dm += ms * du.i[ i ].t ;
		dVv+=-du.i[ i ].pu * gekv * log( 1 - ms * du.i[ i ].t / m ) ; 
		tv += du.i[ i ].t ;
		m = m0 - dm ;
	    if ( m <= 0 ) { errCode = 1 ; goto end ; }
	  }
	  //
	  dV = dV1 - dVv ;
	  if ( dV <= 0 ) { errCode = 2 ; goto end ; }
	  i = du.mu ;
	  ms = du.i[ i ].ms ;
	  tm = m / ms * ( 1 - exp( -dV * ms / du.i[ i ].p / gekv ) ) ;
	  if ( tm > du.i[ du.mu ].t ) { errCode = 3 ; goto end ; }
	  dm  += ms * tm ;
	  dts1= tv + tm ;
	  m   = m0 - dm ;
	  if ( m <= 0 ) { errCode = 1 ; goto end ; }
	  //
	  dV2 = dV1 ;
      dts2 = dts1 ;
	  for ( i = du.mu + 1 ; i < du.k ; i++ ) {
		ms   = du.i[ i ].ms ;
		dm  += ms * du.i[ i ].t ;
		dV2 +=-du.i[ i ].pu * gekv * log( 1 - ms * du.i[ i ].t / m ) ;
        dts2+= du.i[ i ].t ;
		m    = m0 - dm ;
	    if ( m <= 0 ) { errCode = 1 ; goto end ; }
	  }
	  mk = m ; dms = dm ; goto end ;
	}
	//-------------------------------------------------------------------------
	// По расходу топлива
	if ( var == 4 ) {
	  int i ;
      double m, dm, dmv, dmi, ms ;
	  if ( dms <= 0  ) { errCode = 5 ; goto end ; }
	  if ( dms >= m0 ) { errCode = 1 ; goto end ; }
      if ( du.isOnlyMain ) {
        i    = du.mu ;
	    ms   = du.i[ i ].ms ;
	    tm   = dms / ms ;
        dm   = ms * tm ;
	    m    = m0 - dm ;
	    if ( m <= 0 ) { errCode = 1 ; goto end ; }
        mk   = m ;
        dts2 = dts1 = tm ;
	    if ( 1 - ms * tm / m0 <= 0 ) { errCode = 1 ; goto end ; }
		dV1  =-du.i[ i ].pu * gekv * log( 1 - ms * tm / m0 ) ;
        dV2  = dV1 ;
        goto end ;
      }
	  m   = m0 ;
	  dm  = 0 ;
	  dmv = 0 ;
	  dmi = 0 ;
	  //
	  for ( i = 0 ; i < du.mu ; i++ ) {
		ms  = du.i[ i ].ms ;
		dmv+= ms * du.i[ i ].t ;
		m   = m0 - dmv ;
	    if ( m <= 0 ) { errCode = 1 ; goto end ; }
	  }
	  for ( i = du.mu + 1 ; i < du.k ; i++ ) {
		ms  = du.i[ i ].ms ;
		dmi+= ms * du.i[ i ].t ;
		m   = m0 - dmv - dmi ;
	    if ( m <= 0 ) { errCode = 1 ; goto end ; }
	  }
	  dm = dms - dmv - dmi ;
	  if ( dm < 0 ) { errCode = 2 ; goto end ; }
	  i  = du.mu ;
	  ms = du.i[ i ].ms ;
	  tm = dm / ms ;
	  if ( tm > du.i[ du.mu ].t ) { errCode = 3 ; goto end ; }
	  //
	  dV1 = dV2 = dts1 = dts2 = 0 ;
	  //
	  m = m0 ;
	  for ( i = 0 ; i < du.mu ; i++ ) {
		ms  = du.i[ i ].ms ;
		dts1+= du.i[ i ].t ;
        if ( 1 - ms * du.i[ i ].t / m <= 0 ) { errCode = 1 ; goto end ; }        
		dV1+=-du.i[ i ].pu * gekv * log( 1 - ms * du.i[ i ].t / m ) ;
		m  -= ms * du.i[ i ].t ;
        if ( m <= 0 ) { errCode = 1 ; goto end ; }
	  }
	  //
	  i	  = du.mu ;
	  ms   = du.i[ i ].ms ;
	  dts1+= tm ;
      if ( 1 - ms * tm / m <= 0 ) { errCode = 1 ; goto end ; }
	  dV1 +=-du.i[ i ].pu * gekv * log( 1 - ms * tm / m ) ;
	  m   -= ms * tm ;
      if ( m <= 0 ) { errCode = 1 ; goto end ; }
	  //
	  dV2  = dV1 ;
      dts2 = dts1 ;
	  for ( i = du.mu + 1 ; i < du.k ; i++ ) {
		ms   = du.i[ i ].ms ;
        if ( 1 - ms * du.i[ i ].t / m <= 0 ) { errCode = 1 ; goto end ; }
		dV2 +=-du.i[ i ].pu * gekv * log( 1 - ms * du.i[ i ].t / m ) ;
        dts2+= du.i[ i ].t ;
		m   -= ms * du.i[ i ].t ;
        if ( m <= 0 ) { errCode = 1 ; goto end ; }
	  }
	  if ( m <= 0 ) { errCode = 1 ; goto end ; }
	  //
	  mk  = m ; dms = m0 - mk ; goto end ;
	}
	//-------------------------------------------------------------------------
end : ;
    if ( errCode ) {
      if ( errmsg ) {
        switch ( errCode ) {
        case 1 : sprintf(errmsg,"Выгорание топлива\0") ; break ;
        case 2 : sprintf(errmsg,"Величина импульса меньше минимально возможного значения\0") ; break ;
        case 3 : sprintf(errmsg,"Величина импульса больше максимально допустимой величины\0") ; break ;
        case 4 : sprintf(errmsg,"Задайте начальную массу КА\0") ; break ;
        case 5 : sprintf(errmsg,"Задайте расход топлива\0") ; break ;
        case 6 : sprintf(errmsg,"Задайте величину характеристической скорости\0") ; break ;
        case 7 : sprintf(errmsg,"Задайте длительность работы ДУ\0") ; break ;
        case 8 : sprintf(errmsg,"Некорректные характеристики ДУ\0") ; break ;
        case 9 : sprintf(errmsg,"Характеристика ДУ не содержит ни одного участка\0") ; break ;
        default: sprintf(errmsg,"Ошибка расчёта\0") ; break ;
        }
      }
	  errCode =-(errCode+1) ;
    } else {
      if (pdV1) *pdV1 = dV1 ;
	  if (pdV2) *pdV2 = dV2 ;
	  if (pdt1) *pdt1 = dts1 ;
	  if (pdt2) *pdt2 = dts2 ;
	  if (pdms) *pdms = dms ;
	  if (pmk ) *pmk  = mk ;
	}
	return errCode ;
}
//------------------------------------------------------------------------------
int ZFPUCalcTimeMainFromTime(double m0, ZCDU& du, double* pdV1, double* pdV2,
							 double* pdt1, double* pdt2, double* pdms, double* pmk, 
							 double& tm, char* errmsg)
{
	return (ZFPUCalcTimeMain(1, m0, du, pdV1, pdV2, pdt1, pdt2, pdms, pmk, tm, errmsg));
}
//------------------------------------------------------------------------------
int ZFPUCalcTimeMainFromAlldV(double m0, ZCDU& du, double* pdV1, double* pdV2,
							  double* pdt1, double* pdt2, double* pdms, double* pmk, 
							  double& tm, char* errmsg)
{
	return (ZFPUCalcTimeMain(2, m0, du, pdV1, pdV2, pdt1, pdt2, pdms, pmk, tm, errmsg));
}
//------------------------------------------------------------------------------
int ZFPUCalcTimeMainFromdV(double m0, ZCDU& du, double* pdV1, double* pdV2,
						   double* pdt1, double* pdt2, double* pdms, double* pmk, 
						   double& tm, char* errmsg)
{
	return (ZFPUCalcTimeMain(3, m0, du, pdV1, pdV2, pdt1, pdt2, pdms, pmk, tm, errmsg));
}
//------------------------------------------------------------------------------
int ZFPUCalcTimeMainFromdG(double m0, ZCDU& du, double* pdV1, double* pdV2,
						   double* pdt1, double* pdt2, double* pdms, double* pmk, 
						   double& tm, char* errmsg)
{
	return (ZFPUCalcTimeMain(4, m0, du, pdV1, pdV2, pdt1, pdt2, pdms, pmk, tm, errmsg));
}
//------------------------------------------------------------------------------
int ZFCalcDMFromLinePPu(double P0, double PK, double Pu0, double PuK,
						double tau, double& dM) ;
int ZFCalcDVFromLinePPu(double M, double P0, double PK, double Pu0, double PuK,
						double tau, double gekv, double& dV, double& dM) ;
int ZFCalcDVFromLinePPuNumerically(double M, double P0, double PK, double Pu0,
						double PuK, double tau, double gekv, double& dV, double& dM) ;
//------------------------------------------------------------------------------
// Численный расчёт приращения характеристической скорости на участке моделирования работы ДУ
// c линейным изменением тяги и удельной тяги ДУ
// Коды ответа:
//   0 - Ok
//  -1 - некорректно задана начальная масса
//  -2 - некорректно задана тяга ДУ
//  -3 - некорректно задана удельная тяга ДУ
//  -4 - некорректно задана длительность участка моделирования
//  -5 - выгорание топлива
//------------------------------------------------------------------------------
int ZFCalcDVFromLinePPuNumerically(double M, double P0, double PK, double Pu0,
					double PuK, double tau, double gekv, double& dV, double& dM)
{
    if ( M   <=0 ) return-1 ;
    if ( P0 == 0 && Pu0 >  0 ) return-2 ;
    if ( PK == 0 && PuK >  0 ) return-2 ;
    if ( P0  > 0 && Pu0 == 0 ) return-3 ;
    if ( PK  > 0 && PuK == 0 ) return-3 ;
    if ( P0  < 0 || PK  <  0 ) return-2 ;
    if ( Pu0 < 0 || PuK <  0 ) return-3 ;
    if ( tau <=0 ) return-4 ;
    int    i = 0, k = 100 ;
    double dP  = ( PK - P0  ) / tau ;
    double dPu = ( PuK- Pu0 ) / tau ;
    double S   = tau / k ;
    double t   = 0 ;
    //
    dV = 0 ;
    dM = 0 ;
    while( 1 ) {
      if ( i >= k ) break ;
      dV += S * gekv * ( P0 + t * dP ) / ( M - t * ( P0 + t * dP ) / ( Pu0 + t * dPu ) ) ;
      dM += ( P0 + t * dP ) / ( Pu0 + t * dPu ) * S ;
      t  += S ;
      i++ ;
    }
    if ( M - dM <= 0 ) return -5 ;
    return 0 ;
}
//------------------------------------------------------------------------------
// Аналитический расчёт вспомогательных интегралов на интервале от 0 до X
//  I1 = I( 1   / ( A * x*x + B * x + C ) )
//  I2 = I( x   / ( A * x*x + B * x + C ) )
//  I3 = I( x*x / ( A * x*x + B * x + C ) )
//------------------------------------------------------------------------------
static double EpsCalc = 1e-15 ;
//------------------------------------------------------------------------------
int ZFCalcI1(double A, double B, double C, double X, double& I)
{
    double D = 4 * A * C - B * B ;
    D = sqrt( fabs( D ) ) ;
    if ( D > EpsCalc  ) {
      I = 2.0 / D * ( atan( ( 2 * A * X + B ) / D ) - atan( B / D ) ) ;
      return 0 ;
    }
    if ( D <-EpsCalc ) {
      I = 1 / D * ( log( fabs( ( 2 * A * X + B - D ) / ( 2 * A * X + B + D ) ) ) -
      log( ( B - D ) / ( B + D ) ) ) ;
      return 0 ;
    }
    I = - 2.0 * ( 1.0 / ( 2 * A * X + B ) - 1.0 / B ) ;
    return 0 ;
}
int ZFCalcI2(double A, double B, double C, double X, double& I)
{
    double I11, I12 ;
	ZFCalcI1(A, B, C, 0, I11);
	ZFCalcI1(A, B, C, X, I12);
    I = log( fabs( A * X*X + B*X + C ) ) - B * I12 - ( log( fabs( C ) ) - B * I11 ) ;
    I = I / 2.0 / A ;
    return 0 ;
}
int ZFCalcI3(double A, double B, double C, double X, double& I)
{
    double I11, I12 ;
	ZFCalcI1(A, B, C, 0, I11);
	ZFCalcI1(A, B, C, X, I12);
    I = ( X - B / 2.0 / A * log( fabs( A * X*X + B*X + C ) ) + ( B * B - 2 * A * C ) / 2.0 / A * I12 ) -
    ( -B / 2.0 / A * log( fabs( C ) ) + ( B * B - 2 * A * C ) / 2.0 / A * I11 ) ;
    I = I / A ;
    return 0 ;
}
int ZFCalcJ1(double A, double B, double X, double& J)
{
    J = 0 ;
    return 0 ;
}
int ZFCalcJ2(double A, double B, double X, double& J)
{
    J = 0 ;
    return 0 ;
}
//------------------------------------------------------------------------------
// Аналитический расчёт приращения характеристической скорости на участке моделирования работы ДУ
// c линейным изменением тяги и удельной тяги ДУ
//------------------------------------------------------------------------------
int ZFCalcDVFromLinePPu(double M, double P0, double PK, double Pu0, double PuK,
						double tau, double gekv, double& dV, double& dM)
{
    if ( M   <=0 ) return-1 ;
    if ( P0 == 0 && Pu0 >  0 ) return-2 ;
    if ( PK == 0 && PuK >  0 ) return-2 ;
    if ( P0  > 0 && Pu0 == 0 ) return-3 ;
    if ( PK  > 0 && PuK == 0 ) return-3 ;
    if ( P0  < 0 || PK  <  0 ) return-2 ;
    if ( Pu0 < 0 || PuK <  0 ) return-3 ;
    if ( tau <=0 ) return-4 ;
    double dP  = ( PK - P0  ) / tau ;
    double dPu = ( PuK- Pu0 ) / tau ;
    double A   = dP ;
    double B   =-dPu * M + P0 ;
    double C   =-M * Pu0 ;
    double D1  = dP * dPu ;
    double D2  = dP * Pu0 + dPu * P0 ;
    double D3  = P0 * Pu0 ;
    double I1, I2, I3 ;
    int    rc ;
    //
    if ( fabs( dP ) > EpsCalc ) {
		ZFCalcI1(A, B, C, tau, I1);
		ZFCalcI2(A, B, C, tau, I2);
		ZFCalcI3(A, B, C, tau, I3);
		dV =-gekv * ( D1 * I3 + D2 * I2 + D3 * I1 ) ;
    } else {
      dV = 0 ;
    }
    dM = 0 ;
	rc = ZFCalcDMFromLinePPu(P0, PK, Pu0, PuK, tau, dM); RRC;
    //
    if ( M - dM <= 0 ) return -5 ;
    return 0 ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Аналитический расчёт потери массы на участке моделирования работы ДУ
// c линейным изменением тяги и удельной тяги ДУ
int ZFCalcDMFromLinePPu(double P0, double PK, double Pu0, double PuK, 
						double tau, double& dM)
{
    if ( tau < 0 ) return-4 ;
    if ( fabs( tau ) < EpsCalc ) { dM = 0 ; return 0 ; }
    //
    double dP  = ( PK - P0  ) / tau ;
    double dPu = ( PuK- Pu0 ) / tau ;
    dM = 0 ;
    if ( dPu > EpsCalc && Pu0 > EpsCalc ) {
      dM = dP / dPu * tau + ( P0 * dPu - Pu0 * dP ) / dPu / dPu *
      log( fabs( ( Pu0 + dPu * tau ) / Pu0 ) ) ;
    } else {
      if ( dPu < EpsCalc && Pu0 > EpsCalc ) {
        dM = ( P0 * tau + dP / 2.0 * tau * tau ) / Pu0 ;
      } else {
        if ( dPu > EpsCalc && Pu0 < EpsCalc ) {
          if ( P0 < EpsCalc ) dM = dP / dPu * tau ;
          else return -3 ;
        } else return-3 ;
      }
    }
    return 0 ;
}

//------------------------------------------------------------------------------
int ZFPUBuild(ZCDU& DU, ZP& TPU, ZPU& PU)
{
    int i ;
    int nV = TPU.count ;	// nV - кол-во включений
    int nU = DU.k ;			// nU - кол-во участков
   
    PU.Clear( ) ;
	PU.NumPU = TPU.N ;		// Номер циклограммы для ПУ

    for (i=0 ; i<nV ; i++) {
/*
      // заполнение включений
      VPU.Clear( ) ;
      VPU.Num_LogSc_Bas = TPU.numLhs ;
      VPU.Num_LogSc_Add = TPU.numLhs ;
      VPU.Num_SysTime   = 0 ;
      VPU.Num_Vkl       = i + 1 ;
      // Время включения
      VPU.Time_Beg      = TPU.t[ i ] ;
      // Время выключения (результат интегрирования)
      VPU.Time_End      = 0 ;
      // Вес объекта до включения
      VPU.Weight_Beg    = TPU.m[ i ] ;
      // Вес объекта после выключения (результат интегрирования)
      VPU.Weight_End    = 0 ;
 	  // Углы ориентации начальные  (с 1 участка)
 	  VPU.Alfa_Beg    = RadA( TPU.alfa[ i ] ) ;    // Альфа начальный на участке для программы
 	  VPU.Betta_Beg   = RadA( TPU.beta[ i ] ) ;    // Бетта начальный на участке для программы
 	  VPU.Gamma_Beg   = 0.;        	     // Гамма начальный на участке для программы
 	  // Углы ориентации конечные (результат интегрирования)
 	  VPU.Alfa_End  = 0.;      	         // Угол альфа конечный на включении (конец посл. участка)
 	  VPU.Betta_End = 0.;      	         // Угол бетта
 	  VPU.Gamma_End = 0.;      	         // Угол гамма
 	  // Углы ориентации приборные (из БД)
 	  VPU.Alfa_N    = 0.;        	     // Альфа начального рассогласования
 	  VPU.Betta_N   = 0.;        	     // Бетта начального рассогласования
 	  VPU.Gamma_N   = 0.;        	     // Гамма начального рассогласования
 	  // Углы ориентации "дополнительные" (из БД)
 	  VPU.Alfa_0  = 0.;      	         // Угол альфа начальный на включении (начало 1 участка)
 	  VPU.Betta_0 = 0.;      	         // Угол бетта
 	  VPU.Gamma_0 = 0.;      	         // Угол гамма
      // Тип орбиты до и после включения (из БД)
 	  VPU.Type_Orb_Prev = TPU.TypeOrb[ i ] ;
      VPU.Type_Orb_Next = TPU.TypeOrb[ i + 1 ] ;
      //
 	  // Способ задания ориентации импульса (0-угол, скорость изменения угла)
 	  VPU.Type_Operat   = 1 ;
 	  // Номер Системы Ориентации тяги ДУ (из БД)
 	  VPU.Num_SysOrient = TPU.sk[ i ] ;
 	  // Номер Системы Отсчета Времени (Тип аргумента привязки ЦКЛГ)
 	  VPU.Num_SysTime   = 0 ;
	  // Способ задания ориентации импульса
	  // 0 - угол, скорость изменения угла
	  // 1 - проекции единичного вектора тяги,
	  //     проекции угловой скорости
	  VPU.Type_Orient   = 0 ;
	  // Признак учета поправок предыдущего включения
	  VPU.Att_Prev_Vkl  = 0 ;
	  // Признак расчета матрицы ориентации
	  VPU.Att_Calc_Matr = 0 ;
	  // Признак мгновенной отработки импульса
	  // 0 - интервальная отработка тяги ДУ, в кг
	  // 1 - мгновенная отработка тяги ДУ, в м/с
	  VPU.Att_Impuls_V  = 0 ;
	  // Признак неразрывности углов ориентации импульса на участках
	  // 0 - допускается разрывность (скачки) углов
	  // 1 - углы ориентации импульса определяются начальным углом включения
	  //     (углом 1 участка)
	  VPU.Att_Continuity_Angle = 1 ;
      // Заполнение участков
      double tau, t, m ;
      t = TPU.t[ i ] ;
      m = TPU.m[ i ] ;
      bool isMain = ( DU.isOnlyMain || TPU.isOnlyMain[ i ] ) ;
      nU = isMain ? DU.mu + 1 : DU.k ;
      for ( j = ( isMain ? DU.mu : 0 ) ; j < nU ; j++ ) {
        UPU.Clear( ) ;
        UPU.Num_Uch    = ( isMain ? 1 : j + 1 ) ;  // номер участка
        UPU.Dlt_Weight = 0.0 ;     		   // Сброс (потеря) массы (веса)
        UPU.Alfa       = RadA( TPU.alfa[ i ] ) ; //
        UPU.Betta      = RadA( TPU.beta[ i ] ) ; //
        UPU.Gamma      = 0 ;               //
        UPU.Alfa_t     = 0 ;               //
        UPU.Betta_t    = 0 ;               //
        UPU.Gamma_t    = 0 ;               //
        tau = ( j == DU.mu ? TPU.dtm[ i ] : DU.i[ j ].t ) ;
        UPU.Weight     = m ;
        UPU.Time       = t ;   	           // Время начала участка в ссс
        UPU.Delta_T    = tau / 86400.0 ;   // Длительность участка в ссс
        UPU.P          = DU.i[ j ].p ;     //
        UPU.Dif_P      = 0 ;               //
        UPU.Dif_W      = DU.i[ j ].p / DU.i[ j ].pu * 86400.0 ;
	    UPU.Step_N	   = UPU.Delta_T / 100.0 ; // Шаг интегрирования начальный
	    UPU.Step_K	   = UPU.Delta_T / 10.0 ;  // Шаг интегрирования конечный
        // Шкала учета поправок
        if ( j < DU.mu ) UPU.Scala = 1 ;   // Выход на режим
        if ( j ==DU.mu ) UPU.Scala = 3 ;   // Основной участок
        if ( j > DU.mu ) UPU.Scala = 5 ;   // Последействие
        m-= UPU.Delta_T * UPU.Dif_W ;
        t+= tau / 86400.0 ;
        // занесение в участка во включение
        VPU.List_Uch.push_back( UPU ) ;
      }
      // занесение в включения в ПУ
      SPU.List_Vkl.push_back( VPU ) ;
      VPU.List_Uch.clear( ) ;
*/
    }

    return 0 ;
}

//------------------------------------------------------------------------------


