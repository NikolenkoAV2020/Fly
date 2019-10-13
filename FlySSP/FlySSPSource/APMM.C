//-----------------------------------------------------------------------
// APMM.C
// ЛИНЕЙНАЯ ЧЕБЫШЕВСКАЯ АППРОКСИМАЦИЯ ЗАДАННОЙ ДИСКРЕТНОЙ ФУНКЦИИ
// ЛИНЕЙНОЙ КОМБИНАЦИЕЕЙ ЗАДАННЫХ НЕПРЕРЫВНЫХ ФУНКЦИЙ
// Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
// Версия  : 01.00 / 10.08.1997 /
//-----------------------------------------------------------------------
#include <math.h>
#include "ssp.h"

//-----------------------------------------------------------------------
//	SUBROUTINE apmm
//
//  PURPOSE
//     APPROXIMATE A FUNCTION TABULATED IN N POINTS BY ANY LINEAR
//     COMBINATION OF M GIVEN CONTINUOUS FUNCTIONS IN THE SENSE
//     OF CHEBYSHEV.
//
//	USAGE
//	   apmm(FCT,N,M,TOP,IHE,PIV,T,ITER,IER)
//	   PARAMETER FCT REQUIRES AN EXTERNAL STATEMENT IN THE
//	   CALLING PROGRAM.
//
//	DESCRIPTION OF PARAMETERS
//	   FCT    - NAME OF SUBROUTINE TO BE SUPPLIED BY THE USER.
//              IT COMPUTES VALUES OF M GIVEN FUNCTIONS FOR
//              ARGUMENT VALUE X.
//              USAGE
//              CALL FCT(Y,X,K)
//              DESCRIPTION OF PARAMETERS
//        Y   - DOUBLE PRECISION RESULT VECTOR OF DIMEN-
//              SION M CONTAINING THE VALUES OF GIVEN
//		        CONTINUOUS FUNCTIONS FOR GIVEN ARGUMENT X
//	      X   - DOUBLE PRECISON ARGUMENT VALUE
//        K   - AN INTEGER VALUE WHICH IS EQUAL TO M-1
//              REMARKS
//              IF APPROXIMATION BY NORMAL CHEBYSHEV, SHIFTED
//              CHEBYSHEV, LEGENDRE, LAGUERRE, HERMITE POLYNO-
//              MIALS IS DESIRED SUBROUTINES DCNP,DCSP,DLEP,
//              DLAP,DHEP, RESPECTIVELY FROM SSP COULD BE USED.
//     N      - NUMBER OF DATA POINTS DEFINING THE FUNCTION WHICH
//              IS TO BE APPROXIMATED
//     M      - NUMBER OF GIVEN CONTINUOUS FUNCTIONS FROM WHICH
//              THE APPROXIMATING FUNCTION IS CONSTRUCTED.
//     TOP    - DOUBLE PRECISION VECTOR OF DIMENSION 3*N.
//	            ON ENTRY IT MUST CONTAIN FROM TOP(1) UP TO TOP(N)
//    		    THE GIVEN N FUNCTION VALUES AND FROM TOP(N+1) UP
//		        TO TOP(2*N) THE CORRESPONDING NODES
//		        ON RETURN TOP CONTAINS FROM TOP(1) UP TO TOP(N)
//		        THE ERRORS AT THOSE N NODES.
//		        OTHER VALUES OF TOP ARE SCRATCH.
//	   IHE    - INTEGER VECTOR OF DIMENSION 3*M+4*N+6
//	   PIV    - DOUBLE PRECISION VECTOR OF DIMENSION 3*M+6.
//		        ON RETURN PIV CONTAINS AT PIV(1) UP TO PIV(M) THE
//		        RESULTING COEFFICIENTS OF LINEAR APPROXIMATION.
//	   T      - DOUBLE PRECISION AUXILIARY VECTOR OF DIMENSION
//		        (M+2)*(M+2)
//	   ITER   - RESULTANT INTEGER WHICH SPECIFIES THE NUMBER OF
//              ITERATIONS NEEDED
//	   IER    - RESULTANT ERROR PARAMETER CODED IN THE FOLLOWING
//              FORM
//              IER=0  - NO ERROR
//              IER=1  - THE NUMBER OF ITERATIONS HAS REACHED
//                       THE INTERNAL MAXIMUM N+M
//              IER=-1 - NO RESULT BECAUSE OF WRONG INPUT PARA-
//                       METER M OR N OR SINCE AT SOME ITERATION
//                       NO SUITABLE PIVOT COULD BE FOUND
//
// REMARKS
//     NO ACTION BESIDES ERROR MESSAGE IN CASE M LESS THAN 1 OR
//	   N LESS THAN 2.
//
// SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
//     THE EXTERNAL SUBROUTINE FCT MUST BE FURNISHED BY THE USER.
//
// METHOD
//     THE PROBLEM OF APPROXIMATION A TABULATED FUNCTION BY ANY
//     LINEAR COMBINATION OF GIVEN FUNCTIONS IN THE SENSE OF
//     CHEBYSHEV (I.E. TO MINIMIZE THE MAXIMUM ERROR) IS TRANS-
//     FORMED INTO A LINEAR PROGRAMMING PROBLEM. DAPMM USES A
//     REVISED SIMPLEX METHOD TO SOLVE A CORRESPONDING DUAL
//     PROBLEM. FOR REFERENCE, SEE
//     I.BARRODALE/A.YOUNG, ALGORITHMS FOR BEST L-SUB-ONE AND
//	   L-SUB-INFINITY, LINEAR APPROXIMATIONS ON A DISCRETE SET,
//     NUMERISCHE MATHEMATIK, VOL.8, ISS.3 (1966), PP.295-306.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//Тест
#include <malloc.h>

