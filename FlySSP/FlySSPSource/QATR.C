/*---------------------------------------------------------------------------
  QATR.C
	àçíÖÉêàêéÇÄçàÖ îìçäñàà èé èêÄÇàãì íêÄèÖñàâ ë ùäëíêÄèéãüñàÖâ
	èé åÖíéÑì êéåÅÖêÉÄ
  Ä¢‚Æ‡   : è•‡•¢Æ§ Ø‡Æ£‡†¨¨Î Ø†™•‚† SSP ≠† ë ¢ÎØÆ´≠®´ è†·Î≠™Æ¢ Ç.Ç.
  Ç•‡·®Ô  : 01.00 / 10.08.1997 /
---------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	SUBROUTINE QATR


	PURPOSE
	   TO COMPUTE AN APPROXIMATION FOR INTEGRAL(FCT(X), SUMMED
	   OVER X FROM XL TO XU).

	USAGE
	   QATR (XL,XU,EPS,NDIM,FCT,Y,IER,AUX)
	   PARAMETER FCT REQUIRES AN EXTERNAL STATEMENT.

	DESCRIPTION OF PARAMETERS
	   XL     - DOUBLE PRECISION LOWER BOUND OF THE INTERVAL.
           XU     - DOUBLE PRECISION UPPER BOUND OF THE INTERVAL.
           EPS    - SINGLE PRECISION UPPER BOUND OF THE ABSOLUTE ERROR.
	   NDIM   - THE DIMENSION OF THE AUXILIARY STORAGE ARRAY AUX.
                    NDIM-1 IS THE MAXIMAL NUMBER OF BISECTIONS OF
                    THE INTERVAL (XL,XU).
	   FCT    - THE NAME OF THE EXTERNAL DOUBLE PRECISION FUNCTION
		    SUBPROGRAM USED.
           Y      - RESULTING DOUBLE PRECISION APPROXIMATION FOR THE
                    INTEGRAL VALUE.
           IER    - A RESULTING ERROR PARAMETER.
           AUX    - AUXILIARY DOUBLE PRECISION STORAGE ARRAY WITH
                    DIMENSION NDIM.

        REMARKS
           ERROR PARAMETER IER IS CODED IN THE FOLLOWING FORM
	   IER=0  - IT WAS POSSIBLE TO REACH THE REQUIRED ACCURACY.
                    NO ERROR.
           IER=1  - IT IS IMPOSSIBLE TO REACH THE REQUIRED ACCURACY
		    BECAUSE OF ROUNDING ERRORS.
	   IER=2  - IT WAS IMPOSSIBLE TO CHECK ACCURACY BECAUSE NDIM
                    IS LESS THAN 5, OR THE REQUIRED ACCURACY COULD NOT
                    BE REACHED WITHIN NDIM-1 STEPS. NDIM SHOULD BE
                    INCREASED.

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           THE EXTERNAL DOUBLE PRECISION FUNCTION SUBPROGRAM FCT(X)
           MUST BE CODED BY THE USER. ITS DOUBLE PRECISION ARGUMENT X
           SHOULD NOT BE DESTROYED.

        METHOD
           EVALUATION OF Y IS DONE BY MEANS OF TRAPEZOIDAL RULE IN
	   CONNECTION WITH ROMBERGS PRINCIPLE. ON RETURN Y CONTAINS
	   THE BEST POSSIBLE APPROXIMATION OF THE INTEGRAL VALUE AND
           VECTOR AUX THE UPWARD DIAGONAL OF ROMBERG SCHEME.
           COMPONENTS AUX(I) (I=1,2,...,IEND, WITH IEND LESS THAN OR
           EQUAL TO NDIM) BECOME APPROXIMATIONS TO INTEGRAL VALUE WITH
           DECREASING ACCURACY BY MULTIPLICATION WITH (XU-XL).
           FOR REFERENCE, SEE
           (1) FILIPPI, DAS VERFAHREN VON ROMBERG-STIEFEL-BAUER ALS
               SPEZIALFALL DES ALLGEMEINEN PRINZIPS VON RICHARDSON,
               MATHEMATIK-TECHNIK-WIRTSCHAFT, VOL.11, ISS.2 (1964),
	       PP.49-54.
	   (2) BAUER, ALGORITHM 60, CACM, VOL.4, ISS.6 (1961), PP.255.

     ..................................................................
 */
int qatr(double xl,double xu,double eps,int  ndim,
	 double (*fct)(double),double *y,double aux[])
{
int    ier,i, ii, j, ji, jj;
double delt1, delt2, e,
       h, hd, hh, p, q, sm, x;




/*     PREPARATIONS OF ROMBERG-LOOP */
aux[0] = .5e0*((*fct)( xl ) + (*fct)( xu ));
h = xu - xl;
switch( for_if( ndim - 1) ){
	case -1: goto L_8;
	case  0: goto L_8;
	case  1: goto L_1;
	}
L_1:
switch( for_if(h) ){
	case -1: goto L_2;
	case  0: goto L_10;
	case  1: goto L_2;
	}

/*     NDIM IS GREATER THAN 1 AND H IS NOT EQUAL TO 0. */
L_2:
hh = h;
e = eps /fabs( h );
delt2 = 0.;
p = 1.e0;
jj = 1;
for( i = 2 ; i <= ndim; i++ ){
	(*y) = aux[0];
	delt1 = delt2;
	hd = hh;
	hh = .5e0*hh;
	p = .5e0*p;
	x = xl + hh;
	sm = 0.e0;
	for( j = 1; j <= jj; j++ ){
		sm = sm + (*fct)( x );
		x = x + hd;
		}
	aux[i-1] = .5e0*aux[i - 1-1] + p*sm;
	/*     A NEW APPROXIMATION OF INTEGRAL VALUE IS COMPUTED BY MEANS OF
	     TRAPEZOIDAL RULE.

	     START OF ROMBERGS EXTRAPOLATION METHOD. */
	q = 1.e0;
	ji = i - 1;
	for( j = 1 ; j <= ji; j++ ){
		ii = i - j;
		q = q + q;
		q = q + q;
		aux[ii-1] = aux[ii + 1-1] + (aux[ii + 1-1] - aux[ii-1])/(q -
		 1.e0);
		}
	/*     END OF ROMBERG-STEP
	 */
	delt2 = fabs( (*y) - aux[0] );
	switch( for_if(i - 5) ){
		case -1: goto L_7;
		case  0: goto L_5;
		case  1: goto L_5;
		}
L_5:
	switch( for_if(delt2 - e) ){
		case -1: goto L_10;
		case  0: goto L_10;
		case  1: goto L_6;
		}
L_6:
	switch( for_if(delt2 - delt1) ){
		case -1: goto L_7;
		case  0: goto L_11;
		case  1: goto L_11;
		}
L_7:
	jj = jj + jj;
	}
L_8:
ier = 2;
L_9:
(*y) = h*aux[0];
return ier;
L_10:
ier = 0;
goto L_9;
L_11:
ier = 1;
(*y) = h*(*y);
return ier;
}

