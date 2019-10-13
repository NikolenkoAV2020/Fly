//-------------------------------------------------------------------------------
// ZFChebyshov.cpp : 
// Апроксимация полиномами Чебышова
// Исходные коды заимствованны из внешних источников 13.03.2019
//-------------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyChebyshovSource\\ZFChebyshov.h>

//-------------------------------------------------------------------------------
// Функция расчета коэфф. Чебышева
void ComputeChebyshevFactor(double* time, double* ism, int n, int Degree, int reg,
							long double* b, long double* g, long double* c)
{
	double sum1, sum2, f = 0;
	// Колличество слогаемых полинома
	int k = Degree;
	
	register int i;
	int j;
	b[0] = g[0] = g[1] = 0.;
	for (j = 2; j <= k + 2; j++) {
		sum1 = sum2 = 0.;
		if (reg == 2 || (reg == 1 && j == k + 2)) {
			if (j>2) BetGamForChebyshev(time, n, j, b, g);
			for (i = 0; i<n; i++) {
				f = ChebyshevFunc(time[i], j, b, g);
				sum1+= ism[i] * f;
				sum2+= f*f;
			}
			c[j - 2] = sum1 / sum2;
	}	}
}

//-------------------------------------------------------------------------------
// Функция пересчета коэфф. Чебышева в коэфф.степенного полинома
void ChebyshevFactorToPolinim(long double* b, long double* g, long double* c,
							  int Degree, double* ApprKft)
{
	// Колличество слогаемых полинома
	int k = Degree;

	long double alfa[_MAX_STEP_APPROKS_POL + 2][_MAX_STEP_APPROKS_POL + 2];
	memset(alfa, 0, sizeof(alfa));
	memset(ApprKft, 0, sizeof(double)*(k + 1));

	int i, j;
	k += 2;
	for (i = 2; i <= k; i++) {
		for (j = 2; j <= k; j++) {
			if (i == j) { alfa[i][j] = 1; continue; }
			if (i >j) { alfa[i][j] = 0; continue; }
			alfa[i][j] = alfa[i][j - 1] * b[j - 2] + alfa[i][j - 2] * g[j - 2] +
						 alfa[i - 1][j - 1];
	}	}
	// Вычисление коэффициентов степенного полинома 
	for (i = 2; i <= k; i++) {
		for (j = 2; j <= k; j++) ApprKft[i - 2] += c[j - 2] * alfa[i][j];
	}
}

//-------------------------------------------------------------------------------
// Функция расчета коэфф. Чебышева и их пересчета в коэфф.степенного полинома
void AproxByChebyshevPolinom(double *T, double *F, int Size, int Degree,
							 double *ApprKft)
{
	static long double 
		b[_MAX_STEP_APPROKS_POL + 1],
		g[_MAX_STEP_APPROKS_POL + 1],
		c[_MAX_STEP_APPROKS_POL + 1];

	memset(b, 0, sizeof(b));
	memset(g, 0, sizeof(g));
	memset(c, 0, sizeof(c));

	ComputeChebyshevFactor(T, F, Size, Degree, 2, b, g, c);
	ChebyshevFactorToPolinim(b, g, c, Degree, ApprKft);
}

