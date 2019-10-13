/*------------------------------------------------------------------------
  SG13.C
    СГЛАЖИВАНИЕ ФУНКЦИИ, ЗАДАННОЙ ТАБЛИЦЕЙ ЗНАЧЕНИЙ
    В НЕРАНООТСТОЯЩИХ ТОЧКАХ,  С ПОМОЩЬЮ МНОГОЧЛЕНА
    ПЕРВОЙ СТЕПЕНИ, ПОСТРОЕННОГО ПО ТРЕМ ПОСЛЕДОВА-
    ТЕЛЬНЫМ ТОЧКАМ МЕТОДОМ НАИМЕНЬШИХ КВАДРАТОВ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"
/*     ..................................................................

	Подпрограмма (SUBROUTINE) sg13

	Назначение (PURPOSE)
		 Подпрограмма  вычисляет  множество Z1, Z2, ..., Zn
	   значений функции Y(X), заданной множествами X1,X2,...,Xn
	   значений  аргумента  и  Y1, Y2, ..., Yn  соответствующих
	   значений функции.
		 По каждым трем последовательным точкам (Xj-2,Yj-2),
	   (Xj-1,Yj-1), (Xj,Yj) для j = 3,...,n строится последова-
	   тельность многочленов первой степени вида
				 Wj(X)= Aj*X +Bj
	   дающих в этих точках наименьшее отклонение от Y в смысле
	   МНК.
		 Для i = 2, ... , n-1, то есть в каждой точке Xi за
	   исключением конечных  точек X1 и Xn, в качестве сглажен-
	   ного значения функции  Zi берется вычисленное в точке Xi
	   значение многочлена Wi+1(X), в качестве Z1 - вычисленное
	   в точке X1 значение многочлена W3(X), а  в качестве Zn -
	   значение многочлена Wn(X), вычисленное при X=Xn.
	   (TO COMPUTE A VECTOR OF SMOOTHED FUNCTION VALUES GIVEN
	   VECTORS  OF ARGUMENT VALUES AND CORRESPONDING FUNCTION
	   VALUES.)

	Обращение к подпрограмме (USAGE)
	   IER=sg13(X,Y,Z,NDIM);

	Описание параметров (DESCRIPTION OF PARAMETERS):
	   X     -  массив длины NDIM заданных значений аргумента
		    (GIVEN VECTOR OF ARGUMENT VALUES (DIMENSION NDIM),
	   Y     -  массив длины NDIM заданных значений аргумента
		    (GIVEN VECTOR OF FUNCTION VALUES CORRESPONDING TO X
		    (DIMENSION NDIM),
	   Z     -  массив длины NDIM вычисляемых сглаженных значений
		    функции
		    (RESULTING VECTOR OF SMOOTHED FUNCTION VALUES
		    (DIMENSION NDIM),
	   NDIM  -  количество заданных значений функции и аргумента
		    (DIMENSION OF VECTORS X, Y AND Z).
	   Функция возвращает индикатор ошибки (RESULTING ERROR PARAMETER)
	IER, принимающий одно из следующих значений:
		    IER =  0, если ошибок нет (NO ERROR);
		    IER = -1, если NDIM меньше 3
			     (NDIM IS LESS THAN 3).

	Примечания (REMARKS)
	   (1)   Если IER = -1, то вычисления не проводились.
		 (IF IER=-1 THERE HAS BEEN NO COMPUTATION.)
	   (2)   Массив Z может совпадать с X или Y. Если X
		 или Y не совпадает с Z, то они не портятся
		 в процессе вычислений.
		 (Z CAN HAVE THE SAME STORAGE ALLOCATION AS Y.
		 IF Y IS DISTINCT FROM Z, THEN IT IS NOT
		 DESTROYED.).

	SUBROUTINES AND SUBPROGRAMS REQUIRED
	   NONE

	METHOD
	   IF X IS THE (SUPPRESSED) VECTOR OF ARGUMENT VALUES, THEN
	   EXCEPT AT THE ENDPOINTS X(1) AND X(NDIM), EACH SMOOTHED
	   VALUE Z(I) IS OBTAINED BY EVALUATING AT X(I) THE LEAST-
	   SQUARES POLYNOMIAL OF DEGREE 1 RELEVANT TO THE 3 SUCCESSIVE
	   POINTS (X(I+K),Y(I+K)) K = -1,0,1.  (SEE HILDEBRAND, F.B.,
	   INTRODUCTION TO NUMERICAL ANALYSIS, MC GRAW-HILL, NEW YORK/
	   TORONTO/LONDON, 1956, PP. 295-302.)

     ..................................................................
 */
int sg13(double x[], double y[], double z[], int ndim)
{
int     i, ier;
double  h, t1, t2, t3, xm, ym;
double const ci3=1.0/3.0;/* 0.3333333 */

/*        TEST OF DIMENSION */
ier = -1;
if( (ndim - 3) < 0 )
  return ier; /*        ERROR EXIT IN CASE NDIM IS LESS THAN 3 */

/*        START LOOP */
for( i = 2; i < ndim; i++ ){
	xm = ci3*(x[i - 2] + x[i - 1] + x[i]);
	ym = ci3*(y[i - 2] + y[i - 1] + y[i]);
	t1 = x[i - 2] - xm;
	t2 = x[i - 1] - xm;
	t3 = x[i] - xm;
	xm = t1*t1 + t2*t2 + t3*t3;
	if( xm > 0.0)
	   xm = (t1*(y[i - 2] - ym)
	      +  t2*(y[i - 1] - ym)
	      +  t3*(y[i]     - ym))/xm;
	/*        CHECK FIRST POINT */
	if( i <= 2 )
	   h = xm*t1 + ym;
	z[i - 2] = h;
	h = xm*t2 + ym;
	}
/*        END OF LOOP
	UPDATE LAST TWO COMPONENTS */
z[ndim - 2] = h;
z[ndim - 1] = xm*t3 + ym;
ier = 0;
return ier;
}

