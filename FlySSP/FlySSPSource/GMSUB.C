/*------------------------------------------------------------------------
  GMSUB.C
    ВЫЧИТАНИЕ ДВУХ МАТРИЦ ОБЩЕГО ВИДА
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE GMSUB

        PURPOSE
           SUBTRACT ONE GENERAL MATRIX FROM ANOTHER TO FORM RESULTANT
           MATRIX

        USAGE
	   gmsub(A,B,R,N,M)

        DESCRIPTION OF PARAMETERS
	   A - NAME OF FIRST INPUT MATRIX
           B - NAME OF SECOND INPUT MATRIX
           R - NAME OF OUTPUT MATRIX
           N - NUMBER OF ROWS IN A,B,R
           M - NUMBER OF COLUMNS IN A,B,R

        REMARKS
           ALL MATRICES MUST BE STORED AS GENERAL MATRICES

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
	   MATRIX B ELEMENTS ARE SUBTRACTED FROM CORRESPONDING MATRIX A
	   ELEMENTS

     ..................................................................
 */
void  gmsub(double a[], double b[], double r[], int n, int m)
{
int i, nm;
/*        CALCULATE NUMBER OF ELEMENTS */
nm =n*m;
/*        SUBTRACT MATRICES */
for( i = 0; i < nm; i++ ){
	r[i] = a[i] - b[i];
	}
return;
}

