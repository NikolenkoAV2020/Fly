/*------------------------------------------------------------------------
  CSPS.C
    ЗНАЧЕНИЕ РАЗЛОЖЕНИЯ ПО СМЕЩЕННЫМ ПОЛИНОМАМ ЧЕБЫШЕВА
  Автор	 : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия : 01.00 / 01.06.1994 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

	SUBROUTINE csps

        PURPOSE
           COMPUTES THE VALUE OF AN N-TERM EXPANSION IN SHIFTED
	   CHEBYSHEV POLYNOMIALS WITH COEFFICIENT VECTOR C
	   FOR ARGUMENT VALUE X.

	USAGE
	   Y=csps(X,C,N)

        DESCRIPTION OF PARAMETERS
           Y     - RESULT VALUE
                   DOUBLE PRECISION VARIABLE
           X     - ARGUMENT VALUE
                   DOUBLE PRECISION VARIABLE
           C     - COEFFICIENT VECTOR OF GIVEN EXPANSION
                   COEFFICIENTS ARE ORDERED FROM LOW TO HIGH
                   DOUBLE PRECISION VECTOR
           N     - DIMENSION OF COEFFICIENT VECTOR C

        REMARKS
	   OPERATION IS BYPASSED IN CASE N LESS THAN 1

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           DEFINITION
           Y=SUM(C(I)*TS(I-1,X), SUMMED OVER I FROM 1 TO N).
           EVALUATION IS DONE BY MEANS OF BACKWARD RECURSION
           USING THE RECURRENCE EQUATION FOR SHIFTED
           CHEBYSHEV POLYNOMIALS
           TS(N+1,X)=(4*X-2)*TS(N,X)-TS(N-1,X).

     ..................................................................
 */
double csps(double x,double  c[],int n)
{
double y;
int    i, k;
double arg, h0, h1, h2;
y=0.0;

/*        TEST OF DIMENSION */
if(n <= 0 )
  return y;

if( (n - 2) < 0 )
  {
  y = c[0];
  return y;
  }
/*        INITIALIZATION */

arg = x + x - 1.e0; arg = arg + arg;
h1 = 0.e0;          h0 = 0.e0;
for( i = 1 ; i <= n; i++ ){
	k = n - i;  	h2 = h1;
	h1 = h0;
	h0 = arg*h1 - h2 + c[k + 1-1];
	}
y = 0.5e0*(c[0] - h2 + h0);
return y;
}

