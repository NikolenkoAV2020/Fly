// NRR.h
// ���� �������� 28.08.2014
//----------------------------------------------------------------------------------------------------
#if !defined(NRR_READER)
#define NRR_READER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NRHeader.h"
#include "NRContent.h"

//----------------------------------------------------------------------------------------------------------
// ������� Rinex
//----------------------------------------------------------------------------------------------------------
class NRR {
public :
	NRR( ) ;
	~NRR( ) ;
	
	int  Read ( char* FileName ) ;
	int  Write( ) ;
	int  Splitting( ) ;
	int  SplittingParam( VRTime& Time, VRParam& VP, int jn =-1, int jk =-1, VRSplitPoint* pVJ = NULL, VVRParam* pVPS = NULL, int* pCountFindSplit = NULL ) ;
	void PrintContent( char* FileName ) ;

private:
	int SkipSpace( char* S ) ;
	int SkipChar( char* S ) ;
	int SkipNum( char* S ) ;
	int SkipZero( char* S ) ;
	int GetNum( char* S, double& d ) ;
	int GetWord( char* S, RWord& HW ) ;
	int GetSpace( char* S, char* T ) ;

	void LineToWords( char* S, VRWord& VHW, bool isHeader ) ;
	int StrToVRKA( char* S, VRKA& VKA ) ;

	int BuildString(char* s, VRWord& VW, int i, int j) ;
	int BuildDouble(double& d, RWord& W) ;
	int BuildFloat (float& d, RWord& W) ;
	int BuildShort (short& d, RWord& W) ;
	int BuildDate  (DATE& D, RWord& W);
	int BuildTime  (TIME& T, RWord& W) ;

protected :
	int ReadName( ) ;
	int ReadHiader( int fd ) ;
	int ReadHiaderItem( char* S ) ;
	int ReadLine( int fd, char* S ) ; 

	int ReadTimePoint( int fd ) ;
	int WriteTimePoint( FILE* pf, int TimeIndex, VINT& k ) ;

	int CalcSKO( VRSplitPoint& VJ, VDBL& VDPDT, double& maxDP, double& minDP, double& sDP, double& sDPsko ) ;

	// ���������
	NRHeader  m_H ;
	// ��������� ������ 
	NRContent m_C ;

	typedef struct {
		char s[256] ;
	} HeaderSourceItem ;
	vector<HeaderSourceItem> HeaderSource ;

	// ������ ����������� ����� �����
	char m_FileName[ 512 ] ;
	DATE m_dt;
	char m_Name[ 32 ] ;	// ��������� ������������ �������
	char m_Obj[ 32 ] ;	// �������
	long m_Nv ;			// ����� ����� ������ ����� ��� ���������� ����� 
	long m_Na ;			// ����� ������ ��������� ���� �� ����� ��� ���������� �����.
} ; 

//----------------------------------------------------------------------------------------------------
#endif // #if !defined(NRR_READER)
