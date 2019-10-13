#include <stdio.h>
#include <math.h>
#include "ssp.h"
/*     ..................................................................

        SUBROUTINE STPRG

	PURPOSE
           TO PERFORM A STEPWISE MULTIPLE REGRESSION ANALYSIS FOR A
           DEPENDENT VARIABLE AND A SET OF INDEPENDENT VARIABLES.  AT
           EACH STEP, THE VARIABLE ENTERED INTO THE REGRESSION EQUATION
           IS THAT WHICH EXPLAINS THE GREATEST AMOUNT OF VARIANCE
           BETWEEN IT AND THE DEPENDENT VARIABLE (I.E. THE VARIABLE
	   WITH THE HIGHEST PARTIAL CORRELATION WITH THE DEPENDENT
           VARIABLE).  ANY VARIABLE CAN BE DESIGNATED AS THE DEPENDENT
           VARIABLE.  ANY INDEPENDENT VARIABLE CAN BE FORCED INTO OR
           DELETED FROM THE REGRESSION EQUATION, IRRESPECTIVE OF ITS
           CONTRIBUTION TO THE EQUATION.

        USAGE
	   STPRG (M,N,D,XBAR,IDX,PCT,NSTEP,ANS,L,B,S,T,LL,IER)

        DESCRIPTION OF PARAMETERS
           M    - TOTAL NUMBER OF VARIABLES IN DATA MATRIX
           N    - NUMBER OF OBSERVATIONS
           D    - INPUT MATRIX (M X M) OF SUMS OF CROSS-PRODUCTS OF
		  DEVIATIONS FROM MEAN.  THIS MATRIX WILL BE DESTROYED.
           XBAR - INPUT VECTOR OF LENGTH M OF MEANS
           IDX  - INPUT VECTOR OF LENGTH M HAVING ONE OF THE FOLLOWING
                  CODES FOR EACH VARIABLE.
                    0 - INDEPENDENT VARIABLE AVAILABLE FOR SELECTION
                    1 - INDEPENDENT VARIABLE TO BE FORCED INTO THE
                        REGRESSION EQUATION
		    2 - VARIABLE NOT TO BE CONSIDERED IN THE EQUATION
                    3 - DEPENDENT VARIABLE
                  THIS VECTOR WILL BE DESTROYED
           PCT  - A CONSTANT VALUE INDICATING THE PROPORTION OF THE
                  TOTAL VARIANCE TO BE EXPLAINED BY ANY INDEPENDENT
                  VARIABLE.  THOSE INDEPENDENT VARIABLES WHICH FALL
		  BELOW THIS PROPORTION WILL NOT ENTER THE REGRESSION
                  EQUATION.  TO ENSURE THAT ALL VARIABLES ENTER THE
                  EQUATION, SET PCT = 0.0.
           NSTEP- OUTPUT VECTOR OF LENGTH 5 CONTAINING THE FOLLOWING
                  INFORMATION
                     NSTEP(1)- THE NUMBER OF THE DEPENDENT VARIABLE
                     NSTEP(2)- NUMBER OF VARIABLES FORCED INTO THE
			       REGRESSION EQUATION
                     NSTEP(3)- NUMBER OF VARIABLE DELETED FROM THE
                               EQUATION
                     NSTEP(4)- THE NUMBER OF THE LAST STEP
                     NSTEP(5)- THE NUMBER OF THE LAST VARIABLE ENTERED
           ANS  - OUTPUT VECTOR OF LENGTH 11 CONTAINING THE FOLLOWING
		  INFORMATION FOR THE LAST STEP
                     ANS(1)- SUM OF SQUARES REDUCED BY THIS STEP
                     ANS(2)- PROPORTION OF TOTAL SUM OF SQUARES REDUCED
                     ANS(3)- CUMULATIVE SUM OF SQUARES REDUCED UP TO
                             THIS STEP
                     ANS(4)- CUMULATIVE PROPORTION OF TOTAL SUM OF
                             SQUARES REDUCED
		     ANS(5)- SUM OF SQUARES OF THE DEPENDENT VARIABLE
                     ANS(6)- MULTIPLE CORRELATION COEFFICIENT
                     ANS(7)- F RATIO FOR SUM OF SQUARES DUE TO
                             REGRESSION
                     ANS(8)- STANDARD ERROR OF THE ESTIMATE (RESIDUAL
                             MEAN SQUARE)
                     ANS(9)- INTERCEPT CONSTANT
                     ANS(10)-MULTIPLE CORRELATION COEFFICIENT ADJUSTED
                             FOR DEGREES OF FREEDOM.
                     ANS(11)-STANDARD ERROR OF THE ESTIMATE ADJUSTED
                             FOR DEGREES OF FREEDOM.
           L    - OUTPUT VECTOR OF LENGTH K, WHERE K IS THE NUMBER OF
                  INDEPENDENT VARIABLES IN THE REGRESSION EQUATION.
		  THIS VECTOR CONTAINS THE NUMBERS OF THE INDEPENDENT
                  VARIABLES IN THE EQUATION.
           B    - OUTPUT VECTOR OF LENGTH K, CONTAINING THE PARTIAL
                  REGRESSION COEFFICIENTS CORRESPONDING TO THE
                  VARIABLES IN VECTOR L.
           S    - OUTPUT VECTOR OF LENGTH K, CONTAINING THE STANDARD
                  ERRORS OF THE PARTIAL REGRESSION COEFFICIENTS,
                  CORRESPONDING TO THE VARIABLES IN VECTOR L.
           T    - OUTPUT VECTOR OF LENGTH K, CONTAINING THE COMPUTED
                  T-VALUES CORRESPONDING TO THE VARIABLES IN VECTOR L.
           LL   - WORKING VECTOR OF LENGTH M
           IER  - 0, IF THERE IS NO ERROR.
                  1, IF RESIDUAL SUM OF SQUARES IS NEGATIVE OR IF THE
		  PIVOTAL ELEMENT IN THE STEPWISE INVERSION PROCESS IS
                  ZERO.  IN THIS CASE, THE VARIABLE WHICH CAUSES THIS
                  ERROR IS NOT ENTERED IN THE REGRESSION, THE RESULT
                  PRIOR TO THIS STEP IS RETAINED, AND THE CURRENT
                  SELECTION IS TERMINATED.

        REMARKS
           THE NUMBER OF DATA POINTS MUST BE AT LEAST GREATER THAN THE
           NUMBER OF INDEPENDENT VARIABLES PLUS ONE.  FORCED VARIABLES
           ARE ENTERED INTO THE REGRESSION EQUATION BEFORE ALL OTHER
           INDEPENDENT VARIABLES.  WITHIN THE SET OF FORCED VARIABLES,
           THE ONE TO BE CHOSEN FIRST WILL BE THAT ONE WHICH EXPLAINS
           THE GREATEST AMOUNT OF VARIANCE.
	   INSTEAD OF USING, AS A STOPPING CRITERION, A PROPORTION OF
           THE TOTAL VARIANCE, SOME OTHER CRITERION MAY BE ADDED TO
           SUBROUTINE STOUT.

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           STOUT(NSTEP,ANS,L,B,S,T,NSTOP)
           THIS SUBROUTINE MUST BE PROVIDED BY THE USER.  IT IS AN
           OUTPUT ROUTINE WHICH WILL PRINT THE RESULTS OF EACH STEP OF
           THE REGRESSION ANALYSIS.  NSTOP IS AN OPTION CODE WHICH IS
           ONE IF THE STEPWISE REGRESSION IS TO BE TERMINATED, AND IS
           ZERO IF IT IS TO CONTINUE.  THE USER MUST CONSIDER THIS IF
           SOME OTHER STOPPING CRITERION THAN VARIANCE PROPORTION IS TO
           BE USED.

        METHOD
           THE ABBREVIATED DOOLITTLE METHOD IS USED TO (1) DECIDE VARI-
           ABLES ENTERING IN THE REGRESSION AND (2) COMPUTE REGRESSION
           COEFFICIENTS.  REFER TO C. A. BENNETT AND N. L. FRANKLIN,
           'STATISTICAL ANALYSIS IN CHEMISTRY AND THE CHEMICAL INDUS-
           TRY', JOHN WILEY AND SONS, 1954, APPENDIX 6A.

     ..................................................................
 */
