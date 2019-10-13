//------------------------------------------------------------------------
//   PSUB.C
//     Вычитание одного пoлинoма из другого
//   Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
//   Версия : 01.00 / 10.06.1996 /
//------------------------------------------------------------------------
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE PSUB

        PURPOSE
           SUBTRACT ONE POLYNOMIAL FROM ANOTHER

	USAGE
           CALL PSUB(Z,IDIMZ,X,IDIMX,Y,IDIMY)

        DESCRIPTION OF PARAMETERS
           Z     - VECTOR OF RESULTANT COEFFICIENTS, ORDERED FROM
                   SMALLEST TO LARGEST POWER
           IDIMZ - DIMENSION OF Z (CALCULATED)
           X     - VECTOR OF COEFFICIENTS FOR FIRST POLYNOMIAL, ORDERED
                   FROM SMALLEST TO LARGEST POWER
           IDIMX - DIMENSION OF X (DEGREE IS IDIMX-1)
           Y     - VECTOR OF COEFFICIENTS FOR SECOND POLYNOMIAL,
                   ORDERED FROM SMALLEST TO LARGEST POWER
           IDIMY - DIMENSION OF Y (DEGREE IS IDIMY-1)

        REMARKS
           VECTOR Z MAY BE IN SAME LOCATION AS EITHER VECTOR X OR
           VECTOR Y ONLY IF THE DIMENSION OF THAT VECTOR IS NOT LESS
           THAN THE OTHER INPUT VECTOR
           THE RESULTANT POLYNOMIAL MAY HAVE TRAILING ZERO COEFFICIENTS

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           DIMENSION OF RESULTANT VECTOR IDIMZ IS CALCULATED AS THE
           LARGER OF THE TWO INPUT VECTOR DIMENSIONS. COEFFICIENTS IN
	   VECTOR Y ARE THEN SUBTRACTED FROM CORRESPONDING COEFFICIENTS
	   IN VECTOR X.

     ..................................................................
 */
void psub(double z[], int *idimz, double x[], int idimx, double y[], int idimy)
{
int i, ndim;


/*     TEST DIMENSIONS OF SUMMANDS */
ndim = idimx;
switch( for_if(idimx - idimy) ){
	case -1: goto L_10;
	case  0: goto L_20;
	case  1: goto L_20;
	}
L_10:
ndim = idimy;
L_20:
switch( for_if(ndim) ){
	case -1: goto L_90;
	case  0: goto L_90;
	case  1: goto L_30;
	}
L_30:
for( i = 1; i <= ndim; i++ ){
	switch( for_if(i - idimx) ){
		case -1: goto L_40;
		case  0: goto L_40;
		case  1: goto L_60;
		}
L_40:
	switch( for_if(i - idimy) ){
		case -1: goto L_50;
		case  0: goto L_50;
		case  1: goto L_70;
		}
L_50:
	z[i-1] = x[i-1] - y[i-1];
	goto L_80;
L_60:
	z[i-1] = -y[i-1];
	goto L_80;
L_70:
	z[i-1] = x[i-1];
L_80:
	;
	}
L_90:
(*idimz) = ndim;
return;
} /* end of function */

