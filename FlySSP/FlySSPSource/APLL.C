//------------------------------------------------------------------------
//   APLL.C
//     —Œ—“¿¬À≈Õ»≈ —»—“≈Ã€ ÕŒ–Ã¿À‹Õ€’ ”–¿¬Õ≈Õ»…, ¬Œ«Õ» ¿ﬁŸ≈… œ–» (Õ¿»À”◊ÿ≈…
//     ¬ —Ã€—À≈ Ã≈“Œƒ¿ Õ¿»Ã≈Õ‹ÿ»’  ¬¿ƒ–¿“Œ¬) ¿œœ–Œ —»Ã¿÷»» «¿ƒ¿ÕÕŒ…
//     ƒ»— –≈“ÕŒ… ‘”Õ ÷»» À»Õ≈…ÕŒ…  ŒÃ¡»Õ¿÷»≈≈… ‘”Õ ÷»…
//   ¿‚ÚÓ  : œÂÂ‚Ó‰ ÔÓ„‡ÏÏ˚ Ô‡ÍÂÚ‡ SSP Ì‡ — ‚˚ÔÓÎÌËÎ œ‡Ò˚ÌÍÓ‚ ¬.¬.
//   ¬ÂÒËˇ : 01.00 / 10.06.1996 /
//------------------------------------------------------------------------
#include "ssp.h"

//------------------------------------------------------------------------
// SUBROUTINE apll
// PURPOSE
// SET UP NORMAL EQUATIONS FOR A LINEAR LEAST SQUARES FIT
// TO A GIVEN DISCRETE FUNCTION
// 
// USAGE
// apll(FFCT,N,IP,P,WORK,DATI,IER)
// SUBROUTINE FFCT REQUIRES AN EXTERNAL STATEMENT

