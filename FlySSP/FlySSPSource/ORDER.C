/*   ORDER.C
     îéêåàêéÇÄçàÖ àá èéãçéâ åÄíêàñõ äéÇÄêàÄñàâ èéÑåÄíêàñ
     äéùîîàñàÖçíéÇ äéêêÖãüñàà åÖÜÑì çÖáÄÇàëàåõåà èÖêÖåÖççõåà
     à ÇÖäíéêÄ ÇáÄàåçõï äéêêÖãüñàâ åÖÜÑì çÖáÄÇàëàåõåà èÖêÖåÖççõåà
     à ëÄåéâ èÖêÖåÖççéâ
*/
//#include <stdio.h>
//#include <math.h>
#include "ssp.h"
/*     ..................................................................

        SUBROUTINE ORDER

        PURPOSE
           CONSTRUCT FROM A LARGER MATRIX OF CORRELATION COEFFICIENTS
	   A SUBSET MATRIX OF INTERCORRELATIONS AMONG INDEPENDENT
           VARIABLES AND A VECTOR OF INTERCORRELATIONS OF INDEPENDENT
	   VARIABLES WITH DEPENDENT VARIABLE.  THIS SUBROUTINE IS
           NORMALLY USED IN THE PERFORMANCE OF MULTIPLE AND POLYNOMIAL
	   REGRESSION ANALYSES.

        USAGE
           CALL ORDER (M,R,NDEP,K,ISAVE,RX,RY)

        DESCRIPTION OF PARAMETERS
           M     - NUMBER OF VARIABLES AND ORDER OF MATRIX R.
	   R     - INPUT MATRIX CONTAINING CORRELATION COEFFICIENTS.
                   THIS SUBROUTINE EXPECTS ONLY UPPER TRIANGULAR
		   PORTION OF THE SYMMETRIC MATRIX TO BE STORED (BY
		   COLUMN) IN R.  (STORAGE MODE OF 1)
           NDEP  - THE SUBSCRIPT NUMBER OF THE DEPENDENT VARIABLE.
	   K     - NUMBER OF INDEPENDENT VARIABLES TO BE INCLUDED
                   IN THE FORTHCOMING REGRESSION. K MUST BE GREATER
                   THAN OR EQUAL TO 1.
           ISAVE - INPUT VECTOR OF LENGTH K+1 CONTAINING, IN ASCENDING
                   ORDER, THE SUBSCRIPT NUMBERS OF K INDEPENDENT
                   VARIABLES TO BE INCLUDED IN THE FORTHCOMING REGRES-
                   SION.
                   UPON RETURNING TO THE CALLING ROUTINE, THIS VECTOR
		   CONTAINS, IN ADDITION, THE SUBSCRIPT NUMBER OF
                   THE DEPENDENT VARIABLE IN K+1 POSITION.
	   RX    - OUTPUT MATRIX (K X K) CONTAINING INTERCORRELATIONS
		   AMONG INDEPENDENT VARIABLES TO BE USED IN FORTH-
		   COMING REGRESSION.
           RY    - OUTPUT VECTOR OF LENGTH K CONTAINING INTERCORRELA-
                   TIONS OF INDEPENDENT VARIABLES WITH DEPENDENT
                   VARIABLES.

        REMARKS
           NONE

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
	   FROM THE SUBSCRIPT NUMBERS OF THE VARIABLES TO BE INCLUDED
	   IN THE FORTHCOMING REGRESSION, THE SUBROUTINE CONSTRUCTS THE
	   MATRIX RX AND THE VECTOR RY.

     ..................................................................
 */
//#pragma argsused
void order(int m, double r[], int ndep, int k, int isave[], double rx[], double ry[])
{
int  i, j, l, l1, l2, mm;
/*        ...............................................................

	IF A DOUBLE PRECISION VERSION OF THIS ROUTINE IS DESIRED, THE
	C IN COLUMN 1 SHOULD BE REMOVED FROM THE DOUBLE PRECISION
	STATEMENT WHICH FOLLOWS.

     DOUBLE PRECISION R,RX,RY

	THE C MUST ALSO BE REMOVED FROM DOUBLE PRECISION STATEMENTS
	APPEARING IN OTHER ROUTINES USED IN CONJUNCTION WITH THIS
	ROUTINE.

	...............................................................

     COPY INTERCORRELATIONS OF INDEPENDENT VARIABLES
     WITH DEPENDENT VARIABLE
 */
m=0;
mm = m;
for( j = 1; j <= k ; j++ ){
	l2 = isave[j-1];
	switch( for_if(ndep - l2) ){
		case -1: goto L_122;
		case  0: goto L_123;
		case  1: goto L_123;
		}
L_122:
	l = ndep + (l2*l2 - l2)/2;
	goto L_125;
L_123:
	l = l2 + (ndep*ndep - ndep)/2;
L_125:
	ry[j-1] = r[l-1];

	/*     COPY A SUBSET MATRIX OF INTERCORRELATIONS AMONG
	     INDEPENDENT VARIABLES
	 */
	for( i = 1 ; i <= k; i++ ){
		l1 = isave[i-1];
		switch( for_if(l1 - l2) ){
			case -1: goto L_127;
			case  0: goto L_128;
			case  1: goto L_128;
			}
L_127:
		l = l1 + (l2*l2 - l2)/2;
		goto L_129;
L_128:
		l = l2 + (l1*l1 - l1)/2;
L_129:
		mm = mm + 1;
//L_130:
		rx[mm-1] = r[l-1];
		}
	}

/*     PLACE THE SUBSCRIPT NUMBER OF THE DEPENDENT
     VARIABLE IN ISAVE(K+1)
 */
isave[k + 1-1] = ndep;
return;
}

