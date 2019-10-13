// NRR.h
// ���� �������� 28.08.2014
//----------------------------------------------------------------------------------------------------
#if !defined(NRR_CONTENT)
#define NRR_CONTENT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define S_CANCEL	-1 
#define S_END		-2 


typedef struct {
	long	TimeIndex ; 
	double	V ;
} RItem ;

typedef vector<RItem> VRParam ; 
typedef vector<VRParam> VVRParam ; 

typedef struct {
	DATE	D;
	TIME	T ;
	double	t ;
	double  fx ;
} RTime ;

typedef vector<RTime> VRTime ; 

typedef struct {
	char	Key;
	int		SysNum ;
} RKA ;

typedef vector<RKA> VRKA ; 

typedef struct {
	RKA KA ;
	int i ; 
} IRKA ;

typedef vector<IRKA>	VIRKA ; 
typedef vector<int>		VINT ; 
typedef vector<double>  VDBL ; 
typedef vector<VINT>	VVINT ; 

typedef struct {
	int TimeIndex ;
	int ValueIndex ;
} RSplitPoint ;

typedef vector<RSplitPoint>	 VRSplitPoint ; 
typedef vector<VRSplitPoint> VVRSplitPoint ; 

bool operator < ( IRKA KA1, IRKA KA2 ) ;
bool operator > ( IRKA KA1, IRKA KA2 ) ;

bool operator < ( RSplitPoint P1, RSplitPoint P2 ) ;
bool operator > ( RSplitPoint P1, RSplitPoint P2 ) ;

class NRR ;

class NRContent {
public :

	friend class NRR ; 

	NRContent( ) ;
	~NRContent( ) ;
	
	void Clear( ) ;
	void Init ( int Column ) ;

	// �������� ��������� �� � ����
	int  AddKAtoShade( RKA KA, int* IndKA ) ;
	// �������� ������ ������� 
	void AddTime(DATE D, TIME T, double fx);
	// �������� �������� V ��������� NumParam ��� �� ��� ���������� ������������ ������� �������
	int  AddValue( double V, int NumParam,  RKA KA ) ;
	// ����� ����� � �������� i
	int  GetTime(int i, DATE* D, TIME* T, double* t);
	// ���������� ����� � ��������� (VVRParam P) ������� ��� ������� ��������� ��� �� (RKA KA)
	int  GetShadeKA( RKA KA ) ;		
	// i-� ��������� � ����� ��
	RKA  GetKA( int i ) ; 	
	// ���������� ����� � ��������� (VVRParam P) ������� ��� ������� ��������� i-�� ���������� � ����� �� � ������ Key
	int  GetIndexKA( int i, char Key ) ;
	// ����� ����������� �� ��� ������� �������� ���������� � ����������� �����
	int  GetCountKA( ) ;
	int  GetCountColumn( ) ;  	  
	
	void PrintParams( char* FileName ) ;

	VVRParam P ;

	static int DATAtoJlDay(DATE D, long& JlDay);
	static double TIMEtoSec( TIME T ) ;

protected :
	VINT* ShadesKA( char KeyKA ) ; 		
	VRKA* IndexsKA( ) ;	
	bool  IsSplitTimeIndex( int indParam, int TimeIndex ) ;

	VRTime	Time ;
	VINT	GShade ;
	VINT	RShade ;
	VINT	XShade ;
	VRKA	AIndex ;
	int		CountRColumn ;
	VVRSplitPoint SplitPoint ;
} ;

#endif // #if !defined(NRR_CONTENT)
