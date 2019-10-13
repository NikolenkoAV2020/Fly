//--------------------------------------------------------------------------------
// EdfStr.cpp :
// А. Николенко 03.08.2018
//--------------------------------------------------------------------------------
#include <stdafx.h>
#include <FlyFormat.h>
#pragma hdrstop

//------------------------------------------------------------------------------
bool CFormatString::isSysSetLoad = 0 ;
char CFormatString::SysSepDec  = 0 ;
char CFormatString::SysSepDate = 0 ;
char CFormatString::SysSepTime = 0 ;
char CFormatString::SysSepList = 0 ;

//------------------------------------------------------------------------------
void FPTOFPS ( FormatProperty& p, FormatPropertySave& ps )
{
    ps.Value = p.Value ;
    ps.Dec  = p.Dec ;
    ps.Len = p.Len ;
    ps.Spr1 = p.Spr1 ;
    ps.Spr2 = p.Spr2 ;
    ps.Format = p.Format ;
    ps.FormatIO = p.FormatIO ;
    ps.FormatMM = p.FormatMM ;
    ps.Calendar = p.Calendar ;
    ps.FixedFields = p.FixedFields ;
    ps.FixedFirstField = p.FixedFirstField ;
    ps.FixedLastField = p.FixedLastField ;
    ps.SpaceLeftField = p.SpaceLeftField ;
    ps.Space = p.Space ;
    ps.Sign = p.Sign ;
    ps.NotShowSign = p.NotShowSign ;
    ps.Interval = p.Interval ;
    ps.Max = p.Max ;
    ps.Min = p.Min ;
    ps.ArrowKeys = p.ArrowKeys ;
    ps.TestValiVar = p.TestValiVar ;
}

//------------------------------------------------------------------------------
void FPSTOFP ( FormatPropertySave& ps, FormatProperty& p )
{
    p.Value = ps.Value ;
    p.Dec  = ps.Dec ;
    p.Len = ps.Len ;
    p.Spr1 = ps.Spr1 ;
    p.Spr2 = ps.Spr2 ;
    p.Format = ps.Format ;
    p.FormatIO = ps.FormatIO ;
    p.FormatMM = ps.FormatMM ;
    p.Calendar = ps.Calendar ;
    p.FixedFields = ps.FixedFields ;
    p.FixedFirstField = ps.FixedFirstField ;
    p.FixedLastField = ps.FixedLastField ;
    p.SpaceLeftField = ps.SpaceLeftField ;
    p.Space = ps.Space ;
    p.Sign = ps.Sign ;
    p.NotShowSign = ps.NotShowSign ;
    p.Interval = ps.Interval ;
    p.Max = ps.Max ;
    p.Min = ps.Min ;
    p.ArrowKeys = ps.ArrowKeys ;
    p.TestValiVar = ps.TestValiVar ;
}

//------------------------------------------------------------------------------
CFormatString::CFormatString( )
{
	m_BindFlag       = 0 ;
    m_ValiSetData    = true ;
    m_bisNeedSetText = false ;

    m_isAutoChangeApply = 1 ;

    m_NewField =-1 ;
    m_OldField =-1 ;

    m_Value    = 0 ;
	m_Dec      = 6 ;
	m_Len      = 6 ;
	m_Spr1     = ',' ;
	m_Spr2     = ':' ;
	m_Format   = EDF_NOTFORMAT ;
	m_FormatIO = EDF_NOTFORMAT ;
	m_FormatMM = EDF_NOTFORMAT ;
    m_Calendar = 0 ;
	m_FixedFields     = 0 ;
    m_FixedFirstField = EDF_PROPERTY_NOTSET ;
	m_FixedLastField  = EDF_PROPERTY_NOTSET ;
	m_SpaceLeftField  = 0 ;
	m_Space       = 0 ;
	m_Sign        = 1 ;
	m_NotShowSign = 1 ;
	m_Interval    = 0 ;
    m_Max         =-1 ;
    m_Min         = 0 ;
    //
    m_ArrowKeys   = 0 ;
    m_TestValiVar = 0 ;
    //
    m_UndoBufSize = 10 ;
    //
    m_AltString = 0 ;
    m_AltCount  = 0 ;
    //
	memset ( m_Text, 0, 256 ) ;
	m_Pos  = 0 ;
    m_s    = 0 ;
	m_np   = 0 ;
	memset( &m_ip[ 0 ][ 0 ], 0, 7 * sizeof( int ) ) ;
	memset( &m_ip[ 1 ][ 0 ], 0, 7 * sizeof( int ) ) ;
	memset( &m_lp, 0, 7 * sizeof( int ) ) ;
	memset( &m_f, 0, 7 * sizeof( double ) ) ;
	m_fp[ 0 ] = m_fp[ 1 ] = m_fp[ 2 ] = m_fp[ 3 ] = m_fp[ 4 ] = m_fp[ 5 ] = m_fp[ 6 ] = false ;
    //
    m_isRO = 0 ;
    //
    if ( !isSysSetLoad ) {
      SysSepDec = ',' ;
      SysSepDate= '.' ;
      SysSepTime= ':' ;
      SysSepList= ';' ;
      //char c ;
      //if ( !ReadSysDecSeparator ( c ) ) SysSepDec  = c ;
      //if ( !ReadSysDateSeparator( c ) ) SysSepDate = c ;
      //if ( !ReadSysTimeSeparator( c ) ) SysSepTime = c ;
      //if ( !ReadSysListSeparator( c ) ) SysSepList = c ;
      isSysSetLoad = 1 ;
    }
	//
	m_CheckingCode = 0 ;
	m_BuildCode    = 0 ;
}

//------------------------------------------------------------------------------
CFormatString::CFormatString(const CFormatString& F)
{
	*this = F;
}

//------------------------------------------------------------------------------
CFormatString::~CFormatString( )
{   DelAltString( ) ; }

