//------------------------------------------------------------------------
//   ARAT.C
//   ¿œœ–Œ —»Ã¿÷»ﬂ «¿ƒ¿ÕÕŒ… ƒ»— –≈“ÕŒ… ‘”Õ ÷»» –¿÷»ŒÕ¿À‹ÕŒ… ‘”Õ ÷»≈… Õ¿
//	 Œ—ÕŒ¬≈ ÃÕ 
//   ¿‚ÚÓ  : œÂÂ‚Ó‰ ÔÓ„‡ÏÏ˚ Ô‡ÍÂÚ‡ SSP Ì‡ — ‚˚ÔÓÎÌËÎ œ‡Ò˚ÌÍÓ‚ ¬.¬.
//   ¬ÂÒËˇ : 01.00 / 10.06.1996 /
//------------------------------------------------------------------------
#include <math.h>
#include "ssp.h"

//------------------------------------------------------------------------
//	SUBROUTINE ARAT
//
//  PURPOSE
//	   CALCULATE BEST RATIONAL APPROXIMATION OF A DISCRETE
//     FUNCTION IN THE LEAST SQUARES SENSE
//
//  USAGE
//	   ARAT(DATI,N,WORK,P,IP,IQ,IER)
//
//	DESCRIPTION OF PARAMETERS
//	   DATI  - TWODIMENSIONAL ARRAY WITH 3 COLUMNS AND N ROWS
//			   THE FIRST COLUMN MUST CONTAIN THE GIVEN ARGUMENTS,
//			   THE SECOND COLUMN THE GIVEN FUNCTION VALUES AND
//			   THE THIRD COLUMN THE GIVEN WEIGHTS IF ANY.
//			   IF NO WEIGHTS ARE TO BE USED THEN THE THIRD
//			   COLUMN MAY BE DROPPED , EXCEPT THE FIRST ELEMENT
//			   WHICH MUST CONTAIN A NONPOSITIVE VALUE
//			   DATI MUST BE OF DOUBLE PRECISION
//     N     - NUMBER OF NODES OF THE GIVEN DISCRETE FUNCTION
//	   WORK  - WORKING STORAGE WHICH IS OF DIMENSION
//             (IP+IQ)*(IP+IQ+1)+4*N+1 AT LEAST.
//             ON RETURN THE VALUES OF THE NUMERATOR ARE CONTAINED
//             IN WORK(N+1) UP TO WORK(2*N), WHILE THE VALUES OF
//             THE DENOMINATOR ARE STORED IN WORK(2*N+1) UP TO
//             WORK(3*N)
//             WORK MUST BE OF DOUBLE PRECISION
//     P     - RESULTANT COEFFICIENT VECTOR OF DENOMINATOR AND
//			   NUMERATOR. THE DENOMINATOR IS STORED IN FIRST IQ
//			   LOCATIONS, THE NUMERATOR IN THE FOLLOWING IP
//			   LOCATIONS.
//             COEFFICIENTS ARE ORDERED FROM LOW TO HIGH.
//             P MUST BE OF DOUBLE PRECISION
//     IP    - DIMENSION OF THE NUMERATOR   (INPUT VALUE)
//     IQ    - DIMENSION OF THE DENOMINATOR (INPUT VALUE)
//     IER   - RESULTANT ERROR PARAMETER
//             IER =-1 MEANS FORMAL ERRORS
//             IER = 0 MEANS NO ERRORS
//             IER = 1,2 MEANS POOR CONVERGENCE OF ITERATION
//             IER IS ALSO USED AS INPUT VALUE
//   		   A NONZERO INPUT VALUE INDICATES AVAILABILITY OF AN
//	    	   INITIAL APPROXIMATION STORED IN P
//
// REMARKS
//		THE COEFFICIENT VECTORS OF THE DENOMINATOR AND NUMERATOR
//      OF THE RATIONAL APPROXIMATION ARE BOTH STORED IN P
//      STARTING WITH LOW POWERS (DENOMINATOR FIRST).
//      IP+IQ MUST NOT EXCEED N, ALL THREE VALUES MUST BE POSITIVE.
//      SINCE CHEBYSHEV POLYNOMIALS ARE USED AS FUNDAMENTAL
//      FUNCTIONS, THE ARGUMENTS SHOULD BE REDUCED TO THE INTERVAL
//      (-1,1). THIS CAN ALWAYS BE ACCOMPLISHED BY MEANS OF A LINEAR
//      TRANSFORMATION OF THE ORIGINALLY GIVEN ARGUMENTS.
//	    IF A FIT IN OTHER FUNCTIONS IS REQUIRED, DCNP AND DCNPS MUST
//	    BE REPLACED BY SUBROUTINES WHICH ARE OF ANALOGOUS DESIGN.
//
// SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
//      DAPLL, DAPFS, DFRAT, DCNPS, DCNP
//      DCNP IS REQUIRED WITHIN DFRAT
//
// METHOD
//		THE ITERATIVE SCHEME USED FOR CALCULATION OF THE
//      APPROXIMATION IS REPEATED SOLUTION OF THE NORMAL EQUATIONS
//      WHICH ARE OBTAINED BY LINEARIZATION.
//      A REFINED TECHNIQUE OF THIS LINEAR LEAST SQUARES APPROACH
//	    IS USED WHICH GUARANTEES THAT THE DENOMINATOR IS FREE OF
//	    ZEROES WITHIN THE APPROXIMATION INTERVAL.
//	    FOR REFERENCE SEE
//      D.BRAESS, UEBER DAEMPFUNG BEI MINIMALISIERUNGSVERFAHREN,
//      COMPUTING(1966), VOL.1, ED.3, PP.264-272.
//      D.W.MARQUARDT, AN ALGORITHM FOR LEAST-SQUARES ESTIMATION
//      OF NONLINEAR PARAMETERS,
//      JSIAM(1963), VOL.11, ED.2, PP.431-441.
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int arat(double dati[], int n, double work[], double p[], int ip, int iq)
{
	int ier;
	int i, iend,
		ierv[3], incr, ipq, iqp1, ires, irhs, irp, irq, iter, ix, j, k, 
		kount, l, limit, m, ndp, nne;
	double eps, eta;
	double diag, dsave, osum, relax, ssoe, sum, t;
	ier=0;
	//        DIMENSIONED LOCAL VARIABLE 
	//        DIMENSIONED DUMMY VARIABLES 
	//        INITIALIZE TESTVALUES 
	limit = 20;
	eta = 1.e-29;
	eps = 1.e-14;
	//eps = 1.e-22;
	//        CHECK FOR FORMAL ERRORS 
	switch( for_if(n) ){
		case -1: goto L_4;
		case  0: goto L_4;
		case  1: goto L_1;
	}
L_1:
	switch( for_if(ip) ){
		case -1: goto L_4;
		case  0: goto L_4;
		case  1: goto L_2;
	}
L_2:
	switch( for_if(iq) ){
		case -1: goto L_4;
		case  0: goto L_4;
		case  1: goto L_3;
	}
L_3:
	ipq = ip + iq;
	switch( for_if(n - ipq) ){
		case -1: goto L_4;
		case  0: goto L_5;
		case  1: goto L_5;
	}
	//        ERROR RETURN IN CASE OF FORMAL ERRORS 
L_4:
	ier = -1;
	return ier;

	//        INITIALIZE ITERATION PROCESS 
L_5:
	kount = 0;
	ierv[1] = ip;
	ierv[2] = iq;
	ndp = n + n + 1;
	nne = ndp + ndp;
	ix = ipq - 1;
	iqp1 = iq + 1;
	irhs = nne + ipq*ix/2;
	iend = irhs + ix;
	
	//        TEST FOR AVAILABILITY OF AN INITIAL APPROXIMATION 
	switch( for_if(ier) ){
		case -1: goto L_8;
		case  0: goto L_6;
		case  1: goto L_8;
	}
	//        INITIALIZE NUMERATOR AND DENOMINATOR 
L_6:
	for( i = 2; i <= ipq; i++ ){
		p[i-1] = 0.e0;
	}
	p[0] = 1.e0;
	//        CALCULATE VALUES OF NUMERATOR AND DENOMINATOR FOR INITIAL
	//		  APPROXIMATION 
L_8:
	for( j = 1 ; j <= n; j++ ){
		t = dati[j-1];
		i = j + n;
		work[i-1]=cnps(t, &p[iqp1-1], ip );
		k = i + n;
		work[k-1]=cnps(t, p, iq );
	}
	//        SET UP NORMAL EQUATIONS (MAIN LOOP OF ITERATION) 
L_10:
	apll( frat, n, ix, work, &work[iend + 1-1], dati,  ierv);
	//        CHECK FOR ZERO DENOMINATOR 
	switch( for_if(ierv[0]) ){
		case -1: goto L_4;
		case  0: goto L_11;
		case  1: goto L_4;
	}
L_11:
	incr = 0;
	relax = 2.e0;
	//        RESTORE MATRIX IN WORKING STORAGE 
L_12:
	j = iend;
	for( i = nne; i <=iend; i++ ){
		j = j + 1;
		work[i-1] = work[j-1];
	}
	switch( for_if(kount) ){
		case -1: goto L_14;
		case  0: goto L_14;
		case  1: goto L_15;
	}
	//        DSAVE SQUARE SUM OF ERRORS 
L_14:
	osum = work[iend-1];
	diag = osum*eps;
	k = iq;

	//        ADD CONSTANT TO DIAGONAL 
	switch( for_if(work[nne-1]) ){
		case -1: goto L_17;
		case  0: goto L_17;
		case  1: goto L_19;
	}
L_15:
	switch( for_if(incr) ){
		case -1: goto L_19;
		case  0: goto L_19;
		case  1: goto L_16;
	}
L_16:
	k = ipq;
L_17:
	j = nne - 1;
	for( i = 1; i <= k; i++ ){
		work[j-1] = work[j-1] + diag;
		j = j + i;
	}
	//        SOLVE NORMAL EQUATIONS 
L_19:
	//int  apfs(double work[],int ip,int *ires,int iop,double eps,double eta)
	ier=apfs( &work[nne-1], ix, &ires, 1, eps, eta);

	//        CHECK FOR FAILURE OF EQUATION SOLVER 
	switch( for_if(ires) ){
		case -1: goto L_4;
		case  0: goto L_4;
		case  1: goto L_20;
	}
	//        TEST FOR DEFECTIVE NORMALEQUATIONS 
L_20:
	switch( for_if(ires - ix) ){
		case -1: goto L_21;
		case  0: goto L_24;
		case  1: goto L_24;
	}
L_21:
	switch( for_if(incr) ){
		case -1: goto L_22;
		case  0: goto L_22;
		case  1: goto L_23;
	}
L_22:
	diag = diag*0.125e0;
L_23:
	diag = diag + diag;
	incr = incr + 1;
	//        START WITH OVER RELAXATION
	relax = 8.e0;
	switch( for_if(incr - limit) ){
		case -1: goto L_12;
		case  0: goto L_45;
		case  1: goto L_45;
	}
	//        CALCULATE VALUES OF CHANGE OF NUMERATOR AND DENOMINATOR 
L_24:
	l = ndp;
	j = nne + ires*(ires - 1)/2 - 1;
	k = j + iq;
	work[j-1] = 0.e0;
	irq = iq;
	irp = ires - iq + 1;
	switch( for_if(irp) ){
		case -1: goto L_25;
		case  0: goto L_26;
		case  1: goto L_26;
	}
L_25:
	irq = ires + 1;
L_26:
	for( i = 1; i <= n; i++ ){
		t = dati[i-1];
		work[i-1] = 0.e0;
		work[i-1]=cnps(t, &work[k-1], irp );
		m = l + n;
		work[m-1]=cnps(  t, &work[j-1], irq );
		switch( for_if(work[m-1]*work[l-1]) ){
			case -1: goto L_27;
			case  0: goto L_29;
			case  1: goto L_29;
		}
L_27:	sum = work[l-1]/work[m-1];
		switch( for_if(relax + sum) ){
			case -1: goto L_29;
			case  0: goto L_29;
			case  1: goto L_28;
		}
L_28:	relax = -sum;
L_29:	l = l + 1;
	}
	//        MODIFY RELAXATION FACTOR IF NECESSARY 
	ssoe = osum;
	iter = limit;
L_30:
	sum = 0.e0;
	relax = relax*0.5e0;
	for( i = 1; i <= n; i++ ){
		m = i + n;
		k = m + n;
		l = k + n;
		dsave = dati[m-1] - (work[m-1] + relax*work[i-1])/(work[k-1] + 
		relax*work[l-1]);
		dsave = dsave*dsave;
		switch( for_if(dati[ndp-1])) {
			case -1: goto L_32;
			case  0: goto L_32;
			case  1: goto L_31;
		}
L_31:	dsave = dsave*dati[k-1];
L_32:	sum = sum + dsave;
	}
	switch( for_if(iter) ){
		case -1: goto L_45;
		case  0: goto L_33;
		case  1: goto L_33;
	}
L_33:
	iter = iter - 1;
	switch( for_if(sum - osum) ){
		case -1: goto L_34;
		case  0: goto L_37;
		case  1: goto L_35;
	}
L_34:
	osum = sum;
goto L_30;
	//        TEST FOR IMPROVEMENT 
L_35:
	switch( for_if(osum - ssoe) ){
		case -1: goto L_36;
		case  0: goto L_30;
		case  1: goto L_30;
	}
L_36:
	relax = relax + relax;
L_37:
	t = 0.;
	dsave = 0.e0;
	k = ires + 1;
	for( i = 2; i <= k; i++ ){
		j = j + 1;
		t = t + fabs( p[i-1] );
		p[i-1] = p[i-1] + relax*work[j-1];
		dsave = dsave + fabs( p[i-1] );
	}
	//        UPDATE CURRENT VALUES OF NUMERATOR AND DENOMINATOR 
	for( i = 1; i <= n; i++ ){
		j = i + n;
		k = j + n;
		l = k + n;
		work[j-1] = work[j-1] + relax*work[i-1];
		work[k-1] = work[k-1] + relax*work[l-1];
	}
	//        TEST FOR CONVERGENCE 
	switch( for_if(incr) ){
		case -1: goto L_40;
		case  0: goto L_40;
		case  1: goto L_42;
	}
L_40:
	switch( for_if(ssoe - osum - relax*osum*(double)( eps )) ){
		case -1: goto L_46;
		case  0: goto L_46;
		case  1: goto L_41;
	}
L_41:
	switch( for_if(fabs( t - dsave ) - relax*dsave*(double)( eps )) ){
		case -1: goto L_46;
		case  0: goto L_46;
		case  1: goto L_42;
	}
L_42:
	switch( for_if(osum - dsave*(double)( eta )) ){
		case -1: goto L_46;
		case  0: goto L_46;
		case  1: goto L_43;
	}
L_43:
	kount = kount + 1;
	switch( for_if(kount - limit) ){
		case -1: goto L_10;
		case  0: goto L_44;
		case  1: goto L_44;
	}
	//        ERROR RETURN IN CASE OF POOR CONVERGENCE 
L_44:
	ier = 2;
	return ier;
L_45:
	ier = 1;
	return ier;
	//        NORMAL RETURN 
L_46:
	ier = 0;
	return ier;
}

