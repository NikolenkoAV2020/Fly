/*------------------------------------------------------------------------
  LOC.C
    ВЫЧИСЛЕНИЕ ВЕКТОРНОГО ИНДЕКСА ЭЛЕМЕНТА МАТРИЦЫ, РАСПОЛОЖЕННОЙ В
    ПАМЯТИ В ВЕКТОРНОЙ ФОРМЕ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"
/*     ..................................................................

	SUBROUTINE LOC

	PURPOSE
	   COMPUTE A VECTOR SUBSCRIPT FOR AN ELEMENT IN A MATRIX OF
	   SPECIFIED STORAGE MODE

	USAGE
	   LOC (I,J,IR,N,M,MS)

        DESCRIPTION OF PARAMETERS
           I   - ROW NUMBER OF ELEMENT
           J   - COLUMN NUMBER  OF ELEMENT
           IR  - RESULTANT VECTOR SUBSCRIPT
           N   - NUMBER OF ROWS IN MATRIX
           M   - NUMBER OF COLUMNS IN MATRIX
           MS  - ONE DIGIT NUMBER FOR STORAGE MODE OF MATRIX
                  0 - GENERAL
                  1 - SYMMETRIC
                  2 - DIAGONAL

        REMARKS
           NONE

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           MS=0   SUBSCRIPT IS COMPUTED FOR A MATRIX WITH N*M ELEMENTS
                  IN STORAGE (GENERAL MATRIX)
           MS=1   SUBSCRIPT IS COMPUTED FOR A MATRIX WITH N*(N+1)/2 IN
                  STORAGE (UPPER TRIANGLE OF SYMMETRIC MATRIX). IF
		  ELEMENT IS IN LOWER TRIANGULAR PORTION, SUBSCRIPT IS
                  CORRESPONDING ELEMENT IN UPPER TRIANGLE.
           MS=2   SUBSCRIPT IS COMPUTED FOR A MATRIX WITH N ELEMENTS
                  IN STORAGE (DIAGONAL ELEMENTS OF DIAGONAL MATRIX).
                  IF ELEMENT IS NOT ON DIAGONAL (AND THEREFORE NOT IN
                  STORAGE), IR IS SET TO ZERO.

		 ..................................................................
*/
//#pragma argsused
void loc(int i, int j, int *ir, int n, int m, int ms)
{
int irx, jrx, ix, jx;
ix = n;
ix = i;
jx = j;
switch( ms ){
	case GENERAL:
		irx = m *ix + jx;
		break;
	case  SYMMETRIC:
// проверить, хотя взято из раб. программы
		ix=i;jx=j;
		if( i > j )
		   {ix=j;jx=i;}
		irx=(m-ix);         // строк внизу
		irx=(irx+1)*irx;
		irx>>=1;//  /2           в нижнем треугольнике
		jrx=(m+1)*m;
		jrx>>=1;//  /2           всего в треугольнике
		irx=jrx-irx;// осталось вверху
		irx+=(jx-ix);         // индекс в строке
		break;
	case  DIAGONAL:
		irx = -1;
		if((ix - jx)==0)
			{
			irx = ix;
			}
		break;
	}
(*ir) = irx;
return;
}
		/*
		if((ix - jx)<=0)
			irx = jx + (ix*ix - ix)/2;
			else
			irx = ix + (jx*jx - jx)/2;
	 */

/*
	 for ( ii=0;ii<n;ii++)
		 for (j1=0,jj=0;jj<m;j1+=(m-jj),jj++) // столбец
			 for (k=0;k<m;k++) // строки
				 {
				 if(k<=jj)
					 {
					 for(k1=0,l=0;l<k;k1+=(m-l),l++);
						 irx = k1+jj-k;
					 }
					 else irx = j1+k-jj;
				 if(ix==k && jx==jj)
					goto L_0;
				 }
		 L_0:
*/
