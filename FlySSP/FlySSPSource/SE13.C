/*------------------------------------------------------------------------
  SE13.C
    СГЛАЖИВАНИЕ ФУНКЦИИ, ЗАДАННОЙ ТАБЛИЦЕЙ ЗНАЧЕНИЙ
    В  РАНООТСТОЯЩИХ  ТОЧКАХ, С  ПОМОЩЬЮ МНОГОЧЛЕНА
    ПЕРВОЙ СТЕПЕНИ, ПОСТРОЕННОГО ПО ТРЕМ ПОСЛЕДОВА-
    ТЕЛЬНЫМ ТОЧКАМ МЕТОДОМ НАИМЕНЬШИХ КВАДРАТОВ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

	Подпрограмма (SUBROUTINE) se13

	Назначение (PURPOSE)
		 Подпрограмма  вычисляет  множество Z1, Z2, ..., Zn
	   значений функции  Y(X), заданной множеством Y1,Y2,...,Yn
	   ее значений в N равноотстоящих точках Xi c шагом Xi-Xi-1
	   (i=2,...,n).
		 По каждым трем последовательным точкам (Xj-2,Yj-2),
	   (Xj-1,Yj-1), (Xj,Yj)  для j = 3,...,n строится последова-
	   тельность  многочленов первой степени вида
				 Wj(X)= Aj*X +Bj
	   дающих в этих точках наименьшее отклонение от Y в смысле
	   МНК.
		 Для i=2,...,n-1, то есть в каждой точке Xi за исключением
	   конечных точек X1 и Xn, в качестве сглаженного значения функции
	   Zi берется вычисленное в точке Xi значение многочлена Wi+1(X),
	   в качестве Z1 - вычисленное в точке X1 значение многочлена W3(X),
	   а в качестве Zn - значение многочлена Wn(X), вычисленное при X=Xn.
		 (TO COMPUTE A VECTOR OF SMOOTHED FUNCTION VALUES
	   GIVEN A VECTOR OF FUNCTION VALUES WHOSE ENTRIES CORRESPOND
	   TO EQUIDISTANTLY SPACED ARGUMENT VALUES.)

	Обращение к подпрограмме (USAGE)
	   IER=se13(Y,Z,NDIM);

	Описание параметров (DESCRIPTION OF PARAMETERS):
	   Y     -  массив длины NDIM заданных значений функции
		    (GIVEN VECTOR OF FUNCTION VALUES (DIMENSION NDIM),
	   Z     -  массив длины NDIM вычисляемых сглаженных значений
		    функции
		    (RESULTING VECTOR OF SMOOTHED FUNCTION VALUES
		    (DIMENSION NDIM),
	   NDIM  -  количество заданных значений функции и аргумента
		    (DIMENSION OF VECTORS Y AND Z)
	   Функция возвращает индикатор ошибки (RESULTING ERROR PARAMETER)
	IER, принимающий одно из следующих значений:
		    IER =  0, если ошибок нет (NO ERROR);
		    IER = -1, если NDIM меньше 3
			     (NDIM IS LESS THAN 3).

	Примечания (REMARKS)
	   (1)   Если IER = -1, то вычисления не проводились.
		 (IF IER=-1 THERE HAS BEEN NO COMPUTATION.)
	   (2)   Массив Z может совпадать с Y. Если Y не совпадает
		 с Z, то он не портятся в процессе вычислений.
		 (Z CAN HAVE THE SAME STORAGE ALLOCATION AS Y.  IF Y
		 IS DISTINCT FROM Z, THEN IT IS NOT DESTROYED.)

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
int se13(double y[], double z[], int ndim)
{
int   i, ier;
double  a, b, c;
double const c5=5.0,     /* 5.0 */
	     ci6=1.0/6.0,/* 0.1666667 */
	     ci3=1.0/3.0;/* 0.3333333 */
/*        TEST OF DIMENSION */
ier = -1;
if( (ndim - 3) < 0 )
  return ier; /*        ERROR EXIT IN CASE NDIM IS LESS THAN 3 */

/*        PREPARE LOOP */
b = ci6*(c5*y[0] + y[1] + y[1] - y[2]);
c = ci6*(c5*y[ndim-1] + y[ndim - 2] + y[ndim - 2] - y[ndim - 3]);
/*        START LOOP */
for( i = 2; i < ndim; i++ ){
	a = b;
	b = ci3 *(y[i - 2] + y[i - 1] + y[i]);
	z[i - 2] = a;
	}
/*        END OF LOOP
	UPDATE LAST TWO COMPONENTS */
z[ndim - 2] = b;
z[ndim - 1] = c;
ier = 0;
return ier;
}

