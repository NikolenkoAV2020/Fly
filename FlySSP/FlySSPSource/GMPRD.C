/*------------------------------------------------------------------------
  GMPRD.C
    ПРОИЗВЕДЕНИЕ ДВУХ МАТРИЦ ОБЩЕГО ВИДА
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE GMPRD

	PURPOSE
           MULTIPLY TWO GENERAL MATRICES TO FORM A RESULTANT GENERAL
           MATRIX

        USAGE
	   gmprd(A,B,R,N,M,L)

        DESCRIPTION OF PARAMETERS
	   A - NAME OF FIRST INPUT MATRIX
           B - NAME OF SECOND INPUT MATRIX
           R - NAME OF OUTPUT MATRIX
           N - NUMBER OF ROWS IN A
           M - NUMBER OF COLUMNS IN A AND ROWS IN B
	   L - NUMBER OF COLUMNS IN B

        REMARKS
           ALL MATRICES MUST BE STORED AS GENERAL MATRICES
           MATRIX R CANNOT BE IN THE SAME LOCATION AS MATRIX A
           MATRIX R CANNOT BE IN THE SAME LOCATION AS MATRIX B
           NUMBER OF COLUMNS OF MATRIX A MUST BE EQUAL TO NUMBER OF ROW
           OF MATRIX B

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   NONE

	METHOD
	   THE M BY L MATRIX B IS PREMULTIPLIED BY THE N BY M MATRIX A
           AND THE RESULT IS STORED IN THE N BY L MATRIX R.

     ..................................................................
 */
void  gmprd(double a[], double b[], double r[], int n, int m, int l)
{
int i,j,k;
/*   Произведение матриц общего вида R=A*B,
   где - a[n*m] матрица общего вида ,//[l*m]
       - b[m*l] матрица общего вида ,//[m*n]
       - r[n*l] матрица общего вида ,//[l*n]
*/
if(!a || !b || !r)
  return ;
if(l<=0 || m<=0 || n<=0 )
  return ;
for ( i=0;i<n;i++)
  for ( k=0;k<l;k++)
     for (*(r+(i*l)+k)=0.0, j=0;j<m;j++)
       *(r+(i*l)+k) +=( *(a+(i*m)+j) * *(b+(j*l)+k) );
return;
}
/*
int i, ib, ik, ir, j, ji, k;
ir = 0;
ik = -m;
for( k = 1; k <= l; k++ ){
	ik = ik + m;
	for( j = 1; j <= n; j++ ){
		ir = ir + 1;
		ji = j - n;
		ib = ik;
		r[ir-1] = 0;
		for( i = 1; i <= m; i++ ){
			ji = ji + n;
			ib = ib + 1;
			r[ir-1] = r[ir-1] + a[ji-1]*b[ib-1];
			}
		}
	}
*/
