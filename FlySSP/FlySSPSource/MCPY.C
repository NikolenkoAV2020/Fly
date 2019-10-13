/*------------------------------------------------------------------------
  MCPY.C
	  КОПИРОВАНИЕ МАТРИЦЫ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"
/*        ...............................................................

	SUBROUTINE MCPY

        PURPOSE
           COPY ENTIRE MATRIX

        USAGE
	   mcpy(A,R,N,M,MS)

        DESCRIPTION OF PARAMETERS
           A - NAME OF INPUT MATRIX
	   R - NAME OF OUTPUT MATRIX
           N - NUMBER OF ROWS IN A OR R
           M - NUMBER OF COLUMNS IN A OR R
           MS  - ONE DIGIT NUMBER FOR STORAGE MODE OF MATRIX A (AND R)
                  0 - GENERAL
                  1 - SYMMETRIC
                  2 - DIAGONAL

        REMARKS
	   NONE

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           LOC

	METHOD
	   EACH ELEMENT OF MATRIX A IS MOVED TO THE CORRESPONDING
	   ELEMENT OF MATRIX R

		 ..................................................................
 */
void  mcpy(double a[], double r[], int n, int m, int ms)
{
int i, it;
/*        COMPUTE VECTOR LENGTH, IT */
loc( n-1, m-1, &it, n, m, ms );
/*        COPY MATRIX */
for( i = 0; i <=it; i++ )
	r[i] = a[i];
return;
}

