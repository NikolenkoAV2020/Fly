/*------------------------------------------------------------------------
  HEPS.C
    ЗНАЧЕНИЕ РАЗЛОЖЕНИЯ ПО ЭРМИТУ
  Автор	 : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия : 01.00 / 01.06.1994 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

	SUBROUTINE heps

	PURPOSE
	   COMPUTES THE VALUE OF AN N-TERM EXPANSION IN HERMITE
	   POLYNOMIALS WITH COEFFICIENT VECTOR C FOR ARGUMENT VALUE X.

	USAGE
	   Y=heps(X,C,N)

        DESCRIPTION OF PARAMETERS
           Y     - RESULT VALUE
                   DOUBLE PRECISION VARIABLE
           X     - ARGUMENT VALUE
		   DOUBLE PRECISION VARIABLE
	   C     - COEFFICIENT VECTOR OF GIVEN EXPANSION
                   COEFFICIENTS ARE ORDERED FROM LOW TO HIGH
                   DOUBLE PRECISION VECTOR
           N     - DIMENSION OF COEFFICIENT VECTOR C

        REMARKS
           OPERATION IS BYPASSED IN CASE N LESS THAN 1

	SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
	   DEFINITION
	   Y=SUM(C(I)*H(I-1,X), SUMMED OVER I FROM 1 TO N).
	   EVALUATION IS DONE BY MEANS OF UPWARD RECURSION
	   USING THE RECURRENCE EQUATION FOR HERMITE POLYNOMIALS
	   H(N+1,X)=2*(X*H(N,X)-N*H(N-1,X)).
//Тест
#include <alloc.h>
#include <ssp.h>

double fuser(double x)
{
return (86.5*x*x*x + 6.25*x*x-3.33*x-0.12);
}

#define N 30
#define M 6

int main(void)
{
int     n=N, m=M, mmm, ier, iter;
int    *ihe;
double   *top,*piv,*t,*work;
double    pol[M]={0,0,0,0,0,0};
double    xd,x0,x,d,sq;
int     i;

  mmm=(m+1)*(m+2)/2;
  // Размещение необходимых массивов
  ihe  = (int *) calloc(3*m+4*n+6,sizeof(int));
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
double heps( double x, double c[], int n)
{
int     i;
double  h0, h1, h2, y;
/*        TEST OF DIMENSION */
y=0.0;
if(n <= 0 )
  return y;

y = c[0];
if( (n - 2) < 0 )
  return y;
/*        INITIALIZATION */
h0 = 1.e0; h1 = x + x;
for( i = 2; i <= n; i++ ){
	h2 = x*h1 - (double)(i - 1)*h0;
	h0 = h1;
	h1 = h2 + h2;
	y = y + c[i-1]*h0;
	}
return y;
}

