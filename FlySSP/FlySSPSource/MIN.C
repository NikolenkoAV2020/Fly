/*------------------------------------------------------------------------
  MIN.C
       ПРОГРАММЫ ПОИСКА МИНИМАЛЬНОГО ЭЛЕМЕНТА
  Автор   : Перевод программы пакета SSP на С выполнил Пасынков В.В.
  Версия  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include <stdarg.h>	// ANSI variable arg macros 
#include "ssp.h"
#ifndef INT_MAX
#include <limits.h>
#endif

/*

#ifndef INLN_MXMN
	// NOTE: These functions guarantee min() problems won't occur due to
	// side effects - at the cost of an extra function call. 
int imin(int a,int b)
	{
		return( a < b ? a : b );
	}
long lmin(long a,long b)
	{
		return( a < b ? a : b );
	}
double fmin(double a,double b)
	{
		return( a < b ? a : b );
	}
#endif

	// NOTE:
	// The vfmini(), vfminl(), vminfi(), & vminfl() functions are defined
	// as macros (in "f_rt.h") in terms of the three basic functions below.

	// All of the functions in the rest of this file are,
	// VARIABLE NUMBER OF ARGRUMENT FUNCTIONS!

double vfmin(double  mn, ... )	// FLOATING POINT MINIMUM 
{
double a;
va_list args;	// arg ptr 

if( mn == FEND )
	return( 0. );	// NULL arg list 

va_start(args,mn);	// initialize variable args 

while( (a=va_arg(args,double)) != FEND )
	mn = (mn < a) ? mn : a;

return( mn );
}

long vlmin(long  mn, ... )	// LONG INTEGER MINIMUM 
{
long a;
va_list args;	// arg ptr 

if( mn == LEND )
	return( 0L );	// NULL arg list 

va_start(args,mn);	// initialize variable args

while( (a=va_arg(args,long)) != LEND )
	mn = (mn < a) ? mn : a;

return( mn );
}

int vmin(int  mn, ... )	// INTEGER MINIMUM 
//int mn;			    // (min0() in F77)
{
int a;
va_list args;	// arg ptr 

if( mn == IEND )
	return( 0 );	// NULL arg list 

va_start(args,mn);	// initialize variable args

while( (a=va_arg(args,int)) != IEND )
	mn = (mn < a) ? mn : a;

return( mn );
}

*/