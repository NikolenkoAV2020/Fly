/*------------------------------------------------------------------------
	  QTFG.C
	  ИНТЕГРИРОВАНИЕ ФУНКЦИИ, ЗАДАННОЙ ТАБЛИЧНО В НЕРАВНООТСТОЯЩИХ
	  ТОЧКАХ, ПО ФОРМУЛЕ ТРАПЕЦИЙ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 26.04.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	ПОДПРОГРАММА QTFG
	SUBROUTINE QTFG

	НАЗНАЧЕНИЕ
	   Подпрограмма вычисляет множество z(1),...,z(n) значений
	   интегралов
				x(i)
				 Ї
		  z(i)=z[x(i)]=  │  y(x) dx
				 ї
			       x(1)
	   от функции y(x), заданной множествами x(1),...,x(n)
	   упорядоченных по возрастанию или убыванию аргумента и
	   соответствующих значений функции.
	PURPOSE
	   TO COMPUTE THE VECTOR OF INTEGRAL VALUES FOR A GIVEN
	   GENERAL TABLE OF ARGUMENT AND FUNCTION VALUES.

	Обращение к подпрограмме
	USAGE
	   qtfg(x,y,z,ndim);

	DESCRIPTION OF PARAMETERS
	   x      - массив длинной ndim заданных значений аргумента.
		    THE INPUT VECTOR OF ARGUMENT VALUES.
	   y      - массив длинной ndim заданных значений функции.
		    THE INPUT VECTOR OF FUNCTION VALUES.
	   z      - массив длинной ndim вычисленных значений интегралов,
		    может совпадать с x или с у.
		    THE RESULTING VECTOR OF INTEGRAL VALUES. z MAY BE
		    IDENTICAL WITH x OR y.
	   ndim   - число точек, в которых задана интегрируемая функция;
		    при ndim<1 происходит выход из программы
		    THE DIMENSION OF VECTORS x,y,z.

	REMARKS
	   NO ACTION IN CASE NDIM LESS THAN 1.

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           BEGINNING WITH Z(1)=0, EVALUATION OF VECTOR Z IS DONE BY
           MEANS OF TRAPEZOIDAL RULE (SECOND ORDER FORMULA).
           FOR REFERENCE, SEE
	   F.B.HILDEBRAND, INTRODUCTION TO NUMERICAL ANALYSIS,
	   MCGRAW-HILL, NEW YORK/TORONTO/LONDON, 1956, PP.75.

     ..................................................................
 */
void  qtfg(double x[], double y[], double z[], int ndim)
{
int     i;
double  sum1, sum2;
sum2 = 0.0;
if(ndim<=0)
  return;
if(ndim==1)
  {
  z[0] = sum2;
  return;
  }

/*     INTEGRATION LOOP */
for( i = 1; i < ndim; i++ ){
	sum1 = sum2;
	sum2 = sum2 + 0.5*(x[i] - x[i - 1])*(y[i] + y[i - 1]);
	z[i - 1] = sum1;
	}
z[ndim-1] = sum2;
return;
}

