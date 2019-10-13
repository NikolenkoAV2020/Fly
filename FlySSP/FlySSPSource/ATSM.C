/*------------------------------------------------------------------------
  ATSM.C
	   ВЫБОРКА ТАБЛИЦЫ ИЗ ТАБЛИЦЫ С МОНОТОННЫМИ ЗНАЧЕНИЯМИ
	   АРГУМЕНТА
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

#include "ssp.h"

/*     ..................................................................

	Подпрограмма (SUBROUTINE) atsm

	Назначение (PURPOSE)
		 Подпрограмма по заданному значению  аргумента X функции
	   Y(X)  и  заданной  монотонной  таблице  Yi и  Xi  ( i=1(1)m )
	   значений этой функции и ее аргумента или таблице Yi, Y'i и Xi
	   ( i=1(1)m ) значений этой функции, ee производной и аргумента
	   формирует массив  значений  аргумента  Xj  и  массив значений
	   функции Yj (или   массивы  значений  функции Yj и значений ее
	   производной Y'j) заданного размера n (n<=m), упорядоченные по
	   степени близости значения Xj, к заданному значению X.
		  Формируемые  массивы  упорядочиваются  таким образом,
	   чтобы  модули  разностей  │X-Xj│  возрастали  с  увеличением
	   индекса j.
		 (NDIM POINTS OF A GIVEN TABLE WITH MONOTONIC ARGUMENTS
	   ARE SELECTED AND ORDERED SUCH THAT
		   ABS(ARG(I)-X).GE.ABS(ARG(J)-X) IF I.GT.J.)

	Обращение к подпрограмме (USAGE)
	   atsm(X,Z,F,IROW,ICOL,ARG,VAL,NDIM);

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

	   Подпрограмма (SUBROUTINE) ATSM.
		  Подпрограмма  может применяться для упорядочения мас-
	   сивов, используемых подпрограммами ALI, AHI, ASFI.
		  (SUBROUTINE DATSM ESPECIALLY CAN BE USED FOR GENERATING
	   THE TABLE (ARG,VAL) NEEDED IN SUBROUTINES DALI, AHI, AND ACFI).


        METHOD
           SELECTION IS DONE BY SEARCHING THE SUBSCRIPT J OF THAT
           ARGUMENT, WHICH IS NEXT TO X (BINARY SEARCH).
           AFTERWARDS NEIGHBOURING ARGUMENT VALUES ARE TESTED AND
           SELECTED IN THE ABOVE SENSE.

     ..................................................................
 */
void atsm(double x, double z[], double f[],
	  int irow, int icol,
	  double arg[], double val[], int ndim)
{
int  i, j, jjl, jjr, jl, jr, k, kk, n;




/*     CASE IROW=1 IS CHECKED OUT */
switch( for_if(irow - 1) ){
	case -1: goto L_23;
	case  0: goto L_21;
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

/*     CASE IROW.GE.2
     SEARCHING FOR SUBSCRIPT J SUCH THAT Z(J) IS NEXT TO X. */
L_3:
switch( for_if(z[irow-1] - z[0]) ){
	case -1: goto L_5;
	case  0: goto L_4;
	case  1: goto L_4;
	}
L_4:
j = irow;
i = 1;
goto L_6;
L_5:
i = irow;
j = 1;
L_6:
k = (j + i)/2;
switch( for_if(x - z[k-1]) ){
	case -1: goto L_7;
	case  0: goto L_7;
	case  1: goto L_8;
	}
L_7:
j = k;
goto L_9;
L_8:
i = k;
L_9:
switch( for_if(abs( j - i ) - 1) ){
	case -1: goto L_10;
	case  0: goto L_10;
	case  1: goto L_6;
	}
L_10:
switch( for_if(fabs( z[j-1] - x ) - fabs( z[i-1] - x )) ){
	case -1: goto L_12;
	case  0: goto L_12;
	case  1: goto L_11;
	}
L_11:
j = i;

/*     TABLE SELECTION */
L_12:
k = j; jl = 0; jr = 0;
for( i = 1; i <= n; i++ ){
	arg[i-1] = z[k-1];
	switch( for_if(icol - 1) ){
		case -1: goto L_14;
		case  0: goto L_14;
		case  1: goto L_13;
		}
L_13:
	val[2*i - 1-1] = f[k-1];
	kk = k + irow;
	val[2*i-1] = f[kk-1];
	goto L_15;
L_14:
	val[i-1] = f[k-1];
L_15:
	jjr = j + jr;
	switch( for_if(jjr - irow) ){
		case -1: goto L_16;
		case  0: goto L_18;
		case  1: goto L_18;
		}
L_16:
	jjl = j - jl;
	switch( for_if(jjl - 1) ){
		case -1: goto L_19;
		case  0: goto L_19;
		case  1: goto L_17;
		}
L_17:
	switch( for_if(fabs( z[jjr + 1-1] - x ) - fabs( z[jjl - 1-1]
	  - x )) ){
		case -1: goto L_19;
		case  0: goto L_19;
		case  1: goto L_18;
		}
L_18:
	jl = jl + 1;
	k = j - jl;
	goto L_20;
L_19:
	jr = jr + 1;
	k = j + jr;
L_20:
	;
	}
return;

/*     CASE IROW=1 */
L_21:
arg[0] = z[0];
val[0] = f[0];
switch( for_if(icol - 2) ){
	case -1: goto L_23;
	case  0: goto L_22;
	case  1: goto L_23;
	}
L_22:
val[1] = f[1];
L_23:
return;
}