double fuserApmm(double x)
{
	return (86.5*x*x*x + 6.25*x*x - 3.33*x - 0.12);
}

#define N 30
#define M 6

int apmmTest(void)
{
	int     n = N, m = M, mmm, ier, iter;
	int    *ihe;
	double *top, *piv, *t, *work;
	double  pol[M] = { 0, 0, 0, 0, 0, 0 };
	double  xd, x0, x, d, sq;
	int     i;

	mmm = (m + 1)*(m + 2) / 2;
	// Размещение необходимых массивов
	ihe  = (int*   )calloc(3*m+4*n+6, sizeof(int));      if (!ihe) return -2;
	piv  = (double*)calloc(3*m+6, sizeof(double));       if (!piv) return -2;
	t    = (double*)calloc((m+2)*(m+2), sizeof(double)); if (!t) return -2;
	work = (double*)calloc(mmm, sizeof(double)); if (!work) return -2;
	top  = (double*)calloc(n*3, sizeof(double)); if (!top) return -2;
	// Формирование тестового массива
	for (i = 0; i<n; i++)
	{
		//Аргумент
		x = top[n+i] = 0.1*i; 
		//Значение
		top[i] = fuserApmm(x);
	}
	normx11(&top[n], n, &x0, &xd); //приведение аргумента функции к интервалу [-1,1]
	// Аппроксимация табличной функции с использованием полиномов Эрмита
	ier = apmm(hep, n, m, top, ihe, piv, t, &iter);
	if (ier != 0) return-1;
	//Результат  0          1         2          3         4        5
	//       674.027594 604.065016 201.064703 32.963258 0.000000 0.000000
	// Преобразование разложения функции по полиномам Эрмита в
	// алгебраический полином
	thep(xd, x0, pol, m, &piv[0], top);
	// Результат  0         1         2          3         4       5
	//	 -0.120000  -3.330000  6.250000  86.500000 0.000000 0.000000
	// Контроль аппроксисмации
	for (i = 0; i<n; i++) {
		x = i*0.1;
		d = x0 + xd*x;
		// ВЫЧИСЛЕНИЕ ЗНАЧЕНИЙ ОРТОГОНАЛЬНЫХ ПОЛИНОМОВ ЭРМИТА
		sq = heps(d, piv, m);
		x = sq - fuserApmm(x);// невязка
	}
	// освобождение памяти
	free(work); free(top); free(t); free(piv); free(ihe);
	return 0;
}

