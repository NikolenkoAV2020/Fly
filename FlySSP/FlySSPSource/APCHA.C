//------------------------------------------------------------------------
//  APCHA.C
//  ÀÏÏĞÎÊÑÈÌÀÖÈß ÇÀÄÀÍÍÎÉ ÄÈÑÊĞÅÒÍÎÉ ÔÓÍÊÖÈÈ
//  ÎĞÒÎÃÎÍÀËÜÍÛÌÈ ÏÎËÈÍÎÌÀÌÈ ×ÅÁÛØÅÂÀ C ÀÂÒÎÌÀÒÈ×ÅÑÊÈÌ ÂÛÁÎĞÎÌ
//  ÑÒÅÏÅÍÈ ÀÏÏĞÎÊÑÈÌÈĞÓŞÙÅÃÎ ÏÎËÈÍÎÌÀ
//  Àâòîğ   : Ïàñûíêîâ Â.Â.
//  Âåğñèÿ  : 01.00 / 01.06.1994 /
//------------------------------------------------------------------------
#include <string.h>
#include <math.h>
#include "ssp.h"

int apcha(double *top, double *work, int n, int m, int st_max,
	double *sqkr, double *c,
	double *xd, double *x0,
	float eps, float eta)
{
	double sum, snam, del, kappa = 0.5, sq_kr;/*kappa=0.05*/
	int    k, i, ier, ires, pow, n1, dn;
	double xa;

	if (!top || !work || !sqkr || !c || !xd || !x0)	return -1;
	ier = apch(top, n, st_max, xd, x0, work);
	pow = st_max;
	ier = apfs(work, st_max, &ires, -2, eps, eta);
	st_max = ires;
	if (ier == 1) {
		pow = ires; ires = (pow - 1)*pow / 2;
	} else {
		pow = ires; ires = (pow - 1)*pow / 2;
	}
	if (n<1) return -2;
	for (k = 1; k <= st_max; k++) {
		n1 = (k - 1)*k / 2;   dn = (k + 1)*k / 2;   dn -= n1;
		memcpy(&c[0], &work[n1], dn*sizeof(double));
		pow = k;
		for (sum = 0.0, i = 0; i<n; i++) {  /* ğàñ÷åò îòêëîíåíèé */
			xa = (*x0) + (*xd)*top[i];
			del = cnps(xa, c, pow);
			xa = (top[n + i] - del);
			sum += (xa*xa);
		}
		sum /= n;
		for (i = 0, del = 1.; i<((m>n - m) ? n - m : m); del *= (double)(n - i) / (i + 1), i++);/*Âû÷èñëåíèå êîıôô. áèíîìà*/
		snam = ((k + 1)*(1 + log((double)n / (k + 1))) - log(kappa / del)) / n;
		/*   snam=((k)*(1+log((double)n/(k)))-log(kappa/del))/n;*/
		if (snam<0.0) return 0;
		snam = 1 - sqrt(snam);
		if (snam <= 0.)  return 0;
		sq_kr = sum / snam;
		if (k>1) {
			if (*sqkr <= sq_kr)	break;
		}
		*sqkr = sq_kr;
	}
	if (k<0) return -2;
	/*k-=1;*/
	if (k >= st_max) k = st_max;
	n1 = (k - 1)*k / 2; dn = (k + 1)*k / 2; dn -= n1;
	memcpy(c, &work[n1], dn*sizeof(double));
	return k;
}

