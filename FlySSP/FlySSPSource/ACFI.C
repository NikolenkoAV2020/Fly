//------------------------------------------------------------------------
// ACFI.C
// ИНТЕРПОЛИРОВАНИЕ ФУНКЦИИ С ПОМОЩЬЮ НЕПРЕРЫВНОЙ ДРОБИ
// Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
// Версия  : 01.00 / 10.08.1997 /
//------------------------------------------------------------------------
#include <math.h>
#include "ssp.h"

//------------------------------------------------------------------------
// SUBROUTINE ACFI
//
// PURPOSE
// TO INTERPOLATE FUNCTION VALUE Y FOR A GIVEN ARGUMENT VALUE
// X USING A GIVEN TABLE (ARG,VAL) OF ARGUMENT AND FUNCTION
// VALUES.
//
// USAGE
// acfi(X,ARG,VAL,Y,NDIM,EPS,IER)
//
// DESCRIPTION OF PARAMETERS
//		X      - DOUBLE PRECISION ARGUMENT VALUE SPECIFIED BY INPUT.
//		ARG    - DOUBLE PRECISION INPUT VECTOR (DIMENSION NDIM) OF
//				 ARGUMENT VALUES OF THE TABLE (POSSIBLY DESTROYED).
//		VAL    - DOUBLE PRECISION INPUT VECTOR (DIMENSION NDIM) OF
//				 FUNCTION VALUES OF THE TABLE (DESTROYED).
//		Y      - RESULTING INTERPOLATED DOUBLE PRECISION FUNCTION
//				 VALUE.
//		NDIM   - AN INPUT VALUE WHICH SPECIFIES THE NUMBER OF
//				 POINTS IN TABLE (ARG,VAL).
//		EPS    - SINGLE PRECISION INPUT CONSTANT WHICH IS USED AS
//				 UPPER BOUND FOR THE ABSOLUTE ERROR.
//		IER    - A RESULTING ERROR PARAMETER.
//
// Примечание REMARKS
// (1)	TABLE (ARG,VAL) SHOULD REPRESENT A SINGLE-VALUED
//		FUNCTION AND SHOULD BE STORED IN SUCH A WAY, THAT THE
//		DISTANCES ABS(ARG(I)-X) INCREASE WITH INCREASING
//		SUBSCRIPT I. TO GENERATE THIS ORDER IN TABLE (ARG,VAL),
//		SUBROUTINES DATSG, DATSM OR DATSE COULD BE USED IN A
//		PREVIOUS STAGE.
// (2)	NO ACTION BESIDES ERROR MESSAGE IN CASE NDIM LESS
//		THAN 1.
// (3)	INTERPOLATION IS TERMINATED EITHER IF THE DIFFERENCE
//		BETWEEN TWO SUCCESSIVE INTERPOLATED VALUES IS
//		ABSOLUTELY LESS THAN TOLERANCE EPS, OR IF THE ABSOLUTE
//		VALUE OF THIS DIFFERENCE STOPS DIMINISHING, OR AFTER
//		(NDIM-1) STEPS (THE NUMBER OF POSSIBLE STEPS IS
//		DIMINISHED IF AT ANY STAGE INFINITY ELEMENT APPEARS IN
//		THE DOWNWARD DIAGONAL OF INVERTED-DIFFERENCES-SCHEME
//		AND IF IT IS IMPOSSIBLE TO ELIMINATE THIS INFINITY
//		ELEMENT BY INTERCHANGING OF TABLE POINTS).
//		FURTHER IT IS TERMINATED IF THE PROCEDURE DISCOVERS TWO
//		ARGUMENT VALUES IN VECTOR ARG WHICH ARE IDENTICAL.
//		DEPENDENT ON THESE FOUR CASES, ERROR PARAMETER IER IS
//		CODED IN THE FOLLOWING FORM
//		IER=0 - IT WAS POSSIBLE TO REACH THE REQUIRED
//		ACCURACY (NO ERROR).
//		IER=1 - IT WAS IMPOSSIBLE TO REACH THE REQUIRED
//		ACCURACY BECAUSE OF ROUNDING ERRORS.
//		IER=2 - IT WAS IMPOSSIBLE TO CHECK ACCURACY BECAUSE
//		NDIM IS LESS THAN 2, OR THE REQUIRED ACCURACY
//		COULD NOT BE REACHED BY MEANS OF THE GIVEN
//		TABLE. NDIM SHOULD BE INCREASED.
//		IER=3 - THE PROCEDURE DISCOVERED TWO ARGUMENT VALUES
//		IN VECTOR ARG WHICH ARE IDENTICAL.
//		SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED NONE
//		METHOD
//		INTERPOLATION IS DONE BY CONTINUED FRACTIONS AND INVERTED-
//		DIFFERENCES-SCHEME. ON RETURN Y CONTAINS AN INTERPOLATED
//		FUNCTION VALUE AT POINT X, WHICH IS IN THE SENSE OF REMARK
// (3)	OPTIMAL WITH RESPECT TO GIVEN TABLE. FOR REFERENCE, SEE
//		F.B.HILDEBRAND, INTRODUCTION TO NUMERICAL ANALYSIS,
//		MCGRAW-HILL, NEW YORK/TORONTO/LONDON, 1956, PP.395-406.
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//Тест
int acfiTest(void)
{
	double arg[]={1.3  ,1.4   ,1.5   ,1.6},
		   val[]={0.934,0.9523,0.9661,0.9763},
		   x=1.43,y;
	int	   ier, n=4;
	//точное значение          0.9569
	//предполагаемый результат 0.95687155
	//тестовый результат  ACFI 0.956866315789473 eps=1e-8 ier=2
	//тестовый результат  ACFI 0.956866315789473 eps=1e-4 ier=0
	ier=acfi(x, arg, val, &y, n, 1.0e-8f);
	ier=acfi(x, arg, val, &y, n, 1.0e-4f);
	return ier;
}

