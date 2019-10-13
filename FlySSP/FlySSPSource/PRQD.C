/*------------------------------------------------------------------------
  PRQD.C
	   êÄëóÖí ÇëÖï ÑÖâëíÇàíÖãúçõï à äéåèãÖäëçõï äéêçÖâ
	   ÄãÉÖÅêÄàóÖëäéÉé èéãàçéåÄ
  Ä¢‚Æ‡  : è•‡•¢Æ§ Ø‡Æ£‡†¨¨Î Ø†™•‚† SSP ≠† ë ¢ÎØÆ´≠®´ è†·Î≠™Æ¢ Ç.Ç.
  Ç•‡·®Ô : 01.00 / 01.06.1994 /
------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE PRQD

	PURPOSE
           CALCULATE ALL REAL AND COMPLEX ROOTS OF A GIVEN POLYNOMIAL
	   WITH REAL COEFFICIENTS.

        USAGE
	   CALL PRQD(C,IC,Q,E,POL,IR,IER)

        DESCRIPTION OF PARAMETERS
           C     - COEFFICIENT VECTOR OF GIVEN POLYNOMIAL
                   COEFFICIENTS ARE ORDERED FROM LOW TO HIGH
                   THE GIVEN COEFFICIENT VECTOR GETS DIVIDED BY THE
                   LAST NONZERO TERM
	   IC    - DIMENSION OF VECTOR C
           Q     - WORKING STORAGE OF DIMENSION IC
                   ON RETURN Q CONTAINS REAL PARTS OF ROOTS
	   E     - WORKING STORAGE OF DIMENSION IC
		   ON RETURN E CONTAINS COMPLEX PARTS OF ROOTS
           POL   - WORKING STORAGE OF DIMENSION IC
                   ON RETURN POL CONTAINS THE COEFFICIENTS OF THE
                   POLYNOMIAL WITH CALCULATED ROOTS
                   THIS RESULTING COEFFICIENT VECTOR HAS DIMENSION IR+1
                   COEFFICIENTS ARE ORDERED FROM LOW TO HIGH
           IR    - NUMBER OF CALCULATED ROOTS
                   NORMALLY IR IS EQUAL TO DIMENSION IC MINUS ONE
	   IER   - RESULTING ERROR PARAMETER. SEE REMARKS

        REMARKS
	   THE REAL PART OF THE ROOTS IS STORED IN Q(1) UP TO Q(IR)
	   CORRESPONDING COMPLEX PARTS ARE STORED IN E(1) UP TO E(IR).
           IER = 0 MEANS NO ERRORS
           IER = 1 MEANS NO CONVERGENCE WITH FEASIBLE TOLERANCE
           IER = 2 MEANS POLYNOMIAL IS DEGENERATE (CONSTANT OR ZERO)
           IER = 3 MEANS SUBROUTINE WAS ABANDONED DUE TO ZERO DIVISOR
           IER = 4 MEANS THERE EXISTS NO S-FRACTION
           IER =-1 MEANS CALCULATED COEFFICIENT VECTOR REVEALS POOR
                   ACCURACY OF THE CALCULATED ROOTS.
		   THE CALCULATED COEFFICIENT VECTOR HAS LESS THAN
                   3 CORRECT DIGITS.
	   THE FINAL COMPARISON BETWEEN GIVEN AND CALCULATED
           COEFFICIENT VECTOR IS PERFORMED ONLY IF ALL ROOTS HAVE BEEN
           CALCULATED.
	   THE MAXIMAL RELATIVE ERROR OF THE COEFFICIENT VECTOR IS
           RECORDED IN Q(IR+1).

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
	   THE ROOTS OF THE POLYNOMIAL ARE CALCULATED BY MEANS OF
	   THE QUOTIENT-DIFFERENCE ALGORITHM WITH DISPLACEMENT.
	   REFERENCE
	   H.RUTISHAUSER, DER QUOTIENTEN-DIFFERENZEN-ALGORITHMUS,
	   BIRKHAEUSER, BASEL/STUTTGART, 1957.

     ..................................................................
 */
