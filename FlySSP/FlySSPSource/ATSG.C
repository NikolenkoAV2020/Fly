/*------------------------------------------------------------------------
  ATSG.C
	   ВЫБОРКА ТАБЛИЦЫ ИЗ ТАБЛИЦЫ ОБЩЕГО ВИДА
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

#include "ssp.h"

/*     ..................................................................

	Подпрограмма (SUBROUTINE) atsg

	Назначение (PURPOSE)
		 Подпрограмма по заданному значению аргумента X функции
	   F(X) и заданной  монотонной таблице Fi и Xi ( i=1(1)m )
	   значений этой функции и ее аргумента или таблице Fi, F'i и Xi
	   ( i=1(1)m ) значений этой функции, ee производной и аргумента
	   формирует массив значений  аргумента  Xj  и  массив значений
	   функции F'j (или массивы  значений  функции Fj и значений ее
	   производной F'j) заданного размера n (n<=m), упорядоченные по
	   степени близости значения Xj, к заданному значению X.
		  Формируемые  массивы  упорядочиваются  таким образом,
	   чтобы  модули  разностей  │X-Xj│  возрастали  с  увеличением
	   индекса j.
		  (NDIM POINTS OF A GIVEN GENERAL TABLE ARE SELECTED AND
	   ORDERED SUCH THAT
		     ABS(ARG(I)-X).GE.ABS(ARG(J)-X) IF I.GT.J.)

	Обращение к подпрограмме (USAGE)
	   atsg(X,Z,F,WORK,IROW,ICOL,ARG,VAL,NDIM);

	Описание параметров (DESCRIPTION OF PARAMETERS):
	   X      - заданное значение аргумента (SEARCH ARGUMENT),
	   Z      - массив длины IROW табличных значений аргумента,
		    расположенных в порядке возрастания или убывания
		    аргумента
		    (VECTOR OF ARGUMENT VALUES (DIMENSION IROW).
		    THE ARGUMENT VALUES MUST BE STORED IN INCREASING
		    OR DECREASING SEQUENCE),
	   F      - массив длины IROW заданных значений функции (в случае
		    если ICOL=1) или массив размерноности IROW*2 (в случае
		    если ICOL=2), первый столбец которого заданные значения
		    фанкции, а второй - значения производной.
		      (IN CASE ICOL=1, F IS THE VECTOR OF FUNCTION
		    VALUES (DIMENSION IROW).
		       IN CASE ICOL=2, F IS A DOUBLE PRECISION IROW
		    BY 2 MATRIX. THE FIRST COLUMN SPECIFIES VECTOR
		    OF FUNCTION VALUES AND THE SECOND VECTOR OF
		    DERIVATIVES).
	   WORK   - рабочий массив длиной IROW
		    (WORKING STORAGE (DIMENSION IROW)).
	   IROW   - количество заданных табличных значений функции
		    (THE DIMENSION OF EACH COLUMN IN MATRIX F).
	   ICOL   - число строк в масиве F (при одномерно хранении матрицы)
		    (THE NUMBER OF COLUMNS IN F (I.E. 1 OR 2)).
	   ARG    - формируемый подпрограммой массив длины NDIM
		    упорядоченных значений аргумента
		    (RESULTING VECTOR OF SELECTED AND ORDERED
		    ARGUMENT VALUES (DIMENSION NDIM).
	   VAL    - формируемый подпрограммой массив длины NDIM (ICOL=1)
		    значений функции или NDIM*2 (ICOL=2) значений функции
		    и ее производной.
		      Значения функции и ее производной располагаются
		    парми, каждое значение функции непосредственно
		    предшествует соответствующему значению производной.
		      (RESULTING VECTOR OF SELECTED FUNCTION VALUES
		    (DIMENSION NDIM) IN CASE ICOL=1.
		       IN CASE ICOL=2, VAL IS THE VECTOR OF FUNCTION
		    AND DERIVATIVE VALUES (DIMENSION 2*NDIM) WHICH ARE
		    STORED IN PAIRS (I.E. EACH FUNCTION VALUE IS FOLLOWED
		    BY ITS DERIVATIVE VALUE).
	   NDIM   - заданное число элементов, выбираемых при упорядочивании
		    заданных массивов Z и F.
		     (THE NUMBER OF POINTS WHICH MUST BE SELECTED OUT OF
		    THE GIVEN TABLE).

	Примечания (REMARKS)
	   Нельзя задавать IROW меньше чем 1
	   (NO ACTION IN CASE IROW LESS THAN 1).

	   IF INPUT VALUE NDIM IS GREATER THAN IROW, THE PROGRAM
	   SELECTS ONLY A MAXIMUM TABLE OF IROW POINTS.  THEREFORE THE
	   USER OUGHT TO CHECK CORRESPONDENCE BETWEEN TABLE (ARG,VAL)
	   AND ITS DIMENSION BY COMPARISON OF NDIM AND IROW, IN ORDER
	   TO GET CORRECT RESULTS IN FURTHER WORK WITH TABLE (ARG,VAL).
	   THIS TEST MAY BE DONE BEFORE OR AFTER CALLING

	   Подпрограмма (SUBROUTINE) ATSG.
		  Подпрограмма  может применяться для упорядочения мас-
	   сивов, используемых подпрограммами ALI, AHI, ASFI.
		  (SUBROUTINE ATSG ESPECIALLY CAN BE USED FOR GENERATING
	   THE TABLE (ARG,VAL) NEEDED IN SUBROUTINES ALI, AHI, AND ACFI.)

	METHOD
           SELECTION IS DONE BY GENERATING THE VECTOR WORK WITH
           COMPONENTS WORK(I)=ABS(Z(I)-X) AND AT EACH OF THE NDIM STEPS
           (OR IROW STEPS IF NDIM IS GREATER THAN IROW)
           SEARCHING FOR THE SUBSCRIPT OF THE SMALLEST COMPONENT, WHICH
           IS AFTERWARDS REPLACED BY A NUMBER GREATER THAN
           MAX(WORK(I)).

     ..................................................................
 */