//-------------------------------------------------------------------------------
// Функция расчета коэфф. Чебышева и их пересчета в коэфф.степенного полинома
void AproxMNKByChebyshevPolinom(double *T, double *F, int Size,
								int Degree, double *ApprKft)
{
	/*
	int     n = 40, m = 8, mmm, ier, iter;
	int    *ihe;
	//double *top,
	double  top[120];
	double *piv, *t, *work;
	double  pol[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double  xd, x0, x, d, sq;
	int     i;

	mmm = (m + 1)*(m + 2) / 2;
	// Размещение необходимых массивов
	ihe = (int*)calloc(3 * m + 4 * n + 6, sizeof(int));		 if (!ihe)  return -2;
	piv = (double*)calloc(3 * m + 6, sizeof(double));		 if (!piv)  return -2;
	t = (double*)calloc((m + 2)*(m + 2), sizeof(double)); if (!t)    return -2;
	work = (double*)calloc(mmm, sizeof(double));		 if (!work) return -2;
	//top = (double*)calloc(n * 3, sizeof(double));		 if (!top)  return -2;
	// Формирование тестового массива
	//for (i = 0; i<n; i++) {
	//	x = top[n + i] = 0.1*i;//Аргумент
	//	top[i] = FFFF(x); //Значение
	//}
	double dd = 0;
	for (i = 0; i < 40; i++) {
	x = top[n + i] = dd;//Аргумент
	top[i] = FF(x); //Значение
	dd += 0.1;
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
	FILE* fp = fopen("Cheb2.txt", "w");
	for (i = 0; i < n; i++) {
	x = i*0.1;
	d = x0 + xd*x;
	// ВЫЧИСЛЕНИЕ ЗНАЧЕНИЙ ОРТОГОНАЛЬНЫХ ПОЛИНОМОВ ЧЕБЫШЕВА
	sq = cnps(d, piv, m);
	double V = 0;
	for (int j = m; j > 0; j--) {
	V = V*x + pol[j - 1];
	}
	double FV = FF(x);
	double D  = V - FV;// невязка
	fprintf(fp, "%lf	%lf	%lf	%lf\n", x, V, FV, D);

	}
	fclose(fp);
	*/

	static long double
		b[_MAX_STEP_APPROKS_POL + 1],
		g[_MAX_STEP_APPROKS_POL + 1],
		c[_MAX_STEP_APPROKS_POL + 1];

	memset(b, 0, sizeof(b));
	memset(g, 0, sizeof(g));
	memset(c, 0, sizeof(c));

	ComputeChebyshevFactor(T, F, Size, Degree, 2, b, g, c);
	ChebyshevFactorToPolinim(b, g, c, Degree, ApprKft);
}

//-------------------------------------------------------------------------------
// Вычисление полинома Чебышёва (алгебраического полинома)
//		Degree	- Степень полинома.
//		ApprKft	- Коэффициенты полинома ApprKft[0], ApprKft[1], ... , 
//				  ApprKft[Degree]. Всего (Degree+1) штук.
//		t		- Аргумент полинома.
// Возвращается значение полинома
//      P = ApprKft[0] + ApprKft[1]*t + ApprKft[2]*t*t + ApprKft[Degree]*t*t...t
double ComputeChebyshevPolinom(int Degree, double *ApprKft, double t)
{
	double P = ApprKft[Degree];
	for (int j = Degree; j > 0; j--) P = P*t + ApprKft[j - 1];
	return P;
}


//-------------------------------------------------------------------------------
// Функция расчета betta, gamma
void BetGamForChebyshev(double *time, int n, int j, long double *b, long double *g)
{
	int i = 0;
	long double bet1 = 0, bet2 = 0, gam1 = 0, gam2 = 0, f1 = 0, f2 = 0;
	for (i = 0; i<n; i++) {
		//f1=FUNC_F(time[i],(int)(j-1),b,g) ; 
		_Calc_Fj_Cheb Calc_Fj_Cheb(time[i], b, g);
		f1 = Calc_Fj_Cheb.FUNC_F((int)(j - 1));
		bet1 += time[i] * f1*f1;
		bet2 += f1*f1;
		if (j>3) {
			//f2=FUNC_F(time[i],(int)(j-2),b,g); 
			f2 = Calc_Fj_Cheb.FUNC_F((int)(j - 2));
			gam1 += time[i] * f1*f2;
			gam2 += f2*f2;
		}
	}
	if (bet2)		 b[j - 2] = -bet1 / bet2;
	if (j>3 && gam2) g[j - 2] = -gam1 / gam2;
}

