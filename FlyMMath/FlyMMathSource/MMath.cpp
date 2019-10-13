//------------------------------------------------------------------------------
// MMath.cpp
// А. Николенко 03.08.2018
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <math.h>
#pragma hdrstop
//------------------------------------------------------------------------------
#include <FlyMMathSource\\MMath.h>
#include <FlyMMathSource\\MMathDef.h>
//------------------------------------------------------------------------------
FLYMMATH_API double EpsMZ = 1e-16;

FLYMMATH_API double MAX3(double a, double b, double c)
{
	double d = Max(a, b) ;
	return Max(d, c);
}

FLYMMATH_API double MIN3(double a, double b, double c)
{
	double d = Min(a, b);
	return Min(b, c);
}

FLYMMATH_API double MAX4(double a, double b, double c, double d)
{
	double e = Max(a, b) ;
	double f = Max(c, d) ;
	return Max(e, f) ;
}

FLYMMATH_API double MIN4(double a, double b, double c, double d)
{
	double e = Min(a, b);
	double f = Min(c, d);
	return Min(e, f);
}

FLYMMATH_API double AMAX3(double a, double b, double c)
{
	double d = Max(fabs(a), fabs(b));
	return Max(d, fabs(c));
}

FLYMMATH_API double AMIN3(double a, double b, double c)
{
	double d = Min(fabs(a), fabs(b));
	return Min(d, fabs(c));
}

FLYMMATH_API double AMAX4(double a, double b, double c, double d)
{
	double e = Max(fabs(a), fabs(b));
	double f = Max(fabs(c), fabs(d));
	return Max(e, f);
}

FLYMMATH_API double AMIN4(double a, double b, double c, double d)
{
	double e = Min(fabs(a), fabs(b));
	double f = Min(fabs(c), fabs(d));
	return Min(e, f);
}

FLYMMATH_API double Inf_sup(const double X1, const double X2, const double X3)
{
	double d = Max(X1, X2);
	return Min(d, X3);
}

//------------------------------------------------------------------------------
// Переходы между размерностями
FLYMMATH_API double TO_KM(double dtkm)	   { return(dtkm*1e4); }
FLYMMATH_API double TO_DTKM(double km)	   { return(km / 1e4); }
FLYMMATH_API double TO_KMS(double dtkmsss) { return(dtkmsss / 8.64); }
FLYMMATH_API double TO_DTKMSSS(double kms) { return(kms*8.64); }
FLYMMATH_API double TO_RAD(double grad)	   { return(grad / M_GRARAD); }
FLYMMATH_API double TO_GRAD(double rad)	   { return(rad*M_GRARAD); }
FLYMMATH_API void VOSK_TO_RAD(double* O)   { FORKi(2, 6) O[i] /= M_GRARAD; O[0] /= 1e4; }
FLYMMATH_API void VOSK_TO_GRAD(double* O)  { FORKi(2, 6) O[i] *= M_GRARAD; O[0] *= 1e4; }
FLYMMATH_API void VASK_TO_KM(double* RV)   { FORKi(0, 3) { RV[i] *= 1e4; RV[i + 3] /= 8.64; } }
FLYMMATH_API void VASK_TO_DTKM(double* RV) { FORKi(0, 3) { RV[i] /= 1e4; RV[i + 3] *= 8.64; } }

//void VASK_TO_KM   (double* RV)	   { for (int i=0 ; i<3 ; i++) { RV[i]*= 1e4 ; RV[i+3]/= 8.64 ; } } 
//void VASK_TO_DTKM (double* RV)	   { for (int i=0 ; i<3 ; i++) { RV[i]/= 1e4 ; RV[i+3]*= 8.64 ; } } 
//void VOSK_TO_RAD  (double* O)	   { for (int i=2 ; i<6 ; i++) O[i]/= k_grarad ; O[0]/= 1e4 ; } 
//void VOSK_TO_GRAD (double* O)	   { for (int i=2 ; i<6 ; i++) O[i]*= k_grarad ; O[0]*= 1e4 ; } 

//------------------------------------------------------------------------------
FLYMMATH_API double RadA(double u) { return (u / M_GRARAD); }
FLYMMATH_API double GradA(double u) { return (u*M_GRARAD); }

//------------------------------------------------------------------------------
// Знак числа
//double Sign (double x) { return ( x < 0 ? -1.0 : 1.0 ) ; }
//double lSign(long x)   { return ( x < 0 ? -1.0 : 1.0 ) ; }
//double iSign(short x)  { return ( x < 0 ? -1.0 : 1.0 ) ; }

//------------------------------------------------------------------------------
// Изменение знака числа (a) , если число (b) отрицательное
//double Sign2(double a, double b) { return((b>=0)?a:-a); }

//------------------------------------------------------------------------------
FLYMMATH_API double Trigs(double x)
{ if ( x > 0 ) return 1 ; if ( x < 0 ) return-1 ; return 0 ; }

//------------------------------------------------------------------------------
FLYMMATH_API void CalcMathZero(double* z)
{
   double epsp1 ; EpsMZ  = 1.0 ;
   do { EpsMZ*= 0.5 ; epsp1= EpsMZ + 1.0 ; } while ( epsp1 > 1.0 ) ;
   if ( z != 0 ) *z = EpsMZ ;
}

//------------------------------------------------------------------------------
FLYMMATH_API bool IsEqMath(double X, double Y)
{ if ( fabs( X - Y ) <= EpsMZ ) return 1 ; return 0 ; }

//------------------------------------------------------------------------------
FLYMMATH_API double ToRoundOff(double x, int dec, bool isUpp)
{
    if (!x) return 0 ;
    double y = fabs(x) ;
    double S = x>0 ? 1.0:-1.0 ;
    double m = pow(10.0, dec) ;
    isUpp = S>0 ? isUpp:!isUpp ;
    y = y*m ;
    y = isUpp ? ceil(y):floor(y) ;
    y = S*y/m ;
    return y ;
}

//------------------------------------------------------------------------------
FLYMMATH_API double ToRoundOffUpp(double x, int dec) { return(ToRoundOff(x, dec, 1)); }
FLYMMATH_API double ToRoundOffDown(double x, int dec) { return(ToRoundOff(x, dec, 0)); }

//------------------------------------------------------------------------------
FLYMMATH_API double ToRoundLen(double x, int Len, bool isUpp)
{
    if (!x) return 0 ;
    double y = fabs(x) ;
    double S = x>0 ? 1.0:-1.0 ;
    double l = pow(10.0, floor(log10(y))) ;
    return (l*ToRoundOff(S*y/l, Len-1, isUpp)) ;
}

//------------------------------------------------------------------------------
FLYMMATH_API double ToRoundLenUpp(double x, int Len) { return(ToRoundLen(x, Len, 1)); }
FLYMMATH_API double ToRoundLenDown(double x, int Len) { return(ToRoundLen(x, Len, 0)); }

