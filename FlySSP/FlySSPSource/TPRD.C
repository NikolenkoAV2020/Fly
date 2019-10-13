/*------------------------------------------------------------------------
  TPRD.C
	  “†…… ’€‘‚€‰ €’–› € „“ƒ“ €’–“
  €Άβ®ΰ   : ¥ΰ¥Ά®¤ ―ΰ®£ΰ ¬¬λ ― ª¥β  SSP ­  ‘ Άλ―®«­¨«  αλ­ª®Ά ‚.‚.
  ‚¥ΰα¨ο  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"
/*     ..................................................................

	SUBROUTINE TPRD

	PURPOSE
	   “†…… ’€‘‚€‰ €’–› € € „“ƒ“
	   €’–“ ‚
	   (TRANSPOSE A MATRIX AND POSTMULTIPLY BY ANOTHER TO FORM
	   A RESULTANT MATRIX)

	€™……  „ƒ€… (USAGE)
	   tprd(A,B,R,N,M,MSA,MSB,L)

	€€…’› (DESCRIPTION OF PARAMETERS)
	   A - €‘‘‚ ‹……’‚ ‘•„‰ €’–›
	       (NAME OF FIRST INPUT MATRIX)
	   B - €‘‘‚ ‹……’‚ ‚’‰ ‘•„‰ €’–›
	       (NAME OF SECOND INPUT MATRIX)
	   R - €‘‘‚ ‹……’‚ …‡“‹’“™…‰ €’–›
	       (NAME OF OUTPUT MATRIX)
	   N - —‘‹ ‘’ ‚ €’–… €  ‚(NUMBER OF ROWS IN A AND B)
	   M - —‘‹ ‘’‹–‚ €’–› €  R
	       (NUMBER OF COLUMNS IN A AND ROWS IN R)
	   MSA - ………€, ‡€—…… ’‰ “€‡›‚€…’ ‘‘
		 •€… €’–› €)
		 (ONE DIGIT NUMBER FOR STORAGE MODE OF MATRIX A)
		  0 - ™‰ ‘‘ •€…(GENERAL)
		  1 - ‘…’—›‰(SYMMETRIC)
		  2 - „€ƒ€‹›‰(DIAGONAL)
	   MSB - SAME AS MSA EXCEPT FOR MATRIX B
	   L - —‘‹ ‘’‹–‚ ‚ €’–€• ‚  R
	       (NUMBER OF COLUMNS IN B AND R)

	REMARKS
	   MATRIX R CANNOT BE IN THE SAME LOCATION AS MATRICES A OR B

	SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   LOC

	METHOD
	   MATRIX TRANSPOSE OF A IS NOT ACTUALLY CALCULATED. INSTEAD,
	   ELEMENTS IN MATRIX A ARE TAKEN COLUMNWISE RATHER THAN
	   ROWWISE FOR MULTIPLICATION BY MATRIX B.
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
void  tprd(double a[], double b[], double r[], int n, int m, int msa, int msb, int l)
{
int i, ia, ib, ir, j, k, ms;
/*        SPECIAL CASE FOR DIAGONAL BY DIAGONAL */
ms = (msa)*10 + (msb);
if(ms==22)
  {
  for( i = 0; i < n; i++ )
	r[i] = a[i]*b[i];
  return;
  }
/*        MULTIPLY TRANSPOSE OF A BY B */
ir = 0;
for( j = 0; j < n; j++ ){
	for( k = 0; k < l; k++ ){
		for(r[ir] = 0.0 ,i = 0; i < m; i++ ){
		  if(ms!=0)
		    {
		    loc( i, j, &ia, n, m, msa );
		    if(ia<0)
		      continue;
		    loc( i, k, &ib, m, l, msb );
		    if(ib<0)
		      continue;
		    }
		    else
		    {
		    ia = n*i + j;
		    ib = l*i + k;
		    }
		  r[ir] += (a[ia]*b[ib]);
		  }
		ir = ir + 1;
		}
	}
return;
}

