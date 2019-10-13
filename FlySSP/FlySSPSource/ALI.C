//------------------------------------------------------------------------
// ALI.C
// ИНТЕРПОЛИРОВАНИЕ ФУНКЦИИ С ПОМОЩЬЮ ИНТЕРПОЛЯЦИОННОГО ПРОЦЕССА
// ЭЙТКЕНА-ЛАГРАНЖА
// Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
// Версия  : 01.00 / 10.08.1997 /
//------------------------------------------------------------------------
#include <math.h>
#include "ssp.h"

double INF_SUP(const double X1, const double X2, const double X3);

//------------------------------------------------------------------------
// Подпрограмма (SUBROUTINE) ALI
// 
// Назначение(PURPOSE)
// Вычисляет значение Y однозначной функции Y=Y(X) для заданнного
// значения X ее аргумента по таблице Xi, Yi (i=1,2,...,N)
// соответствующих значений аргумента и функции.
// TO INTERPOLATE FUNCTION VALUE Y FOR A GIVEN ARGUMENT VALUE
// X USING A GIVEN TABLE (ARG,VAL) OF ARGUMENT AND FUNCTION
// VALUES.
// 
// Обращение к подпрограмме (USAGE)
// IER=ali(X,ARG,VAL,Y,NDIM,EPS);
// 
// Входные и выходные параметры (DESCRIPTION OF PARAMETERS)
// X      - заданное значение аргумента, для которого вычисляется
// интерполированное значение функции,
// DOUBLE PRECISION ARGUMENT VALUE SPECIFIED BY INPUT.
// ARG    - массив длиной NDIM заданных табличных значений
// аргумента. В процессе вычислений этот массив не
// портится.
// DOUBLE PRECISION INPUT VECTOR (DIMENSION NDIM) OF
// ARGUMENT VALUES OF THE TABLE (NOT DESTROYED).
// VAL    - массив длиной NDIM заданных табличных значений
// функции. В процессе вычислений этот массив портится.
// DOUBLE PRECISION INPUT VECTOR (DIMENSION NDIM) OF
// FUNCTION VALUES OF THE TABLE (DESTROYED).
// Y      - вычисляемое интерполированное значение функции.
// RESULTING INTERPOLATED DOUBLE PRECISION FUNCTION
// VALUE.
// NDIM   - число таблично заданных значений аргумента и функции;
// если NDIM<1, то осуществляется выход из подпрограммы
// без вычислений.
// AN INPUT VALUE WHICH SPECIFIES THE NUMBER OF
// POINTS IN TABLE (ARG,VAL).
// EPS    - заданная верхняя граница абсолютной погрешности
// интерполяции.
// SINGLE PRECISION INPUT CONSTANT WHICH IS USED AS
// UPPER BOUND FOR THE ABSOLUTE ERROR.
// FOR THE ABSOLUTE ERROR.
// Подпрограмма возвращает индикатор ошибки IER
// (A RESULTING ERROR PARAMETER)
// Примечание (REMARKS)
// (1) TABLE (ARG,VAL) SHOULD REPRESENT A SINGLE-VALUED
// FUNCTION AND SHOULD BE STORED IN SUCH A WAY, THAT THE
// DISTANCES ABS(ARG(I)-X) INCREASE WITH INCREASING
// SUBSCRIPT I. TO GENERATE THIS ORDER IN TABLE (ARG,VAL),
// SUBROUTINES DATSG, DATSM OR DATSE COULD BE USED IN A
// PREVIOUS STAGE.
// (2) NO ACTION BESIDES ERROR MESSAGE IN CASE NDIM LESS
// THAN 1.
// (3) INTERPOLATION IS TERMINATED EITHER IF THE DIFFERENCE
// BETWEEN TWO SUCCESSIVE INTERPOLATED VALUES IS
// ABSOLUTELY LESS THAN TOLERANCE EPS, OR IF THE ABSOLUTE
// VALUE OF THIS DIFFERENCE STOPS DIMINISHING, OR AFTER
// (NDIM-1) STEPS. FURTHER IT IS TERMINATED IF THE
// PROCEDURE DISCOVERS TWO ARGUMENT VALUES IN VECTOR ARG
// WHICH ARE IDENTICAL. DEPENDENT ON THESE FOUR CASES,
// ERROR PARAMETER IER IS CODED IN THE FOLLOWING FORM
// IER=0 - достигается заданная точность, ошибок нет.
// IT WAS POSSIBLE TO REACH THE REQUIRED
// ACCURACY (NO ERROR).
// IER=1 - заданная точность не достигается из-за
// ошибок округления.
// IT WAS IMPOSSIBLE TO REACH THE REQUIRED
// ACCURACY BECAUSE OF ROUNDING ERRORS.
// IER=2 - заданная точность не достигается или достигаемая
// точность не может быть проверена, так как мало
// значение NDIM.
// IT WAS IMPOSSIBLE TO CHECK ACCURACY BECAUSE
// NDIM IS LESS THAN 3, OR THE REQUIRED ACCURACY
// COULD NOT BE REACHED BY MEANS OF THE GIVEN
// TABLE. NDIM SHOULD BE INCREASED.
// IER=3 - интерполяция прекращена до достижения заданной
// точности из-за обнаружения одинаковых значений
// в массиве аргумента.
// THE PROCEDURE DISCOVERED TWO ARGUMENT VALUES
// IN VECTOR ARG WHICH ARE IDENTICAL.
// 
// SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
// NONE
// 
// METHOD
// INTERPOLATION IS DONE BY MEANS OF AITKENS SCHEME OF
// LAGRANGE INTERPOLATION. ON RETURN Y CONTAINS AN INTERPOLATED
// FUNCTION VALUE AT POINT X, WHICH IS IN THE SENSE OF REMARK
// (3) OPTIMAL WITH RESPECT TO GIVEN TABLE. FOR REFERENCE, SEE
// F.B.HILDEBRAND, INTRODUCTION TO NUMERICAL ANALYSIS,
// MCGRAW-HILL, NEW YORK/TORONTO/LONDON, 1956, PP.49-50.
//------------------------------------------------------------------------
// x-заданное значение аргумента для которго вычисляется интерполированное
// значение функции;
// arg-массив длиной ndim заданных табличных значений аргумента; в процессе
// вычислений этот массив не портится;
// val-массив длиной ndim заданных табличных значений функции; в процессе
// вычислений этот массив портится;
// y-вычисляемое интерполированное значение функции;
// ndim-число таблично заданных значений функции;если ndim<1, осуществляется
// выход из подпрограммы без выполнения вычислений;
// eps-заданная верхняя граница абсолютной погрешности интерполяции.
// Подпрограмма возвращает одно из следующих значений
// 0-если достигается заданная при обращении точность интерполяции,
// ошибок нет;
// 1-заданная точность не достигается из-за ошибок округления;
// 2-заданная точность не достигается или достигаемая точность не может
// быть проверена, так как мало значений ndim;
// 3-интерполяция прекращена до достижения заданной точности из-за обна-
// ружения двух одинаковых значений в массиве arg.
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//Тест
int aliTest(void)
{
	double arg[] ={1.3  ,1.4   ,1.5   ,1.6},
		   val[]={0.934,0.9523,0.9661,0.9763},
           val1[]={0.934,0.9523,0.9661,0.9763},
           x=1.43,y;
	int i,ier,n=4;
	//в®з­®Ґ §­ зҐ­ЁҐ          0.9569
	//ЇаҐ¤Ї®« Ј Ґ¬л© аҐ§г«мв в 0.95687155
	//вҐбв®ўл© аҐ§г«мв в  ALI  0.95687155  eps=[1e-16,1e-5]  ier=2
	//вҐбв®ўл© аҐ§г«мв в  ALI  0.95687155  eps=1e-4          ier=0
	ier=ali(x,arg,val,&y,n,1e-8f,0);for(i=0;i<4;val[i]=val1[i],i++);
	ier=ali(x,arg,val,&y,n,1e-4f,0);
	return ier;
}

