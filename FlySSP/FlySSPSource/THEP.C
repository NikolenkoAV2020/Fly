/*------------------------------------------------------------------------
  THEP.C
      ПРЕОБРАЗОВАНИЕ ПОЛИНОМА ЭРМИТА В АЛГЕБРАИЧЕСКИЙ ПОЛИНОМ
  Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия : 01.00 / 01.06.1994 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

	SUBROUTINE thep

	PURPOSE
	   A SERIES EXPANSION IN HERMITE POLYNOMIALS WITH INDEPENDENT
	   VARIABLE X IS TRANSFORMED TO A POLYNOMIAL WITH INDEPENDENT
	   VARIABLE Z, WHERE X=A*Z+B

	USAGE
	   thep(A,B,POL,N,C,WORK)

	DESCRIPTION OF PARAMETERS
	   A     - FACTOR OF LINEAR TERM IN GIVEN LINEAR TRANSFORMATION
		   DOUBLE PRECISION VARIABLE
	   B     - CONSTANT TERM IN GIVEN LINEAR TRANSFORMATION
		   DOUBLE PRECISION VARIABLE
	   POL   - COEFFICIENT VECTOR OF POLYNOMIAL (RESULTANT VALUE)
		   COEFFICIENTS ARE ORDERED FROM LOW TO HIGH
		   DOUBLE PRECISION VECTOR
	   N     - DIMENSION OF COEFFICIENT VECTOR POL AND C
	   C     - COEFFICIENT VECTOR OF GIVEN EXPANSION
		   COEFFICIENTS ARE ORDERED FROM LOW TO HIGH
		   POL AND C MAY BE IDENTICALLY LOCATED
		   DOUBLE PRECISION VECTOR
	   WORK  - WORKING STORAGE OF DIMENSION 2*N
		   DOUBLE PRECISION ARRAY

	REMARKS
	   COEFFICIENT VECTOR C REMAINS UNCHANGED IF NOT COINCIDING
	   WITH COEFFICIENT VECTOR POL.
	   OPERATION IS BYPASSED IN CASE N LESS THAN 1.
	   THE LINEAR TRANSFORMATION X=A*Z+B OR Z=(1/A)(X-B) TRANSFORMS
	   THE RANGE (-C,C) IN X TO THE RANGE (ZL,ZR) IN Z WHERE
	   ZL=-(C+B)/A AND ZR=(C-B)/A.
	   FOR GIVEN ZL, ZR AND C WE HAVE A=2C/(ZR-ZL) AND
	   B=-C(ZR+ZL)/(ZR-ZL)

	SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   NONE

	METHOD
	   THE TRANSFORMATION IS BASED ON THE RECURRENCE EQUATION
	   FOR HERMITE POLYNOMIALS H(N,X)
	   H(N+1,X)=2*(X*H(N,X)-N*H(N-1,X)),
	   WHERE THE FIRST TERM IN BRACKETS IS THE INDEX
	   THE SECOND IS THE ARGUMENT.
	   STARTING VALUES ARE H(0,X)=1,H(1,X)=2*X.
	   THE TRANSFORMATION IS IMPLICITLY DEFINED BY MEANS OF
	   X=A*Z+B TOGETHER WITH
	   SUM(POL(I)*Z**(I-1), SUMMED OVER I FROM 1 TO N)
	   =SUM(C(I)*H(I-1,X), SUMMED OVER I FROM 1 TO N).
//Тест
#include <alloc.h>
#include <ssp.h>

double fuser(double x)
{
return (86.5*x*x*x + 6.25*x*x-3.33*x-0.12);
}

#define N 30
#define M 6

short main(void)
{
short     n=N, m=M, mmm, ier, iter;
short    *ihe;
double   *top,*piv,*t,*work;
double    pol[M]={0,0,0,0,0,0};
double    xd,x0,x,d,sq;
short     i;

  mmm=(m+1)*(m+2)/2;
  // Размещение необходимых массивов
  ihe  = (short *) calloc(3*m+4*n+6,sizeof(short));
  if(ihe==NULL)
    return -2;
  piv = (double*) calloc(3*m+6,sizeof(double));
  if(piv==NULL)
    return -2;
  t = (double*) calloc( (m+2)*(m+2),sizeof(double));
  if(t==NULL)
    return -2;
  work  = (double*) calloc(mmm,sizeof(double));
  if(work==NULL)
    return -2;
  top  = (double *) calloc(n*3,sizeof(double));
  if(top==NULL)
    return -2;
  // Формирование тестового массива
  for(i=0;i<n;i++)
    {
    x=top[n+i]=0.1*i;//Аргумент
    top[i]=fuser(x); //Значение
    }
normx(&top[n],n,&x0,&xd);//приведение аргумента функции к интервалу [-1,1]
  // Аппроксимация табличной функции с использованием полиномов Эрмита
ier=apmm(hep,n,m,top,ihe,piv,t,&iter);
if(ier!=0)
  return -1;
//Результат  0          1         2          3         4        5
//       674.027594 604.065016 201.064703 32.963258 0.000000 0.000000
// Преобразование разложения функции по полиномам Эрмита в
     алгебраический полином
thep(xd,x0,pol,m,&piv[0],top);
// Результат  0         1         2          3         4       5
//	 -0.120000  -3.330000  6.250000  86.500000 0.000000 0.000000
  // Контроль аппроксисмации
for(i=0;i<n;i++)
{
      x=i*0.1;
      d=x0+xd*x;
      // ВЫЧИСЛЕНИЕ ЗНАЧЕНИЙ ОРТОГОНАЛЬНЫХ ПОЛИНОМОВ ЭРМИТА
      sq=heps(d, piv, m);
      x=sq-fuser(x);// невязка
}
  // освобождение памяти
free(work);free(top);
free(t);free(piv);
free(ihe);
return 0;
}
     .................................................................. */
void thep(double a, double b, double pol[], int n, double c[], double work[])
{
int    j, k;
double fi, h, p, x0, xd;
/*        TEST OF DIMENSION */
if( (n - 1) < 0) goto L_2;
  else
  if( (n - 1) == 0) goto L_1;
    else goto L_3;
/*        DIMENSION LESS THAN 2 */
L_1:
pol[0] = c[0];
L_2:
return;

L_3:
xd = a + a;
x0 = b + b;
pol[0] = c[0] + c[1]*x0;
pol[1] = c[1]*xd;
if( (n - 2) <= 0) goto L_2;
/*        INITIALIZATION */
work[0] = 1.e0; work[1] = x0;
work[2] = 0.e0; work[3] = xd;
fi = 2.e0;
/*        CALCULATE COEFFICIENT VECTOR OF NEXT HERMITE POLYNOMIAL
	AND ADD MULTIPLE OF THIS VECTOR TO POLYNOMIAL POL */
for( j = 3; j <= n; j++ ){
	p = 0.e0;
	for( k = 2; k <= j; k++ ){
		h = p*xd + work[2*k - 2-1]*x0 - fi*work[2*k - 3-1];
		p = work[2*k - 2-1];
		work[2*k - 2-1] = h;
		work[2*k - 3-1] = p;
		pol[k - 1-1] = pol[k - 1-1] + h*c[j-1];
		}
	work[2*j - 1-1] = 0.e0;
	work[2*j-1] = p*xd;
	fi = fi + 2.e0;
	pol[j-1] = c[j-1]*work[2*j-1];
	}
return;
}

