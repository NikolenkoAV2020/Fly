//---------------------------------------------------------------------------
//  CORRE.C
//    РАСЧЕТ СРЕДНИХ ЗНАЧЕНИЙ, СТАНДАРТНЫХ ОТКЛОНЕНИЙ, КОВАРИАЦИЙ И
//    КОЭФФИЦИЕНТОВ КОРРЕЛЯЦИИ
//  Автор	 : перевод на С выполнил Пасынков В.В.
//  Версия : 01.00 / 25.09.2000 /
//---------------------------------------------------------------------------
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	SUBROUTINE CORRE

	PURPOSE
	   COMPUTE MEANS, STANDARD DEVIATIONS, SUMS OF CROSS-PRODUCTS
	   OF DEVIATIONS, AND CORRELATION COEFFICIENTS.

	USAGE
	   CORRE (N,M,X,XBAR,STD,RX,R,B,D,T)

	DESCRIPTION OF PARAMETERS
	   N     - число наблюдений
		   NUMBER OF OBSERVATIONS. N MUST BE > OR = TO 2.
	   M     - число переменных
		   NUMBER OF VARIABLES. M MUST BE > OR = TO 1.
	   X[n][m]- матрица наблюдений [n][m]
		   IF IO=0, THE VALUE OF X IS 0.0.
		   IF IO=1, X IS THE INPUT MATRIX (N BY M) CONTAINING
			    DATA.
	   XBAR[m]-вектор средних
		   OUTPUT VECTOR OF LENGTH M CONTAINING MEANS.
	   STD[m] - вектор СКО
		   OUTPUT VECTOR OF LENGTH M CONTAINING STANDARD
		   DEVIATIONS.
	   RX[m][m]- матрица ковариаций
		   OUTPUT MATRIX (M X M) CONTAINING SUMS OF CROSS-
		   PRODUCTS OF DEVIATIONS FROM MEANS.
	   R[(m+1)*m/2] - матрица коэффициентов корреляции
		   c симметричным способом хранения
		   OUTPUT MATRIX (ONLY UPPER TRIANGULAR PORTION OF THE
		   SYMMETRIC MATRIX OF M BY M) CONTAINING CORRELATION
		   COEFFICIENTS.  (STORAGE MODE OF 1)
	   B[m]  - вектор диагональных элементов матрицы ковариаций
		   OUTPUT VECTOR OF LENGTH M CONTAINING THE DIAGONAL
		   OF THE MATRIX OF SUMS OF CROSS-PRODUCTS OF
		   DEVIATIONS FROM MEANS.
	   D[m]  - WORKING VECTOR OF LENGTH M.
	   T[m]  - WORKING VECTOR OF LENGTH M.

	REMARKS
	   NONE

	SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   DATA(I,D) - THIS SUBROUTINE MUST BE PROVIDED BY THE USER.

	METHOD
	   PRODUCT-MOMENT CORRELATION COEFFICIENTS ARE COMPUTED.
	...............................................................

     INITIALIZATION
 */

void corre(int n,int m,double x[],
	   double xbar[],double std[],double rx[],
	   double r[],double b[],double d[],double t[],
	   void (*data)(int, double *) )
{
//static int    i,j, jk, k, kk, l;
int    i,j, jk, k, kk, l;
double fkk, fn;
int    io;

io=0;
if(!data)
  {
  io=1;
  }
for( j = 0; j < m; j++ ){
	b[j] = 0.0;
	t[j] = 0.0;
	}
k = (m*m + m)/2; // k = ((m+1)*m)/2;
for( i = 0 ; i < k; i++ )
	r[i] = 0.0;

fn = (double)n;
//l = 0;

if(io==0) goto L_127;

/*     DATA ARE ALREADY IN CORE */
for(j = 0; j < m; j++ ){
	for(l=j, i = 0; i < n; i++ ){
		t[j] += x[l];  // сумма наблюдений j-й переменной
		l += m;
		}
	xbar[j] = t[j];
	t[j] = t[j]/fn;        // МО j-й переменной
	}

for( l=0, i = 0; i < n; i++ ){
	for( j = 0; j < m; j++ ){
		d[j]  = x[l] - t[j]; // разность l-го элемента МО j-й переменной
		b[j] += d[j];
		l = l + 1;     //
		}
	//расчет строки матрицы ковариаций
	for( j = 0; j < m; j++ ){
		for( k = 0 ; k <= j; k++ ){
			loc(j, k, &jk, m, m,SYMMETRIC);
			r[jk] += (d[j]*d[k]);
			}
		}
	}
goto L_205;

/*
       READ OBSERVATIONS AND CALCULATE TEMPORARY
       MEANS FROM THESE DATA IN T(J)
 */
L_127:
kk = n;
l=0;
for(i = 0; i < n; i++ ){
	data( i, d );
	for( j = 0; j < n; j++ ){
		t[j] += d[j];
		}
	}
fkk = (double)kk;
for( j = 0; j < m; j++ ){
	xbar[j] = t[j];
	t[j] /= fkk;
	}

/*
       CALCULATE SUMS OF CROSS-PRODUCTS OF DEVIATIONS
       FROM TEMPORARY MEANS FOR M OBSERVATIONS
 */
for( i = 0 ; i < kk; i++ ){
	data( i, rx );
	for(l = 0,jk = 0, j = 0 ; j < m; j++ ){
		d[j] = rx[l] - t[j];
		l = l + 1;
		}
	for( j = 0 ; j < m; j++ ){
		b[j] += d[j];
		for( k = 0 ; k <= j; k++ ){
			loc(j, k, &jk, m, m,SYMMETRIC);
			r[jk] += (d[j]*d[k]);
			}
		}
	}

/*     CALCULATE MEANS */
L_205:
for(j = 0; j < m; j++ ){
	xbar[j] /= fn;

	/*
	     ADJUST SUMS OF CROSS-PRODUCTS OF DEVIATIONS
	     FROM TEMPORARY MEANS
	 */
	for( k = 0 ; k <= j; k++ ){
		loc(j, k, &jk, m, m,SYMMETRIC);
		r[jk] -= (b[j]*b[k]/fn);
		}
	}

/*     CALCULATE CORRELATION COEFFICIENTS */
for(j = 0 ; j < m; j++ ){
	loc(j, j, &jk, m, m,SYMMETRIC);
	std[j] = sqrt( fabs( r[jk] ) );
	}
for( j = 0; j < m; j++ ){
	for( k = j; k < m; k++ ){
		loc(j, k, &jk, m, m,SYMMETRIC);
		l = m*j + k;
		rx[l] = r[jk];
		l = m*k + j;
		rx[l] = r[jk];
		if( (std[j]*std[k]) == 0) r[jk] = 0.0;
		  else r[jk] /= (std[j]*std[k]);
		}
	}
/*     CALCULATE STANDARD DEVIATIONS  */
fn = sqrt( fn - 1.0 );
for( j = 0; j < m; j++ ){
	std[j] /= fn;
	}

/*
     COPY THE DIAGONAL OF THE MATRIX OF SUMS OF CROSS-PRODUCTS OF
     DEVIATIONS FROM MEANS.
 */
l = -m;
for( i = 0; i < m; i++ ){
	l = l + m + 1;
	b[i] = rx[l-1];
	}
return;
}

