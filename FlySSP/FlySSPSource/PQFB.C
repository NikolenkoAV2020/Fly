/*------------------------------------------------------------------------
  PQFB.C
      НАХОЖДЕНИЕ ...
  Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия : 01.00 / 01.06.1994 /
------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	SUBROUTINE PQFB

	PURPOSE
	   TO FIND AN APPROXIMATION Q(X)=Q1+Q2*X+X*X TO A QUADRATIC
	   FACTOR OF A GIVEN POLYNOMIAL P(X) WITH REAL COEFFICIENTS.

        USAGE
	   CALL PQFB(C,IC,Q,LIM,IER)

        DESCRIPTION OF PARAMETERS
           C   - INPUT VECTOR CONTAINING THE COEFFICIENTS OF P(X) -
                 C(1) IS THE CONSTANT TERM (DIMENSION IC)
           IC  - DIMENSION OF C
           Q   - VECTOR OF DIMENSION 4 - ON INPUT Q(1) AND Q(2) MUST
                 CONTAIN INITIAL GUESSES FOR Q1 AND Q2 - ON RETURN Q(1)
                 AND Q(2) CONTAIN THE REFINED COEFFICIENTS Q1 AND Q2 OF
                 Q(X), WHILE Q(3) AND Q(4) CONTAIN THE COEFFICIENTS A
                 AND B OF A+B*X, WHICH IS THE REMAINDER OF THE QUOTIENT
		 OF P(X) BY Q(X)
           LIM - INPUT VALUE SPECIFYING THE MAXIMUM NUMBER OF
                 ITERATIONS TO BE PERFORMED
           IER - RESULTING ERROR PARAMETER (SEE REMARKS)
                 IER= 0 - NO ERROR
                 IER= 1 - NO CONVERGENCE WITHIN LIM ITERATIONS
                 IER=-1 - THE POLYNOMIAL P(X) IS CONSTANT OR UNDEFINED
                          - OR OVERFLOW OCCURRED IN NORMALIZING P(X)
                 IER=-2 - THE POLYNOMIAL P(X) IS OF DEGREE 1
                 IER=-3 - NO FURTHER REFINEMENT OF THE APPROXIMATION TO
                          A QUADRATIC FACTOR IS FEASIBLE, DUE TO EITHER
			  DIVISION BY 0, OVERFLOW OR AN INITIAL GUESS
                          THAT IS NOT SUFFICIENTLY CLOSE TO A FACTOR OF
                          P(X)

        REMARKS
           (1)  IF IER=-1 THERE IS NO COMPUTATION OTHER THAN THE
                POSSIBLE NORMALIZATION OF C.
           (2)  IF IER=-2 THERE IS NO COMPUTATION OTHER THAN THE
                NORMALIZATION OF C.
           (3)  IF IER =-3  IT IS SUGGESTED THAT A NEW INITIAL GUESS BE
		MADE FOR A QUADRATIC FACTOR.  Q, HOWEVER, WILL CONTAIN
		THE VALUES ASSOCIATED WITH THE ITERATION THAT YIELDED
                THE SMALLEST NORM OF THE MODIFIED LINEAR REMAINDER.
           (4)  IF IER=1, THEN, ALTHOUGH THE NUMBER OF ITERATIONS LIM
                WAS TOO SMALL TO INDICATE CONVERGENCE, NO OTHER PROB-
                LEMS HAVE BEEN DETECTED, AND Q WILL CONTAIN THE VALUES
                ASSOCIATED WITH THE ITERATION THAT YIELDED THE SMALLEST
                NORM OF THE MODIFIED LINEAR REMAINDER.
           (5)  FOR COMPLETE DETAIL SEE THE DOCUMENTATION FOR
                SUBROUTINES PQFB AND DPQFB.

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   NONE

        METHOD
           COMPUTATION IS BASED ON BAIRSTOW'S ITERATIVE METHOD.  (SEE
           WILKINSON, J.H., THE EVALUATION OF THE ZEROS OF ILL-CON-
           DITIONED POLYNOMIALS (PART ONE AND TWO), NUMERISCHE MATHE-
           MATIK, VOL.1 (1959), PP. 150-180, OR HILDEBRAND, F.B.,
	   INTRODUCTION TO NUMERICAL ANALYSIS, MC GRAW-HILL, NEW YORK/
	   TORONTO/LONDON, 1956, PP. 472-476.)

     ..................................................................
 */
