/*---------------------------------------------------------------------------
  RANDU.C
	ÉÖçÖêÄíéê èëÖÇÑéëãìóÄâçõï óàëÖã èé êÄÇçéåÖêçéåì áÄäéçì
  Ä¢‚Æ‡	 : Ø•‡•¢Æ§ ≠† ë ¢ÎØÆ´≠®´ è†·Î≠™Æ¢ Ç.Ç.
  Ç•‡·®Ô : 01.00 / 15.04.1994 /
---------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

        SUBROUTINE RANDU

        PURPOSE
           COMPUTES UNIFORMLY DISTRIBUTED RANDOM REAL NUMBERS BETWEEN
           0 AND 1.0 AND RANDOM INTEGERS BETWEEN ZERO AND
           2**31. EACH ENTRY USES AS INPUT AN INTEGER RANDOM NUMBER
           AND PRODUCES A NEW INTEGER AND REAL RANDOM NUMBER.

        USAGE
	   RANDU(IX,IY,YFL)

	DESCRIPTION OF PARAMETERS
	   IX - FOR THE FIRST ENTRY THIS MUST CONTAIN ANY ODD INTEGER
                NUMBER WITH NINE OR LESS DIGITS. AFTER THE FIRST ENTRY,
                IX SHOULD BE THE PREVIOUS VALUE OF IY COMPUTED BY THIS
                SUBROUTINE.
           IY - A RESULTANT INTEGER RANDOM NUMBER REQUIRED FOR THE NEXT
                ENTRY TO THIS SUBROUTINE. THE RANGE OF THIS NUMBER IS
                BETWEEN ZERO AND 2**31
           YFL- THE RESULTANT UNIFORMLY DISTRIBUTED, FLOATING POINT,
                RANDOM NUMBER IN THE RANGE 0 TO 1.0

        REMARKS
           THIS SUBROUTINE IS SPECIFIC TO SYSTEM/360 AND WILL PRODUCE
	   2**29 TERMS BEFORE REPEATING.  THE REFERENCE BELOW DISCUSSES
           SEEDS (65539 HERE), RUN PROBLEMS, AND PROBLEMS CONCERNING
           RANDOM DIGITS USING THIS GENERATION SCHEME.  MACLAREN AND
           MARSAGLIA, JACM 12, P. 83-89, DISCUSS CONGRUENTIAL
           GENERATION METHODS AND TESTS.  THE USE OF TWO GENERATORS OF
           THE RANDU TYPE, ONE FILLING A TABLE AND ONE PICKING FROM THE
           TABLE, IS OF BENEFIT IN SOME CASES.  65549 HAS BEEN
           SUGGESTED AS A SEED WHICH HAS BETTER STATISTICAL PROPERTIES

     ..................................................................
 */
double  randu(long ix,long *iy)
{
double yfl;

(*iy) = ix * 65539L;
if( (*iy) < 0)
   {
   (*iy) += ( long ) 2147483647L ;
   (*iy) += 1L;
   }
yfl = (*iy);
yfl *= 0.4656613e-9;
return yfl;
} /* end of function */

