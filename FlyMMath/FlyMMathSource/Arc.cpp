//------------------------------------------------------------------------------
// Arc.cpp
// А. Николенко 03.08.2018
//------------------------------------------------------------------------------
#include <stdafx.h>
#include <math.h>
#include <FlyMMathSource\\MMathDef.h>
#pragma hdrstop
//------------------------------------------------------------------------------
#include "Arc.h"

//------------------------------------------------------------------------------
FLYMMATH_API double Vali2PI(double u, bool FlagPI)
{
    if ( FlagPI ) {
		if (u>M_DPI) u = u - (int)(u / M_DPI) * M_DPI;
		if (u<0    ) u = u -((int)(u / M_DPI) - 1) * M_DPI;
    } else {
		if (u>360.0) u = u- (int)(u/360.0)*360.0 ;
		if (u<0    ) u = u-((int)(u/360.0)-1)*360.0 ;
    }
    return u ;
}

FLYMMATH_API double ValiPI(double u, bool FlagPI)
{
    if (FlagPI) {
		if (u> M_PI) u = u - ((int)(u / M_DPI) + 1) * M_DPI;
		if (u<-M_PI) u = u - ((int)(u / M_DPI) - 1) * M_DPI;
    } else {
      if (u> 180.0) u = u-((int)(u/(360.0))+1)*360.0 ;
      if (u<-180.0) u = u-((int)(u/(360.0))-1)*360.0 ;
    }
    return u ;
}

FLYMMATH_API double ARad(double u)
{
	return (u/KU);
}

FLYMMATH_API double AGrd(double u)
{
	return (u*KU);
}

int  CArc::PI2_global_flag = 1 ;

CArc::CArc(CArc& arc)
{
    m_a        = arc.m_a ;
    m_rad_flag = arc.m_rad_flag ;
}

CArc::CArc ( double arc, int radian_flag )
{
    m_rad_flag = radian_flag ;
    if ( m_rad_flag ) m_a = arc ;
    else              m_a = arc * KU ;
    Interval ( ) ;
}

double CArc::CalcSC ( double s, double c )
{
    double u ;
    if ( fabs( s ) + fabs( c ) < EpsMZ ) return 0 ;
    u = atan2( s, c ) ;
    if ( CArc::PI2_global_flag ) ValiArc2PI( u ) ;
    return u ;
}

void CArc::ValiArc2PI( double& u )
{
	if (u > M_DPI) u = u - (int)(u / (M_DPI)) * M_DPI;
	if (u < 0) u = u - ((int)(u / (M_DPI)) - 1) * M_DPI;
}

void CArc::ValiArcPI ( double& u )
{
	if (u > M_PI) u = u - ((int)(u / (M_DPI)) + 1) * M_DPI;
	if (u <-M_PI) u = u - ((int)(u / (M_DPI)) - 1) * M_DPI;
}

int CArc::QtrArc ( double u )
{
	if (u >= 0 && u < M_PI_2) return 1;
	if (u >= M_DPI / 2.0 && u < M_DPI) return 2;
	if (u >= M_DPI   && u < 3.0 * M_DPI / 2.0) return 3;
    return 4 ;
}

void CArc::Interval ( )
{
   if ( CArc::PI2_global_flag ) Int_02P ( ) ;
   else                         Int_PP  ( ) ;
}

void CArc::Int_PP  ( )
{
   if ( m_rad_flag ) {
	   if (m_a > M_PI) m_a = m_a - ((int)(m_a / M_DPI) + 1) * M_DPI;
	   if (m_a <-M_PI) m_a = m_a - ((int)(m_a / M_DPI) - 1) * M_DPI;
   }
   else {
        if ( m_a > 180.0 ) m_a = m_a - ( ( int )( m_a / ( 360.0 ) ) + 1 ) * 360.0 ;
        if ( m_a <-180.0 ) m_a = m_a - ( ( int )( m_a / ( 360.0 ) ) - 1 ) * 360.0 ;
   }
}

