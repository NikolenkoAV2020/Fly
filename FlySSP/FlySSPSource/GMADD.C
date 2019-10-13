/*------------------------------------------------------------------------
  GMADD.C
    СЛОЖЕНИЕ ДВУХ МАТРИЦ ОБЩЕГО ВИДА
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE GMADD

        PURPOSE
           ADD TWO GENERAL MATRICES TO FORM RESULTANT GENERAL MATRIX

        USAGE
	   gmadd(A,B,R,N,M)

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
           ADDITION IS PERFORMED ELEMENT BY ELEMENT

     ..................................................................
 */
void  gmadd(double a[], double b[], double r[], int n, int m)
{
int i, nm;
/*        CALCULATE NUMBER OF ELEMENTS */
nm = n*m;
/*        ADD MATRICES     */
for( i = 0; i < nm; i++ )
	r[i] = a[i] + b[i];
return;
}

