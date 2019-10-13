//----------------------------------------------------------------------------------------------------------
// NRR.cpp
// Артём Николаев 28.08.2014
//----------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>

#include <FlyTime.h>
#include "NRR.h"

NRR::NRR( ) 
{
	memset(&m_FileName, 0, sizeof(m_FileName));
	memset(&m_dt, 0, sizeof(DATE));
	memset(m_Name, 0, sizeof(m_Name));
	memset(m_Obj, 0, sizeof(m_Obj));
	m_Nv = 0;
	m_Na = 0;
}

//----------------------------------------------------------------------------------------------------------
NRR::~NRR( ) 
{
	
}

//----------------------------------------------------------------------------------------------------------
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ 
//----------------------------------------------------------------------------------------------------------
// Возврат
// >0 - Ok (смещение в строке)
// -1 - достигнут конец строки
int NRR::SkipSpace( char* S )
{
    int k = 0 ;
    while ( 1 ) {
		if ( S[ 0 ] == '\0' ) { k =-1 ; break ; }
		if ( S[ 0 ] != ' ' ) break ; 
		S++ ; k++ ;
    }
    return k ;
}

//----------------------------------------------------------------------------------------------------------
// Возврат
// >0 - Ok (смещение в строке)
// -1 - достигнутконец строки
int NRR::SkipChar( char* S )
{
    int k = 0 ;
    while ( 1 ) {
		if ( S[ 0 ] == '\0' ) { k =-1 ; break ; }
		if ( S[ 0 ] == '0' || S[ 0 ] == '1' || S[ 0 ] == '2' ||
			S[ 0 ] == '3' || S[ 0 ] == '4' || S[ 0 ] == '5' || S[ 0 ] == '6' ||
			S[ 0 ] == '7' || S[ 0 ] == '8' || S[ 0 ] == '9' ) break ;
		S++ ; k++ ;
    }
    return k ;
}

//----------------------------------------------------------------------------------------------------------
// Возврат
// >0 - Ok (смещение в строке)
// -1 - достигнут конец строки
int NRR::SkipNum( char* S )
{
    int k = 0 ;
    while ( 1 ) {
		if ( S[ 0 ] == '\0' ) break ; 
		if ( S[ 0 ] == '0' || S[ 0 ] == '1' || S[ 0 ] == '2' ||
			S[ 0 ] == '3' || S[ 0 ] == '4' || S[ 0 ] == '5' || S[ 0 ] == '6' ||
			S[ 0 ] == '7' || S[ 0 ] == '8' || S[ 0 ] == '9' ) {
			S++ ; k++ ; continue ;
		}
		break ;
    }
    return k ;
}

//----------------------------------------------------------------------------------------------------------
// Возврат
//>=0 - Ok (смещение в строке)
// -1 - достигнут конец строки
int NRR::SkipZero( char* S )
{
    int k = 0 ;
    while ( 1 ) {
		if ( S[ 0 ] == '0' ) { S++ ; k++ ; continue ; }
		break ;
    }
    return k ;
}