int pqfb(double c[],int ic,double q[],int lim)
{
int    i, j, l, ll, n, ier;
double a, a1, aa, b, b1, bb, c1, ca, cb, cc, cd, dq1, dq2, eps, eps1,
       h, hh, q1, q2, qq1, qq2, qqq1, qqq2;

h=hh=0;


/*        TEST ON LEADING ZERO COEFFICIENTS */
ier = 0;
j = ic + 1;
L_1:
j = j - 1;
switch( for_if(j - 1) ){
	case -1: goto L_40;
	case  0: goto L_40;
	case  1: goto L_2;
	}
L_2:
switch( for_if(c[j-1]) ){
	case -1: goto L_3;
	case  0: goto L_1;
	case  1: goto L_3;
	}

/*        NORMALIZATION OF REMAINING COEFFICIENTS */
L_3:
a = c[j-1];
switch( for_if(a - 1.) ){
	case -1: goto L_4;
	case  0: goto L_6;
	case  1: goto L_4;
	}
L_4:
for( i = 1; i <= j; i++ ){
	c[i-1] = c[i-1]/a;
	overfl( &n );
	switch( for_if(n - 2) ){
		case -1: goto L_40;
		case  0: goto L_5;
		case  1: goto L_5;
		}
L_5:
	;
	}

/*        TEST ON NECESSITY OF BAIRSTOW ITERATION */
L_6:
switch( for_if(j - 3) ){
	case -1: goto L_41;
	case  0: goto L_38;
	case  1: goto L_7;
	}

/*        PREPARE BAIRSTOW ITERATION */
L_7:
eps = 1.e-6;
eps1 = 1.e-3;
l = 0;
ll = 0;
q1 = q[0];
q2 = q[1];
qq1 = 0.;
qq2 = 0.;
aa = c[0];
bb = c[1];
cb = fabs( aa );
ca = fabs( bb );
switch( for_if(cb - ca) ){
	case -1: goto L_8;
	case  0: goto L_9;
	case  1: goto L_10;
	}
L_8:
cc = cb + cb;
cb = cb/ca;
ca = 1.;
goto L_11;
L_9:
cc = ca + ca;
ca = 1.;
cb = 1.;
goto L_11;
L_10:
cc = ca + ca;
ca = ca/cb;
cb = 1.;
L_11:
cd = cc*.1;

/*        START BAIRSTOW ITERATION
        PREPARE NESTED MULTIPLICATION */
L_12:
a = 0.;
b = a;
a1 = a;
b1 = a;
i = j;
qqq1 = q1;
qqq2 = q2;
dq1 = hh;
dq2 = h;

/*        START NESTED MULTIPLICATION */
L_13:
h = -q1*b - q2*a + c[i-1];
overfl( &n );
switch( for_if(n - 2) ){
	case -1: goto L_42;
	case  0: goto L_14;
	case  1: goto L_14;
	}
L_14:
b = a;
a = h;
i = i - 1;
switch( for_if(i - 1) ){
	case -1: goto L_18;
	case  0: goto L_15;
	case  1: goto L_16;
	}
L_15:
h = 0.;
L_16:
h = -q1*b1 - q2*a1 + h;
overfl( &n );
switch( for_if(n - 2) ){
	case -1: goto L_42;
	case  0: goto L_17;
	case  1: goto L_17;
	}
L_17:
c1 = b1;
b1 = a1;
a1 = h;
goto L_13;
/*        END OF NESTED MULTIPLICATION

        TEST ON SATISFACTORY ACCURACY */
L_18:
h = ca*fabs( a ) + cb*fabs( b );
switch( for_if(ll) ){
	case -1: goto L_19;
	case  0: goto L_19;
	case  1: goto L_39;
	}
L_19:
l = l + 1;
switch( for_if(fabs( a ) - eps*fabs( c[0] )) ){
	case -1: goto L_20;
	case  0: goto L_20;
	case  1: goto L_21;
	}
L_20:
switch( for_if(fabs( b ) - eps*fabs( c[1] )) ){
	case -1: goto L_39;
	case  0: goto L_39;
	case  1: goto L_21;
	}

/*        TEST ON LINEAR REMAINDER OF MINIMUM NORM */
L_21:
switch( for_if(h - cc) ){
	case -1: goto L_22;
	case  0: goto L_22;
	case  1: goto L_23;
	}
L_22:
aa = a;
bb = b;
cc = h;
qq1 = q1;
qq2 = q2;

/*        TEST ON LAST ITERATION STEP */
L_23:
switch( for_if(l - lim) ){
	case -1: goto L_28;
	case  0: goto L_28;
	case  1: goto L_24;
	}

/*        TEST ON RESTART OF BAIRSTOW ITERATION WITH ZERO INITIAL GUESS */
L_24:
switch( for_if(h - cd) ){
	case -1: goto L_43;
	case  0: goto L_43;
	case  1: goto L_25;
	}
L_25:
switch( for_if(q[0]) ){
	case -1: goto L_27;
	case  0: goto L_26;
	case  1: goto L_27;
	}
L_26:
switch( for_if(q[1]) ){
	case -1: goto L_27;
	case  0: goto L_42;
	case  1: goto L_27;
	}
L_27:
q[0] = 0.;
q[1] = 0.;
goto L_7;

/*        PERFORM ITERATION STEP */
L_28:
hh = vfmax( fabs( a1 ), fabs( b1 ), fabs( c1 ), FEND );
switch( for_if(hh) ){
	case -1: goto L_42;
	case  0: goto L_42;
	case  1: goto L_29;
	}
L_29:
a1 = a1/hh;
b1 = b1/hh;
c1 = c1/hh;
h = a1*c1 - b1*b1;
switch( for_if(h) ){
	case -1: goto L_30;
	case  0: goto L_42;
	case  1: goto L_30;
	}
L_30:
a = a/hh;
b = b/hh;
hh = (b*a1 - a*b1)/h;
h = (a*c1 - b*b1)/h;
q1 = q1 + hh;
q2 = q2 + h;
/*        END OF ITERATION STEP

	TEST ON SATISFACTORY RELATIVE ERROR OF ITERATED VALUES */
switch( for_if(fabs( hh ) - eps*fabs( q1 )) ){
	case -1: goto L_31;
	case  0: goto L_31;
	case  1: goto L_33;
	}
L_31:
switch( for_if(fabs( h ) - eps*fabs( q2 )) ){
	case -1: goto L_32;
	case  0: goto L_32;
	case  1: goto L_33;
	}
L_32:
ll = 1;
goto L_12;

/*        TEST ON DECREASING RELATIVE ERRORS */
L_33:
switch( for_if(l - 1) ){
	case -1: goto L_12;
	case  0: goto L_12;
	case  1: goto L_34;
	}
L_34:
switch( for_if(fabs( hh ) - eps1*fabs( q1 )) ){
	case -1: goto L_35;
	case  0: goto L_35;
	case  1: goto L_12;
	}
L_35:
switch( for_if(fabs( h ) - eps1*fabs( q2 )) ){
	case -1: goto L_36;
	case  0: goto L_36;
	case  1: goto L_12;
	}
L_36:
switch( for_if(fabs( qqq1*hh ) - fabs( q1*dq1 )) ){
	case -1: goto L_37;
	case  0: goto L_44;
	case  1: goto L_44;
	}
L_37:
switch( for_if(fabs( qqq2*h ) - fabs( q2*dq2 )) ){
	case -1: goto L_12;
	case  0: goto L_44;
	case  1: goto L_44;
	}
/*        END OF BAIRSTOW ITERATION

        EXIT IN CASE OF QUADRATIC POLYNOMIAL */
L_38:
q[0] = c[0];
q[1] = c[1];
q[2] = 0.;
q[3] = 0.;
return ier;

/*        EXIT IN CASE OF SUFFICIENT ACCURACY */
L_39:
q[0] = q1;
q[1] = q2;
q[2] = a;
q[3] = b;
return ier;

/*        ERROR EXIT IN CASE OF ZERO OR CONSTANT POLYNOMIAL */
L_40:
ier = -1;
return ier;

/*        ERROR EXIT IN CASE OF LINEAR POLYNOMIAL */
L_41:
ier = -2;
return ier;

/*        ERROR EXIT IN CASE OF NONREFINED QUADRATIC FACTOR */
L_42:
ier = -3;
goto L_44;

/*        ERROR EXIT IN CASE OF UNSATISFACTORY ACCURACY */
L_43:
ier = 1;
L_44:
q[0] = qq1;
q[1] = qq2;
q[2] = aa;
q[3] = bb;
return ier;
}

