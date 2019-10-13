//------------------------------------------------------------------------
//   PCLA.C
//     MOVE POLYNOMIAL X TO Y
//   Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
//   Версия : 01.00 / 10.06.1996 /
//------------------------------------------------------------------------
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE PCLA

        PURPOSE
           MOVE POLYNOMIAL X TO Y

        USAGE
           CALL PCLA(Y,IDIMY,X,IDIMX)

        DESCRIPTION OF PARAMETERS
           Y     - VECTOR OF RESULTANT COEFFICIENTS, ORDERED FROM
                   SMALLEST TO LARGEST POWER
           IDIMY - DIMENSION OF Y
           X     - VECTOR OF COEFFICIENTS FOR POLYNOMIAL, ORDERED
                   FROM SMALLEST TO LARGEST POWER
           IDIMX - DIMENSION OF X

        REMARKS
           NONE

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           IDIMY IS REPLACED BY IDIMX AND VECTOR X IS MOVED TO Y

     ..................................................................
 */
void pcla(double y[], int *idimy, double x[], int idimx)
{
int i;


(*idimy) = idimx;
switch( for_if(idimx) ){
	case -1: goto L_30;
	case  0: goto L_30;
	case  1: goto L_10;
	}
L_10:
for( i = 1; i <= idimx; i++ ){
	y[i-1] = x[i-1];
	}
L_30:
return;
}

