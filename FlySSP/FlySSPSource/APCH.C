//------------------------------------------------------------------------
// APяH.C
// янярюбкемхе яхярелш мнплюкэмшу спюбкмемхи, бнгмхйючыеи опх
// опхакхфемхх дхяйпермни тсмйжхх лмнцнвкемнл он лерндс мюхлемэьху
// йбюдпюрнб
// юБРНП   : оЕПЕБНД ОПНЦПЮЛЛШ ОЮЙЕРЮ SSP МЮ я БШОНКМХК оЮЯШМЙНБ б.б.
// бЕПЯХЪ  : 01.00 / 10.08.1997 /
//------------------------------------------------------------------------
#include "ssp.h"

//------------------------------------------------------------------------
// кхмеимюъ веашьебяйюъ юоопнйяхлюжхъ гюдюммни дхяйпермни
// тсмйжхх кхмеимни йнлахмюжхееи гюдюммшу меопепшбмшу тсмйжхи
// ондопнцпюллю apch
//
// мюгмювемхе
// ондопнцпюллю янярюбкъер яхярелс мнплюкэмшу спюбмемхи,
// бнгмхйючыеи опх опхакхфемхх дхяйпермни тсмйжхх
// лмнцнвкемнл он лерндс мюхлемэьху йбюдпюрнб
//
// напюыемхе й ондопнцпюлле
// apch(DATI,N,IP,XD,X0,WORK,IER)
//
// бундмше х бшундмше оюпюлерпш
//		DATI  - бундмни люяяхб пюглепмнярх 3*N (хкх пюглепмнярх 2*N+1),
//				яндепфюыхи гюдюммше гмювемхъ юпцслемрю, яннрберярбсчыхе
//				гмювемхъ тсмйжхх х N гмювемхи беяю, еякх нмх еярэ.
//				еякх беянб мер, рн (2*N+1)-и щкелемр люяяхбю яндепфхр
//				меонкнфхрекэсч бекхвхмс.
//				яндепфхлне люяяхбю янупюмъеряъ.
//		N     - вхякн гюдюммшу рнвей;
//		IP    - вхякн лмнцнвкемнб веашьебю, хяонкэгселшу б йювеярбе
//				тсмдюлемрюкэмшу тсмйжхи (IP<=N);
//		XD    - йнмярюмрю-лмнфхрекэ опенапюгнбюмхъ накюярх юпцслемрнб
//		X0    - ябнандмши вкем кхмеимнцн опенапюгнбюмхъ накюярх юпцслемрнб
//		WORK  - пюанвхи люяяхб пюглепмнярх (IP+1)*(IP+2)/2;
//				опх бшунде хг ондопнцпюллш яндепфхр яхллерпхвеяйсч люрпхжс
//				йнщттхжхемрнб яхярелш мнплюкэмшу спюбмемхи б яфюрни
//				(рпесцнкэмни) тнпле, опюбше вюярх яхярелш х бгбеьеммсч
//				ясллс йбюдпюрнб.
//		IER   - бнгбпюыюелши хмдхйюрнп ньхайх
//
//		IER =-1 MEANS FORMAL ERRORS IN DIMENSION
//		IER = 0 MEANS NO ERRORS
//		IER = 1 MEANS COINCIDING ARGUMENTS
//
// REMARKS
// NO WEIGHTS ARE USED IF THE VALUE OF DATI(2*N+1) IS
// NOT POSITIVE.
// EXECUTION OF SUBROUTINE DAPCH IS A PREPARATORY STEP FOR
// CALCULATION OF LEAST SQUARES FITS IN CHEBYSHEV POLYNOMIALS
// IT SHOULD BE FOLLOWED BY EXECUTION OF SUBROUTINE DAPFS
//
// SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED NONE
//
// METHOD
// THE LEAST SQUARE FIT IS DETERMINED USING CHEBYSHEV
// POLYNOMIALS AS FUNDAMENTAL FUNCTION SYSTEM.
// THE METHOD IS DISCUSSED IN THE ARTICLE
// A.T.BERZTISS, LEAST SQUARES FITTING TO IRREGULARLY SPACED
// DATA, SIAM REVIEW, VOL.6, ISS.3, 1964, PP. 203-227.
//------------------------------------------------------------------------

