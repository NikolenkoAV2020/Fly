//------------------------------------------------------------------------
// CNPS.C
// ЗНАЧЕНИЕ РАЗЛОЖЕНИЯ ПО ЧЕБЫШЕВУ
// Автор	  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
// Версия  : 01.00 / 10.08.1997 /
//------------------------------------------------------------------------
#include "ssp.h"

//------------------------------------------------------------------------
// SUBROUTINE cnps
//
// PURPOSE
// COMPUTES THE VALUE OF AN N-TERM EXPANSION IN CHEBYSHEV
// POLYNOMIALS WITH COEFFICIENT VECTOR C FOR ARGUMENT VALUE X.
//
// USAGE
// Y=cnps(X,C,N)
//
// DESCRIPTION OF PARAMETERS
//		Y     - RESULT VALUE, DOUBLE PRECISION VARIABLE
//		X     - ARGUMENT VALUE, DOUBLE PRECISION VARIABLE
//		C     - COEFFICIENT VECTOR OF GIVEN EXPANSION
//				COEFFICIENTS ARE ORDERED FROM LOW TO HIGH,
//				DOUBLE PRECISION VECTOR
//		N     - DIMENSION OF COEFFICIENT VECTOR C
// REMARKS OPERATION IS BYPASSED IN CASE N LESS THAN 1
//
// SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED NONE
//
// METHOD DEFINITION
//		Y=SUM(C(I)*T(I-1,X), SUMMED OVER I FROM 1 TO N).
// EVALUATION IS DONE BY MEANS OF BACKWARD RECURSION
// USING THE RECURRENCE EQUATION FOR CHEBYSHEV POLYNOMIALS
// T(N+1,X)=2*X*T(N,X)-T(N-1,X).
//------------------------------------------------------------------------
#include "ssp.h"

//------------------------------------------------------------------------
double cnps(double x, double  c[], int n)
{
	double y;
	int    i, k;
	double arg, h0, h1, h2;

	y = 0.0;

	// TEST OF DIMENSION
	if (n<=0) return y;

	if ((n-2)<0) {	y = c[0]; return y;	}

	// INITIALIZATION
	arg = x+x; h1 = 0.e0; h0 = 0.e0;

	for (i=1; i<=n; i++){
		k  = n-i;
		h2 = h1;
		h1 = h0;
		h0 = arg*h1-h2+c[k];
	}

	y = 0.5e0*(c[0]-h2+h0);
	return y;
}

//------------------------------------------------------------------------
double fuserCnps(double x)
{
	return (86.5*x*x*x + 6.25*x*x - 3.33*x - 0.12);
}

//------------------------------------------------------------------------
#define N 30
#define M 6
#include <malloc.h>

//------------------------------------------------------------------------
int TestCNPS(void)
{
	int     n = N, m = M, mmm, ier, iter;
	int    *ihe;
	double *top, *piv, *t, *work;
	double  pol[M] = { 0, 0, 0, 0, 0, 0 };
	double  xd, x0, x, d, sq;
	int     i;

	mmm = (m+1)*(m+2)/2;
	// Размещение необходимых массивов
	ihe  = (int*   )calloc(3*m+4*n+6, sizeof(int));		 if (!ihe)  return -2;
	piv  = (double*)calloc(3*m+6, sizeof(double));		 if (!piv)  return -2;
	t    = (double*)calloc((m+2)*(m+2), sizeof(double)); if (!t)    return -2;
	work = (double*)calloc(mmm, sizeof(double));		 if (!work) return -2;
	top  = (double*)calloc(n*3, sizeof(double));		 if (!top)  return -2;
	// Формирование тестового массива
	for (i = 0; i<n; i++) {
		x = top[n + i] = 0.1*i;//Аргумент
		top[i] = fuserCnps(x); //Значение
	}
	//приведение аргумента функции к интервалу [-1,1]
	normx11(&top[n], n, &x0, &xd);
	// Аппроксимация табличной функции с использованием полиномов Чебышева
	ier = apmm(cnp, n, m, top, ihe, piv, t, &iter);
	if (ier != 0) return -1;
	//Результат  0          1         2          3         4        5
	//       674.027594 1010.350484 402.129406 65.926516 -0.000000 -0.000000
	// Преобразование разложения функции по полиномам Чебышева в
	// алгебраический полином
	tcnp(xd, x0, pol, m, &piv[0], top);
	// Результат  0         1         2          3         4       5
	//	 -0.120000  -3.330000  6.250000  86.500000 0.000000 0.000000
	// Контроль аппроксисмации
	for (i = 0; i<n; i++) {
		x = i*0.1;
		d = x0+xd*x;
		// ВЫЧИСЛЕНИЕ ЗНАЧЕНИЙ ОРТОГОНАЛЬНЫХ ПОЛИНОМОВ ЧЕБЫШЕВА
		sq = cnps(d, piv, m);
		x = sq - fuserCnps(x);// невязка
	}
	// освобождение памяти
	free(work); free(top); free(t); free(piv); free(ihe);
	return 0;
}