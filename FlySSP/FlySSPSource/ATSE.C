//------------------------------------------------------------------------
// ATSE.C
// ВЫБОРКА ТАБЛИЦЫ ИЗ ТАБЛИЦЫ С РАВНООТСТОЯЩИМИ ЗНАЧЕНИЯМИ
// АРГУМЕНТА
// Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
// Версия  : 01.00 / 18.09.1997 /
//------------------------------------------------------------------------
#include "ssp.h"

//------------------------------------------------------------------------
// Подпрограмма (SUBROUTINE) atse
//
// Назначение (PURPOSE)
// Подпрограмма по заданному значению аргумента X функции
// Y(X), заданному  массивом  F  значений  Yi  этой функции или
// значений Yi и значений ее производной Y'i в n равноотстоящих
// точках Xi ( i=1(1)m ), определенных  по заданному начальному
// значению ZS  аргумента и его приращению DZ, формирует массив
// значений  аргумента  Xj  и  массив значений функции Y'j (или
// массив  значений  функции Y'j и значений ее производной Y'j)
// заданного размера n (n<=m), упорядоченные по степени близости
// значения Xj, к заданному значению X.
// Формируемые  массивы  упорядочиваются  таким образом,
// чтобы  модули  разностей  │X-Xj│  возрастали  с  увеличением
// индекса j.
// (NDIM POINTS OF A GIVEN TABLE WITH EQUIDISTANT ARGUMENTS
// ARE SELECTED AND ORDERED SUCH THAT
// ABS(ARG(I)-X).GE.ABS(ARG(J)-X) IF I.GT.J.)
// 
// Обращение к подпрограмме (USAGE)
// atse(X,ZS,DZ,F,IROW,ICOL,ARG,VAL,NDIM);
// 
// Описание параметров (DESCRIPTION OF PARAMETERS):
// X      - заданное значение аргумента (SEARCH ARGUMENT).
// ZS     - заданное начальное значение аргумента
// (STARTING VALUE OF ARGUMENTS).
// DZ     - заданное приращение аргумента
// (INCREMENT OF ARGUMENT VALUES).
// F      - массив длины IROW заданных значений функции (в случае
// если ICOL=1) или массив размерноности IROW*2 (в случае
// если ICOL=2), первый столбец которого заданные значения
// фанкции, а второй - значения производной.
// (IN CASE ICOL=1, F IS THE VECTOR OF FUNCTION
// VALUES (DIMENSION IROW).
// IN CASE ICOL=2, F IS A DOUBLE PRECISION IROW
// BY 2 MATRIX. THE FIRST COLUMN SPECIFIES VECTOR
// OF FUNCTION VALUES AND THE SECOND VECTOR OF
// DERIVATIVES).
// IROW   - количество заданных табличных значений функции
// (THE DIMENSION OF EACH COLUMN IN MATRIX F).
// ICOL   - число строк в масиве F (при одномерно хранении матрицы)
// (THE NUMBER OF COLUMNS IN F (I.E. 1 OR 2)).
// ARG    - формируемый подпрограммой массив длины NDIM
// упорядоченных значений аргумента
// (RESULTING VECTOR OF SELECTED AND ORDERED
// ARGUMENT VALUES (DIMENSION NDIM).
// VAL    - формируемый подпрограммой массив длины NDIM (ICOL=1)
// значений функции или NDIM*2 (ICOL=2) значений функции
// и ее производной.
// Значения функции и ее производной располагаются
// парми, каждое значение функции непосредственно
// предшествует соответствующему значению производной.
// (RESULTING VECTOR OF SELECTED FUNCTION VALUES
// (DIMENSION NDIM) IN CASE ICOL=1.
// IN CASE ICOL=2, VAL IS THE VECTOR OF FUNCTION
// AND DERIVATIVE VALUES (DIMENSION 2*NDIM) WHICH ARE
// STORED IN PAIRS (I.E. EACH FUNCTION VALUE IS FOLLOWED
// BY ITS DERIVATIVE VALUE).
// NDIM   - заданное число элементов, выбираемых при упорядочивании
// заданных массивов Z и F.
// (THE NUMBER OF POINTS WHICH MUST BE SELECTED OUT OF
// THE GIVEN TABLE).
// 
// Примечание (REMARKS)
// Нельзя задавать IROW меньше чем 1
// (NO ACTION IN CASE IROW LESS THAN 1).
// IF INPUT VALUE NDIM IS GREATER THAN IROW, THE PROGRAM
// SELECTS ONLY A MAXIMUM TABLE OF IROW POINTS.  THEREFORE THE
// USER OUGHT TO CHECK CORRESPONDENCE BETWEEN TABLE (ARG,VAL)
// AND ITS DIMENSION BY COMPARISON OF NDIM AND IROW, IN ORDER
// TO GET CORRECT RESULTS IN FURTHER WORK WITH TABLE (ARG,VAL).
// THIS TEST MAY BE DONE BEFORE OR AFTER CALLING
// 
// Подпрограмма (SUBROUTINE) ATSE.
// Подпрограмма  может применяться для упорядочения мас-
// сивов, используемых подпрограммами ALI, AHI, ASFI.
// (SUBROUTINE ATSE ESPECIALLY CAN BE USED FOR GENERATING
// THE TABLE (ARG,VAL) NEEDED IN SUBROUTINES ALI, AHI AND ACFI).
// 
// SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
// NONE
// 
// METHOD
// SELECTION IS DONE BY COMPUTING THE SUBSCRIPT J OF THAT
// ARGUMENT, WHICH IS NEXT TO X.
// AFTERWARDS NEIGHBOURING ARGUMENT VALUES ARE TESTED AND
// SELECTED IN THE ABOVE SENSE.
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void atse(double x, double zs, double dz,
	  double f[], int irow, int icol,
	  double arg[], double val[], int ndim)
{
int    i, ii, iii, j, jl, jr, n;
double di;

if( irow < 1)
  return;
if(  irow == 1 )
  goto L_17;

/*     CASE DZ=0 IS CHECKED OUT */
if(dz==0.0)
  goto L_17;
n = ndim;
/*     IF N IS GREATER THAN IROW, N IS SET EQUAL TO IROW. */
switch( for_if(n - irow) ){
	case -1: goto L_4;
	case  0: goto L_4;
	case  1: goto L_3;
	}
L_3:
n = irow;

/*     COMPUTATION OF STARTING SUBSCRIPT J. */
L_4:
j = (int)((x - zs)/dz + 1.5e0);
switch( for_if(j) ){
	case -1: goto L_5;
	case  0: goto L_5;
	case  1: goto L_6;
	}
L_5:
j = 1;
L_6:
switch( for_if(j - irow) ){
	case -1: goto L_8;
	case  0: goto L_8;
	case  1: goto L_7;
	}
L_7:
j = irow;

/*     GENERATION OF TABLE ARG,VAL IN CASE DZ.NE.0. */
L_8:
ii = j; jl = 0; jr = 0;
for( i = 1; i <= n; i++ ){
	di=(double)(ii - 1);
	arg[i-1] = zs +  di*dz;
	switch( for_if(icol - 2) ){
		case -1: goto L_9;
		case  0: goto L_10;
		case  1: goto L_10;
		}
L_9:
	val[i-1] = f[ii-1];
	goto L_11;
L_10:
	val[2*i - 1-1] = f[ii-1];
	iii = ii + irow;
	val[2*i-1] = f[iii-1];
L_11:
	switch( for_if(j + jr - irow) ){
		case -1: goto L_12;
		case  0: goto L_15;
		case  1: goto L_12;
		}
L_12:
	switch( for_if(j - jl - 1) ){
		case -1: goto L_13;
		case  0: goto L_14;
		case  1: goto L_13;
		}
L_13:
	switch( for_if((arg[i-1] - x)*dz) ){
		case -1: goto L_14;
		case  0: goto L_15;
		case  1: goto L_15;
		}
L_14:
	jr = jr + 1;
	ii = j + jr;
	goto L_16;
L_15:
	jl = jl + 1;
	ii = j - jl;
L_16:
	;
	}
return;

/*     CASE DZ=0 */
L_17:
arg[0] = zs;
val[0] = f[0];
if(icol == 2)
  val[1] = f[1];
return;
}

//	  В  качестве  начального  значения  ZS  аргумента  при
//	  обращении  к подпрограмме должно быть задано  ближайшее  к X
//	  значение Xi.
