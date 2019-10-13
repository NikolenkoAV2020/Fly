//------------------------------------------------------------------------
//   PVSUB.C
//     SUBSTITUTE VARIABLE OF A POLYNOMIAL BY ANOTHER POLYNOMIAL
//   Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
//   Версия : 01.00 / 10.06.1996 /
//------------------------------------------------------------------------
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE PVSUB

        PURPOSE
           SUBSTITUTE VARIABLE OF A POLYNOMIAL BY ANOTHER POLYNOMIAL

        USAGE
           CALL PVSUB(Z,IDIMZ,X,IDIMX,Y,IDIMY,WORK1,WORK2)

	DESCRIPTION OF PARAMETERS
           Z     - VECTOR OF COEFFICIENTS FOR RESULTANT POLYNOMIAL,
                   ORDERED FROM SMALLEST TO LARGEST POWER
           IDIMZ - DIMENSION OF Z
           X     - VECTOR OF COEFFICIENTS FOR ORIGINAL POLYNOMIAL,
                   ORDERED FROM SMALLEST TO LARGEST POWER
           IDIMX - DIMENSION OF X
           Y     - VECTOR OF COEFFICIENTS FOR POLYNOMIAL WHICH IS
                   SUBSTITUTED FOR VARIABLE, ORDERED FROM SMALLEST TO
                   LARGEST POWER
           IDIMY - DIMENSION OF Y
           WORK1 - WORKING STORAGE ARRAY (SAME DIMENSION AS Z)
	   WORK2 - WORKING STORAGE ARRAY (SAME DIMENSION AS Z)

        REMARKS
           NONE

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           PMPY
           PADDM
	   PCLA

        METHOD
           VARIABLE OF POLYNOMIAL X IS SUBSTITUTED BY POLYNOMIAL Y
           TO FORM POLYNOMIAL Z. DIMENSION OF NEW POLYNOMIAL IS
           (IDIMX-1)*(IDIMY-1)+1. SUBROUTINE REQUIRES TWO WORK AREAS

     ..................................................................
 */
void pvsub(double z[], int *idimz, double x[], int idimx, double y[], int idimy,
	   double work1[], double work2[])
{
int    i, idimr, iw1, iw2;
double fact;


/*     TEST OF DIMENSIONS */
switch( for_if(idimx - 1) ){
	case -1: goto L_1;
	case  0: goto L_3;
	case  1: goto L_3;
	}
L_1:
(*idimz) = 0;
L_2:
return;

L_3:
(*idimz) = 1;
z[0] = x[0];
switch( for_if(idimy*idimx - idimy) ){
	case -1: goto L_2;
	case  0: goto L_2;
	case  1: goto L_4;
	}
L_4:
iw1 = 1;
work1[0] = 1.;

for( i = 2; i <= idimx; i++ ){
	pmpy( work2, &iw2, y, idimy, work1, iw1 );
	pcla( work1, &iw1, work2, iw2 );
	fact = x[i-1];
	paddm( z, &idimr, z, *idimz, fact, work1, iw1 );
	(*idimz) = idimr;
	}
goto L_2;
}

