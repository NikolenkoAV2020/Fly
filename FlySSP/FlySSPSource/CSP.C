/*------------------------------------------------------------------------
  CNP.C
    ВЫЧИСЛЕНИЕ ЗНАЧЕНИЙ СМЕЩЕННЫХ ПОЛИНОМОВ ЧЕБЫШЕВА [0,1]
  Автор	 : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия : 01.00 / 10.06.1996 /
------------------------------------------------------------------------*/
#include "ssp.h"
/*     ..................................................................

	SUBROUTINE csp

	PURPOSE
	   COMPUTE THE VALUES OF THE SHIFTED CHEBYSHEV POLYNOMIALS
	   TS(N,X) FOR ARGUMENT X AND ORDERS 0 UP TO N.

	USAGE
	   csp(Y,X,N)

	DESCRIPTION OF PARAMETERS
	   Y     - RESULT VECTOR OF DIMENSION N+1 CONTAINING THE VALUES
		   OF SHIFTED CHEBYSHEV POLYNOMIALS OF ORDER 0 UP TO N
		   FOR GIVEN ARGUMENT X.
		   DOUBLE PRECISION VECTOR.
		   VALUES ARE ORDERED FROM LOW TO HIGH ORDER
	   X     - ARGUMENT OF SHIFTED CHEBYSHEV POLYNOMIAL
		   DOUBLE PRECISION VARIABLE.
	   N     - ORDER OF SHIFTED CHEBYSHEV POLYNOMIAL

	REMARKS
	   N LESS THAN 0 IS TREATED AS IF N WERE 0

	SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   NONE

	METHOD
	   EVALUATION IS BASED ON THE RECURRENCE EQUATION FOR
	   SHIFTED CHEBYSHEV POLYNOMIALS TS(N,X)
	   TS(N+1,X)=(4*X-2)*TS(N,X)-TS(N-1,X),
	   WHERE THE FIRST TERM IN BRACKETS IS THE ORDER,
	   THE SECOND IS THE ARGUMENT.
	   STARTING VALUES ARE TS(0,X)=1, TS(1,X)=2*X-1.

     ..................................................................
 */
void csp(double y[],double x,int n)
{
int    i;
double f;
/*        TEST OF ORDER */
y[0] = 1.e0;
if( n <= 0 )
  return;
y[1] = x + x - 1.e0;
if( (n - 1) <=0 )
  return;
/*        INITIALIZATION */
f = y[1] + y[1];

for( i = 2; i <= n; i++ )
	y[i + 1-1] = f*y[i-1] - y[i - 1-1];
return;
}

