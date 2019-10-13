/*---------------------------------------------------------------------------
  RANDTD.C
	ГЕНЕРАТОР ПСЕВДОСЛУЧАЙНЫХ ЧИСЕЛ ПО РАВНОМЕРНОМУ ЗАКОНУ (Пайк,Хилл)
  Автор	 : перевод на С выполнил Пасынков В.В.
  Версия : 01.00 / 15.04.1994 /
---------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

	SUBROUTINE RANDTD

	PURPOSE
	   COMPUTES UNIFORMLY DISTRIBUTED RANDOM REAL NUMBERS BETWEEN
	   0 AND 1.0 AND RANDOM INTEGERS BETWEEN ZERO AND
	   2**31. EACH ENTRY USES AS INPUT AN INTEGER RANDOM NUMBER
	   AND PRODUCES A NEW INTEGER AND REAL RANDOM NUMBER.

	USAGE
	   RANDTD(X)

     ..................................................................
 */
double  randtd(void)
{
static long i=783637L;
const  long cf=2796203L;
double x;

i *= 125L;
i -= (i/cf)*cf;
x = (double)i;
x/=(double)(cf-1L);
return x;
}

