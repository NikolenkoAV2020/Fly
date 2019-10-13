/*------------------------------------------------------------------------
  MTRA.C
	  ТРАНСПОНИРОВАНИЕ МАТРИЦЫ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"
/*     ..................................................................

        SUBROUTINE MTRA

	PURPOSE
           TRANSPOSE A MATRIX

        USAGE
	   mtra(A,R,N,M,MS)

        DESCRIPTION OF PARAMETERS
           A - NAME OF MATRIX TO BE TRANSPOSED
	   R - NAME OF OUTPUT MATRIX
           N - NUMBER OF ROWS OF A AND COLUMNS OF R
           M - NUMBER OF COLUMNS OF A AND ROWS OF R
           MS  - ONE DIGIT NUMBER FOR STORAGE MODE OF MATRIX A (AND R)
                  0 - GENERAL
		  1 - SYMMETRIC
                  2 - DIAGONAL

        REMARKS
           MATRIX R CANNOT BE IN THE SAME LOCATION AS MATRIX A

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   MCPY

        METHOD
           TRANSPOSE N BY M MATRIX A TO FORM M BY N MATRIX R BY MOVING
           EACH ROW OF A INTO THE CORRESPONDING COLUMN OF R. IF MATRIX
	   A IS SYMMETRIC OR DIAGONAL, MATRIX R IS THE SAME AS A.

     .................................................................. */
void mtra(double a[], double r[], int n, int m, int ms)
{
int i, ij, ir, j;
/*        IF MS IS 1 OR 2, COPY A  */
if(ms==0)
  {
  /*        TRANSPOSE GENERAL MATRIX  */
  ir = 0;
  for( i = 1; i <= n; i++ ){
	  ij = i - n;
	  for( j = 1; j <= m; j++ ){
		  ij = ij + n;
		  ir = ir + 1;
		  r[ir-1] = a[ij-1];
		  }
	  }

  }
  else mcpy( a, r, n, n, ms );
return;
}

