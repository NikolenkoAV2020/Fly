/*------------------------------------------------------------------------
  MOMEN.C
	    ВЫЧИСЛЕНИЕ ПЕРВЫХ ЧЕТЫРЕХ МОМЕНТОВ
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 26.04.1997 /
------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	SUBROUTINE MOMEN

	PURPOSE
	   TO FIND THE THE FIRST FOUR MOMENTS FOR GROUPED DATA ON
	   EQUAL CLASS INTERVALS.

	USAGE
	   MOMEN (F,UBO,NOP,ANS)

        DESCRIPTION OF PARAMETERS
           F   - GROUPED DATA (FREQUENCIES).  GIVEN AS A VECTOR OF
                 LENGTH (UBO(3)-UBO(1))/UBO(2)
           UBO - 3 CELL VECTOR, UBO(1) IS LOWER BOUND AND UBO(3) UPPER
                 BOUND ON DATA.  UBO(2) IS CLASS INTERVAL.  NOTE THAT
                 UBO(3) MUST BE GREATER THAN UBO(1).
           NOP - OPTION PARAMETER.  IF NOP = 1, ANS(1) = MEAN.  IF
                 NOP = 2, ANS(2) = SECOND MOMENT.  IF NOP = 3, ANS(3) =
                 THIRD MOMENT.  IF NOP = 4, ANS(4) = FOURTH MOMENT.
                 IF NOP = 5, ALL FOUR MOMENTS ARE FILLED IN.
	   ANS - OUTPUT VECTOR OF LENGTH 4 INTO WHICH MOMENTS ARE PUT.

        REMARKS
           NOTE THAT THE FIRST MOMENT IS NOT CENTRAL BUT THE VALUE OF
           THE MEAN ITSELF.  THE MEAN IS ALWAYS CALCULATED.  MOMENTS
           ARE BIASED AND NOT CORRECTED FOR GROUPING.

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           REFER TO M. G. KENDALL, 'THE ADVANCED THEORY OF STATISTICS',
	   V.1, HAFNER PUBLISHING COMPANY, 1958, CHAPTER 3.

     ..................................................................
 */
void momen(double f[], double ubo[], int nop, double ans[])
{
int    i, jump, n;
double fi, t, a,a2;

jump=n=0;
for( i = 0; i < 4; i++ ) ans[i] = 0.0;


/*     CALCULATE THE NUMBER OF CLASS INTERVALS */
n = (int)((ubo[2] - ubo[0])/ubo[1] + 0.5);

/*     CALCULATE TOTAL FREQUENCY */
t = 0.0;
for( i = 0; i < n; i++ )
	t += f[i];
/*
switch( for_if(nop - 5) ){
	case -1: goto L_130;
	case  0: goto L_120;
	case  1: goto L_115;
	}
*/
if( (nop-5) < 0) goto L_130;
  else if( (nop-5) == 0) goto L_120;
	 else goto L_115;
L_115:
nop = 5;
L_120:
jump = 1;
goto L_150;
L_130:
jump = 2;

/*        FIRST MOMENT */
L_150:
for( i = 0; i < n; i++ ){
	fi = i+1;
	ans[0] += ( f[i]*(ubo[0] + (fi - 0.5)*ubo[1])  );
	}
ans[0] /= t;

switch( nop ){
	case 1: goto L_350;
	case 2: goto L_200;
	case 3: goto L_250;
	case 4: goto L_300;
	case 5: goto L_200;
	}

/*        SECOND MOMENT */
L_200:
for( i = 0; i < n; i++ ){
	fi = i+1;
        a=ubo[0] + (fi - 0.5)*ubo[1] - ans[0];
	ans[1] += (f[i]*a*a);
	}
ans[1] /= t;
switch( jump ){
	case 1: goto L_250;
	case 2: goto L_350;
	}

/*        THIRD MOMENT */
L_250:
for( i = 0; i < n; i++ ){
	fi = i+1;
        a=ubo[0] + (fi - 0.5)*ubo[1] - ans[0];
	ans[2] = ans[2] + f[i]*a*a*a;
	}
ans[2] /= t;
switch( jump ){
	case 1: goto L_300;
	case 2: goto L_350;
	}

/*        FOURTH MOMENT */
L_300:
for( i = 0; i < n; i++ ){
	fi = i+1;
        a=ubo[0] + (fi - 0.5)*ubo[1] - ans[0];
        a2=a*a;
	ans[3] += (f[i]*a2*a2);
	}
ans[3] /= t;
L_350:
return;
}

