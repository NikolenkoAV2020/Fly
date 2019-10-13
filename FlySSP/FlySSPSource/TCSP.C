/*------------------------------------------------------------------------
  TCSP.C
      ПРЕОБРАЗОВАНИЕ СМЕЩЕННОГО ПОЛИНОМА ЧЕБЫШЕВА В АЛГЕБРАИЧЕСКИЙ ПОЛИНОМ
  Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия : 01.00 / 01.06.1994 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

	SUBROUTINE tcsp

        PURPOSE
           A SERIES EXPANSION IN SHIFTED CHEBYSHEV POLYNOMIALS WITH
           INDEPENDENT VARIABLE X IS TRANSFORMED TO A POLYNOMIAL WITH
           INDEPENDENT VARIABLE Z, WHERE X=A*Z+B.

        USAGE
	   tcsp(A,B,POL,N,C,WORK)

	DESCRIPTION OF PARAMETERS
	   A     - FACTOR OF LINEAR TERM IN GIVEN LINEAR TRANSFORMATION
		   DOUBLE PRECISION VARIABLE
	   B     - CONSTANT TERM IN GIVEN LINEAR TRANSFORMATION
                   DOUBLE PRECISION VARIABLE
           POL   - COEFFICIENT VECTOR OF POLYNOMIAL (RESULTANT VALUE)
                   COEFFICIENTS ARE ORDERED FROM LOW TO HIGH
                   DOUBLE PRECISION VECTOR
           N     - DIMENSION OF COEFFICIENT VECTORS POL AND C
           C     - GIVEN COEFFICIENT VECTOR OF EXPANSION
                   POL AND C MAY BE IDENTICALLY LOCATED
		   COEFFICIENTS ARE ORDERED FROM LOW TO HIGH
                   DOUBLE PRECISION VECTOR
           WORK  - WORKING STORAGE OF DIMENSION 2*N
                   DOUBLE PRECISION ARRAY

        REMARKS
           COEFFICIENT VECTOR C REMAINS UNCHANGED IF NOT COINCIDING
           WITH COEFFICIENT VECTOR POL.
           OPERATION IS BYPASSED IN CASE N LESS THAN 1.
           THE LINEAR TRANSFORMATION X=A*Z+B OR Z=(1/A)(X-B) TRANSFORMS
           THE RANGE (0,1) IN X TO THE RANGE (ZL,ZR) IN Z, WHERE
           ZL=-B/A AND ZR=(1-B)/A.
           FOR GIVEN ZL, ZR WE HAVE A=1/(ZR-ZL) AND B=-ZL/(ZR-ZL).

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           THE TRANSFORMATION IS BASED ON THE RECURRENCE EQUATION FOR
           SHIFTED CHEBYSHEV POLYNOMIALS TS(N,X)
           TS(N+1,X)=(4*X-2)*TS(N,X)-TS(N-1,X),
           WHERE THE FIRST TERM IN BRACKETS IS THE INDEX,
           THE SECOND IS THE ARGUMENT.
           STARTING VALUES ARE TS(0,X)=1, TS(1,X)=2*X-1.
           THE TRANSFORMATION IS IMPLICITLY DEFINED BY MEANS OF
           X=A*Z+B TOGETHER WITH
	   SUM(POL(I)*Z**(I-1), SUMMED OVER I FROM 1 TO N)
	   =SUM(C(I)*TS(I-1,X), SUMMED OVER I FROM 1 TO N).

     ..................................................................
 */
void tcsp(double a,double b,double pol[],int n,double c[],double work[])
{
int    j, k;
double h, p, x0, xd;



/*        TEST OF DIMENSION */

if( (n - 1) < 0) goto L_2;
  else
  if( (n - 1) == 0) goto L_1;
    else goto L_3;

/*        DIMENSION LESS THAN 2 */
L_1:
pol[0] = c[0];
L_2:
return;

L_3:
xd = a + a;
x0 = b + b - 1.e0;
pol[0] = c[0] + c[1]*x0;
pol[1] = c[1]*xd;
if( (n - 2) <= 0) goto L_2;
/*        INITIALIZATION */
work[0] = 1.e0;
work[1] = x0;
work[2] = 0.e0;
work[3] = xd;
xd = xd + xd;
x0 = x0 + x0;

/*        CALCULATE COEFFICIENT VECTOR OF NEXT SHIFTED CHEBYSHEV
	POLYNOMIAL AND ADD MULTIPLE OF THIS VECTOR TO POLYNOMIAL POL */
for( j = 3; j <= n; j++ ){
	p = 0.e0;
	for( k = 2; k <= j; k++ ){
		h = p - work[2*k - 3-1] + x0*work[2*k - 2-1];
		p = work[2*k - 2-1];
		work[2*k - 2-1] = h;
		work[2*k - 3-1] = p;
		pol[k - 1-1] = pol[k - 1-1] + h*c[j-1];
		p = xd*p;
		}
	work[2*j - 1-1] = 0.e0;
	work[2*j-1] = p;
	pol[j-1] = c[j-1]*p;
	}
return;
}

