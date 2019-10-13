//------------------------------------------------------------------------
// AHI.C
// ИНТЕРПОЛИРОВАНИЕ ФУНКЦИИ С ПОМОЩЬЮ ИНТЕРПОЛЯЦИОННОГО ПРОЦЕССА
// ЭЙТКЕНА-ЭРМИТА
// Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
// Версия  : 01.00 / 10.08.1997 /
//------------------------------------------------------------------------
#include <math.h>
#include "ssp.h"
#include <FlyMMath.h>

double INF_SUP(const double X1, const double X2, const double X3)
{
	double d = Max(X1, X2);
	return Min(d, X3);
}

//------------------------------------------------------------------------
// Подпрограмма (SUBROUTINE) AHI
//
// Назначение(PURPOSE)
// Вычисляет значение Y однозначной функции Y=Y(X) для заданнного
// значения X ее аргумента по таблице Xi, Yi , dYi/dXi (i=1,2,...,N)
// соответствующих значений аргумента, функции и ее первой
// производной.
// TO INTERPOLATE FUNCTION VALUE Y FOR A GIVEN ARGUMENT VALUE
// X USING A GIVEN TABLE (ARG,VAL) OF ARGUMENT, FUNCTION, AND
// DERIVATIVE VALUES.
//
// Обращение к подпрограмме (USAGE)
//		IER=ahi (X,ARG,VAL,Y,NDIM,EPS);
//
// Входные и выходные параметры (DESCRIPTION OF PARAMETERS)
//		X      - заданное значение аргумента, для которого вычисляется
//				 интерполированное значение функции,
//		ARG    - массив длиной NDIM заданных табличных значений
//				 аргумента. В процессе вычислений этот массив не
//				 портится.
//				 DOUBLE PRECISION INPUT VECTOR (DIMENSION NDIM) OF
//				 ARGUMENT VALUES OF THE TABLE (NOT DESTROYED).
//		VAL    - массив длиной NDIM*2 заданных табличных значений
//				 функции и ее производной.
//				 Значения функции и ее производной распалагаются парами,
//				 т.е. VAL[j]=Yi, VAL[j+1]=dYi/dXi
//				 i=1,...,NDIM, j=0,2,4, ..., 2NDIM-1.
//				 В процессе вычислений этот массив портится.
//				 DOUBLE PRECISION INPUT VECTOR (DIMENSION 2*NDIM) OF
//				 FUNCTION AND DERIVATIVE VALUES OF THE TABLE (DES-
//				 TROYED). FUNCTION AND DERIVATIVE VALUES MUST BE
//				 STORED IN PAIRS, THAT MEANS BEGINNING WITH FUNCTION
//				 VALUE AT POINT ARG(1) EVERY FUNCTION VALUE MUST BE
//				 FOLLOWED BY THE DERIVATIVE VALUE AT THE SAME POINT.
//		Y      - вычисляемое интерполированное значение функции.
//				 RESULTING INTERPOLATED DOUBLE PRECISION FUNCTION VALUE.
//		NDIM   - число таблично заданных значений аргумента и функции;
//				 если NDIM<1, то осуществляется выход из подпрограммы
//				 без вычислений.
//				 AN INPUT VALUE WHICH SPECIFIES THE NUMBER OF
//				 POINTS IN TABLE (ARG,VAL).
//		EPS    - заданная верхняя граница абсолютной погрешности интерполяции.
//				 SINGLE PRECISION INPUT CONSTANT WHICH IS USED AS
//				 UPPER BOUND FOR THE ABSOLUTE ERROR.
//		n8	   - параметр назначение которого не понятно !!!!!
// Подпрограмма возвращает индикатор ошибки IER (A RESULTING ERROR PARAMETER)
//
// Примечание (REMARKS)
// (1)	TABLE (ARG,VAL) SHOULD REPRESENT A SINGLE-VALUED
//		FUNCTION AND SHOULD BE STORED IN SUCH A WAY, THAT THE
//		DISTANCES ABS(ARG(I)-X) INCREASE WITH INCREASING
//		SUBSCRIPT I. TO GENERATE THIS ORDER IN TABLE (ARG,VAL),
//		SUBROUTINES DATSG, DATSM OR DATSE COULD BE USED IN A
//		PREVIOUS STAGE.
// (2)	NO ACTION BESIDES ERROR MESSAGE IN CASE NDIM LESS THAN 1.
// (3)	INTERPOLATION IS TERMINATED EITHER IF THE DIFFERENCE
//		BETWEEN TWO SUCCESSIVE INTERPOLATED VALUES IS
//		ABSOLUTELY LESS THAN TOLERANCE EPS, OR IF THE ABSOLUTE
//		VALUE OF THIS DIFFERENCE STOPS DIMINISHING, OR AFTER
//		(2*NDIM-2) STEPS. FURTHER IT IS TERMINATED IF THE
//		PROCEDURE DISCOVERS TWO ARGUMENT VALUES IN VECTOR ARG
//		WHICH ARE IDENTICAL. DEPENDENT ON THESE FOUR CASES,
//		ERROR PARAMETER IER IS CODED IN THE FOLLOWING FORM
//		IER=0 - достигается заданная точность, ошибок нет.
//		IT WAS POSSIBLE TO REACH THE REQUIRED
//		ACCURACY (NO ERROR).
//		IER=1 - заданная точность не достигается из-за ошибок округления.
//				IT WAS IMPOSSIBLE TO REACH THE REQUIRED
//				ACCURACY BECAUSE OF ROUNDING ERRORS.
//		IER=2 - заданная точность не достигается или достигаемая
//				точность не может быть проверена, так как мало
//				значение NDIM.
//				IT WAS IMPOSSIBLE TO CHECK ACCURACY BECAUSE
//				NDIM IS LESS THAN 3, OR THE REQUIRED ACCURACY
//				COULD NOT BE REACHED BY MEANS OF THE GIVEN
//				TABLE. NDIM SHOULD BE INCREASED.
//		IER=3 - интерполяция прекращена до достижения заданной
//				точности из-за обнаружения одинаковых значений
//				в массиве аргумента.
//
// THE PROCEDURE DISCOVERED TWO ARGUMENT VALUES
// IN VECTOR ARG WHICH ARE IDENTICAL.
//
// SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED NONE
//
// METHOD
// INTERPOLATION IS DONE BY MEANS OF AITKENS SCHEME OF
// HERMITE INTERPOLATION. ON RETURN Y CONTAINS AN INTERPOLATED
// FUNCTION VALUE AT POINT X, WHICH IS IN THE SENSE OF REMARK
// (3)	OPTIMAL WITH RESPECT TO GIVEN TABLE. FOR REFERENCE, SEE
//		F.B.HILDEBRAND, INTRODUCTION TO NUMERICAL ANALYSIS,
//		MCGRAW-HILL, NEW YORK/TORONTO/LONDON, 1956, PP.314-317, AND
//		GERSHINSKY/LEVINE, AITKEN-HERMITE INTERPOLATION,
//		JACM, VOL.11, ISS.3 (1964), PP.352-356.
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//Тест
//------------------------------------------------------------------------
#define N 15

