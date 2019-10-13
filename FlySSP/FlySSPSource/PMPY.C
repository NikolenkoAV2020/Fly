//------------------------------------------------------------------------
//   PMPY.C
//     Умножение oднoгo пoлинoмa нa дpугoй
//   Автор  : Перевод программы пакета SSP на С выполнил Пасынков В.В.
//   Версия : 01.00 / 10.06.1996 /
//------------------------------------------------------------------------
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE PMPY

        PURPOSE
           MULTIPLY TWO POLYNOMIALS

        USAGE
           CALL PMPY(Z,IDIMZ,X,IDIMX,Y,IDIMY)

        DESCRIPTION OF PARAMETERS
           Z     - VECTOR OF RESULTANT COEFFICIENTS, ORDERED FROM
                   SMALLEST TO LARGEST POWER
           IDIMZ - DIMENSION OF Z (CALCULATED)
           X     - VECTOR OF COEFFICIENTS FOR FIRST POLYNOMIAL, ORDERED
                   FROM SMALLEST TO LARGEST POWER
	   IDIMX - DIMENSION OF X (DEGREE IS IDIMX-1)
           Y     - VECTOR OF COEFFICIENTS FOR SECOND POLYNOMIAL,
                   ORDERED FROM SMALLEST TO LARGEST POWER
	   IDIMY - DIMENSION OF Y (DEGREE IS IDIMY-1)

        REMARKS
           Z CANNOT BE IN THE SAME LOCATION AS X
           Z CANNOT BE IN THE SAME LOCATION AS Y

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
	   DIMENSION OF Z IS CALCULATED AS IDIMX+IDIMY-1
           THE COEFFICIENTS OF Z ARE CALCULATED AS SUM OF PRODUCTS
           OF COEFFICIENTS OF X AND Y , WHOSE EXPONENTS ADD UP TO THE
	   CORRESPONDING EXPONENT OF Z.

     ..................................................................
 */
void pmpy(double z[], int *idimz, double x[], int idimx, double y[], int idimy)
{
int i, j, k;

if(idimx*idimy <=0)
  {
  (*idimz) = 0;
  return;
  }
k=(*idimz) = idimx + idimy - 1;
for( i = 0; i < k; i++ ) z[i] = 0.;

for( i = 0; i < idimx; i++ ){
	for( j = 0; j < idimy; j++ ){
		k = i + j ;
		z[k] += (x[i]*y[j]);
		}
	}
return;
}

