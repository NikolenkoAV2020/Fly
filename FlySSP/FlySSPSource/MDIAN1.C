//---------------------------------------------------------------------------
//  MDIAN1.C
//    €‘—…’ ‚›—‰ …„€›
//  €Άβ®ΰ  :  αλ­ª®Ά ‚.‚.
//  ‚¥ΰα¨ο : 01.00 / 25.09.2000 /
//---------------------------------------------------------------------------
#include "ssp.h"
#include <stdlib.h>

int _dcmp(const void *a1, const void *a2)
{
double *x1=(double *)a1,
       *x2=(double *)a2;
if( *x1 > *x2)
  return 1;
  else
  if( *x1 < *x2 )
    return -1;
return 0;
}

double mdian1(double x[],int n)
{
int    n2,n2p;
double xmed=0;
  if(n>0 && x!=NULL)
    {
	qsort(x,n,sizeof(double),_dcmp);
	n2p=(n2=n/2)+1;
	xmed=(n % 2 ? x[n2p-1] : 0.5*(x[n2-1]+x[n2p-1]));
    }
return xmed;
}
