/*------------------------------------------------------------------------
  MAX.C
       ПРОГРАММЫ ПОИСКА МАКСИМАЛЬНОГО ЭЛЕМЕНТА
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
/* max.c - support for F77 max() function variable arg translations
				by FOR_C (TM) from COBALT BLUE
          copyright Lightfoot & Associates, Inc., 1988
                    ALL RIGHTS RESERVED
 */

#include <stdarg.h>	/* ANSI variable arg macros */
#ifndef INT_MAX
#include <limits.h>
#endif
#define IEND	(INT_MAX-1)
#define LEND	(LONG_MAX-1)
#define FEND	(1.e+38-1.)

#ifndef INLN_MXMN
	/* NOTE: These functions guarantee max() problems won't occur due to
			side effects - at the cost of an extra function call. */
int imax(int a,int b)
	{
		return( a > b ? a : b );
	}
long lmax(long a,long b)
	{
		return( a > b ? a : b );
	}
double fmax(double a,double b)
	{
		return( a > b ? a : b );
	}
#endif

	/* NOTE:
		The vfmaxi(), vfmaxl(), vmaxfi(), & vmaxfl() functions are defined
		as macros (in "f_rt.h") in terms of the three basic functions below.

		All of the remaining functions in this file are,
			VARIABLE NUMBER OF ARGRUMENT FUNCTIONS!
	 */


//double vfmax( mx, ... )	/* FLOATING POINT MAXIMUM */
//double mx;			/* (amax1(), dmax1() in F77) */

double vfmax(double  mx, ... )	/* FLOATING POINT MAXIMUM */
{
double a;
va_list args;	/* arg ptr */

if( mx == FEND )
	return( 0. );	/* NULL arg list */

va_start(args,mx);	/* initialize variable args */

while( (a=va_arg(args,double)) != FEND )
	mx = (mx > a) ? mx : a;

return( mx );
}

	/*		*		*		*		*/


long vlmax(long  mx, ... )	/* LONG INTEGER MAXIMUM */
//long mx;
{
long a;
va_list args;	/* arg ptr */

if( mx == LEND )
	return( 0L );	/* NULL arg list */

va_start(args,mx);	/* initialize variable args */

while( (a=va_arg(args,long)) != LEND )
	mx = (mx > a) ? mx : a;

return( mx );
}

	/*		*		*		*		*/

int vimax(int  mx, ... )	/* INTEGER MAXIMUM */
//int mx;			/* (max0() in F77) */
{
int a;
va_list args;	/* arg ptr */

if( mx == IEND )
	return( 0 );	/* NULL arg list */

va_start(args,mx);	/* initialize variable args */

while( (a=va_arg(args,int)) != IEND )
	mx = (mx > a) ? mx : a;

return( mx );
}
