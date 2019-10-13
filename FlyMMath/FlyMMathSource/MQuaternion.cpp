//------------------------------------------------------------------------------
// MQuaterniom.cpp
// А. Николенко 03.08.2018
//------------------------------------------------------------------------------
#include <stdafx.h>
#pragma hdrstop
//------------------------------------------------------------------------------
#include <FlyMMathSource\\MMath.h>
#include <FlyMMathSource\\MQuaternion.h>

//------------------------------------------------------------------------------
int QuaternionToMatrix(double W, double X, double Y, double Z, double* M, bool dim4)
{
    double xx = X * X;
    double xy = X * Y;
    double xz = X * Z;
    double xw = X * W;

    double yy = Y * Y;
    double yz = Y * Z;
    double yw = Y * W;

    double zz = Z * Z;
    double zw = Z * W;

    M[0]  = 1.0 - 2.0 * ( yy + zz );
    M[1]  =       2.0 * ( xy - zw );
    M[2]  =       2.0 * ( xz + yw );

    M[3+dim4]  =       2.0 * ( xy + zw );
    M[4+dim4]  = 1.0 - 2.0 * ( xx + zz );
    M[5+dim4]  =       2.0 * ( yz - xw );

    M[6+2*dim4]  =       2.0 * ( xz - yw );
    M[7+2*dim4]  =       2.0 * ( yz + xw );
    M[8+2*dim4] = 1.0 - 2.0 * ( xx + yy );

    if ( dim4 ) {
        M[3] = M[7] = M[11] = M[12] = M[13] = M[14] = 0;
        M[15] = 1;
    }
    for ( int i = 0 ; i < 9 + dim4*7 ; i++ ) {
        if ( fabs( M[ i ] ) < 1e-15 ) M[ i ] = 0 ;
    }
	return 0 ;
}
//------------------------------------------------------------------------------
int QuaternionToMatrix33( double w, double x, double y, double z, double* M )
{
    return( QuaternionToMatrix( w, x, y, z, M, 0 ) ) ;
}
//------------------------------------------------------------------------------
int QuaternionToMatrix44( double w, double x, double y, double z, double* M )
{
    return( QuaternionToMatrix( w, x, y, z, M, 1 ) ) ;
}

//------------------------------------------------------------------------------


 