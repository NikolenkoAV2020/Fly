#include <stdafx.h>
#pragma hdrstop

#include <FlyMMathSource\\CalcFurie.h>
 
//------------------------------------------------------------------------------
CFurie::CFurie( )
{
    Clear( ) ;
}
//------------------------------------------------------------------------------
CFurie::~CFurie( )
{
    Clear( ) ;
}
//------------------------------------------------------------------------------
void CFurie::Clear( )
{
    N = 0  ;
    T = 0 ;
    A0 = 0 ;
    I.clear( ) ;
    Y.clear( ) ;
    X.clear( ) ;
}
//------------------------------------------------------------------------------
void CFurie::AddPointXY( double x, double y )
{
    X.push_back( x ) ;
    Y.push_back( y ) ;
}
//------------------------------------------------------------------------------
int CFurie::Make( int nN )
{
    N = 0  ;
    T = 0 ;
    A0 = 0 ;
    I.clear( ) ;

    if ( X.size( ) < 1 || Y.size( ) < 1 ) return-1 ;
    N = nN ;
    I.resize( N ) ;

    int i, j ;
    int n = X.size( ) ;
    if ( n < 3 ) return-2 ;

    T = X[ n - 1 ] - X[ 0 ] ;

    A0 = 0 ;
    for ( i = 0 ; i < n - 1 ; i++ ) {
        A0+= 0.5 * ( Y[ i ] + Y[ i + 1 ] ) * ( NormX( i + 1 ) - NormX( i ) ) ;
    }
    A0/= M_PI ;

    for ( j = 1 ; j <= N ; j++ ) {
        double A, B ;
        double x, x1, nj ;
        A = 0 ;
        B = 0 ;
        for ( i = 0 ; i < n - 1 ; i++ ) {
            x  = NormX( i ) ;
            x1 = NormX( i + 1 ) ;
            nj = j ;
            A+= 0.5 * ( Y[ i ]*cos( nj*x ) + Y[ i + 1 ]*cos( nj*x1 ) ) * ( x1 - x ) ;
            B+= 0.5 * ( Y[ i ]*sin( nj*x ) + Y[ i + 1 ]*sin( nj*x1 ) ) * ( x1 - x ) ;
        }
        I[ j - 1 ].A  = A / M_PI ;
        I[ j - 1 ].B  = B / M_PI ;
        I[ j - 1 ].n  = nj ;
        I[ j - 1 ].fi = 0 ;
    }
    return 0 ;
}
//------------------------------------------------------------------------------
int CFurie::Calc( double dX, double& F )
{
    double x = NormValX( dX ) ;
    F = A0 / 2.0 ;
    for ( int j = 0 ; j < N ; j++ ) {
        F+= I[ j ].A*cos( x*I[ j ].n ) + I[ j ].B*sin( x*I[ j ].n ) ;
    }
    return 0 ;
}
//------------------------------------------------------------------------------
int CFurie::CalcDif( double dX, double& dFdX )
{
    double n, x = NormValX( dX ) ;
    dFdX = 0.0 ;
    for ( int j = 0 ; j < N ; j++ ) {
        n = I[ j ].n ;
        dFdX+= n * (-I[ j ].A*sin( x*n ) + I[ j ].B*cos( x*n ) ) ;
    }
    dFdX*= ( M_DPI / T ) ;
    return 0 ;
}
//------------------------------------------------------------------------------
int CFurie::CalcInteg( double dX, double& IF )
{
    double n, x = NormValX( dX ) ;
    IF = A0 / 2.0 * x ;
    for ( int j = 0 ; j < N ; j++ ) {
        n = I[ j ].n ;
        IF+= ( I[ j ].A*sin( x*n ) - I[ j ].B*cos( x*n ) ) / n ;
    }
    IF*= ( T / M_DPI ) ;
    return 0 ;
}
//------------------------------------------------------------------------------
double CFurie::NormX( int i )
{
    double R = M_PI * ( 2.0 * ( X[ i ] - X[ 0 ] ) / T - 1.0 ) ;
    return R ;
}
//------------------------------------------------------------------------------
double CFurie::NormValX( double dX )
{
    double R = M_PI * ( 2.0 * ( dX - X[ 0 ] ) / T - 1.0 ) ;
    return R ;
}
//------------------------------------------------------------------------------
