//------------------------------------------------------------------------
// ModPUManager.cpp
// �. ��������� 13.08.2018
//------------------------------------------------------------------------
#include <stdafx.h>
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ModPUManager.h>

//-------------------------------------------------------------------------------
// �������� �������� ����������
//-------------------------------------------------------------------------------
ZPUManager::ZPUManager( )
{
	Clear() ;
}

//-------------------------------------------------------------------------------
ZPUManager::~ZPUManager( ) 
{
	Clear() ;
}

//-------------------------------------------------------------------------------
void ZPUManager::Clear( ) 
{
	for (int i=0 ; i<MAX_PU_COUNT_MOD ; i++) PU[i].Clear() ;
	Count = 0 ;
	ClearIndex() ;
}

//-------------------------------------------------------------------------------
void ZPUManager::Init()
{
	ClearIndex() ;
}

//-------------------------------------------------------------------------------
void ZPUManager::ClearIndex( ) 
{
	memset(IndexVN,0, MAX_PU_COUNT_MOD*sizeof(int)) ;
	memset(IndexVP,0, MAX_PU_COUNT_MOD*sizeof(int)) ;
	memset(IndexV, 0, MAX_PU_COUNT_MOD*sizeof(int)) ;
	memset(IndexU, 0, MAX_PU_COUNT_MOD*sizeof(int)) ;
	TIndex = 0 ;
	NextTimePoint = 0 ;
	PrevTimePoint = 0 ;
	TimePointKey  = 0 ;

	PrevIDMSG = 0 ;
	NextIDMSG = 0 ;
	CurIDMSG  = 0 ;
}