int prqd(double c[],int ic,double  q[],double  e[],double  pol[],int *ir)
{
int    i, iend, ista, j, jbeg, jend, k, kount, limit, nran, nsav,ier;
double eps, esav, expt, o, p, r, s, t, tdelt, teps, tol, u, v, w;


/*      DIMENSIONED DUMMY VARIABLES */

/*        NORMALIZATION OF GIVEN POLYNOMIAL
	   TEST OF DIMENSION
        IR CONTAINS INDEX OF HIGHEST COEFFICIENT */
p=t=w=0.0;
ier = 0;
(*ir) = ic;
eps = 1.e-6;
tol = 1.e-3;
limit = 10*ic;
kount = 0;
L_1:
switch( for_if((*ir) - 1) ){
	case -1: goto L_79;
	case  0: goto L_79;
	case  1: goto L_2;
	}

/*        DROP TRAILING ZERO COEFFICIENTS */
L_2:
switch( for_if(c[(*ir)-1]) ){
	case -1: goto L_4;
	case  0: goto L_3;
	case  1: goto L_4;
	}
L_3:
(*ir) = (*ir) - 1;
goto L_1;

/*           REARRANGEMENT OF GIVEN POLYNOMIAL
        EXTRACTION OF ZERO ROOTS */
L_4:
o = 1./c[(*ir)-1];
iend = (*ir) - 1;
ista = 1;
nsav = (*ir) + 1;
jbeg = 1;

/*        Q(J)=1.
        Q(J+I)=C(IR-I)/C(IR)
        Q(IR)=C(J)/C(IR)
        WHERE J IS THE INDEX OF THE LOWEST NONZERO COEFFICIENT */
for( i = 1; i <= (*ir); i++ ){
	j = nsav - i;
	switch( for_if(c[i-1]) ){
		case -1: goto L_7;
		case  0: goto L_5;
		case  1: goto L_7;
		}
L_5:
	switch( jbeg ){
		case 1: goto L_6;
		case 2: goto L_8;
		}
L_6:
	nsav = nsav + 1;
	q[ista-1] = 0.;
	e[ista-1] = 0.;
	ista = ista + 1;
	goto L_9;
L_7:
	jbeg = 2;
L_8:
	q[j-1] = c[i-1]*o;
	c[i-1] = q[j-1];
L_9:
	;
	}

/*           INITIALIZATION */
esav = 0.;
q[ista-1] = 0.;
L_10:
nsav = (*ir);

/*        COMPUTATION OF DERIVATIVE */
expt = (*ir) - ista;
e[ista-1] = expt;
for( i = ista; i <= iend; i++ ){
	expt = expt - 1.0;
	pol[i + 1-1] = eps*fabs( q[i + 1-1] ) + eps;
	e[i + 1-1] = q[i + 1-1]*expt;
	}

/*        TEST OF REMAINING DIMENSION */
switch( for_if(ista - iend) ){
	case -1: goto L_12;
	case  0: goto L_20;
	case  1: goto L_60;
	}
L_12:
jend = iend - 1;

/*        COMPUTATION OF S-FRACTION */
for( i = ista; i <= jend; i++ ){
	switch( for_if(i - ista) ){
		case -1: goto L_13;
		case  0: goto L_16;
		case  1: goto L_13;
		}
L_13:
	switch( for_if(fabs( e[i-1] ) - pol[i + 1-1]) ){
		case -1: goto L_14;
		case  0: goto L_14;
		case  1: goto L_16;
		}

	/*        THE GIVEN POLYNOMIAL HAS MULTIPLE ROOTS, THE COEFFICIENTS OF
		THE COMMON FACTOR ARE STORED FROM Q(NSAV) UP TO Q(IR) */
L_14:
	nsav = i;
	for( k = i; k <= jend; k++ ){
		switch( for_if(fabs( e[k-1] ) - pol[k + 1-1]) ){
			case -1: goto L_15;
			case  0: goto L_15;
			case  1: goto L_80;
			}
L_15:
		;
		}
	goto L_21;

	/*           EUCLIDEAN ALGORITHM */
L_16:
	for( k = i; k <= iend; k++ ){
		e[k + 1-1] = e[k + 1-1]/e[i-1];
		q[k + 1-1] = e[k + 1-1] - q[k + 1-1];
		switch( for_if(k - i) ){
			case -1: goto L_18;
			case  0: goto L_17;
			case  1: goto L_18;
			}

		/*        TEST FOR SMALL DIVISOR */
L_17:
		switch( for_if(fabs( q[i + 1-1] ) - pol[i + 1-1]) ){
			case -1: goto L_80;
			case  0: goto L_80;
			case  1: goto L_19;
			}
L_18:
		q[k + 1-1] = q[k + 1-1]/q[i + 1-1];
		pol[k + 1-1] = pol[k + 1-1]/fabs( q[i + 1-1] );
		e[k-1] = q[k + 1-1] - e[k-1];
L_19:
		;
		}
	}
L_20:
q[(*ir)-1] = -q[(*ir)-1];

/*           THE DISPLACEMENT EXPT IS SET TO 0 AUTOMATICALLY.
           E(ISTA)=0.,Q(ISTA+1),...,E(NSAV-1),Q(NSAV),E(NSAV)=0.,
           FORM A DIAGONAL OF THE QD-ARRAY.
        INITIALIZATION OF BOUNDARY VALUES */
L_21:
e[ista-1] = 0.;
nran = nsav - 1;
L_22:
e[nran + 1-1] = 0.;

/*           TEST FOR LINEAR OR CONSTANT FACTOR
        NRAN-ISTA IS DEGREE-1 */
switch( for_if(nran - ista) ){
	case -1: goto L_24;
	case  0: goto L_23;
	case  1: goto L_31;
	}

/*        LINEAR FACTOR */
L_23:
q[ista + 1-1] = q[ista + 1-1] + expt;
e[ista + 1-1] = 0.;

/*        TEST FOR UNFACTORED COMMON DIVISOR */
L_24:
e[ista-1] = esav;
switch( for_if((*ir) - nsav) ){
	case -1: goto L_60;
	case  0: goto L_60;
	case  1: goto L_25;
	}

/*        INITIALIZE QD-ALGORITHM FOR COMMON DIVISOR */
L_25:
ista = nsav;
esav = e[ista-1];
goto L_10;

/*        COMPUTATION OF ROOT PAIR */
L_26:
p = p + expt;

/*           TEST FOR REALITY */
switch( for_if(o) ){
	case -1: goto L_27;
	case  0: goto L_28;
	case  1: goto L_28;
	}

/*           COMPLEX ROOT PAIR */
L_27:
q[nran-1] = p;
q[nran + 1-1] = p;
e[nran-1] = t;
e[nran + 1-1] = -t;
goto L_29;

/*           REAL ROOT PAIR */
L_28:
q[nran-1] = p - t;
q[nran + 1-1] = p + t;
e[nran-1] = 0.;

/*           REDUCTION OF DEGREE BY 2 (DEFLATION) */
L_29:
nran = nran - 2;
goto L_22;

/*        COMPUTATION OF REAL ROOT */
L_30:
q[nran + 1-1] = expt + p;

/*           REDUCTION OF DEGREE BY 1 (DEFLATION) */
nran = nran - 1;
goto L_22;

/*        START QD-ITERATION */
L_31:
jbeg = ista + 1;
jend = nran - 1;
teps = eps;
tdelt = 1.e-2;
L_32:
kount = kount + 1;
p = q[nran + 1-1];
r = fabs( e[nran-1] );

/*           TEST FOR CONVERGENCE */
switch( for_if(r - teps) ){
	case -1: goto L_30;
	case  0: goto L_30;
	case  1: goto L_33;
	}
L_33:
s = fabs( e[jend-1] );

/*        IS THERE A REAL ROOT NEXT */
switch( for_if(s - r) ){
	case -1: goto L_38;
	case  0: goto L_38;
	case  1: goto L_34;
	}

/*        IS DISPLACEMENT SMALL ENOUGH */
L_34:
switch( for_if(r - tdelt) ){
	case -1: goto L_36;
	case  0: goto L_35;
	case  1: goto L_35;
	}
L_35:
p = 0.;
L_36:
o = p;
for( j = jbeg; j <= nran; j++ ){
	q[j-1] = q[j-1] + e[j-1] - e[j - 1-1] - o;

	/*           TEST FOR SMALL DIVISOR */
	switch( for_if(fabs( q[j-1] ) - pol[j-1]) ){
		case -1: goto L_81;
		case  0: goto L_81;
		case  1: goto L_37;
		}
L_37:
	e[j-1] = q[j + 1-1]*e[j-1]/q[j-1];
	}
q[nran + 1-1] = -e[nran-1] + q[nran + 1-1] - o;
goto L_54;

/*        CALCULATE DISPLACEMENT FOR DOUBLE ROOTS
           QUADRATIC EQUATION FOR DOUBLE ROOTS
           X**2-(Q(NRAN)+Q(NRAN+1)+E(NRAN))*X+Q(NRAN)*Q(NRAN+1)=0 */
L_38:
p = 0.5*(q[nran-1] + e[nran-1] + q[nran + 1-1]);
o = p*p - q[nran-1]*q[nran + 1-1];
t = sqrt( fabs( o ) );

/*        TEST FOR CONVERGENCE */
switch( for_if(s - teps) ){
	case -1: goto L_26;
	case  0: goto L_26;
	case  1: goto L_39;
	}

/*        ARE THERE COMPLEX ROOTS */
L_39:
switch( for_if(o) ){
	case -1: goto L_43;
	case  0: goto L_40;
	case  1: goto L_40;
	}
L_40:
switch( for_if(p) ){
	case -1: goto L_42;
	case  0: goto L_41;
	case  1: goto L_41;
	}
L_41:
t = -t;
L_42:
p = p + t;
r = s;
goto L_34;

/*        MODIFICATION FOR COMPLEX ROOTS
        IS DISPLACEMENT SMALL ENOUGH */
L_43:
switch( for_if(s - tdelt) ){
	case -1: goto L_44;
	case  0: goto L_35;
	case  1: goto L_35;
	}

/*        INITIALIZATION */
L_44:
o = q[jbeg-1] + e[jbeg-1] - p;

/*        TEST FOR SMALL DIVISOR */
switch( for_if(fabs( o ) - pol[jbeg-1]) ){
	case -1: goto L_81;
	case  0: goto L_81;
	case  1: goto L_45;
	}
L_45:
t = pow(t/o, 2);
u = e[jbeg-1]*q[jbeg + 1-1]/(o*(1. + t));
v = o + u;
kount = kount + 2;

/*        THREEFOLD LOOP FOR COMPLEX DISPLACEMENT */
for( j = jbeg; j <= nran; j++ ){
	o = q[j + 1-1] + e[j + 1-1] - u - p;

	/*        TEST FOR SMALL DIVISOR */
	switch( for_if(fabs( v ) - pol[j-1]) ){
		case -1: goto L_46;
		case  0: goto L_46;
		case  1: goto L_49;
		}
L_46:
	switch( for_if(j - nran) ){
		case -1: goto L_81;
		case  0: goto L_47;
		case  1: goto L_81;
		}
L_47:
	expt = expt + p;
	switch( for_if(fabs( e[jend-1] ) - tol) ){
		case -1: goto L_48;
		case  0: goto L_48;
		case  1: goto L_81;
		}
L_48:
	p = 0.5*(v + o - e[jend-1]);
	o = p*p - (v - u)*(o - u*t - o*w*(1. + t)/q[jend-1]);
	t = sqrt( fabs( o ) );
	goto L_26;

	/*           TEST FOR SMALL DIVISOR */
L_49:
	switch( for_if(fabs( o ) - pol[j + 1-1]) ){
		case -1: goto L_46;
		case  0: goto L_46;
		case  1: goto L_50;
		}
L_50:
	w = u*o/v;
	t = t*pow(v/o, 2);
	q[j-1] = v + w - e[j - 1-1];
	u = 0.;
	switch( for_if(j - nran) ){
		case -1: goto L_51;
		case  0: goto L_52;
		case  1: goto L_52;
		}
L_51:
	u = q[j + 2-1]*e[j + 1-1]/(o*(1. + t));
L_52:
	v = o + u - w;

	/*        TEST FOR SMALL DIVISOR */
	switch( for_if(fabs( q[j-1] ) - pol[j-1]) ){
		case -1: goto L_81;
		case  0: goto L_81;
		case  1: goto L_53;
		}
L_53:
	e[j-1] = w*v*(1. + t)/q[j-1];
	}
q[nran + 1-1] = v - e[nran-1];
L_54:
expt = expt + p;
teps = teps*1.1;
tdelt = tdelt*1.1;
switch( for_if(kount - limit) ){
	case -1: goto L_32;
	case  0: goto L_55;
	case  1: goto L_55;
	}

/*        NO CONVERGENCE WITH FEASIBLE TOLERANCE
           ERROR RETURN IN CASE OF UNSATISFACTORY CONVERGENCE */
L_55:
ier = 1;

/*        REARRANGE CALCULATED ROOTS */
L_56:
iend = nsav - nran - 1;
e[ista-1] = esav;
switch( for_if(iend) ){
	case -1: goto L_59;
	case  0: goto L_59;
	case  1: goto L_57;
	}
L_57:
for( i = 1; i <= iend; i++ ){
	j = ista + i;
	k = nran + 1 + i;
	e[j-1] = e[k-1];
	q[j-1] = q[k-1];
	}
L_59:
(*ir) = ista + iend;

/*        NORMAL RETURN */
L_60:
(*ir) = (*ir) - 1;
switch( for_if((*ir)) ){
	case -1: goto L_78;
	case  0: goto L_78;
	case  1: goto L_61;
	}

/*        REARRANGE CALCULATED ROOTS */
L_61:
for( i = 1; i <= (*ir); i++ ){
	q[i-1] = q[i + 1-1];
	e[i-1] = e[i + 1-1];
	}

/*        CALCULATE COEFFICIENT VECTOR FROM ROOTS */
pol[(*ir) + 1-1] = 1.;
iend = (*ir) - 1;
jbeg = 1;
for( j = 1; j <= (*ir); j++ ){
	ista = (*ir) + 1 - j;
	o = 0.;
	p = q[ista-1];
	t = e[ista-1];
	switch( for_if(t) ){
		case -1: goto L_65;
		case  0: goto L_63;
		case  1: goto L_65;
		}

	/*        MULTIPLY WITH LINEAR FACTOR */
L_63:
	for( i = ista; i <= (*ir); i++ ){
		pol[i-1] = o - p*pol[i + 1-1];
		o = pol[i + 1-1];
		}
	goto L_69;
L_65:
	switch( jbeg ){
		case 1: goto L_66;
		case 2: goto L_67;
		}
L_66:
	jbeg = 2;
	pol[ista-1] = 0.;
	goto L_69;

	/*        MULTIPLY WITH QUADRATIC FACTOR */
L_67:
	jbeg = 1;
	u = p*p + t*t;
	p = p + p;
	for( i = ista; i <= iend; i++ ){
		pol[i-1] = o - p*pol[i + 1-1] + u*pol[i + 2-1];
		o = pol[i + 1-1];
		}
	pol[(*ir)-1] = o - p;
L_69:
	;
	}
switch( for_if(ier) ){
	case -1: goto L_78;
	case  0: goto L_70;
	case  1: goto L_78;
	}

/*        COMPARISON OF COEFFICIENT VECTORS, IE. TEST OF ACCURACY */
L_70:
p = 0.;
for( i = 1; i <= (*ir); i++ ){
	switch( for_if(c[i-1]) ){
		case -1: goto L_72;
		case  0: goto L_71;
		case  1: goto L_72;
		}
L_71:
	o = fabs( pol[i-1] );
	goto L_73;
L_72:
	o = fabs( (pol[i-1] - c[i-1])/c[i-1] );
L_73:
	switch( for_if(p - o) ){
		case -1: goto L_74;
		case  0: goto L_75;
		case  1: goto L_75;
		}
L_74:
	p = o;
L_75:
	;
	}
switch( for_if(p - tol) ){
	case -1: goto L_77;
	case  0: goto L_76;
	case  1: goto L_76;
	}
L_76:
ier = -1;
L_77:
q[(*ir) + 1-1] = p;
e[(*ir) + 1-1] = 0.;
L_78:
return ier;

/*        ERROR RETURNS
	   ERROR RETURN FOR POLYNOMIALS OF DEGREE LESS THAN 1 */
L_79:
ier = 2;
(*ir) = 0;
return ier;

/*           ERROR RETURN IF THERE EXISTS NO S-FRACTION */
L_80:
ier = 4;
(*ir) = ista;
goto L_60;

/*           ERROR RETURN IN CASE OF INSTABLE QD-ALGORITHM */
L_81:
ier = 3;
goto L_56;
}

