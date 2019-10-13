//------------------------------------------------------------------------
//   PVAL.C
//     Расчет значения пoлинoма для аргумента (схема Горнера)
//   Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
//   Версия : 01.00 / 10.06.1996 /
//------------------------------------------------------------------------
#include "ssp.h"
/*     ..................................................................

	SUBROUTINE PVAL

        PURPOSE
           EVALUATE A POLYNOMIAL FOR A GIVEN VALUE OF THE VARIABLE

        USAGE
           CALL PVAL(RES,ARG,X,IDIMX)

        DESCRIPTION OF PARAMETERS
           RES    - RESULTANT VALUE OF POLYNOMIAL
           ARG    - GIVEN VALUE OF THE VARIABLE
           X      - VECTOR OF COEFFICIENTS, ORDERED FROM SMALLEST TO
                    LARGEST POWER
	   IDIMX  - DIMENSION OF X

        REMARKS
           NONE

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
	   EVALUATION IS DONE BY MEANS OF NESTED MULTIPLICATION

     ..................................................................
 */
double pval( double arg, double x[], int idimx)
{
double  res;
int     j;

res = 0.;
j = idimx;
L_1:
switch( for_if(j) ){
	case -1: goto L_3;
	case  0: goto L_3;
	case  1: goto L_2;
	}
L_2:
res = res*arg + x[j-1];
j = j - 1;
goto L_1;
L_3:
return res;
}