//----------------------------------------------------------------------------------------------------------
// Возврат
// >0 - Ok (смещение в строке)
// -1 - число не найдено
int NRR::GetNum( char* S, double& d )
{
    int    dk = 0, k = 0 ;
    char   SubS[ 256 ] ;
    double Zn = 1.0  ;
    dk = SkipChar( S ) ; if ( dk < 0 ) return -1 ;
    S = &S[ dk ] ; k+= dk ;
    if ( dk && *( S - 1 ) == '-' ) Zn =-1.0 ;
    dk = SkipZero( S ) ; if ( dk < 0 ) return -1 ;
    S = &S[ dk ] ; k+= dk ;
    bool isZero = ( dk > 0 ) ;
    dk = SkipNum ( S ) ;
    if ( dk < 0 ) {
		if ( isZero ) { d = 0 ; return k ; }
		else return -1 ;
    }
    if ( dk > 0 ) {
		memcpy( SubS, S, dk ) ; SubS[ dk ] = '\0' ; k+= dk ;
		sscanf( SubS, "%lf", &d ) ;
    } else {
		if ( isZero ) d = 0 ;
		else return-1 ;
    }
    d *= Zn ;
    return k ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::GetWord( char* S, RWord& HW ) 
{
	HW.nspace = 0 ;
	HW.n = 0 ;
	HW.s[ 0 ] = '\0' ;

    int    dk = 0, k = 0 ;
    char   SubS[ 256 ] ;
	while( 1 ) {
		if ( *S == '\0' ) { SubS[ k ] = '\0' ; k =-1 ; break ; }
		if ( *S == ' ' ) { 
			SubS[ k ] = '\0' ;
			while( *S == ' ' ) { k++ ; S++ ; HW.nspace++ ; }
			break ; 
		}
		SubS[ k ] = *S ;	
		k++ ; S++ ;
	}
	if ( strlen( SubS ) ) {
		sprintf( HW.s, "%s", SubS ) ;
		HW.n = strlen( SubS ) ;
	}

	return k ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::GetSpace( char* S, char* T ) 
{
    int    dk = 0, k = 0 ;
    char   SubS[ 256 ] ;
	while( 1 ) {
		if ( *S == '\0' ) { SubS[ k ] = '\0' ; k =-1 ; break ; }
		if ( *S != ' ' ) { SubS[ k ] = '\0' ; break ; }
		SubS[ k ] = ' ' ;	
		k++ ; S++ ;
	}
	if ( strlen( SubS ) ) sprintf( T, "%s", SubS ) ;
	return k ;
}

//----------------------------------------------------------------------------------------------------------
//  Функции преобразования строк в данные
//----------------------------------------------------------------------------------------------------------
int NRR::BuildString( char* s, VRWord& VHW, int i, int j ) 
{
	s[ 0 ] = '\0' ;
	for( int k = i ; k <= j ; k++ ) {
		sprintf( &s[ strlen( s ) ], "%s", VHW[ k ].s ) ;
		if ( k < j && VHW[ k ].nspace ) {
			int m = strlen( s ) ;
			memset( &s[ m ], 32, VHW[ k ].nspace ) ;
			s[ m + VHW[ k ].nspace ] = '\0' ;
		}
	}
	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::BuildFloat( float& F, RWord& W ) 
{
	double D ;
	int rc = BuildDouble( D, W ) ;
	F = ( float )D ;
	return rc ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::BuildDouble( double& D, RWord& W ) 
{
	char*  s ; 
	int    n ; 
	double d ;

	s = &W.s[ 0 ] ;
	n = GetNum( s, d ) ; if ( n < 0 ) return S_FALSE ; 
	D = ( float )d ;
	s+= n ;
	if ( s[ 0 ] == '.' || s[ 0 ] == ',' ) {
		n = SkipChar( s ) ; if ( n < 0 ) return S_FALSE ; 
		s+= n ;
		n = GetNum( s, d ) ; if ( n < 0 ) return S_FALSE ; 
		D += d / pow( 10, n ) ;
	}

	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::BuildShort( short& D, RWord& W ) 
{
	char*  s ; 
	int    n ; 
	double d ;
	
	s = &W.s[ 0 ] ;
	n = GetNum( s, d ) ; if ( n < 0 ) return S_FALSE ; 
	D = ( short )d ;
	
	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::BuildDate(DATE& D, RWord& W)
{
	char*  s = &W.s[ 0 ] ; 
	int    n ; 
	double d ;

	memset( &D, 0, sizeof( D ) ) ;
	
	n = GetNum( s, d ) ; if ( n < 0 ) return S_FALSE ; 
	D.d = ( int )d ;
	s+= n ;
	n = SkipChar( s ) ; if ( n < 0 ) return S_FALSE ; 
	s+= n ;
	n = GetNum( s, d ) ; if ( n < 0 ) return S_FALSE ; 
	D.m = ( int )d ;
	s+= n ;
	n = SkipChar( s ) ; if ( n < 0 ) return S_FALSE ; 
	s+= n ;
	n = GetNum( s, d ) ; if ( n < 0 ) return S_FALSE ; 
	D.g = ( int )d ;

	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::BuildTime( TIME& T, RWord& W ) 
{
	char*  s = &W.s[ 0 ] ; 
	int    n ; 
	double d ;

	memset( &T, 0, sizeof( T ) ) ;
	
	// Часы должны быть обязвтельно
	n = GetNum( s, d ) ; if ( n < 0 ) return S_FALSE ; 
	T.h = ( int )d ;
	s+= n ;
	n = SkipChar( s ) ; if ( n < 0 ) return S_FALSE ; 
	s+= n ;

	// Минуты должны быть обязвтельно
	n = GetNum( s, d ) ; if (n<0) return S_FALSE; 
	T.m = ( int )d ;
	s+= n ;
	n = SkipChar( s ) ; if (n<0) return S_OK; 
	s+= n ;

	// Секунды
	n = GetNum( s, d ) ; if (n<0) return S_OK; 
	T.s = ( int )d ;
	s+= n ;
	n = SkipChar( s ) ; if (n<0) return S_OK; 
	s+= n ;

	// Доли секунд 
	n = GetNum(s, d) ; if (n<0) return S_OK; 
	T.s+= (short)(d/pow(10.0, n));

	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
// ОСНОВНЫЕ ФУНКЦИИ ЧТЕНИЯ РАЗДЕЛОВ RINEX
//----------------------------------------------------------------------------------------------------------
int NRR::ReadName( ) 
{
	char* S = &m_FileName[ 0 ] ;
	int n, i = 0 ;
	while( 1 ) {
		if ( *S == '_' ) break ;
		if ( *S == '\0' ) return S_FALSE ;
		m_Name[ i ] = *S ;
		S++ ; i++ ;
	}
	m_Name[ i ] = '\0' ;
	
	double d ;
	n = GetNum( S, d ) ; if ( n < 0 ) return S_FALSE ;
	S+= n ;
	m_dt.g = ( int )d ;
	n = GetNum( S, d ) ; if ( n < 0 ) return S_FALSE ;
	S+= n ;
	m_dt.m = abs( ( int )d ) ;
	n = GetNum( S, d ) ; if ( n < 0 ) return S_FALSE ;
	S+= n ;
	m_dt.d = abs( ( int )d ) ;
	
	while ( 1 ) {
		if ( *S != '_' ) break ;
		if ( *S == '\0' ) return S_FALSE ;
		S++ ;
	}
	
	i = 0 ;
	while ( 1 ) {
		if ( *S == '_' ) break ;
		if ( *S == '\0' ) return S_FALSE ;
		m_Obj[ i ] = S[ i ] ; 
		S++ ; i++ ;
	}
	m_Obj[ i ] = '\0' ;
	
	n = GetNum( S, d ) ; if ( n < 0 ) return S_FALSE ;
	S+= n ;
	m_Nv = ( int )d ;
	
	n = GetNum( S, d ) ; if ( n < 0 ) return S_FALSE ;
	S+= n ;
	m_Na = ( int )d ;
	
	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::ReadHiader( int fd ) 
{
	m_H.Clear( ) ;
	HeaderSource.clear( ) ;
	
	HeaderSourceItem hi ; 
	char subS[ 512 ] ;
	char endkey[ 14 ] ; 
	
	while( 1 ) {
		if ( ReadLine( fd, subS ) != S_OK ) return S_FALSE ;
		sprintf( hi.s, "%s\0", subS ) ;
		HeaderSource.push_back( hi ) ;

		memcpy( endkey, &subS[ 60 ], 13 ) ;	endkey[ 13 ] = '\0' ;
		if ( !strcmp( endkey, "END OF HEADER" ) ) break ;
		ReadHiaderItem( subS ) ; 
	}
	
	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::ReadHiaderItem( char* S ) 
{
	// 0 - раздел отсутствует, 1 - раздел найден пустым, 2 - раздел найден, 3 - раздел найден, не верный формат
	char*	skey = &S[ 60 ] ; 
	char	sd[ 80 ] ; memcpy( sd, S, 60 ) ; sd[ 60 ] = '\0' ;

	VRWord VHW ;
	LineToWords( sd, VHW, 1 ) ;
	int NW = VHW.size( ) ;
	
	//--------------------------------------------------- 
	if ( !memcmp( skey, "RINEX VERSION / TYPE", 20 ) ) {
		//2.11           OBSERVATION DATA    G 
		m_H.Version = 0 ;
		memset( m_H.TypeFile, 0, sizeof( m_H.TypeFile ) ) ;
		memset( m_H.TypeData, 0, sizeof( m_H.TypeData ) ) ;
		if ( !NW ) { m_H.is_RINEX_VERSION_TYPE = 1 ; return S_FALSE ; }

		if ( NW > 0 ) 
			if ( BuildFloat( m_H.Version, VHW[ 0 ] ) != S_OK ) { m_H.is_RINEX_VERSION_TYPE = 3 ; return S_FALSE ; }

		if ( NW > 1 ) {
			if ( BuildString( m_H.TypeData, VHW, NW - 1, NW - 1 ) != S_OK ) { m_H.is_RINEX_VERSION_TYPE = 3 ; return S_FALSE ; }
			if ( BuildString( m_H.TypeFile, VHW, 1, NW - 2 ) != S_OK ) { m_H.is_RINEX_VERSION_TYPE = 3 ; return S_FALSE ; }
		}

		m_H.is_RINEX_VERSION_TYPE = 2 ; 
		return S_OK ;
	}

	//--------------------------------------------------- 
	if ( !memcmp( skey, "PGM / RUN BY / DATE", 19 ) ) {
		//GRACE_GPS1B         PNBO                 08/08/2014 09:45   
		memset( m_H.ProgName, 0, sizeof( m_H.ProgName ) ) ;
		memset( m_H.ProgVer, 0, sizeof( m_H.ProgVer ) ) ;
		memset( &m_H.BuildDate, 0, sizeof( m_H.BuildDate ) ) ;
		memset( &m_H.BuildTime, 0, sizeof( m_H.BuildTime ) ) ;

		if ( !NW ) { m_H.is_PGM_RUNBYDATE = 1 ; return S_FALSE ; }

		if ( NW > 0 ) if ( BuildString( m_H.ProgName, VHW, 0, 0 ) != S_OK ) { m_H.is_PGM_RUNBYDATE = 3 ; return S_FALSE ; }
		if ( NW > 1 ) if ( BuildString( m_H.ProgVer,  VHW, 1, 1 ) != S_OK ) { m_H.is_PGM_RUNBYDATE = 3 ; return S_FALSE ; }
		if ( NW > 2 ) if ( BuildDate( m_H.BuildDate, VHW[ 2 ] )   != S_OK ) { m_H.is_PGM_RUNBYDATE = 3 ; return S_FALSE ; }
		if ( NW > 3 ) if ( BuildTime( m_H.BuildTime, VHW[ 3 ] )   != S_OK ) { m_H.is_PGM_RUNBYDATE = 3 ; return S_FALSE ; }

		m_H.is_PGM_RUNBYDATE = 2 ;
		return S_OK ;
	}

	//--------------------------------------------------- 
	if ( !memcmp( skey, "COMMENT", 7 ) ) {
		//No comments                                                 COMMENT      
		if ( NW ) {
			char Com[ 256 ] ;
			if ( BuildString( Com, VHW, 0, NW - 1 ) != S_OK ) return S_FALSE ; 
			m_H.Comments.push_back( Com ) ;
			m_H.is_COMMENT = 2 ; 		
		}
		return S_OK ;
	}

	//--------------------------------------------------- 
	if ( !memcmp( skey, "MARKER NAME", 11 ) ) {
		//GRACE-A                                                     MARKER NAME 
		if ( !NW ) { m_H.is_MARKER_NAME = 1 ; return S_FALSE ; }
		if ( BuildString( m_H.Marker, VHW, 0, NW - 1 ) != S_OK ) return S_FALSE ; 
		m_H.is_MARKER_NAME = 2 ;
		return S_OK ;
	}

	//--------------------------------------------------- 
	if ( !memcmp( skey, "OBSERVER / AGENCY", 17 ) ) {
		//GPS1B                                                       OBSERVER / AGENCY   
		if ( !NW ) { m_H.is_OBSERVER_AGENCY = 1 ; return S_FALSE ; }
		if ( BuildString( m_H.ObservAgency, VHW, 0, NW - 1 ) != S_OK ) return S_FALSE ; 
		m_H.is_OBSERVER_AGENCY = 2 ;
		return S_OK ;
	}

	if ( !memcmp( skey, "APPROX POSITION XYZ", 19 ) ) {
		if ( !NW ) { m_H.is_APPROX_POSITION = 1 ; return S_FALSE ; }		
		if ( NW < 3 ) { m_H.is_APPROX_POSITION = 3 ; return S_FALSE ; }		
		if ( BuildDouble( m_H.apX, VHW[ 0 ] ) != S_OK ) { m_H.is_APPROX_POSITION = 3 ; return S_FALSE ; }
		if ( BuildDouble( m_H.apY, VHW[ 1 ] ) != S_OK ) { m_H.is_APPROX_POSITION = 3 ; return S_FALSE ; }
		if ( BuildDouble( m_H.apZ, VHW[ 2 ] ) != S_OK ) { m_H.is_APPROX_POSITION = 3 ; return S_FALSE ; }
		m_H.is_APPROX_POSITION = 2 ;
		return S_OK ;
	}

	if ( !memcmp( skey, "ANTENNA: DELTA H/E/N", 20 ) ) {
		if ( !NW ) { m_H.is_ANTENNA_DELTA = 1 ; return S_FALSE ; }		
		if ( NW < 3 ) { m_H.is_ANTENNA_DELTA = 3 ; return S_FALSE ; }		
		if ( BuildDouble( m_H.aH, VHW[ 0 ] ) != S_OK ) { m_H.is_ANTENNA_DELTA = 3 ; return S_FALSE ; }
		if ( BuildDouble( m_H.aE, VHW[ 1 ] ) != S_OK ) { m_H.is_ANTENNA_DELTA = 3 ; return S_FALSE ; }
		if ( BuildDouble( m_H.aN, VHW[ 2 ] ) != S_OK ) { m_H.is_ANTENNA_DELTA = 3 ; return S_FALSE ; }
		m_H.is_ANTENNA_DELTA = 2 ;
		return S_OK ;
	}

	if ( !memcmp( skey, "WAVELENGTH FACT L1/2", 20 ) ) {
		if ( !NW ) { m_H.is_WAVELENGTH_FACT = 1 ; return S_FALSE ; }		
		if ( BuildShort( m_H.WL1,   VHW[ 0 ] ) != S_OK ) { m_H.is_WAVELENGTH_FACT = 3 ; return S_FALSE ; }
		if ( NW > 1 ) if ( BuildShort( m_H.WL2, VHW[ 1 ] ) != S_OK ) { m_H.is_WAVELENGTH_FACT = 3 ; return S_FALSE ; }
		m_H.is_WAVELENGTH_FACT = 2 ;
		return S_OK ;
	}

	if ( !memcmp( skey, "# / TYPES OF OBSERV", 19 ) ) {
		if ( !NW ) { m_H.is_TYPES_OF_OBSERV = 1 ; return S_FALSE ; }		
		if ( BuildShort( m_H.CountParams,   VHW[ 0 ] ) != S_OK ) { m_H.is_TYPES_OF_OBSERV = 3 ; return S_FALSE ; }
		if ( m_H.CountParams != NW + 1 ) { m_H.is_TYPES_OF_OBSERV = 3 ; return S_FALSE ; }
		char sName[ 256 ] ;
		m_H.NameParam.clear( ) ;
		for( int i = 0 ; i < m_H.CountParams ; i++ ) {			
			if ( BuildString( sName, VHW, i + 1, i + 1 ) != S_OK ) { m_H.is_TYPES_OF_OBSERV = 3 ; return S_FALSE ; }
			m_H.NameParam.push_back( sName ) ;

		}
		m_H.is_TYPES_OF_OBSERV = 2 ;
		return S_OK ;
	}

	if ( !memcmp( skey, "INTERVAL", 8 ) ) {
		if ( !NW ) { m_H.is_INTERVAL = 1 ; return S_FALSE ; }		
		if ( BuildFloat( m_H.dt, VHW[ 0 ] ) != S_OK ) { m_H.is_INTERVAL = 3 ; return S_FALSE ; }
		m_H.is_INTERVAL = 2 ;
		return S_OK ;
	}

	if ( !memcmp( skey, "LEAP SECONDS", 12 ) ) {
		if ( !NW ) { m_H.is_LEAP_SECONDS = 1 ; return S_FALSE ; }		
		if ( BuildFloat( m_H.LeapSec, VHW[ 0 ] ) != S_OK ) { m_H.is_LEAP_SECONDS = 3 ; return S_FALSE ; }
		m_H.is_LEAP_SECONDS = 2 ;
		return S_OK ;
	}

	if ( !memcmp( skey, "TIME OF FIRST OBS", 17 ) ) {
		if ( !NW ) { m_H.isTIME_OF_FIRST_OBS = 1 ; return S_FALSE ; }		
		short  n ;
		double d ;
		if ( BuildShort( n, VHW[ 0 ] ) != S_OK ) { m_H.isTIME_OF_FIRST_OBS = 3 ; return S_FALSE ; }
		m_H.FirstDate.g = n ;
		if ( BuildShort( n, VHW[ 1 ] ) != S_OK ) { m_H.isTIME_OF_FIRST_OBS = 3 ; return S_FALSE ; }
		m_H.FirstDate.m = n ;
		if ( BuildShort( n, VHW[ 2 ] ) != S_OK ) { m_H.isTIME_OF_FIRST_OBS = 3 ; return S_FALSE ; }
		m_H.FirstDate.d = n ;
		if ( BuildShort( n, VHW[ 3 ] ) != S_OK ) { m_H.isTIME_OF_FIRST_OBS = 3 ; return S_FALSE ; }
		m_H.FirstTime.h = n ;
		if ( BuildShort( n, VHW[ 4 ] ) != S_OK ) { m_H.isTIME_OF_FIRST_OBS = 3 ; return S_FALSE ; }
		m_H.FirstTime.m = n ;
		if ( BuildDouble( d, VHW[ 5 ] ) != S_OK ) { m_H.isTIME_OF_FIRST_OBS = 3 ; return S_FALSE ; }
		m_H.FirstTime.s = ( int )d ;
		m_H.FirstTime.d = d - m_H.FirstTime.s ;
		if ( BuildString( m_H.TimeScale, VHW, 6, 6 ) != S_OK ) { m_H.isTIME_OF_FIRST_OBS = 3 ; return S_FALSE ; }
		m_H.isTIME_OF_FIRST_OBS = 2 ;
		return S_OK ;
	}

	RHiaderItem HI ;
	sprintf( HI.Name, "%s\0", skey ) ;
	sprintf( HI.Text, "%s\0", sd ) ;
	m_H.Unknown.push_back( HI ) ;
	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
void NRR::LineToWords( char* S, VRWord& VHW, bool isHeader )
{	
	VHW.clear( ) ;

	char	sd[ 80 ], *s ; 
	int		n ;

	if ( isHeader ) {
		s = &sd[ 0 ] ; 
		memcpy( sd, S, 60 ) ; sd[ 60 ] = '\0' ;
	}  else s = S ; 

	RWord  HW ;
	while( 1 ) { 
		n = GetWord( s, HW ) ;
		if ( HW.n ) VHW.push_back( HW ) ;
		if ( n < 0 ) break ;
		s += n ;
	}
}

//----------------------------------------------------------------------------------------------------------
int NRR::StrToVRKA( char* S, VRKA& VKA )
{
	RKA KA ;
	int CountKA ;

	char subs[ 64 ] ;
	int n ; 
	n = SkipNum( S ) ; if ( n < 0 ) return S_FALSE ;
	memcpy( subs, S, n ) ; subs[ n ] = '\0' ; sscanf( subs, "%d", &CountKA ) ;
	S+= n ;

	while( 1 ) {
		n = SkipChar( S ) ; if ( n < 0 ) return S_FALSE ;
		if ( n != 1 ) return S_FALSE ;	
		KA.Key = S[ 0 ] ;
		S+= n ;

		n = SkipNum( S ) ; if ( n < 0 ) return S_FALSE ;
		memcpy( subs, S, n ) ; subs[ n ] = '\0' ; sscanf( subs, "%d", &KA.SysNum ) ;
		S+= n ;
		VKA.push_back( KA ) ;
		if ( S[ 0 ] == '\0' ) break ;
	}
	if ( CountKA != VKA.size( ) ) return S_FALSE ;

	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::ReadTimePoint( int fd ) 
{
	char  subS[ 512 ] ;	
	if ( ReadLine( fd, subS ) != S_OK ) return S_END ;

	VRWord VHW ;
	LineToWords( subS, VHW, 0 ) ;
	int NW = VHW.size( ) ;
	if ( NW < 8 ) return S_FALSE ;

	short  k ;
	double d ;
	DATE   D;
	TIME   T ;
	BuildShort( k, VHW[ 0 ] ) ; D.g = k ;
	BuildShort( k, VHW[ 1 ] ) ; D.m = k ;
	BuildShort( k, VHW[ 2 ] ) ; D.d = k ;
	BuildShort( k, VHW[ 3 ] ) ; T.h = k ;
	BuildShort( k, VHW[ 4 ] ) ; T.m = k ;
	BuildDouble( d, VHW[ 5 ] ) ; T.s = ( int )d ; T.d = d - T.s ;

	double	fx ;
	BuildDouble( d, VHW[ 6 ] ) ; fx = d ;

	VRKA VKA ;
	if ( StrToVRKA( &VHW[ 7 ].s[ 0 ], VKA ) != S_OK ) return S_FALSE ;
	int CountKA = VKA.size( ) ;

	m_C.AddTime( D, T, fx ) ;

	for ( int i = 0 ; i < CountKA ; i++ ) {
		m_C.AddKAtoShade( VKA[ i ], 0 ) ;
		if ( ReadLine( fd, subS ) != S_OK ) return S_FALSE ;	
		LineToWords( subS, VHW, 0 ) ;
		int NW = VHW.size( ) ;
		if ( NW < m_H.CountParams ) return S_FALSE ;

		for ( int j = 0 ; j < m_H.CountParams ; j++ ) {
			if ( BuildDouble( d, VHW[ j ] ) != S_OK ) return S_FALSE ; 
			if ( m_C.AddValue( d, j, VKA[ i ] ) != S_OK ) return S_FALSE ; 
		}
	}
/*
int NN = m_C.P.size( ) ;
static int NNN = NN ;
if ( NN > NNN ) {

{
	NNN = NN ;
	FILE * pf = fopen( "___KA.txt", "w" ) ;
	for( int i = 0 ; i < m_C.GShade.size( ) ; i++  ) {
		//if ( m_C.GShade[ i ] < 0 ) continue ;
		fprintf( pf, "%d\r\n", m_C.GShade[ i ] ) ;
	}
	fclose( pf ) ;

	pf = fopen( "___KA2.txt", "w" ) ;
	for( i = 0 ; i < m_C.GIndex.size( ) ; i++  ) {
		if ( m_C.GIndex[ i ] < 0 ) continue ;
		fprintf( pf, "%d\r\n", m_C.GIndex[ i ] ) ;
	}
	fclose( pf ) ;
}

NN = NN ;
}
int MM = m_C.Time.size( ) ;
int CC = m_C.GetCountKA( ) ;
*/
	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
// Чтение строки текста до \r\n
//----------------------------------------------------------------------------------------------------------
int NRR::ReadLine( int fd, char* S )  
{
	int i, n, N = 85 ; 
	char subS[ 512 ] ;

	S[ 0 ] = '\0' ;

	while( 1 ) {
		n = read( fd, subS,  N ) ; if ( !n ) break ; 
		subS[ n ] = '\0' ;
		i = n - 1 ;
		while( i ) { if ( subS[ i ] == '\n' || subS[ i ] == '\r' ) break ; i-- ; }	
		if ( i ) {
			subS[ i ] = '\0' ;
			if ( i - n + 1 ) lseek( fd, i - n + 1, SEEK_CUR ) ;
		}
		sprintf( &S[ strlen( S ) ], "%s\0", subS ) ;
		if ( i ) break ;
	}

	n = strlen( S ) ;
	if ( !n ) return FALSE ;
	int nSpace = SkipSpace( S ) ;
	if ( n == nSpace ) return FALSE ; 
	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
// Собствеено функция чтения Rinex-файла
//----------------------------------------------------------------------------------------------------------
int NRR::Read( char* FileName ) 
{
	sprintf( m_FileName, "%s\0", FileName ) ;
	if ( ReadName( ) != S_OK ) return S_FALSE ;

	int fd = open( FileName, O_TEXT  ) ; 
	if ( fd < 0 ) return S_FALSE ;

	if ( ReadHiader( fd ) != S_OK ) return S_FALSE ;

	m_C.Clear( ) ;
	m_C.Init( m_H.CountParams ) ;

	int rc ;
	while( 1 ) {
		rc = ReadTimePoint( fd ) ; 
		if ( rc != S_OK ) break ;
	}
	close( fd ) ;

	if ( rc == S_END ) {
		// файл прочитан корректно
	}

	if ( rc == S_FALSE ) {
		// произошла ошибка
		return S_FALSE ;
	}

	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::Write( ) 
{
	sprintf( m_FileName, "%s_\0", m_Name ) ;
	sprintf( &m_FileName[ strlen( m_FileName ) ], "%4d\0",  m_dt.g ) ;
	sprintf( &m_FileName[ strlen( m_FileName ) ], "-%.2d\0", m_dt.m ) ;
	sprintf( &m_FileName[ strlen( m_FileName ) ], "-%.2d\0", m_dt.d ) ;
	sprintf( &m_FileName[ strlen( m_FileName ) ], "_%s\0",  m_Obj ) ;
	sprintf( &m_FileName[ strlen( m_FileName ) ], "_%.2d\0", m_Nv ) ;
	sprintf( &m_FileName[ strlen( m_FileName ) ], "_%d____\0", m_Na ) ;
	sprintf( &m_FileName[ strlen( m_FileName ) ], ".130\0" ) ;

	FILE* pf = fopen( m_FileName, "w" ) ;

	int			i, rc = S_OK ; 
	int			CountParams = m_C.P.size( ) ;
	int			CountTime   = m_C.Time.size( ) ;

	for (i = 0; i < (int)HeaderSource.size(); i++) fprintf(pf, "%s\n", HeaderSource[i].s);

	VINT k ;
	k.resize( CountParams ) ;
	for( i = 0 ; i < CountParams ; i++ ) k[ i ] = 0 ;

	for( i = 0 ; i < CountTime ; i++ ) {
		rc = WriteTimePoint( pf, i, k ) ; if ( rc != S_OK ) break ; 
	}

	fclose( pf ) ; pf = 0 ;
	return rc ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::WriteTimePoint( FILE* pf, int TimeIndex, VINT& k ) 
{
	int   i, j ;
	int   CountColumn = m_C.GetCountColumn( ) ; if ( !CountColumn ) return S_FALSE ;
	int   CountParams = m_C.P.size( ) ; if ( !CountParams ) return S_FALSE ;
	int   CountKA = 0 ;
	RTime RT = m_C.Time[ TimeIndex ] ;
	RKA   KA ;
	char  SKA[ 256 ] ; 
	IRKA  IKA ;
	VIRKA FKA ;

	// Проверка делимости нацело
	if ( ( CountParams / CountColumn ) * CountColumn != CountParams ) return S_FALSE ; ;

	// Просмотр всех векторов в хранилище в указанной временной точке TimeIndex на наличие значение параметра 
	for( i = 0 ; i < CountParams ; i+= CountColumn ) {
		bool needKA = false ;
		for( j = 0 ; j < CountColumn ; j++ ) {
			if ( m_C.P[ i + j ][ k[ i + j ] ].TimeIndex == TimeIndex ) { needKA = true ; break ; }
		}
		if ( needKA ) {
			KA = m_C.GetKA( i / CountColumn ) ; 
			if ( KA.SysNum < 0 ) if ( !CountParams ) return S_FALSE ;
			IKA.KA = KA ;
			IKA.i  = i ; 
			FKA.push_back( IKA ) ;
		}
	}

	sort( FKA.begin( ), FKA.end( ) ) ;

	CountKA = FKA.size( ) ;
	sprintf( SKA, "%d\0", CountKA ) ;
	for( i = 0 ; i < CountKA ; i++ ) sprintf( &SKA[ strlen( SKA ) ], "%c%.2d\0", FKA[ i ].KA.Key, FKA[ i ].KA.SysNum ) ;

	// Формирование строки заголовка временной точки
	fprintf( pf, "%3.2d%3.2d%3.2d%3.2d%3.2d %010.7lf%3d %s\n", ( RT.D.g < 100 ? RT.D.g : ( RT.D.g - 2000 ) ), RT.D.m, RT.D.d, RT.T.h, RT.T.m, RT.T.s + RT.T.d, ( int )RT.fx, SKA ) ;

	// Запись в файл значений всех параметров в указанной временной точке TimeIndex
	for( i = 0 ; i < CountKA ; i++ ) {
		int l = FKA[ i ].i ;
		for( j = 0 ; j < CountColumn ; j++ ) {
			if ( m_C.P[ l + j ][ k[ l + j ] ].TimeIndex == TimeIndex ) { 				
				if ( m_C.IsSplitTimeIndex( l + j, TimeIndex ) ) {
					fprintf( pf, "              " ) ;
				} else {
					if ( j ) fprintf( pf, "  " ) ;
					fprintf( pf, "%14.3lf", m_C.P[ l + j ][ k[ l + j ] ].V ) ;
				}
				k[ l + j ]++ ;
			} else {
				if ( j ) fprintf( pf, "  " ) ;
				fprintf( pf, "              " ) ;
			}
		}
		fprintf( pf, "\n" ) ;
	}

	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
void NRR::PrintContent( char* FileName ) 
{
	m_C.PrintParams( FileName ) ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::Splitting( ) 
{
	int rc = S_OK ;
	int SN = m_C.P.size( ) ;

	for( int i = 0 ; i < SN ; i++ ) {
		VRSplitPoint AVJ ;

		// Поиск разрывов по времени (формирование интервалов)
		int j, N = m_C.P[ i ].size( ) ;
		VINT indR ;
		for( j = 1 ; j < N ; j++ ) {
			if ( ( m_C.P[ i ][ j ].TimeIndex - m_C.P[ i ][ j - 1 ].TimeIndex ) != 1 ) indR.push_back( j - 1 ) ;
		}

		// Поиск скачков для каждого найденного интервала 
		int NR = indR.size( ) ;
		if ( !NR ) rc = SplittingParam( m_C.Time, m_C.P[ i ],-1,-1, &AVJ, NULL, NULL ) ;
		else {
			VRSplitPoint VJ ;
			int  jn, jk ; 
			for( j = 0 ; j < NR + 1 ; j++ ) {
				jn = j ? ( indR[ j - 1 ] + 1 ) :-1 ;
				jk = j < NR ? indR[ j ] :-1 ;
				rc = SplittingParam( m_C.Time, m_C.P[ i ], jn, jk, &VJ, NULL, NULL ) ; if ( rc != S_OK ) break ;
				for (int k = 0; k < (int)VJ.size(); k++) {
					AVJ.push_back( VJ[ k ] ) ;
				}
			}
		}	

		// Проверка корректности поиска скачков
		//if ( rc != S_OK ) continue ;

		// Сохраняем найденые точки 
		m_C.SplitPoint.push_back( AVJ ) ;

		// Удаляем массив индексов точек в которых найдены скачки для массива m_C.P[ i ]
		AVJ.clear( ) ;
	}

	return rc ;
}

//----------------------------------------------------------------------------------------------------------
// Функция разбиения массива значений произвильного параметра по скачкам
//	Time	- массив времени
//	VP		- массив значений параметра
//	jn		- начальный индекс массива (начиная с которого идет поиск скачков)
//	jk		- конечный индекс массива (заканчивая которым включительно идет поиск скачков) 
//	VPS		- мектор массивов на которые разбивается исходный VP
//	CountFindSplit - Число найденных скачков
//----------------------------------------------------------------------------------------------------------
int NRR::SplittingParam( VRTime& Time, VRParam& VP, int jn, int jk, VRSplitPoint* pVJ, VVRParam* pVPS, int* pCountFindSplit ) 
{
	int		N, j ;
	double	T0 ;

	// VPS - вектор векторов 
	if ( pVPS ) {
		N = pVPS->size( ) ;
		for( j = 0 ; j < N ; j++ ) (*pVPS)[ j ].clear( ) ;
		pVPS->clear( ) ;
	}

	if ( pVJ ) pVJ->clear( ) ;

	N = VP.size( ) ; 
	if ( pCountFindSplit ) *pCountFindSplit = 0 ;	
	if ( !N ) return S_OK ;

	if ( jn < 0 ) jn = 0 ;
	if ( jk < 0 ) jk = N - 1 ;
	if ( ( jk - jn + 1 ) < 3 ) return S_OK ;

	// Вычисление первой производной вдоль массива
	double	dPdT, dPdT2 ;	
	double	iT, iT2 ;
	double	iP, iP2 ;
	VDBL	VDPDT ;		
	VRSplitPoint VJ ;	
	RSplitPoint  SP ;
	for ( j = jn ; j <= jk ; j++ ) {
		// Индекс в массиве времени 
		int jTime = VP[ j ].TimeIndex ;
		// Врямя j-ой точки 
		iT = Time[ j ].t ;
		// Значение параметра в j-ой точке 
		iP = VP[ j ].V ;
		// Сохраняем время первой точки чтоб в дальнейшем оперировать относительным временем
		if ( !( j - jn ) ) T0 = iT ;
		// Используем относительное время 
		iT-= T0 ;
		// собственно расчет производной
		if ( ( j - jn ) > 0 ) {
			dPdT = ( iP - iP2 ) / ( iT - iT2 ) ;
			VDPDT.push_back( dPdT ) ;
		} else dPdT = 0 ;
		iT2 = iT ;
		iP2 = iP ;
		dPdT2 = dPdT ;
	}

	// Поиск скачка
	double maxDP ;
	double minDP ;
	double sDP = 0 ;
	double saDP = 0 ;
	double sDPsko = 0 ;
	while( 1 ) {
		int SizeVJ = VJ.size( ) ;
		// анализ производной параметра по времени
		// Фактически перерасчет среднего значения производной и ее СКО за вычетом уже 
		// найденных точек разрыва 
		CalcSKO( VJ, VDPDT, maxDP, minDP, sDP, sDPsko ) ;
		// Определяем скачки и их колличество
		VJ.clear( ) ;
		for ( j = 0 ; j < jk - jn ; j++ ) {
			// Значение производной в точке
			double val = VDPDT[ j ] ;
			// Факт скачка определяем по токлонению производной в рассматриваемой точке
			// от производной в точке предыдущей 
			if ( j > 0 ) {
				if ( fabs( val - VDPDT[ j - 1 ] ) > 3.0 * sDPsko ) {
					// Найден скачек значения параметра
					SP.ValueIndex = j + jn ;
					SP.TimeIndex  = VP[ j + jn ].TimeIndex ;
					VJ.push_back( SP ) ; j++ ;
				}
			} else {
				if ( fabs( val - VDPDT[ j + 1 ] ) > 3.0 * sDPsko ) {
					// Найден скачек значения параметра
					SP.ValueIndex = j + jn ;
					SP.TimeIndex  = VP[ j + jn ].TimeIndex ;
					VJ.push_back( SP  ) ; j++ ;
				}
			}
		}
		// Если не найдено новых скачков то завершаем цикл поиска
		if ( SizeVJ == VJ.size( ) ) break ;
		// Более достоверным алгоритмом может быть алгоритм сравнения производной в точке 
		// со значением экспоненциальной скользящей средней Параметрами этой скользящей
		// являются интервал осреднения и показатель экспоненты
	}

	if ( VJ.size( ) && pCountFindSplit ) *pCountFindSplit = VJ.size( ) ;

	// Точки разрыва не найдены
	if ( !VJ.size( ) ) return S_OK ;

	// Заполняем результитующий вектор массивов на которые делится исходный массив
	if ( pVPS ) {
		for (int k = 0; k < (int)VJ.size() + 1; k++) {
			VRParam OI ;
			OI = VP ;	
			if ( k ) {
				VRParam::iterator itN ;
				VRParam::iterator itK ;
				int NK ; 
				int NN ;
				if ( k == VJ.size( ) ) {
					NN = VJ[ k - 1 ].ValueIndex ;
					itN = OI.begin( ) ; 
					itK = OI.begin( ) ; itK+= NN + 1 ;
					OI.erase( itN, itK ) ;
				} else {
					NN = VJ[ k - 1 ].ValueIndex ;
					NK = VJ[ k ].ValueIndex ; 
					itN = OI.begin( ) ; itN+= ( NK + 1 ) ;
					itK	= OI.end( ) ; 
					OI.erase( itN, itK ) ;					
					itN = OI.begin( ) ; 
					itK = OI.begin( ) ; itK+= NN + 1 ;
					OI.erase( itN, itK ) ;
				}
			} else OI.resize( VJ[ 0 ].ValueIndex + 1 ) ;			
			(*pVPS).push_back( OI ) ;
		}
	}

	// Заполняем вектор индексов точек в которых наблюдаются скачки
	if ( pVJ ) *pVJ = VJ ;

	// Удаляем промежуточные данные (значения производных и индексы точек разрыва)
	VDPDT.clear( ) ;
	VJ.clear( ) ;

	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
int NRR::CalcSKO( VRSplitPoint& VJ, VDBL& VDPDT, double& maxDP, double& minDP, double& sDP, double& sDPsko ) 
{
	int j, k, NI = VDPDT.size( ) ;
	k = 0 ;
	sort( VJ.begin( ), VJ.end( ) ) ;
	for ( j = 0 ; j < NI ; j++ ) {
		// VJ - это номера точек разрыва Для анализа производной точки разрыва пропускаем
		if ( k < (int)VJ.size( ) && j == VJ[ k ].ValueIndex ) { k++ ; continue ; }
		// Вычисление максимальной и мнимальной производной на интервале 
		if ( j ) {
			if ( maxDP < VDPDT[ j ] ) maxDP = VDPDT[ j ] ;
			if ( minDP > VDPDT[ j ] ) minDP = VDPDT[ j ] ;			
		} else maxDP = minDP = VDPDT[ j ] ;		
		// Вычисляем сумму производных для получения в дальнейшем средней производной на интервале 
		sDP += VDPDT[ j ] ;
	}
	// Средняя производная на интервале 
	sDP /= ( NI - k ) ;
	// Вычисление среднего квадратичного отклонения производной от своего среднего значения
	sDPsko = 0 ;
	for ( j = 0 ; j < NI ; j++ ) sDPsko = pow( VDPDT[ j ] - sDP, 2 ) ;
	sDPsko = sqrt( sDPsko / NI ) ;
	
	return 0 ; 
}

//----------------------------------------------------------------------------------------------------------