//------------------------------------------------------------------------
int ali(double x, double arg[], double val[], double *y,
		int ndim, float eps,int n8)
{
	int    ier, i, iend, j, k;
	double delt1, delt2,a,b,c;
	double h;

	if(n8<1)  n8=(int)INF_SUP(2,n8,8);

	ier = 2;
	delt2 = 0.0;
	if( (ndim - 1) < 0) goto L_9;
	else
	if( (ndim - 1) == 0) goto L_7;

	//     START OF AITKEN-LOOP 
	for( j = 2 ; j <= ndim; j++ ){
		delt1 = delt2;
	    iend = j - 1;
		k=j-1;
	    for( i = 0 ; i < iend; i++ ){
			h = arg[i] - arg[j-1];
			if( h == 0.0) goto L_13;
			//a=(val[i]*(x - arg[j-1]) - val[j-1]*(x -arg[i]))/h;
			b=val[i]*(x - arg[k]);
			c=val[k]*(x - arg[i]);
			a=(b - c)/h;
			val[k] =a;
       }
		// delt2 = fabs( val[j-1] - val[iend-1] );
		delt2 = fabs( val[k] - val[iend-1] );
		if ((j - 2) <= 0) continue;
		if ((delt2-(double)eps)<=0.0) goto L_10;
		if (j >= n8) {
			if ((delt2 - delt1) < 0.0) continue;
			else goto L_11;
		}
    }
	//     END OF AITKEN-LOOP
L_7:
	j = ndim;
L_8:
	(*y) = val[j-1];
L_9:
	return ier;
	//     THERE IS SUFFICIENT ACCURACY WITHIN NDIM-1 ITERATION STEPS
L_10:
	ier = 0;
	goto L_8;
	//    TEST VALUE DELT2 STARTS OSCILLATING 
L_11:
	ier = 1;
L_12:
	j = iend;
	goto L_8;
	//    THERE ARE TWO IDENTICAL ARGUMENT VALUES IN VECTOR ARG 
L_13:
	ier = 3;
	goto L_12;
}

