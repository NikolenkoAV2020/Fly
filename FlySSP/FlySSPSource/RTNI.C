/*---------------------------------------------------------------------------
  RTNI.C
	êÖòÖçàÖ çÖãàçÖâçéÉé ìêÄÇçÖçàü åÖíéÑéå çúûíéçÄ
  Ä¢‚Æ‡	 : è†·Î≠™Æ¢ Ç.Ç.
  Ç•‡·®Ô  : 01.00 / 10.08.1997 /
---------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	RTNI

        PURPOSE
	   TO SOLVE GENERAL NONLINEAR EQUATIONS OF THE FORM F(X)=0
           BY MEANS OF NEWTON-S ITERATION METHOD.

        USAGE
	   RTNI (X,F,DERF,FCT,XST,EPS,IEND,IER)
	   PARAMETER FCT REQUIRES AN EXTERNAL STATEMENT.

	DESCRIPTION OF PARAMETERS
	   X      - DOUBLE PRECISION RESULTANT ROOT OF EQUATION F(X)=0.
	   F      - DOUBLE PRECISION RESULTANT FUNCTION VALUE AT
		    ROOT X.
	   DERF   - DOUBLE PRECISION RESULTANT VALUE OF DERIVATIVE
		    AT ROOT X.
	   FCT    - NAME OF THE EXTERNAL SUBROUTINE USED. IT COMPUTES
		    TO GIVEN ARGUMENT X FUNCTION VALUE F AND DERIVATIVE
		    DERF. ITS PARAMETER LIST MUST BE X,F,DERF, WHERE
		    ALL PARAMETERS ARE DOUBLE PRECISION.
	   XST    - DOUBLE PRECISION INPUT VALUE WHICH SPECIFIES THE
		    INITIAL GUESS OF THE ROOT X.
	   EPS    - SINGLE PRECISION INPUT VALUE WHICH SPECIFIES THE
                    UPPER BOUND OF THE ERROR OF RESULT X.
           IEND   - MAXIMUM NUMBER OF ITERATION STEPS SPECIFIED.
	   IER    - RESULTANT ERROR PARAMETER CODED AS FOLLOWS
                     IER=0 - NO ERROR,
                     IER=1 - NO CONVERGENCE AFTER IEND ITERATION STEPS,
                     IER=2 - AT ANY ITERATION STEP DERIVATIVE DERF WAS
                             EQUAL TO ZERO.

	REMARKS
           THE PROCEDURE IS BYPASSED AND GIVES THE ERROR MESSAGE IER=2
	   IF AT ANY ITERATION STEP DERIVATIVE OF F(X) IS EQUAL TO 0.
	   POSSIBLY THE PROCEDURE WOULD BE SUCCESSFUL IF IT IS STARTED
           ONCE MORE WITH ANOTHER INITIAL GUESS XST.

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   THE EXTERNAL SUBROUTINE FCT(X,F,DERF) MUST BE FURNISHED
           BY THE USER.

        METHOD
           SOLUTION OF EQUATION F(X)=0 IS DONE BY MEANS OF NEWTON-S
           ITERATION METHOD, WHICH STARTS AT THE INITIAL GUESS XST OF
	   A ROOT X. CONVERGENCE IS QUADRATIC IF THE DERIVATIVE OF
           F(X) AT ROOT X IS NOT EQUAL TO ZERO. ONE ITERATION STEP
	   REQUIRES ONE EVALUATION OF F(X) AND ONE EVALUATION OF THE
	   DERIVATIVE OF F(X). FOR TEST ON SATISFACTORY ACCURACY SEE
           FORMULAE (2) OF MATHEMATICAL DESCRIPTION.
           FOR REFERENCE, SEE R. ZURMUEHL, PRAKTISCHE MATHEMATIK FUER
           INGENIEURE UND PHYSIKER, SPRINGER, BERLIN/GOETTINGEN/
	   HEIDELBERG, 1963, PP.12-17.

     ..................................................................
 */
double rtni(double * f,double *derf, double (*fct)(double,double *),
	   double xst,float eps,int iend,int *ier)
{
int    i;
double x, a, dx, tol, tolf;
/*     PREPARE ITERATION */
(*ier) = 0;
x = xst;
tol = x;
(*f)=(*fct)( tol, derf );
tolf = 100.*(eps);


/*     START ITERATION LOOP */
for( i = 1 ; i <= iend; i++ ){
	switch( for_if((*f)) ){
		case -1: goto L_1;
		case  0: goto L_7;
		case  1: goto L_1;
		}

	/*     EQUATION IS NOT SATISFIED BY X */
L_1:
	switch( for_if((*derf)) ){
		case -1: goto L_2;
		case  0: goto L_8;
		case  1: goto L_2;
		}

	/*     ITERATION IS POSSIBLE */
L_2:
	dx = (*f)/(*derf);
	x = x - dx;
	tol = x;
	(*f)=(*fct)( tol,derf );

	/*     TEST ON SATISFACTORY ACCURACY */
	tol = (eps);
	a = fabs( x );
	switch( for_if(a - 1.e0) ){
		case -1: goto L_4;
		case  0: goto L_4;
		case  1: goto L_3;
		}
L_3:
	tol = tol*a;
L_4:
	switch( for_if(fabs( dx ) - tol) ){
		case -1: goto L_5;
		case  0: goto L_5;
		case  1: goto L_6;
		}
L_5:
	switch( for_if(fabs( (*f) ) - tolf) ){
		case -1: goto L_7;
		case  0: goto L_7;
		case  1: goto L_6;
		}
L_6:
	;
	}
/*     END OF ITERATION LOOP


     NO CONVERGENCE AFTER IEND ITERATION STEPS. ERROR RETURN. */
(*ier) = 1;
L_7:
return x;

/*     ERROR RETURN IN CASE OF ZERO DIVISOR */
L_8:
(*ier) = 2;
return x;
} /* end of function */

