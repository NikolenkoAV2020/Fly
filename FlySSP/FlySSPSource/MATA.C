/*------------------------------------------------------------------------
  MATA.C
     УМНОЖЕНИЕ СЛЕВА МАТРИЦЫ НА ТРАНСПОНИРОВАННУЮ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

#include "ssp.h"
/*     ..................................................................

	SUBROUTINE MATA

	PURPOSE
	   PREMULTIPLY A MATRIX BY ITS TRANSPOSE TO FORM A
	   SYMMETRIC MATRIX

	USAGE
	   MATA(A,R,N,M,MS);

        DESCRIPTION OF PARAMETERS
           A  - NAME OF INPUT MATRIX
           R  - NAME OF OUTPUT MATRIX
           N  - NUMBER OF ROWS IN A
           M  - NUMBER OF COLUMNS IN A. ALSO NUMBER OF ROWS AND
                NUMBER OF COLUMNS OF R.
           MS  - ONE DIGIT NUMBER FOR STORAGE MODE OF MATRIX A
                  0 - GENERAL
		  1 - SYMMETRIC
		  2 - DIAGONAL

	REMARKS
           MATRIX R CANNOT BE IN THE SAME LOCATION AS MATRIX A
           MATRIX R IS ALWAYS A SYMMETRIC MATRIX WITH A STORAGE MODE=1

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           LOC

        METHOD
           CALCULATION OF (A TRANSPOSE A) RESULTS IN A SYMMETRIC MATRIX
           REGARDLESS OF THE STORAGE MODE OF THE INPUT MATRIX. THE
	   ELEMENTS OF MATRIX A ARE NOT CHANGED.

     ..................................................................
 */
void mata(double a[], double r[], int n, int m, int ms)
{
int  i, ia, ib, ir, j, k;
ir=0;
for( k = 0; k < m; k++ ){
	for( j = k; j < m; j++ ){
		for(r[ir] = 0.0 ,i = 0; i < n; i++ ){
			loc( i, j, &ia, n, m, ms );
			loc( i, k, &ib, n, m, ms );
			r[ir] += a[ia]*a[ib];
			}
		ir+=1;
		}
	}
return;
}

