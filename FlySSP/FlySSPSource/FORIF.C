/*------------------------------------------------------------------------
	  FORIF.C
	  ВЫЧИСЛЕНИЕ КОЭФФИЦИЕНТОВ ФУРЬЕ ПЕРИОДИЧЕСКОЙ ФУНКЦИИ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 26.04.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	ПОДПРОГРАММА (SUBROUTINE) forif

	НАЗНАЧЕНИЕ
	   Подпрограмма вычисляет коэффициенты A(0),A(k),B(k) (k=1,2,...,M)
	   ряда Фурье:
	       F(X)=A(0)+SUM(A(K)COS KX+B(K)SIN KX)
	   заданной аналитически на интервале [0,2*M_PI].
	PURPOSE
	   FOURIER ANALYSIS OF A GIVEN PERIODIC FUNCTION IN THE
	   RANGE 0-2PI
	   COMPUTES THE COEFFICIENTS OF THE DESIRED NUMBER OF TERMS
	   IN THE FOURIER SERIES F(X)=A(0)+SUM(A(K)COS KX+B(K)SIN KX)
	   WHERE K=1,2,...,M TO APPROXIMATE THE COMPUTED VALUES OF A
	   GIVEN FUNCTION SUBPROGRAM

	Обращение к подпрограмме (USAGE)
	   ier=forif(fun,n,m,a,b);

	DESCRIPTION OF PARAMETERS
	   fun-имя внешней подпрограммы, предназначенной для вычисления
	       значений функции в заданных точках
	       y=fun(x), x-аргумент
	       NAME OF FUNCTION SUBPROGRAM TO BE USED FOR COMPUTING
	       DATA POINTS
	   n  -число, определяющее подинтервал таким образом, что 2N+1
	       точка содержится на интервале [0,2*M_PI];
	       DEFINES THE INTERVAL SUCH THAT 2N+1 POINTS ARE TAKEN
	       OVER THE INTERVAL (0,2PI). THE SPACING IS THUS 2PI/2N+1
	   m  -максимальный порядок аппроксимирующих гармоник;
	       THE MAXIMUM ORDER OF THE HARMONICS TO BE FITTED
	   a  -массив длиной m+1 вычисленных коэффициентов Фурье при
	       косинусах a[0],a[1],...,a[m]
	       RESULTANT VECTOR OF FOURIER COSINE COEFFICIENTS OF
	       LENGTH M+1
	       A SUB 0, A SUB 1,..., A SUB M
	   b  -массив длиной m+1 вычисленных коэффициентов Фурье при
	       синусах b[0]=0,b[1],...,b[m]
	       RESULTANT VECTOR OF FOURIER SINE COEFFICIENTS OF
	       LENGTH M+1
	       B SUB 0, B SUB 1,..., B SUB M
	   Подпрограмма возвращает
	   ier-индикатор ошибки, принимающий при выполнении программы
	       следующие значения:
	       0 - ошибок нет, 
	       1 - (1-n) <= m,
	       2 - (2-m) < 0,
	       RESULTANT ERROR CODE WHERE
	       IER=0  NO ERROR
	       IER=1  N NOT GREATER OR EQUAL TO M
	       IER=2  M LESS THAN 0

	REMARKS
           M MUST BE GREATER THAN OR EQUAL TO ZERO
	   N MUST BE GREATER THAN OR EQUAL TO M
           THE FIRST ELEMENT IN VECTOR B IS ZERO IN ALL CASES

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           FUN-NAME OF USER FUNCTION SUBPROGRAM USED FOR COMPUTING
               DATA POINTS
           CALLING PROGRAM MUST HAVE FORTRAN EXTERNAL STATEMENT
           CONTAINING NAMES OF FUNCTION SUBPROGRAMS LISTED IN CALL TO
           FORIF

	METHOD
           USES RECURSIVE TECHNIQUE DESCRIBED IN A. RALSTON, H. WILF,
	   'MATHEMATICAL METHODS FOR DIGITAL COMPUTERS', JOHN WILEY
           AND SONS, NEW YORK, 1960, CHAPTER 24. THE METHOD OF
           INDEXING THROUGH THE PROCEDURE HAS BEEN MODIFIED TO
           SIMPLIFY THE COMPUTATION.

     ..................................................................
 */
int forif(double (*fun)(double ), int n, int m, double a[], double b[])
{
int    j, ier;
double ai, an, c, c1, coef, const_, funz, q, s, s1, u0, u1, u2, x;


/*        CHECK FOR PARAMETER ERRORS */
ier = 0;
// L_20:
if(m<0)
  goto L_30;
  else goto L_40;
L_30:
ier = 2;
return ier;
L_40:
if( (m-n)<=0)
  goto L_60;
  else goto L_50;
L_50:
ier = 1;
return ier;

/*        COMPUTE AND PRESET CONSTANTS  */
L_60:
an = n;
coef = 2.0/(2.0*an + 1.0);
const_ = M_PI*coef;//3.141593*
s1 = sin( const_ );
c1 = cos( const_ );
c = 1.0;
s = 0.0;
j = 1;
funz = (*fun)( 0.0 );
L_70:
u2 = 0.0;
u1 = 0.0;
ai = 2*n;

/*        FORM FOURIER COEFFICIENTS RECURSIVELY */
L_75:
x = ai*const_;
u0 = (*fun)( x ) + 2.0*c*u1 - u2;
u2 = u1;
u1 = u0;
ai = ai - 1.0;
if( ai <= 0.0)
  goto L_80;
  else goto L_75;
L_80:
a[j-1] = coef*(funz + c*u1 - u2);
b[j-1] = coef*s*u1;
if( (j - (m + 1)) < 0)
  goto L_90;
  else goto L_100;
L_90:
q = c1*c - s1*s;
s = c1*s + s1*c;
c = q;
j = j + 1;
goto L_70;
L_100:
a[0] = a[0]*0.5;
return ier;
}

