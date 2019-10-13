/*------------------------------------------------------------------------
  DET5.C
    „””……–‚€…… ”“–, ‡€„€‰ ’€‹–…‰ ‡€—…‰
    ‚ €‚’‘’™• ’—€•,  ”“‹€ ‹€ƒ€†€ 
    ’ “‡‹€
  €Άβ®ΰ   : ¥ΰ¥Ά®¤ ―ΰ®£ΰ ¬¬λ ― ª¥β  SSP ­  ‘ Άλ―®«­¨«  αλ­ª®Ά ‚.‚.
  ‚¥ΰα¨ο  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE DET5

        PURPOSE
           TO COMPUTE A VECTOR OF DERIVATIVE VALUES GIVEN A VECTOR OF
           FUNCTION VALUES WHOSE ENTRIES CORRESPOND TO EQUIDISTANTLY
           SPACED ARGUMENT VALUES.

        USAGE
           CALL DET5(H,Y,Z,NDIM,IER)

        DESCRIPTION OF PARAMETERS
           H     -  CONSTANT DIFFERENCE BETWEEN SUCCESSIVE ARGUMENT
                    VALUES (H IS POSITIVE IF THE ARGUMENT VALUES
                    INCREASE AND NEGATIVE OTHERWISE)
           Y     -  GIVEN VECTOR OF FUNCTION VALUES (DIMENSION NDIM)
           Z     -  RESULTING VECTOR OF DERIVATIVE VALUES (DIMENSION
                    NDIM)
           NDIM  -  DIMENSION OF VECTORS Y AND Z
           IER   -  RESULTING ERROR PARAMETER
                    IER = -1  - NDIM IS LESS THAN 5
                    IER =  0  - NO ERROR
                    IER =  1  - H = 0

        REMARKS
           (1)   IF IER = -1,1, THEN THERE IS NO COMPUTATION.
           (2)   Z CAN HAVE THE SAME STORAGE ALLOCATION AS Y.  IF Y IS
                 DISTINCT FROM Z, THEN IT IS NOT DESTROYED.

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           IF X IS THE (SUPPRESSED) VECTOR OF ARGUMENT VALUES, THEN
           EXCEPT AT THE POINTS X(1),X(2),X(NDIM-1) AND X(NDIM), Z(I)
           IS THE DERIVATIVE AT X(I) OF THE LAGRANGIAN INTERPOLATION
           POLYNOMIAL OF DEGREE 4 RELEVANT TO THE 5 SUCCESSIVE POINTS
           (X(I+K),Y(I+K)) K = -2,-1,...,2.  (SEE HILDEBRAND, F.B.,
           INTRODUCTION TO NUMERICAL ANALYSIS, MC GRAW-HILL, NEW YORK/
           TORONTO/LONDON, 1956, PP. 82-84.)

     ..................................................................
 */
int det5(double h, double y[], double z[], int ndim)
{
int    ier, i;
double a, b, c, hh, yy;




/*        TEST OF DIMENSION */
switch( for_if(ndim - 5) ){
	case -1: goto L_4;
	case  0: goto L_1;
	case  1: goto L_1;
	}

/*        TEST OF STEPSIZE */
L_1:
switch( for_if(h) ){
	case -1: goto L_2;
	case  0: goto L_5;
	case  1: goto L_2;
	}

/*        PREPARE DIFFERENTIATION LOOP */
L_2:
hh = .08333333333333333/h;
yy = y[ndim - 4-1];
b = hh*(-25.*y[0] + 48.*y[1] - 36.*y[2] + 16.*y[3] - 3.*y[4]);
c = hh*(-3.*y[0] - 10.*y[1] + 18.*y[2] - 6.*y[3] + y[4]);

/*        START DIFFERENTIATION LOOP */
for( i = 5 ; i <= ndim; i++ ){
	a = b;
	b = c;
	c = hh*(y[i - 4-1] - y[i-1] + 8.*(y[i - 1-1] - y[i - 3-1]));
//L_3:
	z[i - 4-1] = a;
	}
/*        END OF DIFFERENTIATION LOOP

        NORMAL EXIT */
ier = 0;
a = hh*(-yy + 6.*y[ndim - 3-1] - 18.*y[ndim - 2-1] + 10.*y[ndim - 1-1]
  + 3.*y[ndim-1]);
z[ndim-1] = hh*(3.*yy - 16.*y[ndim - 3-1] + 36.*y[ndim - 2-1]
  - 48.*y[ndim - 1-1] + 25.*y[ndim-1]);
z[ndim - 1-1] = a;
z[ndim - 2-1] = c;
z[ndim - 3-1] = b;
return ier;

/*        ERROR EXIT IN CASE NDIM IS LESS THAN 5 */
L_4:
ier = -1;
return ier;

/*        ERROR EXIT IN CASE OF ZERO STEPSIZE */
L_5:
ier = 1;
return ier;
}

