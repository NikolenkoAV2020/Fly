//-------------------------------------------------------------------------------
// ZFDUCalc.cpp
// �. ��������� 16.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZCDU.h>
#include <FlyCoreSource\\ZFDUCalc.h>

//------------------------------------------------------------------------------
// ������ ������������ ��������� ������� �� ���������
// var :											in
//   1  - �� ������� ������ �� �� ������� ����
//   2  - �� dV � ������ �������� �������������
//   3  - �� dV ��� ����� �������� �������������
//   4  - �� ������� �������
//   5  - �� ��������� ������� ������ ��
// m0   - ��������� ����� ��						in
// du   - �������������� ��							in
// pdV1 - Vx ��� �������� �������������             in-out
// pdV2 - Vx � �������� �������������				in-out
// pdt1 - ������������ ������ �� ������� ����		in-out
// pdt2 - ��������� ������������ ������    	     	in-out
// pdms - ��������� ������ �������					in-out
// pmk  - �������� ����� ��							out
// tm   - ������������  ��������� �������			out
//------------------------------------------------------------------------------
int ZFPUCalcTimeMain(int var, double m0, ZCDU& du, double* pdV1, double* pdV2, 
					 double* pdt1, double* pdt2, double* pdms, double* pmk, 
					 double& tm, char* errmsg)
{
    double dV1 = pdV1?*pdV1:0 ;  // Vx ��� ����� �������� �������������
	double dV2 = pdV2?*pdV2:0 ;  // Vx � ������ �������� �������������
	double dts1= pdt1?*pdt1:0 ;  // ������������ ������ �� ��� �������� �������������
	double dts2= pdt2?*pdt2:0 ;  // ������������ ������ �� � ��������� �������������
	double dms = pdms?*pdms:0 ;  // ��������� ������ �������
	double mk  = pmk?*pmk:0 ;    // �������� �����
    double gekv ;
	double epsdV ;
	int    errCode = 0 ;
    //
    if (du.k==0) { errCode = 9 ; goto end ; }
    if (du.k==1 && du.i[0].ms==0 && du.i[0].p==0 && du.i[0].pu==0) { errCode = 9 ; goto end ; }
    if (du.isNotValiDU) { errCode = 8 ; goto end ; }
	// ��� �������� � �������� � ������
	gekv = 9.80665 ;
	epsdV= 1e-12 ;
	// ��� �������� � ������ ������� ������
    if (du.isDimKmS) {
      // ��� �������� � ��� � ��
	  epsdV = epsdV*1e4 ;
      gekv  = gekv/1e3  ;
    } else {
      // ��� �������� � ��� � ����
	  epsdV = epsdV*0.00864 ;
      gekv  = gekv*8.64*86.4 ;
    }
	//
	if (m0<=0) { errCode = 4 ; goto end ; }
	//-------------------------------------------------------------------------
	// �� ������� ������ ��
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
        i    = du.mu ;							// ������ ��������� �������
        tm   = (var==1?dts1:dts2) ;				// ����� ������ ��	
        dts1 = dts2 = tm ;						// ��� �� ������ �� ����� �� �������� �������������
		ms   = du.i[i].ms ;			
	    dm   = ms*tm ;							// ������ ������� ����� ��������� �������
	    dV   =-du.i[i].pu*gekv*log(1-ms*tm/m) ;	// ������������������ �������� ��������
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
		// �� ������� ������ �� ������� ���� �� 
        tm   = dts1-tv ;	// ������������ ��������� �������
        dts2 = dts1+ts ;
      } else {
		// �� ���������� ������� � ������ �������� �������������
        tm   = dts2-tv-ts ;	// ������������ ��������� �������
        dts1 = dts2-ts ;
      }
	  if (tm<0) { errCode = 2 ; goto end ; }
	  if (tm>du.i[du.mu].t) { errCode = 3 ; goto end ; }
	  // ����� �� �����
	  for (i=0 ; i<du.mu ; i++) {
		ms = du.i[i].ms ;
		dm+= ms*du.i[i].t ;
		dV+=-du.i[i].pu*gekv*log(1-ms*du.i[i].t/m) ;
		m  = m0-dm ;
	    if (m<=0) { errCode = 1 ; goto end ; }
	  }
	  // �������� �������
	  ms = du.i[ i ].ms ;
	  dm+= ms*tm ;
	  dV+=-du.i[i].pu*gekv*log(1-ms*tm/m) ;
	  m  = m0-dm ;
	  if (m<=0) { errCode = 1 ; goto end ; }
	  dV1 = dV ;
	  // ������� �������������
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
	// �� dV � ������ �������� �������������
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
	// �� dV ��� ����� �������� �������������
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
	// �� ������� �������
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
        case 1 : sprintf(errmsg,"��������� �������\0") ; break ;
        case 2 : sprintf(errmsg,"�������� �������� ������ ���������� ���������� ��������\0") ; break ;
        case 3 : sprintf(errmsg,"�������� �������� ������ ����������� ���������� ��������\0") ; break ;
        case 4 : sprintf(errmsg,"������� ��������� ����� ��\0") ; break ;
        case 5 : sprintf(errmsg,"������� ������ �������\0") ; break ;
        case 6 : sprintf(errmsg,"������� �������� ������������������ ��������\0") ; break ;
        case 7 : sprintf(errmsg,"������� ������������ ������ ��\0") ; break ;
        case 8 : sprintf(errmsg,"������������ �������������� ��\0") ; break ;
        case 9 : sprintf(errmsg,"�������������� �� �� �������� �� ������ �������\0") ; break ;
        default: sprintf(errmsg,"������ �������\0") ; break ;
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
// ��������� ������ ���������� ������������������ �������� �� ������� ������������� ������ ��
// c �������� ���������� ���� � �������� ���� ��
// ���� ������:
//   0 - Ok
//  -1 - ����������� ������ ��������� �����
//  -2 - ����������� ������ ���� ��
//  -3 - ����������� ������ �������� ���� ��
//  -4 - ����������� ������ ������������ ������� �������������
//  -5 - ��������� �������
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
// ������������� ������ ��������������� ���������� �� ��������� �� 0 �� X
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
// ������������� ������ ���������� ������������������ �������� �� ������� ������������� ������ ��
// c �������� ���������� ���� � �������� ���� ��
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
// ������������� ������ ������ ����� �� ������� ������������� ������ ��
// c �������� ���������� ���� � �������� ���� ��
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
    int nV = TPU.count ;	// nV - ���-�� ���������
    int nU = DU.k ;			// nU - ���-�� ��������
   
    PU.Clear( ) ;
	PU.NumPU = TPU.N ;		// ����� ����������� ��� ��

    for (i=0 ; i<nV ; i++) {
/*
      // ���������� ���������
      VPU.Clear( ) ;
      VPU.Num_LogSc_Bas = TPU.numLhs ;
      VPU.Num_LogSc_Add = TPU.numLhs ;
      VPU.Num_SysTime   = 0 ;
      VPU.Num_Vkl       = i + 1 ;
      // ����� ���������
      VPU.Time_Beg      = TPU.t[ i ] ;
      // ����� ���������� (��������� ��������������)
      VPU.Time_End      = 0 ;
      // ��� ������� �� ���������
      VPU.Weight_Beg    = TPU.m[ i ] ;
      // ��� ������� ����� ���������� (��������� ��������������)
      VPU.Weight_End    = 0 ;
 	  // ���� ���������� ���������  (� 1 �������)
 	  VPU.Alfa_Beg    = RadA( TPU.alfa[ i ] ) ;    // ����� ��������� �� ������� ��� ���������
 	  VPU.Betta_Beg   = RadA( TPU.beta[ i ] ) ;    // ����� ��������� �� ������� ��� ���������
 	  VPU.Gamma_Beg   = 0.;        	     // ����� ��������� �� ������� ��� ���������
 	  // ���� ���������� �������� (��������� ��������������)
 	  VPU.Alfa_End  = 0.;      	         // ���� ����� �������� �� ��������� (����� ����. �������)
 	  VPU.Betta_End = 0.;      	         // ���� �����
 	  VPU.Gamma_End = 0.;      	         // ���� �����
 	  // ���� ���������� ��������� (�� ��)
 	  VPU.Alfa_N    = 0.;        	     // ����� ���������� ���������������
 	  VPU.Betta_N   = 0.;        	     // ����� ���������� ���������������
 	  VPU.Gamma_N   = 0.;        	     // ����� ���������� ���������������
 	  // ���� ���������� "��������������" (�� ��)
 	  VPU.Alfa_0  = 0.;      	         // ���� ����� ��������� �� ��������� (������ 1 �������)
 	  VPU.Betta_0 = 0.;      	         // ���� �����
 	  VPU.Gamma_0 = 0.;      	         // ���� �����
      // ��� ������ �� � ����� ��������� (�� ��)
 	  VPU.Type_Orb_Prev = TPU.TypeOrb[ i ] ;
      VPU.Type_Orb_Next = TPU.TypeOrb[ i + 1 ] ;
      //
 	  // ������ ������� ���������� �������� (0-����, �������� ��������� ����)
 	  VPU.Type_Operat   = 1 ;
 	  // ����� ������� ���������� ���� �� (�� ��)
 	  VPU.Num_SysOrient = TPU.sk[ i ] ;
 	  // ����� ������� ������� ������� (��� ��������� �������� ����)
 	  VPU.Num_SysTime   = 0 ;
	  // ������ ������� ���������� ��������
	  // 0 - ����, �������� ��������� ����
	  // 1 - �������� ���������� ������� ����,
	  //     �������� ������� ��������
	  VPU.Type_Orient   = 0 ;
	  // ������� ����� �������� ����������� ���������
	  VPU.Att_Prev_Vkl  = 0 ;
	  // ������� ������� ������� ����������
	  VPU.Att_Calc_Matr = 0 ;
	  // ������� ���������� ��������� ��������
	  // 0 - ������������ ��������� ���� ��, � ��
	  // 1 - ���������� ��������� ���� ��, � �/�
	  VPU.Att_Impuls_V  = 0 ;
	  // ������� ������������� ����� ���������� �������� �� ��������
	  // 0 - ����������� ����������� (������) �����
	  // 1 - ���� ���������� �������� ������������ ��������� ����� ���������
	  //     (����� 1 �������)
	  VPU.Att_Continuity_Angle = 1 ;
      // ���������� ��������
      double tau, t, m ;
      t = TPU.t[ i ] ;
      m = TPU.m[ i ] ;
      bool isMain = ( DU.isOnlyMain || TPU.isOnlyMain[ i ] ) ;
      nU = isMain ? DU.mu + 1 : DU.k ;
      for ( j = ( isMain ? DU.mu : 0 ) ; j < nU ; j++ ) {
        UPU.Clear( ) ;
        UPU.Num_Uch    = ( isMain ? 1 : j + 1 ) ;  // ����� �������
        UPU.Dlt_Weight = 0.0 ;     		   // ����� (������) ����� (����)
        UPU.Alfa       = RadA( TPU.alfa[ i ] ) ; //
        UPU.Betta      = RadA( TPU.beta[ i ] ) ; //
        UPU.Gamma      = 0 ;               //
        UPU.Alfa_t     = 0 ;               //
        UPU.Betta_t    = 0 ;               //
        UPU.Gamma_t    = 0 ;               //
        tau = ( j == DU.mu ? TPU.dtm[ i ] : DU.i[ j ].t ) ;
        UPU.Weight     = m ;
        UPU.Time       = t ;   	           // ����� ������ ������� � ���
        UPU.Delta_T    = tau / 86400.0 ;   // ������������ ������� � ���
        UPU.P          = DU.i[ j ].p ;     //
        UPU.Dif_P      = 0 ;               //
        UPU.Dif_W      = DU.i[ j ].p / DU.i[ j ].pu * 86400.0 ;
	    UPU.Step_N	   = UPU.Delta_T / 100.0 ; // ��� �������������� ���������
	    UPU.Step_K	   = UPU.Delta_T / 10.0 ;  // ��� �������������� ��������
        // ����� ����� ��������
        if ( j < DU.mu ) UPU.Scala = 1 ;   // ����� �� �����
        if ( j ==DU.mu ) UPU.Scala = 3 ;   // �������� �������
        if ( j > DU.mu ) UPU.Scala = 5 ;   // �������������
        m-= UPU.Delta_T * UPU.Dif_W ;
        t+= tau / 86400.0 ;
        // ��������� � ������� �� ���������
        VPU.List_Uch.push_back( UPU ) ;
      }
      // ��������� � ��������� � ��
      SPU.List_Vkl.push_back( VPU ) ;
      VPU.List_Uch.clear( ) ;
*/
    }

    return 0 ;
}

//------------------------------------------------------------------------------