void atsg(double x, double z[], double f[],
	  double work[], int irow, int icol,
	  double arg[], double val[], int ndim)
{
int    i, ii, iii, j, n;
double b, delta;

switch( for_if(irow) ){
	case -1: goto L_11;
	case  0: goto L_11;
	case  1: goto L_1;
	}
L_1:
n = ndim;
/*     IF N IS GREATER THAN IROW, N IS SET EQUAL TO IROW. */
switch( for_if(n - irow) ){
	case -1: goto L_3;
	case  0: goto L_3;
	case  1: goto L_2;
	}
L_2:
n = irow;

/*     GENERATION OF VECTOR WORK AND COMPUTATION OF ITS GREATEST ELEMENT. */
L_3:
b = 0.e0;
for( i = 1; i <= irow; i++ ){
	delta = fabs( z[i-1] - x );
	switch( for_if(delta - b) ){
		case -1: goto L_5;
		case  0: goto L_5;
		case  1: goto L_4;
		}
L_4:
	b = delta;
L_5:
	work[i-1] = delta;
	}

/*     GENERATION OF TABLE (ARG,VAL) */
b = b + 1.e0;
for( j = 1; j <= n; j++ ){
	delta = b;
	for( i = 1; i <= irow; i++ ){
		switch( for_if(work[i-1] - delta) ){
			case -1: goto L_6;
			case  0: goto L_7;
			case  1: goto L_7;
			}
L_6:
		ii = i;
		delta = work[i-1];
L_7:
		;
		}
	arg[j-1] = z[ii-1];
	switch( for_if(icol - 1) ){
		case -1: goto L_8;
		case  0: goto L_9;
		case  1: goto L_8;
		}
L_8:
	val[2*j - 1-1] = f[ii-1];
	iii = ii + irow;
	val[2*j-1] = f[iii-1];
	goto L_10;
L_9:
	val[j-1] = f[ii-1];
L_10:
	work[ii-1] = b;
	}
L_11:
return;
}

