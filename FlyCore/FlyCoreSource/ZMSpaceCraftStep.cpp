//-------------------------------------------------------------------------------
// ZMSpaceCraftStep.cpp
// �. ��������� 03.08.2018
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>	// �������������� �������

//---------------------------------------------------------------------------
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZHBL.h>
#include <FlyCoreSource\\ZFUS.h>
#include <FlyCoreSource\\ZNIP.h>
#include <FlyCoreSource\\ZCalc.h>

//-------------------------------------------------------------------------------
int ZMSC::GoStepA(ZSC_SI* SI, int dir, double* notFurtherTime, ModEventName* BreakKey) 
{
	if (dir) SetDirect(dir) ;
	PreTestVit() ;

	// ��������� ��������� ����������� ����
	double OldMaxH = m_cs.step_max ; 
	double OldMinH = m_cs.step_min ;

	double* X = &((*this)[1]) ;
	double* t = &((*this)[0]) ; 

	int  rc = 0 ; 

	if (BreakKey) *BreakKey = ModEvent_EMPTY ; 
	// ���� ������ ����� �� ����� �������� ����� ���������� ��� ��������������,
	if (notFurtherTime) {
		// �� ��� ���������� ������� ����������� ������������ ������������ ��� �������������.
		// ������ ���� ���������� ��� �� ��� ����������� ����� ������� �� ���
		if (*t+m_cs.step_max > *notFurtherTime) m_cs.step_max = *notFurtherTime-*t ;
		// ��� ������������� �������� ������������ ���� �������������� �������� 
		// � ������������ � ����� ��������� ������������ ����� ��������������
		m_cs.step_min = m_cs.step_min<m_cs.step_max?m_cs.step_min:m_cs.step_max/10.0 ;
	}

	// � ������ ����� ������ �� ���������� �������������� ����� ���������� ������,
	// � � �������� ���� �������������� � ���������, ������� ����� ������� �� ���.
	// ������ ������� ������� �������� ������� ��������� � ���������� �� � �������
	// ��������� ������� ������������� (������� ��������� ��)
	// � ��������� ������ ������� ������ ����� ����� ���������� ���, �� ���� ������
	// ������ ���������� ����� �������������� ("������" ��� ������ ������ - 8 ����� 
	// ��� ������ 7-�� �������)
	if (LSF.isDU) {
		long IDDUMSG = 0 ;
		MPU.TimeCheckForStep(*t, dir, &IDDUMSG, &m_cs.step_max) ;
		// ��� ������������� �������� ������������ ���� �������������� �������� 
		// � ������������ � ����� ��������� ������������ ����� ��������������
		m_cs.step_min = m_cs.step_min<m_cs.step_max?m_cs.step_min:m_cs.step_max/10.0 ;
		// ���� ���������� ����� ������� �� ���
		if (IDDUMSG) { 
			// ���������� �� ������������� ����������� ����������� �� �������� ���������
			// ��� ������ �� ���� � ����������� dir
			Restart() ;
			// �������� �������
			rc = OnMessage(IDDUMSG, t, X) ; RRC ; 
		} 
	}
	// ����������� �������� ������ ����� ����� � ������ ������ �������� �������� �� ��� ��������
	// ��������: ����� ����� �� (��������� ������ �� � �.�.), ���� � ���� ��� ����� �� ��, ���������
	// ���������������� ������������� �� (�������� �������� ��� ��������� ������� ��������� �������)...
	// !!!!
	// � ��������� ����� �������� ������� � ������ ������ �������� �� ����������� �� �������
	// ����� ���������� � ����������� ����������
	// !!!!
	// ���������� ��� ��������������
	rc = StepSelf(0); if (rc) return rc;
	// �������� �����
	TestVit() ; 
	// ��������� ���������� 
	rc = StepCalc(1) ;
	// ��������� �������
	if (!rc) rc = StepBreak(*t, X);

	// ������������ ������� ����������� ���� ��������������
	if (notFurtherTime && BreakKey) {
		// ������ ����� �� ����� �������� ����� ���������� ��� ��������������
		if (*t == *notFurtherTime) *BreakKey = ModEvent_ONBREAKTIME ;
	}
	if (LSF.isDU) {
		// ��������� ������ ������� ������ �� ������� �� (���������, ����������...)


	}

	// ��������������� ���������� ��� ��������������
	m_cs.step_max = OldMaxH ;
	m_cs.step_min = OldMinH ;

	// ����������� ��������� ���������� �������� � ������ ����������
	if (!rc) {
		// ������ ���� ��� �������������� � ������� ��������� �������� (FSC)
		// ������������ �������� � ����������� ������ ������ ��������.
		// ������� ����������� ��������������� ����������� (FSC ---> FSR)
		memcpy(&FSR, &FSC, sizeof(FSC)) ; 
		// ���� ������ ��������� ��� ���������� �������� SI, �� ��� �� ��������� 
		// ������ ��������� �������� ����������� ��������.
		if (SI) memcpy(SI, &FSC, sizeof(FSC)) ; 
	}

	return rc ;
}

