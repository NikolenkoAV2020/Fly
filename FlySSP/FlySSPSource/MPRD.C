/*------------------------------------------------------------------------
  MPRD.C
	  ПРОИЗВЕДЕНИЕ ДВУХ МАТРИЦ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE MPRD

        PURPOSE
           MULTIPLY TWO MATRICES TO FORM A RESULTANT MATRIX

        USAGE
	   mprd(A,B,R,N,M,MSA,MSB,L)

        DESCRIPTION OF PARAMETERS
           A - NAME OF FIRST INPUT MATRIX
           B - NAME OF SECOND INPUT MATRIX
           R - NAME OF OUTPUT MATRIX
           N - NUMBER OF ROWS IN A AND R
           M - NUMBER OF COLUMNS IN A AND ROWS IN B
           MSA - ONE DIGIT NUMBER FOR STORAGE MODE OF MATRIX A
                  0 - GENERAL
                  1 - SYMMETRIC
		  2 - DIAGONAL
           MSB - SAME AS MSA EXCEPT FOR MATRIX B
           L - NUMBER OF COLUMNS IN B AND R

        REMARKS
           MATRIX R CANNOT BE IN THE SAME LOCATION AS MATRICES A OR B
           NUMBER OF COLUMNS OF MATRIX A MUST BE EQUAL TO NUMBER OF ROW
           OF MATRIX B

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           LOC

        METHOD
           THE M BY L MATRIX B IS PREMULTIPLIED BY THE N BY M MATRIX A
           AND THE RESULT IS STORED IN THE N BY L MATRIX R. THIS IS A
           ROW INTO COLUMN PRODUCT.
           THE FOLLOWING TABLE SHOWS THE STORAGE MODE OF THE OUTPUT
           MATRIX FOR ALL COMBINATIONS OF INPUT MATRICES
                         A                B                R
                      GENERAL          GENERAL          GENERAL
                      GENERAL          SYMMETRIC        GENERAL
                      GENERAL          DIAGONAL         GENERAL
		      SYMMETRIC        GENERAL          GENERAL
                      SYMMETRIC        SYMMETRIC        GENERAL
                      SYMMETRIC        DIAGONAL         GENERAL
                      DIAGONAL         GENERAL          GENERAL
                      DIAGONAL         SYMMETRIC        GENERAL
                      DIAGONAL         DIAGONAL         DIAGONAL

     ..................................................................
 */
void mprd(double a[], double b[], double r[], int n, int m, int msa, int msb, int l)
{
int i, ia, ib, ir, j, k, ms;

/*        SPECIAL CASE FOR DIAGONAL BY DIAGONAL */
ms = msa*10 + msb;
if(ms==22)
  {
	  for( i = 0; i < n; i++ )
		  r[i] = a[i]*b[i];
	  return;
  }
/*        ALL OTHER CASES  */
ir = 0;
for( j = 0; j < n; j++ ){
	for( k = 0; k < l; k++ ){
		for( r[ir] = 0,i = 0; i < m; i++ ){
		  if(ms!=0)
		    {
		    loc( j, i, &ia, n, m, msa );
		    if(ia<0)
		      continue;
		    loc( i, k, &ib, m, l, msb );
		    if(ib<0)
		      continue;
		    }
		    else
		    {
		    ia = m*j + i;
		    ib = l*i + k;
		    }
		  r[ir] += (a[ia]*b[ib]);
		  }
		ir += 1;
		}
	}
return;
}

