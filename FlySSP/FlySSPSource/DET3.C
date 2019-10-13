/*------------------------------------------------------------------------
  DET3.C
    „””……–‚€… ”“–, ‡€„€‰ ’€‹–…‰ ‡€—…‰
    ‚ €‚’‘’™• ’—€•,  ”“‹€ ‹€ƒ€†€ 
    ’… “‡‹€
  €Άβ®ΰ   : ¥ΰ¥Ά®¤ ―ΰ®£ΰ ¬¬λ ― ª¥β  SSP ­  ‘ Άλ―®«­¨«  αλ­ª®Ά ‚.‚.
  ‚¥ΰα¨ο  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"
/*     ..................................................................

        SUBROUTINE DET3

        PURPOSE
           TO COMPUTE A VECTOR OF DERIVATIVE VALUES GIVEN A VECTOR OF
           FUNCTION VALUES WHOSE ENTRIES CORRESPOND TO EQUIDISTANTLY
	   SPACED ARGUMENT VALUES.

        USAGE
	   CALL DET3(H,Y,Z,NDIM,IER)

	DESCRIPTION OF PARAMETERS
	   H     -  CONSTANT DIFFERENCE BETWEEN SUCCESSIVE ARGUMENT
		    VALUES (H IS POSITIVE IF THE ARGUMENT VALUES
		    INCREASE AND NEGATIVE OTHERWISE)
	   Y     -  GIVEN VECTOR OF FUNCTION VALUES (DIMENSION NDIM)
	   Z     -  RESULTING VECTOR OF DERIVATIVE VALUES (DIMENSION
		    NDIM)
	   NDIM  -  DIMENSION OF VECTORS Y AND Z
	   IER   -  RESULTING ERROR PARAMETER
		    IER = -1  - NDIM IS LESS THAN 3
		    IER =  0  - NO ERROR
		    IER =  1  - H = 0

	REMARKS
	   (1)   IF IER = -1,1, THEN THERE IS NO COMPUTATION.
           (2)   Z CAN HAVE THE SAME STORAGE ALLOCATION AS Y. IF Y IS
                 DISTINCT FROM Z, THEN IT IS NOT DESTROYED.

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

	METHOD
	   IF X IS THE (SUPPRESSED) VECTOR OF ARGUMENT VALUES, THEN
	   EXCEPT AT THE ENDPOINTS X(1) AND X(NDIM), Z(I) IS THE
           DERIVATIVE AT X(I) OF THE LAGRANGIAN INTERPOLATION
           POLYNOMIAL OF DEGREE 2 RELEVANT TO THE 3 SUCCESSIVE POINTS
           (X(I+K),Y(I+K)) K = -1,0,1.  (SEE HILDEBRAND, F.B.,
           INTRODUCTION TO NUMERICAL ANALYSIS, MC-GRAW-HILL, NEW YORK/
           TORONTO/LONDON, 1956, PP.82-84.)

     ..................................................................
 */
int det3(double h, double y[], double z[], int ndim)
{
int   ier,i;
double a, b, hh, yy;




/*        TEST OF DIMENSION */
switch( for_if(ndim - 3) ){
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
hh = .5/h;
yy = y[ndim - 2-1];
b = y[1] + y[1];
b = hh*(b + b - y[2] - y[0] - y[0] - y[0]);

/*        START DIFFERENTIATION LOOP */
for( i = 3 ; i <= ndim; i++ ){
	a = b;
	b = hh*(y[i-1] - y[i - 2-1]);
//L_3:
	z[i - 2-1] = a;
	}
/*        END OF DIFFERENTIATION LOOP

	NORMAL EXIT */
ier = 0;
a = y[ndim - 1-1] + y[ndim - 1-1];
z[ndim-1] = hh*(y[ndim-1] + y[ndim-1] + y[ndim-1] - a -
 a + yy);
z[ndim - 1-1] = b;
return ier;

/*        ERROR EXIT IN CASE NDIM IS LESS THAN 3 */
L_4:
ier = -1;
return ier;

/*        ERROR EXIT IN CASE OF ZERO STEPSIZE */
L_5:
ier = 1;
return ier;
}

