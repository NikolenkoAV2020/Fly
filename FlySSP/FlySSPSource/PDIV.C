//------------------------------------------------------------------------
//   PDIV.C
//     Дeлeниe oднoгo пoлинoмa нa дpугoй
//   Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
//   Версия : 01.00 / 10.06.1996 /
//------------------------------------------------------------------------
#include "ssp.h"
/*     ..................................................................

        SUBROUTINE PDIV

        PURPOSE
	   DIVIDE ONE POLYNOMIAL BY ANOTHER

        USAGE
	   PDIV(P,IDIMP,X,IDIMX,Y,IDIMY,TOL,IER)

        DESCRIPTION OF PARAMETERS
           P     - RESULTANT VECTOR OF INTEGRAL PART
           IDIMP - DIMENSION OF P
           X     - VECTOR OF COEFFICIENTS FOR DIVIDEND POLYNOMIAL,
                   ORDERED FROM SMALLEST TO LARGEST POWER. IT IS
                   REPLACED BY REMAINDER AFTER DIVISION.
	   IDIMX - DIMENSION OF X
           Y     - VECTOR OF COEFFICIENTS FOR DIVISOR POLYNOMIAL,
                   ORDERED FROM SMALLEST TO LARGEST POWER
	   IDIMY - DIMENSION OF Y
           TOL   - TOLERANCE VALUE BELOW WHICH COEFFICIENTS ARE
		   ELIMINATED DURING NORMALIZATION
           IER   - ERROR CODE. 0 IS NORMAL, 1 IS FOR ZERO DIVISOR

        REMARKS
           THE REMAINDER R REPLACES X.
           THE DIVISOR Y REMAINS UNCHANGED.
	   IF DIMENSION OF Y EXCEEDS DIMENSION OF X, IDIMP IS SET TO
           ZERO AND CALCULATION IS BYPASSED

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           PNORM

	METHOD
	   POLYNOMIAL X IS DIVIDED BY POLYNOMIAL Y GIVING INTEGER PART
	   P AND REMAINDER R SUCH THAT X = P*Y + R.
	   DIVISOR Y AND REMAINDER  VECTOR GET NORMALIZED.

     ..................................................................
 */
int pdiv(double p[], int *idimp, double x[], int idimx, double y[], int idimy,
	 double tol)
{
int  ier;
int  i, ii, j, k;


idimy=pnorm( y, idimy, tol );
switch( for_if(idimy) ){
	case -1: goto L_50;
	case  0: goto L_50;
	case  1: goto L_10;
	}
L_10:
(*idimp) = idimx - idimy + 1;
switch( for_if((*idimp)) ){
	case -1: goto L_20;
	case  0: goto L_30;
	case  1: goto L_60;
	}

/*     DEGREE OF DIVISOR WAS GREATER THAN DEGREE OF DIVIDEND
 */
L_20:
(*idimp) = 0;
L_30:
ier = 0;
L_40:
return ier;

/*     Y IS ZERO POLYNOMIAL
 */
L_50:
ier = 1;
goto L_40;

/*     START REDUCTION
 */
L_60:
idimx = idimy - 1;
i = (*idimp);
L_70:
ii = i + idimx;
p[i-1] = x[ii-1]/y[idimy-1];

/*     SUBTRACT MULTIPLE OF DIVISOR
 */
for( k = 1; k <= idimx; k++ ){
	j = k - 1 + i;
	x[j-1] = x[j-1] - p[i-1]*y[k-1];
	}
i = i - 1;
switch( for_if(i) ){
	case -1: goto L_90;
	case  0: goto L_90;
	case  1: goto L_70;
	}

/*     NORMALIZE REMAINDER POLYNOMIAL
 */
L_90:
idimx=pnorm( x, idimx, tol );
goto L_30;
}

