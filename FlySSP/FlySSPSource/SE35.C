/*------------------------------------------------------------------------
  SE35.C
    СГЛАЖИВАНИЕ ФУНКЦИИ, ЗАДАННОЙ ТАБЛИЦЕЙ ЗНАЧЕНИЙ
    В  РАНООТСТОЯЩИХ  ТОЧКАХ, С  ПОМОЩЬЮ МНОГОЧЛЕНА
    ТРЕТЬЕЙ СТЕПЕНИ, ПОСТРОЕННОГО ПО ПЯТИ ПОСЛЕДОВА-
    ТЕЛЬНЫМ ТОЧКАМ МЕТОДОМ НАИМЕНЬШИХ КВАДРАТОВ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

	Подпрограмма (SUBROUTINE) se35

	Назначение (PURPOSE)
		 Подпрограмма  вычисляет  множество Z1, Z2, ..., Zn
	   значений функции  Y(X), заданной множеством Y1,Y2,...,Yn
	   ее значений в N равноотстоящих точках Xi c шагом Xi-Xi-1
	   (i=2,...,n).
		 По каждым пяти последовательным точкам (Xj-2,Yj-2),
	   (Xj-1,Yj-1), (Xj,Yj)  для j = 5,...,n строится последова-
	   тельность  многочленов третьей степени вида
		   Wj(X)= Aj*X**3 + Bj*X**2 + Cj*X + Dj
	   дающих в этих точках наименьшее отклонение от Y в смысле
	   МНК.
		 Для i=3, ... , n-2, в качестве сглаженного значения
	   функции Zi берется вычисленное в точке Xi значение много-
	   члена, построенного по пяти точкам (Xi+k,Yi+k) (k=-2,...,2),
	   т.е. многочлена Wi+2(X). В качестве сглаженных значений Z1
	   и Z2 берутся вычисленные в точках X1 и X2 значения много-
	   члена W5(X), а в качестве Zn-1 и Zn - значения многочлена
	   Wn(X), вычисленные в точках Xn-1 и Xn.
		 (TO COMPUTE A VECTOR OF SMOOTHED FUNCTION VALUES GIVEN
	   A VECTOR OF FUNCTION VALUES WHOSE ENTRIES CORRESPOND TO
	   EQUIDISTANTLY SPACED ARGUMENT VALUES.)

	Обращение к подпрограмме (USAGE)
	   IER=se35(Y,Z,NDIM);

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
		    IER = -1, если NDIM меньше 5
			     (NDIM IS LESS THAN 5).
	Примечания (REMARKS)
	   (1)   Если IER = -1, то вычисления не проводились.
		 (IF IER=-1 THERE HAS BEEN NO COMPUTATION.)
	   (2)   Массив Z может совпадать с Y. Если Y не совпадает
		 с Z, то он не портятся в процессе вычислений.
		 (Z CAN HAVE THE SAME STORAGE ALLOCATION AS Y.  IF Y
		 IS DISTINCT FROM Z, THEN IT IS NOT DESTROYED.)

	SUBROUTINE AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           IF X IS THE (SUPPRESSED) VECTOR OF ARGUMENT VALUES, THEN
           EXCEPT AT THE POINTS X(1),X(2),X(NDIM-1) AND X(NDIM), EACH
           SMOOTHED VALUE Z(I) IS OBTAINED BY EVALUATING AT X(I) THE
           LEAST-SQUARES POLYNOMIAL OF DEGREE 3 RELEVANT TO THE 5
	   SUCCESSIVE POINTS (X(I+K),Y(I+K)) K = -2,-1,...,2.  (SEE
	   HILDEBRAND, F.B., INTRODUCTION TO NUMERICAL ANALYSIS,
	   MC GRAW-HILL, NEW YORK/TORONTO/LONDON, 1956, PP. 295-302.)

     ..................................................................
 */
int se35(double y[], double z[], int ndim)
{
int  ier,i;
double a, b, c, d;
double const c1=0.014285714285714286,
	     c2=0.057142857142857143,
	     c3=0.08571428571428571;

ier=-1;
/*        TEST OF DIMENSION */
if( (ndim - 5) < 0 )
  return ier; /*        ERROR EXIT IN CASE NDIM IS LESS THAN 5 */

/*        PREPARE LOOP */
b = y[0];      c = y[1];
/*        START LOOP */
for( i = 4; i < ndim; i++ ){
	a = b;
	b = c;
	c = y[i - 2];
	/*        GENERATE FOURTH CENTRAL DIFFERENCE */
	d = c - b - y[i - 1];
	d = d + d + c;
	d = d + d + a + y[i];
	/*        CHECK FIRST TWO COMPONENTS */
	if( i <= 4 )
	  {
	  z[0] = a - c1*d;
	  z[1] = b + c2*d;
	  }
	z[i - 2] = c - c3*d;
	}
/*        END OF LOOP
	UPDATE LAST TWO COMPONENTS */
z[ndim - 2] = y[ndim - 2] + (c2*d);
z[ndim - 1] = y[ndim - 1] - (c1*d);
ier = 0;
return ier;
}
