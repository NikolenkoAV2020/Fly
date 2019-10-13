/*------------------------------------------------------------------------
  MSUB.C
	  ВЫЧИТАНИЕ ДВУХ МАТРИЦ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

				SUBROUTINE MSUB

        PURPOSE
           ADD TWO MATRICES ELEMENT BY ELEMENT TO FORM RESULTANT
	   MATRIX

        USAGE
		 msub(A,B,R,N,M,MSA,MSB)

				DESCRIPTION OF PARAMETERS
	   A - NAME OF INPUT MATRIX
           B - NAME OF INPUT MATRIX
					 R - NAME OF OUTPUT MATRIX
           N - NUMBER OF ROWS IN A,B,R
					 M - NUMBER OF COLUMNS IN A,B,R
           MSA - ONE DIGIT NUMBER FOR STORAGE MODE OF MATRIX A
                  0 - GENERAL
									1 - SYMMETRIC
		  2 - DIAGONAL
           MSB - SAME AS MSA EXCEPT FOR MATRIX B

        REMARKS
					 NONE

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
					 LOC

				METHOD
           STORAGE MODE OF OUTPUT MATRIX IS FIRST DETERMINED. ADDITION
		 OF CORRESPONDING ELEMENTS IS THEN PERFORMED.
	   THE FOLLOWING TABLE SHOWS THE STORAGE MODE OF THE OUTPUT
           MATRIX FOR ALL COMBINATIONS OF INPUT MATRICES
                         A                B                 R
                      GENERAL          GENERAL          GENERAL
                      GENERAL          SYMMETRIC        GENERAL
											GENERAL          DIAGONAL         GENERAL
		      SYMMETRIC        GENERAL          GENERAL
                      SYMMETRIC        SYMMETRIC        SYMMETRIC
											SYMMETRIC        DIAGONAL         SYMMETRIC
                      DIAGONAL         GENERAL          GENERAL
					DIAGONAL         SYMMETRIC        SYMMETRIC
					DIAGONAL         DIAGONAL         DIAGONAL

     .................................................................. */
void  msub(double a[], double b[], double r[], int n, int m, int msa, int msb)
{
int    i, ija, ijb, ijr, j, msr, mtest, nm;
double ael, bel;
/*        DETERMINE STORAGE MODE OF OUTPUT MATRIX */
if( (msa - msb) ==0 )
	{
	loc( n-1, m-1, &nm, n, m, msa );
	/*        ADD MATRICES FOR OTHER CASES */
	for( i = 0; i <=nm; i++ )
		r[i] = a[i] - b[i];
	return;
	}

mtest = msa*msb;
msr = 0;
if(mtest>0)
	{
	msr = 1;
	}
	else
	{
	if((mtest - 2)>0)
		msr = 2;
	}

/*        LOCATE ELEMENTS AND PERFORM ADDITION */
for( i = 0; i < n; i++ ){
	for( j = 0 ; j < m; j++ ){
		loc( i, j, &ijr, n, m, msr );
		if(ijr<0)
			continue;
		loc( i, j, &ija, n, m, msa );
		ael = 0.0;
		if(ija!=-1)
			ael = a[ija];
		loc( i, j, &ijb, n, m, msb );
		bel = 0.0;
		if(ijb!=-1)
			bel = b[ijb];
		r[ijr] = ael - bel;
		}
	}
return;

}