//-------------------------------------------------------------------------------
// ��������� ����������� � �������� ���������� ���� ��� ���������
// ������ �� ������� ������ ��������.
// ��� ���������� ��������� ��������� ������ �������� (�������� ��������������)
// �� ����������, � ������ � ���� �� ���������� ����� � ����������� ��������������
// ��������� ����������� � ��������� ZSC_SI* SI
int ZMSC::BackStep(ZSC_SI* SI, double h, ModEventName idevent) 
{
	double  t  = (*this)[0] ; 
	int     oldVit = Vitok75 ;
	int     olddir = GetDirect() ;
	int     dir = 1 ;
	int     rc = 0 ;

	// ���������� ����������� ����
	if (h<0) { h = fabs(h) ; dir =-1 ; }
	SetDirect(dir) ;
	// �������� �����
	PreTestVit() ;

	while(1) {
		double eps = 1e-13 ;
		double h2 = h ;
		// �������� ����� ������� ������������� �������� ��������� �����������
		// ������ � ������ �������� ������������� ���� �� � ���
		if (LSF.isDU) {
			long   IDDUMSG = 0 ;
			double maxstep = h2 ;
			MPU.TimeCheckForStep(t, dir, &IDDUMSG, &maxstep) ;
			// ��� ������������� ��������� ��� � ����� ������������ 
			// ����� � ����� ������� �� ���
			if (IDDUMSG) {
				// ���� ������� ����� �������
				// ���� ��������� ��� ������ ���������� �� �������� ������� ��
				// ����� �������, �� ��� ���������...
				if (h2>maxstep) h2 = maxstep ;
			}
		}
		h-= h2 ;
		//---------------------------------------------------------------------------
		// ���������� ��� ��������������
		// ��� ����������� �������� ������ ������������ �� ����� ������������ ��������������,
		// � ������ h, ������� ������ ������ h ������� � ������� 
		// ((double)dir)*h
		int rc = StepAdjusted(((double)dir)*h2, 0) ; if (rc) goto end ; 
		// ���� ��������� ����������� ����� ��������� �� ��������� ���� ����������� 
		if (fabs(h)<eps) break ;
		// � ��������� ������ ���������� ����� � ��� �� ������������ �� ��������� ����� �������
	}

	// �������� �����
	TestVit() ;
	// ��������� ���������� 
	rc=StepCalc(2, idevent, SI) ; 

end: ;
	SetDirect(olddir) ;
	Vitok75 = oldVit ;
	return rc ;
}

