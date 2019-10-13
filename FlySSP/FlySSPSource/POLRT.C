//------------------------------------------------------------------------
//   POLRT.C
//     Вычиcлeниe дeйcтвитeльныx и кoмплeкcныx кopнeй пoлинoмa
//   Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
//   Версия : 01.00 / 10.06.1996 /
//------------------------------------------------------------------------
#include "ssp.h"
#include <math.h>

/*     ..................................................................

        SUBROUTINE POLRT

        PURPOSE
           COMPUTES THE REAL AND COMPLEX ROOTS OF A REAL POLYNOMIAL

        USAGE
           CALL POLRT(XCOF,COF,M,ROOTR,ROOTI,IER)

	DESCRIPTION OF PARAMETERS
           XCOF -VECTOR OF M+1 COEFFICIENTS OF THE POLYNOMIAL
                 ORDERED FROM SMALLEST TO LARGEST POWER
           COF  -WORKING VECTOR OF LENGTH M+1
           M    -ORDER OF POLYNOMIAL
           ROOTR-RESULTANT VECTOR OF LENGTH M CONTAINING REAL ROOTS
                 OF THE POLYNOMIAL
           ROOTI-RESULTANT VECTOR OF LENGTH M CONTAINING THE
                 CORRESPONDING IMAGINARY ROOTS OF THE POLYNOMIAL
           IER  -ERROR CODE WHERE
                 IER=0  NO ERROR
                 IER=1  M LESS THAN ONE
		 IER=2  M GREATER THAN 36
                 IER=3  UNABLE TO DETERMINE ROOT WITH 500 INTERATIONS
                        ON 5 STARTING VALUES
                 IER=4  HIGH ORDER COEFFICIENT IS ZERO

        REMARKS
           LIMITED TO 36TH ORDER POLYNOMIAL OR LESS.
           FLOATING POINT OVERFLOW MAY OCCUR FOR HIGH ORDER
           POLYNOMIALS BUT WILL NOT AFFECT THE ACCURACY OF THE RESULTS.

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

	METHOD
	   NEWTON-RAPHSON ITERATIVE TECHNIQUE.  THE FINAL ITERATIONS
	   ON EACH ROOT ARE PERFORMED USING THE ORIGINAL POLYNOMIAL
	   RATHER THAN THE REDUCED POLYNOMIAL TO AVOID ACCUMULATED
	   ERRORS IN THE REDUCED POLYNOMIAL.

     ..................................................................
 */