int  apch(double dati[], int n, int ip, double * xd, double * x0, double work[])
{
	int    ier, i, iend, j, jj, k, kk, ll, m, mt2, mt2m;
	double df, sum, t, xa, xe, xm;
	//       DIMENSIONED DUMMY VARIABLES 
	//       CHECK FOR FORMAL ERRORS IN SPECIFIED DIMENSIONS 
	if ((n - 1)  < 0) goto L_19;
	else
		if ((n - 1) == 0) goto L_20;
	if (ip <= 0) goto L_19;
	//       SEARCH SMALLEST AND LARGEST ARGUMENT 
	if ((ip - n) > 0) goto L_19;

	k = (ip + 1)*(ip + 2) / 2;
	//       SET WORKING STORAGE AND RIGHT HAND SIDE TO ZERO 
	for (i = 0; i < k; i++) work[i] = 0.0;

	xa = dati[0]; (*x0) = xa; xe = 0.e0;
	for (i = 0; i < n; i++){
		xm = dati[i];
		if ((xa - xm) > 0.0)
			xa = xm;
		if (((*x0) - xm) < 0.0)
			(*x0) = xm;
	}
	//       INITIALIZE CALCULATION OF NORMAL EQUATIONS 
	(*xd) = (*x0) - xa;
	m = (ip*(ip + 1)) / 2;
	iend = m + ip + 1;
	mt2 = ip + ip;
	mt2m = mt2 - 1;
	//       SET WORKING STORAGE AND RIGHT HAND SIDE TO ZERO
	//for( i = 0; i < ip; i++ ){
	//j = mt2 - i;
	//work[j] = 0.e0;
	//work[i] = 0.e0;
	//k = m + i;
	//work[k] = 0.e0;
	//}

	//       CHECK FOR DEGENERATE ARGUMENT RANGE 
	if ((*xd) <= 0.0) goto L_20;
	//       CALCULATE CONSTANTS FOR REDUCTION OF ARGUMENTS 
	(*x0) = -((*x0) + xa) / (*xd);
	(*xd) = 2.e0 / (*xd);
	//       START GREAT LOOP OVER ALL GIVEN POINTS 
	for (sum = 0.e0, i = 0; i < n; i++){
		t = dati[i] * (*xd) + (*x0);
		j = i + n;
		df = dati[j];
		// CALCULATE AND STORE VALUES OF CHEBYSHEV POLYNOMIALS FOR ARGUMENT T 
		xa = 1.e0;	xm = t;
		if (dati[2 * n] > 0.0)
		{
			j = j + n;
			xa = dati[j];
			xm = t*xa;
		}
		t = t + t;
		sum = sum + df*df*xa;
		df = df + df;
		j = 0;
	L_12:
		k = m + j;
		work[k] = work[k] + df*xa;
	L_13:
		work[j] = work[j] + xa;
		if ((j - (mt2m - 1)) >= 0) continue;
		j = j + 1;
		xe = t*xm - xa;
		xa = xm;
		xm = xe;
		if ((j - (ip - 1)) <= 0) goto L_12;
		else goto L_13;
	}
	work[iend - 1] = sum + sum;
	//       CALCULATE MATRIX OF NORMAL EQUATIONS 
	ll = m - 1; kk = mt2m; jj = 0; k = kk - 1;
	for (j = 0; j < m; j++){
		work[ll] = work[k] + work[jj];
		ll = ll - 1;
		if ((k - jj) > 0)
		{
			jj = jj + 1;
			k = k - 1;
		}
		else
		{
			kk = kk - 2;
			k = kk - 1;
			jj = 0;
		}
	}
	ier = 0;
	return ier;
	//       ERROR RETURN IN CASE OF FORMAL ERRORS 
L_19:
	ier = -1;
	return ier;
	//       ERROR RETURN IN CASE OF COINCIDING ARGUMENTS 
L_20:
	ier = 1;
	return ier;
}

