/*------------------------------------------------------------------------
	  QSF.C
	  ИНТЕГРИРОВАНИЕ ФУНКЦИИ, ЗАДАННОЙ ТАБЛИЦЕЙ ЗНАЧЕНИЙ
	  В РАВНООТСТОЯЩИХ ТОЧКАХ, ПО ФОРМУЛАМ НЬЮТОНА-КОТЕСА
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 26.04.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	ПОДПРОГРАММА qsf
	SUBROUTINE qsf

	НАЗНАЧЕНИЕ
	   Подпрограмма вычисляет множество z(1),...,z(n) значений
	   интегралов
				x(i)
				 Ї
		  z(i)=z[x(i)]=  │  y(x) dx
				 ї
			       x(1)
	   от функции y(x), заданной множествами значений y(1),...,y(n)
	   в равноотостоящих точках x(i)=x(1)+(i-1)*h с шагом аргумента
	   h  по квадратурным формулам Ньютона-Котеса.
	PURPOSE
	   TO COMPUTE THE VECTOR OF INTEGRAL VALUES FOR A GIVEN
	   EQUIDISTANT TABLE OF FUNCTION VALUES.

	Обращение к подпрограмме
	USAGE
	   qsf(h,y,z,ndim);

	DESCRIPTION OF PARAMETERS
	   h      - шаг изменеия аргумента.
		    THE INCREMENT OF ARGUMENT VALUES.
	   y      - массив длинной ndim заданных значений функции.
		    THE INPUT VECTOR OF FUNCTION VALUES.
	   z      - массив длинной ndim вычисленных значений интегралов,
		    может совпадать у.
		    THE RESULTING VECTOR OF INTEGRAL VALUES. Z MAY BE
		    IDENTICAL WITH Y.
	   ndim   - число точек, в которых задана интегрируемая функция;
		    при ndim<3 происходит выход из программы
	   NDIM   - THE DIMENSION OF VECTORS Y AND Z.

	REMARKS
	   NO ACTION IN CASE NDIM LESS THAN 3.

	SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           BEGINNING WITH Z(1)=0, EVALUATION OF VECTOR Z IS DONE BY
           MEANS OF SIMPSONS RULE TOGETHER WITH NEWTONS 3/8 RULE OR A
	   COMBINATION OF THESE TWO RULES. TRUNCATION ERROR IS OF
           ORDER H**5 (I.E. FOURTH ORDER METHOD). ONLY IN CASE NDIM=3
           TRUNCATION ERROR OF Z(2) IS OF ORDER H**4.
           FOR REFERENCE, SEE
           (1) F.B.HILDEBRAND, INTRODUCTION TO NUMERICAL ANALYSIS,
	       MCGRAW-HILL, NEW YORK/TORONTO/LONDON, 1956, PP.71-76.
           (2) R.ZURMUEHL, PRAKTISCHE MATHEMATIK FUER INGENIEURE UND
               PHYSIKER, SPRINGER, BERLIN/GOETTINGEN/HEIDELBERG, 1963,
               PP.214-221.

     ..................................................................
 */
void qsf(double h, double y[], double z[], int ndim)
{
int    i;
double aux, aux1, aux2, ht, sum1, sum2;




//ht = 0.3333333*(h);
ht = h/3.0;
if( (ndim - 5) < 0 )
   goto L_7;
if( (ndim - 5) == 0 )
   goto L_8;
if( (ndim - 5) > 0 )
   goto L_1;

/*     NDIM IS GREATER THAN 5. PREPARATIONS OF INTEGRATION LOOP */
L_1:
sum1 = y[1] + y[1];
sum1 = sum1 + sum1;
sum1 = ht*(y[0] + sum1 + y[2]);
aux1 = y[3] + y[3];
aux1 = aux1 + aux1;
aux1 = sum1 + ht*(y[2] + aux1 + y[4]);
aux2 = ht*(y[0] + 3.875*(y[1] + y[4]) + 2.625*(y[2] + y[3]) + y[5]);
sum2 = y[4] + y[4];
sum2 = sum2 + sum2;
sum2 = aux2 - ht*(y[3] + sum2 + y[5]);
z[0] = 0.;
aux = y[2] + y[2];
aux = aux + aux;
z[1] = sum2 - ht*(y[1] + aux + y[3]);
z[2] = sum1;
z[3] = sum2;
if( (ndim - 6) <= 0 )
   goto L_5;
if( (ndim - 6) > 0 )
   goto L_2;
/*     INTEGRATION LOOP */
L_2:
for( i = 7; i <= ndim; i += 2 ){
	sum1 = aux1;
	sum2 = aux2;
	aux1 = y[i - 1-1] + y[i - 1-1];
	aux1 = aux1 + aux1;
	aux1 = sum1 + ht*(y[i - 2-1] + aux1 + y[i-1]);
	z[i - 2-1] = sum1;
	if( (i-ndim) < 0)
	   goto L_3;
	   else goto L_6;
L_3:
	aux2 = y[i-1] + y[i-1];
	aux2 = aux2 + aux2;
	aux2 = sum2 + ht*(y[i - 1-1] + aux2 + y[i + 1-1]);
	z[i - 1-1] = sum2;
	}
L_5:
z[ndim - 1-1] = aux1;
z[ndim-1] = aux2;
return;
L_6:
z[ndim - 1-1] = sum2;
z[ndim-1] = aux1;
return;
/*     END OF INTEGRATION LOOP */
L_7:
if( (ndim - 3) < 0 )
   goto L_12;
if( (ndim - 3) == 0 )
   goto L_11;
if( (ndim - 3) > 0 )
   goto L_8;
/*     NDIM IS EQUAL TO 4 OR 5 */
L_8:
sum2 = 1.125*ht*(y[0] + y[1] + y[1] + y[1] + y[2] + y[2] + y[2] +
 y[3]);
sum1 = y[1] + y[1];
sum1 = sum1 + sum1;
sum1 = ht*(y[0] + sum1 + y[2]);
z[0] = 0.;
aux1 = y[2] + y[2];
aux1 = aux1 + aux1;
z[1] = sum2 - ht*(y[1] + aux1 + y[3]);
if( (ndim - 5) < 0 )
   goto L_10;
if( (ndim - 5) >= 0 )
   goto L_9;
L_9:
aux1 = y[3] + y[3];
aux1 = aux1 + aux1;
z[4] = sum1 + ht*(y[2] + aux1 + y[4]);
L_10:
z[2] = sum1;
z[3] = sum2;
return;

/*     NDIM IS EQUAL TO 3 */
L_11:
sum1 = ht*(1.25*y[0] + y[1] + y[1] - .25*y[2]);
sum2 = y[1] + y[1];
sum2 = sum2 + sum2;
z[2] = ht*(y[0] + sum2 + y[2]);
z[0] = 0.;
z[1] = sum1;
L_12:
return;
} 

