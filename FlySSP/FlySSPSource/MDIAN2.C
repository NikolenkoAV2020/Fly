//---------------------------------------------------------------------------
//  MDIAN2.C
//    €‘—…’ ‚›—‰ …„€›
//  €Άβ®ΰ  :  αλ­ª®Ά ‚.‚.
//  ‚¥ΰα¨ο : 01.00 / 25.09.2000 /
//---------------------------------------------------------------------------
#include "ssp.h"
#include <math.h>

#define BIG 1.0e30
#define AFAC 1.5
#define AMP 1.5

double mdian2(double x[],int n)
{
int    np,nm,j;
double xx,xp,xm,sumx,sum,eps,stemp,dum,ap,am,aa,a;
double xmed=0;
  if(n>0 && x)
    {
	if(n==1)
	  return x[0];
	a=0.5*(x[0]+x[n-1]);
	eps=fabs(x[n-1]-x[0]);
	am = -(ap=BIG);
	for (;;) {
		sum=sumx=0.0;
		np=nm=0;
		xm = -(xp=BIG);
		for (j=0;j<n;j++) {
			xx=x[j];
			if (xx != a) {
				if (xx > a) {
					++np;
					if (xx < xp) xp=xx;
				} else if (xx < a) {
					++nm;
					if (xx > xm) xm=xx;
				}
				sum += dum=1.0/(eps+fabs(xx-a));
				sumx += xx*dum;
			}
		}
		stemp=(sumx/sum)-a;
		if (np-nm >= 2) {
			am=a;
			aa =  stemp < 0.0 ? xp : xp+stemp*AMP;
			if (aa > ap) aa=0.5*(a+ap);
			eps=AFAC*fabs(aa-a);
			a=aa;
		} else if (nm-np >= 2) {
			ap=a;
			aa = stemp > 0.0 ? xm : xm+stemp*AMP;
			if (aa < am) aa=0.5*(a+am);
			eps=AFAC*fabs(aa-a);
			a=aa;
		} else {
			if (n % 2 == 0) {
				xmed = 0.5*(np == nm ? xp+xm : np > nm ? a+xp : xm+a);
			} else {
				xmed = np == nm ? a : np > nm ? xp : xm;
			}
			return xmed;
		}
	}
    }
return xmed;
}

#undef BIG
#undef AFAC
#undef AMP
