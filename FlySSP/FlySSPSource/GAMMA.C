/*---------------------------------------------------------
  GAMMA.C
    РАСЧЕТ ЗНАЧЕНИЯ ГАММА-ФУНКЦИИ ДЛЯ ПРОИЗВОЛЬНОГО АРГУМЕНТА
  Автор	  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
---------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/*    ..................................................................

	 GMMMA

	 PURPOSE
	    COMPUTES THE GAMMA FUNCTION FOR A GIVEN ARGUMENT

	 USAGE
	  IER =  GMMMA(XX,GX);

	 DESCRIPTION OF PARAMETERS
	    XX -THE ARGUMENT FOR THE GAMMA FUNCTION
	    *GX -THE RESULTANT GAMMA FUNCTION VALUE
	    IER-RESULTANT ERROR CODE WHERE
		IER=0  NO ERROR
		IER=1  XX IS WITHIN .000001 OF BEING A NEGATIVE INTEGER
		IER=2  XX GT 57, OVERFLOW, GX SET TO 1.0E38

	 REMARKS
	    NONE

	 SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	    NONE

	 METHOD
	    THE RECURSION RELATION AND POLYNOMIAL APPROXIMATION
	    BY C.HASTINGS,JR., 'APPROXIMATIONS FOR DIGITAL COMPUTERS',
	    PRINCETON UNIVERSITY PRESS, 1955

      ..................................................................*/


int gamma(float XX,float *GX)
{
int    IER;
float  ERR,X,Y,GY;
  if((double)XX-57.>0.0) goto l4;
    else goto l6;
    l4:
      IER=2;
	  *GX=(float)1.E38;
      return IER;
    l6:
     X=XX;
	 ERR=(float)1.0E-6;      IER=0;      *GX=(float)1.0;
	  if((double)X-2.0 > 0.0) goto l15;
      else goto l50;

   l10:
	  if((double)X-2.0 > 0.0) goto l15;
      else goto l110;
   l15: X=X-(float)1.0;
      *GX *=X;
      goto l10;
   l50:
	  if((double)X-1.0 < 0.0) goto l60;
	  if((double)X-1.0 == 0.0) goto l120;
	  if((double)X-1.0 > 0.0) goto l110;

/*       SEE if X IS NEAR NEGATIVE INTEGER OR ZERO */

   l60:
	  if((double)(X-ERR) <= 0.0) goto l62;
      else  goto l80;
   l62:
      Y=(float)floor(X)-X;
	  if(fabs(Y)-(double)ERR > 0.0) goto l64;
       else goto l130;
   l64:
	  if((double)(1.0-Y-ERR) > 0.0) goto l70;
       else goto l130;

/*       X NOT NEAR A NEGATIVE INTEGER OR ZERO */
   l70:
	  if( (double)X-1.0 > 0.0) goto l110;
       else goto l80;
   l80:
    *GX/=X;         X=X+1.0f;
      goto l70;
  l110:
   Y=X-1.0f;
   GY=1.0f+Y*(-0.5771017f+Y*(+0.9858540f+Y*(-0.8764218f+Y*(+0.8328212f+
	 Y*(-0.5684729f+Y*(+0.2548205f+Y*(-0.05149930f)))))));
  *GX *=GY;
  l120:
   return IER;
  l130:
   IER=1;
  return IER;
}
