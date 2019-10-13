//------------------------------------------------------------------------
// APFS.C
// пеьемхе яхярелш мнплюкэмшу спюбмемхи, бнгмхйючыеи опх мюхксвьеи
// б ялшяке лерндю мюхлемэьху йбюдпюрнб юоопнйяхлюжхх гюдюммни
// дхяйпермни тсмйжхх кхмеимни йнлахмюжхееи тсмйжхи
// юБРНП   : оЕПЕБНД ОПНЦПЮЛЛШ ОЮЙЕРЮ SSP МЮ я БШОНКМХК оЮЯШМЙНБ б.б.
// бЕПЯХЪ  : 01.00 / 10.08.1997 /
//------------------------------------------------------------------------
#include <math.h>
#include "ssp.h"

//------------------------------------------------------------------------
// ондопнцпюллю apfs
// 
// мюгмювемхе
// пеьемхе яхярелш мнплюкэмшу спюбмемхи, бнгмхйючыеи опх
// опхакхфемхх тсмйжхи лернднл мюхлемэьху йбюдпюрнб
//
// напюыемхе й ондопнцпюлле
// apfs(WORK,IP,IRES,IOP,EPS,ETA,IER)
// 
// бундмше х бшундмше оюпюлерпш
//		WORK  - бундмни люяяхб пюглепмнярх (IP+1)*(IP+2)/2;
//				янгдюеряъ ондопнцпюллюлх appl, apch.
//				яндепфхр яхллерпхвеяйсч люрпхжс йнщттхжхемрнб яхярелш
//				мнплюкэмшу спюбмемхи б яфюрни (рпесцнкэмни) тнпле х
//				опюбше вюярх яхярелш х бгбеьеммсч ясллс йбюдпюрнб.
//		IP    - гюдюммне вхякн тсмдюлемрюкэмшу тсмйжхи, хяонкэгселшу
//				дкъ нршяйюмхъ мюхксвьецн опхакхфемхъ он лмй.
//		IRES  - бшундмни оюпюлерп - пюглепмнярэ бшвхякеммнцн
//				мюхксвьецн б ялшяке лмй опхакхфемхъ
//				LET N1, N2, DENOTE THE FOLLOWING NUMBERS
//				N1 = MAXIMAL DIMENSION FOR WHICH NO LOSS OF
//				SIGNIFICANCE WAS INDICATED DURING FACTORIZATION
//				N2 = SMALLEST DIMENSION FOR WHICH THE SQUARE SUM OF
//				THE ERRORS DOES NOT EXCEED TEST=ABS(ETA*FSQ)
//				THEN IRES=MINO(IP,N1) IF IOP IS NONNEGATIVE
//				AND  IRES=MINO(IP,N1,N2) IF IOP IS NEGATIVE
//		IOP   - бундмни оюпюлерп, сопюбкъчыхи пюанрни опнцпюллш
//				(INPUT PARAMETER FOR SELECTION OF OPERATION)
//				IOP = 0 бшонкмъеряъ пюгкнфемхе люрпхжш ю мю
//				опнхгбедемхе дбсу рпесцнкэмшу люрпхж
//				ю=trans(р)*р
//				гюдюммше опюбше вюярх декъряъ мю trans(р),
//				бшвхякъеряъ ясллю йбюдпюрнб онцпеьмняреи.
//				бепумхи рпесцнкэмхй р оепшяшкюеряъ б WORK.
//				MEANS TRIANGULAR FACTORIZATION, DIVISION OF
//				THE RIGHT HAND SIDE BY TRANSPOSE(T) AND
//				CALCULATION OF THE SQUARE SUM OF ERRORS IS
//				PERFORMED ONLY
//				IOP = +1 OR -1 MEANS THE SOLUTION OF DIMENSION IRES
//				IS CALCULATED ADDITIONALLY
//				IOP = +2 OR -2 MEANS ALL SOLUTIONS FOR DIMENSION ONE
//				UP TO IRES ARE CALCULATED ADDITIONALLY
//		EPS   - RELATIVE TOLERANCE FOR TEST ON LOSS OF SIGNIFICANCE.
//				A SENSIBLE VALUE IS BETWEEN 1.E-10 AND 1.E-15
//		ETA   - RELATIVE TOLERANCE FOR TOLERATED SQUARE SUM OF
//				ERRORS. A REALISTIC VALUE IS BETWEEN 1.E0 AND 1.E-15
//		IER   - RESULTANT ERROR PARAMETER
//				IER =-1 MEANS NONPOSITIVE IP
//				IER = 0 MEANS NO LOSS OF SIGNIFICANCE DETECTED
//				AND SPECIFIED TOLERANCE OF ERRORS REACHED
//				IER = 1 MEANS LOSS OF SIGNIFICANCE DETECTED OR
//				SPECIFIED TOLERANCE OF ERRORS NOT REACHED
// 
// REMARKS
// THE ABSOLUTE TOLERANCE USED INTERNALLY FOR TEST ON LOSS OF
// SIGNIFICANCE IS TOL=ABS(EPS*SNGL(WORK(1))).
// THE ABSOLUTE TOLERANCE USED INTERNALLY FOR THE SQUARE SUM OF
// ERRORS IS ABS(ETA*SNGL(FSQ)).
// IOP GREATER THAN 2 HAS THE SAME EFFECT AS IOP = 2.
// IOP LESS THAN -2 HAS THE SAME EFFECT AS IOP =-2.
// IRES = 0 MEANS THE ABSOLUTE VALUE OF EPS IS NOT LESS THAN
// ONE AND/OR WORK(1) IS NOT POSITIVE AND/OR IP IS NOT POSITIVE
// 
// SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
// NONE
// 
// METHOD
// CALCULATION OF THE LEAST SQUARES FITS IS DONE USING
// CHOLESKYS SQUARE ROOT METHOD FOR SYMMETRIC FACTORIZATION.
// THE INCORPORATED TEST ON LOSS OF SIGNIFICANCE MEANS EACH
// RADICAND MUST BE GREATER THAN THE INTERNAL ABSOLUTE
// TOLERANCE TOL.
// IN CASE OF LOSS OF SIGNIFICANCE IN THE ABOVE SENSE ONLY A
// SUBSYSTEM OF THE NORMAL EQUATIONS IS SOLVED.
// IN CASE OF NEGATIVE IOP THE TRIANGULAR FACTORIZATION IS
// TERMINATED PREMATURELY EITHER IF THE SQUARE SUM OF THE
// ERRORS DOES NOT EXCEED ETA*FSQ OR IF THERE IS INDICATION
// FOR LOSS OF SIGNIFICANCE
//------------------------------------------------------------------------
int  apfs(double work[], int ip, int *ires, int iop, double eps, double eta)
{
	int    ier, i, iadr, iend, ipiv, ipp1, ite,
		   j, ja, je, jj, jk, k;
	double test, tol, piv, sum;
	
	//        DIMENSIONED DUMMY VARIABLES */
	(*ires) = 0;
	//        TEST OF SPECIFIED DIMENSION */
	if (ip <= 0) {
		//        ERROR RETURN IN CASE OF ILLEGAL DIMENSION */
		ier = -1;
		return ier;
	}
	//        INITIALIZE FACTORIZATION PROCESS */
	ipiv = 0; ipp1 = ip + 1; ier = 1;
	ite = ip*ipp1 / 2; iend = ite + ipp1 - 1;
	tol = fabs(eps*work[0]);
	test = fabs(eta*work[iend]);
	//        START LOOP OVER ALL ROWS OF WORK */
	for (i = 1; i <= ip; i++){
		ipiv = ipiv + i;
		ja = ipiv - (*ires);
		je = ipiv - 1;

		//        FORM SCALAR PRODUCT NEEDED TO MODIFY CURRENT ROW ELEMENTS */
		jk = ipiv;
		for (k = i; k <= ipp1; k++){
			sum = 0.e0;
			if ((*ires) > 0)
			{
				jk = jk - (*ires);
				for (j = ja; j <= je; j++){
					sum = sum + work[j - 1] * work[jk - 1];
					jk = jk + 1;
				}
			}
			if ((jk - ipiv) <= 0.0)
			{
				//        TEST FOR LOSS OF SIGNIFICANCE */
				sum = work[ipiv - 1] - sum;
				if ((sum - tol) <= 0.0)  goto L_12;
				sum = sqrt(sum);
				work[ipiv - 1] = sum;
				piv = 1.e0 / sum;
			}
			else
			{
				//        UPDATE OFF-DIAGONAL TERMS */
				sum = (work[jk - 1] - sum)*piv;
				work[jk - 1] = sum;
			}
			jk = jk + k;
		}
		//        UPDATE SQUARE SUM OF ERRORS */
		work[iend] -= (sum*sum);
		//        RECORD ADDRESS OF LAST PIVOT ELEMENT */
		(*ires) = (*ires) + 1;
		iadr = ipiv;
		//        TEST FOR TOLERABLE ERROR IF SPECIFIED */
		if (iop < 0)
			if ((work[iend] - test) <= 0.0)
				goto L_13;
	}

	if (iop == 0) goto L_22;
	//        PERFORM BACK SUBSTITUTION IF SPECIFIED */
L_12:
	if (iop == 0) goto L_23;
	else goto L_14;
L_13:
	ier = 0;
L_14:
	ipiv = (*ires);
	while (ipiv > 0)
	{
		sum = 0.e0;
		ja = ite + ipiv; jj = iadr;
		jk = iadr;       k = ipiv;
		for (i = 1; i <= ipiv; i++){
			work[jk - 1] = (work[ja - 1] - sum) / work[jj - 1];
			if ((k - 1) <= 0) break;
			je = jj - 1;
			for (sum = 0.e0, j = k; j <= ipiv; j++){
				sum = sum + work[jk - 1] * work[je - 1];
				jk = jk + 1;	je = je + j;
			}
			jk = je - ipiv;	ja = ja - 1;
			jj = jj - k;   	k = k - 1;
		}
		if ((iop / 2) == 0) goto L_23;
		iadr = iadr - ipiv;
		ipiv = ipiv - 1;
	}
	//        NORMAL RETURN */
L_22:
	ier = 0;
L_23:
	return ier;
}


