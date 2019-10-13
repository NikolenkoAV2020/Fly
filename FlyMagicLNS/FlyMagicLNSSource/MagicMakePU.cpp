//-------------------------------------------------------------------------------
// MagicMakePU.cpp : 
// �. ��������� 23.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>		// �������������� �������
#include <FlyCore.h>		// �������������� �������
#include <FlyMagicLNS.h>	//

//-------------------------------------------------------------------------------
int MagicMakePU(int NumPU, double m0, double* dms, ZSFlightRule& U, ZCDU& DU, ZPU& PU, int is, int ie)
{
	double t0, tm, dm ;
	int    i, j, rc = 0 ; 
	int    je = ie<0 ? (U.n-1):(ie>=U.n ? (U.n-1):ie) ;
	int    js = is<0 ? 0:(is>je? je:is) ;

	if (dms) *dms = 0 ; 
	for (j=js; j<=je; j++) {
		// ������ ������������ ��������� ������� ������ ��
		rc = ZFPUCalcTimeMainFromTime(m0, DU, NULL, NULL, &U.dt[j], NULL, &dm, NULL, tm, NULL) ; RRC ;
		// ��������� ����� �� ��������� ������ ������� �� ��������� � �������������
		// ��������� ������� tm
		m0-= dm ;
		if (dms) *dms+= dm ; 
		// ��������� ��
		ZVPU VPU ;	
		t0 = U.t[j] ;
		for (i=0 ; i<DU.k ; i++) {
			// ������� ��������� ����������
			if (i>=DU.mu && DU.isOnlyMain) break ;
			ZPUItem IPU ;	
			IPU.var = varPU_pms ;
			IPU.t   = t0 ;			
			IPU.dt  = (i==DU.mu)?tm:DU.i[i].t ;  
			IPU.P   = DU.i[i].p ;
			IPU.ms  = DU.i[i].p/DU.i[i].pu ;
			IPU.dP  = 0 ;
			IPU.dms = 0 ; 
			IPU.a   = U.alf[j] ; 
			IPU.b   = U.bet[j] ; 
			IPU.da  = 0 ;
			IPU.db  = 0 ;
			// ��������� ������� �� ���������
			VPU << IPU ;
			t0+= IPU.dt/k_cbc ;
		}
		// ��������� ��������� � ��������� ����������
		PU << VPU ;
	}
	PU.NumPU = NumPU ;
	return 0 ;
}

//-------------------------------------------------------------------------------