//------------------------------------------------------------------------------
FLYMMATH_API int MainRank(double x)
{
    if ( x == 0 ) return 0 ;
    double y  = fabs( x ) ;
    int    l  = ( int )floor( log10( y ) ) ;
    return l ;
}

//------------------------------------------------------------------------------
FLYMMATH_API void ReCalcMaxMinAxis(double& MaxV, double& MinV, int Len)
{
    int mR1 = MainRank(MaxV) ;
    int mR2 = MainRank(MinV) ;
    int mR  = mR1>mR2 ? mR1:mR2 ;
    int Len2=  Len-1 ;
    double eps = pow(10.0, mR-(Len-1)) ;
    if (mR1==mR2) {
      MaxV = ToRoundLenUpp (MaxV, Len) ;
      MinV = ToRoundLenDown(MinV, Len) ;
    } else {
      if (mR1>mR2) {
        MaxV = ToRoundLenUpp(MaxV, Len) ;
        MinV = ToRoundLenUpp(MinV-eps, Len2) ;
      } else {
        if (mR2>mR1) {
          MaxV = ToRoundLenDown(MaxV+eps, Len2) ;
          MinV = ToRoundLenDown(MinV, Len) ;
    } } }
}

//------------------------------------------------------------------------------
FLYMMATH_API double ValiDA(double dl, char dim)
{
    double d=dim?180.0:M_PI ;
    while(dl> d) dl-=2*d ;
    while(dl<-d) dl+=2*d ;
    return dl ;
}

