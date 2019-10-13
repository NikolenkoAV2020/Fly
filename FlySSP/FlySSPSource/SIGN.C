/* sign.c - support for F77 sign() function translations
				by FOR_C (TM) from COBALT BLUE
          copyright Lightfoot & Associates, Inc., 1987, 1988
                    ALL RIGHTS RESERVED
 */
#include <math.h>
#include "ssp.h"

#ifndef INLN_SIGN
	int isign( int a, int b )		/* integer sign transfer */
	{
		return( b < 0 ? -abs(a) : abs(a) );
	}

	long lsign( long a, long b )	/* long sign transfer */
	{
		return( b < 0 ? -labs(a) : labs(a) );
	}

	double dsign( double a, double b )/* floating point sign transfer */
	{
		return( b < 0 ? -fabs(a) : fabs(a) );
	}
#endif
