/*---------------------------------------------------------
	NDTRI.C
	ОБРАТНАЯ ФУНКЦИЯ НОРМАЛЬНОГО РАСПРЕДЕЛЕНИЯ
	Автор	: Пасынков В.В.
	Версия 	: 01.00 / 25.09.1996 /
---------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/*.......................................................................

	ПОДПРОГРАММА NDTRI

        PURPOSE
          COMPUTES X = P**(-1)(Y), THE ARGUMENT X SUCH THAT Y= P(X) =
          THE PROBABILITY THAT THE RANDOM VARIABLE U, DISTRIBUTED
          NORMALLY(0,1), IS LESS THAN OR EQUAL TO X.  F(X), THE
	  ORDINATE OF THE NORMAL DENSITY, AT X, IS ALSO COMPUTED.

	USAGE
	  NDTRI(P,X,D,IER)

        DESCRIPTION OF PARAMETERS
          P   - INPUT PROBABILITY.
          X   - OUTPUT ARGUMENT SUCH THAT P = Y = THE PROBABILITY THAT
                   U, THE RANDOM VARIABLE, IS LESS THAN OR EQUAL TO X.
          D   - OUTPUT DENSITY, F(X).
	  IER - OUTPUT ERROR CODE
                = -1 IF P IS NOT IN THE INTERVAL (0,1), INCLUSIVE.
		  X=D=.99999E+37 IN THIS CASE
                = 0 IF THERE IS NO ERROR.  SEE REMARKS, BELOW.

	REMARKS
          MAXIMUM ERROR IS 0.00045.
          IF P = 0, X IS SET TO -(10)**74.  D IS SET TO 0.
          IF P = 1, X IS SET TO  (10)**74.  D IS SET TO 0.

	SUBROUTINES AND SUBPROGRAMS REQUIRED
	  NONE

        METHOD
	  BASED ON APPROXIMATIONS IN C. HASTINGS, APPROXIMATIONS FOR
          DIGITAL COMPUTERS, PRINCETON UNIV. PRESS, PRINCETON, N.J.,
          1955.  SEE EQUATION 26.2.23, HANDBOOK OF MATHEMATICAL
	  FUNCTIONS, ABRAMOWITZ AND STEGUN, DOVER PUBLICATIONS, INC.,
          NEW YORK.

.......................................................................
 */
/*
  Подпрограмма NDTRI
  alpha - уровень значимости [0.0,0.1]
  Используется для расчета доверительных границ нормального
  отклонения (или alpha-значений нормального отклонения):
  │u│   = u        = │u│        = u
     P     (1+P)/2      1-alpha    1 - alpha/2
  Например:
  a) 5% уровень значимости (двухсторонний)
       │u│    = u      = 1.96
	  0.95   0.975
  ndtri(1.0-0.05/2,&u,&d);   P=0.975 -> u=1.96
  б) 5% ошибка 2-го рода (односторонний)
  ndtri(1.0-0.05,&u,&d);     P=0.95  -> u=1.645
*/
int ndtri( float p,float  *x,float  *d)
{
int      ie;
float    t, t2;


ie = 0;
(*x) = (float)0.99999e37;
(*d) = (*x);
//if(p<0)
switch( for_if(p) ){
	case -1: goto L_1;
	case  0: goto L_4;
	case  1: goto L_2;
	}
L_1:
ie = -1;
goto L_12;
L_2:
switch( for_if(p - 1.0) ){
	case -1: goto L_7;
	case  0: goto L_5;
	case  1: goto L_1;
	}
L_4:
(*x) = (float)-.999999e37;
L_5:
(*d) = 0.0;
goto L_12;


L_7:
(*d) = p;
switch( for_if((*d) - 0.5f) ){
	case -1: goto L_9;
	case  0: goto L_9;
	case  1: goto L_8;
	}
L_8:
(*d) = 1.0f - (*d);
L_9:
t2 = (float)log( 1.0/((*d)*(*d)) );
t = (float)sqrt( t2 );
(*x) = t - (2.515517f + 0.802853f*t + 0.010328f*t2)/(1.0f + 1.432788f*t +
 0.189269f*t2 + 0.001308f*t*t2);
switch( for_if(p - 0.5f) ){
	case -1: goto L_10;
	case  0: goto L_10;
	case  1: goto L_11;
	}
L_10:
(*x) = -(*x);
L_11:
(*d) = (float)(0.3989423f*exp( -(*x)*(*x)/2.0 ));
L_12:
return ie;
}