//------------------------------------------------------------------------------
FLYMMATH_API double ValiA(double dl, char dim)
{
	double d = dim ? 360.0 : M_DPI;
    while(dl>d) dl-=d ;
    while(dl<0) dl+=d ;
    return dl ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int QtrArc(double u, char dim)
{
    if (dim) {
		if (u>=0     && u<90.0 ) return 1 ;
		if (u>=90.0  && u<180.0) return 2 ;
		if (u>=180.0 && u<270.0) return 3 ;
    } else {
		if (u >= 0 && u<M_PI / 2.0) return 1;
		if (u >= M_PI / 2.0 && u<M_PI) return 2;
		if (u >= M_PI       && u<3.0*M_PI / 2.0) return 3;
    }
    return 4 ;
}

//------------------------------------------------------------------------------
FLYMMATH_API double AtanSC(double tn, double sc, char dim)
{
	double pi = dim ? 180.0 : M_PI;
    double u = Atan(tn, dim) ;
    if (sc>0) u = u<0?u+2.0*pi:u ;
    else u+= pi ;
    return u ;
}

//------------------------------------------------------------------------------
FLYMMATH_API double AcosSS(double c, double ss, char dim)
{
	double pi = dim ? 180.0 : M_PI;
    double u = Acos( c, dim ) ;
    if ( ss < 0 ) u = 2.0 * pi - u ;
    return u ;
}

//------------------------------------------------------------------------------
FLYMMATH_API double AsinSC(double s, double sc, char dim)
{
	double pi = dim ? 180.0 : M_PI;
    double u ;
    if ( s > 0.9999999999 ) return( u = pi / 2 ) ;
    else { if ( s <-0.9999999999 ) return( u = 3 * pi / 2 ) ; }
    u = Atan( s / sqrt( 1 - s * s ), dim ) ;
    if ( sc < 0 ) u = pi - u ;
    if ( s < 0 && sc > 0 ) u = 2.0 * pi + u ;
    return u ;
}

//------------------------------------------------------------------------------
FLYMMATH_API double Cos(double u, char dim) { return(cos((dim ? RadA(u) : u))); }
FLYMMATH_API double Sin(double u, char dim) { return(sin((dim ? RadA(u) : u))); }
FLYMMATH_API double Tan(double u, char dim) { return(tan((dim ? RadA(u) : u))); }

//------------------------------------------------------------------------------
FLYMMATH_API double Asin(double s, char dim)
{ double u = asin((fabs(s)<=1.0?s:Sign1(s))) ; return((dim?GradA(u):u)) ; }

FLYMMATH_API double Acos(double c, char dim)
{ double u = acos((fabs(c)<=1.0?c:Sign1(c))) ; return((dim?GradA(u):u)) ; }

FLYMMATH_API double Atan(double t, char dim)
{ double u = atan( t ) ; return( ( dim ? GradA( u ) : u ) ) ; }

FLYMMATH_API double Atan2(double y, double x, char dim)
{ double u = atan2( y, x ) ;  return( ( dim ? GradA( u ) : u ) ) ; }

//------------------------------------------------------------------------------
FLYMMATH_API double Sinh(double x)
{ double temp = sinh( x ) ;  return temp ; }

FLYMMATH_API double Cosh(double x)
{ double temp = cosh( x ) ; return temp ; }

FLYMMATH_API double Tanh(double x)
{ double temp = tanh( x ) ; return temp ; }

FLYMMATH_API double Asinh(double x)
{ x = x ; return 0 ; }

FLYMMATH_API double Acosh(double x)
{ x = x ; return 0 ; }

FLYMMATH_API double Atanh(double x)
{ if ( fabs(x) < 1. ) return Log ( ( x + 1. ) / ( 1. - x ) ) / 2.;
  else                return 0 ; }

//------------------------------------------------------------------------------
FLYMMATH_API double Sqrt(double x)
{ if ( x < 0. ) return 0. ; return ( sqrt ( x  ) ) ; }

FLYMMATH_API double Pow(double x, double y)
{ if (y==0) return 1.0 ; return(pow(x, y)) ; }

//------------------------------------------------------------------------------
FLYMMATH_API double Log(double x)
{ if ( x < 0. ) x = 0.; return ( log( x ) ) ; }

//-------------------------------------------------------------------------------
// Определение наибольшего общего делителя D двух целых чисел A и B
FLYMMATH_API int MFindNOD(long A, long B, long& D)
{
	while(A!=B) {
		if (A>B) A = A-B ;
		else	 B = B-A ;
	} 
	D=A ;
	// НОД найден
	if (D>0) return 0 ;
	// НОД не существует оба числа делятся только на 1 и сами на себя
	D = 1 ;
	return-1 ;
}

//-------------------------------------------------------------------------------
// Анализ дроби на возможность сокращения делителя и знаменателя
//                              K
// Анализ рационального числа -----
//                              L
//                   KD
// Сокращение её до -----
//                   LD
//-------------------------------------------------------------------------------
FLYMMATH_API int IsItCompletelyDivided(int K, int L, int& KD, int& LD)
{
	int K1 = (K/L)*L ;
	int L1 = (L/K)*K ;
	KD = LD = 0 ;
	if (K1==K) {
		// K делится на L без остатка
		KD = K/L ; LD = 1 ;
		return 2 ;
	}
	if (L1==L) {
		// L делится на K без остатка
		LD = L/K ; KD = 1 ;
		return 3 ;
	}
	long D ;
	MFindNOD(K, L, D) ;
	KD = K/D ;
	LD = L/D ;
	return(D>1 ? 1:0) ;
}

//-------------------------------------------------------------------------------
// Решение алгебраического уравнения третьей степени по формулам Кардано
// A[0]*x3+A[1]*x2+A[2]*x+A[3] = 0 ; 
// Результат решения в 
//		Xr			-	действительная часть корня
//		Xi			-	мнимая часть корня
//		CountRoot	-	число найденных корней 
// Если Xi[.] = 0, то корень действительный
//-------------------------------------------------------------------------------
FLYMMATH_API int MSolv3DegreeEquation(double* K, double* Xr, double* Xi, int* CountRoot)
{
	double a1 = K[0] ;
	double a2 = K[1] ;
	double a3 = K[2] ;
	double a4 = K[3] ;
	double av = a2/a1 ;
	double bv = a3/a1 ;
	double cv = a4/a1 ;
	double p  =-av*av/3.0+bv ;
	double q  = 2.0*pow(av/3.0, 3.0)-av*bv/3.0+cv ;
	double Q  = pow(p/3.0, 3.0)+pow(q/2.0, 2.0) ;
	//
	double A  = pow(-q/2.0+sqrt(Q), 1.0/3.0) ;
	double B  =-p/3.0/A ;  
	//
	Xr[0] = A+B-av/3.0 ;
	Xi[0] = 0 ;
	//
	Xr[1] = 0 ;
	Xi[1] = 0 ;
	//
	Xr[2] = 0 ;
	Xi[2] = 0 ;
	//
	if (CountRoot) *CountRoot = 1 ;

	return 0 ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int MMatr(double* A, double* B, double* C, int n, int k, int m)
{
	// A: nxk B: kxm C: nxm
    int i, j, l ;
	for (i=0 ; i<n ; i++) {
		for (j=0 ; j<m ; j++) {
			C[i*m+j] = 0 ;
			for (l=0 ; l<k ; l++) C[i*m+j]+= A[i*k+l]*B[l*m+j] ;
	}	}	
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MMatrVactor(double* A, double* V, double* R, int n, int m)
{
    int  i, j ;
    for( i = 0 ; i < n ; i ++ ) {
      R[ i ] = 0 ;
      for ( j = 0 ; j < m ; j ++ ) R[ i ] += A[ i * m + j ] * V[ j ] ;
    }
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API double NormVector(double* A, double* B, int n)
{
	int i ;
    double d=0 ; for(i=0; i<n; i++) d+= A[i]*A[i] ;
	d = sqrt(d) ;
    if (d<1e-13) return d ;
    for (i=0; i<n; i++) (B?B[i]:A[i]) = A[i]/d ;
    return d ;
}
//------------------------------------------------------------------------------
FLYMMATH_API double ModVector(double* A, int n)
{
    double d=0 ; for(int i=0; i<n; i++) d+= A[i]*A[i] ;
    return sqrt(d) ; 
}
//------------------------------------------------------------------------------
FLYMMATH_API double MScalVector(double* A, double* B, int n)
{
    double d=0 ; for(int i=0; i<n; i++) d+= A[i]*B[i] ;
    return d  ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MVector(double* A, double* B, double* C)
{
    C[0] = A[1]*B[2]-A[2]*B[1] ;
    C[1] = A[2]*B[0]-A[0]*B[2] ;
    C[2] = A[0]*B[1]-A[1]*B[0] ;
    return 0 ;
}

//------------------------------------------------------------------------------
FLYMMATH_API void SumVector(double* A, double* B, double* C, int n, double* a, double* b)
{
    for (int i=0; i<n; i++) C[i] = (a?(*a)*A[i]:A[i])+(b?(*b)*B[i]:B[i]) ;
}

//------------------------------------------------------------------------------
FLYMMATH_API void SubtractVector(double* A, double* B, double* C, int n, double* a, double* b)
{
    for (int i=0; i<n; i++) C[i] = (a?(*a)*A[i]:A[i])-(b?(*b)*B[i]:B[i]) ;
}

//------------------------------------------------------------------------------
FLYMMATH_API int VectorAngl(double* A, double* B, double& U, char dim)
{
   double ma = Sqrt( A[ 0 ] * A[ 0 ] + A[ 1 ] * A[ 1 ] + A[ 2 ] * A[ 2 ] ) ;
   double mb = Sqrt( B[ 0 ] * B[ 0 ] + B[ 1 ] * B[ 1 ] + B[ 2 ] * B[ 2 ] ) ;
   double d = ( A[ 0 ] * B[ 0 ] + A[ 1 ] * B[ 1 ] + A[ 2 ] * B[ 2 ] ) / ( ma * mb ) ;
   U = Acos( d, dim ) ;
   return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MatrRotationAxis(double* M, double* R, double* axis0, double angle, int flag)
{
   double ma = sqrt( axis0[ 0 ]*axis0[ 0 ] + axis0[ 1 ]*axis0[ 1 ] + axis0[ 2 ]*axis0[ 2 ] ) ;
   if ( ma < 1e-14 ) return 1 ;
   double axis[ 3 ] = { axis0[ 0 ] / ma, axis0[ 1 ] / ma, axis0[ 2 ] / ma } ; 
   double Cine = cos ( angle ) ;
   double Sine = sin ( angle ) ;
   double T[ 9 ] ;
   double old[ 9 ] ;

   if ( flag & VMF_ROTATION_NEXT ) memcpy( old, M, 9 * sizeof( double ) ) ;
   if ( !( flag & VMF_ROTATION_DIRUP ) ) Sine = -Sine ;

   T[0] = axis[ 0 ] * axis[ 0 ] + ( 1- axis[ 0 ] * axis[ 0 ] ) * Cine;
   T[1] = axis[ 0 ] * axis[ 1 ] * ( 1- Cine ) + axis[ 2 ] * Sine;
   T[2] = axis[ 0 ] * axis[ 2 ] * ( 1- Cine ) - axis[ 1 ] * Sine;

   T[3] = axis[ 0 ] * axis[ 1 ] * ( 1- Cine ) - axis[ 2 ] * Sine;
   T[4] = axis[ 1 ] * axis[ 1 ] + ( 1- axis[ 1 ] * axis[ 1 ] ) * Cine;
   T[5] = axis[ 1 ] * axis[ 2 ] * ( 1- Cine ) + axis[ 0 ] * Sine;

   T[6] = axis[ 0 ] * axis[ 2 ] * ( 1- Cine ) + axis[ 1 ] * Sine;
   T[7] = axis[ 1 ] * axis[ 2 ] * ( 1- Cine ) - axis[ 0 ] * Sine;
   T[8] = axis[ 2 ] * axis[ 2 ] + ( 1- axis[ 2 ] * axis[ 2 ] ) * Cine;

   if ( flag & VMF_ROTATION_NEXT ) MMatr( T, old, R, 3, 3, 3 ) ;
   else memcpy( R, T, 9 * sizeof( double ) ) ;
   return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MatrRotation(double* M, double* R, double u1, double u2, double u3, int flag, int* axis)
{
   double old[ 9 ], T[ 9 ] ;
   double s1 = sin( u1 ), c1 = cos( u1 ) ;
   double s2 = sin( u2 ), c2 = cos( u2 ) ;
   double s3 = sin( u3 ), c3 = cos( u3 ) ;

   if ( flag & VMF_ROTATION_NEXT ) memcpy( old, M, 9 * sizeof( double ) ) ;
   if ( flag & VMF_ROTATION_AILER ) {  // z, x, y
     T[ 0 ] = c1*c3 - s1*s2*s3 ;  T[ 1 ] = s1*c3 + c1*s2*s3 ; T[ 2 ] =-c2*s3 ;
     T[ 3 ] =-s1*c2 ;             T[ 4 ] = c1*c2 ;            T[ 5 ] = s2 ;
     T[ 6 ] = c1*s3 + s1*s2*c3 ;  T[ 7 ] = s1*s3 - c1*s2*c3 ; T[ 8 ] = c2*c3 ;
     goto end ;
   }
   if ( flag & VMF_ROTATION_KRILOV ) { // z, x, z
     T[ 0 ] = c1*c3 - s1*c2*s3 ;  T[ 1 ] = s1*c3 + c1*c2*s3 ; T[ 2 ] = s2*s3 ;
     T[ 3 ] =-c1*s3 - s1*c2*c3 ;  T[ 4 ] =-s1*s3 + c1*c2*c3 ; T[ 5 ] = s2*c3 ;
     T[ 6 ] = s1*s2 ;             T[ 7 ] =-c1*s2 ;            T[ 9 ] = c2 ;
     goto end ;
   }
   if ( flag & VMF_ROTATION_USER ) {
     if ( axis == 0 ) return 0 ;
     double u ;
     int    f ;
     memcpy( T, M, 9 * sizeof( double ) ) ;
     for( int i = 0 ; i < 3 ; i ++ ) {
       if ( i == 0 ) { f = 0 ; u = u1 ; }
       else            f = VMF_ROTATION_NEXT ;
       if ( i == 1 )  u = u2 ;
       if ( i == 2 )  u = u3 ;
       if ( axis[ i ] & VMF_ROTATION_X ) MatrRotationX ( T, T, u, axis[ i ] | f ) ;
       if ( axis[ i ] & VMF_ROTATION_Y ) MatrRotationY ( T, T, u, axis[ i ] | f ) ;
       if ( axis[ i ] & VMF_ROTATION_Z ) MatrRotationZ ( T, T, u, axis[ i ] | f ) ;
     }
   }
end : ;
   if ( flag & VMF_ROTATION_NEXT ) MMatr( T, old, R, 3, 3, 3 ) ;
   else memcpy( R, T, 9 * sizeof( double ) ) ;
   return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MatrRotationX(double* M, double* R, double angle, int flag)
{
   double C[ 9 ] ; memset( &C[ 0 ], 0, sizeof( C ) ) ;
   C[ 0 ] = C[ 4 ] = C[ 8 ] = 1 ;
   if ( !( flag & VMF_ROTATION_DIRUP ) ) angle = -angle ;
   double Cose = cos ( angle );
   double Sine = sin ( angle );
   C[ 4 ] = Cose ;
   C[ 7 ] =-Sine ;
   C[ 5 ] = Sine ;
   C[ 8 ] = Cose ;
   if ( flag & VMF_ROTATION_NEXT ) {
     double C2[ 9 ] ;
     memcpy( C2, M, 9 * sizeof( double ) ) ;
     MMatr( C, C2, R, 3, 3, 3 ) ;
   } else {
     memcpy( R, C, 9 * sizeof( double ) ) ;
   }
   return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MatrRotationY(double* M, double* R, double angle, int flag)
{
   double C[ 9 ] ; memset( &C[ 0 ], 0, sizeof( C ) ) ;
   C[ 0 ] = C[ 4 ] = C[ 8 ] = 1 ;
   if ( !( flag & VMF_ROTATION_DIRUP ) ) angle =-angle ;
   double Cose = cos( angle ) ;
   double Sine = sin( angle ) ;
   C[ 0 ] = Cose ;
   C[ 6 ] = Sine ;
   C[ 2 ] =-Sine ;
   C[ 8 ] = Cose ;
   if ( flag & VMF_ROTATION_NEXT ) {
     double C2[ 9 ] ;
     memcpy( C2, M, 9 * sizeof( double ) ) ;
     MMatr( C, C2, R, 3, 3, 3 ) ;
   } else {
     memcpy( R, C, 9 * sizeof( double ) ) ;
   }
   return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MatrRotationZ(double* M, double* R, double angle, int flag)
{
   double C[ 9 ] ; memset( &C[ 0 ], 0, sizeof( C ) ) ;
   C[ 0 ] = C[ 4 ] = C[ 8 ] = 1 ;
   if ( !( flag & VMF_ROTATION_DIRUP ) ) angle = -angle ;
   double Cose = cos( angle ) ;
   double Sine = sin( angle ) ;
   C[ 0 ] = Cose ;
   C[ 3 ] =-Sine ;
   C[ 1 ] = Sine ;
   C[ 4 ] = Cose ;
   if ( flag & VMF_ROTATION_NEXT ) {
     double C2[ 9 ] ;
     memcpy( C2, M, 9 * sizeof( double ) ) ;
     MMatr( C, C2, R, 3, 3, 3 ) ;
   } else {
     memcpy( R, C, 9 * sizeof( double ) ) ;
   }
   return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int MMatrTransp(double* A, double* T, int n, int m)
{
    int  i, j ;
    if (n!=m) {
		for (i=0; i<n; i++) { for(j=0; j<m; j++) 
			T[j*n+i] = A[i*m+j] ;
	}	} else {
		double z ;
		memcpy( T, A, 9 * sizeof( double ) ) ;
		for (i=0; i<n; i++) { for (j=0; j<i; j++) {
			z = T[j*n+i] ; T[j*n+i] = T[i*m+j] ; T[i*m+j] = z ;
	}	}	}
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int SolutionLineSystem(int n, double* a0, double* b0, double* x)
{
    int     i, j, k, ic = 0 ;
    double*  a ;    // Копия матрици системы
    double*  b ;    // Копия столбца правых частей
    int*     s ;    // Тень преобразования матрици системы
    double   el_v ; // Ведущий элемент
    double   tf ;
    int      ti ;
    //
    a = new double[ n*n ] ; memcpy( a, a0, n*n * sizeof( double ) ) ;
    b = new double[ n ] ;   memcpy( b, b0, n * sizeof( double ) ) ;
    s = new int[ n ] ;      memset( s, 0, n * sizeof( int ) ) ;
    // Инициируем тень
    for ( i = 0 ; i < n ; i ++ ) s[ i ] = i ;
    // Прямой ход преобразования матрици системы к треугольному виду
    for ( i = 0 ; i < n - 1 ; i ++ ) {
      // Определяем ведущий элемент
      el_v = fabs( a[ i * n ] ) ; j = 0 ;
      for ( k = 1 ; k < n ; k++ ) {
        if ( el_v < fabs( a[ i * n + k ] ) ) j = k ;
      }
      el_v = a[ i * n + j ] ;
      // Если строка нулевая то решения нет
      if ( fabs( el_v ) < 1e-15 ) {
        //MakeVectorErr ( VM_ERROR_DET_MTR_ZERO ) ;
      	//ic = VM_ERROR_DET_MTR_ZERO ;
        ic = 1 ;
        goto end ; // система линейно зависима, единственного решения не существует
      }
	  // Меняем местами столбцы согласно найденому ведущему элементу
      if ( i != j ) {
        for ( k = 0 ; k < n ; k++ ) {
          tf = a[ k * n + i ] ; a[ k * n + i ] = a[ k * n + j ] ; a[ k * n + j ] = tf ;
          ti = s[ i ] ; s[ i ] = s[ j ] ; s[ j ] = ti ;
        }
      }
	  // Выполняем преобразование со строками
      for ( j = i + 1 ; j < n ; j ++ )  {
        b[ j ] = b[ j ] - b[ i ] * a[ j * n + i ] / a[ i * n + i ] ;
        double d = -a[ j * n + i ] / a[ i * n + i ] ;
        for ( k = 0 ; k < n ; k ++ ) {
          a[ j * n + k ] = a[ j * n + k ] + d * a[ i * n + k ] ;
        }
      }
	}
    // Обратный ход
    for ( i = n - 1 ; i >= 0 ; i -- ) {
      // Если текущий элемент на диагонали матрицы нулевой, то решения нет
      if ( fabs( a[ i * n + i ] ) < 1e-15 ) {
        memset ( x, 0, n * sizeof( double ) ) ;
        //MakeVectorErr ( VM_ERROR_DET_MTR_ZERO ) ;
	    //ic = VM_ERROR_DET_MTR_ZERO ;
        ic = 1 ;
        goto end ;
      }
	  // Ведущий элемент
      el_v = b[ i ] ;
	  // Определение текущей компоненты искомого решения
      for ( j = n - 1 ; j > i ; j -- ) {
        el_v -= x[ ( int )s[ j ] ] * a[ i * n + j ] ;
      }
      x[ ( int )s[ i ] ] = el_v / a[ i * n + i ] ;
    }
end : ;
    if ( a ) delete a ; a = 0 ;
    if ( b ) delete b ; b = 0 ;
    if ( s ) delete s ; s = 0 ;
	return ic ;
}

////////////////////////////////////////////////////////////////////////////////
// Интерполяция по Адамсу 7-го порядка по 8 точкам
//		Hist - массив строк в каждой из которых лежит вектор (t+кинематические параметры)
//			   например t,x,y,z,Vx,Vy,Vz.
//			   строки расположены по возрастанию или убыванию времени
//		e1   - 0 <= e1 <= 7 отношение шага интерполяции к шагу групп интегрирования
//		txy  - Результат - интерполируемые кинематические параметры
//			   Вектор формируется на момент времени равный
//			   t = Hist[0][0] + e*h, если Hist[0][0] < Hist[7][0] 
//			   t = Hist[0][0] - e*h, если Hist[0][0] > Hist[7][0] 
//			   где h - шаг групп интегрирования.   
//		nx	 - число компонент интерполируемого вектора без учёта времени
////////////////////////////////////////////////////////////////////////////////
FLYMMATH_API int MITA8(double* Hist[8], int nx, double e, double *txy)
{
    static double K[56] = {
    0.1984126984126984e-03,  -0.4166666666666667e-02,   0.3472222222222222e-01,
   -0.1458333333333333e+00,   0.3222222222222222e+00,  -0.3500000000000000e+00,
	0.1428571428571428e+00,  -0.1388888888888889e-02,   0.3055555555555555e-01,
   -0.2638888888888889e+00,   0.1138888888888889e+01,  -0.2568055555555556e+01,
    0.2830555555555556e+01,  -0.1166666666666667e+01,   0.4166666666666667e-02,
   -0.9583333333333334e-01,   0.8625000000000000e+00,  -0.3854166666666667e+01,
    0.8933333333333334e+01,  -0.1005000000000000e+02,   0.4200000000000000e+01,
   -0.6944444444444444e-02,   0.1666666666666667e+00,  -0.1569444444444444e+01,
    0.7333333333333333e+01,  -0.1767361111111111e+02,   0.2050000000000000e+02,
   -0.8750000000000000e+01,   0.6944444444444444e-02,  -0.1736111111111111e+00,
    0.1715277777777778e+01,  -0.8465277777777779e+01,   0.2161111111111111e+02,
   -0.2636111111111111e+02,   0.1166666666666667e+02,  -0.4166666666666667e-02,
    0.1083333333333333e+00,  -0.1125000000000000e+01,   0.5916666666666667e+01,
   -0.1637083333333333e+02,   0.2197500000000000e+02,  -0.1050000000000000e+02,
    0.1388888888888889e-02,  -0.3750000000000000e-01,   0.4097222222222222e+00,
   -0.2312500000000000e+01,   0.7088888888888889e+01,  -0.1115000000000000e+02,
    0.7000000000000000e+01,  -0.1984126984126984e-03,   0.5555555555555556e-02,
   -0.6388888888888888e-01,   0.3888888888888889e+00,  -0.1343055555555555e+01,
    0.2605555555555556e+01,  -0.2592857142857143e+01 } ;
	int i, j, k, l ;
    static double A[8] ;

    memset(txy, 0, (2*nx+1)*sizeof(double)) ;
	for (j=0 ; j<8 ; j++) {
		l = 7*j ;
		for (i=1, A[j] = K[l] ; i<7 ; i++) A[j] = A[j]*e+K[l+i] ;
    }
	// Интерполяция координат
    for (k=1 ; k<nx+1 ; k++) {
		for (j=0 ; j<8 ; j++) txy[k]+= Hist[7-j][k]*A[j] ;
		txy[k] = txy[k]*e+Hist[0][k] ;
    }
	// Интерполяция времени
	txy[0] = Hist[0][0]-e*(Hist[0][0]-Hist[1][0]) ;
	return 0 ;
}

//------------------------------------------------------------------------------
// Интерполяция по Адамсу 4 порядка по 5 точкам
//		points	- массив из 5 точек вдоль траектории
//		e		- Коэффициэнт интерполяции
//				  Отрицательная величина в диапазоне [-1, 0]. 	
//		TVR		- Массив результатов интерполяции (t,vx,vy,vz,x,y,z)	
//int MITA(double points[5][7], double e, double TVR[7])
FLYMMATH_API int MITA4(double fld1[5][7], double e, double agin[7])
{
/*
	const double c[12] = {
	0.04166666666666666,
	-0.125,
	0.125,
	-0.04166666666666666,
	-0.33333333333333333,
	0.83333333333333333,
	-0.66666666666666666,
	0.16666666666666666,
	0.91666666666666666,
	-1.5,
	0.75,
	-0.16666666666666666 } ;
 	double f[4], ew, ew1, zne ;
	int    i, j;

	ew = fabs(e) ;
	zne= e/ew ; 
	ew1= ew*ew ;
	for (i=0 ; i<4 ; i++) f[i] = ((ew*c[0+i]+c[4+i])*ew+c[8+i])*ew1 ;
	for (i=0 ; i<7 ; i++) {
		TVR[i] = (f[0]-ew)*points[1][i] ;
		for (j=1 ; j<4 ; j++) TVR[i]+= f[j]*points[j+1][i] ;
		TVR[i] = points[0][i]-zne*TVR[i] ;
	}
*/
	double c1 = 0.04166666666;
	double c2 =-0.125;
	double c3 = 0.125;
	double c4 =-0.0416666666;
	double c5 =-0.3333333333;
	double c6 = 0.8333333333;
	double c7 =-0.6666666666;
	double c8 = 0.1666666666;
	double c9 = 0.9166666666;
	double c10=-1.5;
	double c11= 0.75;
	double c12=-0.16666666666;
	double f1,f2,f3,f4;
	double ew,ew1,zne;
	int    i ;

	ew =fabs(e);
	zne=e/ew;
	ew1=ew*ew;
	f1 =((ew*c1+c5)*ew+c9)*ew1;
	f2 =((ew*c2+c6)*ew+c10)*ew1;
	f3 =((ew*c3+c7)*ew+c11)*ew1;
	f4 =((ew*c4+c8)*ew+c12)*ew1;

	for (i=0; i<7; i++) 
		agin[i] = fld1[0][i] - zne*(f1*fld1[1][i]+f2*fld1[2][i]+f3*fld1[3][i]+f4*fld1[4][i]);

	return 0 ;
}

//------------------------------------------------------------------------------
//	TVR	- t,vx,vy,vz,x,y,z
FLYMMATH_API int MGSCtoHBL(double TVR[7], double& bge, double& lge, double& hge)
{
	const double amax = 6378137.0 ;  
	const double alfa = 0.335289187e-2 ;  
	const double a4   = 9.8665573318e-1 ;
	double r, r1, r2, r3 ;

	r1 = TVR[4]*TVR[4]+TVR[5]*TVR[5] ;
	r2 = TVR[6]*TVR[6] ;
	r3 = r1+r2 ;
	r  = sqrt(r3) ;
	hge = r ;			//-amax*(1-alfa*r2/r/r);
	bge =atan(TVR[6]/sqrt(a4*r1)) ;
	if (TVR[5]!=0) goto M1 ;
	if (TVR[6]!=0) goto M4 ;
	lge = 0 ; goto M3 ;
M4: ;
	if (TVR[5]<0) goto M5 ;
	lge = M_PI_2; goto M3;
M5: ;
	lge = 3*M_PI_2; goto M3;
M1: ;
	if (TVR[4]< 0) goto M6 ;
	if (TVR[5]!=0) goto M2 ;
	lge = 0 ; goto M3 ;
M2: ;
	lge =atan(TVR[5]/TVR[4]);
	if (TVR[5]>0) goto M3 ;
	lge = lge + 2.0*M_PI; goto M3;
M6: ;
	if (TVR[5]!=0) goto M7 ;
	lge = M_PI; goto M3;
M7: ;
	lge = atan(TVR[5]/TVR[4]) ; 
	lge = lge + M_PI;
 M3: ;
	return 0 ;
}

//------------------------------------------------------------------------------
// Интерполяция по Адамсу 4 порядка по 5 точкам на заданную долготу
//		agin	- t,vx,vy,vz,x,y,z
FLYMMATH_API int MITAfromL(double lz, double l1, double l2, double fld[5][7], double& bge, double* TVR)
{
	double hge, lge, e1, e2, e, de ;
	double fld1[5][7]; 
	e1 = l2-l1 ;
	if (e1 <0) e1 = e1 + 2.0*M_PI;
	e2 = lz-l2 ;
	if (e2 >= 0) e2 = e2 - 2.0*M_PI;
	e = e2/e1 ; 
	for (int l=0 ; l<7 ; l++) {	for (int k=0 ; k<5 ; k++) fld1[k][l] = fld[k][l] ; }
	while(1) {
		MITA4(fld1, e, TVR) ;
		MGSCtoHBL(TVR, bge, lge, hge) ;
		e2 = lz-lge ;
		if (e2 >= 0) { if (e2 >= M_PI_2) e2 = e2 - 2.0*M_PI; }
		else { if (e2 <= -M_PI_2) e2 = e2 + 2.0*M_PI; }
		if (fabs(e2)<0.0001) break ;
		de = e2/e1 ; 
		e  = e+de ; 
	}
	return 0 ;
}

//------------------------------------------------------------------------------
// Интерполяция по Адамсу 4 порядка по 5 точкам на заданную широту
//		agin	- t,vx,vy,vz,x,y,z
FLYMMATH_API int MITAfromB(double bz, double b1, double b2, double fld[5][7], double& lge, double* TVR)
{
	double rb1,rb2,bge,hge,e,e1,de ;
	double fld1[5][7] ;
	rb1 = b2-b1 ;
	rb2 = bz-b2 ;
	e   = rb2/rb1 ;
	for (int l=0 ; l<7 ; l++) { for (int k=0 ; k<5 ; k++) fld1[k][l] = fld[k][l] ; }
	while(1) {
		MITA4(fld1, e, TVR) ;
		MGSCtoHBL(TVR, bge, lge, hge) ;
		e1 = bz-bge ;
		if (fabs(e1)<0.0001) break ;
		de = e1/rb1 ;
		e  = e+de ; 
	}
 	return 0 ;
}

//------------------------------------------------------------------------------
// Кодирование в двоичной системе числа с плавающей точкой ( d -> S )
FLYMMATH_API int DoubleCode2(double d, char* S, int sizeNet, int MaxBinaryDec)
{
    d = fabs( d ) ;
    //
    static char  s[ 256 ] ;
    double d1 = floor( d ) ;
    double d2 = d - d1 ;
    double m, p ;
    int    k, l1, l2 ;
    // Целая часть
    m  = 2 ; l1 = 0 ; s[ 0 ] = '\0' ;
    while( 1 ) {
      p  = d1 / m ;  k = ( int )( ( p - floor( p ) ) * 2.0 ) ;
      d1-= k * m / 2.0 ; m*= 2 ; s[ l1 ] = k ? '1' : '0' ;
      if ( d1 <= 0 ) break ;
      l1++ ;
    }
    l1++ ; s[ l1 ] = '\0' ;
    if ( l1 + 1 >= sizeNet ) {
      for( k = 0 ; k < l1 + 1 - sizeNet ; k++ ) s[ k ] = '0' ;
    }
    for( k = 0 ; k < l1 ; k++ ) S[ k ] = s[ l1 - k - 1 ] ; S[ k ] = '\0' ;
    if ( l1 + 1 >= sizeNet ) return-1 ;
    if ( d2 == 0 ) return 0 ;
    if ( MaxBinaryDec == 0 ) return-2 ;
    // Дробная часть
    m = 2 ; l2 = 0 ; s[ 0 ] = '\0' ;
    while( 1 ) {
      p  = d2 * m ; k = ( int )floor( p ) ;
      d2-= k / m ; m*= 2 ; s[ l2 ] = k ? '1' : '0' ;
      if ( d2 <= 0 ) break ; if ( l1 + l2 + 1 >= 32 || l2 == MaxBinaryDec - 1 ) break ;
      l2++ ;
    }
    l2++ ; s[ l2 ] = '\0' ;
    S[ l1 ] = ',' ;
    for( k = 0 ; k < l2 ; k++ ) S[ l1 + 1 + k ] = s[ k ] ; S[ l1 + 1 + k ] = '\0' ;
    //
    if ( l1 + l2 >= sizeNet ) return-1 ;
    if ( l2 == MaxBinaryDec ) return-2 ;
    return 0 ;
}
//------------------------------------------------------------------------------
// Раскодирование числа d с плавающей точкой заданного в двоичной системе S ( S -> d )
FLYMMATH_API int DoubleDeCode2(char* S, double& d)
{
    int    i, l = strlen( S ), j = l ;
    double m ;
    for( i = 0 ; i < l ; i++ ) if ( S[ i ] == ',' ) break ; if ( i < l ) j = i ;
    m = 1 ; d = 0 ;
    for( i = j - 1 ; i >=0 ; i-- ) { if ( S[ i ] == '1' ) d+= m ; m*= 2.0 ; }
    m = 0.5 ;
    for( i = j + 1 ; i < l ; i++ ) { if ( S[ i ] == '1' ) d+= m ; m/= 2.0 ; }
    return 0 ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int RoundDownBinaryDouble(double d, double& fd, int MaxBinaryDec)
{
    char S[ 256 ] ;
    int rc = DoubleCode2( d, S, 255, MaxBinaryDec ) ;
    DoubleDeCode2( S, fd ) ;
    return rc ;
}
//------------------------------------------------------------------------------
FLYMMATH_API int NearestIndex(double x, double* xi, int n)
{
    int j = 0 ;
    double mi, mm = fabs( xi[ 0 ] - x ) ;
    for ( int i = 1 ; i < n ; i++ ) {
      mi = fabs( xi[ i ] - x ) ;
      if ( mi < mm ) { mm = mi ; j = i ; }
    }
    return j ;
}
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
FLYMMATH_API double scal_pr_norm(double *a, double *b, short n)
{
	double c = 0;
	short i;
	for (i = 0; i<n; c += a[i] * b[i++]);
	c /= sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	c /= sqrt(b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);
	return c;
}

//------------------------------------------------------------------------------
FLYMMATH_API double scal_pr(double *a, double *b, short n)
{
	double c = 0;
	short i;
	for (i = 0; i<n; c += a[i] * b[i++]);
	return c;
}

//------------------------------------------------------------------------------
FLYMMATH_API void wek_pr(double* a, double* b, double* d)
{
	d[0] = a[1] * b[2] - a[2] * b[1];
	d[1] = a[2] * b[0] - a[0] * b[2];
	d[2] = a[0] * b[1] - a[1] * b[0];
}

//------------------------------------------------------------------------------
FLYMMATH_API void wek_pr_norm(double* a, double* b, double* d)
{
	double dd;
	short i;
	d[0] = a[1] * b[2] - a[2] * b[1];
	d[1] = a[2] * b[0] - a[0] * b[2];
	d[2] = a[0] * b[1] - a[1] * b[0];
	dd = sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
	if (dd) for (i = 0; i < 3; i++) d[i] /= dd;
}

//------------------------------------------------------------------------------
//	транспонирование а(m,n) в b(n,m); если m=n (a)=(b)  
//------------------------------------------------------------------------------
FLYMMATH_API void TRAM(double* a, double* b, short m, short n)
{
	double d, c;
	short i, j, i2;
	for (i = 0; i < m; i++) {
		i2 = i*n;
		if (m == n) {
			for (j = i; j < n; j++) {
				d = a[j + i2]; c = a[j*n + i];   /* d=a(i,j) , c=a(j,i)  */
				b[j + i2] = c; b[j*m + i] = d;   /* b(i,j)=c , b(j,i)=d  */
			}
		}
		else {
			for (j = 0; j < n; b[j*m + i] = a[j + i2], j++);
		}
	}
}

//---------------------------------------------------------------------------
// Умножение вектора <3> на транспонированную квадратную матрицу [3x3]
//    m - квадратная матрица (3х3), v - вектор, rez - результат
//---------------------------------------------------------------------------
FLYMMATH_API void UMN_MATR_TRN_VECT(double *m, double *v, double *rez)
{
	rez[0] = v[0] * m[0] + v[1] * m[3] + v[2] * m[6];
	rez[1] = v[0] * m[1] + v[1] * m[4] + v[2] * m[7];
	rez[2] = v[0] * m[2] + v[1] * m[5] + v[2] * m[8];
	return;
}

//---------------------------------------------------------------------------
// Умножение вектора <3> на транспонированную квадратную матрицу [3x3]
// Результат добавляется к предыдущему
//    m - квадратная матрица (3х3), v - вектор, rez - результат
//---------------------------------------------------------------------------
FLYMMATH_API void UMN_MATR_TRN_VECT_ADD(double *m, double *v, double *rez)
{
	rez[0] += v[0] * m[0] + v[1] * m[3] + v[2] * m[6];
	rez[1] += v[0] * m[1] + v[1] * m[4] + v[2] * m[7];
	rez[2] += v[0] * m[2] + v[1] * m[5] + v[2] * m[8];
	return;
}

//---------------------------------------------------------------------------
// Умножение вектора <3> на квадратную матрицу [3x3]
//    m - квадратная матрица (3х3), v - вектор, rez - результат
//---------------------------------------------------------------------------
FLYMMATH_API void UMN_MATR_PRM_VECT(double *m, double *v, double *rez)
{
	rez[0] = v[0] * m[0] + v[1] * m[1] + v[2] * m[2];
	rez[1] = v[0] * m[3] + v[1] * m[4] + v[2] * m[5];
	rez[2] = v[0] * m[6] + v[1] * m[7] + v[2] * m[8];
	return;
}

//---------------------------------------------------------------------------
// Умножение вектора <3> на квадратную матрицу [3x3]
// Результат добавляется к предыдущему
//    m - квадратная матрица (3х3), v - вектор, rez - результат
//---------------------------------------------------------------------------
FLYMMATH_API void UMN_MATR_PRM_VECT_ADD(double *m, double *v, double *rez)
{
	rez[0] += +v[0] * m[0] + v[1] * m[1] + v[2] * m[2];
	rez[1] += +v[0] * m[3] + v[1] * m[4] + v[2] * m[5];
	rez[2] += +v[0] * m[6] + v[1] * m[7] + v[2] * m[8];
	return;
}

//---------------------------------------------------------------------------
// Умножение матрицы А(m,n) на матрицу В(n,l), результат помещается в С(m,l)
//---------------------------------------------------------------------------
FLYMMATH_API void UMN_MATR(double* dMatrA, double* dMatrB, double* dMatrC, short M, short N, short L)
{
	short i, j, k;
	for (i = 0; i < M; i++) {
		for (j = 0; j < L; j++) {
			for (k = 0, dMatrC[i*L + j] = 0.; k < N; k++) {
				dMatrC[i*L + j] += dMatrA[i*N + k] * dMatrB[k*L + j];
			}
		}
	}
}

//---------------------------------------------------------------------------
// Умножение матрицы А(m,n) на матрицу В(n,l), результат добавляется в С(m,l)
//---------------------------------------------------------------------------
FLYMMATH_API void UMN_MATR_ADD(double* dMatrA, double* dMatrB, double* dMatrC, short M, short N, short L)
{
	short i, j, k;
	for (i = 0; i < M; i++) {
		for (j = 0; j < L; j++) {
			for (k = 0; k < N; k++) {
				dMatrC[i*L + j] += dMatrA[i*N + k] * dMatrB[k*L + j];
			}
		}
	}
}

//------------------------------------------------------------------------------
// Умножение двух произвольных матриц l*m на m*n
//------------------------------------------------------------------------------
FLYMMATH_API void Um_Mat(double *Mt1, double *Mt2, double *Rez, short l, short m, short n)
{
	short  i, j, k;
	double *a;
	a = Mt2;

	for (i = 0; i<l; i++) {
		for (j = 0; j<n; j++) {
			*Rez = 0;
			for (k = 0; k<m; k++) {
				if (k) a += n;
				*Rez += (*(Mt1++))*(*a);
			}
			Mt1 -= m;
			a = (++Mt2);
			Rez++;
		}
		Mt1 += m;
		a = (Mt2 -= n);
	}
	return;
}

//------------------------------------------------------------------------------
// Умножение матрицы А(m,n) на матрицу В(n,l), результат С(m,l)
//  если n>=l , то допускается (A)=(C)
//------------------------------------------------------------------------------
FLYMMATH_API void UMN(double *a, double *b, double *c, short m, short n, short l)
{
	double *d = new double[l + 1];
	double sum = 0.;
	short i, j, k;
	long  i1, i2;

	for (i = 0; i<m; i++) {
		i1 = i*n; i2 = i*l;
		for (j = 0; j<l; d[j] = sum, j++) {
			sum = 0.;
			for (k = 0; k<n; sum += a[i1 + k] * b[j + l*k], k++);
		}
		for (k = 0; k<l; c[i2 + k] = d[k], k++);
	}
	delete[] d;
}

//------------------------------------------------------------------------------
// Умножение вектора <3> на квадратную матрицу [3x3]
//    m - квадратная матрица (3х3), v - вектор, rez - результат
//------------------------------------------------------------------------------
FLYMMATH_API void umpr(double *m, double *v, double *rez)
{
	rez[0] = v[0] * m[0] + v[1] * m[1] + v[2] * m[2];
	rez[1] = v[0] * m[3] + v[1] * m[4] + v[2] * m[5];
	rez[2] = v[0] * m[6] + v[1] * m[7] + v[2] * m[8];
	return;
}

//------------------------------------------------------------------------------
// Умножение вектора <3> на транспонированную квадратную матрицу [3x3]
//    m - квадратная матрица (3х3), v - вектор, rez - результат
//------------------------------------------------------------------------------
FLYMMATH_API void umobr(double *m, double *v, double *rez)
{
	rez[0] = v[0] * m[0] + v[1] * m[3] + v[2] * m[6];
	rez[1] = v[0] * m[1] + v[1] * m[4] + v[2] * m[7];
	rez[2] = v[0] * m[2] + v[1] * m[5] + v[2] * m[8];
	return;
}

//------------------------------------------------------------------------------
//   Расчет угла от оси ОХ в координатной плоскости
//	 по значениям координат X и Y.
//------------------------------------------------------------------------------
//   Входные параметры:
//       X,Y  - координаты точки на плоскости.
//   Возвращаемое значение:
//      Значение угла от оси OX против часовой стрелки
//	    в интервале от -3.14.... до +3.14....(в радианах).
//------------------------------------------------------------------------------
FLYMMATH_API double dargum(double X, double Y)
{
	double x1, x2, darg;
	x1 = fabs(X);
	x2 = fabs(Y);
	if ((x1 + x2) > 0.) {
		if (x1 <= x2) darg = -atan(X / Y) + M_PI*Sign2(0.5, Y);
		else {
			darg = atan(Y / X);
			if (X <= 0.) {
				if (Y >= 0.) darg = darg + M_PI;
				else        darg = darg - M_PI;
			}
		}
	}
	else return 0.0;
	return darg;
}

//------------------------------------------------------------------------------
FLYMMATH_API double FK(int N)
{
	double R = 1;
	for (int i = 1; i <= N; i++) R *= (double)i;
	return R;
}

//------------------------------------------------------------------------------
FLYMMATH_API double FR(double *k)
{
	return sqrt(k[0]*k[0] + k[1]*k[1] + k[2]*k[2]);
}

//------------------------------------------------------------------------------
FLYMMATH_API double FR2(double *k)
{
	return (k[0]*k[0] + k[1]*k[1] + k[2]*k[2]);
}

//------------------------------------------------------------------------------
FLYMMATH_API double FRR(double *v1, double *v2)
{
	double S = 0.0, D;
	for (int i = 0; i<3; i++) { D = v1[i] - v2[i]; S += D*D; }
	return sqrt(S);
}

//------------------------------------------------------------------------------
// Учёт возможности сползвния начала импульсы на предыдущий виток
// или наоборот на следующий
// Приведение аргумента широты в диапазон [0; 360] град с 
// одновременной, при необходимости, коррекцией витка. 
template <typename ArgType, typename VitType>
FLYMMATH_API void SuitabilityTurnArg(ArgType& u, VitType& v, bool fromGrad)
{
	ArgType d = (ArgType)(fromGrad ? 360.0 : k_dwapi);
	while (u>d)   { u-= d; ++v; }
	while (u<0.0) { u+= d; --v; }
}

// Инстанцирование шаблонной функции
template FLYMMATH_API void SuitabilityTurnArg(double& u, int&  v, bool fromGrad);
template FLYMMATH_API void SuitabilityTurnArg(double& u, long& v, bool fromGrad);
template FLYMMATH_API void SuitabilityTurnArg(float&  u, int&  v, bool fromGrad);
template FLYMMATH_API void SuitabilityTurnArg(float&  u, long& v, bool fromGrad);

//------------------------------------------------------------------------------
