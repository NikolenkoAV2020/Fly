/*---------------------------------------------------------------------------
  MINV.C
	 ОБРАЩЕНИЕ МАТРИЦЫ
  Автор	  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
---------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	SMINV

        PURPOSE
           INVERT A MATRIX

	USAGE
	   SMINV(A,N,D,L,M)

        DESCRIPTION OF PARAMETERS
           A - INPUT MATRIX, DESTROYED IN COMPUTATION AND REPLACED BY
               RESULTANT INVERSE.
	   N - ORDER OF MATRIX A
	   D - RESULTANT DETERMINANT
	   L - WORK VECTOR OF LENGTH N
           M - WORK VECTOR OF LENGTH N

        REMARKS
	   MATRIX A MUST BE A GENERAL MATRIX

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
	   THE STANDARD GAUSS-JORDAN METHOD IS USED. THE DETERMINANT
	   IS ALSO CALCULATED. A DETERMINANT OF ZERO INDICATES THAT
	   THE MATRIX IS SINGULAR.

//Тест
int main(void)
{
double   a[9],d;
int    n=3,l[3],m[3];

a[0]=1.0;a[1]=2.0;a[2]=3.0;
a[3]=4.0;a[4]=5.0;a[5]=6.0;
a[6]=7.0;a[7]=8.0;a[8]=1.0;
sminv(a,n,&d,l,m);

//Результат
// det=24;
// a[0]=-1.7916(6);a[1]= 0.916(6);a[2]=-0.125;
// a[3]= 1.583(3) ;a[4]=-0.83(3) ;a[5]= 0.25;
// a[6]=-0.125    ;a[7]= 0.25    ;a[8]=-0.125;
return 0;
}

     .................................................................. */
void sminv(double a[],int n,double *d,int l[],int m[])
{
int i, ij, ik, iz, j, ji, jk, jp, jq, jr, k, ki, kj, kk, nk;
double biga, hold;


/*        ...............................................................

	IF A DOUBLE PRECISION VERSION OF THIS ROUTINE IS DESIRED, THE
	C IN COLUMN 1 SHOULD BE REMOVED FROM THE DOUBLE PRECISION
	STATEMENT WHICH FOLLOWS.

     DOUBLE PRECISION A,D,BIGA,HOLD

        THE C MUST ALSO BE REMOVED FROM DOUBLE PRECISION STATEMENTS
        APPEARING IN OTHER ROUTINES USED IN CONJUNCTION WITH THIS
        ROUTINE.

	THE DOUBLE PRECISION VERSION OF THIS SUBROUTINE MUST ALSO
	CONTAIN DOUBLE PRECISION FORTRAN FUNCTIONS.  ABS IN STATEMENT
	10 MUST BE CHANGED TO DABS.

	............................................................... */


/*	SEARCH FOR LARGEST ELEMENT                                        */

(*d) = 1.0;
nk = -n;
for( k = 1; k <= n; k++ ){
	nk = nk + n;	l[k-1] = k;	m[k-1] = k;	kk = nk + k;
	biga = a[kk-1];
	for( j = k; j <= n; j++ ){
		iz = n*(j - 1);
		for( i = k; i <= n; i++ ){
			ij = iz + i;
			if( (fabs( biga ) - fabs( a[ij-1] )) < 0.0)
			{
			biga = a[ij-1];	l[k-1] = i; m[k-1] = j;
			}
			}
		}

	/*        INTERCHANGE ROWS */
	j = l[k-1];
	if( (j - k) > 0)
	  {
	  ki = k - n;
	  for( i = 1; i <= n; i++ ){
		  ki = ki + n;	hold = -a[ki-1];
		  ji = ki - k + j; a[ki-1] = a[ji-1];
		  a[ji-1] = hold;
		  }
	  }

	/*        INTERCHANGE COLUMNS	 */
	i = m[k-1];
	if( (i - k) > 0)
	  {
	  jp = n*(i - 1);
	  for( j = 1; j <= n; j++ ){
		  jk = nk + j;	   ji = jp + j;
		  hold = -a[jk-1];
		  a[jk-1] = a[ji-1]; a[ji-1] = hold;
		  }
	  }

	/*        DIVIDE COLUMN BY MINUS PIVOT (VALUE OF PIVOT ELEMENT IS
		CONTAINED IN BIGA)   	 */
	if(biga==0)
	  {
	  (*d) = 0.0;
	  return;
	  }
	for( i = 1; i <= n; i++ ){
		if( (i - k) == 0 ) continue;
		ik = nk + i;
		a[ik-1] = a[ik-1]/(-biga);
		}

	/*        REDUCE MATRIX      	 */
	for( i = 1; i <= n; i++ ){
		ik = nk + i;	hold = a[ik-1];	ij = i - n;
		for( j = 1; j <= n; j++ ){
			ij = ij + n;
			if( (i - k) == 0 ) continue;
			if( (j - k) == 0 ) continue;
			kj = ij - i + k;
			a[ij-1] = hold*a[kj-1] + a[ij-1];
			}
		}

	/*        DIVIDE ROW BY PIVOT	 */
	kj = k - n;
	for( j = 1; j <= n; j++ ){
		kj = kj + n;
		if( (j - k) == 0 ) continue;
		a[kj-1] = a[kj-1]/biga;
		}

	/*        PRODUCT OF PIVOTS  	 */
	(*d) = (*d)*biga;

	/*        REPLACE PIVOT BY RECIPROCAL	 */
	a[kk-1] = 1.0/biga;
	}

/*        FINAL ROW AND COLUMN INTERCHANGE    */
k = n;
while( (--k) > 0)
  {
  i = l[k-1];
  if( (i - k) >0 )
     {
     jq = n*(k - 1); jr = n*(i - 1);
     for( j = 1; j <=n; j++ ){
	     jk = jq + j;	hold = a[jk-1];
	     ji = jr + j;
	     a[jk-1] = -a[ji-1]; a[ji-1] = hold;
	     }
     }
  j = m[k-1];
  if( (j - k) <= 0) continue;
  ki = k - n;
  for( i = 1; i <= n; i++ ){
	  ki = ki + n;	hold = a[ki-1];
	  ji = ki - k + j;
	  a[ki-1] = -a[ji-1]; a[ji-1] = hold;
	  }
  } /*while*/
// L_150:
return;
}

