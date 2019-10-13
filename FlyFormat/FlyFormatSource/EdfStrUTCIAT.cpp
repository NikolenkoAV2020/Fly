//------------------------------------------------------------------------------
// EdfStrUTCIAT.cpp : 
// А. Николенко 03.08.2018
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyTime.h>
#include <FlyFormat.h>
#pragma hdrstop

//------------------------------------------------------------------------------
bool CalcDMVTime( double TATime, int& D, int& M, int& G, int& H, int& Min, double& S )
{
	TATime += GetLeapSeconds(TATime);
    TATime+= 0.125 ;
	//
    CRDate     dmy ;
	CRGradTime gt ;
    double dt, V, d ;
    int    k ;
    dt = fabs( TATime ) ;
    dt = dt - floor( dt ) ;
    V  = TATime ;
    //
    if ( dt > 0 && TATime < 0 ) { V-= 1 ; dt = 1 - dt ; }
    dmy = ( long )V + JULDAY_01011975_FROMZERO ;
	dmy.SetFlags( 3 ) ;
	dmy.GetFields( &D, &M, &G ) ;
    gt.fromDouble( dt, EDF_SSS ) ;
    gt.GetFields ( &H, &Min, &k, &d ) ; S = k + d / 1000.0 ;
    return 0 ;
}
//------------------------------------------------------------------------------
bool CalcTATime( double& TATime, int D, int M, int G, int H, int Min, double S )
{
    CRDate     dmy ;
    CRGradTime gt ;
    long       days ;
    //
    int k = ( int )S ;
    double d = ( S - k ) * 1000.0 ;
    gt.SetFields( H, Min, k, d ) ;
	TATime = gt.toDouble( EDF_SSS ) ;
    //
	dmy.SetFlags( 3 ) ;
    if( dmy.IsValiFields( D, M, G ) ) return 1 ;
	dmy.SetFields( D, M, G ) ;
    days = dmy.julDayNumber() ;
	days-= JULDAY_01011975_FROMZERO ;
    //
    TATime+= ( double )days ;
	TATime -= GetLeapSeconds(TATime);
    TATime-= 0.125 ;
    //
    return 0 ;
}

//------------------------------------------------------------------------------
