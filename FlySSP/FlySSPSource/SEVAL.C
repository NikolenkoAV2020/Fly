/*------------------------------------------------------------------------
  SEVAL.C

  Автор   : Перевод программы С c FORTRAN (Форсайт) выполнил Пасынков В.В.
  Версия  : 01.00 / 21.05.2000 /
------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"
/* ********************************************
		       BЫЧ.KYБИЧ.CПЛAЙHA
 SEVAL=Y(I)+B(I)*(U-X(I))+C(I)*(U-X(I))**2+D(I)*(U-X(I))**3
  X(I).LT.U.LT.X(I+1)
 IF U.LT.X(1)  ,TO I=1
 IF U.GE.X(N)  ,TO I=N
       BXOД:
 N - ЧИCЛO YЗЛOB N>2
 U - APГYMEHT
 X,Y - MACCИBЫ AБCЦИCC И OPДИHAT
 B,C,D - MACCИBЫ KOЭФФ-B
 ИHTEPBAЛ HAX-CЯ ДBOИЧHЫM ПOИCKOM
******************************************** */


double seval(int n, double u, double x[], double y[],
		      double b[], double c[], double d[])
{
int    j, k;
double dx, seval_v;
static int i = 1;

if( i >= n )
	i = 1;
if( u < x[i-1] )
	goto L_10;
if( u < x[i + 1-1] )
	goto L_30;
/*  ДBOИЧH. ПOИCK */
L_10:
i = 1;
j = n + 1;
L_20:
k = (i + j)/2;
if( u < x[k-1] )
	j = k;
if( u >= x[k-1] )
	i = k;
if( j > i + 1 )
	goto L_20;
/*  BЫЧИCЛEHИE CПЛAЙHA S(X) */
L_30:
dx = u - x[i-1];
seval_v = y[i-1] + dx*(b[i-1] + dx*(c[i-1] + dx*d[i-1]));
return( seval_v );
}

