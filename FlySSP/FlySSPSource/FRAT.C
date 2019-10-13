//------------------------------------------------------------------------
//   FRAT.C
//     ÄèèêéäëàåÄñàü áÄÑÄççéâ ÑàëäêÖíçéâ îìçäñàà êÄñàéçÄãúçéâ îìçäñàÖâ çÄ
//	       éëçéÇÖ åçä
//   Ä¢‚Æ‡  : è•‡•¢Æ§ Ø‡Æ£‡†¨¨Î Ø†™•‚† SSP ≠† ë ¢ÎØÆ´≠®´ è†·Î≠™Æ¢ Ç.Ç.
//   Ç•‡·®Ô : 01.00 / 10.06.1996 /
//------------------------------------------------------------------------
#include "ssp.h"
/*     ..................................................................

        SUBROUTINE FRAT

	PURPOSE
	   FRAT IS USED FOR HANDLING OF DATA AND FUNDAMENTAL FUNCTIONS
	   WITH RATIONAL APPROXIMATION. IT IS A SUBSTANTIAL PART OF
	   RATIONAL APPROXIMATION AND HAS NO MEANING INDEPENDENTLY

	USAGE
	   FRAT(I,N,M,P,DATI,WGT,IER)

	DESCRIPTION OF PARAMETERS
	   I     - SUBSCRIPT OF CURRENT DATA POINT
	   N     - NUMBER OF ALL DATA POINTS
	   M     - NUMBER OF FUNDAMENTAL FUNCTIONS USED
	   P     - ARRAY OF DIMENSION M+1 AT LEAST, WHICH CONTAINS
		   ON RETURN THE VALUES OF THE M FUNDAMENTAL
		   FUNCTIONS, FOLLOWED BY CURRENT FUNCTION VALUE
	   DATI  - ARRAY CONTAINING GIVEN N ARGUMENTS, FOLLOWED
		   BY N FUNCTION VALUES AND FINALLY BY 1 RESPECTIVELY
		   N WEIGHT VALUES
	   WGT   - RESULTANT WEIGHT FACTOR USED FOR I-TH TERM
	   IER   - RESULTANT ERROR PARAMETER, COMBINED WITH INPUT
		   VALUES FOR CONTROL
		   IER(2) MEANS DIMENSION OF NUMERATOR
		   IER(3) MEANS DIMENSION OF DENOMINATOR
		   IER(1) IS USED AS RESULTANT ERROR PARAMETER,
		   IER(1) = 0 IN CASE OF NO ERRORS
		   IER(1) = 1 OTHERWISE (ZERO DENOMINATOR)

	REMARKS
	   VECTOR IER IS USED FOR COMMUNICATION BETWEEN ARAT AND FRAT

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           CNP

	METHOD
	   CF. MATHEMATICAL DESCRIPTION OF SUBROUTINE ARAT

     ..................................................................
double fun(double x, double y)
//double *x, *y;
{
double fun_v;


fun_v = 1./(*x);
return( fun_v );
}
*/
void frat(int i, int n, int m, double p[], double dati[], double *wgt, int ier[])
{
int    ii, ip, ipq, iq, iqm1, j;
double f, fden, fnum, t;



/*        DIMENSIONED DUMMY VARIABLES */

/*        INITIALIZATION */
ip   = m;
ip   = ier[1];
iq   = ier[2];
iqm1 = iq - 1;
ipq  = ip + iq;

/*        LOOK UP ARGUMENT, FUNCTION VALUE AND WEIGHT
	LOOK UP NUMERATOR AND DENOMINATOR */
t = dati[i-1];
j = i + n;
f = dati[j-1];
fnum = p[j-1];
j = j + n;
(*wgt) = 1.;
switch( for_if(dati[2*n + 1-1]) ){
	case -1: goto L_2;
	case  0: goto L_2;
	case  1: goto L_1;
	}
L_1:
(*wgt) = dati[j-1];
L_2:
fden = p[j-1];

/*        CALCULATE FUNCTION VALUE USED */
f = f*fden - fnum;

/*        CHECK FOR ZERO DENOMINATOR */
switch( for_if(fden) ){
	case -1: goto L_4;
	case  0: goto L_3;
	case  1: goto L_4;
	}

/*        ERROR RETURN IN CASE OF ZERO DENOMINATOR */
L_3:
ier[0] = 1;
return;

/*        CALCULATE WEIGHT FACTORS USED */
L_4:
(*wgt) = (*wgt)/(fden*fden);
fnum = -fnum/fden;

/*        CALCULATE FUNDAMENTAL FUNCTIONS */
j = iqm1;
switch( for_if(ip - iq) ){
	case -1: goto L_6;
	case  0: goto L_6;
	case  1: goto L_5;
	}
L_5:
j = ip - 1;
L_6:
cnp( &p[iq-1], t, j );

/*        STORE VALUES OF DENOMINATOR FUNDAMENTAL FUNCTIONS */
//L_7:
switch( for_if(iqm1) ){
	case -1: goto L_10;
	case  0: goto L_10;
	case  1: goto L_8;
	}
L_8:
for( ii = 1; ii <= iqm1; ii++ ){
	j = ii + iq;
	p[ii-1] = p[j-1]*fnum;
	}

/*        STORE FUNCTION VALUE */
L_10:
p[ipq-1] = f;

/*        NORMAL RETURN */
ier[0] = 0;
return;
}

