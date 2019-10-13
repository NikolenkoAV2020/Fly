/*------------------------------------------------------------------------
  TLEP.C
      ПРЕОБРАЗОВАНИЕ ПОЛИНОМА ЛЕЖАНДРА В АЛГЕБРАИЧЕСКИЙ ПОЛИНОМ
  Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия : 01.00 / 01.06.1994 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE DTLEP

        PURPOSE
           A SERIES EXPANSION IN LEGENDRE POLYNOMIALS WITH INDEPENDENT
           VARIABLE X IS TRANSFORMED TO A POLYNOMIAL WITH INDEPENDENT
           VARIABLE Z, WHERE X=A*Z+B

        USAGE
	   TLEP(A,B,POL,N,C,WORK)

        DESCRIPTION OF PARAMETERS
           A     - FACTOR OF LINEAR TERM IN GIVEN LINEAR TRANSFORMATION
                   DOUBLE PRECISION VARIABLE
           B     - CONSTANT TERM IN GIVEN LINEAR TRANSFORMATION
                   DOUBLE PRECISION VARIABLE
           POL   - COEFFICIENT VECTOR OF POLYNOMIAL (RESULTANT VALUE)
                   COEFFICIENTS ARE ORDERED FROM LOW TO HIGH
                   DOUBLE PRECISION VECTOR
           N     - DIMENSION OF COEFFICIENT VECTORS POL AND C
           C     - GIVEN COEFFICIENT VECTOR OF EXPANSION
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
           THE RANGE (-1,+1) IN X TO THE RANGE (ZL,ZR) IN Z, WHERE
           ZL=-(1+B)/A AND ZR=(1-B)/A.
           FOR GIVEN ZL, ZR WE HAVE A=2/(ZR-ZL) AND B=-(ZR+ZL)/(ZR-ZL)

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           THE TRANSFORMATION IS BASED ON THE RECURRENCE EQUATION
           FOR LEGENDRE POLYNOMIALS P(N,X)
           P(N+1,X)=2*X*P(N,X)-P(N-1,X)-(X*P(N,X)-P(N-1,X))/(N+1),
           WHERE THE FIRST TERM IN BRACKETS IS THE INDEX,
           THE SECOND IS THE ARGUMENT.
           STARTING VALUES ARE P(0,X)=1, P(1,X)=X.
           THE TRANSFORMATION IS IMPLICITLY DEFINED BY MEANS OF
           X=A*Z+B TOGETHER WITH
	   SUM(POL(I)*Z**(I-1), SUMMED OVER I FROM 1 TO N)
           =SUM(C(I)*P(I-1,X), SUMMED OVER I FROM 1 TO N).
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
  // Аппроксимация табличной функции с использованием полиномов Лежандра
ier=apmm(lep,n,m,top,ihe,piv,t,&iter);
if(ier!=0)
  return -1;
//Результат  0            1            2          3         4        5
	 539.984458  970.794575  536.172542  105.482425 -0.000000 -0.000000
// Преобразование разложения функции по полиномам Лежандра в
     алгебраический полином
tlep(xd,x0,pol,m,&piv[0],top);
// Результат  0         1         2          3         4       5
//	 -0.120000  -3.330000  6.250000  86.500000 0.000000 0.000000
  // Контроль аппроксисмации
for(i=0;i<n;i++)
{
      x=i*0.1;
      d=x0+xd*x;
      // ВЫЧИСЛЕНИЕ ЗНАЧЕНИЙ ОРТОГОНАЛЬНЫХ ПОЛИНОМОВ Лежандра
      sq=leps(d, piv, m);
      x=sq-fuser(x);// невязка
}
  // освобождение памяти
free(work);free(top);
free(t);free(piv);
free(ihe);
return 0;
}

     ..................................................................
 */
void tlep(double a, double b, double pol[], int n, double c[], double work[])
{
int  j, k;
double fi, h, p, q, q1;
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
pol[0] = c[0] + b*c[1];
pol[1] = a*c[1];
if( (n - 2) <= 0) goto L_2;
/*        INITIALIZATION */
work[0] = 1.e0;
work[1] = b;
work[2] = 0.e0;
work[3] = a;
fi = 1.e0;

/*        CALCULATE COEFFICIENT VECTOR OF NEXT LEGENDRE POLYNOMIAL
	AND ADD MULTIPLE OF THIS VECTOR TO POLYNOMIAL POL */
for( j = 3; j <= n; j++ ){
	fi = fi + 1.e0;
	q = 1.e0/fi - 1.e0;
	q1 = 1.e0 - q;
	p = 0.e0;
	for( k = 2; k <= j; k++ ){
		h = (a*p + b*work[2*k - 2-1])*q1 + q*work[2*k - 3-1]
		 ;
		p = work[2*k - 2-1];
		work[2*k - 2-1] = h;
		work[2*k - 3-1] = p;
		pol[k - 1-1] = pol[k - 1-1] + h*c[j-1];
		}
	work[2*j - 1-1] = 0.e0;
	work[2*j-1] = a*p*q1;
	pol[j-1] = c[j-1]*work[2*j-1];
	}
return;
}

