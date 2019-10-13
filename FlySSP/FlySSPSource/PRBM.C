/*------------------------------------------------------------------------
  PRBM.C
      êÄëóÖí ÇëÖï ÑÖâëíÇàíÖãúçõï à äéåèãÖäëçõï äéêçÖâ
      ÄãÉÖÅêÄàóÖëäéÉé èéãàçéåÄ
  Ä¢‚Æ‡  : è•‡•¢Æ§ Ø‡Æ£‡†¨¨Î Ø†™•‚† SSP ≠† ë ¢ÎØÆ´≠®´ è†·Î≠™Æ¢ Ç.Ç.
  Ç•‡·®Ô : 01.00 / 01.06.1994 /
------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	SUBROUTINE PRBM

	PURPOSE
	   TO CALCULATE ALL REAL AND COMPLEX ROOTS OF A GIVEN
	   POLYNOMIAL WITH REAL COEFFICIENTS.

	USAGE
	   PRBM (C,IC,RR,RC,POL,IR,IER)

	DESCRIPTION OF PARAMETERS
	   C      - INPUT VECTOR CONTAINING THE COEFFICIENTS OF THE
		    GIVEN POLYNOMIAL. COEFFICIENTS ARE ORDERED FROM
		    LOW TO HIGH. ON RETURN COEFFICIENTS ARE DIVIDED
		    BY THE LAST NONZERO TERM.
	   IC     - DIMENSION OF VECTORS C, RR, RC, AND POL.
	   RR     - RESULTANT VECTOR OF REAL PARTS OF THE ROOTS.
	   RC     - RESULTANT VECTOR OF COMPLEX PARTS OF THE ROOTS.
	   POL    - RESULTANT VECTOR OF COEFFICIENTS OF THE POLYNOMIAL
		    WITH CALCULATED ROOTS. COEFFICIENTS ARE ORDERED
		    FROM LOW TO HIGH (SEE REMARK 4).
	   IR     - OUTPUT VALUE SPECIFYING THE NUMBER OF CALCULATED
                    ROOTS. NORMALLY IR IS EQUAL TO IC-1.
           IER    - RESULTANT ERROR PARAMETER CODED AS FOLLOWS
                     IER=0  - NO ERROR,
                     IER=1  - SUBROUTINE PQFB RECORDS POOR CONVERGENCE
                              AT SOME QUADRATIC FACTORIZATION WITHIN
                              50 ITERATION STEPS,
		     IER=2  - POLYNOMIAL IS DEGENERATE, I.E. ZERO OR
                              CONSTANT,
                              OR OVERFLOW IN NORMALIZATION OF GIVEN
			      POLYNOMIAL,
                     IER=3  - THE SUBROUTINE IS BYPASSED DUE TO
			      SUCCESSIVE ZERO DIVISORS OR OVERFLOWS
                              IN QUADRATIC FACTORIZATION OR DUE TO
                              COMPLETELY UNSATISFACTORY ACCURACY,
                     IER=-1 - CALCULATED COEFFICIENT VECTOR HAS LESS
                              THAN THREE CORRECT SIGNIFICANT DIGITS.
                              THIS REVEALS POOR ACCURACY OF CALCULATED
                              ROOTS.

        REMARKS
           (1) REAL PARTS OF THE ROOTS ARE STORED IN RR(1) UP TO RR(IR)
	       AND CORRESPONDING COMPLEX PARTS IN RC(1) UP TO RC(IR).
           (2) ERROR MESSAGE IER=1 INDICATES POOR CONVERGENCE WITHIN
	       50 ITERATION STEPS AT SOME QUADRQTIC FACTORIZATION
               PERFORMED BY SUBROUTINE PQFB.
           (3) NO ACTION BESIDES ERROR MESSAGE IER=2 IN CASE OF A ZERO
               OR CONSTANT POLYNOMIAL. THE SAME ERROR MESSAGE IS GIVEN
               IN CASE OF AN OVERFLOW IN NORMALIZATION OF GIVEN
               POLYNOMIAL.
           (4) ERROR MESSAGE IER=3 INDICATES SUCCESSIVE ZERO DIVISORS
	       OR OVERFLOWS OR COMPLETELY UNSATISFACTORY ACCURACY AT
               ANY QUADRATIC FACTORIZATION PERFORMED BY
               SUBROUTINE PQFB. IN THIS CASE CALCULATION IS BYPASSED.
	       IR RECORDS THE NUMBER OF CALCULATED ROOTS.
               POL(1),...,POL(J-IR) ARE THE COEFFICIENTS OF THE
	       REMAINING POLYNOMIAL, WHERE J IS THE ACTUAL NUMBER OF
               COEFFICIENTS IN VECTOR C (NORMALLY J=IC).
           (5) IF CALCULATED COEFFICIENT VECTOR HAS LESS THAN THREE
               CORRECT SIGNIFICANT DIGITS THOUGH ALL QUADRATIC
               FACTORIZATIONS SHOWED SATISFACTORY ACCURACY, THE ERROR
               MESSAGE IER=-1 IS GIVEN.
           (6) THE FINAL COMPARISON BETWEEN GIVEN AND CALCULATED
	       COEFFICIENT VECTOR IS PERFORMED ONLY IF ALL ROOTS HAVE
               BEEN CALCULATED. IN THIS CASE THE NUMBER OF ROOTS IR IS
               EQUAL TO THE ACTUAL DEGREE OF THE POLYNOMIAL (NORMALLY
	       IR=IC-1). THE MAXIMAL RELATIVE ERROR OF THE COEFFICIENT
               VECTOR IS RECORDED IN RR(IR+1).

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           SUBROUTINE PQFB     QUADRATIC FACTORIZATION OF A POLYNOMIAL
                               BY BAIRSTOW ITERATION.

        METHOD
           THE ROOTS OF THE POLYNOMIAL ARE CALCULATED BY MEANS OF
	   SUCCESSIVE QUADRATIC FACTORIZATION PERFORMED BY BAIRSTOW
           ITERATION. X**2 IS USED AS INITIAL GUESS FOR THE FIRST
           QUADRATIC FACTOR, AND FURTHER EACH CALCULATED QUADRATIC
	   FACTOR IS USED AS INITIAL GUESS FOR THE NEXT ONE. AFTER
           COMPUTATION OF ALL ROOTS THE COEFFICIENT VECTOR IS
	   CALCULATED AND COMPARED WITH THE GIVEN ONE.
	   FOR REFERENCE, SEE J. H. WILKINSON, THE EVALUATION OF THE
	   ZEROS OF ILL-CONDITIONED POLYNOMIALS (PART ONE AND TWO),
	   NUMERISCHE MATHEMATIK, VOL.1 (1959), PP.150-180.

     ..................................................................
 */

