/*------------------------------------------------------------------------
  DGT3.C
    ÑàîîÖêÖçñàêéÇÄÖàÖ îìçäñàà, áÄÑÄççéâ íÄÅãàñÖâ áçÄóÖçàâ
    Ç çÖêÄÇçééíëíéüôàï íéóäÄï
  Ä¢‚Æ‡   : è•‡•¢Æ§ Ø‡Æ£‡†¨¨Î Ø†™•‚† SSP ≠† ë ¢ÎØÆ´≠®´ è†·Î≠™Æ¢ Ç.Ç.
  Ç•‡·®Ô  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"
/*     ..................................................................

        SUBROUTINE DGT3

        PURPOSE
           TO COMPUTE A VECTOR OF DERIVATIVE VALUES GIVEN VECTORS OF
           ARGUMENT VALUES AND CORRESPONDING FUNCTION VALUES.

	USAGE
           CALL DGT3(X,Y,Z,NDIM,IER)

        DESCRIPTION OF PARAMETERS
           X     -  GIVEN VECTOR OF ARGUMENT VALUES (DIMENSION NDIM)
           Y     -  GIVEN VECTOR OF FUNCTION VALUES CORRESPONDING TO X
                    (DIMENSION NDIM)
           Z     -  RESULTING VECTOR OF DERIVATIVE VALUES (DIMENSION
                    NDIM)
           NDIM  -  DIMENSION OF VECTORS X,Y AND Z
           IER   -  RESULTING ERROR PARAMETER
                    IER  = -1  - NDIM IS LESS THAN 3
                    IER  =  0  - NO ERROR
                    IER POSITIVE  - X(IER) = X(IER-1) OR X(IER) =
                                    X(IER-2)

        REMARKS
           (1)   IF IER = -1,2,3, THEN THERE IS NO COMPUTATION.
           (2)   IF IER =  4,...,N, THEN THE DERIVATIVE VALUES Z(1)
                 ,..., Z(IER-1) HAVE BEEN COMPUTED.
           (3)   Z CAN HAVE THE SAME STORAGE ALLOCATION AS X OR Y.  IF
                 X OR Y IS DISTINCT FROM Z, THEN IT IS NOT DESTROYED.

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           EXCEPT AT THE ENDPOINTS X(1) AND X(NDIM), Z(I) IS THE
           DERIVATIVE AT X(I) OF THE LAGRANGIAN INTERPOLATION
           POLYNOMIAL OF DEGREE 2 RELEVANT TO THE 3 SUCCESSIVE POINTS
           (X(I+K),Y(I+K)) K = -1,0,1. (SEE HILDEBRAND, F.B.,
           INTRODUCTION TO NUMERICAL ANALYSIS, MC GRAW-HILL, NEW YORK/
           TORONTO/LONDON, 1956, PP. 64-68.)

     ..................................................................
 */
int dgt3(double x[], double y[], double z[], int ndim)
{
int    i, ier;
double a, b, dy1, dy2, dy3;




/*        TEST OF DIMENSION AND ERROR EXIT IN CASE NDIM IS LESS THAN 3 */
ier = -1;
switch( for_if(ndim - 3) ){
	case -1: goto L_8;
	case  0: goto L_1;
	case  1: goto L_1;
	}

/*        PREPARE DIFFERENTIATION LOOP */
L_1:
a = x[0];
b = y[0];
i = 2;
dy2 = x[1] - a;
switch( for_if(dy2) ){
	case -1: goto L_2;
	case  0: goto L_9;
	case  1: goto L_2;
	}
L_2:
dy2 = (y[1] - b)/dy2;

/*        START DIFFERENTIATION LOOP */
for( i = 3; i <= ndim; i++ ){
	a = x[i-1] - a;
	switch( for_if(a) ){
		case -1: goto L_3;
		case  0: goto L_9;
		case  1: goto L_3;
		}
L_3:
	a = (y[i-1] - b)/a;
	b = x[i-1] - x[i - 1-1];
	switch( for_if(b) ){
		case -1: goto L_4;
		case  0: goto L_9;
		case  1: goto L_4;
		}
L_4:
	dy1 = dy2;
	dy2 = (y[i-1] - y[i - 1-1])/b;
	dy3 = a;
	a = x[i - 1-1];
	b = y[i - 1-1];
	switch( for_if(i - 3) ){
		case -1: goto L_5;
		case  0: goto L_5;
		case  1: goto L_6;
		}
L_5:
	z[0] = dy1 + dy3 - dy2;
L_6:
	z[i - 1-1] = dy1 + dy2 - dy3;
	}
/*        END DIFFERENTIATION LOOP

        NORMAL EXIT */
ier = 0;
i = ndim;
L_7:
z[i-1] = dy2 + dy3 - dy1;
L_8:
return ier;

/*        ERROR EXIT IN CASE OF IDENTICAL ARGUMENTS */
L_9:
ier = i;
i = i - 1;
switch( for_if(i - 2) ){
	case -1: goto L_8;
	case  0: goto L_8;
	case  1: goto L_7;
	}
return ier;
}