//-------------------------------------------------------------------------------
// Функция расчета коэффициента f полинома Чебышева
double ChebyshevFunc(double t, int j, long double *b, long double *g)
{
	double bet = 0, gam = 0, fb, fg;
	if (j <2) return 0.;
	if (j == 2) return 1.;

	bet = b[j - 2];
	if (j>3) gam = g[j - 2];

	fb = ChebyshevFunc(t, (int)(j - 1), b, g)*(t + bet);

	fg = (j == 3) ? 0 : gam*ChebyshevFunc(t, (int)(j - 2), b, g);
	return (fb + fg);
}


/*
//-------------------------------------------------------------------------------
// Дальше я мало понимаю
//-------------------------------------------------------------------------------
// Функция расчета коэфф. Чебышева
//reg = 1 - будут рассчитываться коэфф. Чебышева 
//			только для старшей степени (Pow)
//reg = 2 - будут рассчитываться коэфф. Чебышева 
//			для всех степеней (от 0 до Pow включительно)
//т.е для расчета значений аппр. полинома заданной степени (Pow) нужно:
// 1) вызов: K_TCH(...,int Pow,int reg=2,...)
// 2) вызов: Rez_Aprox_Cheb(...)
void K_TCH(double* time, double* ism, int n, int Pow, int reg,
	long double* b, long double* g, long double* c)
{
	long double sum1 = 0, sum2 = 0, t0 = 0, f = 0;
	int i, j;
	t0 = time[0];

	for (i = 0; i < n; time[i++] -= t0);
	b[0] = g[0] = g[1] = 0;
	for (j = 2; j <= Pow + 2; j++) {
		sum1 = sum2 = 0.;
		if (reg == 2 || (reg == 1 && j == Pow + 2)) {
			if (j>2) BetGamForChebyshev(time, n, j, b, g);
			for (i = 0; i<n; i++) {
				//f=FUNC_F(time[i],j,b,g);
				_Calc_Fj_Cheb Calc_Fj_Cheb(time[i], b, g);
				f = Calc_Fj_Cheb.FUNC_F(j);
				sum1 += ism[i] * f;
				sum2 += f*f;
			}
			c[j - 2] = sum1 / sum2;
		}
	}
	for (i = 0; i<n; time[i++] += t0);
}

//-------------------------------------------------------------------------------
double SN_TCH(double* time, double* ism, double t, int n, int k, int reg1,
	long double* b, long double* g, long double* c)
{
	long double z = 0;
	int j, reg = 1;
	t -= time[0];

	_Calc_Fj_Cheb Calc_Fj_Cheb(t, b, g);
	for (j = 0; j <= k; j++) {
		if ((reg1 == 2) || (reg1 == 1 && j == k))
			K_TCH(time, ism, n, j, reg, b, g, c);
		z += c[j] * Calc_Fj_Cheb.FUNC_F((int)(j + 2));
	}
	return ((double)z);
}

//-------------------------------------------------------------------------------
long double Rez_Aprox_Cheb(double Arg, int Pow, const long double* b,
	const long double* g, const long double* c)
{
	_Calc_Fj_Cheb Calc_Fj_Cheb(Arg, b, g);
	long double z = 0;
	for (int j = 0; j <= Pow; j++) z += c[j] * Calc_Fj_Cheb.FUNC_F((int)(j + 2));
	return z;
}

//-------------------------------------------------------------------------------
long double Rez_Aprox_Cheb(double Arg, const vector<_Koef_Cheb> &V_Koef_Cheb)
{
	int Pow = V_Koef_Cheb.size() - 1;
	long double *b = new long double[Pow + 1];
	long double *g = new long double[Pow + 1];
	long double *c = new long double[Pow + 1];

	for (int j = 0; j <= Pow; j++) {
		const _Koef_Cheb &Koef_Cheb = V_Koef_Cheb[j];
		b[j] = Koef_Cheb.b;
		g[j] = Koef_Cheb.g;
		c[j] = Koef_Cheb.c;
	}
	long double z = Rez_Aprox_Cheb(Arg, Pow, b, g, c);

	delete[]b;
	delete[]g;
	delete[]c;

	return z;
}
*/
//-------------------------------------------------------------------------------


