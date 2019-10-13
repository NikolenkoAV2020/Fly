//----------------------------------------------------------------------------------------------------------
// NRContent.cpp
// Артём Николаев 28.08.2014
//----------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

#include <FlyTime.h>
#include "NRContent.h"

//----------------------------------------------------------------------------------------------------------
bool operator < ( IRKA KA1, IRKA KA2 ) 
{
	if ( KA1.KA.Key != KA2.KA.Key ) return ( KA1.KA.Key < KA2.KA.Key ) ;
	if ( KA1.KA.SysNum < KA2.KA.SysNum ) return true ;
	return false ;
}

bool operator > ( IRKA KA1, IRKA KA2 ) 
{
	if ( KA1.KA.Key != KA2.KA.Key ) return ( KA1.KA.Key > KA2.KA.Key ) ;
	if ( KA1.KA.SysNum > KA2.KA.SysNum ) return true ;
	return false ;
}

//----------------------------------------------------------------------------------------------------------
bool operator < ( RSplitPoint P1, RSplitPoint P2 ) 
{
	if ( P1.ValueIndex < P2.ValueIndex ) return true ;
	return false ;
}

bool operator > ( RSplitPoint P1, RSplitPoint P2 ) 
{
	if ( P1.ValueIndex > P2.ValueIndex ) return true ;
	return false ;
}

//----------------------------------------------------------------------------------------------------------
static long operator - (DATE D1, DATE D2)
{
	long JlDay1 ; NRContent::DATAtoJlDay( D1, JlDay1 ) ;
	long JlDay2 ; NRContent::DATAtoJlDay( D2, JlDay2 ) ;
	return ( JlDay1 - JlDay2 ) ;
}

static long operator - (TIME T1, TIME T2)
{
	double t1 = T1.h * 3600.0 + T1.m * 60 + T1.s + T1.d;
	double t2 = T2.h * 3600.0 + T2.m * 60 + T2.s + T2.d;
	return ((long)(t1 - t2));
}