int polrt(double xcof[], double cof[], int m, double rootr[], double rooti[])
{
int    i, ict, ifit, in, itemp, kj1, l, mt, n, n2, nx, nxx,ier;
double fi;
double alpha, dx, dy, sumsq, temp, u, ux, uy, v, x, xo, xpr, xt, xt2, 
 y, yo, ypr, yt, yt2;


/*        ...............................................................

        IF A DOUBLE PRECISION VERSION OF THIS ROUTINE IS DESIRED, THE
        C IN COLUMN 1 SHOULD BE REMOVED FROM THE DOUBLE PRECISION
        STATEMENT WHICH FOLLOWS.

     DOUBLE PRECISION XCOF,COF,ROOTR,ROOTI

        THE C MUST ALSO BE REMOVED FROM DOUBLE PRECISION STATEMENTS
        APPEARING IN OTHER ROUTINES USED IN CONJUNCTION WITH THIS
        ROUTINE.
        THE DOUBLE PRECISION VERSION MAY BE MODIFIED BY CHANGING THE
        CONSTANT IN STATEMENT 78 TO 1.0D-12 AND IN STATEMENT 122 TO
        1.0D-10.  THIS WILL PROVIDE HIGHER PRECISION RESULTS AT THE
        COST OF EXECUTION TIME

        ...............................................................
 */
ifit = 0;
n = m;
ier = 0;
switch( for_if(xcof[n + 1-1]) ){
	case -1: goto L_10;
	case  0: goto L_25;
	case  1: goto L_10;
	}
L_10:
switch( for_if(n) ){
	case -1: goto L_15;
	case  0: goto L_15;
	case  1: goto L_32;
	}

/*        SET ERROR CODE TO 1
 */
L_15:
ier = 1;
L_20:
return ier;

/*        SET ERROR CODE TO 4
 */
L_25:
ier = 4;
goto L_20;

/*        SET ERROR CODE TO 2
 */
L_30:
ier = 2;
goto L_20;
L_32:
switch( for_if(n - 36) ){
	case -1: goto L_35;
	case  0: goto L_35;
	case  1: goto L_30;
	}
L_35:
nx = n;
nxx = n + 1;
n2 = 1;
kj1 = n + 1;
for( l = 1; l <= kj1; l++ ){
	mt = kj1 - l + 1;
	cof[mt-1] = xcof[l-1];
	}

/*        SET INITIAL VALUES
 */
L_45:
xo = .00500101;
yo = 0.01000101;

/*        ZERO INITIAL VALUE COUNTER
 */
in = 0;
L_50:
x = xo;

/*        INCREMENT INITIAL VALUES AND COUNTER
 */
xo = -10.0*yo;
yo = -10.0*x;

/*        SET X AND Y TO CURRENT VALUE
 */
x = xo;
y = yo;
in = in + 1;
goto L_59;
L_55:
ifit = 1;
xpr = x;
ypr = y;

/*        EVALUATE POLYNOMIAL AND DERIVATIVES
 */
L_59:
ict = 0;
L_60:
ux = 0.0;
uy = 0.0;
v = 0.0;
yt = 0.0;
xt = 1.0;
u = cof[n + 1-1];
switch( for_if(u) ){
	case -1: goto L_65;
	case  0: goto L_130;
	case  1: goto L_65;
	}
L_65:
for( i = 1; i <= n; i++ ){
	l = n - i + 1;
	temp = cof[l-1];
	xt2 = x*xt - y*yt;
	yt2 = x*yt + y*xt;
	u = u + temp*xt2;
	v = v + temp*yt2;
	fi = i;
	ux = ux + fi*xt*temp;
	uy = uy - fi*yt*temp;
	xt = xt2;
	yt = yt2;
	}
sumsq = ux*ux + uy*uy;
switch( for_if(sumsq) ){
	case -1: goto L_75;
	case  0: goto L_110;
	case  1: goto L_75;
	}
L_75:
dx = (v*uy - u*ux)/sumsq;
x = x + dx;
dy = -(u*uy + v*ux)/sumsq;
y = y + dy;
//L_78:
switch( for_if(fabs( dy ) + fabs( dx ) - 1.0e-05) ){
	case -1: goto L_100;
	case  0: goto L_80;
	case  1: goto L_80;
	}

/*        STEP ITERATION COUNTER
 */
L_80:
ict = ict + 1;
switch( for_if(ict - 500) ){
	case -1: goto L_60;
	case  0: goto L_85;
	case  1: goto L_85;
	}
L_85:
switch( for_if(ifit) ){
	case -1: goto L_100;
	case  0: goto L_90;
	case  1: goto L_100;
	}
L_90:
switch( for_if(in - 5) ){
	case -1: goto L_50;
	case  0: goto L_95;
	case  1: goto L_95;
	}

/*        SET ERROR CODE TO 3
 */
L_95:
ier = 3;
goto L_20;
L_100:
for( l = 1; l <= nxx; l++ ){
	mt = kj1 - l + 1;
	temp = xcof[mt-1];
	xcof[mt-1] = cof[l-1];
	cof[l-1] = temp;
	}
itemp = n;
n = nx;
nx = itemp;
switch( for_if(ifit) ){
	case -1: goto L_120;
	case  0: goto L_55;
	case  1: goto L_120;
	}
L_110:
switch( for_if(ifit) ){
	case -1: goto L_115;
	case  0: goto L_50;
	case  1: goto L_115;
	}
L_115:
x = xpr;
y = ypr;
L_120:
ifit = 0;
//L_122:
switch( for_if(fabs( y ) - 1.0e-4*fabs( x )) ){
	case -1: goto L_135;
	case  0: goto L_125;
	case  1: goto L_125;
	}
L_125:
alpha = x + x;
sumsq = x*x + y*y;
n = n - 2;
goto L_140;
L_130:
x = 0.0;
nx = nx - 1;
nxx = nxx - 1;
L_135:
y = 0.0;
sumsq = 0.0;
alpha = x;
n = n - 1;
L_140:
cof[1] = cof[1] + alpha*cof[0];
//L_145:
for( l = 2; l <= n; l++ ){
	cof[l + 1-1] = cof[l + 1-1] + alpha*cof[l-1] - sumsq*cof[l - 1-1];
	}
L_155:
rooti[n2-1] = y;
rootr[n2-1] = x;
n2 = n2 + 1;
switch( for_if(sumsq) ){
	case -1: goto L_160;
	case  0: goto L_165;
	case  1: goto L_160;
	}
L_160:
y = -y;
sumsq = 0.0;
goto L_155;
L_165:
switch( for_if(n) ){
	case -1: goto L_20;
	case  0: goto L_20;
	case  1: goto L_45;
	}
return ier;
}