int prbm(double c[], int ic,
	  double rr[],double rc[], double pol[],int *ir)
{
int    i, ist, j, l, lim, n, ier;
double a, b, eps, h, q[4], q1, q2;
/*        TEST ON LEADING ZERO COEFFICIENTS */
eps = 1.e-3;
lim = 50;
(*ir) = ic + 1;
L_1:
(*ir) = (*ir) - 1;
switch( for_if((*ir) - 1) ){
	case -1: goto L_42;
	case  0: goto L_42;
	case  1: goto L_2;
	}
L_2:
switch(for_if(c[(*ir)-1])){
	case -1: goto L_3;
	case  0: goto L_1;
	case  1: goto L_3;
	}

/*        WORK UP ZERO ROOTS AND NORMALIZE REMAINING POLYNOMIAL */
L_3:
ier = 0;
j = (*ir);
l = 0;
a = c[(*ir)-1];
for( i = 1; i <= (*ir); i++ ){
	switch( for_if(l) ){
		case -1: goto L_4;
		case  0: goto L_4;
		case  1: goto L_7;
		}
L_4:
	switch(for_if(c[i-1])){
		case -1: goto L_6;
		case  0: goto L_5;
		case  1: goto L_6;
		}
L_5:
	rr[i-1] = 0.;
	rc[i-1] = 0.;
	pol[j-1] = 0.;
	j = j - 1;
	goto L_8;
L_6:
	l = 1;
	ist = i;
	j = 0;
L_7:
	j = j + 1;
	c[i-1] = c[i-1]/a;
	pol[j-1] = c[i-1];
	overfl( &n );
	switch( for_if(n - 2) ){
		case -1: goto L_42;
		case  0: goto L_8;
		case  1: goto L_8;
		}
L_8:
	;
	}

/*        START BAIRSTOW ITERATION */
q1 = 0.;
q2 = 0.;
L_9:
switch( for_if(j - 2) ){
	case -1: goto L_33;
	case  0: goto L_10;
	case  1: goto L_14;
	}

/*        DEGREE OF RESTPOLYNOMIAL IS EQUAL TO ONE */
L_10:
a = pol[0];
rr[ist-1] = -a;
rc[ist-1] = 0.;
(*ir) = (*ir) - 1;
q2 = 0.;
switch( for_if((*ir) - 1) ){
	case -1: goto L_13;
	case  0: goto L_13;
	case  1: goto L_11;
	}
L_11:
for( i = 2; i <= (*ir); i++ ){
	q1 = q2;
	q2 = pol[i + 1-1];
	pol[i-1] = a*q2 + q1;
	}
L_13:
pol[(*ir) + 1-1] = a + q2;
goto L_34;
/*        THIS IS BRANCH TO COMPARISON OF COEFFICIENT VECTORS C AND POL

        DEGREE OF RESTPOLYNOMIAL IS GREATER THAN ONE */
L_14:
for( l = 1; l <= 10; l++ ){
	n = 1;
L_15:
	q[0] = q1;
	q[1] = q2;
	i=pqfb( pol, j, q, lim );
	switch( for_if(i) ){
		case -1: goto L_16;
		case  0: goto L_24;
		case  1: goto L_23;
		}
L_16:
	switch( for_if(q1) ){
		case -1: goto L_18;
		case  0: goto L_17;
		case  1: goto L_18;
		}
L_17:
	switch(for_if(q2)){
		case -1: goto L_18;
		case  0: goto L_21;
		case  1: goto L_18;
		}
L_18:
	switch( n ){
		case 1: goto L_19;
		case 2: goto L_20;
		case 3: goto L_19;
		case 4: goto L_21;
		}
L_19:
	q1 = -q1;
	n = n + 1;
	goto L_15;
L_20:
	q2 = -q2;
	n = n + 1;
	goto L_15;
L_21:
	q1 = 1. + q1;
	q2 = 1. - q2;
	}

/*        ERROR EXIT DUE TO UNSATISFACTORY RESULTS OF FACTORIZATION */
ier = 3;
(*ir) = (*ir) - j;
return ier;

/*        WORK UP RESULTS OF QUADRATIC FACTORIZATION */
L_23:
ier = 1;
L_24:
q1 = q[0];
q2 = q[1];

/*        PERFORM DIVISION OF FACTORIZED POLYNOMIAL BY QUADRATIC FACTOR */
b = 0.;
a = 0.;
i = j;
L_25:
h = -q1*b - q2*a + pol[i-1];
pol[i-1] = b;
b = a;
a = h;
i = i - 1;
switch( for_if(i - 2) ){
	case -1: goto L_26;
	case  0: goto L_26;
	case  1: goto L_25;
	}
L_26:
pol[1] = b;
pol[0] = a;

/*        MULTIPLY POLYNOMIAL WITH CALCULATED ROOTS BY QUADRATIC FACTOR */
l = (*ir) - 1;
switch( for_if(j - l) ){
	case -1: goto L_27;
	case  0: goto L_27;
	case  1: goto L_29;
	}
L_27:
for( i = j; i <= l; i++ )
	pol[i - 1-1] = pol[i - 1-1] + pol[i-1]*q2 + pol[i + 1-1]*q1;
L_29:
pol[l-1] = pol[l-1] + pol[l + 1-1]*q2 + q1;
pol[(*ir)-1] = pol[(*ir)-1] + q2;

/*        CALCULATE ROOT-PAIR FROM QUADRATIC FACTOR X*X+Q2*X+Q1 */
h = -.5*q2;
a = h*h - q1;
b = sqrt( fabs( a ) );
switch( for_if(a) ){
	case -1: goto L_30;
	case  0: goto L_30;
	case  1: goto L_31;
	}
L_30:
rr[ist-1] = h;
rc[ist-1] = b;
ist = ist + 1;
rr[ist-1] = h;
rc[ist-1] = -b;
goto L_32;
L_31:
b = h + Sign2( b, h );
rr[ist-1] = q1/b;
rc[ist-1] = 0.;
ist = ist + 1;
rr[ist-1] = b;
rc[ist-1] = 0.;
L_32:
ist = ist + 1;
j = j - 2;
goto L_9;

/*        SHIFT BACK ELEMENTS OF POL BY 1 AND COMPARE VECTORS POL AND C */
L_33:
(*ir) = (*ir) - 1;
L_34:
a = 0.;
for( i = 1; i <= (*ir); i++ ){
	q1 = c[i-1];
	q2 = pol[i + 1-1];
	pol[i-1] = q2;
	switch( for_if(q1) ){
		case -1: goto L_35;
		case  0: goto L_36;
		case  1: goto L_35;
		}
L_35:
	q2 = (q1 - q2)/q1;
L_36:
	q2 = fabs( q2 );
	switch( for_if(q2 - a) ){
		case -1: goto L_38;
		case  0: goto L_38;
		case  1: goto L_37;
		}
L_37:
	a = q2;
L_38:
	;
	}
i = (*ir) + 1;
pol[i-1] = 1.;
rr[i-1] = a;
rc[i-1] = 0.;
switch( for_if(ier) ){
	case -1: goto L_39;
	case  0: goto L_39;
	case  1: goto L_41;
	}
L_39:
switch( for_if(a - eps) ){
	case -1: goto L_41;
	case  0: goto L_41;
	case  1: goto L_40;
	}

/*        WARNING DUE TO POOR ACCURACY OF CALCULATED COEFFICIENT VECTOR */
L_40:
ier = -1;
L_41:
return ier;

/*        ERROR EXIT DUE TO DEGENERATE POLYNOMIAL OR OVERFLOW IN
        NORMALIZATION */
L_42:
ier = 2;
(*ir) = 0;
return ier;
}

