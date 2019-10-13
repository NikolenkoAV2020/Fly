//-----------------------------------------------------------------------------
// ZCDU.cpp
// А. Николенко 16.08.2018
//-----------------------------------------------------------------------------
#include <stdafx.h>

#include <FlyCoreSource\\ZCDU.h>

//-----------------------------------------------------------------------------
int ZFisNotValiDU(ZCDU& DU)
{
	DU.isNotValiDU = 1 ;
    if (DU.k<=0) return 1 ;
    int j = 0 ;
    for (int i=0 ; i<DU.k ; i++) {
		if (i>0 && i<DU.k-1) {
			if (DU.i[i].p<=0) return 2 ;
			if (DU.i[i].t<=0) return 3 ;
		}
		if (!(DU.i[i].pu<=1e-12 && DU.i[i].ms<1e-12)) {
			if (DU.i[i].pu<=1e-12) DU.i[i].pu = DU.i[i].p/DU.i[i].ms ;
			if (DU.i[i].ms<=1e-12) DU.i[i].ms = DU.i[i].p/DU.i[i].pu ;
		} else if (i>0 && i<DU.k-1) return 4 ;
		j++ ;
    }
    DU.isNotValiDU = j>0?0:1 ;
    return 0 ;
}

//-----------------------------------------------------------------------------
int ZFDUFindMain(ZCDU& DU)
{
    int i, mu = 0 ;
    double maxt = 0 ;
    for (i=0 ; i<DU.k ; i++) {
		if (maxt<DU.i[i].t) { maxt = DU.i[i].t ; mu=i ; }
	}
    DU.mp = mu<DU.k?DU.i[mu].p:0 ;
    DU.mt = mu<DU.k?DU.i[mu].t:0 ;
    DU.mu = mu ;
    DU.ip = DU.mu<DU.k-1?1:0 ;
    return 0 ;
}

//-----------------------------------------------------------------------------