//-------------------------------------------------------------------------------
void ZPUManager::Indexing(double t, int dir) 
{
	// ���������� ����� �������
	//   1. ��������� � ������� ��������������� ������� t				IndexV[i] � IndexU[i] 
	//      ���� 0, �� ������ � ������ ������ ������� t ��������� ��������� ���������� i ���
	//   2. ���������� ������ ���������									IndexVN[i]
	//      ���� 0, �� ��������� �� �������, ���� -1, �� ��������� ���
	//   3. ���������� ����� ���������									IndexVP[i]
    //      ���� 0, �� ��������� �� �������, ���� -1, �� ��������� ���
	//
	// ���� ��� ������� ������� ������� 
	// ���������� ��������� ������ ��, �� � ���������� ������ �������� 
	//		PU[ i ].IsOn = true 
	//		IndexV[i] > 0 
	//		IndexU[i] > 0 
	TimePointKey = 0 ;
	PrevIDMSG    = 0 ;
	NextIDMSG    = 0 ;
	CurIDMSG     = 0 ;
	// ������� �������� ����������
	for (int i=0 ; i<MAX_PU_COUNT_MOD ; i++) {
		PU[ i ].IsOn = false ; 
		if ( PU[ i ].isEmpty( ) ) {
			IndexVN[i] = 0 ;
			IndexVP[i] = 0 ;
			IndexV[i]  = 0 ;
			IndexU[i]  = 0 ;
			break ;
		}

		double eps = 1e-13 ;
		double tn, tk ;
		int j0 = 0 ;				// ��������� ������ ��� �������� ��������
		int jm = PU[i].Size() ;		// �������� ������ ��� �������� ��������
		if (TIndex) {
			if (t >= TIndex) j0 = (IndexV[i] ? (IndexV[i]-1):(IndexVN[i]>0 ? (IndexVN[i]-1):0)) ;
			else j0 = (IndexV[i] ? (IndexV[i]-1):(IndexVP[i]>0 ? (IndexVP[i]-1):0)) ;
		}
		j0 = (j0>0?(j0-1):0) ;

		IndexVP[i] = 0 ;	// ������ ��������� ����������� ��� �� i	
		IndexVN[i] = 0 ;	// ������ ��������� ���������� (��� ��������) ��� �� i
		// ������� ���������
		for (int j=j0 ; j<jm ; j++) {
			tn = PU[i].V[j].TS() ;	// ����� ������ ���������
			tk = PU[i].V[j].TE() ;	// ����� ����� ��������� 
			if (t>tk) {
				IndexVP[i] = j+1 ;
				PrevTimePoint = (!i || PrevTimePoint<tk) ? tk:PrevTimePoint ;
				TimePointKey|= 1 ; 
				PrevIDMSG = MMSG_TDU_OFF ;
			}
			if (!IndexVN[i] && t<tn) {
				IndexVN[i] = j+1 ;
				NextTimePoint = (!i || NextTimePoint>tn) ? tn:NextTimePoint ;
				TimePointKey|= 2 ; 
				NextIDMSG = MMSG_TDU_ON ;
				break ;
			}
			if (fabs(t-tn)<eps || fabs(t-tk)<eps || (t-tn)*(t-tk)<0) {
				// ������� ��������� ������� �������� ������ ������� t
				PU[ i ].IsOn = true ; 
				IndexV[i] = j+1 ;
				IndexU[i] = 0 ;
				int km = PU[i].V[j].Size() ; 
				// ������� ��������
				for (int k=0 ; k<km ; k++) {
					tn = PU[i].V[j].U[k].TS() ;
					tk = PU[i].V[j].U[k].TE() ;
					if (fabs(t-tn)<eps || fabs(t-tk)<eps ||	(t-tn)*(t-tk)<0) {
						// ������ ������� ������� �������� ������ ������� t
						if (dir>0) {
							if (fabs(t-tn)<eps) {
								if (k==0) {		
									// ������ ���������								
									CurIDMSG = MMSG_TDU_ON ;
									PU[ i ].IsOn = true ; 
								} else {			
									// ������ ���������� �������
									CurIDMSG = MMSG_TDU_STEPON ;
									PU[ i ].IsOn = true ; 
							}	}
							if (fabs(t-tk)<eps) {
								if (k==km-1) {	
									// ����� ���������
									CurIDMSG = MMSG_TDU_OFF ;
									// ���-��� ��������� ����� ���������� ������� ������� ��������� ������ ��
									// �� ���������, ��� �� �� �������
									PU[ i ].IsOn = false ; 
								} else {			
									// ����� ���������� �������
									CurIDMSG = MMSG_TDU_STEPOFF ;
									PU[ i ].IsOn = true ;
									// ���������� ��������� ��������� �������
									continue ;
						}	}	}
						if (dir<0) {
							if (fabs(t-tn)<eps) {
								if (k==0) { 
									// ������ ���������
									CurIDMSG = MMSG_TDU_ON ;
									// ���-��� ������� � �������� ����������� (dir<0),
									// �� ���������, ��� �� �� �������
									PU[ i ].IsOn = false ; 
								} else {			
									// ������ ���������� �������
									CurIDMSG = MMSG_TDU_STEPON ;
									PU[ i ].IsOn = true ; 
							}	}
							if (fabs(t-tk)<eps) {
								if (k==km-1) {	
									// ����� ���������
									CurIDMSG = MMSG_TDU_OFF ;
									PU[ i ].IsOn = true ; 
								} else {			
									// ����� ���������� �������
									CurIDMSG = MMSG_TDU_STEPOFF ;
									PU[ i ].IsOn = true ; 
						}	}	}
						if (PU[ i ].IsOn) { 
							IndexU[i] = k+1 ;
							PrevTimePoint = (!i || PrevTimePoint<tn) ? tn:PrevTimePoint ;
							NextTimePoint = (!i || NextTimePoint>tk) ? tk:NextTimePoint ;
							TimePointKey |= 4 ; 
						} 
						break ;
					} // ����� �������� �������	
				} // ����� �������� ��������
				if (IndexU[i]==0) {
					IndexV[i] = 0 ;
					PU[ i ].IsOn = false ; 
				}
				break ;
			} // ����� �������� ���������	
		} // ����� �������� ���������
		if (!IndexVN[i]) IndexVN[i] =-1 ;
		if (!IndexVP[i]) IndexVP[i] =-1 ;
		TIndex = t ;
	} // ����� �������� �������� ����������
}

//-------------------------------------------------------------------------------
int ZPUManager::TimeCheckForStep(double t, int dir, long* IDDUMSG, double* maxstep)
{
	if (IDDUMSG) *IDDUMSG = 0 ; 

	// ���� ��������� ��������� ����� �� ��������� ����������
	// ����� ��������� ����� � ������ ����� �������
	// ����������� ������� ����� ��� ������
	Indexing(t, dir) ;

	if(CurIDMSG) {
		// ������� ����� ���������� ��� ����� ������� �� ���
		// ���������� ������������� ���������� �������
		if (IDDUMSG) *IDDUMSG = CurIDMSG ; 
	}

	// ���� ������� ����� ������� ������ � ����������� �������� ����� 
	// ��� ����� ������� ����� � ������� �����, �� ������������ ������������
	// ��� �������������� ����� �������, ����� �� ��������� ���������������
	// ��������� �����
	if ((TimePointKey&2 || TimePointKey&4) && dir>0) {
		if (maxstep) {
			if (t+*maxstep>NextTimePoint) *maxstep = NextTimePoint-t ;
	}	}	
	if ((TimePointKey&1 || TimePointKey&4) && dir<0) {
		if (maxstep) {
			if ((t-*maxstep)<PrevTimePoint) *maxstep = t-PrevTimePoint ;
	}	}	

	return 0 ;
}