//------------------------------------------------------------------------
int acfi(double x, double arg[], double val[], double *y, 
		 int ndim, float eps)
{
	int    ier, i, ii, iii, j, jend;
	float  delt1, delt2;
	double aux, h, p1, p2, p3, q1, q2, q3, z;

	ier = 2;
	if ((ndim - 1) <= 0) goto L_20;
	else goto L_1;
L_1:
	(*y) = val[0];
	delt2 = (float)0.0;
	if ((ndim - 1) <= 0) goto L_20;
	else goto L_2;
	//     PREPARATIONS FOR INTERPOLATION LOOP 
L_2:
	p2 = 1.e0;
	p3 = (*y);
	q2 = 0.e0;
	q3 = 1.e0;

	//     START INTERPOLATION LOOP 
	for (i = 2; i <= ndim; i++){
		ii = 0;
		p1 = p2;
		p2 = p3;
		q1 = q2;
		q2 = q3;
		z = (*y);
		delt1 = delt2;
		jend = i - 1;

		//     COMPUTATION OF INVERTED DIFFERENCES 
	L_3:
		aux = val[i - 1];
		for (j = 1; j <= jend; j++){
			h = val[i - 1] - val[j - 1];
			if ((fabs(h) - 1.e-13*fabs(val[i - 1])) <= 0.0) goto L_4;
			else goto L_9;
		L_4:
			if ((arg[i - 1] - arg[j - 1]) == 0.0) goto L_17;
			else goto L_5;
		L_5:
			if ((j - jend) < 0) goto L_8;
			else goto L_6;
			//     INTERCHANGE ROW I WITH ROW I+II 
		L_6:
			ii = ii + 1;
			iii = i + ii;
			if ((iii - ndim) <= 0) goto L_7;
			else goto L_19;
		L_7:
			val[i - 1] = val[iii - 1];
			val[iii - 1] = aux;
			aux = arg[i - 1];
			arg[i - 1] = arg[iii - 1];
			arg[iii - 1] = aux;
			goto L_3;

			//     COMPUTATION OF VAL(I) IN CASE VAL(I)=VAL(J) AND J LESS THAN I-1 
		L_8:
			val[i - 1] = 1.e75;
			goto L_10;

			//     COMPUTATION OF VAL(I) IN CASE VAL(I) NOT EQUAL TO VAL(J) 
		L_9:
			val[i - 1] = (arg[i - 1] - arg[j - 1]) / h;
		L_10:
			;
		}

		//     INVERTED DIFFERENCES ARE COMPUTED COMPUTATION OF NEW Y
		p3 = val[i - 1] * p2 + (x - arg[i - 1 - 1])*p1;
		q3 = val[i - 1] * q2 + (x - arg[i - 1 - 1])*q1;
		if (q3 == 0.0) goto L_12;
		else goto L_11;
	L_11:
		(*y) = p3 / q3;
		goto L_13;
	L_12:
		(*y) = 1.e75;
	L_13:
		delt2 = (float)fabs(z - (*y));
		if ((delt2 - eps) <= (float)0.0) goto L_19;
		else goto L_14;
	L_14:
		if ((i - 10) < 0) goto L_16;
		else goto L_15;
	L_15:
		if ((delt2 - delt1) < (float)0.0) goto L_16;
		else goto L_18;
	L_16:;
	}
	//     END OF INTERPOLATION LOOP  
	return ier;

	//     THERE ARE TWO IDENTICAL ARGUMENT VALUES IN VECTOR ARG 
L_17:
	ier = 3;
	return ier;

	//     TEST VALUE DELT2 STARTS OSCILLATING 
L_18:
	(*y) = z;
	ier = 1;
	return ier;

	//     THERE IS SATISFACTORY ACCURACY WITHIN NDIM-1 STEPS 
L_19:
	ier = 0;
L_20:
	return ier;
}