// DESCRIPTION OF PARAMETERS
//		FFCT  - USER CODED SUBROUTINE WHICH MUST BE DECLARED
//				EXTERNAL IN THE MAIN PROGRAM. IT IS CALLED
//				CALL FFCT(I,N,IP,P,DATI,WGT,IER) AND RETURNS
//				THE VALUES OF THE FUNDAMENTAL FUNCTIONS FOR
//				THE I-TH ARGUMENT IN P(1) UP TO P(IP)
//				FOLLOWED BY THE I-TH FUNCTION VALUE IN P(IP+1)
//				N IS THE NUMBER OF ALL POINTS
//				P,DATI,WGT MUST BE OF DOUBLE PRECISION.
//				DATI IS A DUMMY PARAMETER WHICH IS USED AS ARRAY
//				NAME. THE GIVEN DATA SET MAY BE ALLOCATED IN DATI
//				WGT IS THE WEIGHT FACTOR FOR THE I-TH POINT
//				IER IS USED AS RESULTANT ERROR PARAMETER IN FFCT
//		IP    - NUMBER OF FUNDAMENTAL FUNCTIONS USED FOR LEAST
//				SQUARES FIT
//				IP SHOULD NOT EXCEED N
//		P     - WORKING STORAGE OF DIMENSION IP+1, WHICH
//				IS USED AS INTERFACE BETWEEN APLL AND THE USER
//				CODED SUBROUTINE FFCT
//				P MUST BE OF DOUBLE PRECISION.
//		WORK  - WORKING STORAGE OF DIMENSION (IP+1)*(IP+2)/2.
//				ON RETURN WORK CONTAINS THE SYMMETRIC COEFFICIENT
//				MATRIX OF THE NORMAL EQUATIONS IN COMPRESSED FORM,
//				I.E. UPPER TRINGULAR PART ONLY STORED COLUMNWISE.
//				THE FOLLOWING IP POSITIONS CONTAIN THE RIGHT
//				HAND SIDE AND WORK((IP+1)*(IP+2)/2) CONTAINS
//				THE WEIGHTED SQUARE SUM OF THE FUNCTION VALUES
//				WORK MUST BE OF DOUBLE PRECISION.
//		DATI  - DUMMY ENTRY TO COMMUNICATE AN ARRAY NAME BETWEEN
//				MAIN LINE AND SUBROUTINE FFCT.
//				DATI MUST BE OF DOUBLE PRECISION.
//		IER   - RESULTING ERROR PARAMETER
//				IER =-1 MEANS FORMAL ERRORS IN SPECIFIED DIMENSIONS
//				IER = 0 MEANS NO ERRORS
//				IER = 1 MEANS ERROR IN EXTERNAL SUBROUTINE FFCT
//
// REMARKS
// TO ALLOW FOR EASY COMMUNICATION OF INTEGER VALUES
// BETWEEN MAINLINE AND EXTERNAL SUBROUTINE FFCT, THE ERROR
// PARAMETER IER IS TREATED AS A VECTOR OF DIMENSION 1 WITHIN
// SUBROUTINE APLL. ADDITIONAL COMPONENTS OF IER MAY BE
// INTRODUCED BY THE USER FOR COMMUNICATION BACK AND FORTH.
// IN THIS CASE, HOWEVER, THE USER MUST SPECIFY IER AS A
// VECTOR IN HIS MAINLINE.
// EXECUTION OF SUBROUTINE APLL IS A PREPARATORY STEP FOR
// CALCULATION OF THE LINEAR LEAST SQUARES FIT.
// NORMALLY IT IS FOLLOWED BY EXECUTION OF SUBROUTINE DAPFS
// 
// SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
// THE EXTERNAL SUBROUTINE FFCT MUST BE FURNISHED BY THE USER
// 
// METHOD
// HANDLING OF THE GIVEN DATA SET (ARGUMENTS,FUNCTION VALUES
// AND WEIGHTS) IS COMPLETELY LEFT TO THE USER
// ESSENTIALLY HE HAS THREE CHOICES
//		(1) THE I-TH VALUES OF ARGUMENT, FUNCTION VALUE AND WEIGHT
//			ARE CALCULATED WITHIN SUBROUTINE FFCT FOR GIVEN I.
//		(2) THE I-TH VALUES OF ARGUMENT, FUNCTION VALUE AND WEIGHT
//			ARE DETERMINED BY TABLE LOOK UP. THE STORAGE LOCATIONS
//			REQUIRED ARE ALLOCATED WITHIN THE DUMMY ARRAY DATI
//			(POSSIBLY IN P TOO, IN EXCESS OF THE SPECIFIED IP + 1
//			LOCATIONS).
//			ANOTHER POSSIBILITY WOULD BE TO USE COMMON AS INTERFACE
//			BETWEEN MAIN LINE AND SUBROUTINE FFCT AND TO ALLOCATE
//			STORAGE FOR THE DATA SET IN COMMON.
//		(3) THE I-TH VALUES OF ARGUMENT, FUNCTION VALUE AND WEIGHT
//			ARE READ IN FROM AN EXTERNAL DEVICE. THIS MAY BE EASILY
//			ACCOMPLISHED SINCE I IS USED STRICTLY INCREASING FROM
//			ONE UP TO N WITHIN APLL
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void apll(void(*ffct)(int, int, int, double *, double *, double *, int *),
		  int n, int ip, double p[], double work[], double dati[], 
		  int irr[])
{
	//int    ier;
	//int    irr[2];
	int    i, ipp1, j, k, l, m;
	double aux, wgt;
	//ier=0;
	//irr[0]=irr[1]=0;
	//        DIMENSIONED DUMMY VARIABLES 
	//        CHECK FOR FORMAL ERRORS IN SPECIFIED DIMENSIONS 
	switch (for_if(n)){
		case -1: goto L_10;
		case  0: goto L_10;
		case  1: goto L_1;
	}
L_1:
	switch (for_if(ip)){
		case -1: goto L_10;
		case  0: goto L_10;
		case  1: goto L_2;
	}
L_2:
	switch (for_if(n - ip)){
		case -1: goto L_10;
		case  0: goto L_3;
		case  1: goto L_3;
	}
	//        SET WORKING STORAGE AND RIGHT HAND SIDE TO ZERO 
L_3:
	ipp1 = ip + 1;
	m = ipp1*(ip + 2) / 2;
	irr[0] = 0;
	for (i = 1; i <= m; i++) {
		work[i - 1] = 0.e0;
	}
	//        START GREAT LOOP OVER ALL GIVEN POINTS 
	for (i = 1; i <= n; i++) {
		ffct(i, n, ip, p, dati, &wgt, irr);
		switch (for_if(irr[0])){
			case -1: goto L_9;
			case  0: goto L_5;
			case  1: goto L_9;
		}
L_5:	j = 0;
		for (k = 1; k <= ipp1; k++){
			aux = p[k - 1] * wgt;
			for (l = 1; l <= k; l++){
				j = j + 1;
				work[j - 1] = work[j - 1] + p[l - 1] * aux;
	}	}	}
	//        NORMAL RETURN */
L_9:
	//ier=irr[0];
	return;
	//        ERROR RETURN IN CASE OF FORMAL ERRORS 
L_10:
	irr[0] = -1;
	return;
}


