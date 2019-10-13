/*------------------------------------------------------------------------
  DBAR.C
    ÑàîîÖêÖçñàêéÇÄÖàÖ îìçäñàà çÄ ÉêÄçàñÖ àçíÖêÇÄãÄ
    åÖíéÑéå ùäëíêÄèéãüñàà êàóÄêÑëéçÄ à êéåÅÖêÉÄ
  Ä¢‚Æ‡   : è•‡•¢Æ§ Ø‡Æ£‡†¨¨Î Ø†™•‚† SSP ≠† ë ¢ÎØÆ´≠®´ è†·Î≠™Æ¢ Ç.Ç.
  Ç•‡·®Ô  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/*     ..................................................................

     SUBROUTINE DBAR

     PURPOSE
        TO COMPUTE, AT A GIVEN POINT X, AN APPROXIMATION Z TO THE
        DERIVATIVE OF AN ANALYTICALLY GIVEN FUNCTION FCT THAT IS 11-
	TIMES DIFFERENTIABLE IN A DOMAIN CONTAINING A CLOSED INTERVAL -
        THE SET OF T BETWEEN X AND X+H (H POSITIVE OR NEGATIVE) - USING
        FUNCTION VALUES ONLY ON THAT INTERVAL.

      USAGE
        CALL DBAR(X,H,IH,FCT,Z)
        PARAMETER FCT REQUIRES AN EXTERNAL STATEMENT

     DESCRIPTION OF PARAMETERS
	X   - THE POINT AT WHICH THE DERIVATIVE IS TO BE COMPUTED
        H   - THE NUMBER THAT DEFINES THE CLOSED INTERVAL WHOSE END-
              POINTS ARE X AND X+H (SEE PURPOSE)
        IH  - INPUT PARAMETER (SEE REMARKS AND METHOD)
	      IH NON-ZERO - THE SUBROUTINE GENERATES THE INTERNAL
                            VALUE HH
              IH    =   0 - THE INTERNAL VALUE HH IS SET TO H
        FCT - THE NAME OF THE EXTERNAL FUNCTION SUBPROGRAM THAT WILL
              GENERATE THE NECESSARY FUNCTION VALUES
        Z   - RESULTING DERIVATIVE VALUE

     REMARKS
        (1)  IF H = 0, THEN THERE IS NO COMPUTATION.
	(2)  THE (MAGNITUDE OF THE) INTERNAL VALUE HH, WHICH IS DETER-
             MINED ACCORDING TO IH, IS THE MAXIMUM STEP-SIZE USED IN
             THE COMPUTATION OF THE ONE-SIDED DIVIDED DIFFERENCES (SEE
             METHOD.)  IF IH IS NON-ZERO, THEN THE SUBROUTINE GENERATES
	     HH ACCORDING TO CRITERIA THAT BALANCE ROUND-OFF AND TRUN-
             CATION ERROR.  HH ALWAYS HAS THE SAME SIGN AS H AND IT IS
             ALWAYS LESS THAN OR EQUAL TO THE MAGNITUDE OF H IN AB-
             SOLUTE VALUE, SO THAT ALL COMPUTATION OCCURS IN THE CLOSED
             INTERVAL DETERMINED BY H.

     SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
        THE EXTERNAL FUNCTION SUBPROGRAM FCT(T) MUST BE FURNISHED BY
        THE USER.

     METHOD
        THE COMPUTATION OF Z IS BASED ON RICHARDSON'S AND ROMBERG'S
        EXTRAPOLATION METHOD AS APPLIED TO THE SEQUENCE OF ONE-SIDED
	DIVIDED DIFFERENCES ASSOCIATED WITH THE POINT PAIRS
	(X,X+(K*HH)/10)K=1,...,10.  (SEE FILLIPI, S. AND ENGELS, H.,
	ALTES UND NEUES ZUR NUMERISCHEN DIFFERENTIATION, ELECTRONISCHE
	DATENVERARBEITUNG, ISS. 2 (1966), PP. 57-65.)

     ..................................................................
 */
double dbar(double x, double h, int ih, double (*fct)(double))
{
int     i, j, jj;
double  a, aux[10], b, c, d, d1, d2, dh, dz, hh, z;

/*        NO ACTION IN CASE OF ZERO INTERVAL LENGTH */
switch( for_if(h) ){
	case -1: goto L_1;
	case  0: goto L_17;
	case  1: goto L_1;
	}

/*        GENERATE STEPSIZE HH FOR DIVIDED DIFFERENCES */
L_1:
c = fabs( h );
b = h;
d = x;
d = fct( d );
switch( for_if(ih) ){
	case -1: goto L_2;
	case  0: goto L_9;
	case  1: goto L_2;
	}
L_2:
hh = .5;
switch( for_if(c - hh) ){
	case -1: goto L_3;
	case  0: goto L_4;
	case  1: goto L_4;
	}
L_3:
hh = b;
L_4:
hh = Sign2(hh, b);
z = fabs( (fct( x+ hh ) - d)/hh );
a = fabs( d );
hh = 1.;
switch( for_if(a - 1.) ){
	case -1: goto L_6;
	case  0: goto L_6;
	case  1: goto L_5;
	}
L_5:
hh = hh*a;
L_6:
switch( for_if( z - 1.) ){
	case -1: goto L_8;
	case  0: goto L_8;
	case  1: goto L_7;
	}
L_7:
hh = hh/z;
L_8:
switch( for_if(hh - c) ){
	case -1: goto L_10;
	case  0: goto L_10;
	case  1: goto L_9;
	}
L_9:
hh = b;
L_10:
hh = Sign2(hh, b);

/*        INITIALIZE DIFFERENTIATION LOOP */
z = (fct( x+ hh ) - d)/hh;
j = 10;
jj = j - 1;
aux[j-1] = z;
dh = hh/(float)( j );
dz = 1.e38;

/*        START DIFFERENTIATION LOOP */
L_11:
j = j - 1;
c = j;
hh = c*dh;
aux[j-1] = ( fct(x + hh) - d)/hh;

/*        INITIALIZE EXTRAPOLATION LOOP */
d2 = 1.e38;
b = 0.;
a = 1./c;

/*        START EXTRAPOLATION LOOP */
for( i = j; i <= jj; i++ ){
	d1 = d2;
	b = b + a;
	hh = (aux[i-1] - aux[i + 1-1])/b;
	aux[i + 1-1] = aux[i-1] + hh;

	/*        TEST ON OSCILLATING INCREMENTS */
	d2 = fabs( hh );
	switch( for_if(d2 - d1) ){
		case -1: goto L_12;
		case  0: goto L_13;
		case  1: goto L_13;
		}
L_12:
	;
	}
/*        END OF EXTRAPOLATION LOOP

        UPDATE RESULT VALUE Z */
i = jj + 1;
goto L_14;
L_13:
d2 = d1;
jj = i;
L_14:
switch( for_if(d2 - dz) ){
	case -1: goto L_15;
	case  0: goto L_16;
	case  1: goto L_16;
	}
L_15:
dz = d2;
z = aux[i-1];
L_16:
switch( for_if(j - 1) ){
	case -1: goto L_17;
	case  0: goto L_17;
	case  1: goto L_11;
	}
/*        END OF DIFFERENTIATION LOOP
 */
L_17:
return z;
}