//-------------------------------------------------------------------------------
// ������ ��������� �� �� ���� �������� ���������
// ForTheCase:
//   0 - � ������ ������������� 
//   1 - ����� ���������� ���������� ����
//   2 - ����� ������������ �� ������ ��������������� �������,
//       � ��� ����� �� ������ ���������� ���������������
int ZMSC::StepCalc(char ForTheCase, ModEventName idevent, ZSC_SI* _SI) 
{
	int     rc = 0 ;
	ZSC_SI* SI = nullptr ;
	switch(ForTheCase) {
		case 0:
			SI = &FSC ;
			SI->E.ID = ModEvent_INIT ;
			SI->E.Value = SI->E.Value2 = SI->E.Value3 = SI->E.Value4 = 0.0 ;
			SI->E.Value5 = 0 ;
			memset(&FSE, 0,    sizeof(ZSC_SI)) ; 
			memset(&FSP, 0,    sizeof(ZSC_SI)) ; 
			memset(&FSR, 0,    sizeof(ZSC_SI)) ; 
			break ;
		case 1:
			SI = &FSC  ;
			SI->E.ID = ModEvent_STEP ;
			if (FSP.E.ID!=ModEvent_EMPTY) SI->E.Value = (FSC.ta-FSP.ta)*k_cbc ; 
			else SI->E.Value = 0 ;
			SI->E.Value2 = SI->E.Value3 = SI->E.Value4 = 0.0 ;
			SI->E.Value5 = 0 ;
			memcpy(&FSP, &FSC, sizeof(ZSC_SI)) ; 
			memset(&FSE, 0, sizeof(ZSC_SI)) ; 
			break ;
		case 2:
			SI = &FSE ;
			// ����� (������ �� �����������)
			SI->ta = SI->E.ta = (*this)[0] ;			
			// ������� - ����������� �����
			SI->E.ID = ModEvent_BKSTEP ;	
			// �������� ������� - ��� (�� ������� ���������) � ��������
			SI->E.Value = (FSC.ta-FSE.ta)*k_cbc ;	
			SI->E.Value2 = SI->E.Value3 = SI->E.Value4 = 0.0 ;
			SI->E.Value5 = 0 ;
			break ;
		default: assert(0) ;
	}

	SI->E.ta  = (*this)[0] ;
	SI->Vit75 = Vitok75 ;
	int rc1, rc2, rc3, rc4, rc5, rc6 ;
	rc1 = FillStepInfo75 (SI, (3|(SI->isOSK1975bpl?4:0))) ;
	rc2 = FillStepInfo00 (SI, (3|(SI->isOSK2000bpl?4:0))) ;
	rc3 = FillStepInfoTE (SI, (3|(SI->isOSKTEbpl  ?4:0))) ;
	rc4 = FillStepInfoTES(SI, (3|(SI->isOSKTESbpl ?4:0))) ;
	rc5 = FillStepInfoGSK(SI) ;
	rc6 = FillStepInfoTHBL(SI) ;

	if (rc1||rc2||rc3||rc4||rc5||rc6) {
		AddErrorMSG( "������ ���������� ������� ���������� ��������" ) ; 
		return 1 ;
	}

	// ������� ���������
	SI->ExternalEqCount	= ZCExternalEquations::CountExtEquations ;
	SI->ExternalEqOn	= ZCExternalEquations::IsExtEqOn() ;
	if (SI->ExternalEqCount) memcpy(SI->ExternalEq, &m_a[m_n], SI->ExternalEqCount*sizeof(double)) ;
	else memset(SI->ExternalEq, 0, EXTEQ_MAXCOUNT*sizeof(double)) ;

	// ����� (������ �� �����������)
	SI->ta = (*this)[0] ;
	// ���� ������
	rc1 = ZFUS(SI->ta, SI->GSK, SI->ASK2000, SI->SA);
	if (rc1) { AddErrorMSG("������ ������� ����� ������") ; return rc1 ;	}
	// ����
	rc1 = ZCalcMSSV(SI->ta, SI->THBLFR[3], SI->tmss) ; 
	if (rc1) { AddErrorMSG("������ ������� ����") ; return rc1 ;	}
	// ���
	TA_DMB(0, &SI->Data, &SI->Time, &SI->ta) ; 

	// �� � �������	
	TheDblTime Dblt = SI->ta-0.5 ; 
	Dblt+= TheT_JD1975 ;
	SI->T.fromIAT(Dblt) ;

	// �����
	SI->m = (*this)[NM()-1] ;

	if (idevent>0) {
		SI->E.ID = idevent ;
		// ��������� �������������� �������
		if (idevent==ModEvent_VITNU || idevent==ModEvent_VITVU || idevent==ModEvent_ONVIT) {
			SI->E.Value = SI->VitTE ;
		}
		// ������������ ������� ���� ��������������
		bool isZRVEvent = IsZRVEvent((ModEventName)idevent) ;
		if (isZRVEvent) {
			int indNip = IndNipZRVEvent((ModEventName)idevent) ;
			double Params[7] ;
			ZNIP* NIP =  &ALLNIPS[EventMng.INips(indNip)] ;
			CalcZRVParams(NIP, SI->GSK, Params) ;
			//
			SI->E.ID     = (ModEventName)IDZRVEvent((ModEventName)idevent) ;
			// ���� �����, ����
			SI->E.Value  = Params[0] ;
			// ������, ����
			SI->E.Value2 = Params[4] ;
			// ���������, �� 
			SI->E.Value3 = Params[2] ;
			// �������� ��������� ���������, ��/���
			SI->E.Value4 = Params[3] ;
			// ������ ���� ��� �������� ������� ��� �������
			SI->E.Value5 = indNip ;
		}
	}

	if (ForTheCase==0) {
		CheckVitEvent(false) ;  
		memcpy(&FSE, &FSC, sizeof(ZSC_SI)) ;
		memcpy(&FSR, &FSC, sizeof(ZSC_SI)) ;
	}
	if (ForTheCase==1) {
		// ���� �������������� �������
		// ��� ����������� ������� � �������� ���������� ���� ������� ������������� ����������
		rc = EventMng.TestEvents(this) ; 
		memcpy(&FSR, &FSC, sizeof(ZSC_SI)) ;
	}

	if(_SI) memcpy(_SI, SI, sizeof(ZSC_SI)) ;
	return rc ;
}

