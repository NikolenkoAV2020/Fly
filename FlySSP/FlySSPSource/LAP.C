/*------------------------------------------------------------------------
  LAP.C
    ВЫЧИСЛЕНИЕ ЗНАЧЕНИЙ ОРТОГОНАЛЬНЫХ ПОЛИНОМОВ ЛАГЕРРА [0,+беск]
  Автор	 : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия : 01.00 / 10.06.1996 /
------------------------------------------------------------------------*/
#include "ssp.h"
/*     ..................................................................

	SUBROUTINE lap

	PURPOSE
	   COMPUTE THE VALUES OF THE LAGUERRE POLYNOMIALS L(N,X)
	   FOR ARGUMENT VALUE X AND ORDERS 0 UP TO N.

	USAGE
	   lap(Y,X,N)

	DESCRIPTION OF PARAMETERS
	   Y     - RESULT VECTOR OF DIMENSION N+1 CONTAINING THE VALUES
                   OF LAGUERRE POLYNOMIALS OF ORDER 0 UP TO N
                   FOR GIVEN ARGUMENT X.
                   DOUBLE PRECISION VECTOR.
                   VALUES ARE ORDERED FROM LOW TO HIGH ORDER
           X     - ARGUMENT OF LAGUERRE POLYNOMIAL
                   DOUBLE PRECISION VARIABLE.
	   N     - ORDER OF LAGUERRE POLYNOMIAL

        REMARKS
           N LESS THAN 0 IS TREATED AS IF N WERE 0

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           EVALUATION IS BASED ON THE RECURRENCE EQUATION FOR
           LAGUERRE POLYNOMIALS L(N,X)
           L(N+1,X)=2*L(N,X)-L(N-1,X)-((1+X)*L(N,X)-L(N-1,X))/(N+1),
	   WHERE THE FIRST TERM IN BRACKETS IS THE ORDER,
	   THE SECOND IS THE ARGUMENT.
	   STARTING VALUES ARE L(0,X)=1, L(1,X)=1.-X.

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
  // Аппроксимация табличной функции с использованием полиномов Лагерра
ier=apmm(lap,n,m,top,ihe,piv,t,&iter);
if(ier!=0)
  return -1;
//Результат  0            1            2             3         4        5
//       4275.223125 -8776.315313  6355.226750  -1582.236375 0.000000 -0.000000
// Преобразование разложения функции по полиномам Лагерра в
     алгебраический полином
tlap(xd,x0,pol,m,&piv[0],top);
// Результат  0         1         2          3         4       5
//	 -0.120000  -3.330000  6.250000  86.500000 0.000000 0.000000
  // Контроль аппроксисмации
for(i=0;i<n;i++)
{
      x=i*0.1;
      d=x0+xd*x;
      // ВЫЧИСЛЕНИЕ ЗНАЧЕНИЙ ОРТОГОНАЛЬНЫХ ПОЛИНОМОВ Лагерра
      sq=laps(d, piv, m);
      x=sq-fuser(x);// невязка
}
  // освобождение памяти
free(work);free(top);
free(t);free(piv);
free(ihe);
return 0;
}
     .................................................................. */
void lap(double y[],double x,int n)
{
int  i;
double t;
/*        TEST OF ORDER */
y[0] = 1.e0;
if( n <= 0 )
  return;
y[1] = 1.e0 - x;
if( (n - 1) <=0 )
  return;
/*        INITIALIZATION */
t = 1.e0 + x;
for( i = 2; i <= n; i++ )
  y[i + 1-1] = y[i-1] - y[i - 1-1] + y[i-1] - (t*y[i-1] - y[i - 1-1])/(double)(i);
return;
}

