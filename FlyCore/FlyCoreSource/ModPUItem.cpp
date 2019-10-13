//------------------------------------------------------------------------
// ModPUItem.cpp
// �. ��������� 24.08.2018
//------------------------------------------------------------------------
#include <stdafx.h>
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ModPUManager.h>
//-------------------------------------------------------------------------------
// ������� ��������� ����������
//-------------------------------------------------------------------------------
ZPUItem::ZPUItem ( ) {
	var = varPU_ppu ;
	t   = 0 ;  // ����� ��������� � ���
	dt  = 0 ;  // ������������ ��������� � ��� 
	P   = 0 ;  // ���� � ���
	Pu  = 0 ;  // �������� ���� � ���
	dP  = 0 ;  // ����������� �� ���� �� ������� ��� / ���
	dPu = 0 ;  // ����������� �� �������� ���� ��
	a   = 0 ;  // ���������� ��������
	b   = 0 ;  //
	da  = 0 ;
	db  = 0 ;
}
//-------------------------------------------------------------------------------
double ZPUItem::TS() { return(t) ; }

//-------------------------------------------------------------------------------
double ZPUItem::TE() { return(t+dt/k_cbc) ; }

//-------------------------------------------------------------------------------
// ������ ��������� � ����������� ������� ���������
//	time			- ����� �� ������� ������������� ���������
//	m				- ��������� ����� �� � ������ ������� time 
//	Arln			- ������� ��������� � ����������� ������� ���������
//	pMs, pP, pPu	- ��������� ������, ����, �������� ���� �� ������ ������� � ������ ������� time
int ZPUItem::CalcARLN(double time, double m, double* Arln, double* pMs, double* pP, double* pPu) 
{	
	if (time<t || time>t+dt/k_cbc) {
		// ���� �������� ����� time ����� ��� ������� �������
		// �� ���� �� ������� t ��� ����� t+dt,
		// ����� ���������� ������� ���������
		memset(Arln, 0, 3*sizeof(double)) ;
		return 0 ;
	}
	double AM ;
	double Pdu ;
	double PUdu ;
	double MSdu ;

	// ����� �� ������ ������� � ��������
	double dtime = (time-t)*k_cbc ;

	Pdu  = P ;
	if (dP!= 0) Pdu+= dP*dtime ;
	if (Pdu<1e-13) {
		AddErrorMSG( "� ��������� ���������� ������ ������������ ����� �������� ����" ) ; 
		return 1 ;
	}

	switch (var) {
		case varPU_ppu:
			PUdu = Pu ;
			if (dPu!=0) PUdu+= dPu*dtime ;
			if (PUdu<1e-13) {
				AddErrorMSG( "� ��������� ���������� ������ ������������ ����� �������� �������� ����" ) ; 
				return 1 ;
			}
			MSdu = Pdu/PUdu ;
			break ;
		case varPU_pms:
			MSdu = ms ;
			if (dms!= 0) MSdu+= dms*dtime ;
			if (MSdu<1e-13) {
				AddErrorMSG( "� ��������� ���������� ������ ������������ ����� �������� ���������� ������� �������" ) ; 
				return 1 ;
			}
			PUdu = Pdu/MSdu ;
			break ;
		default:
			AddErrorMSG( "����� ����������� ��� ������� ��������� ����������" ) ; 
			return 1 ;
	}

	if (pMs) *pMs = MSdu ;
	if (pPu) *pPu = PUdu ;
	if (pP ) *pP  = Pdu ;
	// ������ ��������� � �/���2
	AM = Pdu*k_g0/m ;
	// ���� ��������� �� ��
	double at = a+da*dtime ; 
	double bt = b+db*dtime ; 
	// ������ �������� � ����������� ��
	double ra = TO_RAD(at) ;
	double rb = TO_RAD(bt) ;
	double sb = sin(rb) ;
	double cb = cos(rb) ;
	double sa = sin(ra) ;
	double ca = cos(ra) ;
	Arln[0] = AM*sb ;		// ���������� �����������
	Arln[1] = AM*cb*ca ;	// ���������������		
	Arln[2] = AM*cb*sa ;	// ������������
	
	return 0 ;
}

//-------------------------------------------------------------------------------
// �������� ��������� ���� ��������
// "������" ��� ������� ������� ������
bool operator < ( const ZPUItem &U1, const ZPUItem &U2 ) 
{ return( U1.t < U2.t ? true : false ) ; }

//-------------------------------------------------------------------------------