int ahiTest(void)
{
	double y,x,val[2*N],arg[N];
	int    i,ier,n;
	n=N;
	for(i=0;i<n;i++) {
		arg[i]=i*0.01;
		//y= 3*x*x - 4x + 6;
		val[i*2]=3.0*arg[i]*arg[i]-4.0*arg[i]+6.0;
		//dy/dt= 6*x - 4;
		val[i*2+1]=6.0*arg[i]-4.0;
	}
	x=n*0.01/4.0*3.0;
	//x=0.1125; n=15
	//точное значение          5.58796875
	//тестовый результат  AHI  5.58796875056168 eps=1e-16 ier=1;
	//тестовый результат  AHI  5.58796874999957 eps=1e-10 ier=0;
	y=(3.0*x-4.0)*x+6.0; //точное значение
	//ier=ahi(x,arg,val,&y,n,1e-16);
	ier=ahi(x,arg,val,&y,n,1e-10f,0);
	return ier;
}

//------------------------------------------------------------------------
int ahi(double x, double arg[], double val[], double *y,
		int ndim, float eps, int n8)
{
	int      ier, i, iend, j, k, l, m;
	double   delt1, delt2;
	double   h, h1, h2;

	if (n8<1)  n8 = (int)INF_SUP(2, n8, 8);

	ier = 2;
	h2 = x - arg[0];
	if ((ndim - 1) < 0) goto L_2;
	else
		if ((ndim - 1) == 0) goto L_1;
		else goto L_3;
L_1:	(*y) = val[0] + val[1] * h2;
L_2:	return ier;
		// VECTOR ARG HAS MORE THAN 1 ELEMENT.
		// THE FIRST STEP PREPARES VECTOR VAL SUCH THAT AITKEN SCHEME CAN BE
		// USED. 
L_3:	i = 1;
		for (j = 2; j <= ndim; j++){
			h1 = h2;
			h2 = x - arg[j - 1];
			(*y) = val[i - 1];
			val[i - 1] = (*y) + val[i + 1 - 1] * h1;
			h = h1 - h2;
			if (h == 0.0) goto L_13;
			else goto L_4;
L_4:		val[i + 1 - 1] = (*y) + (val[i + 2 - 1] - (*y))*h1 / h;
// L_5:
			i = i + 2;
		}
		val[i - 1] = val[i - 1] + val[i + 1 - 1] * h2;
		//     END OF FIRST STEP PREPARE AITKEN SCHEME
		delt2 = 0.0;
		iend = i - 1;
		//     START AITKEN-LOOP
		for (i = 1; i <= iend; i++) {
			delt1 = delt2;
			(*y) = val[0];
			m = (i + 3) / 2;
			h1 = arg[m - 1];
			for (j = 1; j <= i; j++){
				k = i + 1 - j;
				l = (k + 1) / 2;
				h = arg[l - 1] - h1;
				if (h == 0.0) goto L_14;
				//  else goto L_6;
				//L_6:
				val[k - 1] = (val[k - 1] * (x - h1) - val[k + 1 - 1] * (x - arg[l - 1])) / h;
			}
			delt2 = (float)fabs((*y) - val[0]);
			if ((delt2 - (double)eps) <= 0.0) goto L_11;
			else goto L_7;
L_7:		if ((j - n8) < 0) goto L_9;
			else goto L_8;
L_8:		if ((delt2 - delt1) < 0.0) goto L_9;
			else goto L_12;
L_9:		;
		}
		//     END OF AITKEN-LOOP
L_10:	(*y) = val[0];
		return ier;
		//     THERE IS SUFFICIENT ACCURACY WITHIN 2*NDIM-2 ITERATION STEPS
L_11:	ier = 0;
		goto L_10;
		//     TEST VALUE DELT2 STARTS OSCILLATING
L_12:	ier = 1;
		return ier;
		//     THERE ARE TWO IDENTICAL ARGUMENT VALUES IN VECTOR ARG 
L_13:	(*y) = val[0];
L_14:	ier = 3;
		return ier;
}

