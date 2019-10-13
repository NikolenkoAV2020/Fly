/*------------------------------------------------------------------------
  XCPY.C
     Выбop и зaпиcь пoдмaтpицы из зaдaннoй мaтpицы
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	Подпрограмма (SUBROUTINE) XCPY

	Назначение (PURPOSE)
	   Формирование  матрицы-результата  R путем переписи
	   NR строк и MR столбцов исходной матрицы A, начиная
	   с элемента, находящегося на пересечении L-й строки
	   и K-го столбца
			R(i,j)=A(l+i-1, k+j-1)
	   COPY A PORTION OF A MATRIX

	Обращение к подпрограмме (USAGE)
	   xcpy(A,R,L,K,NR,MR,NA,MA,MS);

	Описание параметров (DESCRIPTION OF PARAMETERS)
	   A  - имя исходной матрицы (NAME OF INPUT MATRIX)
	   R  - имя результирующей матрицы (NAME OF OUTPUT MATRIX)
	   L  - номер строки матрицы А, в которой находится первый
		переписываемый в матрицу R элемент
		ROW OF A WHERE FIRST ELEMENT OF R CAN BE FOUND
	   K  - номер столбца матрицы А, в которой находится первый
		переписываемый в матрицу R элемент
		COLUMN OF A WHERE FIRST ELEMENT OF R CAN BE FOUND
	   NR - число строк, переписываемых в R
		NUMBER OF ROWS TO BE COPIED INTO R
	   MR - число столбцов, переписываемых в R
		NUMBER OF COLUMNS TO BE COPIED INTO R
	   NA - число строк в матрице А
		NUMBER OF ROWS IN A
	   MA - число столбцов в матрице А
		NUMBER OF COLUMNS IN A
	   MS - переменная, указывающая способ хранения матриц
		ONE DIGIT NUMBER FOR STORAGE MODE OF MATRIX A
		  0 - GENERAL
		  1 - SYMMETRIC
		  2 - DIAGONAL

	REMARKS
	   MATRIX R CANNOT BE IN THE SAME LOCATION AS MATRIX A
	   MATRIX R IS ALWAYS A GENERAL MATRIX

	SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   LOC

	METHOD
	   MATRIX R IS FORMED BY COPYING A PORTION OF MATRIX A. THIS
	   IS DONE BY EXTRACTING NR ROWS AND MR COLUMNS OF MATRIX A,
	   STARTING WITH ELEMENT AT ROW L, COLUMN K

     ..................................................................
// Тест
{
static double A[49]={
 0, 1, 2, 3, 4, 5, 6,
10,11,12,13,14,15,16,
20,21,22,23,24,25,26,
30,31,32,33,34,35,36,
40,41,42,43,44,45,46,
50,51,52,53,54,55,56,
60,61,62,63,64,65,66,
},
S[28]={
 0, 1, 2, 3, 4, 5, 6,
   11,12,13,14,15,16,
      22,23,24,25,26,
	 33,34,35,36,
	    44,45,46,
	       55,56,
		  66,
},
R[100],Q[100];
memset(R,'\0',sizeof(double)*100);
xcpy(A, R, 0,0, 3, 3, 7, 7, GENERAL);
memset(R,'\0',sizeof(double)*100);
xcpy(A, R, 1,1, 3, 3, 7, 7, GENERAL);
memset(R,'\0',sizeof(double)*100);
xcpy(A, R, 0,0, 3, 5, 7, 7, GENERAL);
memset(R,'\0',sizeof(double)*100);
xcpy(A, R, 1,1, 3, 4, 7, 7, GENERAL);
memset(R,'\0',sizeof(double)*100);
xcpy(A, R, 2,2, 3, 3, 7, 7, GENERAL);
memset(R,'\0',sizeof(double)*100);
xcpy(A, R, 1,1, 6, 6, 7, 7, GENERAL);

memset(R,'\0',sizeof(double)*100);
xcpy(S, R, 0,0, 3, 3, 7, 7, SYMMETRIC);
mstr(R, Q, 3,GENERAL,SYMMETRIC);
memset(R,'\0',sizeof(double)*100);
xcpy(S, R, 1,1, 3, 3, 7, 7, SYMMETRIC);
mstr(R, Q, 3,GENERAL,SYMMETRIC);
memset(R,'\0',sizeof(double)*100);
xcpy(S, R, 0,0, 3, 5, 7, 7, SYMMETRIC);
memset(R,'\0',sizeof(double)*100);
xcpy(S, R, 1,1, 3, 4, 7, 7, SYMMETRIC);
memset(R,'\0',sizeof(double)*100);
xcpy(S, R, 2,2, 3, 3, 7, 7, SYMMETRIC);
memset(R,'\0',sizeof(double)*100);
xcpy(S, R, 1,1, 6, 6, 7, 7, SYMMETRIC);
mstr(R, Q, 6,GENERAL,SYMMETRIC);

rt[0]=0;
}
*/

void xcpy(double a[], double r[],
	  int l, int k, int nr, int mr, int na, int ma, int ms)
{
int i, ia, ir, j, k2, l2;

/* INITIALIZE  */
ir = 0;
l2 = l + nr - 1;
k2 = k + mr - 1;

for( i = l; i <= l2; i++ ){
  for( j = k; j <= k2; j++ ){
		ir = ir + 1;
		r[ir-1] = 0.0;

		/*        LOCATE ELEMENT FOR ANY MATRIX STORAGE MODE */
		loc(i, j, &ia, na, ma, ms);

		/*        TEST FOR ZERO ELEMENT IN DIAGONAL MATRIX
		switch( for_if(a[ia]) ){
			case -1: goto L_4;
			case  0: goto L_5;
			case  1: goto L_4;
			}
		 */
//L_4:
        r[ir-1] = a[ia];
//L_5:		;
		}
	}
return;
}

