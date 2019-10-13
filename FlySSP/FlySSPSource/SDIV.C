/*------------------------------------------------------------------------
  SDIV.C
     УМНОЖЕНИЕ КАЖДОГО ЭЛЕМЕНТА МАТРИЦЫ НА СКАЛЯР
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	SUBROUTINE SDIV

        PURPOSE
           DIVIDE EACH ELEMENT OF A MATRIX BY A SCALAR TO FORM A
           RESULTANT MATRIX

        USAGE
           CALL SDIV(A,C,R,N,M,MS)

        DESCRIPTION OF PARAMETERS
           A - NAME OF INPUT MATRIX
           C - SCALAR
           R - NAME OF OUTPUT MATRIX
           N - NUMBER OF ROWS IN MATRIX A AND R
	   M - NUMBER OF COLUMNS IN MATRIX A AND R
           MS  - ONE DIGIT NUMBER FOR STORAGE MODE OF MATRIX A (AND R)
                  0 - GENERAL
                  1 - SYMMETRIC
                  2 - DIAGONAL

        REMARKS
           IF SCALAR IS ZERO, DIVISION IS PERFORMED ONLY ONCE TO CAUSE
           FLOATING POINT OVERFLOW CONDITION

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   LOC

	METHOD
	   EACH ELEMENT OF MATRIX IS DIVIDED BY SCALAR

     ..................................................................
 */
void sdiv(double a[], double c, double r[], int n, int m, int ms)
{
int  i, it;

/*        COMPUTE VECTOR LENGTH, IT  */
loc( n-1, m-1, &it, n, m, ms );
/*        DIVIDE BY SCALAR (IF SCALAR IS ZERO, DIVIDE ONLY ONCE)  */
if( c == 0.0)  it = 0;
for( i = 0; i <= it; i++ )
	r[i] = a[i]/c;
return;
}

