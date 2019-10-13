//------------------------------------------------------------------------
// CNP.C
// ВЫЧИСЛЕНИЕ ЗНАЧЕНИЙ ОРТОГОНАЛЬНЫХ ПОЛИНОМОВ ЧЕБЫШЕВА  [-1,1]
// Автор	 : Перевод программы пакета SSP на С выполнил Пасынков В.В.
// Версия  : 01.00 / 10.08.1997 /
//------------------------------------------------------------------------
#include "ssp.h"

//------------------------------------------------------------------------
// SUBROUTINE cnp
//
// PURPOSE
// COMPUTE THE VALUES OF THE CHEBYSHEV POLYNOMIALS T(N,X)
// FOR ARGUMENT VALUE X AND ORDERS 0 UP TO N.
//
// USAGE  cnp(Y,X,N)
//
// DESCRIPTION OF PARAMETERS
//		Y     - RESULT VECTOR OF DIMENSION N+1 CONTAINING THE VALUES
//				OF CHEBYSHEV POLYNOMIALS OF ORDER 0 UP TO N
//				FOR GIVEN ARGUMENT X.
//				DOUBLE PRECISION VECTOR.
//				VALUES ARE ORDERED FROM LOW TO HIGH ORDER
//		Y     - RESULT VALUE
//				DOUBLE PRECISION VARIABLE.
//		X     - ARGUMENT OF CHEBYSHEV POLYNOMIAL
//		N     - ORDER OF CHEBYSHEV POLYNOMIAL
//
// REMARKS. N LESS THAN 0 IS TREATED AS IF N WERE 0
//
// SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED NONE
//
// METHOD
// EVALUATION IS BASED ON THE RECURRENCE EQUATION FOR
// CHEBYSHEV POLYNOMIALS T(N,X)
// T(N+1,X)=2*X*T(N,X)-T(N-1,X),
// WHERE THE FIRST TERM IN BRACKETS IS THE ORDER,
// THE SECOND IS THE ARGUMENT.
// STARTING VALUES ARE T(0,X)=1, T(1,X)=X.
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//Тест
double fuserCnp(double x)
{
	return (86.5*x*x*x + 6.25*x*x-3.33*x-0.12);
}

#include <malloc.h>

#define N 30
#define M 6

int cnpTest(void)
{
	int     n=N, m=M, mmm, ier, iter;
	int    *ihe;
	double *top,*piv,*t,*work;
	double  pol[M]={0,0,0,0,0,0};
	double  xd,x0,x,d,sq;
	int     i;

	mmm=(m+1)*(m+2)/2;
	// Размещение необходимых массивов
	ihe  = (int*   ) calloc(3*m+4*n+6,sizeof(int));      if(!ihe) return -2;
	piv  = (double*) calloc(3*m+6,sizeof(double));       if(!piv) return -2;
	t    = (double*) calloc((m+2)*(m+2),sizeof(double)); if(!t) return -2;
	work = (double*) calloc(mmm,sizeof(double)); if(!work) return -2;
	top  = (double*) calloc(n*3,sizeof(double)); if(!top) return -2;
	// Формирование тестового массива
	for(i=0;i<n;i++) {
		x=top[n+i]=0.1*i;//Аргумент
		top[i] = fuserCnp(x); //Значение
	}
	normx11(&top[n],n,&x0,&xd);//приведение аргумента функции к интервалу [-1,1]
	// Аппроксимация табличной функции с использованием полиномов Чебышева
	ier=apmm(cnp,n,m,top,ihe,piv,t,&iter);
	if(ier!=0) return -1;
	// Результат  0          1         2          3         4        5
	//       674.027594 1010.350484 402.129406 65.926516 -0.000000 -0.000000
	// Преобразование разложения функции по полиномам Чебышева в
	// алгебраический полином
	tcnp(xd,x0,pol,m,&piv[0],top);
	// Результат  0         1         2          3         4       5
	//	 -0.120000  -3.330000  6.250000  86.500000 0.000000 0.000000
	// Контроль аппроксисмации
	for(i=0;i<n;i++) {
		x=i*0.1;
		d=x0+xd*x;
		// ВЫЧИСЛЕНИЕ ЗНАЧЕНИЙ ОРТОГОНАЛЬНЫХ ПОЛИНОМОВ ЧЕБЫШЕВА
		sq=cnps(d, piv, m);
		x = sq - fuserCnp(x);// невязка
	}
	// освобождение памяти
	free(work); free(top); free(t); free(piv); free(ihe);
	return 0;
}

//------------------------------------------------------------------------
void cnp(double y[], double x, int n)
{
	int    i;
	double f;
	y[0] = 1.e0;
	if (n<=0) return;
	y[1] = x;
	if ((n-1) <= 0) return;
	//        INITIALIZATION */
	f = x+x ;
	for (i=2; i<=n; i++) y[i] = f*y[i-1]-y[i-2];
	return;
}