//------------------------------------------------------------------------------
void CFormatString::Clone( const CFormatString& F )
{
	m_Pos = F.m_Pos ;
    m_s   = F.m_s ;
	m_np  = F.m_np ;
    //
    memcpy( m_ip, F.m_ip, 14* sizeof( int ) ) ;
    memcpy( m_lp, F.m_lp, 7 * sizeof( int ) ) ;
    memcpy( m_fp, F.m_fp, 7 * sizeof( bool ) ) ;
    memcpy( m_f,  F.m_f,  7 * sizeof( double ) ) ;
    //
    m_bisNeedSetText = F.m_bisNeedSetText ;
    //
    SetAltString( F.m_AltString, F.m_AltCount ) ;
    //
	m_Value = F.m_Value ;
    //
	m_Dec      = F.m_Dec ;
	m_Len      = F.m_Len ;
	m_Spr1     = F.m_Spr1 ;
	m_Spr2     = F.m_Spr2 ;
	m_Format   = F.m_Format ;
	m_FormatIO = F.m_FormatIO ;
	m_FormatMM = F.m_FormatMM ;
    m_Calendar = F.m_Calendar ;
	m_FixedFields     = F.m_FixedFields ;
	m_FixedFirstField = F.m_FixedFirstField ;
	m_FixedLastField  = F.m_FixedLastField ;
	m_SpaceLeftField  = F.m_SpaceLeftField ;
	m_Space       = F.m_Space ;
	m_Sign        = F.m_Sign ;
	m_NotShowSign = F.m_NotShowSign ;
	m_Interval    = F.m_Interval ;
    m_Max         = F.m_Max ;
    m_Min         = F.m_Min ;
    //
    m_BindFlag    = F.m_BindFlag ;
    m_isAutoChangeApply = F.m_isAutoChangeApply ;
    //
    m_ArrowKeys   = F.m_ArrowKeys ;
    m_TestValiVar = F.m_TestValiVar ;
	m_ValiSetData = F.m_ValiSetData ;
}
//------------------------------------------------------------------------------
void CFormatString::DelAltString( )
{
    if ( m_AltString ) {
      for ( int i = 0 ; i < m_AltCount ; i++ ) {
        if ( m_AltString[ i ] ) delete [] m_AltString[ i ] ; m_AltString[ i ] = 0 ;
      }
      m_AltCount = 0 ;
      delete [] m_AltString ; m_AltString = 0 ;
    }
}
//------------------------------------------------------------------------------
void CFormatString::SetAltString( char** AltString, int AltCount, int AltCountDrop )
{
    DelAltString( ) ;
    m_AltCount  = AltCount ;
    if ( !AltString || !AltCount ) return ;
    m_AltCountDrop = AltCountDrop ;
    m_AltString = new char*[ m_AltCount ] ;
    for ( int  i = 0 ; i < m_AltCount ; i++ ) {
      int  l = strlen( AltString[ i ] ) ;
      m_AltString[ i ] = new char[ l + 2 ] ;
      memcpy( m_AltString[ i ], AltString[ i ], l ) ;
      m_AltString[ i ][ l ] = '\0' ;
      l = l ;
    }
}
//--------------------------------------------------------------------------------
void CFormatString::SetAltString( vector<string>& AltString, int AltCountDrop )
{
    DelAltString( ) ;
	int AltCount = AltString.size( ) ; if ( !AltCount ) return ;
    m_AltCount  = AltCount ;
    m_AltCountDrop = AltCountDrop ;
    m_AltString = new char*[ m_AltCount ] ;
    for ( int  i = 0 ; i < m_AltCount ; i++ ) {
      int  l = AltString[ i ].length( ) ;
      m_AltString[ i ] = new char[ l + 2 ] ;
      memcpy( m_AltString[ i ], AltString[ i ].c_str( ), l ) ;
      m_AltString[ i ][ l ] = '\0' ;
      l = l ;
    }
}
//--------------------------------------------------------------------------------
int CFormatString::GetAltCount( ) 
{
	if ( m_Format >= EDF_YESNO && m_Format <= EDF_CHECKYN ) return 2 ;
	if ( m_Format == EDF_CHECKCOLOR ) return 3 ;
	if ( m_Format == EDF_ALT ) return m_AltCount ;
	return 0 ;
}
//------------------------------------------------------------------------------
void CFormatString:: SetAltCount( int AltCount ) 
{	m_AltCount = AltCount ; }
//------------------------------------------------------------------------------
int CFormatString::GetAltCountDrop( ) 
{	return m_AltCountDrop ; }
//------------------------------------------------------------------------------
void CFormatString::SetAltCountDrop( int AltCountDrop ) 
{	m_AltCountDrop = AltCountDrop ; }
//------------------------------------------------------------------------------
char* CFormatString::GetAltText ( int index ) 
{
	if ( m_Format < EDF_YESNO && m_Format > EDF_ALT ) return 0 ;
	if ( index >= GetAltCount( ) || index < 0 ) return 0 ;
	if ( m_Format == EDF_01 ) return( index ? "1" : "0" ) ;
	if ( m_Format == EDF_ALT ) return( ( m_AltString ? m_AltString[ index ] : 0 ) ) ;
	if ( m_Format == EDF_YESNO ) return( index ? "Да" : "Нет" ) ;
	if ( m_Format == EDF_INSERT ) return( index ? "Вкл" : "Выкл" ) ;
	return 0 ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedType( int TypeFormat, double Value, int FormatIO )
{
    switch ( TypeFormat ) {
    case EDF_TFNOTFORMAT : break ;
    case EDF_TFDATA :     FormatedDate    ( Value, FormatIO ) ; break ;
    case EDF_TFDATATIME : FormatedDateTime( Value, FormatIO ) ; break ;
    case EDF_TFTIME :     FormatedTime    ( Value, FormatIO ) ; break ;
    case EDF_TFANGL :     FormatedGrad    ( Value, FormatIO ) ; break ;
    case EDF_TFVITARG :   FormatedVitGrad ( Value, FormatIO ) ; break ;
    case EDF_TFNUMBER :   FormatedNumber  ( Value, FormatIO ) ; break ;
    case EDF_TFINT :      FormatedInt     ( Value, FormatIO ) ; break ;
    case EDF_TFBOOL :     FormatedBool    ( Value ) ; break ;
    case EDF_TFKEYNU :    FormatedKeyNu   ( Value, 1 ) ; break ;
    default :             FormatedNumber  ( Value, FormatIO ) ;
    }
}
//------------------------------------------------------------------------------
void CFormatString::Formated( int Format, double Value, int FormatIO )
{
    switch ( Format ) {
    case EDF_NOTFORMAT :  break ;
    case EDF_DMY :       FormatedDate       ( Value, FormatIO ) ; break ;
    case EDF_DMYHMS :    FormatedDateTime   ( Value, FormatIO ) ; break ;
    case EDF_HMS :       FormatedTimeHMS    ( Value, FormatIO ) ; break ;
    case EDF_SSSHMS :    FormatedTimeSSSHMS ( Value, FormatIO ) ; break ;
    case EDF_MILLIS :    FormatedTimeMiliSec( Value, FormatIO ) ; break ;
    case EDF_SEC :       FormatedTimeSec    ( Value, FormatIO ) ; break ;
    case EDF_MIN :       FormatedTimeMin    ( Value, FormatIO ) ; break ;
    case EDF_HOUR :      FormatedTimeHour   ( Value, FormatIO ) ; break ;
    case EDF_SSS :       FormatedTimeSSS    ( Value, FormatIO ) ; break ;
    case EDF_GMS :       FormatedGMS        ( Value, FormatIO ) ; break ;
    case EDF_GRAD :      FormatedGrad       ( Value, FormatIO ) ; break ;
    case EDF_RAD :       FormatedRad        ( Value, FormatIO ) ; break ;
    case EDF_VITGMS :    FormatedVitGMS     ( Value, FormatIO ) ; break ;
    case EDF_VITGRAD :   FormatedVitGrad    ( Value, FormatIO ) ; break ;
    case EDF_VITRAD :    FormatedVitRad     ( Value, FormatIO ) ; break ;
    case EDF_FLOAT :     FormatedFloat      ( Value, FormatIO ) ; break ;
    case EDF_FIXED :     FormatedFixed      ( Value, FormatIO ) ; break ;
    case EDF_INTEGER :   FormatedInt        ( Value, FormatIO ) ; break ;
    case EDF_YESNO :     FormatedBoolYN     ( Value ) ; break ;
    case EDF_INSERT :    FormatedBoolIns    ( Value ) ; break ;
    case EDF_01 :        FormatedBool01     ( Value ) ; break ;
    case EDF_KEYNU :     FormatedKeyNu      ( Value, 0 ) ; break ;
    case EDF_KEYNUFULL : FormatedKeyNu      ( Value, 1 ) ; break ;
    default : return ;
    }
}
//------------------------------------------------------------------------------
void CFormatString::PostFormated( FormatProperty& p, double Value, int FormatIO )
{
    p.Value    = Value ;
    p.FormatIO = FormatIO ;
    SetFormatProperty( p ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedNumber( double Value, int FormatIO )
{
    FormatedFixed( Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedFixed( double Value, int FormatIO )
{
    FormatProperty p ; FormatedFixed( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedFloat( double Value, int FormatIO )
{
    FormatProperty p ; FormatedFloat( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedInt  ( double Value, int FormatIO )
{
    FormatProperty p ; FormatedInt( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedBool( double Value )
{
    FormatProperty p ; FormatedBool( p ) ;
    PostFormated( p, Value, EDF_NOTFORMAT ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedBoolYN( double Value )
{
    FormatProperty p ; FormatedBoolYN( p ) ;
    PostFormated( p, Value, EDF_NOTFORMAT ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedBoolIns( double Value )
{
    FormatProperty p ; FormatedBoolIns( p ) ;
    PostFormated( p, Value, EDF_NOTFORMAT ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedBool01( double Value )
{
    FormatProperty p ; FormatedBool01( p ) ;
    PostFormated( p, Value, EDF_NOTFORMAT ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedDate ( double Value, int FormatIO )
{
    FormatProperty p ; FormatedDate( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedDateTime ( double Value, int FormatIO )
{
    FormatProperty p ; FormatedDateTime( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedGrad ( double Value, int FormatIO )
{
    FormatProperty p ; FormatedGrad( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedRad ( double Value, int FormatIO )
{
    FormatProperty p ; FormatedRad( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedGMS  ( double Value, int FormatIO )
{
    FormatProperty p ; FormatedGMS( p ) ; 
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedVitGrad ( double Value, int FormatIO )
{
    FormatProperty p ; FormatedVitGrad( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedVitRad ( double Value, int FormatIO )
{
    FormatProperty p ; FormatedVitRad( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedVitGMS( double Value, int FormatIO )
{
    FormatProperty p ; FormatedVitGMS( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTime ( double Value, int FormatIO )
{
    FormatedTimeHMS( Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeHMS( double Value, int FormatIO )
{
    FormatProperty p ; FormatedTimeHMS( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeSSSHMS( double Value, int FormatIO )
{
    FormatProperty p ; FormatedTimeSSSHMS( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeMiliSec ( double Value, int FormatIO )
{
    FormatProperty p ; FormatedTimeMiliSec( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeSec ( double Value, int FormatIO )
{
    FormatProperty p ; FormatedTimeSec( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeMin( double Value, int FormatIO )
{
    FormatProperty p ; FormatedTimeMin( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeHour( double Value, int FormatIO )
{
    FormatProperty p ; FormatedTimeHour( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeSSS( double Value, int FormatIO )
{
    FormatProperty p ; FormatedTimeSSS( p ) ;
    PostFormated( p, Value, FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedAlt( char** AltString, int AltCount, double Value )
{
    m_Value  = Value ;
    m_Format = EDF_ALT ;
    SetAltString( AltString, AltCount ) ;
    if ( m_isAutoChangeApply ) BuildFromData( ) ;
}
//--------------------------------------------------------------------------------
void CFormatString::FormatedAlt( vector<string>& AltString, double Value )
{
    m_Value  = Value ;
    m_Format = EDF_ALT ;
    SetAltString( AltString ) ;
    if ( m_isAutoChangeApply ) BuildFromData( ) ;
}
//--------------------------------------------------------------------------------
void CFormatString::FormatedCustomKey( int* Len, int KeyCount, int* Key )
{
    FormatProperty p ; FormatedCustomKey( p ) ;
    BuildTampleteCustomKey( Len, KeyCount ) ;
    if ( Key ) SetFieldsValue( Key ) ;
    SetFormatProperty( p ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedKeyNu( double Value, bool isFull )
{
    FormatProperty p ; FormatedKeyNu( p, isFull ) ;
    PostFormated( p, Value, p.FormatIO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::SetFormatProperty ( FormatProperty& inf )
{
    m_Value = inf.Value ;
	m_Dec   = inf.Dec;
	m_Len   = inf.Len ;
	m_Spr1  = inf.Spr1 ;
	m_Spr2  = inf.Spr2 ;
	m_Format   = inf.Format ;
	m_FormatIO = inf.FormatIO ;
	m_FormatMM = inf.FormatMM ;
    m_Calendar = inf.Calendar ;
	m_FixedFields     = inf.FixedFields ;
    m_FixedFirstField = inf.FixedFirstField ;
	m_FixedLastField  = inf.FixedLastField ;
	m_SpaceLeftField  = inf.SpaceLeftField ;
	m_Space       = inf.Space ;
	m_Sign        = inf.Sign ;
	m_NotShowSign = inf.NotShowSign ;
	m_Interval    = inf.Interval ;
    m_Max         = inf.Max ;
    m_Min         = inf.Min ;
    //
    if ( m_Format == EDF_SSSHMS ) {
      m_Interval = 1 ;
      m_Len = m_Len < 1 ? 1 : m_Len ;
    }
    //
    m_ArrowKeys   = inf.ArrowKeys ;
    m_TestValiVar = inf.TestValiVar ;
    //
    if ( m_FormatIO == 0 ) SetFormatIODefault( ) ;
    if ( m_isAutoChangeApply ) BuildFromData( ) ;
    //
    //SetEditDim ( ( char* )inf.TextDim. c_str( ) ) ;
    //SetEditName( ( char* )inf.TextName.c_str( ) ) ;
    //SetEditHint( ( char* )inf.TextHint.c_str( ) ) ;
}
//------------------------------------------------------------------------------
void CFormatString::GetFormatProperty ( FormatProperty& inf )
{
    GetEditText( ) ;
    //
    char *s = 0 ;
    //inf.TextName = ( s = GetEditName( ) ? s : "" ) ;
    //inf.TextDim  = ( s = GetEditDim ( ) ? s : "" ) ;
    //inf.TextHint = ( s = GetEditHint( ) ? s : "" ) ;
    //
    int rc, inMaxMin ;
    rc = BuildValue( inMaxMin ) ;
    if ( !rc || inMaxMin ) m_Value = 0 ;
    else inf.Value = m_Value ;
    //
	inf.Dec            = m_Dec ;
	inf.Len            = m_Len ;
	inf.Spr1           = m_Spr1 ;
	inf.Spr2           = m_Spr2 ;
	inf.Format         = m_Format ;
	inf.FormatIO       = m_FormatIO ;
	inf.FormatMM       = m_FormatMM ;
    inf.Calendar       = m_Calendar ;
	inf.FixedFields    = m_FixedFields ;
    inf.FixedFirstField= m_FixedFirstField ;
	inf.FixedLastField = m_FixedLastField ;
	inf.SpaceLeftField = m_SpaceLeftField ;
	inf.Space          = m_Space ;
	inf.Sign           = m_Sign ;
	inf.NotShowSign    = m_NotShowSign ;
	inf.Interval       = m_Interval ;
    inf.Max            = m_Max ;
    inf.Min            = m_Min ;
    //
    inf.ArrowKeys    = m_ArrowKeys ;
    inf.TestValiVar  = m_TestValiVar ;
}
//------------------------------------------------------------------------------
bool CFormatString::SetDec( int  Dec )
{   m_Dec = Dec ; FormatChangeApply( 0 ) ; return 0 ; }
//------------------------------------------------------------------------------
int CFormatString::GetDec( )
{   return( m_Dec ) ; }
//------------------------------------------------------------------------------
bool CFormatString::SetLen( int  Len )
{   m_Len = Len ; FormatChangeApply( 0 ) ; return 0 ; }
//------------------------------------------------------------------------------
int  CFormatString::GetLen( )
{   return m_Len ; }
//------------------------------------------------------------------------------
bool CFormatString::SetKeyNULenKa( int  Len ) 
{   
    if ( m_Format != EDF_KEYNUFULL ) {
	  throw( "Ошибочная попытка задания свойств формата ключей НУ" ) ;
	}
	m_Len = Len ; FormatChangeApply( 0 ) ; return 0 ; 
}
//------------------------------------------------------------------------------
int  CFormatString::GetKeyNULenKa( ) 
{
    if ( m_Format != EDF_KEYNUFULL ) throw( "Ошибочная попытка запроса свойств формата ключей НУ" ) ;
	return m_Len ; 
}
//------------------------------------------------------------------------------
bool CFormatString::SetKeyNULenNom( int  Len ) 
{
    if ( m_Format != EDF_KEYNUFULL && m_Format != EDF_KEYNU ) {
	  throw( "Ошибочная попытка задания свойств формата ключей НУ" ) ;
	}
	m_Dec = Len ; FormatChangeApply( 0 ) ; return 0 ; 
}
//------------------------------------------------------------------------------
int  CFormatString::GetKeyNULenNom( ) 
{   
    if ( m_Format != EDF_KEYNUFULL && m_Format != EDF_KEYNU ) throw( "Ошибочная попытка запроса свойств формата ключей НУ" ) ;
	return( m_Dec ) ; 
}
//------------------------------------------------------------------------------
void CFormatString::SetSign( bool Sign )
{   m_Sign = Sign ; FormatChangeApply( 0 ) ; }
//------------------------------------------------------------------------------
bool CFormatString::GetSign( )
{   return( m_Sign ) ; }
//------------------------------------------------------------------------------
void CFormatString::SetNotShowSign( bool NotShowSign )
{   m_NotShowSign = NotShowSign ; FormatChangeApply( 0 ) ; }
//------------------------------------------------------------------------------
bool CFormatString::GetNotShowSign( )
{   return( m_NotShowSign ) ; }
//------------------------------------------------------------------------------
void CFormatString::SetInterval( bool Interval )
{   m_Interval = Interval ; FormatChangeApply( 0 ) ; }
//------------------------------------------------------------------------------
bool CFormatString::GetInterval( )
{   return( m_Interval ) ; }
//------------------------------------------------------------------------------
void CFormatString::SetFixedFields( bool FixedFields )
{   m_FixedFields = FixedFields ; FormatChangeApply( 0 ) ; }
//------------------------------------------------------------------------------
bool CFormatString::GetFixedFields( )
{   return( m_FixedFields ) ; }
//------------------------------------------------------------------------------
void CFormatString::SetFixedFirstField( bool FixedFirstField )
{
    if ( GetFirstField( m_Format ) != 0 ) return ;
    m_FixedFirstField = FixedFirstField ; FormatChangeApply( 0 ) ;
}
//------------------------------------------------------------------------------
bool CFormatString::GetFixedFirstField( )
{   return( ( m_FixedFirstField == EDF_PROPERTY_NOTSET ? false : ( m_FixedFirstField == 0 ? false : true ) ) ) ; }
//------------------------------------------------------------------------------
void CFormatString::SetFixedLastField( bool FixedLastField )
{
    if ( GetDecedField( m_Format, m_Dec ) <= 0 ) return ;
    m_FixedLastField = FixedLastField ; FormatChangeApply( 0 ) ;
}
//------------------------------------------------------------------------------
bool CFormatString::GetFixedLastField( )
{   return( ( m_FixedLastField == EDF_PROPERTY_NOTSET ? false : ( m_FixedLastField == 0 ? false : true ) ) ) ; }
//------------------------------------------------------------------------------
void CFormatString::SetSeparator( char c )
{   m_Spr2 = c ; FormatChangeApply( 0 ) ; }
//------------------------------------------------------------------------------
char CFormatString::GetSeparator( )
{   return( m_Spr2 ) ; }
//------------------------------------------------------------------------------
void CFormatString::SetSeparatorDec( char c )
{   m_Spr1 = c ; FormatChangeApply( 0 ) ; }
//------------------------------------------------------------------------------
char CFormatString::GetSeparatorDec( )
{   return( m_Spr1 ) ; }
//------------------------------------------------------------------------------
void CFormatString::SetLongSeparator( bool LongSeparator )
{   m_Space = LongSeparator ; FormatChangeApply( 0 ) ; }
//------------------------------------------------------------------------------
bool CFormatString::GetLongSeparator( )
{   return( m_Space ) ; }
//------------------------------------------------------------------------------
void CFormatString::GetMaxMin( double& Max, double& Min, int& Format )
{   Max = m_Max ; Min = m_Min ; Format = m_FormatMM ; }
//------------------------------------------------------------------------------
void CFormatString::SetMaxMin( double  Max, double  Min, int  Format )
{   m_Max = Max ; m_Min = Min ; m_FormatMM = Format ; }

//------------------------------------------------------------------------------
char* CFormatString::GetText( )
{   GetEditText( ) ; return m_Text ; }

//------------------------------------------------------------------------------
int CFormatString::GetDataText( char* s )
{   
	s[0]= '\0' ;
	if (m_Format!=EDF_DMY && m_Format!=EDF_DMYHMS) return-1 ; 
	GetEditText( ) ;
	memcpy( s, m_Text, 10) ; s[10] = '\0' ;
	return 0 ; 
}

//------------------------------------------------------------------------------
int CFormatString::GetTimeText( char* s )
{   
	s[0]= '\0' ;
	if (m_Format!=EDF_DMYHMS) return-1 ; 
	GetEditText( ) ;
	int n = strlen(&m_Text[11]) ;
	memcpy(s, &m_Text[11], n) ; s[n] = '\0' ;
	return 0 ; 
}

//------------------------------------------------------------------------------
bool CFormatString::SetText( char* txt, int* dl )
{
	m_BuildCode = 0 ;
	//
    if ( !txt || ( !strlen( txt ) && m_Format != EDF_NOTFORMAT ) ) return 1 ;
    sprintf( m_Text, "%s\0", txt ) ;
	if ( m_Format == EDF_NOTFORMAT ) {
      GetPos( m_Pos ) ;
      SetEditText( ) ;
      SetPos( m_Pos ) ;
	  return 0 ;
	}
	//
    if ( m_Format >= EDF_YESNO && m_Format <= EDF_CHECKYN  &&
    strcmp( txt, "Да" ) != 0 && strcmp( txt, "Нет" ) != 0 &&
    strcmp( txt, "Вкл" ) != 0 && strcmp( txt, "Выкл" ) != 0 &&
    strcmp( txt, "1" ) != 0 && strcmp( txt, "0" ) != 0 &&
    strcmp( txt, " " ) != 0 && strcmp( txt, "" ) != 0 ) {
      if ( m_Format == EDF_YESNO  ) sprintf( m_Text, "Нет\0" ) ;
      else { if ( m_Format == EDF_INSERT ) sprintf( m_Text, "Выкл\0" ) ;
             else { if ( m_Format == EDF_01     ) sprintf( m_Text, "0\0" ) ;
                    else sprintf( m_Text, "\0" ) ; } }
    }
    if ( m_Format == EDF_CHECKCOLOR ) {
      if ( strcmp( txt, "" ) != 0 && strcmp( txt, "0" ) != 0 &&
      strcmp( txt, " " ) != 0 && strcmp( txt, "1" ) != 0 &&
      strcmp( txt, "  " ) != 0 && strcmp( txt, "2" ) != 0 ) sprintf( m_Text, "\0" ) ;
    }
    bool isVali ;
    int  inMaxMin ;
    isVali = BuildValue( inMaxMin, txt ) ; if ( !isVali ) return 1 ;
    if ( dl ) *dl = dl_BuildFromText ;
    GetPos( m_Pos ) ;
    SetEditText( ) ;
    SetPos( m_Pos ) ;
    return !isVali ;
}
//------------------------------------------------------------------------------
bool CFormatString::SetValue( double Value, int FormatIO )
{	
    int oldF = -1 ;
    if ( m_isAutoChangeApply && FormatIO != EDF_NOTFORMAT ) { oldF = m_FormatIO ; m_FormatIO = FormatIO ; }
    m_Value = Value ;
    if ( m_isAutoChangeApply ) BuildFromData( ) ;
    if ( oldF !=-1 ) m_FormatIO = oldF ;
	return 0 ;
}
//------------------------------------------------------------------------------
double CFormatString::GetValue( int FormatIO )
{
    int oldF = -1 ;
    if ( FormatIO != EDF_NOTFORMAT ) { oldF = m_FormatIO ; m_FormatIO  = FormatIO ; }
    double Value = 0 ;
    int inMaxMin ;
    if ( BuildValue( inMaxMin ) ) Value = m_Value ;
    if ( oldF !=-1 ) m_FormatIO = oldF ;
    return Value ;
}
//------------------------------------------------------------------------------
/*
DATA::DATA( )
{
	d = 0 ;
	m = 0 ;
	g = 0 ;
}
*/
//------------------------------------------------------------------------------
bool CFormatString::SetDate( const DATE& d )
{	
	DMYEDF dmy ;
	dmy.d = d.d ;
	dmy.m = d.m ;
	dmy.g = d.g ;
	SetDMY( dmy ) ;
	return 0 ;
}
//------------------------------------------------------------------------------
DATE  CFormatString::GetDate(  )
{
	assert( GetTypeFormat( ) == EDF_TFDATA ) ;
	DMYEDF dmy ;
	dmy = GetDMY(  ) ;
	DATE d ;
	d.d = dmy.d ;
	d.m = dmy.m ;
	d.g = dmy.g ;
    return d ;
}
//------------------------------------------------------------------------------
bool CFormatString::SetTime( const TIME& t )
{	
	HMSEDF hms ;
	hms.h = t.h ;
	hms.m = t.m ;
	hms.s = t.s + t.d ;
	SetHMS( hms ) ;
	return 0 ;
}
//------------------------------------------------------------------------------
TIME CFormatString::GetTime(  )
{
	assert( GetTypeFormat( ) == EDF_TFTIME ) ;
	HMSEDF hms ;
	hms = GetHMS(  ) ;
	TIME t ;
	t.h = hms.h ;
	t.m = hms.m ;
	t.s = (int)floor( hms.s ) ;
	t.d = hms.s - t.s ;
    return t ;
}
//------------------------------------------------------------------------------
bool CFormatString::SetValueSign   (  double s ) 
{
	if ( !m_Sign || m_Format == EDF_NOTFORMAT || ( m_Format == EDF_YESNO && m_Format <= EDF_ALT ) ) return 1 ;
	m_Value = fabs( m_Value ) * ( s >= 0 ? 1 :-1 ) ;
    if ( m_isAutoChangeApply ) BuildFromData( ) ;
	return 0 ;
}
//------------------------------------------------------------------------------
bool CFormatString::ChangeValueSign( )  
{
	if ( !m_Sign || m_Format == EDF_NOTFORMAT || ( m_Format == EDF_YESNO && m_Format <= EDF_ALT ) ) return 1 ;
	m_Value = -m_Value ;
    if ( m_isAutoChangeApply ) BuildFromData( ) ;
	return 0 ;
}
//------------------------------------------------------------------------------
double CFormatString::GetHMSPK( )
{
    double t = 0 ;
    HMSEDF tt = GetHMS( ) ;
    t = tt.h * 10000.0 + tt.m * 100.0 + tt.s ;
    return t ;
}
//------------------------------------------------------------------------------
HMSEDF CFormatString::GetHMS( )
{
    int tf = GetTypeFormat( ) ;
    if ( m_Format != EDF_DMYHMS && tf != EDF_TFTIME ) throw( "Ошибочный запрос времени" ) ;
    HMSEDF t = {0, 0, 0};
    int errField, inMaxMin ;
    int rc = IsValiValue ( errField, inMaxMin ) ; if ( !rc ) return t ;
    if ( m_Format == EDF_DMYHMS ) {
      t.h = (int)m_f[3] ;
      t.m = (int)m_f[4] ;
      t.s = m_f[5] + m_f[6]/pow(10.0, m_Dec) ;
      return t ;
    }
    if ( m_Format == EDF_HMS ) {
      t.h = (int)m_f[0] ;
      t.m = (int)m_f[1] ;
      t.s = m_f[2] + m_f[3]/pow(10.0, m_Dec) ;
      return t ;
    }
    rc = BuildValue(inMaxMin) ;
    if (!rc || inMaxMin) m_Value = 0 ;
    int k1, k2, k3 ;
    double d1 ;
	CRGradTime gt ;
    gt.fromDouble(m_Value, m_FormatIO) ;
    gt.GetFields (&k1, &k2, &k3, &d1) ;
    t.h = k1 ;
    t.m = k2 ;
    t.s = k3+d1 ;
    return t ;
}
//------------------------------------------------------------------------------
GMSEDF CFormatString::GetGMS( )
{
    int tf = GetTypeFormat( ) ;
    if (tf!=EDF_TFANGL) throw( "Ошибочный запрос угла" ) ;
    GMSEDF g = {0, 0, 0} ;
    int errField, inMaxMin ;
    int rc = IsValiValue(errField, inMaxMin) ; if (!rc) return g ;
    if (m_Format==EDF_GMS) {
      g.g = (int)m_f[0] ;
      g.m = (int)m_f[1] ;
      g.s = m_f[2]+m_f[3]/pow(10.0, m_Dec) ;
      return g ;
    }
    rc = BuildValue(inMaxMin) ;
    if (!rc || inMaxMin) m_Value = 0 ;
    int k1, k2, k3 ;
    double d1 ;
	CRGradTime gt ;
    gt.fromDouble(m_Value, m_FormatIO) ;
    gt.GetFields (&k1, &k2, &k3, &d1) ;
    g.g = k1 ;
    g.m = k2 ;
    g.s = k3+d1 ;
    return g ;
}
//------------------------------------------------------------------------------
void CFormatString::SetHMSPK( double t )
{
    HMSEDF tt = {0, 0, 0};
    tt.h = (int)(t/10000.0) ;
    tt.m = (int)((t-tt.h*10000.0)/100.0) ;
    tt.s = t-tt.h*10000.0-tt.m*100.0 ;
    SetHMS(tt) ;
}
//------------------------------------------------------------------------------
void CFormatString::SetHMS(HMSEDF t)
{
    int tf = GetTypeFormat( ) ;
    if ( m_Format != EDF_DMYHMS && tf != EDF_TFTIME ) throw( "Ошибочная попытка задания времени" ) ;
    if ( m_Format == EDF_DMYHMS ) {
      m_f[ 3 ] = t.h ;
      m_f[ 4 ] = t.m ;
      m_f[ 5 ] = ( int )t.s ;
      m_f[ 6 ] = floor( ( t.s - ( int )t.s ) * pow( 10.0, m_Dec ) ) ;
    }
    if ( m_Format == EDF_HMS ) {
      m_f[ 0 ] = t.h ;
      m_f[ 1 ] = t.m ;
      m_f[ 2 ] = ( int )t.s ;
      m_f[ 3 ] = floor( ( t.s - ( int )t.s ) * pow( 10.0, m_Dec ) ) ;
    }
    int inMaxMin ;
    BuildValueFromFields( inMaxMin ) ; BuildStringFromFields( ) ;
	if ( m_isAutoChangeApply ) SetEditText(  ) ;
}
//------------------------------------------------------------------------------
void CFormatString::SetGMS( GMSEDF g )
{
    int tf = GetTypeFormat( ) ;
    if ( tf != EDF_TFANGL ) throw( "Ошибочная попытка задания угла" ) ;
    if ( m_Format == EDF_GMS ) {
      m_f[ 0 ] = g.g ;
      m_f[ 1 ] = g.m ;
      m_f[ 2 ] = ( int )g.s ;
      m_f[ 3 ] = floor( ( g.s - ( int )g.s ) * pow( 10.0, m_Dec ) ) ;
    }
    int inMaxMin ;
    BuildValueFromFields(  inMaxMin ) ; BuildStringFromFields( ) ;
	if ( m_isAutoChangeApply ) SetEditText( ) ;
}
//------------------------------------------------------------------------------
double CFormatString::GetDMYPK( int TypePK )
{
    double d = 0 ;
    double g ;
    DMYEDF dt = GetDMY( ) ;
    switch ( TypePK ) {
      case 0 : d = dt.g * 10000.0 + dt.m * 100.0 + dt.d ; break ;     // ggggmmmdd.0
      case 1 : d = dt.d * 1000000.0 + dt.m * 10000.0 + dt.g ; break ; // ddmmgggg.0
      case 2 : g = dt.g - 2000.0 ; g = ( g >= 0 ? g : g + 100 ) ;
               d = dt.d * 10000.0 + dt.m * 100.0 + g ; break ;        // ddmmgg.0
    }
    return d ;
}
//------------------------------------------------------------------------------
void CFormatString::SetDMYPK( double d, int TypePK )
{
    DMYEDF dt ;
    switch ( TypePK ) {
      case 0 : // ggggmmmdd.0
        dt.g = ( int )( d / 10000.0 ) ;
        dt.m = ( int )( ( d - dt.g * 10000.0 ) / 100.0 ) ;
        dt.d = ( int )( d - dt.g * 10000.0 - dt.m * 100.0 ) ;
        break ;
      case 1 : // ddmmgggg.0
        dt.d = ( int )( d / 1000000.0 ) ;
        dt.m = ( int )( ( d - dt.d * 1000000.0 ) / 10000.0 ) ;
        dt.g = ( int )( d - dt.d * 1000000.0 - dt.m * 10000.0 ) ;
        break ;
      case 2 : // ddmmgg.0
        dt.d = ( int )( d / 10000.0 ) ;
        dt.m = ( int )( ( d - dt.d * 10000.0 ) / 100.0 ) ;
        dt.g = ( int )( d - dt.d * 10000.0 - dt.m * 100.0 ) ;
        dt.g = dt.g > 50 ? dt.g + 1900 : dt.g + 2000 ;
        break ;
    }
    SetDMY( dt ) ;
}
//------------------------------------------------------------------------------
void CFormatString::SetDMYPK( char* s, int TypePK )
{
    char   str[ 256 ] ;
    char*  ps ;
    int    i = 0, len = strlen( s ) ;
    sprintf( str, "%s\0", s ) ;
    while ( i < len ) {
      if ( str[ i ] != '0' && str[ i ] != '1' && str[ i ] != '2' && str[ i ] != '3' &&
      str[ i ] != '4' && str[ i ] != '5' && str[ i ] != '6' && str[ i ] != '7' &&
      str[ i ] != '8' && str[ i ] != '9' ) i++ ; else break ;
    }
    if ( i == len ) return ;
    ps = &str[ i ] ;
    while ( i < len ) {
      if ( str[ i ] != '0' && str[ i ] != '1' && str[ i ] != '2' && str[ i ] != '3' &&
      str[ i ] != '4' && str[ i ] != '5' && str[ i ] != '6' && str[ i ] != '7' &&
      str[ i ] != '8' && str[ i ] != '9' ) break ;
    }
    if ( i < len ) str[ i ] = '\0' ;
	double d; d = 0;
    i = sscanf(ps, "%lf", &d) ; if (i != 1) return ;
    SetDMYPK(d, TypePK) ;
}
//------------------------------------------------------------------------------
void CFormatString::GetDMYPK( char* s, int TypePK )
{
    double d = GetDMYPK( TypePK ) ;
    sprintf( s, "%.0lf\0", d ) ;
}
//------------------------------------------------------------------------------
DMYEDF CFormatString::GetDMY( )
{
    DMYEDF d = { 0, 0, 0 };
    if ( m_Format != EDF_DMY && m_Format != EDF_DMYHMS ) throw( "Ошибочный запрос даты" ) ;
    int errField, inMaxMin ;
    int rc = IsValiValue ( errField, inMaxMin ) ; if ( !rc ) return d ;
    d.d = ( int )m_f[ 0 ] ;
    d.m = ( int )m_f[ 1 ] ;
    d.g = ( int )m_f[ 2 ] ;
    return d ;
}
//------------------------------------------------------------------------------
void CFormatString::SetDMY( DMYEDF d )
{
    if ( m_Format != EDF_DMY ) throw( "Ошибочная попытка задания даты" ) ;
    CRDate dmy ;
    int rc = dmy.IsValiFields( d.d, d.m, d.g ) ; if ( rc ) throw ( "Неверный формат даты" ) ;
    dmy.SetFields( d.d, d.m, d.g ) ;
    SetValue( dmy.julDayNumber( ) - JULDAY_01011975_FROMZERO ) ;
}
//------------------------------------------------------------------------------
void CFormatString::GetDMYHMS( DMYEDF& d, HMSEDF& t )
{
    if ( m_Format != EDF_DMYHMS && m_Format != EDF_DMY ) throw( "Ошибочный запрос даты и времени" ) ;
    int errField, inMaxMin ;
    int rc = IsValiValue ( errField, inMaxMin ) ; if ( !rc ) return ;
    d.d = ( int )m_f[ 0 ] ;
    d.m = ( int )m_f[ 1 ] ;
    d.g = ( int )m_f[ 2 ] ;
    if ( m_Format == EDF_DMYHMS ) {
      t.h = ( int )m_f[ 3 ] ;
      t.m = ( int )m_f[ 4 ] ;
      t.s = m_f[ 5 ] + m_f[ 6 ] / pow( 10.0, m_Dec ) ;
    }
}
//------------------------------------------------------------------------------
void CFormatString::SetDMYHMS( DMYEDF d, HMSEDF t )
{
    if ( m_Format != EDF_DMYHMS && m_Format != EDF_DMY ) throw( "Ошибочная попытка задания даты и времени" ) ;
    CRDate dmy ;
    int rc = dmy.IsValiFields( d.d, d.m, d.g ) ;
    if ( rc ) throw( "Неверный формат даты" ) ;
    if ( m_Format == EDF_DMYHMS ) {
      if ( t.h < 0 || t.h >= 24 || t.m < 0 || t.m >= 60 || t.s < 0 || t.s >= 60 )
        throw( "Неверный формат времени" ) ;
    }
    dmy.SetFields( d.d, d.m, d.g ) ;
    m_f[ 0 ] = d.d ;
    m_f[ 1 ] = d.m ;
    m_f[ 2 ] = d.g ;
    if ( m_Format == EDF_DMYHMS ) {
      m_f[ 3 ] = t.h ;
      m_f[ 4 ] = t.m ;
      m_f[ 5 ] = ( int )t.s ;
      m_f[ 6 ] = floor( ( t.s - ( int )t.s ) * pow( 10.0, m_Dec ) ) ;
    }
    int inMaxMin ;
    BuildValueFromFields(  inMaxMin ) ; BuildStringFromFields( ) ;
	if ( m_isAutoChangeApply ) SetEditText(  ) ; 
}
//------------------------------------------------------------------------------
int CFormatString::GetTypeFormat( int Format )
{
	Format = ( Format >= 0 ? Format : m_Format ) ;
    // Дата
    if ( Format == EDF_DMY ) return EDF_TFDATA ;
    // Дага-Время
    if ( Format == EDF_DMYHMS ) return EDF_TFDATATIME ;
    // Время
    if ( Format >= EDF_HMS && Format <= EDF_SSS ) return EDF_TFTIME ;
    // Угловая величина
    if ( Format >= EDF_GMS && Format <= EDF_RAD ) return EDF_TFANGL ;
    // Виток Аргумент
    if ( Format >= EDF_VITGMS && Format <= EDF_VITRAD ) return EDF_TFVITARG ;
    // Число
    if ( Format == EDF_FLOAT || Format == EDF_FIXED ) return EDF_TFNUMBER ;
    // Целое
    if ( Format == EDF_INTEGER ) return EDF_TFINT ;
    // Строка
    if ( Format == EDF_NOTFORMAT ) return EDF_TFNOTFORMAT ;
    // Битовая величина
    if ( Format >= EDF_YESNO && Format <= EDF_01 ) return EDF_TFBOOL ;
    // Битовая величина (галочка)
    if ( Format >= EDF_CHECK && Format <= EDF_CHECKCOLOR ) return EDF_TFCHECK ;
    // Ключи НУ
    if ( Format >= EDF_KEYNU && Format <= EDF_KEYNUFULL ) return EDF_TFKEYNU ;
    // Произвольный набор ключей
    if ( Format == EDF_CUSTOMKEY ) return EDF_TFKEY ;
    return 0 ;
}
//------------------------------------------------------------------------------
bool CFormatString::BuildValue( int& inMaxMin, char* Text, int Flag )
{
	bool rc ;	
    if ( m_Format == EDF_NOTFORMAT ) return 1 ;
	m_BuildCode = 0 ;
    dl_BuildFromText = 0 ;
    rc = BuildFromString( Text, &dl_BuildFromText, ( ( Flag & 4 ) != 0 ), ( ( Flag & 8 ) != 0 ) ) ; if ( !rc ) return 0 ;
    rc = BuildValueFromFields( inMaxMin ) ;
    return rc ;
}
//------------------------------------------------------------------------------
bool CFormatString::BuildValueFromFields( int& inMaxMin )
{
	int    rc, k1, k2, k3 ;
	double d1 ;
    double VMM ;
    //
    if ( m_Format == EDF_NOTFORMAT ) return 1 ;
    //
	m_BuildCode = 0 ;
    m_Value     = 0 ;
    inMaxMin    = 0 ;
try {
    switch( m_Format ) {
	  case EDF_DMY : {
        CRDate  dmy ;
	    dmy.SetFlags( 3 ) ;
		k1 = ( int )fabs( m_f[ 0 ] ) ; k2 = ( int )m_f[ 1 ] ; k3 = ( int )m_f[ 2 ] ;
        rc = dmy.IsValiFields( k1, k2, k3 ) ; if ( rc ) return false ;
		dmy.SetFields( k1, k2, k3 ) ;
        m_Value = dmy.julDayNumber() ;
		m_Value-= 2442414 ;
        VMM = m_Value ;
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        } }
		break ;
	  case EDF_DMYHMS :
		d1 = m_f[ 5 ] + m_f[ 6 ] / pow( 10.0, m_lp[ 6 ] ) ;
        CalcTATime( m_Value, ( int )m_f[ 0 ], ( int )m_f[ 1 ], ( int )m_f[ 2 ], ( int )m_f[ 3 ], ( int )m_f[ 4 ], d1 ) ;
        VMM = m_Value ;
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        }
		break ;
	  case EDF_SSSHMS : {
      	CRGradTime gt ;
		k1 = ( int )m_f[ 1 ] ; k2 = ( int )m_f[ 2 ] ;
		k3 = ( int )m_f[ 3 ] ;
		d1 = m_f[ 4 ] / pow( 10.0, m_lp[ 4 ] ) * 1000.0 ;
        gt.SetFields( k1, k2, k3, d1 ) ;
		if ( m_FormatIO != EDF_HMS ) {
		  m_Value = gt.toDouble( m_FormatIO ) ;
          d1 = 1.0 ;
          if ( m_FormatIO == EDF_MILLIS ) d1 = 86400000.0 ;
          if ( m_FormatIO == EDF_SEC    ) d1 = 86400.0 ;
          if ( m_FormatIO == EDF_MIN    ) d1 = 1440.0 ;
          if ( m_FormatIO == EDF_HOUR   ) d1 = 24.0 ;
          m_Value += fabs( m_f[ 0 ] ) * d1 ;
        } else   {
          m_Value = m_f[ 1 ] * 10000 + m_f[ 2 ] * 100 + k3 + d1 / 1000.0 ;
        }
        m_Value*= m_s < 0 ? -1.0 : 1.0 ;
        VMM = gt.toDouble( m_FormatMM ) ;
        VMM*= m_s < 0 ? -1.0 : 1.0 ;
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        } }
        break ;
	  case EDF_HMS :
      case EDF_GMS : {
      	CRGradTime gt ;
		k1 = ( int )fabs( m_f[ 0 ] ) ; k2 = ( int )m_f[ 1 ] ;
		k3 = ( int )m_f[ 2 ] ;
		d1 = m_f[ 3 ] / pow( 10.0, m_lp[ 3 ] ) * 1000.0 ;
        gt.SetFields( k1, k2, k3, d1 ) ;
		if ( ( m_FormatIO != EDF_HMS ) && ( m_FormatIO != EDF_GMS ) )
		  m_Value = gt.toDouble( m_FormatIO ) ;
		else   m_Value = fabs( m_f[ 0 ] ) * 10000 + m_f[ 1 ] * 100 + k3 + d1 / 1000.0 ;
        m_Value*= m_s < 0 ? -1.0 : 1.0 ;
        VMM = gt.toDouble( m_FormatMM ) ;
        VMM*= m_s < 0 ? -1.0 : 1.0 ;
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        } }
        break ;
      case EDF_VITGMS : {
      	CRGradTime gt ;
		k1 = ( int )m_f[ 1 ] ; k2 = ( int )m_f[ 2 ] ;
		k3 = ( int )m_f[ 3 ] ;
		d1 = m_f[ 4 ] / pow( 10.0, m_lp[ 4 ] ) * 1000.0 ;
        gt.SetFields( k1, k2, k3, d1 ) ;
		m_Value = gt.toDouble( EDF_GRAD ) ;
        m_Value = m_f[ 0 ] + m_Value / 360.0 ;
        m_Value*= m_s < 0 ? -1.0 : 1.0 ;
        VMM = m_Value ;
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        } }
        break ;
      case EDF_VITGRAD : {
		m_Value = m_f[ 1 ] + m_f[ 2 ] / pow( 10.0, m_lp[ 2 ] ) ;
        m_Value = m_f[ 0 ] + m_Value / 360.0 ;
        m_Value*= m_s < 0 ? -1.0 : 1.0 ;
        VMM = m_Value ;
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        } }
        break ;
      case EDF_VITRAD : {
		m_Value = m_f[1]+m_f[2]/pow(10.0, m_lp[2]) ;
        m_Value = m_f[0]+m_Value/(2*acos(-1.0)) ;
        m_Value*= m_s<0 ? -1.0:1.0 ;
        VMM = m_Value ;
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        } }
        break ;
      case EDF_MILLIS :
      case EDF_SEC :
	  case EDF_MIN :
	  case EDF_HOUR :
	  case EDF_SSS :
      case EDF_GRAD :
	  case EDF_RAD : {
      	CRGradTime gt ;
   	    d1 = fabs( m_f[ 0 ] ) + m_f[ 1 ] / pow( 10.0, m_lp[ 1 ] ) ;
		gt.fromDouble( d1, m_Format ) ;
		if ( ( m_FormatIO != EDF_HMS ) && ( m_FormatIO != EDF_GMS ) )
		  m_Value = gt.toDouble( m_FormatIO ) ;
		else {
		  gt.GetFields( &k1, &k2, &k3, &d1 ) ;
		  m_Value = k1 * 10000 + k2 * 100 + k3 + d1 / 1000.0;
		}
        m_Value*= m_s < 0 ? -1.0 : 1.0 ;
        VMM = gt.toDouble( m_FormatMM ) ;
        VMM*= m_s < 0 ? -1.0 : 1.0 ;
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        } }
		break ;
	  case EDF_INTEGER :
	  case EDF_01 :
	  case EDF_YESNO :
	  case EDF_INSERT :
	  case EDF_CHECK :
	  case EDF_CHECKYN :
      case EDF_CHECKCOLOR :
        m_Value = fabs( m_f[ 0 ] ) ;
        m_Value*= m_s < 0 ? -1.0 : 1.0 ;
        VMM = m_Value ;
         if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        }
		break ;
	  case EDF_ALT :
        m_Value = m_f[ 0 ] ;
		break ;
	  case EDF_FIXED :
        m_Value = fabs( m_f[ 0 ] ) + m_f[ 1 ] / pow( 10.0, m_lp[ 1 ] ) ;
        m_Value*= m_s < 0 ? -1.0 : 1.0 ;
        VMM = m_Value ;
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        }
	    break ;
	  case EDF_FLOAT :
        m_Value = ( fabs( m_f[ 0 ] ) + m_f[ 1 ] / pow( 10.0, m_lp[ 1 ] ) ) * pow( 10.0, m_f[ 2 ] ) ;
        m_Value*= m_s < 0 ? -1.0 : 1.0 ;
        VMM = m_Value ;
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        }
		break ;
//	  case EDF_KEYNU :
//	  case EDF_KEYNUFULL :
//		double d[ 5 ] ; GetScaleKeyNu( d, m_lp, m_Format ) ;
//		m_Value = 0 ; for ( int i = 0 ; i < m_np ; i++ ) m_Value+= m_f[ i ] * d[ i ] ;
//		break ;
	}
}
catch(...) {
    return false ;
}
	if ( inMaxMin ) { m_BuildCode =-inMaxMin ; return false ; }
	return true ;
}
//------------------------------------------------------------------------------
void CFormatString::CalcNS( int m_Len, int& dl )
{
    dl = m_Len / 3 - 1 ;
	if ( ( m_Len / 3 ) * 3 != m_Len ) dl++ ;
}
//------------------------------------------------------------------------------
void CFormatString::CalcNS1( int m_Len, int& dl )
{
    dl = ( m_Len ) / 4 ;
}
//------------------------------------------------------------------------------
void CFormatString::BuildTampleteCustomKey( int* Len, int KeyCount )
{
	int i ;
    m_s = 0 ;
    memset( m_ip, 0, 14* sizeof( int ) ) ;
    memset( m_fp, 0, 7 * sizeof( bool ) ) ;
    if ( KeyCount > 0 && Len ) {
		memset( m_f,  0, 7 * sizeof( double ) ) ;
		memset( m_lp, 0, 7 * sizeof( int ) ) ;
		m_np = KeyCount ;
		for( i = 0 ; i < m_np ; i++ ) m_lp[ i ] = Len[ i ] ;
    }
    for( i = 0 ; i < m_np ; i++ ) m_fp[ i ] = m_FixedFields ;
    m_fp[ 0 ] = m_FixedFirstField != EDF_PROPERTY_NOTSET ? ( !m_FixedFirstField ? false : true ) : m_fp[ 0 ] ;
    m_fp[ m_np - 1 ] = m_FixedLastField != EDF_PROPERTY_NOTSET ? ( !m_FixedLastField ? false : true ) : m_fp[ m_np - 1 ] ;
    m_ip[ 0 ][ 0 ] = 0 ;
    for ( i = 0 ; i < m_np ; i ++ ) {
		m_ip[ 1 ][ i ] = m_ip[ 0 ][ i ] + ( m_fp[ i ] ? m_lp[ i ] : 1 ) ;
		if ( i < m_np - 1 ) {
			m_ip[ 0 ][ i + 1 ] = m_ip[ 1 ][ i ] + ( m_Space ? 3 : 1 ) ;
		}
	}
}
//------------------------------------------------------------------------------
void CFormatString::SetFieldsValue( int* Fields )
{
    for( int i = 0 ; i < m_np ; i++ ) m_f[ i ] = Fields[ i ] ;
}
//------------------------------------------------------------------------------
void CFormatString::GetFieldsValue( int* Fields )
{
    for(int i=0; i<m_np; i++) Fields[i] = (int)m_f[i] ;
}
//------------------------------------------------------------------------------
void CFormatString::BuildTamplete ( )
{
    if ( m_Format == EDF_CUSTOMKEY ) {
      BuildTampleteCustomKey( ) ; return ;
    }
	int i ;
    m_s = 0 ;
    m_np = 0 ;
    memset( m_ip, 0, 14* sizeof( int ) ) ;
    memset( m_lp, 0, 7 * sizeof( int ) ) ;
    memset( m_f,  0, 7 * sizeof( double ) ) ;
    memset( m_fp, 0, 7 * sizeof( bool ) ) ;
	switch ( m_Format ) {
	 case EDF_DMY :
	      m_np = 3 ;
		  m_lp[ 0 ] = 2 ;  m_lp[ 1 ] = 2 ;  m_lp[ 2 ] = 4 ;
	 break ;
	 case EDF_DMYHMS :
	      m_np = 7 ;
		  m_lp[ 0 ] = 2 ;  m_lp[ 1 ] = 2 ;  m_lp[ 2 ] = 4 ; m_lp[ 3 ] = 2 ;  m_lp[ 4 ] = 2 ;  m_lp[ 5 ] = 2 ;  m_lp[ 6 ] = m_Dec ;
          if ( m_Dec == 0 ) m_np -- ;
     break ;
	 case EDF_HMS :
		  m_np = 4 ;
		  m_lp[ 0 ] = m_Interval ? m_Len : 2 ;
		  m_lp[ 1 ] = 2 ;  m_lp[ 2 ] = 2 ;  m_lp[ 3 ] = m_Dec ;
          if ( m_Dec == 0 ) m_np -- ;
	 break ;
	 case EDF_SSSHMS :
		  m_np = 5 ;
		  m_lp[ 0 ] = m_Len ;
		  m_lp[ 1 ] = 2 ;  m_lp[ 2 ] = 2 ;  m_lp[ 3 ] = 2 ;  m_lp[ 4 ] = m_Dec ;
          if ( m_Dec == 0 ) m_np -- ;
	 break ;
	 case EDF_GMS :
          m_np = 4 ;
          m_lp[ 0 ] = m_Interval ? m_Len : 3 ;
		  m_lp[ 1 ] = 2 ;  m_lp[ 2 ] = 2 ;  m_lp[ 3 ] = m_Dec ;
          if ( m_Dec == 0 ) m_np -- ;
	 break ;
	 case EDF_VITGMS :
          m_np = 5 ;
          m_lp[ 0 ] = m_Interval ? m_Len : 5 ;
		  m_lp[ 1 ] = 3 ;  m_lp[ 2 ] = 2 ;  m_lp[ 3 ] = 2 ;  m_lp[ 4 ] = m_Dec ;
          if ( m_Dec == 0 ) { m_np -- ; m_lp[ 4 ] = 0 ; }
	 break ;
     case EDF_MILLIS :
	 case EDF_SEC :
	 case EDF_MIN :
	 case EDF_HOUR :
	 case EDF_GRAD :
	 case EDF_SSS :
	 case EDF_RAD :
	 case EDF_FIXED :
          m_np = 2 ;
		  if ( m_Format == EDF_FIXED || m_Interval ) {
			m_lp[ 0 ] = m_Len ;
			if ( m_SpaceLeftField ) {
              int dl ;
			  CalcNS( m_Len, dl ) ;
			  m_lp[ 0 ]+= dl ;
			}
		  }
		  else {
		       switch ( m_Format ) {
		       case EDF_MILLIS : m_lp[ 0 ] = 8 ; break ;
		       case EDF_SEC :	 m_lp[ 0 ] = 5 ; break ;
		       case EDF_MIN :	 m_lp[ 0 ] = 4 ; break ;
		       case EDF_HOUR :	 m_lp[ 0 ] = 2 ; break ;
		       case EDF_GRAD :	 m_lp[ 0 ] = 3 ; break ;
		       case EDF_RAD :	 m_lp[ 0 ] = 1 ; break ;
			   }
		  }
		  m_lp[ 1 ] = m_Dec ;
          if ( m_Dec == 0 ) m_np -- ;
	 break ;
	 case EDF_VITGRAD :
	 case EDF_VITRAD :
          m_np = 3 ;
          m_lp[ 0 ] = m_Interval ? m_Len : 5 ;
          switch ( m_Format ) {
		  case EDF_VITGRAD : m_lp[ 1 ] = 3 ; break ;
		  case EDF_VITRAD :	 m_lp[ 1 ] = 1 ; break ;
		  }
		  m_lp[ 2 ] = m_Dec ;
          if ( m_Dec == 0 ) { m_np -- ; m_lp[ 2 ] = 0 ; }
	 break ;
	 case EDF_INTEGER :
	      m_np      = 1 ;
		  m_lp[ 0 ] = m_Len ;
	 break ;
	 case EDF_FLOAT :
          if ( m_Dec > 0 ) {
            m_np = 3 ;
            m_lp[ 0 ] = m_Len ;
		    m_lp[ 1 ] = m_Dec ;
	        m_lp[ 2 ] = 2 ;
          } else {
            m_np = 2 ;
            m_lp[ 0 ] = m_Len ;
	        m_lp[ 1 ] = 2 ;
          }
	 break ;
	 case EDF_KEYNU :
	      m_np = 4 ;
		  m_lp[ 0 ] = m_Dec ;  m_lp[ 1 ] = 2 ;  m_lp[ 2 ] = 3 ; m_lp[ 3 ] = 2 ;
	 break ;
	 case EDF_KEYNUFULL :
	      m_np = 5 ;
		  m_lp[ 0 ] = m_Len ;  m_lp[ 1 ] = m_Dec ;  m_lp[ 2 ] = 2 ;  m_lp[ 3 ] = 3 ; m_lp[ 4 ] = 2 ;
	 break ;
	}
    for ( i = 0 ; i < m_np ; i ++ ) m_fp[ i ] = m_FixedFields ;
    if ( m_FixedFirstField != EDF_PROPERTY_NOTSET && GetFirstField( m_Format ) == 0 ) {
		m_fp[ 0 ] = !m_FixedFirstField ? false : true ;
    }
    if ( m_FixedLastField != EDF_PROPERTY_NOTSET && GetDecedField( m_Format, m_Dec ) > 0 ) {
		m_fp[ GetDecedField( m_Format, m_Dec ) ] = !m_FixedLastField ? false : true ;
    }
    m_ip[ 0 ][ 0 ] = 0 ;
    for ( i = 0 ; i < m_np ; i ++ ) {
	  m_ip[ 1 ][ i ] = m_ip[ 0 ][ i ] + ( m_fp[ i ] ? m_lp[ i ] : 1 ) ;
	  if ( i < m_np - 1 ) {
        m_ip[ 0 ][ i + 1 ] = m_ip[ 1 ][ i ] + ( m_Space ? 3 : 1 ) ;
	    if ( m_Format == EDF_FLOAT && i == 1 ) {
		  m_ip[ 0 ][ i + 1 ] = m_ip[ 1 ][ i ] + ( m_Space ? 3 : 2 ) ;
		}
		if ( m_Format == EDF_DMYHMS && i == 2 ) {
		  m_ip[ 0 ][ i + 1 ] = m_ip[ 1 ][ i ] + 1 ;
		}
		if ( m_Format == EDF_SSSHMS && i == 0 ) {
		  m_ip[ 0 ][ i + 1 ] = m_ip[ 1 ][ i ] + 1 ;
		}
		if ( m_Format >= EDF_VITGMS && m_Format <= EDF_VITRAD && i == 0 ) {
		  m_ip[ 0 ][ i + 1 ] = m_ip[ 1 ][ i ] + 1 ;
		}
	  }
	}
}
//------------------------------------------------------------------------------
void CFormatString::ChangeIP ( int i, int dl )
{
    if ( dl == 0 ) return ;
    if ( i == -1 ) { i = 0  ; m_ip[ 0 ][ 0 ] += dl ; }
    m_ip[ 1 ][ i ] += dl ;
	for ( int j = i + 1 ; j < m_np ; j ++ ) {
	  m_ip[ 0 ][ j ] += dl ; m_ip[ 1 ][ j ] += dl ;
	}
}
//------------------------------------------------------------------------------
void CFormatString::BuildSubString( int i, char* r, bool TestRightZero )
{
    if ( i == m_np ) return ;
	//
	char   str[ 20 ] ;
	int    j, l0, l1, l2, dl ;
	double x = fabs( m_f[ i ] ) ;
	double vmax, vmin ;
	//
	if ( FieldMaxMin ( i, vmin, vmax ) ) {
	  if ( x < vmin ) { x = vmin ; m_ValiSetData = false ; }
	  if ( x > vmax ) { x = vmax ; m_ValiSetData = false ; }
    }
    //
 	x = floor( x ) ;
    sprintf( str, "%.0lf\0", x ) ;
    if ( m_fp[ i ] ||
    ( m_Format == EDF_FLOAT && i == 1 ) ||
    ( m_Format >= EDF_DMYHMS && m_Format <= EDF_FIXED && m_Format != EDF_FLOAT && i == m_np - 1 && i != 0 ) ) {
	  l0 =  m_ip[ 1 ][ i ] -  m_ip[ 0 ][ i ] ;
	  l2 =  m_lp[ i ] ;
	  if (  l0 != l2 ) ChangeIP( i, l2 - l0 ) ;
	  l1 = strlen( str ) ;
      sprintf( &m_Text[  m_ip[ 0 ][ i ] + l2 - l1 ], "%*.0lf\0", l1, x ) ;
      for ( j = 0 ; j < l2 - l1 ; j ++ ) m_Text[  m_ip[ 0 ][ i ] + j ] = '0' ;
	  if ( !m_fp[ i ] ) {
        // Подавление незначащих нулей дробной части
        for ( j = m_ip[ 1 ][ i ] - 1 ; j > m_ip[ 0 ][ i ] ; j -- )  {
          if ( !TestRightZero && j == m_ip[ 0 ][ i ] + l0 - 1 )
          break ;
          if ( m_Text[ j ] != '0' ) break ;
        }
	    m_Text[ j + 1 ] = '\0' ;
        dl = j + 1 - m_ip[ 1 ][ i ] ;
        ChangeIP( i, dl ) ;
        // Есло незначащие нули из строки были удалены
        // то необходимо отобразить строку на экране
        m_bisNeedSetText = 1 ;
      }
	} else {
	  l1 = strlen( str ) ;
	  l2 =  m_ip[ 1 ][ i ] -  m_ip[ 0 ][ i ] ;
	  if (  l1 > l2 ) ChangeIP( i, l1 - l2 ) ;
      sprintf( &m_Text[  m_ip[ 0 ][ i ] ], "%0*.0lf\0", l2, x ) ;
	}
    //
	if ( r != 0  && i < m_np -1 ) sprintf( &m_Text[  m_ip[ 1 ][ i ] ], "%s", r ) ;
    //
    if ( i == 0 ) {
      char txt[ 256 ] ;
      if ( m_s ) {
        if ( m_ip[ 0 ][ i ] != 1 ) {
          memcpy( txt, m_Text, strlen( m_Text ) + 1 ) ;
          memcpy( &m_Text[ 1 ], txt, strlen( txt ) + 1 ) ;
          ChangeIP( -1, 1 - m_ip[ 0 ][ i ] ) ;
        }
        if ( m_s < 0 ) m_Text[ 0 ] = '-' ;
        else m_Text[ 0 ] = '+' ;
      } else {
        if ( m_ip[ 0 ][ i ] != 0 ) {
          memcpy( txt, m_Text, strlen( m_Text ) + 1 ) ;
          memcpy( m_Text, &txt[ m_ip[ 0 ][ i ] ], strlen( txt ) + 1 - m_ip[ 0 ][ i ] ) ;
          ChangeIP( -1, 0 - m_ip[ 0 ][ i ] ) ;
        }
      }
    }
}
//------------------------------------------------------------------------------
void CFormatString::BuildFromData( )
{
	m_BuildCode = 0 ;
    if ( m_Format == EDF_CUSTOMKEY ) {
      BuildTampleteCustomKey( ) ;
      BuildStringFromFields( ) ;
      goto end ;
    }
    //
	if ( m_Format >= EDF_YESNO && m_Format <= EDF_CHECKCOLOR ) {
	  switch ( m_Format ) {
	  case EDF_YESNO :
	    if ( m_Value == 0 ) sprintf( m_Text, "Нет\0" ) ;
		else                sprintf( m_Text, "Да\0" ) ;
		break ;
	  case EDF_INSERT :
		if ( m_Value == 0 ) sprintf( m_Text, "Выкл\0" ) ;
		else                sprintf( m_Text, "Вкл\0" ) ;
		break ;
	  case EDF_01 :
		if ( m_Value == 0 ) sprintf( m_Text, "0\0" ) ;
		else                sprintf( m_Text, "1\0" ) ;
		break ;
      case EDF_CHECK :
      case EDF_CHECKYN :
		if ( m_Value == 0 ) sprintf( m_Text, "\0" ) ;
		else                sprintf( m_Text, " \0" ) ;
		break ;
      case EDF_CHECKCOLOR :
		sprintf( m_Text, "\0" ) ;
		if ( m_Value == 1 ) sprintf( m_Text, " \0" ) ;
		if ( m_Value == 2 ) sprintf( m_Text, "  \0" ) ;
		break ;
	  }
      goto end ;
	}
	BuildTamplete( ) ;
    //
    if ( m_Format != EDF_NOTFORMAT && m_Format != EDF_ALT ) {
      BuildFieldsFromData   ( ) ;
      BuildStringFromFields ( ) ;
    }
    if ( m_Format == EDF_ALT ) {
      m_f[ 0 ] = m_Value ;
      int i = ( int )m_Value ;
      i = i <  0 ? 0 : i ;
      i = i >= m_AltCount ? m_AltCount - 1 : i ;
      if ( m_AltString && m_AltString[ i ] ) sprintf( m_Text, "%s\0", m_AltString[ i ] ) ;
      else m_Text[ 0 ] = '\0' ;
    }	
	int Field ;
	bool isVali ; isVali = IsValiField( Field ) ; 
	if ( isVali ) {
	  m_BuildCode = -IsValiMaxMin( ) ; 
	} else m_BuildCode = Field + 1 ;
    //
end : ;
    SetEditText(  ) ;
}
//------------------------------------------------------------------------------
void CFormatString::BuildFieldsFromData ( )
{
    CRDate     dmy ;
	CRGradTime gt ;
	char       str[ 80 ] ;
	int        i, k1, k2, k3, k4, k5, k6 ;
	double     d1, dt ;
    double     Value = m_Value ;
    //
    dt = fabs( Value ) ;
	switch ( m_Format ) {
	case EDF_DMY :
      dmy = ( long )Value + JULDAY_01011975_FROMZERO ;
	  dmy.SetFlags( 3 ) ;
	  dmy.GetFields( &k1, &k2, &k3 ) ;
	  m_f[ 0 ] = k1 ; m_f[ 1 ] = k2 ; m_f[ 2 ] = k3 ; m_f[ 3 ] = 0 ;
	  break ;
	case EDF_DMYHMS :
      CalcDMVTime( Value, k1, k2, k3, k4, k5, d1 ) ;
      k6 = ( int )d1 ; dt = d1 - k6 ;
	  m_f[ 0 ] = k1 ; m_f[ 1 ] = k2 ; m_f[ 2 ] = k3 ;
      m_f[ 3 ] = k4 ; m_f[ 4 ] = k5 ; m_f[ 5 ] = k6 ;
	  break ;
	case EDF_SSSHMS :
      d1 = 1 ;
      if ( m_FormatIO == EDF_MILLIS ) d1 = 86400000.0 ;
      if ( m_FormatIO == EDF_SEC    ) d1 = 86400.0 ;
      if ( m_FormatIO == EDF_MIN    ) d1 = 1440.0 ;
      if ( m_FormatIO == EDF_HOUR   ) d1 = 24.0 ;
      m_f[ 0 ] = floor( dt / d1 ) ;
	  dt = dt - m_f[ 0 ] * d1 ;
      gt.fromDouble( dt, m_FormatIO ) ;
      gt.GetFields( &k1, &k2, &k3, &d1 ) ;
      m_f[ 1 ] = k1 ; m_f[ 2 ] = k2 ; m_f[ 3 ] = k3 ;
	  dt = d1 / 1000.0 ;
      m_f[ 4 ] =  ( d1 / 1000.0 ) * pow( 10.0, m_Dec ) ;
	  break ;
    case EDF_HMS :
	case EDF_GMS :
	  if ( ( m_Format == EDF_HMS && m_FormatIO != EDF_HMS ) || ( m_Format == EDF_GMS && m_FormatIO != EDF_GMS ) ) {
        gt.fromDouble( dt, m_FormatIO ) ;
        gt.GetFields( &k1, &k2, &k3, &d1 ) ;
	  } else {
		k1 = ( int )( dt / 10000 ) ;
		k2 = ( int )( ( dt - k1 * 10000 ) / 100 ) ;
		d1 = dt - k1 * 10000 - k2 * 100 ;
		k3 = ( int ) d1 ;
		d1 = ( d1 - k3 ) * 1000.0 ;
	  }
      m_f[ 0 ] = k1 ; m_f[ 1 ] = k2 ; m_f[ 2 ] = k3 ;
	  dt = d1 / 1000.0 ;
    break ;
	case EDF_VITGMS :
      m_f[ 0 ] = ( int )dt ;
      dt-= m_f[ 0 ] ; dt *= 360.0 ;
      gt.fromDouble( dt, EDF_GRAD ) ;
      gt.GetFields( &k1, &k2, &k3, &d1 ) ;
      m_f[ 1 ] = k1 ; m_f[ 2 ] = k2 ; m_f[ 3 ] = k3 ;
	  dt = d1 / 1000.0 ;
    break ;
	case EDF_VITGRAD :
      m_f[ 0 ] = ( int )dt ;
      dt-= m_f[ 0 ] ; dt *= 360.0 ;
      m_f[ 1 ] = ( int )dt ;
	  dt = dt - m_f[ 1 ] ;
    break ;
	case EDF_VITRAD :
      m_f[0] = (int)dt ;
      dt-= m_f[0] ; dt*= (2*acos(-1.0)) ;
      m_f[1] = (int)dt ;
	  dt = dt-m_f[1] ;
    break ;
    case EDF_MILLIS :
    case EDF_SEC :
    case EDF_MIN :
    case EDF_HOUR :
    case EDF_SSS :
    case EDF_GRAD :
    case EDF_RAD :
	  if ( ( m_FormatIO != EDF_HMS ) && ( m_FormatIO != EDF_GMS ) ) {
		gt.fromDouble( dt, m_FormatIO ) ;
	  } else {
		k1 = ( int )( dt / 10000 ) ;
		k2 = ( int )( ( dt - k1 * 10000 ) / 100 ) ;
		k3 = ( int )( dt - k1 * 10000 - k2 * 100 ) ;
		d1 = ( dt - k1 * 10000 - k2 * 100 - k3 ) * 1000.0 ;
		gt.SetFields( k1, k2, k2, d1 ) ;
	  }
      d1 = gt.toDouble( m_Format ) ;
      m_f[ 0 ] = ( int )d1 ;
	  dt = d1 ;
      //m_f[ 1 ] = ( d1 - m_f[ 0 ] ) * pow( 10.0, m_Dec ) ;
      break ;
	case EDF_INTEGER :
	  m_f[ 0 ] = floor( dt ) ;
      break ;
	case EDF_FIXED :
	  m_f[ 0 ] = floor( dt ) ;
      break ;
	case EDF_FLOAT : {
      char s1[ 50 ], *ps1 = s1 ;
      char s2[ 50 ], *ps2 = s2 ;
      char s3[ 50 ], *ps3 = s3 ;
	  sprintf( str, "%.*e\0", m_Dec + m_Len - 1, dt ) ;
      str[ 1 ] = '\0' ;
      str[ 1 + m_Dec + m_Len ] = '\0' ;
      sprintf( s1, "%s\0", &str[ 0 ] ) ;
      sprintf( s2, "%s\0", &str[ 2 ] ) ;
      sprintf( s3, "%s\0", &str[ 2 + m_Dec + m_Len ] ) ;
      for( i = 0 ; i < m_Len - 1 ; i ++ ) {
        s1[ i + 1 ] = ps2[ 0 ] ; s1[ i + 2 ] = '\0' ; ps2++ ;
      }
	  sscanf( ps1, "%lf", &m_f[ 0 ] ) ;
	  sscanf( ps2, "%lf", &m_f[ 1 ] ) ;
	  sscanf( ps3, "%lf", &m_f[ 2 ] ) ;
      m_f[ 2 ]-= m_Len - 1 ; }
      break ;
//	case EDF_KEYNU :
//	  double m ; m = pow( 10.0, m_lp[ 0 ] + m_lp[ 1 ] + m_lp[ 2 ] + m_lp[ 3 ] ) ;
//	  double g ; g = dt / m ;
//	  dt = dt - floor( g ) * m ;  
//	case EDF_KEYNUFULL :
//	  double dl, d[ 5 ] ; GetScaleKeyNu( d, m_lp, m_Format ) ;
//	  for( i = 0 ; i < m_np ; i++ ) {
//	    dl = dt / d[ i ] ;
//	    m_f[ i ] = floor( dl ) ; 
//		dt = dt - m_f[ i ] * d[ i ] ;  
//	  }
//      break ;
	}
    // Знак
    if ( m_Format >= EDF_HMS && m_Format <= EDF_INTEGER && m_Sign ) {
      if ( m_NotShowSign ) m_s = m_Value < 0 ? -1 : 0 ;
      else m_s = m_Value < 0 ? -1 : 1 ;
    }
	// Округление
    double Delta ;
	//if ( m_Format != EDF_DMY && m_Format != EDF_FLOAT && m_Format != EDF_ALT && m_Format != EDF_NOTFORMAT && m_np > 1 && m_Dec > 0 ) {
	if ( m_Format >= EDF_DMYHMS && m_Format <= EDF_FIXED &&
    m_Format != EDF_FLOAT && m_np > 1 && m_Dec > 0 ) {
      Delta = dt - floor( dt ) ;
	  Delta = Delta * pow( 10.0, m_Dec ) + 0.5 ;
      m_f[ m_np - 1 ] = floor( Delta ) ;
	  double vmin, vmax ;
      FieldMaxMin ( m_np - 1, vmin, vmax ) ;
	  if ( m_f[ m_np - 1 ] > vmax ) {
        int EndField = m_Format != EDF_DMYHMS ? 0 : 3 ;
        m_f[ m_np - 1 ] = 0 ;
        k1 = m_np - 2 ;
   	    while ( k1 >= EndField ) {
		  FieldMaxMin ( k1, vmin, vmax ) ;
		  if ( m_f[ k1 ] + 1 <= vmax ) { m_f[ k1 ] ++ ; break ; }
          m_f[ k1 ] = 0 ; k1 -- ;
		}
		if ( k1 < EndField ) {
		  for ( k1 = EndField ; k1 < m_np ; k1 ++ ) {
            FieldMaxMin ( k1, vmin, vmax ) ; m_f[ k1 ] = vmax ;
		  }
		}
	  }
	  Delta = 0 ;
	}
}
//------------------------------------------------------------------------------
// Возможна ситуация когда заданое значение редактируемого параметра
// таково, что ширина первого поля не позволяет отобразить значение
// этого поля, соответствующие заданному значению
// В этом случае требуется автоматическая коррекци шаблона, а именнл:
//   - увеличение ширины первого поля до требуемого значения
//   - преобразование величины в интервальную
//------------------------------------------------------------------------------
void CFormatString::TestFildTamplete( int Field )
{
    if ( m_Format != EDF_FIXED ) return ;
    double f = fabs( m_f[ Field ] ) ;
    if ( f == 0 ) return ;
    int Len = ( int )( log10( f ) + 1 ) ;
    if ( Len <= m_lp[ Field ] )  return ;
    m_Interval = 1 ;
    if ( !m_fp[ Field ] ) goto end ;
    m_ip[ 1 ][ Field ] += Len - m_lp[ Field ] ;
	int i ;
    for( i = Field + 1 ; i < m_np ; i ++ ) {
      m_ip[ 0 ][ i ] += Len - m_lp[ Field ] ;
      m_ip[ 1 ][ i ] += Len - m_lp[ Field ] ;
    }
end : ;
    m_lp[ Field ] = Len ;
    if ( !Field ) m_Len = Len ;
}
//------------------------------------------------------------------------------
void CFormatString::BuildStringFromFields( bool TestRightZero )
{
	char c1, c2, c3, *s, r1[ 4 ], r2[ 4 ], r3[ 4 ], rs[ 11 ]= "          " ;
	int  i ;
    //
    c1 = m_Spr1 ; // Десятичный разделитель
    c2 = m_Spr2 ; // Обычный разделитель
    c3 = m_Spr2 ; // Обычный разделитель
    if ( c2 == 0 && isSysSetLoad ) {
      // Если задано использование системного разделителя для даты
      if ( m_Format == EDF_DMY ) c1 = c2 = SysSepDate ;
      // Если задано использование системного разделителя для времени
      if ( m_Format == EDF_HMS || m_Format == EDF_SSSHMS ) c2 = SysSepTime ;
      // Если задано использование системного разделителя для даты-времени
      if ( m_Format == EDF_DMYHMS ) { c2 = SysSepDate ; c3 = SysSepTime ; }
      // Если задано использование системного разделителя для списка ключей НУ
      if ( m_Format == EDF_KEYNU ) c2 = SysSepList ;
      // Если задано использование системного разделителя для списка ключей
      if ( m_Format == EDF_CUSTOMKEY ) c2 = SysSepList ;
    }
    // Если задано использование системного десятичного разделителя
    if ( c1 == 0 && isSysSetLoad ) c1 = SysSepDec ;
    //
    if ( m_Format == EDF_DMY || m_Format == EDF_KEYNU ||
    m_Format == EDF_KEYNUFULL || m_Format == EDF_CUSTOMKEY ) {
      c1 = c2 ;
    }
    //
    if ( c1 == 0 ) c1 = ',' ;
    if ( c2 == 0 ) c2 = '.' ;
    if ( c3 == 0 ) c3 = '.' ;
    //
	if ( m_Space ) {
	  sprintf( r1, " %c \0", c1 ) ;
	  sprintf( r2, " %c \0", c2 ) ;
	  sprintf( r3, " %c \0", c3 ) ;
	} else {
	  sprintf( r1, "%c\0", c1 ) ;
	  sprintf( r2, "%c\0", c2 ) ;
	  sprintf( r3, "%c\0", c3 ) ;
	}
    int idf = GetDecedField( m_Format, m_Dec ) ;
	if ( m_Format != EDF_FLOAT ) {
      TestFildTamplete( 0 ) ;
	  for ( i = 0 ; i < m_np ; i ++ ) {
        // Обычный разделитель
        s = r2 ;
        if ( m_Format == EDF_DMYHMS ) s = i < 2 ? r2 : r3 ;
        // Десятичнвй разделитель
        if ( i == idf - 1 ) s = r1 ;
        // Разделитель между типами данных (например датой и временем)
        if ( ( i == 0 && ( ( m_Format >= EDF_VITGMS && m_Format <= EDF_VITRAD )|| m_Format == EDF_SSSHMS ) ) ||
        ( i == 2 && m_Format == EDF_DMYHMS ) ) {
          rs[ m_ip[ 1 ][ i + 1 ] - m_ip[ 1 ][ i ] ] = '\0' ;
          s = rs ;
        }
        // Последнее поле без разделителя
        if ( i == m_np - 1 ) s = 0 ;
        BuildSubString( i, s, TestRightZero ) ;
	  }
	} else {
      sprintf( r2, "%s\0", ( m_Space ?  " e+" : "e+" ) ) ;
      BuildSubString( 0, r1 ) ;
      BuildSubString( 1, r2 ) ;
      BuildSubString( 2, 0  ) ;
	  if ( m_f[ 2 ] < 0 )  m_Text[  m_ip[ 0 ][ 2 ] - 1 ] = '-' ;
	}
    m_Text[ m_ip[ 1 ][ m_np - 1 ] ] = '\0' ;
}
//------------------------------------------------------------------------------
bool CFormatString::BuildFieldsFromString ( char* str, int* dlc, bool TestLeftZero )
{
    int   i, j, k, l, dl, li ;
	char  s[ 128 ] ;
    char *ss ;
	bool  sign ;
    //
    if ( !str ) {
      GetEditText( ) ;
      str = m_Text ;
    }
    //
    BuildTamplete( ) ;
    memset( m_f, 0, 7 * sizeof( double ) ) ;
    m_Value = 0 ;
    m_s = 0 ;
    //
    if ( m_Format >= EDF_YESNO && m_Format <= EDF_CHECKYN ) {
      if ( strcmp( str, "Да" ) == 0 || strcmp( str, "Вкл" ) == 0 ||
      strcmp( str, "1" ) == 0 || strcmp( str, " " ) == 0 ) m_f[ 0 ] = m_Value = 1 ;
      else m_f[ 0 ] = m_Value = 0 ;
      return 1 ;
    }
    if ( m_Format == EDF_CHECKCOLOR ) {
      m_f[ 0 ] = m_Value = 0 ;
      if ( strcmp( str, " " )  == 0 ) m_f[ 0 ] = m_Value = 1 ;
      if ( strcmp( str, "  " ) == 0 ) m_f[ 0 ] = m_Value = 2 ;
      return 1 ;
    }
    //
	l = strlen( str ) ; if ( l == 0 )  return 0 ;
    //
    if ( str[ 0 ] == '-' || str[ 0 ] == '+' ) {
      if ( m_Sign && ( str[ 0 ] == '-' || ( str[ 0 ] == '+' && !m_NotShowSign ) ) ) {
        ChangeIP( -1, 1 ) ;
        m_s = str[ 0 ] == '-' ? -1 : 1 ;
      }
      if ( !m_Sign || ( str[ 0 ] == '+' && m_NotShowSign ) ) {
        str++ ;
        l-- ;
        m_s = 0 ;
      }
    }
    //
	j = 0 ;
    if ( l <= 0 ) goto end ;
	for ( i = 0 ; i < m_np ; i++ ) {
	  sign = 0 ;
	  while ( str[ j ] != '0' && str[ j ] != '1' && str[ j ] != '2' && str[ j ] != '3' && str[ j ] != '4' &&
	  str[ j ] != '5' && str[ j ] != '6' && str[ j ] != '7' && str[ j ] != '8' && str[ j ] != '9' ) {
	    if ( j == l ) goto end ;
        if ( str[ j ] == '-' ) sign = 1 ;
		j++ ;
	  }
	  k = 0 ;
	  while ( str[ j ] == '0' || str[ j ] == '1' || str[ j ] == '2' || str[ j ] == '3' || str[ j ] == '4' ||
	  str[ j ] == '5' || str[ j ] == '6' || str[ j ] == '7' || str[ j ] == '8' || str[ j ] == '9' ) {
        s[ k ] =  str[ j ] ;
		j++ ; k++ ;
        // Если ширина поля больше максимально допустимой
		if ( k == m_lp[ i ] + 1 ) {
          // Если ситуация возникла для последнего поля
          // то просто игнорируем оставшиеся знаки
          if ( i == m_np - 1 ) break ;
          // в противном случае строка имеет неверный формат
		  m_BuildCode =-3 ;
          return 0 ;
        }
	  }
      s[ k ] = '\0' ;
      if ( m_Format >= EDF_DMYHMS && m_Format <= EDF_FIXED ) {
        if ( m_Format == EDF_FLOAT ) {
          if ( i == 1 && m_Dec < ( int )strlen( s ) ) s[ m_Dec ] = '\0' ;
        }
        //else {
        //if ( i == m_np - 1 && m_Dec < ( int )strlen( s ) ) s[ m_Dec ] = '\0' ;
        //}
      }
      // Подавление левых нулей
      if ( TestLeftZero ) {
        char st[ 128 ] ;
        sprintf( st, "%s\0", s ) ; ss = st ;
        if ( !m_f[ i ] && i != GetDecedField( m_Format, m_Dec ) ) {
          while( ss[ 0 ] == '0' ) { ss++ ; if ( strlen( ss ) == 0 ) break ; }
        }
        if ( strlen( ss ) == 0 ) sprintf( s, "0\0" ) ;
        else sprintf( s, "%s\0", ss ) ;
      }  
	  if ( !m_fp[ i ] ) {
        li  = m_ip[ 1 ][ i ] -  m_ip[ 0 ][ i ] ;
        dl = strlen( s ) - li ;
        if ( dl + li > m_lp[ i ] ) dl = m_lp[ i ] - li ;
        if ( dl + li < 0         ) dl =-li ;
        if ( dl > 0 ) ChangeIP( i, dl ) ;
	  }
	  ss = s ;
      // Подавление левых нулей
      while( ss[ 0 ] == '0' ) { ss++ ; if ( strlen( ss ) == 0 ) break ; }
      if ( strlen( ss ) == 0 ) { m_f[ i ] = 0 ; continue ; }
      if ( sscanf( ss, "%lf", &m_f[ i ] ) != 1 ) return 0 ;
      if ( m_Format >= EDF_DMYHMS && m_Format <= EDF_FIXED ) {
        if ( m_Format == EDF_FLOAT ) {
	      if ( sign == 1 && i == 2 ) m_f[ i ] =-m_f[ i ] ;
          if ( i == 1 && m_lp[ 1 ] > ( m_ip[ 1 ][ 1 ] -  m_ip[ 0 ][ 1 ] ) ) {
            m_f[ 1 ] *= pow( 10.0, m_lp[ 1 ] - ( m_ip[ 1 ][ 1 ] -  m_ip[ 0 ][ 1 ] ) ) ;
          }
        } else {
          if ( i == m_np - 1 && m_lp[ i ] > ( m_ip[ 1 ][ i ] -  m_ip[ 0 ][ i ] ) ) {
            m_f[ i ] *= pow( 10.0, m_lp[ i ] - ( m_ip[ 1 ][ i ] -  m_ip[ 0 ][ i ] ) ) ;
          }
        }
      }
	  if ( j == l ) goto end ;
	}
end : ;
    if ( dlc ) *dlc = j ;
    if ( j == l && i < m_np - 1 ) {
      if ( i == m_np - 2 && m_Format >= EDF_DMYHMS && m_Format <= EDF_FIXED
      && m_Format != EDF_FLOAT ) {
        return 1 ;
      }
      if ( ( i == m_np - 3 || i == m_np - 2 ) && m_Format == EDF_FLOAT ) {
        return 1 ;
      }
      return 0 ;
    }
	return 1 ;
}
//------------------------------------------------------------------------------
bool CFormatString::BuildFromString( char* text, int* dl, bool TestLeftZero, bool TestRightZero, bool isNotSetText )
{
	bool   ret ;
	double vmax, vmin ;
	int    i, rc ;
    //
    m_bisNeedSetText = 0 ;
    if ( !text ) {
      GetEditText( ) ;
      text = m_Text ;
    }
    //
    if ( !text || strlen( text ) == 0 ) {
      m_Value  = 0 ;
      m_f[ 0 ] = 0 ;
      BuildFromData( ) ;
      return true ;
    }
    //
    if ( m_Format == EDF_NOTFORMAT ) return false ;
    if ( m_Format == EDF_ALT    ) {
      for ( i = 0 ; i < m_AltCount ; i++ ) {
        if ( strcmp( text, m_AltString[ i ] ) == 0 ) break ;
      }
      if ( i < m_AltCount ) {
        sprintf( m_Text, "%s\0", m_AltString[ i ] ) ;
        m_Value = m_f[ 0 ] = i ;
        return true ;
      }
      i =-1 ;
      if ( sscanf( text, "%d", &i ) == 1 ) {
        if ( i < 0 || i >= m_AltCount ) i =-1 ;
      }
      if ( i < 0 ) {
        sprintf( m_Text, "%s\0", m_AltString[ 0 ] ) ;
        m_Value = m_fp[ 0 ] = 0 ;
        return false ;
      } else {
        sprintf( m_Text, "%s\0", m_AltString[ i ] ) ;
        m_Value = m_fp[ 0 ] = i ? true : false ;
        return true ;
      }
    }
    if ( m_Format >= EDF_YESNO && m_Format <= EDF_CHECKYN ) {
      if ( strcmp( text, "Да" ) == 0 || strcmp( text, "Вкл" ) == 0 ||
      strcmp( text, "1" ) == 0 || strcmp( text, " " ) == 0 ) m_f[ 0 ] = m_Value = 1 ;
      else m_f[ 0 ] = m_Value = 0 ;
      return true ;
    }
    if ( m_Format == EDF_CHECKCOLOR ) {
      m_f[ 0 ] = m_Value = 0 ;
      if ( strcmp( text, " " )  == 0 || strcmp( text, "1" )  == 0 ) m_f[ 0 ] = m_Value = 1 ;
      if ( strcmp( text, "  " ) == 0 || strcmp( text, "2" )  == 0 ) m_f[ 0 ] = m_Value = 2 ;
      return true ;
    }
    //
    ret = BuildFieldsFromString( text, dl, TestLeftZero ) ; if ( !ret ) goto m_end ;
	for ( i = 0 ; i < m_np ; i ++ ) {
      if ( FieldMaxMin ( i, vmin, vmax ) ) {
	    if ( fabs( m_f[ i ] ) < vmin || fabs( m_f[ i ] ) > vmax ) { m_BuildCode = i + 1 ; goto m_end ; }
	  }
	}
    BuildStringFromFields( TestRightZero ) ;
    //
    int inMaxMin ;
    rc = BuildValueFromFields( inMaxMin ) ;
    if ( !rc || inMaxMin ) m_Value = 0 ;
    //
    if ( m_bisNeedSetText && !isNotSetText ) {
      GetPos( m_Pos ) ;
      SetEditText( ) ;
      SetPos( m_Pos ) ;
      m_bisNeedSetText = 0 ;
    }
	return true ;
m_end : ;
    return false ;
}
//------------------------------------------------------------------------------
bool CFormatString::FieldMaxMin ( int i, double& vmin, double& vmax )
{
    switch ( m_Format ) {
	case EDF_DMY :
		if ( i == 0 ) { vmin = 1 ; vmax = 31 ; }
		if ( i == 1 ) { vmin = 1 ; vmax = 12 ; }
		if ( i == 2 ) { vmin = 0 ; vmax = 3000 ; }
		if ( i > 2 ) return false ;
		break ;
	case EDF_DMYHMS :
        if ( i == 0 ) { vmin = 1 ; vmax = 31 ; }
		if ( i == 1 ) { vmin = 1 ; vmax = 12 ; }
		if ( i == 2 ) { vmin = 0 ; vmax = 3000 ; }
		if ( i == 3 ) { vmin = 0 ; vmax = 23 ; }
		if ( i == 4 ) { vmin = 0 ; vmax = 59 ; }
		if ( i == 5 ) { vmin = 0 ; vmax = 59 ; }
		if ( i == 6 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 6 ) return false ;
		break ;
	case EDF_HMS :
		if ( i == 0 ) {
			 vmin = 0 ;
		     if (!m_Interval )
			      vmax = 23 ;
 		     else vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = 59 ; }
		if ( i == 2 ) { vmin = 0 ; vmax = 59 ; }
		if ( i == 3 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 3 ) return false ;
		break ;
	case EDF_SSSHMS :
		if ( i == 0 ) {
		  vmin = 0 ;
 		  vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = 23 ; }
		if ( i == 2 ) { vmin = 0 ; vmax = 59 ; }
		if ( i == 3 ) { vmin = 0 ; vmax = 59 ; }
		if ( i == 4 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 4 ) return false ;
		break ;
	case EDF_MILLIS :
		if ( i == 0 ) {
		  vmin = 0 ;
		  if (!m_Interval ) vmax = 86399999.0 ;
		  else vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 1 ) return false ;
		break ;
	case EDF_SEC :
		if ( i == 0 ) {
		  vmin = 0 ;
		  if (!m_Interval ) vmax = 86399.0 ;
		  else vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 1 ) return false ;
		break ;
	case EDF_MIN :
		if ( i == 0 ) {
		  vmin = 0 ;
		  if (!m_Interval )vmax = 1439.0 ;
		  else vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 0 ) return false ;
		break ;
	case EDF_HOUR :
		if ( i == 0 ) {
		  vmin = 0 ;
		  if (!m_Interval ) vmax = 23.0 ;
		  else vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 0 ) return false ;
		break ;
	case EDF_SSS :
		if ( i == 0 ) {
		  vmin = 0 ;
		  if (!m_Interval )vmax = 0.0 ;
		  else vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 0 ) return false ;
		break ;
	case EDF_GMS :
		if ( i == 0 ) {
		  vmin = 0 ;
		  if (!m_Interval ) vmax = 359.0 ;
		  else vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = 59 ; }
		if ( i == 2 ) { vmin = 0 ; vmax = 59 ; }
		if ( i == 3 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 3 ) return false ;
		break ;
	case EDF_VITGMS :
		if ( i == 0 ) {
		  vmin = 0 ;
		  vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = 359 ; }
		if ( i == 2 ) { vmin = 0 ; vmax = 59 ; }
		if ( i == 3 ) { vmin = 0 ; vmax = 59 ; }
		if ( i == 4 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 4 ) return false ;
		break ;
	case EDF_VITGRAD :
		if ( i == 0 ) {
		  vmin = 0 ;
		  vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = 359 ; }
		if ( i == 2 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 2  ) return false ;
		break ;
	case EDF_VITRAD :
		if ( i == 0 ) {
		  vmin = 0 ;
		  vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = 6 ; }
		if ( i == 2 ) {vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 2  ) return false ;
		break ;
	case EDF_GRAD :
		if ( i == 0 ) {
		  vmin = 0 ;
		  if (!m_Interval ) vmax = 359 ;
		  else vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 1 ) return false ;
		break ;
	case EDF_RAD :
		if ( i == 0 ) {
		  vmin = 0 ;
		  if (!m_Interval ) vmax = 6.0 ;
		  else vmax = pow( 10.0, m_Len ) - 1 ;
		}
		if ( i == 1 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 0 ) return false ;
		break ;
    case EDF_INTEGER :
		vmin = 0 ; vmax = pow( 10.0, m_Len ) - 1 ;
		if ( i > 0 ) return false ;
        break ;
    case EDF_FIXED :
		if ( i == 0 ) { vmin = 0 ; vmax = pow( 10.0, m_Len ) - 1 ; }
		if ( i == 1 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i > 1 ) return false ;
        break ;
    case EDF_FLOAT :
		if ( i == 0 ) { vmin = 0 ; vmax = pow( 10.0, m_Len ) - 1 ; }
		if ( i == 1 ) { vmin = 0 ; vmax = pow( 10.0, m_Dec ) - 1 ; }
		if ( i == 2 ) { vmin = 0 ; vmax = 99 ; }
		if ( i > 2 ) return false ;
        break ;
	default :
		return false ;
	}
	return true ;
}
//------------------------------------------------------------------------------
void CalcDecFromFormat( int oldFormat, int oldLen, int oldDec, int Interval, int newFormat, int& newLen, int& newDec )
{
    //
    if ( oldFormat >= EDF_HMS && oldFormat <= EDF_SSS && newFormat >= EDF_HMS && newFormat <= EDF_SSS ) goto TimeAng ;
    if ( oldFormat >= EDF_GMS && oldFormat <= EDF_RAD && newFormat >= EDF_GMS && newFormat <= EDF_RAD ) goto TimeAng ;
    if ( oldFormat >= EDF_VITGMS && oldFormat <= EDF_VITRAD && newFormat >= EDF_VITGMS && newFormat <= EDF_VITRAD ) goto TimeAng ;
    if ( ( oldFormat == EDF_FIXED && newFormat == EDF_FLOAT ) || ( oldFormat == EDF_FLOAT && newFormat == EDF_FIXED ) ) goto FixedFloat ;
    return ;
FixedFloat : ;
    newDec = oldDec ;
    newLen = newLen <= 16 ? newLen : 16 ;
    if ( newDec + newLen > 16 ) newDec = 16 - newLen < 0 ? 0 : 16 - newLen ;
    return ;
    //
TimeAng : ;
    double m_eps0 = pow( 10.0,-oldDec ) ;
    double m_lm   = pow( 10.0, oldLen ) - 1 ;
    double rgr    = 180.0 / acos(-1.0 ) ;
    //
    switch( oldFormat )  {
    case EDF_HMS :    m_eps0*= 1.0 ;     m_lm*= 3600.0 ;  break ;
    case EDF_SSSHMS : m_eps0*= 1.0 ;     m_lm*= 86400.0 ; break ;
    case EDF_MILLIS : m_eps0/= 1000.0 ;  m_lm/= 1000.0 ;  break ;
    case EDF_SEC :    m_eps0*= 1.0 ;     m_lm*= 1.0 ;     break ;
    case EDF_MIN :    m_eps0*= 60 ;      m_lm*= 60.0 ;    break ;
    case EDF_HOUR :   m_eps0*= 3600.0 ;  m_lm*= 3600.0 ;  break ;
    case EDF_SSS :    m_eps0*= 86400.0 ; m_lm*= 86400.0 ; break ;
    //
    case EDF_VITGMS :
    case EDF_GMS :    m_eps0/= 3600.0 ;  m_lm*= 1.0 ;     break ;
    case EDF_VITGRAD :
    case EDF_GRAD :   m_eps0*= 1.0 ;     m_lm*= 1.0 ;     break ;
    case EDF_VITRAD :
    case EDF_RAD :    m_eps0*= rgr ;     m_lm*= rgr ;     break ;
    }
    //
	double np1 ;
	double np2 ;
	double ne ;
	double nl ;
    //
    switch( newFormat ) {
    case EDF_HMS :    ne = m_eps0 ;           nl = m_lm / 3600.0 ;  break ;
	case EDF_SSSHMS : ne = m_eps0 ;           nl = m_lm / 86400.0 ; break ;
	case EDF_MILLIS : ne = m_eps0 * 1000.0 ;  nl = m_lm * 1000.0 ;  break ;
	case EDF_SEC :    ne = m_eps0 ;           nl = m_lm ;		    break ;
	case EDF_MIN :    ne = m_eps0 / 60.0 ;    nl = m_lm / 60.00 ;   break ;
	case EDF_HOUR :   ne = m_eps0 / 3600.0 ;  nl = m_lm / 3600.0 ;  break ;
	case EDF_SSS :    ne = m_eps0 / 86400.0 ; nl = m_lm / 86400.0 ; break ;
    //
    case EDF_VITGMS :
	case EDF_GMS :    ne = m_eps0 * 3600.0 ;  nl = m_lm ;	        break ;
    case EDF_VITGRAD :
	case EDF_GRAD :   ne = m_eps0 ;		      nl = m_lm ;		    break ;
    case EDF_VITRAD :
	case EDF_RAD :    ne = m_eps0 / rgr ;     nl = m_lm / rgr ;     break ;
    }
    np1 = -log10( ne ) ; np1+= 0.5 ;
    if ( np1 < 0 ) np1 = 0 ;
    //if ( Interval )
	np2 = log10( nl ) ; np2+= 0.5 ;
    if ( np2 < 0 ) np2 = 0 ;
	np1 = ( int )np1	;
	np2 = ( int )np2	;
	if ( np1 + np2 > 15 ) { np1 = 15 - np2 ; if ( np1 < 0 ) np1 = 0 ; }
	newDec = ( int )np1 ;
	newLen = ( int )np2 ;
}
//------------------------------------------------------------------------------
bool CFormatString::SetFormat( int Format, bool isUpdate )
{
    if ( m_Format == Format ) return 0 ;
    int OldTypeFormat = GetTypeFormat( ) ;
    int NewTypeFormat = GetTypeFormat( Format ) ;
    if ( OldTypeFormat != NewTypeFormat ) return 1 ;
    m_Format = Format ;
    if (  isUpdate ) BuildFromData( ) ;
    return 0 ;
}
//------------------------------------------------------------------------------
int CFormatString::GetFormat( ) { return m_Format ; }
//------------------------------------------------------------------------------
bool CFormatString::SetFormatIO( int FormatIO )
{
    if ( m_FormatIO == FormatIO ) return 0 ;
    if ( FormatIO == 0 ) { SetFormatIODefault( ) ; return 0 ; }
    int OldTypeFormat = GetTypeFormat( m_FormatIO ) ;
    int NewTypeFormat = GetTypeFormat( FormatIO ) ;
    if ( OldTypeFormat != NewTypeFormat ) return 1 ;
    m_FormatIO = FormatIO ;
    if ( m_FormatIO == 0 ) SetFormatIODefault( ) ;
    return 0 ;
}
//------------------------------------------------------------------------------
bool CFormatString::SetFormatIODefault(  )
{
    switch ( m_Format ) {
    case EDF_DMY :
    case EDF_DMYHMS :
    case EDF_SSSHMS :
      m_FormatIO = EDF_SSS ; break ;
    case EDF_HMS :
    case EDF_MILLIS :
    case EDF_SEC :
    case EDF_MIN :
    case EDF_HOUR :
    case EDF_SSS :
      m_FormatIO = EDF_SEC ; break ;
    case EDF_GMS :
    case EDF_GRAD :
    case EDF_RAD :
      m_FormatIO = EDF_GRAD ; break ;
    case EDF_VITGMS :
    case EDF_VITGRAD :
    case EDF_VITRAD :
      m_FormatIO = EDF_VITGRAD ; break ;
    case EDF_FLOAT :
    case EDF_FIXED :
    case EDF_INTEGER :
    case EDF_YESNO :
    case EDF_INSERT :
    case EDF_01 :
    case EDF_CHECK :
    case EDF_CHECKYN :
    case EDF_CHECKCOLOR :
      m_FormatIO = EDF_NOTFORMAT ; break ;
    }
    return 0 ;
}
//------------------------------------------------------------------------------
int CFormatString::GetFormatIO( )
{
    return m_FormatIO ;
}
//------------------------------------------------------------------------------
void CFormatString::ValiPos( int f, int pos )
{
    int  i ;
    if ( pos == -1 ) { if ( GetPos ( pos ) > 0 ) return ; }
    i = GetFild( pos ) ;
    if ( i == -1 ) {
      if ( f ) { SetPos( m_Pos ) ; return ; }
      if ( pos >  0 ) {
        i = GetDFild( pos ) ;
        if ( pos < m_Pos ) pos = m_ip[ 1 ][ i ] ;
        else               pos = m_ip[ 0 ][ i + 1 ] ;
        SetPos( pos ) ;
      }
    }
    m_Pos = pos ;
}
//------------------------------------------------------------------------------
int CFormatString::GetFild( int pos  )
{
    int i ;
    for ( i = 0 ; i < m_np ; i ++ ) {
      if ( pos >= m_ip[ 0 ][ i ] && pos <= m_ip[ 1 ][ i ] ) break ;
    }
    if ( i < m_np ) return i ;
    else            return -1 ;
}
//------------------------------------------------------------------------------
int CFormatString::GetDFild( int pos )
{
    int i ;
    for ( i = 0 ; i < m_np - 1 ; i ++ ) {
      if ( pos > m_ip[ 1 ][ i ] && pos < m_ip[ 0 ][ i + 1 ] ) break ;
    }
    if ( i < m_np - 1 ) return i ;
    else return -1 ;
}
//------------------------------------------------------------------------------
int CFormatString::GetPos ( int& pos  )
{   int start, end ; GetEditSel( start, end ) ; pos = start ; return ( end - start ) ; }
//------------------------------------------------------------------------------
void CFormatString::SetPos ( int pos  )
{   m_Pos = pos ;  SetEditSel( pos, pos ) ; }
//------------------------------------------------------------------------------
bool CFormatString::IsValiValue( int& Field, int& inMaxMin, int Flag )
{
    if ( m_Format == EDF_NOTFORMAT ) return 1 ;
    //
	GetEditText( ) ;
    BuildFieldsFromString( m_Text, 0, ( Flag & EDF_FTEST_KILLLEFTZ ) != 0 ) ;
	//
    inMaxMin = 0 ;
	bool isVali = IsValiField( Field ) ;
	if ( isVali ) {
	  Field =-1 ;
	  isVali = BuildValue( inMaxMin, 0, Flag ) ;
	}
    if ( !isVali && ( Flag & EDF_FTEST_SETFOCUS ) ) {
      SetEditFocus( ) ;
      if ( Field < 0 || m_Format == EDF_DMY || m_Format == EDF_DMYHMS ) SetEditSel( 0, strlen( m_Text ) ) ;
      else SetEditSel( m_ip[ 0 ][ Field ], m_ip[ 1 ][ Field ] ) ;
    }
    if ( !isVali && ( Flag & EDF_FTEST_BEEP ) ) Beep( ) ;
    return isVali ;
}
//------------------------------------------------------------------------------
int CFormatString::IsValiMaxMin( ) 
{
	CRGradTime gt ;
	double VMM = m_Value ;
	int    inMaxMin = 0 ;
	//
    switch( m_Format ) {
	  case EDF_DMY : 
	  case EDF_DMYHMS :
      case EDF_VITGMS : 
      case EDF_VITGRAD : 
      case EDF_VITRAD : 
	  case EDF_INTEGER :
	  case EDF_01 :
	  case EDF_YESNO :
	  case EDF_INSERT :
	  case EDF_CHECK :
	  case EDF_CHECKYN :
      case EDF_CHECKCOLOR :
	  case EDF_FIXED :
	  case EDF_FLOAT :
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        }
		break ;
	  case EDF_SSSHMS : 
	  case EDF_HMS :
      case EDF_GMS :
      case EDF_MILLIS :
      case EDF_SEC :
	  case EDF_MIN :
	  case EDF_HOUR :
	  case EDF_SSS :
      case EDF_GRAD :
	  case EDF_RAD : 
		gt.fromDouble( VMM, m_FormatIO ) ;
        VMM = gt.toDouble( m_FormatMM ) ;
        if ( m_Max >= m_Min ) {
          if ( VMM < m_Min ) inMaxMin = 1 ;
          if ( VMM > m_Max ) inMaxMin = 2 ;
        } 
        break ;
	}
	return inMaxMin ;
}

//------------------------------------------------------------------------------
bool CFormatString::IsValiField( int& Field )
{
    if ( m_Format == EDF_NOTFORMAT ) return 1 ;
    //
    Field = 0 ;
    CRDate dmy ;
    bool isVali = 1 ;
	//
    switch ( m_Format ) {
	case EDF_DMY :
	  isVali = IsValiDate((int)fabs(m_f[0]), (int)m_f[1], (int)m_f[2], &Field) ; 
	  break ;
	case EDF_DMYHMS :
	  isVali = IsValiDate((int)fabs(m_f[0]), (int)m_f[1], (int)m_f[2], &Field ) ; if ( !isVali ) break ;
	  isVali = IsValiTime((int)fabs(m_f[3]), (int)m_f[4], (int)m_f[5], &Field) ; if ( !isVali ) Field += 3 ;
	  break ;
	case EDF_SSSHMS :
	  isVali = IsValiTime((int)fabs(m_f[1]), (int)m_f[2], (int)m_f[3], &Field) ; if ( !isVali ) Field += 1 ; 
	  break ;
	case EDF_HMS :
	  isVali = IsValiTime((int)fabs(m_f[0]), (int)m_f[1], (int)m_f[2], &Field, m_Interval) ; 
	  break ;
	case EDF_SSS :
	  if (m_Interval) break ;
      if (fabs(m_f[0])<0 || fabs(m_f[0])>0) { Field = 0 ; isVali = 0 ; }
	  break ;
	case EDF_HOUR :
	  if (m_Interval) break ;
      if (fabs(m_f[0])<0 || fabs(m_f[0])>23) { Field = 0 ; isVali = 0 ; }
	  break ;
	case EDF_MIN :
	  if (m_Interval) break ;
	  if (fabs(m_f[0])<0 || fabs(m_f[0])>1439) { Field = 0 ; isVali = 0 ; }
	  break ;
	case EDF_SEC :
	  if (m_Interval) break ;
      if (fabs(m_f[0])<0 || fabs(m_f[0])>86399) { Field = 0 ; isVali = 0 ; }
	  break ;
	case EDF_MILLIS :
	  if (m_Interval ) break ;
      if (fabs(m_f[0])<0 || fabs(m_f[0])>86399999.) { Field = 0 ; isVali = 0 ; }
	  break ;
	case EDF_GMS :
	  isVali = IsValiAngle((int)fabs(m_f[0]), (int)m_f[1], m_f[2], &Field, m_Interval) ; 
	  break ;
	case EDF_GRAD :
	  if ( m_Interval ) break ;
      if ( fabs( m_f[ 0 ] ) <  0 || fabs( m_f[ 0 ] ) > 359 ) { Field = 0 ; isVali = 0 ; }
	  break ;
	case EDF_RAD :
	  if ( m_Interval ) break ;
	  if ( fabs(m_f[0])> 6.0) { Field = 0 ; isVali = 0 ; break ; }
      if ( fabs(m_f[0])==6.0 && m_f[1]>(2*acos(-1.0)-6.0)*pow(10.0, m_lp[1])) { Field = 1 ; isVali = 0 ; }
	  break ;
	case EDF_VITGMS :
	  isVali = IsValiAngle((int)fabs(m_f[1]), (int)m_f[2], m_f[3], &Field ) ; if ( !isVali ) Field += 1 ;  
	  break ;
	case EDF_VITGRAD :
      if (m_f[1]< 0 || m_f[1]>359) { Field = 1 ; isVali = 0 ; }
	  break ;
	case EDF_VITRAD :
	  if (m_f[1]< 0 || m_f[1]>6) { Field = 1 ; isVali = 0 ; break ; }
      if (m_f[1]==6 && m_f[2]>(2*acos(-1.0)-6.0)*pow(10.0, m_lp[2])) { Field = 2 ; isVali = 0 ; }
	  break ;
	}
	return isVali ;
}
//------------------------------------------------------------------------------
bool  CFormatString::Checking( int* pField, int* pinMaxMin ) 
{
	int Field, inMaxMin ;
	bool rc = IsValiValue( Field, inMaxMin, 1 | 2 | 4 | 8 ) ;
	if ( pField    ) *pField    = Field ;
	if ( pinMaxMin ) *pinMaxMin = inMaxMin ;
	m_CheckingCode = 0 ;
	if ( Field    >= 0 ) m_CheckingCode = Field ;
	if ( inMaxMin >  0 ) m_CheckingCode =-inMaxMin ;
	return rc ;
}
//------------------------------------------------------------------------------
bool CFormatString::CheckingFields( )
{
    int Field = 0, inMaxMIn = 0 ;
	if ( IsValiValue( Field, inMaxMIn, 0 ) || inMaxMIn ) return 0 ;
    if ( m_Format == EDF_DMY || m_Format == EDF_DMYHMS ) SetEditSel( 0, strlen( m_Text ) ) ;
    else SetEditSel( m_ip[ 0 ][ Field ], m_ip[ 1 ][ Field ] ) ;
    return 1 ;
}
//------------------------------------------------------------------------------
bool CFormatString::CheckingMaxMin( )
{
    int Field = 0, inMaxMIn = 0 ;
	if ( IsValiValue( Field, inMaxMIn, 0 ) || inMaxMIn <= 0 ) return 0 ;
    SetEditSel( 0, strlen( m_Text ) ) ;
    return 1 ;
}
//------------------------------------------------------------------------------
bool CFormatString::CheckingFieldsInEditTime(  bool isFast )
{
	if ( m_TestValiVar != EDF_TESTVALIVAR_EDIT || m_Format == EDF_DMY || m_Format == EDF_DMYHMS ) return 0 ;
    if ( GetPos( m_Pos ) && !isFast ) return 0 ;
    m_NewField = GetFild( m_Pos  ) ;
	bool r = ( ( m_OldField < 0 || m_OldField == m_NewField ) && !isFast ) ? 0 : CheckingFields( ) ;
	GetPos( m_Pos ) ;
	m_OldField = GetFild( m_Pos  ) ;
    return r ;
}
//------------------------------------------------------------------------------
bool CFormatString::NextPrevField ( bool np )
{
    if ( m_Format == EDF_NOTFORMAT ) return false ;
    int fild ;
	GetPos ( m_Pos ) ;
	if ( np ) {
	  fild = GetFild( m_Pos ) + 1 ;
      if ( fild  >= m_np ) fild = 0 ;
	} else {
	  fild = GetFild( m_Pos ) - 1 ;
      if ( fild  < 0 ) fild = m_np - 1 ;
	}
	m_Pos = m_ip[ 0 ][ fild ] ;
	SetEditSel( m_ip[ 0 ][ fild ], m_ip[ 1 ][ fild ] ) ;
	return true ;
}
//------------------------------------------------------------------------------
void CFormatString::SetFild ( double value )
{
	int    j, i, pos, l1, l2 ;
	char   str[ 20 ], text[ 256 ] ;
	double vmin, vmax ;

	GetPos( pos ) ;
	i = GetFild( pos ) ;
    FieldMaxMin ( i, vmin, vmax ) ;
	if ( ( m_f[ i ] == vmin && value <= vmin ) || ( m_f[ i ] == vmax && value >= vmax ) ) {
	  Beep( ) ; return ;
	}
	if ( value < vmin ) value = vmin ;
	if ( value > vmax ) value = vmax ;
	if ( i <  0 ) {
	  if ( pos == 0 && ( m_Text[ 0 ] == '-' || m_Text[ 0 ] == '+' ) ) i = 0 ;
      else return ;
	}
	if ( m_Format == EDF_MILLIS || m_Format == EDF_SEC || m_Format == EDF_MIN ||
	  m_Format == EDF_HOUR || m_Format == EDF_SSS || m_Format == EDF_GRAD || m_Format == EDF_RAD ) {
	  if ( i == 1 ) i = 0 ;
	} else {
	  if ( m_Format == EDF_DMYHMS ) {
	    if ( i == 6 ) i = 5 ;
	  } else {
	    if ( m_Format == EDF_SSSHMS ) {
		  if ( i == 4 ) i = 3 ;
		} else {
		  if ( i == 3 ) i = 2 ;
		}
	  }
	}
	sprintf( str, "%.0lf\0", value ) ;
	l1 = strlen( str ) ;
	if ( !m_fp[ 0 ] ) {
	  l2 =  m_ip[ 1 ][ i ] - m_ip[ 0 ][ i ] ;
	  if ( l1 != l2 ) {
		memcpy( text, m_Text, m_ip[ 0 ][ i ] ) ;
		memcpy(&text[ m_ip[ 0 ][ i ] + l1 ], &m_Text[ m_ip[ 1 ][ i ] ], m_ip[ 1 ][ m_np - 1 ] - m_ip[ 1 ][ i ] + 1 ) ;
        ChangeIP( i, l1 - l2 ) ;
		memcpy( m_Text, text, m_ip[ 1 ][ m_np -1 ] + 1 ) ;
	  }
	  l2 = l1 ;
	}
	else l2 = m_lp[ i ] ;
	for ( j = l2 - l1 ; j < l2 ; j ++ ) m_Text[ m_ip[ 0 ][ i ] + j ] = str[ j - ( l2 - l1 ) ] ;
	for ( j = 0 ; j < l2 - l1 ;  j ++ ) m_Text[ m_ip[ 0 ][ i ] + j ] = '0' ;
	SetEditText( ) ;
	SetEditSel( m_ip[ 0 ][ i ], m_ip[ 1 ][ i ] ) ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedNumber( FormatProperty& p )
{   FormatedFixed( p ) ; }
//------------------------------------------------------------------------------
void CFormatString::FormatedFixed( FormatProperty& p )
{
    p.Value          = 0 ;
    p.Dec            = 6 ;
    p.Len            = 10 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_FIXED ;
    p.FormatIO       = EDF_NOTFORMAT ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = EDF_PROPERTY_NOTSET ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 1 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedFloat( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 15 ;
    p.Len            = 1 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_FLOAT ;
    p.FormatIO       = EDF_NOTFORMAT ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = EDF_PROPERTY_NOTSET ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 1 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedInt  ( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 0 ;
    p.Len            = 4 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_INTEGER ;
    p.FormatIO       = EDF_NOTFORMAT ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = EDF_PROPERTY_NOTSET ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedBool ( FormatProperty& p  )
{   FormatedBoolYN( p ) ; }
//------------------------------------------------------------------------------
void CFormatString::FormatedBoolYN ( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 0 ;
    p.Len            = 0 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_YESNO ;
    p.FormatIO       = EDF_NOTFORMAT ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = EDF_PROPERTY_NOTSET ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedBoolIns ( FormatProperty& p  )
{   FormatedBoolYN( p ) ; p.Format = EDF_INSERT ; }
//------------------------------------------------------------------------------
void CFormatString::FormatedBool01 ( FormatProperty& p  )
{   FormatedBoolYN( p ) ;  p.Format = EDF_01 ; }
//------------------------------------------------------------------------------
void CFormatString::FormatedCheck( FormatProperty& p, int Var  )
{
    FormatedBoolYN( p ) ; p.Format = EDF_CHECK ;
    if ( Var == 1 ) p.Format = EDF_CHECKYN ;
    if ( Var == 2 ) p.Format = EDF_CHECKCOLOR ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedDate ( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 0 ;
    p.Len            = 0 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_DMY ;
    p.FormatIO       = EDF_SSS ;
    p.FormatMM       = EDF_SSS ;
    p.Calendar       = 0 ;
    p.FixedFields    = 1 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = EDF_PROPERTY_NOTSET ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 0 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedDateTime ( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 6 ;
    p.Len            = 2 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_DMYHMS ;
    p.FormatIO       = EDF_SSS ;
    p.FormatMM       = EDF_SSS ;
    p.Calendar       = 0 ;
    p.FixedFields    = 1 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = 0 ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 0 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedGrad ( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 10 ;
    p.Len            = 3 ;
    p.Spr1           = 0 ;
    p.Spr2           = '.' ;
    p.Format         = EDF_GRAD ;
    p.FormatIO       = EDF_GRAD ;
    p.FormatMM       = EDF_GRAD ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = EDF_PROPERTY_NOTSET ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedRad ( FormatProperty& p  )
{
    FormatedGrad ( p ) ;
    p.Dec            = 12 ;
    p.Len            = 1 ;
    p.Format         = EDF_RAD ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedGMS  ( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 6 ;
    p.Len            = 3 ;
    p.Spr1           = 0 ;
    p.Spr2           = '.' ;
    p.Format         = EDF_GMS ;
    p.FormatIO       = EDF_GRAD ;
    p.FormatMM       = EDF_GRAD ;
    p.Calendar       = 0 ;
    p.FixedFields    = 1 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = 0 ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedVitGrad( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 10 ;
    p.Len            = 5 ;
    p.Spr1           = 0 ;
    p.Spr2           = '.' ;
    p.Format         = EDF_VITGRAD ;
    p.FormatIO       = EDF_VITGRAD ;
    p.FormatMM       = EDF_VITGRAD ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = 0 ;
    p.SpaceLeftField = 1 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedVitRad ( FormatProperty& p  )
{
    FormatedGrad ( p ) ;
    p.Dec            = 12 ;
    p.Format         = EDF_VITRAD ;
    p.FormatIO       = EDF_VITRAD ;
    p.FormatMM       = EDF_VITRAD ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedVitGMS  ( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 6 ;
    p.Len            = 5 ;
    p.Spr1           = 0 ;
    p.Spr2           = '.' ;
    p.Format         = EDF_VITGMS ;
    p.FormatIO       = EDF_VITGRAD ;
    p.FormatMM       = EDF_VITGRAD ;
    p.Calendar       = 0 ;
    p.FixedFields    = 1 ;
    p.FixedFirstField= 0 ;
    p.FixedLastField = 0 ;
    p.SpaceLeftField = 1 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTime ( FormatProperty& p  )
{   FormatedTimeHMS( p ) ; }
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeHMS( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 6 ;
    p.Len            = 2 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_HMS ;
    p.FormatIO       = EDF_SEC ;
    p.FormatMM       = EDF_SEC ;
    p.Calendar       = 0 ;
    p.FixedFields    = 1 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = 0 ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeSSSHMS( FormatProperty& p  )
{
    FormatedTimeHMS( p ) ;
    p.Len            = 2 ;
    p.Format         = EDF_SSSHMS ;
    p.FixedFirstField= 0 ;
    p.SpaceLeftField = 1 ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeMiliSec ( FormatProperty& p  )
{
    FormatedTimeSec( p ) ;
    p.Dec            = 3 ;
    p.Len            = 8 ;
    p.Format         = EDF_MILLIS ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeSec ( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 6 ;
    p.Len            = 5 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_SEC ;
    p.FormatIO       = EDF_SEC ;
    p.FormatMM       = EDF_SEC ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = 0 ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeMin( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 8 ;
    p.Len            = 4 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_MIN ;
    p.FormatIO       = EDF_SEC ;
    p.FormatMM       = EDF_SEC ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = 0 ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeHour( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 10 ;
    p.Len            = 2 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_HOUR ;
    p.FormatIO       = EDF_SEC ;
    p.FormatMM       = EDF_SEC ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = 0 ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedTimeSSS( FormatProperty& p  )
{
    p.Value          = 0 ;
    p.Dec            = 13 ;
    p.Len            = 1 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_SSS ;
    p.FormatIO       = EDF_SEC ;
    p.FormatMM       = EDF_SSS ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = 0 ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedKeyNu( FormatProperty& p, bool isFull )
{
    p.Value          = 0 ;
    p.Dec            = 4 ;
    p.Len            = 5 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = isFull ? EDF_KEYNUFULL : EDF_KEYNU ;
    p.FormatIO       = EDF_NOTFORMAT ;
    p.FormatMM       = EDF_NOTFORMAT ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = EDF_PROPERTY_NOTSET ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatedCustomKey( FormatProperty& p )
{
    p.Value          = 0 ;
    p.Dec            = 0 ;
    p.Len            = 0 ;
    p.Spr1           = 0 ;
    p.Spr2           = 0 ;
    p.Format         = EDF_CUSTOMKEY ;
    p.FormatIO       = EDF_NOTFORMAT ;
    p.FormatMM       = EDF_NOTFORMAT ;
    p.Calendar       = 0 ;
    p.FixedFields    = 0 ;
    p.FixedFirstField= EDF_PROPERTY_NOTSET ;
    p.FixedLastField = EDF_PROPERTY_NOTSET ;
    p.SpaceLeftField = 0 ;
    p.Space          = 0 ;
    p.Sign           = 0 ;
    p.NotShowSign    = 1 ;
    p.Interval       = 0 ;
    p.Max            =-1 ;
    p.Min            = 0 ;
    p.ArrowKeys      = 0 ;
    p.TestValiVar    = EDF_TESTVALIVAR_EDIT ;
}
//------------------------------------------------------------------------------
void CFormatString::FormatChangeApply( bool isForce )
{  if ( m_isAutoChangeApply || isForce ) BuildFromData( ) ; }
//------------------------------------------------------------------------------
void CFormatString::SetAutoChangeApply( bool isAutoChangeApply )
{   m_isAutoChangeApply = isAutoChangeApply ; }
//------------------------------------------------------------------------------
bool CFormatString::GetAutoChangeApply( )
{   return m_isAutoChangeApply ; } 
//------------------------------------------------------------------------------
// Возврат :
//>0  - индекс поля содержащего дробную часть
//-1  - дробной части нет
int GetDecedField( int Format, int Dec )
{
    int rc =-1 ;
    switch ( Format ) {
    case EDF_DMY :       rc =-1 ; break ;                        // День месяц год
    case EDF_DMYHMS :    rc = ( Dec > 0 ? 6 :-1 ) ; break ;    // День месяц год Часы минуты секунды
    // Время
    case EDF_HMS :       rc = ( Dec > 0 ? 3 :-1 ) ; break ;    // Часы минуты секунды
    case EDF_SSSHMS :    rc = ( Dec > 0 ? 4 :-1 ) ; break ;    // Сутки Часы минуты секунды
    case EDF_MILLIS :    rc = ( Dec > 0 ? 1 :-1 ) ; break ;    // Милисекунды
    case EDF_SEC :       rc = ( Dec > 0 ? 1 :-1 ) ; break ;    // Секунды
    case EDF_MIN :       rc = ( Dec > 0 ? 1 :-1 ) ; break ;    // Минуты
    case EDF_HOUR :      rc = ( Dec > 0 ? 1 :-1 ) ; break ;    // Часы
    case EDF_SSS :       rc = ( Dec > 0 ? 1 :-1 ) ; break ;    // Сутки
    // Угловая величина
    case EDF_GMS :       rc = ( Dec > 0 ? 3 :-1 ) ; break ;    // Градусы минуты секунды
    case EDF_GRAD :      rc = ( Dec > 0 ? 1 :-1 ) ; break ;    // Градусы
    case EDF_RAD :       rc = ( Dec > 0 ? 1 :-1 ) ; break ;    // Радианы
    case EDF_VITGMS :    rc = ( Dec > 0 ? 4 :-1 ) ; break ;    // Виток + Аргумент широты Градусы минуты секунды
    case EDF_VITGRAD :   rc = ( Dec > 0 ? 2 :-1 ) ; break ;    // Виток + Аргумент широты Градусы
    case EDF_VITRAD :    rc = ( Dec > 0 ? 2 :-1 ) ; break ;    // Виток + Аргумент широты Радианы
    // Число
    case EDF_FLOAT :     rc = ( Dec > 0 ? 1 :-1 ) ; break ;    // Деиствительное число в форме с плавающей точкой
    case EDF_FIXED :     rc = ( Dec > 0 ? 1 :-1 ) ; break ;    // Деиствительное число в форме с фиксированной точкой
    // Целочисленные данные
    case EDF_INTEGER :   rc =-1 ; break ;    // Целое число
    case EDF_YESNO :     rc =-1 ; break ;    // Бит да  - нет
    case EDF_INSERT :    rc =-1 ; break ;    // Бит вкл - выкл
    case EDF_01 :        rc =-1 ; break ;    // Бит 0   - 1
    case EDF_CHECK :     rc =-1 ; break ;    // Бит          // Только для таблицы
    case EDF_CHECKYN :   rc =-1 ; break ;    // Бит          // Только для таблицы
    case EDF_CHECKCOLOR :rc =-1 ; break ;    // Бит          // Только для таблицы
    case EDF_ALT :       rc =-1 ; break ;    // Выбор альтернативы
    //
    case EDF_KEYNU :     rc =-1 ; break ;    // Ключ НУ        (Номер, тип, модификатор, БЦ)
    case EDF_KEYNUFULL : rc =-1 ; break ;    // Полный ключ НУ (КА, номер, тип, модификатор, БЦ)
    case EDF_CUSTOMKEY : rc =-1 ; break ;    // Произвольный набор ключей
    }
    return rc ;
}
//------------------------------------------------------------------------------
// Возврат :
// 0  - фиксирование длинна первого поля определяется свойством FixedFirstField
//-1  -
int GetFirstField( int Format )
{
    int rc =-1 ;
    switch ( Format ) {
    case EDF_DMY :       rc =-1 ; break ;     // День месяц год
    case EDF_DMYHMS :    rc =-1 ; break ;     // День месяц год Часы минуты секунды
    // Время
    case EDF_HMS :       rc =-1 ; break ;     // Часы минуты секунды
    case EDF_SSSHMS :    rc = 0 ; break ;     // Сутки Часы минуты секунды
    case EDF_MILLIS :    rc =-1 ; break ;     // Милисекунды
    case EDF_SEC :       rc =-1 ; break ;     // Секунды
    case EDF_MIN :       rc =-1 ; break ;     // Минуты
    case EDF_HOUR :      rc =-1 ; break ;     // Часы
    case EDF_SSS :       rc =-1 ; break ;     // Сутки
    // Угловая величина
    case EDF_GMS :       rc =-1 ; break ;    // Градусы минуты секунды
    case EDF_GRAD :      rc =-1 ; break ;    // Градусы
    case EDF_RAD :       rc =-1 ; break ;    // Радианы
    case EDF_VITGMS :    rc = 0 ; break ;    // Виток + Аргумент широты Градусы минуты секунды
    case EDF_VITGRAD :   rc = 0 ; break ;    // Виток + Аргумент широты Градусы
    case EDF_VITRAD :    rc = 0 ; break ;    // Виток + Аргумент широты Радианы
    // Число
    case EDF_FLOAT :     rc =-1 ; break ;    // Деиствительное число в форме с плавающей точкой
    case EDF_FIXED :     rc =-1 ; break ;    // Деиствительное число в форме с фиксированной точкой
    // Целочисленные данные
    case EDF_INTEGER :   rc =-1 ; break ;    // Целое число
    case EDF_YESNO :     rc =-1 ; break ;    // Бит да  - нет
    case EDF_INSERT :    rc =-1 ; break ;    // Бит вкл - выкл
    case EDF_01 :        rc =-1 ; break ;    // Бит 0   - 1
    case EDF_CHECK :     rc =-1 ; break ;    // Бит          // Только для таблицы
    case EDF_CHECKYN :   rc =-1 ; break ;    // Бит          // Только для таблицы
    case EDF_CHECKCOLOR :rc =-1 ; break ;    // Бит          // Только для таблицы
    case EDF_ALT :       rc =-1 ; break ;    // Выбор альтернативы
    //
    case EDF_KEYNU :     rc =-1 ; break ;    // Ключ НУ        (Номер, тип, модификатор, БЦ)
    case EDF_KEYNUFULL : rc =-1 ; break ;    // Полный ключ НУ (КА, номер, тип, модификатор, БЦ)
    case EDF_CUSTOMKEY : rc =-1 ; break ;    // Произвольный набор ключей
    }
    return rc ;
}
//--------------------------------------------------------------------------------
void operator << ( double& d, CFormatString& FS ) { FS >> d ; }
void operator >> ( double& d, CFormatString& FS ) { FS << d ; }
//--------------------------------------------------------------------------------
void operator << ( float&  d, CFormatString& FS ) { FS >> d ; }
void operator >> ( float&  d, CFormatString& FS ) { FS << d ; }
//--------------------------------------------------------------------------------
void operator << ( short&  d, CFormatString& FS ) { FS >> d ; }
void operator >> ( short&  d, CFormatString& FS ) { FS << d ; }
//--------------------------------------------------------------------------------
void operator << ( long&   d, CFormatString& FS ) { FS >> d ; }
void operator >> ( long&   d, CFormatString& FS ) { FS << d ; }
//--------------------------------------------------------------------------------
void operator << ( int&    d, CFormatString& FS ) { FS >> d ; }
void operator >> ( int&    d, CFormatString& FS ) { FS << d ; }
//--------------------------------------------------------------------------------
void operator << ( DATE&  d, CFormatString& FS ) { FS >> d ; }
void operator >> ( DATE&  d, CFormatString& FS ) { FS << d ; }
//--------------------------------------------------------------------------------
void operator << ( TIME&  d, CFormatString& FS ) { FS >> d ; }
void operator >> ( TIME&  d, CFormatString& FS ) { FS << d ; }
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
ZString& operator << (ZString& s, const char* cs)
{
	s.Set(cs);
	return s;
}

ZString& operator >> (ZString& s, char* const cs)
{
	sprintf(cs, "%s\0", s.Get());
	return s;
}

