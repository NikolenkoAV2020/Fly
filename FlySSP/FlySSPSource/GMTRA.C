/*------------------------------------------------------------------------
  GMTRA.C
    ТРАНСПОНИРОВАНИЕ МАТРИЦЫ ОБЩЕГО ВИДА
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE GMTRA

	PURPOSE
           TRANSPOSE A GENERAL MATRIX

	USAGE
	   gmtra(A,R,N,M)

	DESCRIPTION OF PARAMETERS
	   A - NAME OF MATRIX TO BE TRANSPOSED
	   R - NAME OF RESULTANT MATRIX
	   N - NUMBER OF ROWS OF A AND COLUMNS OF R
	   M - NUMBER OF COLUMNS OF A AND ROWS OF R

	REMARKS
	   MATRIX R CANNOT BE IN THE SAME LOCATION AS MATRIX A
	   MATRICES A AND R MUST BE STORED AS GENERAL MATRICES

	SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   NONE

	METHOD
	   TRANSPOSE N BY M MATRIX A TO FORM M BY N MATRIX R

     ..................................................................
 */
void  gmtra(double a[], double r[], int n, int m)
{
int i,j;
if(!a || !r)
  return ;
if(m<=0 || n<=0 )
  return ;
for (i=0;i<n;i++)
	for (j=0;j<m;j++)
		r[j*n+i]=a[j+i*m];
return;
}

/*
int i, ij, ir, j;


ir = 0;
for( i = 1; i <= n; i++ ){
	ij = i - n;
	for( j = 1; j <= m; j++ ){
		ij = ij + n;
		ir = ir + 1;
		r[ir-1] = a[ij-1];
		}
	}
*/