//-------------------------------------------------------------------------------
void ZPUManager::GetTimePoint( int& Key, double& PrevT, double& NextT, long& P_IDMSG, long& N_IDMSG ) 
{
	NextT = NextTimePoint ;
	PrevT = PrevTimePoint ;
	Key   = TimePointKey ;
	P_IDMSG = PrevIDMSG ;
	N_IDMSG = NextIDMSG ;
}

//-------------------------------------------------------------------------------
bool ZPUManager::isEmpty ()  { return (Count?false:true) ; }
void ZPUManager::operator << (ZPU &I) { Add(I) ; }
int ZPUManager::Size() { return Count ; }

//-------------------------------------------------------------------------------
int ZPUManager::Add(ZPU &I) 
{
	int i ;
	for (i=0 ; i<MAX_PU_COUNT_MOD; i++) {
		if (PU[i].isEmpty()) {
			PU[i]=I ;
			ClearIndex() ;
			Count++ ;
			break ;
	}	}
	if (i == MAX_PU_COUNT_MOD) {
		AddErrorMSG( "��������� ���������� �� �����������, ����������� �������� ���������� ������ ����������� �����������" ) ; 
		return 1 ;
	}
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZPUManager::Set(ZPU &I, int ind) 
{
	if (ind<0 || ind>=MAX_PU_COUNT_MOD) {
		AddErrorMSG( "��������� ���������� �� �����������, �������� ������" ) ; 
		return 1 ;
	}
	if (PU[ind].isEmpty()) Count++ ;
	else PU[ind].Clear() ;			
	PU[ind] = I ;
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZPUManager::Del(int ind) 
{
	if (ind<0) {
		for (int i=0 ; i<MAX_PU_COUNT_MOD ; i++) PU[i].Clear() ;
		return 0 ;
	}
	if (ind>=MAX_PU_COUNT_MOD) {
		AddErrorMSG( "��������� ���������� �� ������� �� ������������ �������, �������� ������" ) ; 
		return 1 ;
	}
	PU[ind].Clear() ;
	Count-- ;
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZPUManager::CalcRLN(double t, double m, double* arln, double* pMs) 
{ 
	memset(arln, 0, 3*sizeof(double)) ;
	if (pMs) *pMs=0 ;
	// ����� ����������� �� ������� ������� ������ �� �������� ����������
	//Indexing(t) ;
	// ������� ��������� ��� ������ ���������� � ������ ������ ������� ��
	double a[3] ;
	int    i, j ;
	// ������� ���� ����������� �������� ����������
	for (i=0 ; i<MAX_PU_COUNT_MOD ; i++) {
		if (PU[i].isEmpty() || !PU[i].IsOn) {
			if (i) break ;
			else return-1 ;
		}
		// ����� ��������� (������������� �� 1)
		int NumV = IndexV[i] ;
		if (NumV>0) {
			// ����� ������� (������������� �� 1)
			int NumU = IndexU[i] ;
			if (NumU>0) {
				// ���� ������� ���� �� ����������� ��������
				// ������� CalcARLN ������ ������� ��������� ����������
				// ��������� ��������� � �/���2 !!!!!
				double ms ;
				PU[i].V[NumV-1 ].U[NumU-1].CalcARLN(t, m, a, &ms) ;
				// ������������ ��������� �� ���� ���������� 
				// �� ������ ���� ���������� ����������
				for (j=0 ; j<3 ; j++) arln[j]+= a[j] ;
				// ������������ ���������� ������� ������� �� ���� ���������� 
				// �� ������ ���� ���������� ����������
				if (pMs) *pMs+= ms ;
	}	}	}
	// ������� ��������� �� �/���2 � ����/���2
	for (j=0 ; j<3 ; j++) arln[j]*= 8.64*86.4 ;
	return 0 ; 
} 

//-------------------------------------------------------------------------------