void CArc::Int_02P ( )
{
   if ( m_rad_flag ) {
	   if (m_a > M_DPI) m_a = m_a - (int)(m_a / M_DPI) * M_DPI;
	   if (m_a < 0    ) m_a = m_a -((int)(m_a / M_DPI) - 1) * M_DPI;
      }
   else {
        if ( m_a > 360.0  ) m_a = m_a -   ( int )( m_a / ( 360.0  ) ) * 360.0 ;
        if ( m_a < 0      ) m_a = m_a - ( ( int )( m_a / ( 360.0  ) ) - 1 ) * 360.0 ;
      }
}

int CArc::IsRadian  ( )
{
   return m_rad_flag ;
}

void  CArc::SetRadian ( )
{
   if ( !m_rad_flag ) { m_a *= KU ; m_rad_flag = 1 ; }
}

void  CArc::SetGradus ( )
{
   if (  m_rad_flag ) { m_a /= KU ; m_rad_flag = 0 ; }
}

double CArc::ReductionDim ( CArc& arc )
{
   if ( m_rad_flag != arc.m_rad_flag )
        return ( m_rad_flag == 1 ? arc.m_a * KU : arc.m_a / KU ) ;
   else
        return arc.m_a ;
}

CArc::operator double ( ) { return m_a ; }
CArc::operator int    ( ) { return ( int )m_a ; }
CArc::operator short  ( ) { return ( short)m_a ; }

CArc& CArc::operator = ( CArc& arc )
{
   m_a = ReductionDim ( arc ) ;
   Interval ( ) ;
   return *this ;
}

CArc& CArc::operator = ( double a )
{
   m_a = a ;
   Interval ( ) ;
   return *this ;
}

CArc  CArc::operator - ( )
{
   CArc  arc ;
   arc.m_a = -m_a ;
   return arc ;
}

CArc operator + ( CArc& a1, CArc& a2 )
{
   CArc  arc( 0, a1.IsRadian( ) ) ;
   arc.m_a = a1.m_a + a1.ReductionDim( a2 ) ;
   return arc ;
}

CArc operator + ( CArc& a1,  double&  a2 )
{
   CArc  arc( 0, a1.IsRadian( ) ) ;
   arc.m_a = a1.m_a + a2 ;
   return arc ;
}

CArc operator + ( double& a1, CArc& a2 )
{
   CArc  arc( 0, a2.IsRadian( ) ) ;
   arc.m_a = a1 + a2.m_a ;
   return arc ;
}

CArc operator - ( CArc& a1, CArc& a2 )
{
   CArc  arc( 0, a1.IsRadian( ) ) ;
   arc.m_a = a1.m_a - a1.ReductionDim( a2 ) ;
   return arc ;
}

CArc operator - ( CArc& a1, double& a2 )
{
   CArc  arc( 0, a1.IsRadian( ) ) ;
   arc.m_a = a1.m_a - a2 ;
   return arc ;
}

CArc operator - ( double& a1, CArc& a2 )
{
   CArc  arc( 0, a2.IsRadian( ) ) ;
   arc.m_a = a1 - a2.m_a ;
   return arc ;
}

CArc operator * ( CArc& a1, CArc& a2 )
{
   CArc  arc( 0, a1.IsRadian( ) ) ;
   arc.m_a = a1.m_a * a1.ReductionDim( a2 ) ;
   return arc ;
}

CArc operator * ( CArc& a1, double& a2 )
{
   CArc  arc( 0, a1.IsRadian( ) ) ;
   arc.m_a = a1.m_a * a2 ;
   return arc ;
}

CArc operator * ( double& a1, CArc& a2 )
{
   CArc  arc( 0, a2.IsRadian( ) ) ;
   arc.m_a = a1 * a2.m_a ;
   return arc ;
}

CArc operator / ( CArc& a1, CArc& a2 )
{
   CArc  arc( 0, a1.IsRadian( ) ) ;
   arc.m_a = a1.m_a / a1.ReductionDim( a2 ) ;
   return arc ;
}

CArc operator / ( CArc& a1, double& a2 )
{
   CArc  arc( 0, a1.IsRadian( ) ) ;
   arc.m_a = a1.m_a / a2 ;
   return arc ;
}

CArc operator / ( double& a1, CArc& a2 )
{
   CArc  arc( 0, a2.IsRadian( ) ) ;
   arc.m_a = a1 / a2.m_a ;
   return arc ;
}