//-----------------------------------------------------------------------
int apmm(void(*fct)(double *, double, int),
int n, int  m, double *top, int *ihe, double *piv,
double * t, int *iter)
{
	int  ier;
	int  i, ido, ilab, ind, ipiv, ise, j, k, l, ll, nan;
	double dsum, help, repi, saven, tol;




	/*        TEST ON WRONG INPUT PARAMETERS N AND M */
	ier = -1;
	if ((n - 1) <= 0) goto L_81;
	if (m <= 0) goto L_81;
	/*        INITIALIZE CHARACTERISTIC VECTORS FOR THE TABLEAU */
	ier = 0;
	/*        PREPARE TOP-ROW TOP */
	for (i = 1; i <= n; i++){
		k = i + n;
		j = k + n;
		top[j - 1] = top[k - 1];
		top[k - 1] = -top[i - 1];
	}

	/*        PREPARE INVERSE TRANSFORMATION MATRIX T */
	l = m + 2; ll = l*l;
	for (i = 1; i <= ll; i++)
		t[i - 1] = 0.e0;
	k = 1;
	j = l + 1;
	for (i = 1; i <= l; i++){
		t[k - 1] = 1.e0;
		k = k + j;
	}

	/*        PREPARE INDEX-VECTOR IHE */
	for (i = 1; i <= l; i++){
		k = i + l;	j = k + l;
		ihe[i - 1] = 0;	ihe[k - 1] = i;	ihe[j - 1] = 1 - i;
	}
	nan = n + n; k = l + l + l; j = k + nan;
	for (i = 1; i <= nan; i++){
		k = k + 1;	ihe[k - 1] = i;
		j = j + 1;	ihe[j - 1] = i;
	}

	/*        SET COUNTER ITER FOR ITERATION-STEPS */
	(*iter) = -1;
L_8:
	(*iter) = (*iter) + 1;

	/*        TEST FOR MAXIMUM ITERATION-STEPS */
	if ((n + m - (*iter)) > 0) goto L_10;
	ier = 1;
	goto L_69;

	/*        DETERMINE THE COLUMN WITH THE MOST POSITIVE ELEMENT IN TOP */
L_10:
	ise = 0;
	ipiv = 0;
	k = l + l + l;
	saven = 0.e0;

	/*        START TOP-LOOP */
	for (i = 1; i <= nan; i++){
		ido = k + i;
		help = top[i - 1];
		if ((help - saven) > 0)
		{
			saven = help;
			ipiv = i;
		}
		if (ihe[ido - 1] == 0)
			ise = i;
	}
	/*        END OF TOP-LOOP

	IS OPTIMAL TABLEAU REACHED */
	if (ipiv <= 0) goto L_69;
	/*        DETERMINE THE PIVOT-ELEMENT FOR THE COLUMN CHOSEN UPOVE */
	ilab = 1;
	ind = 0;
	j = ise;
	if (j <= 0) goto L_21;
	else goto L_34;
	/*        TRANSFER K-TH COLUMN FROM T TO PIV */
L_16:
	k = (k - 1)*l;
	for (i = 1; i <= l; i++){
		j = l + i;
		k = k + 1;
		piv[j - 1] = t[k - 1];
	}

	/*        IS ANOTHER COLUMN NEEDED FOR SEARCH FOR PIVOT-ELEMENT */
L_18:
	if (ise <= 0) goto L_22;
	ise = -ise;
	/*        TRANSFER COLUMNS IN PIV */
	j = l + 1; ido = l + l;
	for (i = j; i <= ido; i++){
		k = i + l;
		piv[k - 1] = piv[i - 1];
	}
L_21:
	j = ipiv;
	goto L_34;

	/*        SEARCH PIVOT-ELEMENT PIV(IND) */
L_22:
	saven = 1.e38;
	ido = 0;    k = l + 1;
	ll = l + l; ind = 0;

	/*        START PIVOT-LOOP */
	for (i = k; i <= ll; i++){
		j = i + l;	help = piv[i - 1];
		if (help <= 0.0) continue;
		help = -help;
		if (ise != 0) goto L_26;
		if (ihe[j - 1] != 0) goto L_27;
		ido = i;
		continue;
	L_26:
		help = -piv[j - 1] / help;
	L_27:
		if ((help - saven) >= 0.0) continue;
		saven = help;
		ind = i;
	}
	/*        END OF PIVOT-LOOP
	TEST FOR SUITABLE PIVOT-ELEMENT */
	if (ind > 0) goto L_32;
	if (ido <= 0) goto L_68;
	ind = ido;
	/*        PIVOT-ELEMENT IS STORED IN PIV(IND)
	COMPUTE THE RECIPROCAL OF THE PIVOT-ELEMENT REPI */
L_32:
	repi = 1.e0 / piv[ind - 1];
	ind = ind - l;

	/*        UPDATE THE TOP-ROW TOP OF THE TABLEAU */
	ilab = 0;
	saven = -top[ipiv - 1] * repi;
	top[ipiv - 1] = saven;
	/*        INITIALIZE J AS COUNTER FOR TOP-LOOP */
	j = nan;
L_33:
	if ((j - ipiv) == 0) goto L_53;
L_34:
	k = 0;

	/*        SEARCH COLUMN IN TRANSFORMATION-MATRIX T */
	for (i = 1; i <= l; i++){
		if ((ihe[i - 1] - j) != 0) continue;
		k = i;
		if (ilab <= 0) goto L_50;
		else goto L_16;
	}

	/*        GENERATE COLUMN USING SUBROUTINE FCT AND TRANSFORMATION-MATRIX */
	i = l + l + l + nan + j;
	i = ihe[i - 1] - n;
	if (i <= 0)
	{
		i = i + n;
		k = 1;
	}
	i = i + nan;

	/*        CALL SUBROUTINE FCT */
	(*fct)(piv, top[i - 1], m - 1);

	/*        PREPARE THE CALLED VECTOR PIV */
	dsum = 0.e0;
	ido = m;
	for (i = 1; i <= m; i++){
		help = piv[ido - 1];
		if (k <= 0)
			help = -help;
		dsum = dsum + help;
		piv[ido + 1 - 1] = help;
		ido = ido - 1;
	}
	piv[l - 1] = -dsum;
	piv[0] = 1.e0;

	/*        TRANSFORM VECTOR PIV WITH ROWS OF MATRIX T */
	ido = ind;
	if (ilab <= 0) goto L_44;

	k = 1;
L_43:
	ido = k;
L_44:
	dsum = 0.e0;
	help = 0.e0;

	/*        START MULTIPLICATION-LOOP */
	for (i = 1; i <= l; i++){
		dsum = dsum + piv[i - 1] * t[ido - 1];
		tol = fabs(dsum);
		if ((tol - help) > 0.0)
			help = tol;
		ido = ido + l;
	}
	/*        END OF MULTIPLICATION-LOOP */
	tol = 1.e-14*help;
	if ((fabs(dsum) - tol) <= 0.0)
		dsum = 0.e0;

	if (ilab <= 0) goto L_51;
	i = k + l;
	piv[i - 1] = dsum;

	/*        TEST FOR LAST COLUMN-TERM */
	k = k + 1;
	if ((k - l) <= 0) goto L_43;
	else goto L_18;
L_50:
	i = (k - 1)*l + ind;
	dsum = t[i - 1];

	/*        COMPUTE NEW TOP-ELEMENT */
L_51:
	dsum = dsum*saven;
	tol = 1.e-14*fabs(dsum);
	top[j - 1] = top[j - 1] + dsum;
	if ((fabs(top[j - 1]) - tol) <= 0.0)
		top[j - 1] = 0.e0;

	/*        TEST FOR LAST TOP-TERM */
L_53:
	j = j - 1;

	if (j > 0) goto L_33;
	/*        END OF TOP-LOOP
	TRANSFORM PIVOT-COLUMN */
	i = ind + l;
	piv[i - 1] = -1.e0;
	for (i = 1; i <= l; i++){
		j = i + l;
		piv[i - 1] = -piv[j - 1] * repi;
	}

	/*        UPDATE TRANSFORMATION-MATRIX T */
	j = 0;
	for (i = 1; i <= l; i++){
		ido = j + ind;
		saven = t[ido - 1];
		t[ido - 1] = 0.e0;
		for (k = 1; k <= l; k++){
			ise = k + j;
			t[ise - 1] = t[ise - 1] + saven*piv[k - 1];
		}
		j = j + l;
	}

	/*        UPDATE INDEX-VECTOR IHE
	INITIALIZE CHARACTERISTICS */
	j = 0;
	k = 0;
	ise = 0;
	ido = 0;

	/*        START QUESTION-LOOP */
	for (i = 1; i <= l; i++){
		ll = i + l;
		ilab = ihe[ll - 1];
		if ((ihe[i - 1] - ipiv) == 0)
		{
			ise = i;
			j = ilab;
		}
		if ((ilab - ind) == 0)
		{
			ido = i;
			k = ihe[i - 1];
		}
	}
	/*        END OF QUESTION-LOOP

	START MODIFICATION */
	if (k <= 0)
	{
		ihe[ido - 1] = ipiv;
		if (ise <= 0) goto L_67;
		else goto L_65;
	}
	if ((ind - j) == 0) goto L_66;
	ll = l + l + l + nan;
	k = k + ll;
	i = ipiv + ll;
	ilab = ihe[k - 1];
	ihe[k - 1] = ihe[i - 1];
	ihe[i - 1] = ilab;
	if (ise <= 0) goto L_67;
L_65:
	ido = ido + l;
	i = ise + l;
	ihe[ido - 1] = j;
	ihe[i - 1] = ind;
L_66:
	ihe[ise - 1] = 0;
L_67:
	ll = l + l;
	j = ll + ind;
	i = ll + l + ipiv;
	ilab = ihe[i - 1];
	ihe[i - 1] = ihe[j - 1];
	ihe[j - 1] = ilab;
	/*        END OF MODIFICATION */
	goto L_8;

	/*        SET ERROR PARAMETER IER=-1 SINCE NO SUITABLE PIVOT IS FOUND */
L_68:
	ier = -1;

	/*        EVALUATE FINAL TABLEAU
	COMPUTE SAVEN AS MAXIMUM ERROR OF APPROXIMATION AND
	HELP AS ADDITIVE CONSTANCE FOR RESULTING COEFFICIENTS */
L_69:
	saven = 0.e0;
	help = 0.e0;
	k = l + l + l;
	for (i = 1; i <= nan; i++){
		ido = k + i;
		j = ihe[ido - 1];
		if (j <0) goto L_71;
		else
			if (j == 0) goto L_70;
			else continue;
		L_70:	saven = -top[i - 1];
		L_71:	if ((m + j + 1) == 0)
			help = top[i - 1];
	}

	/*        PREPARE T,TOP,PIV */
	t[0] = saven;
	ido = nan + 1;
	j = nan + n;
	for (i = ido; i <= j; i++)
		top[i - 1] = saven;
	for (i = 1; i <= m; i++)
		piv[i - 1] = help;

	/*        COMPUTE COEFFICIENTS OF RESULTING POLYNOMIAL IN PIV(1) UP TO PI
	AND CALCULATE ERRORS AT GIVEN NODES IN TOP(1) UP TO TOP(N) */
	for (i = 1; i <= nan; i++){
		ido = k + i;
		j = ihe[ido - 1];
		if (j < 0) goto L_76;
		else
			if (j == 0) continue;
			else goto L_77;
		L_76:   j = -j;
			piv[j - 1] = help - top[i - 1];
			continue;
		L_77:   if ((j - n) <= 0)
		{
			j = j + nan;
			top[j - 1] = saven + top[i - 1];
		}
	}
	for (i = 1; i <= n; i++){
		ido = nan + i;
		top[i - 1] = top[ido - 1];
	}
L_81:
	return ier;
}