//----------------------------------------------------------------------------------------------------------
int NRContent::DATAtoJlDay(DATE D, long& JlDay)
{
	int flag_afterGrig = 1 ;
	int flag_366 = 0 ;
	if (D.g%4==0) flag_366=1;
	
	if ((flag_afterGrig!=0)&&(D.g%100==0)&&(D.g%400!=0)) flag_366=0;
	if (((int)D.m==2)&&((int)D.d>28+(int)flag_366)) return S_FALSE ;
	
	// Считаем 1.01.m_Year по Юлианскому
	JlDay=(int)(floor((D.g+4715)/4.0))+(D.g+4712)*365;
	// Добавляем Григорианскую поправку
	if (flag_afterGrig!=0) JlDay-= (int)(floor((D.g-1)/100.)-floor((D.g-1)/400.)-2);
	// Добавляем начало невисокосного месяца от начала года
	int month_delta=-1;
	if ((int)D.m<3) month_delta=1;
	if (((int)D.m==9)||((int)D.m==11)) month_delta=0;
	JlDay+=(int)month_delta+((int)D.m-1)*30+((int)D.m/2)-1;
	// Если год високосный, то для марта и далее
	if((int)D.m>2) JlDay+=(int)flag_366;
	JlDay+=(int)D.d-1;
	
	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
double NRContent::TIMEtoSec( TIME T ) 
{
	return 0 ;
}

//----------------------------------------------------------------------------------------------------------
NRContent::NRContent( ) 
{
	Clear( ) ;
}

//----------------------------------------------------------------------------------------------------------
NRContent::~NRContent( ) 
{
	
}

//----------------------------------------------------------------------------------------------------------
void NRContent::Clear( ) 
{
	int i, N = P.size( ) ;
	for( i = 0 ; i < N ; i++ ) P[ i ].clear( ) ;
	P.clear( ) ;

	N = SplitPoint.size( ) ;
	for( i = 0 ; i < N ; i++ ) SplitPoint[ i ].clear( ) ;
	SplitPoint.clear( ) ;
	
	Time.clear( ) ;
	GShade.clear( ) ;
	RShade.clear( ) ;
	XShade.clear( ) ;	
	AIndex.clear( ) ;

	CountRColumn = 0 ;
}

//----------------------------------------------------------------------------------------------------------
void NRContent::Init( int Column ) 
{
	CountRColumn = Column ;
}

//----------------------------------------------------------------------------------------------------------
vector<int>* NRContent::ShadesKA( char KeyKA ) 		
{
	VINT* pv = NULL ;
	switch( KeyKA ) {
	case 'G' : pv = &GShade ; break ;
	case 'R' : pv = &RShade ; break ;
	case 'X' : pv = &XShade ; break ;
	default: return NULL ;
	}
	return pv ;
}

//----------------------------------------------------------------------------------------------------------
VRKA* NRContent::IndexsKA( ) 		
{
	VRKA* pv = NULL ;
	pv = &AIndex ; 
	return pv ;
}

//----------------------------------------------------------------------------------------------------------
int NRContent::GetCountKA( ) 		
{
	return ( AIndex.size( ) ) ; 
}

//----------------------------------------------------------------------------------------------------------
// Добавить системный номер КА в тень
//		Key		-	признак системы
//		SysNum	-	системный номер КА
// Одновременно выделяем место для хранения массива параметров 
int NRContent::AddKAtoShade( RKA KA, int* IndKA ) 
{
	if ( !CountRColumn ) return S_FALSE ;

	VINT* pv = ShadesKA( KA.Key ) ; if ( !pv ) return S_FALSE ;
	VRKA* iv = IndexsKA( ) ; if ( !iv ) return S_FALSE ;

	int i, M = pv->size( ) ;
	if ( KA.SysNum > M ) {	
		pv->resize( KA.SysNum ) ;
		for( i = M ; i < KA.SysNum ; i++ ) (*pv)[ i ] =-1 ;
	} 
	if ( (*pv)[ KA.SysNum - 1 ] < 0 ) {
		int N = P.size( ) ; 
		N = N / CountRColumn ;
		N ++ ;
		P.resize( N * CountRColumn ) ;
		(*pv)[ KA.SysNum - 1 ] = ( N - 1 ) * CountRColumn ;
		iv->resize( N ) ;
		(*iv)[ N - 1 ] = KA ;
	} 
	if ( IndKA ) *IndKA = (*pv)[ KA.SysNum - 1 ] ;

	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
int NRContent::GetShadeKA( RKA KA ) 		
{
	VINT* pv = ShadesKA(KA.Key);
	if (!pv) return-1;
	if ((int)pv->size() < KA.SysNum) return-1;
	int i = (*pv)[KA.SysNum - 1];
	return i;
}

//----------------------------------------------------------------------------------------------------------
RKA NRContent::GetKA( int i ) 		
{
	VRKA* iv = IndexsKA();
	if (!iv || (int)iv->size() <= i) {
		RKA KA;
		KA.SysNum = -1;
		KA.Key = ' ';
		return KA;
	}
	return (*iv)[i];
}

//----------------------------------------------------------------------------------------------------------
int NRContent::GetIndexKA( int i, char Key ) 		
{
	RKA KA ; 
	KA.Key = Key ;
	KA.SysNum = GetKA( i ).SysNum ; if ( KA.SysNum < 0 ) return -1 ; 
	return( GetShadeKA( KA ) ) ;
}

//----------------------------------------------------------------------------------------------------------
void NRContent::AddTime(DATE D, TIME T, double fx)
{
	RTime RT ;
	RT.D = D;
	RT.T = T;
	RT.t = 0;
	RT.fx = fx;
	if ( Time.size( ) ) {
		DATE   D0 = Time[0].D;
		TIME   T0 = Time[0].T;
		long   d = D - D0;
		double s = T - T0;
		RT.t = (double)d + s / 86400.0;
	}
	Time.push_back(RT);
}

//----------------------------------------------------------------------------------------------------------
int NRContent::GetTime(int i, DATE* D, TIME* T, double* t)
{
	if (i >= (int)Time.size()) return S_FALSE;
	if (D) *D = Time[i].D;
	if (T) *T = Time[i].T;
	if (t) *t = Time[i].t;
	return S_OK ;
}

//----------------------------------------------------------------------------------------------------------
int NRContent::GetCountColumn( )   	  
{
	return( CountRColumn ) ;
}

//----------------------------------------------------------------------------------------------------------
int NRContent::AddValue( double V, int NumParam, RKA KA ) 
{
	if (NumParam >= CountRColumn) return S_FALSE;

	int ind = GetShadeKA(KA);
	if (ind < 0) {
		int IndKA;
		if (AddKAtoShade(KA, &IndKA) != S_OK) return S_FALSE;
		ind = IndKA; if (IndKA < 0) return S_FALSE;
	}
	if (ind + NumParam >= (int)P.size()) return S_FALSE;

	if (!Time.size()) return S_FALSE;
	
	RItem RI;
	RI.TimeIndex = Time.size() - 1;
	RI.V = V;
	P[ind + NumParam].push_back(RI);

	return S_OK;
}

//----------------------------------------------------------------------------------------------------------
bool NRContent::IsSplitTimeIndex( int indParam, int TimeIndex )  
{
	if ((int)SplitPoint.size() <= indParam) return false;
	if ((int)Time.size() <= TimeIndex) return false;

	int N = SplitPoint[indParam].size();
	for (int i = 0; i < N; i++) {
		if (SplitPoint[indParam][i].TimeIndex > TimeIndex) break;
		if (SplitPoint[indParam][i].TimeIndex == TimeIndex)
			return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------
void NRContent::PrintParams( char* FileName )
{
	FILE* pf = fopen(FileName, "w"); if (!pf) return;

	DATE	D;
	TIME	T;
	double	t;
	int		i, j, N = P.size();

	VINT k; k.resize(N);
	for (i = 0; i < N; i++) k[i] = 0;

	j = 0;
	while (1) {
		if (GetTime(j, &D, &T, &t) != S_OK) break;
		//fprintf( pf, " %.4d    %.2d.%.2d.%.4d-%.2d:%.2d:%.2d   ", j,  D.d, D.m, D.g, T.h, T.m, T.s ) ; 
		fprintf(pf, " %d    ", j);
		for (i = 0; i < 50; i++) {
			if (k[i] >= (int)P[i].size() || P[i][k[i]].TimeIndex != j) {
				//fprintf( pf, "                   " ) ;
				fprintf(pf, "                  0");
				continue;
			}
			fprintf(pf, "%19.10e", P[i][k[i]].V);
			k[i]++;
		}
		fprintf(pf, "\n");
		j++;
	}

	fclose(pf); pf = 0;
}

//----------------------------------------------------------------------------------------------------------
	
