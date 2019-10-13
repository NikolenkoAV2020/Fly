//------------------------------------------------------------------------
//   PNORM.C
//     Нopмиpoвкa вeктopa кoэффициeнтoв пoлинoмa
//   Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
//   Версия : 01.00 / 10.06.1996 /
//------------------------------------------------------------------------
#include "ssp.h"
#include <math.h>

/*     ..................................................................

        SUBROUTINE PNORM

        PURPOSE
           NORMALIZE COEFFICIENT VECTOR OF A POLYNOMIAL

        USAGE
           CALL PNORM(X,IDIMX,EPS)

        DESCRIPTION OF PARAMETERS
           X      - VECTOR OF ORIGINAL COEFFICIENTS, ORDERED FROM
                    SMALLEST TO LARGEST POWER. IT REMAINS UNCHANGED
           IDIMX  - DIMENSION OF X. IT IS REPLACED BY FINAL DIMENSION
           EPS    - TOLERANCE BELOW WHICH COEFFICIENT IS ELIMINATED

        REMARKS
	   IF ALL COEFFICIENTS ARE LESS THAN EPS, RESULT IS A ZERO
           POLYNOMIAL WITH IDIMX=0 BUT VECTOR X REMAINS INTACT

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

	METHOD
	   DIMENSION OF VECTOR X IS REDUCED BY ONE FOR EACH TRAILING
	   COEFFICIENT WITH AN ABSOLUTE VALUE LESS THAN OR EQUAL TO EPS

     ..................................................................
 */
int pnorm(double x[], int idimx, double eps)
{
L_1:
switch( for_if(idimx) ){
	case -1: goto L_4;
	case  0: goto L_4;
	case  1: goto L_2;
	}
L_2:
switch( for_if(fabs( x[idimx-1] ) - eps) ){
	case -1: goto L_3;
	case  0: goto L_3;
	case  1: goto L_4;
	}
L_3:
idimx = idimx - 1;
goto L_1;
L_4:
return idimx;
}