//-------------------------------------------------------------------------------
int ZMSC::FillStepInfo75(ZSC_SI* SI, int var) 
{
	double* X75 = &((*this)[1]) ;
	if (var&1 || var&2) memcpy(SI->ASK1975, X75, 6*sizeof(double)) ;
	if (var&2) ZASCtoOSC( SI->ASK1975, SI->OSK1975) ;
	if (var&4) {
		if (KA.bpl==0) memcpy(SI->OSK1975bpl, SI->OSK1975, 6*sizeof(double)) ;
		else {
			double T[6] ; ASKtoASKbpl(SI->ASK1975, T) ;
			ZASCtoOSC(T, SI->OSK1975bpl) ;
	}	}
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZMSC::FillStepInfoGSK(ZSC_SI* SI) 
{
	double  t = (*this)[0] ;
	double* X75 = &((*this)[1])  ;

	ZASC1975toGSC(t, X75, SI->GSK, 0) ;   
	SI->VitTE = VitSK(X75, Vitok75, SI->GSK) ;
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZMSC::FillStepInfoTHBL(ZSC_SI* SI) 
{
	double  t = (*this)[0] ;
	double* X75 = &((*this)[1]) ;

	// ����� �� ���� ���������� �������� � ���
	ZASC1975toGSC(t, X75, SI->GSK, 0) ;	

	ZGRtoHBL(SI->GSK, &(SI->THBLFR[1]), 0, 0) ;
	SI->THBLFR[4] = asin(SI->GSK[2]/FR(SI->GSK)) ;
	SI->THBLFR[5] = FR(SI->ASK2000) ;

	double a = SI->OSK1975[0] ;
	double mu= GPZ.GravPole.B0km ;
	// ������������ ������ � ��������
	a*= 1e4 ;
	SI->THBLFR[0] = k_dwapi*sqrt(a*a*a/mu) ;

	SI->THBLFR[1] *= 1e4 ;
	SI->THBLFR[2] *= M_GRARAD;
	SI->THBLFR[3] *= M_GRARAD;
	SI->THBLFR[4] *= M_GRARAD;
	SI->THBLFR[5] *= 1e4;
	if (SI->THBLFR[3]<0) SI->THBLFR[3]+= 360.0 ; 

	double hht[4] ;
	// hht[0] - ������ ��� ���,
	// hht[1] - ������,
	// hht[2] - ����������� ������ �� �������,
	// hht[3] - ����������� ������� �� �������.
	ZCalcHightDif(SI->GSK, hht, false) ;
	SI->THBLFR[6] = hht[2] ; // ����������� ������ �� �������,
	SI->THBLFR[7] = hht[3] ; // ����������� ������� �� �������.
	SI->THBLFR[8] = KoordVitSK(SI, _SK_ASKTE) ; 

	return 0 ;
}

//-------------------------------------------------------------------------------
int ZMSC::FillStepInfo00(ZSC_SI* SI, int var) 
{
	double* X75 = &((*this)[1]) ;
	if (var&1 || var&2) {
		ZASC_1975to2000(X75, SI->ASK2000, 1) ; 
		SI->Vit00 = VitSK(X75, Vitok75, SI->ASK2000) ;
	}
	if (var&2) ZASCtoOSC(SI->ASK2000, SI->OSK2000) ;
	if (var&4) {
		if (KA.bpl==0) memcpy(SI->OSK2000bpl, SI->OSK2000, 6*sizeof(double)) ;
		else {
			double T[6] ; ASKtoASKbpl(SI->ASK2000, T) ;
			ZASCtoOSC(T, SI->OSK2000bpl) ;
	}	}
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZMSC::FillStepInfoTE(ZSC_SI* SI, int var) 
{
	double  t = (*this)[0] ;
	double* X75 = &((*this)[1]) ;
	if (var&1 || var&2) {
		ZASC_1975toTE(t, X75, SI->ASKTE, 1) ;
		SI->VitTE = VitSK(X75, Vitok75, SI->ASKTE) ;
	}
	if (var&2) ZASCtoOSC(SI->ASKTE, SI->OSKTE) ;
	if (var&4) {
		if (KA.bpl==0) memcpy(SI->OSKTEbpl, SI->OSKTE, 6*sizeof(double)) ;
		else {
			double T[6] ; ASKtoASKbpl(SI->ASKTE, T) ;
			ZASCtoOSC(T, SI->OSKTEbpl) ;
	}	}
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZMSC::FillStepInfoTES(ZSC_SI* SI, int var) 
{
	double  t = (*this)[0] ;
	double* X75 = &((*this)[1]) ;

	if (var&1 || var&2) {
		ZASC_1975toTES(t, X75, SI->ASKTES, 1) ;
		SI->VitTES = VitSK(X75, Vitok75, SI->ASKTES) ;
	}
	if (var&2) ZASCtoOSC(SI->ASKTES, SI->OSKTES) ;
	if (var&4) {
		if (KA.bpl==0) memcpy(SI->OSKTESbpl, SI->OSKTES, 6*sizeof(double)) ;
		else {
			double T[6] ; ASKtoASKbpl(SI->ASKTES, T) ;
			ZASCtoOSC(T, SI->OSKTESbpl) ;
	}	}
	return 0 ;
}

//-------------------------------------------------------------------------------

