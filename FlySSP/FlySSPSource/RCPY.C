/*------------------------------------------------------------------------
  RCPY.C
     ПРЕДСТАВЛЕНИЕ СТРОКИ МАТРИЦЫ В ВИДЕ ВИКТОРА
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "ssp.h"
/*     ..................................................................

        SUBROUTINE RCPY

        PURPOSE
           COPY SPECIFIED ROW OF A MATRIX INTO A VECTOR

        USAGE
           CALL RCPY (A,L,R,N,M,MS)

        DESCRIPTION OF PARAMETERS
           A - NAME OF INPUT MATRIX
           L - ROW OF A TO BE MOVED TO R
           R - NAME OF OUTPUT VECTOR OF LENGTH M
	   N - NUMBER OR ROWS IN A
           M - NUMBER OF COLUMNS IN A
           MS  - ONE DIGIT NUMBER FOR STORAGE MODE OF MATRIX A
                  0 - GENERAL
                  1 - SYMMETRIC
                  2 - DIAGONAL

        REMARKS
           NONE

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   LOC

	METHOD
	   ELEMENTS OF ROW L ARE MOVED TO CORRESPONDING POSITIONS
	   OF VECTOR R

     ..................................................................
 */
void rcpy(double a[], int l, double r[], int n, int m, int ms)
{
int j, lj;
for( j = 0; j < m; j++ ) // цикл по числу столбцов
	{
	/*        LOCATE ELEMENT FOR ANY MATRIX STORAGE MODE  */
	loc( l, j, &lj, n, m, ms );
	/*        MOVE ELEMENT TO R 	 */
	r[j] = a[lj];
	}
return;
}

