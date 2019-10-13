/*------------------------------------------------------------------------
  _SING.C
	  РЕШЕНИЕ СЛАУ МЕТОДОМ СИНГУЛЯРНОГО РАЗЛОЖЕНИЯ
  Автор   : Перевод программы на С выполнил Пасынков В.В.
  Версия  : 01.00 / 24.05.1993 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "ssp.h"
//Проверено решение СНУ и обращение матрицы для этого случая
//
int svd(int m, int n, double a[], double w[], double v[], double work[]);

int sing(int m,int n,double *tau,
	 int *ncomp,int nsyst,int masht,double * avt,
	 double a[],double b[],double w[],double *v,double x[],double ainv[],
	 double cond[])
{
//#define A(I_,J_)	(a+(I_)*(m)+(J_))
//#define V(I_,J_)	(v+(I_)*(n)+(J_))
#define A(I_,J_)	(a+(J_)*(m)+(I_))
#define V(I_,J_)	(v+(J_)*(n)+(I_))
int   ierr, i, i1, j, k, k1, k2,l;
double  c, f, g, h, s, taumin;

taumin = 0.5e-20;
/*                                    т
  Сингулярное разложение матрицы U*W*V  */
ierr=svd(m,n,a,w,v, x);
if(ierr!=0)
  return ierr;

g = c = w[0];

l = 1;
//поиск минимального и максимального сингулярного числа
for( i = 1; i < n; i++ ){
  if( g < w[i] )
    g = w[i]; /* максимум */
  if( w[i] < c ){
    c = w[i]; /* минимум */
    l = i + 1;
    }
  }
if( c > taumin )
   cond[0] = g/c;
   else cond[0] = 1.e0/taumin;
taumin = taumin*g;

for( i = 1; i <= (n - 1); i++ ){
	k1 = n - i + 1;
	if( k1 == l )
		goto L_1090;
	h = w[l-1];
	w[l-1] = w[k1-1];
	w[k1-1] = h;
	for( j = 1; j <= m; j++ ){
		h =  *A(l-1,j-1);
		 *A(l-1,j-1) =  *A(k1-1,j-1);
		 *A(k1-1,j-1) = h;
		}
	for( j = 1; j <= n; j++ ){
		h =  *V(l-1,j-1);
		*V(l-1,j-1) =  *V(k1-1,j-1);
		*V(k1-1,j-1) = h;
		}
L_1090:
	h = g;
	for( i1 = 1; i1 <= (k1 - 1); i1++ ){
		k2 = k1 - i1;
		if( w[k2-1] >= h )
			goto L_1100;
		h = w[k2-1];
		l = k2;
L_1100:
		;
		}
	}
if( (*avt) == 0.e0 )
	goto L_1300;
c = (*avt);
l = 1;
for( i = 1; i <= n; i++ ){
	for( s = 0.0e0,j = 1; j <= m; j++ ){
		s = s +  *A(i-1,j-1)*b[j-1];
		}
	s = s*s;
	if( m == n )
		s = s/w[i-1];
	x[i-1] = s;
	if( s >= c )
		goto L_1114;
	c = s;
	l = i;
L_1114:
	;
	}
for( i = 1; i <= (n - 1); i++ ){
	k1 = n - i + 1;
	if( k1 == l )
		goto L_1117;
	h = w[l-1];
	w[l-1] = w[k1-1];
	w[k1-1] = h;
	h = x[l-1];
	x[l-1] = x[k1-1];
	x[k1-1] = h;
	for( j = 1; j <= m; j++ ){
		h =  *A(l-1,j-1);
		 *A(l-1,j-1) =  *A(k1-1,j-1);
		 *A(k1-1,j-1) = h;
		}
	for( j = 1; j <= n; j++ ){
		h =  *V(l-1,j-1);
		 *V(l-1,j-1) =  *V(k1-1,j-1);
		 *V(k1-1,j-1) = h;
		}
L_1117:
	h = (*avt);
	for( i1 = 1; i1 <= (k1 - 1); i1++ ){
		k2 = k1 - i1;
		if( x[k2-1] >= h )
			goto L_1118;
		h = x[k2-1];
		l = k2;
L_1118:
		;
		}
	}
//L_1122:
for( k = 1 ; k <= n; k++ ){
	k1 = (m - k)*(n - k + 1);
	i1 = m + n - 2*k;
	(*avt) = fabs( (*avt) - x[k-1] );
	x[k-1] = x[k-1]/(*avt)*k1/i1;
	}
for( i = 1; i <= n; i++ ){
	if( x[i-1] > 0.9e0 )
		goto L_1150;
	(*ncomp) = i - 1;
	if( (*ncomp) != 0 )
		goto L_1330;
	for( j = 0; j < n; j++ )
		x[j] = 0.e0;
	goto L_5000;
L_1150:	;
	}
(*ncomp) = n;
goto L_1305;
L_1300:
if( (*ncomp) != 0 )
	goto L_1330;
//L_1302:
if( (*tau) != 0.e0 )
	goto L_1320;
L_1305:
h = 1.e0;
L_1310:
h = 0.5e0*h;
f = h + 1.e0;
if( f > 1.e0 )
	goto L_1310;
(*tau) = h*n;
L_1320:
(*tau) = g*(*tau);
L_1330:
if( nsyst == 0.e0 )
	goto L_1410;
for( i = 0; i < n; i++ )
	x[i] = 0.e0;

//Решение СЛАУ
c = g;
for( i = 0; i < n; i++ ){
	if( (*ncomp) == 0 )
		goto L_1350;
	if( i == (*ncomp) + 1 )
		break;
	goto L_1360;
L_1350:
	if( w[i] < (*tau) )
		break;
L_1360:
	if( w[i] >= taumin )
	  {
	  c = w[i];
	  s = 0.e0;
	  for( j = 0; j < m; j++ )
		  s = s +  *A(i,j)*b[j];
	  s = s/w[i];
	  for( j = 0; j < n; j++ )
		  x[j] = x[j] + s* *V(i,j);
	  }
	}
//L_1400:
cond[1] = g/c;
L_1410:
if( masht == 0 )
	goto L_1450;
L_1450:
if( ainv!=NULL)
//Вычисление обратной матрицы
for( i = 0; i < n; i++ ){
	for( j = 0; j < n; j++ ){
		for(s = 0.0e0, k = 0; k < n; k++ ){
			if( (*ncomp) == 0 )
				goto L_1470;
			if( k == (*ncomp) + 1 )
				goto L_1500;
			goto L_1480;
L_1470:
			if( w[k] < (*tau) )
				goto L_1500;
L_1480:
			if( w[k] >= taumin )
			  {
  //	 a[i*n+j]+=(u[j*n+k]*v[i*n+k]/w[k]);
  //			s = s +  *V(k-1,i-1)/w[k-1]* *V(k-1,j-1);
			  s = s +  *A(k,i)/w[k]* *V(k,j);
			  if( m != n )
				  s = s/w[k];
			  }
			}
L_1500:
//		 *A(j-1,i-1) = s;
		 ainv[j*n+i] = s;
		}
	}
L_5000:
return ierr;
#undef	V
#undef	A
}