void stprg(int m, int n, double d[], double xbar[], int idx[],
	   double *pct, int nstep[], double ans[], int l[],
	   double b[], double s[], double t[], int ll[], int *ier)
{
int  i,
 id, ij, ik, ip, j, k, kk, ly, lyp, mk, mx, my, neww, nfo, nl, nstop;
double onm, rd, re;
/*     ..................................................................

        IF A DOUBLE PRECISION VERSION OF THIS ROUTINE IS DESIRED, THE
        C IN COLUMN 1 SHOULD BE REMOVED FROM THE DOUBLE PRECISION
        STATEMENT WHICH FOLLOWS.

     DOUBLE PRECISION D,XBAR,ANS,B,S,T,RD,RE

        THE C MUST ALSO BE REMOVED FROM DOUBLE PRECISION STATEMENTS
        APPEARING IN OTHER ROUTINES USED IN CONJUNCTION WITH THIS
	ROUTINE.

        THE DOUBLE PRECISION VERSION OF THIS SUBROUTINE MUST ALSO
        CONTAIN DOUBLE PRECISION FORTRAN FUNCTIONS.  SQRT IN STATEMENTS
        85,90,114,132,AND 134, MUST BE CHANGED TO DSQRT.

     ..................................................................

        INITIALIZATION
 */
(*ier) = 0;
onm = n - 1;
nfo = 0; nstep[2] = 0;
ans[2] = 0.0;ans[3] = 0.0;
nstop = 0;

/*        FIND DEPENDENT VARIABLE, NUMBER OF VARIABLES TO BE FORCED TO
	ENTER IN THE REGRESSION, AND NUMBER OF VARIABLES TO BE DELETED
 */
for( i = 1; i <= m; i++ ){
	ll[i-1] = 1;
	switch( for_if(idx[i-1]) ){
		case -1: goto L_30;
		case  0: goto L_30;
		case  1: goto L_10;
		}
L_10:
	switch( for_if(idx[i-1] - 2) ){
		case -1: goto L_15;
		case  0: goto L_20;
		case  1: goto L_25;
		}
L_15:
	nfo = nfo + 1;
	idx[nfo-1] = i;
	goto L_30;
L_20:
	nstep[2] = nstep[2] + 1;
	ll[i-1] = -1;
	goto L_30;
L_25:
	my = i;
	nstep[0] = my;
	ly = m*(my - 1);
	lyp = ly + my;
	ans[4] = d[lyp-1];
L_30:
	;
	}
nstep[1] = nfo;

/*        FIND THE MAXIMUM NUMBER OF STEPS
 */
mx = m - nstep[2] - 1;

/*        START SELECTION OF VARIABLES
 */
for( nl = 1; nl <= mx; nl++ ){
	rd = 0;
	switch( for_if(nl - nfo) ){
		case -1: goto L_35;
		case  0: goto L_35;
		case  1: goto L_55;
		}

	/*        SELECT NEXT VARIABLE TO ENTER AMONG FORCED VARIABLES
	 */
L_35:
	for( i = 1; i <= nfo; i++ ){
		k = idx[i-1];
		switch( for_if(ll[k-1]) ){
			case -1: goto L_50;
			case  0: goto L_50;
			case  1: goto L_40;
			}
L_40:
		lyp = ly + k;
		ip = m*(k - 1) + k;
		re = d[lyp-1]*d[lyp-1]/d[ip-1];
		switch( for_if(rd - re) ){
			case -1: goto L_45;
			case  0: goto L_50;
			case  1: goto L_50;
			}
L_45:
		rd = re;
		neww = k;
L_50:
		;
		}
	goto L_75;
/* SELECT NEXT VARIABLE TO ENTER AMONG NON-FORCED VARIABLES */
L_55:
	for( i = 1; i <= m; i++ ){
		switch( for_if(i - my) ){
			case -1: goto L_60;
			case  0: goto L_70;
			case  1: goto L_60;
			}
L_60:
		switch( for_if(ll[i-1]) ){
			case -1: goto L_70;
			case  0: goto L_70;
			case  1: goto L_62;
			}
L_62:
		lyp = ly + i;
		ip = m*(i - 1) + i;
		re = d[lyp-1]*d[lyp-1]/d[ip-1];
		switch( for_if(rd - re) ){
			case -1: goto L_64;
			case  0: goto L_70;
			case  1: goto L_70;
			}
L_64:
		rd = re;
		neww = i;
L_70:
		;
		}

	/*        TEST WHETHER THE PROPORTION OF THE SUM OF SQUARES REDUCED BY
		THE LAST VARIABLE ENTERED IS GREATER THAN OR EQUAL TO THE
		SPECIFIED PROPORTION	 */
L_75:
	switch( for_if(rd) ){
		case -1: goto L_77;
		case  0: goto L_77;
		case  1: goto L_76;
		}
L_76:
	switch( for_if(ans[4] - (ans[2] + rd)) ){
		case -1: goto L_77;
		case  0: goto L_77;
		case  1: goto L_78;
		}
L_77:
	(*ier) = 1;
	goto L_150;
L_78:
	re = rd/ans[4];
	switch( for_if(re - (*pct)) ){
		case -1: goto L_150;
		case  0: goto L_80;
		case  1: goto L_80;
		}

	/*        IT IS GREATER THAN OR EQUAL
	 */
L_80:
	ll[neww-1] = 0;
	l[nl-1] = neww;
	ans[0] = rd;
	ans[1] = re;
	ans[2] = ans[2] + rd;
	ans[3] = ans[3] + re;
	nstep[3] = nl;
	nstep[4] = neww;

	/*        COMPUTE MULTIPLE CORRELATION, F-VALUE FOR ANALYSIS OF
	        VARIANCE, AND STANDARD ERROR OF ESTIMATE
	 */
//L_85:
	ans[5] = sqrt( ans[3] );
	rd = nl;
	re = onm - rd;
	re = (ans[4] - ans[2])/re;
	ans[6] = (ans[2]/rd)/re;
//L_90:
	ans[7] = sqrt( re );

	/*        DIVIDE BY THE PIVOTAL ELEMENT
	 */
	ip = m*(neww - 1) + neww;
	rd = d[ip-1];
	lyp = neww - m;
	for( j = 1 ; j <= m; j++ ){
		lyp = lyp + m;
		switch( for_if(ll[j-1]) ){
			case -1: goto L_100;
			case  0: goto L_94;
			case  1: goto L_97;
			}
L_94:
		switch( for_if(j - neww) ){
			case -1: goto L_96;
			case  0: goto L_98;
			case  1: goto L_96;
			}
L_96:
		ij = m*(j - 1) + j;
		d[ij-1] = d[ij-1] + d[lyp-1]*d[lyp-1]/rd;
L_97:
		d[lyp-1] = d[lyp-1]/rd;
		goto L_100;
L_98:
		d[ip-1] = 1.0/rd;
L_100:
		;
		}

	/*        COMPUTE REGRESSION COEFFICIENTS
	 */
	lyp = ly + neww;
	b[nl-1] = d[lyp-1];
	switch( for_if(nl - 1) ){
		case -1: goto L_112;
		case  0: goto L_112;
		case  1: goto L_105;
		}
L_105:
	id = nl - 1;
	for( j = 1; j <= id; j++ ){
		ij = nl - j;
		kk = l[ij-1];
		lyp = ly + kk;
		b[ij-1] = d[lyp-1];
		for( k = 1; k <= j; k++ ){
			ik = nl - k + 1;
			mk = l[ik-1];
			lyp = m*(mk - 1) + kk;
//L_110:
			b[ij-1] = b[ij-1] - d[lyp-1]*b[ik-1];
			}
		}

	/*        COMPUTE INTERCEPT
	 */
L_112:
	ans[8] = xbar[my-1];
	for( i = 1; i <= nl; i++ ){
		kk = l[i-1];
		ans[8] = ans[8] - b[i-1]*xbar[kk-1];
		ij = m*(kk - 1) + kk;
//L_114:
		s[i-1] = ans[7]*sqrt( d[ij-1] );
//L_115:
		t[i-1] = b[i-1]/s[i-1];
		}

	/*        PERFORM A REDUCTION TO ELIMINATE THE LAST VARIABLE ENTERED	 */
	ip = m*(neww - 1);
	for( i = 1; i <= m; i++ ){
		ij = i - m;
		ik = neww - m;
		ip = ip + 1;
		switch( for_if(ll[i-1]) ){
			case -1: goto L_130;
			case  0: goto L_130;
			case  1: goto L_120;
			}
L_120:
		for( j = 1; j <= m; j++ ){
			ij = ij + m;
			ik = ik + m;
			switch( for_if(ll[j-1]) ){
				case -1: goto L_126;
				case  0: goto L_122;
				case  1: goto L_122;
				}
L_122:
			switch( for_if(j - neww) ){
				case -1: goto L_124;
				case  0: goto L_126;
				case  1: goto L_124;
				}
L_124:
			d[ij-1] = d[ij-1] - d[ip-1]*d[ik-1];
L_126:                  ;
			}
		d[ip-1] = d[ip-1]/(-rd);
L_130:;
		}

	/*        ADJUST STANDARD ERROR OF THE ESTIMATE AND MULTIPLE CORRELATION
		COEFFICIENT	 */
	rd = n - nstep[3];
	rd = onm/rd;
//L_132:
	ans[9] = sqrt( 1.0 - (1.0 - ans[5]*ans[5])*rd );
//L_134:
	ans[10] = ans[7]*sqrt( rd );

	/*        CALL THE OUTPUT SUBROUTINE */
//	stout( nstep, ans, l, b, s, t, &nstop );

	/*        TEST WHETHER THE STEP-WISE REGRESSION WAS TERMINATED IN
		SUBROUTINE STOUT	 */
	switch( for_if(nstop) ){
		case -1: goto L_140;
		case  0: goto L_140;
		case  1: goto L_150;
		}

L_140:
	;
	}

L_150:
return;
}

