//-------------------------------------------------------------------------------
// ZMSpaceCraft.cpp
// А. Николенко 03.08.2018
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>
#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZCISAManager.h>
#include <FlyCoreSource\\ZMForceAtmSMA81.h>
#include <FlyCoreSource\\ZMForceAtmSMA62.h>
#include <FlyCoreSource\\ZMForceAtmSMAGost440181.h>
#include <FlyCoreSource\\ZMForceAtm.h>
#include <FlyCoreSource\\ZHBL.h>

//-------------------------------------------------------------------------------
ZMSC::ZMSC( ) : ZCIntegrator( ) 
{ 
	memset(&FSC,0, sizeof(FSC)) ;
	memset(&FSP,0, sizeof(FSP)) ;
	memset(&FSE,0, sizeof(FSP)) ;
	memset(&FSR,0, sizeof(FSP)) ;
	memset(&FV, 0, sizeof(FV )) ;
	memset(&KA, 0, sizeof(ZKA)) ;
	memset(&NU, 0, sizeof(ZNU)) ;
	memset(&LSF,0, sizeof(ZLSF)) ;

	Vitok75 = 0 ;

	m_ISAManager   = 0 ;
	m_AtmSMA81     = 0 ;
	m_AtmSMA62     = 0 ;
	m_AtmSMA440181 = 0 ;

	HStoryClarify = 0 ;

	PreInitInfo.defautFixedStep = 0;
}

//-------------------------------------------------------------------------------
ZMSC::~ZMSC( ) 
{ 
	Clear( ) ;
	m_pNotice = 0 ;
}

//-------------------------------------------------------------------------------
ZSC_VP* HMIN_VI(ZSC_VI& I) 
{
	ZSC_VP* pVP=nullptr ;
	if (I.Hmin.isFind) pVP = &I.Hmin ;
	if (I.Hmin2.isFind) {
		if (I.Hmin2.H<I.Hmin.H) {
			pVP = &I.Hmin2 ;
			if (I.Hmin3.isFind && I.Hmin3.H<I.Hmin2.H) pVP = &I.Hmin3 ;
		} else {
			if (I.Hmin3.isFind && I.Hmin3.H<I.Hmin.H) pVP = &I.Hmin3 ;
	}	}
	return pVP ;
}

ZSC_VP* HMAX_VI(ZSC_VI& I) 
{
	ZSC_VP* pVP=nullptr ;
	if (I.Hmax.isFind) pVP = &I.Hmax ;
	if (I.Hmax2.isFind) {
		if (I.Hmax2.H>I.Hmax.H) {
			pVP = &I.Hmax2 ;
			if (I.Hmax3.isFind && I.Hmax3.H>I.Hmax2.H) pVP = &I.Hmax3 ;
		} else {
			if (I.Hmax3.isFind && I.Hmax3.H>I.Hmax.H) pVP = &I.Hmax3 ;
	}	}
	return pVP ;
}

ZSC_VP* RMIN_VI(ZSC_VI& I) 
{
	ZSC_VP* pVP=nullptr ;
	if (I.Rmin.isFind) pVP = &I.Rmin ;
	if (I.Rmin2.isFind) {
		if (I.Rmin2.H<I.Rmin.H) {
			pVP = &I.Rmin2 ;
			if (I.Rmin3.isFind && I.Rmin3.H<I.Rmin2.H) pVP = &I.Rmin3 ;
		} else {
			if (I.Rmin3.isFind && I.Rmin3.H<I.Rmin.H) pVP = &I.Rmin3 ;
	}	}
	return pVP ;
}

ZSC_VP* RMAX_VI(ZSC_VI& I) 
{
	ZSC_VP* pVP=nullptr ;
	if (I.Rmax.isFind) pVP = &I.Rmax ;
	if (I.Rmax2.isFind) {
		if (I.Rmax2.H>I.Rmax.H) {
			pVP = &I.Rmax2 ;
			if (I.Rmax3.isFind && I.Rmax3.H>I.Rmax2.H) pVP = &I.Rmax3 ;
		} else {
			if (I.Rmax3.isFind && I.Rmax3.H>I.Rmax.H) pVP = &I.Rmax3 ;
	}	}
	return pVP ;
}

//-------------------------------------------------------------------------------
void ZMSC::Clear( ) 
{	
	FreeStory( ) ;
	if (m_ISAManager  ) delete m_ISAManager ; m_ISAManager=0 ;
	if (m_AtmSMA81    ) delete m_AtmSMA81 ; m_AtmSMA81=0 ;
	if (m_AtmSMA62    ) delete m_AtmSMA62 ; m_AtmSMA62=0 ;
	if (m_AtmSMA440181) delete m_AtmSMA440181 ; m_AtmSMA440181=0 ;

	ZCIntegrator::Delete( ) ; 
}

//-------------------------------------------------------------------------------
int ZMSC::Init(ZKEYNU& NuKey, ZCExternalEquationsDefinition* EED) 
{
	ClearErrorsMSG();
	int rc = FlyGlobalInit(); if (rc) return rc;

	ZNU nu;
	// Загружаем НУ (из файла или БД или ..... 
	rc = LoadNU(NuKey, nu); if (rc) return rc;
	// Инициализируем МД - задаем начальное значение вектора (из НУ)
	// выделяем память, загружаем ЛЩС, параметры ГПЗ, параметры модели атмосферы и т.д.
	// После этого МД готова к интегрированию СДУ
	rc = Init(nu, nullptr, EED); if (rc) return rc;
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZMSC::Init(ZNU& nu_, ZLSF* lsf_, ZCExternalEquationsDefinition* EED) 
{
	int rc ;
	NU = nu_ ;

	if (EED) {
		ExtEqDef =*EED ;
		SetExternalEquationsCount(ExtEqDef.Size()) ;
	}

	// Переводим НУ в СК интегрирования СДУ
	rc = ConvertSkNu(NU, _SK_ASK1975) ; 
	if (rc) { AddErrorMSG( "Ошибка преобразования заданных НУ в АСК 1975 года") ; goto end ; }
	// Загрузка информации о КА
	if (KA.num!=NU.Key.ka) {
		rc = LoadKA( NU.Key.ka, KA) ; if (rc) goto end ; ;
		if (PreInitInfo.defautFixedStep > 0) {
			KA.min_step = KA.max_step = PreInitInfo.defautFixedStep;
		}
	}
	// Загрузка данных логической шкалы сил
	if (lsf_) {
		memcpy(&LSF, lsf_, sizeof(LSF)); 
		NU.numlsf = LSF.num ; 
	} else {
		if (NU.numlsf!=LSF.num) {
			rc = LoadLSF(NU.numlsf, LSF) ; if ( rc ) goto end ;
		}
	}

	//---------------------------------------------------------------------------
	// Создание объектов атмосферы и загрузка параметров модели атмосферы 
	// При необходимости создание диспетчера индексов СА
	if (LSF.vatm>=ATMDMAGOST1977 && LSF.vatm<=ATMMSIS2000) {
		if (!m_ISAManager) m_ISAManager = new ZCISAManager ; 
		if (!m_ISAManager) { rc = 1 ; goto end ; }
	}

	// При необъодимости создаётся объект рассчёта параметров статической атмосферы
	if (LSF.vatm==ATMSMA81 ) { 
		if(!m_AtmSMA81 ) m_AtmSMA81 = new ZMAtmSMA81 ; 
		if (!m_AtmSMA81) { rc = 1 ; goto end ; } 
	}
	if (LSF.vatm==ATMSMA62 ) { 
		if(!m_AtmSMA62 ) m_AtmSMA62 = new ZMAtmSMA62 ; 
		if (!m_AtmSMA62) { rc = 1 ; goto end ; } 
	}
	if (LSF.vatm==ATMSMA440181) { 
		if(!m_AtmSMA440181) m_AtmSMA440181 = new ZMAtmSMA440181 ; 
		if (!m_AtmSMA440181) { rc = 1 ; goto end ; } 
	}

	// Загрузка модели гравитационного поля Земли
	rc = LoadGravPole(&GPZ, LSF.vgpz, LSF.ngpz, LSF.mgpz) ; if (rc) goto end ;

	IntegratorCreateStruct cs ;
	// Интегрируем параметры t, X, Y, Z, Vx, Vy, Vz, m - всего 8 параметров
	cs.n          = 8 ;
	// Метод интегрирования РК 8-го порядка
	cs.RungeType  = ID_RUNGE_8 ;
	cs.AdamsType  = 0 ; //ID_ADAMS_8 ;
	cs.step_min   = KA.min_step / k_cbc ;
	cs.step_max   = KA.max_step / k_cbc ;
	cs.step       = cs.step_min ;
	cs.fixed_step = PreInitInfo.defautFixedStep / k_cbc;
	cs.step_eps   = 1e-13 ;
	// С автоматическим выбором шага
	cs.flag       = F_RUNA_AUTOSTEP ;
	cs.data       = 0 ;
	cs.sph        = 0 ;
	cs.user_break = 0 ;
	cs.arg_index  = 0 ;
	rc = Create(cs) ; if (!rc) { AddErrorMSG( "Ошибка создания интегратора" ) ; return rc ; }

	Vitok75 = NU.vit ;
	// Выкладываем вектор в интегратор
	rc = ZCIntegrator::SetVector(NU.t, NU.X, WeightFullNU()) ; if (rc) goto end ;
	// Расчет всех кинематических параметров и оскулирующих элементов орбиты
	// в начальной точке - в точке НУ
	StepCalc(0) ;
	// Инициация сообщения об изменении интегрируемого вектора 
	rc = OnMessage(MMSG_SET_VECTOR, &(*this)[0], &((*this)[1])) ; if (rc) goto end ;

end : ;
	if (rc) { AddErrorMSG( "Ошибка инициализации модели движения КА" ) ; return rc ; }
	return 0 ;
}

//-------------------------------------------------------------------------------
double ZMSC::GetTime() const
{
	const ZSC_SI* FS = &(FSR.E.ID==ModEvent_EMPTY ? FSC:FSR) ;
	return(FS->ta) ;
}

//-------------------------------------------------------------------------------
long ZMSC::GetVitok(SK_TYPE sk, ZSC_SI* F) const
{
	const ZSC_SI* FS = F? F:&(FSR.E.ID==ModEvent_EMPTY ? FSC:FSR) ;
	switch(sk) {
		case _SK_ASK1975: return FS->Vit75 ; break ;
		case _SK_ASK2000: return FS->Vit00 ; break ;
		case _SK_ASKTE  : return FS->VitTE ; break ;
		case _SK_ASKTES : return FS->VitTES ; break ;
	}
	return 0 ;
}

//-------------------------------------------------------------------------------
double ZMSC::GetU(SK_TYPE sk, ZSC_SI* F, bool isbpl) const
{
	const ZSC_SI* FS = F? F:&(FSR.E.ID==ModEvent_EMPTY ? FSC:FSR) ;
	switch(sk) {
		case _SK_ASK1975: return(isbpl?FS->OSK1975bpl[5]:FS->OSK1975[5]) ; break ;
		case _SK_ASK2000: return(isbpl?FS->OSK2000bpl[5]:FS->OSK2000[5]) ; break ;
		case _SK_ASKTE  : return(isbpl?FS->OSKTEbpl[5]  :FS->OSKTE[5]) ; break ;
		case _SK_ASKTES : return(isbpl?FS->OSKTESbpl[5] :FS->OSKTES[5]) ; break ;
	}
	return 0 ;
}

//-------------------------------------------------------------------------------
void ZMSC::GetKA(ZKA& ka) const { ka = KA; }
//-------------------------------------------------------------------------------
void ZMSC::GetNU(ZNU& nu) const { nu = NU; }
//-------------------------------------------------------------------------------
int ZMSC::SetNU(ZNU& nu)
{ 
	NU = nu ; 	
	SetVector(NU.X, NU.vit, NU.t, WeightFullNU(), NU.sk) ; 
	return 0 ; 
}  

//-------------------------------------------------------------------------------
double ZMSC::WeightNU()     const {return(NU.M0>0 ? NU.M0:KA.M0);}
double ZMSC::WeightFullNU() const {return(WeightNU() + (NU.MF>0 ? NU.MF:KA.MF));}
double ZMSC::WeightFuel()   const {return (m_a[7] - (NU.M0>0 ? NU.M0:KA.M0));}
double ZMSC::WeightFull()   const {return m_a[7];}

//-------------------------------------------------------------------------------
void ZMSC::GetLSF(ZLSF& lsf) const {lsf = LSF;}

//-------------------------------------------------------------------------------
int ZMSC::SetVector(double* X, long Vit, double t, double m, SK_TYPE sk) 
{
	double X75[7], T[7] ;
	int    rc = 0 ;

	X75[0] = t ;
	if (sk==_SK_ASK1975 || sk==_SK_OSK1975) {
		if (sk==_SK_OSK1975) ZOSCtoASC(X, &X75[1]) ;
		else memcpy(&X75[1], X, 6*sizeof(double)) ;
		Vitok75 = Vit ;		
	}
	if (sk==_SK_ASK2000 || sk==_SK_OSK2000) {
		if (sk==_SK_OSK2000) ZOSCtoASC(X, T) ;
		else memcpy(T, X, 6*sizeof(double)) ;
		rc = ZASC_2000to1975(T, &X75[1], 1) ; RRC ;
		TestEpsVit(sk, X, Vit, _SK_ASK1975, &X75[1], Vitok75) ;
		//Vitok75 = VitSK(T, Vit, &X75[1]) ;
	}
	if (sk==_SK_ASKTE || sk==_SK_OSKTE) {
		T[0] = t ;
		if (sk==_SK_OSKTE ) ZOSCtoASC(X, &T[1]) ;
		else memcpy(&T[1], X, 6*sizeof(double)) ;
		ZASC_TEto1975(t, &T[1], &X75[1], 1) ;
		TestEpsVit(sk, X, Vit, _SK_ASK1975, &X75[1], Vitok75) ;
		//Vitok75 = VitSK(&T[1], Vit, &X75[1]) ;
	}
	if (sk==_SK_ASKTES || sk==_SK_OSKTES) {
		T[0] = t ;
		if (sk==_SK_OSKTE) ZOSCtoASC(X, &T[1]) ;
		else memcpy(&T[1], X, 6*sizeof(double)) ;
		ZASC_TESto1975(t, &T[1], &X75[1], 1) ;
		TestEpsVit(sk, X, Vit, _SK_ASK1975, &X75[1], Vitok75) ;
		//Vitok75 = VitSK(&T[1], Vit, &X75[1]) ;
	}
	if (sk==_SK_GSK) {
		rc = ZGSCtoASC1975(t, X, &X75[1], 0) ; 
		TestEpsVit(sk, X, Vit, _SK_ASK1975, &X75[1], Vitok75) ;
		//Vitok75 = VitSK(X, Vit, &X75[1]) ;
	}
	rc = ZCIntegrator::SetVector(X75[0], &X75[1], m) ; RRC ;
	StepCalc(0) ;
	rc = OnMessage(MMSG_SET_VECTOR, &(*this)[0], &((*this)[1])) ; RRC ;
	
	return rc ; 
}

//-------------------------------------------------------------------------------
int ZMSC::GetVector(double* pX, long* Vit, 
					double* pt, double* pm, SK_TYPE sk) const
{
	int rc = 0 ;
	const ZSC_SI* FS = &(FSR.E.ID==ModEvent_EMPTY ? FSC:FSR) ;

	if (pt) *pt = FS->ta ;
	if (pm) *pm = FS->m ;

	switch(sk) { 
	case _SK_ASK1975:
		if (Vit) *Vit = FS->Vit75 ;
		if (pX ) memcpy( pX, FS->ASK1975, 6*sizeof(double)) ;
		break ;
	case _SK_OSK1975:
		if (Vit) *Vit = FS->Vit75 ;
		if (pX ) memcpy( pX, FS->OSK1975, 6*sizeof( double ) ) ;
		break ;
	case _SK_GSK:
		if (Vit) *Vit = FS->VitTE ;
		if (pX ) memcpy( pX, FS->GSK, 6*sizeof( double ) ) ;
		break ;
	case _SK_ASK2000:
		if (Vit) *Vit = FS->Vit00 ;
		if (pX ) memcpy( pX, FS->ASK2000, 6*sizeof( double ) ) ;
		break ;
	case _SK_OSK2000:
		if (Vit) *Vit = FS->Vit00 ;
		if (pX ) memcpy( pX, FS->OSK2000, 6*sizeof( double ) ) ;
		break ;
	case _SK_ASKTE:
		if (Vit) *Vit = FS->VitTE ;
		if (pX ) memcpy( pX, FS->ASKTE, 6*sizeof( double ) ) ;
		break ;
	case _SK_OSKTE:
		if (Vit) *Vit = FS->VitTE ;
		if (pX ) memcpy( pX, FS->OSKTE, 6*sizeof( double ) ) ;
		break ;
	case _SK_ASKTES:
		if (Vit) *Vit = FS->VitTES ;
		if (pX ) memcpy( pX, FS->ASKTES, 6*sizeof( double ) ) ;
		break ;
	case _SK_OSKTES:
		if (Vit) *Vit = FS->VitTES ;
		if (pX ) memcpy( pX, FS->OSKTES, 6*sizeof( double ) ) ;
		break ;		
	}
	return rc ;
}

//-------------------------------------------------------------------------------
int ZMSC::StepBreak( double t, double* X ) 
{
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZMSC::OnMessage( long ModMsgID, double* t, double* X) 
{
	//if ( ModMsgID == MMSG_TDU_ON     && SignalOnStartDU ) { SignalOnStartDU( this, MMSG_TDU_ON, ( void* )&t, ( void* )X ) ; return 0 ; } 
	//if ( ModMsgID == MMSG_TDU_OFF    && SignalOnEndDU   ) { SignalOnEndDU  ( this, MMSG_TDU_OFF, ( void* )&t, ( void* )X ) ; return 0 ; }
	//if ( ModMsgID == MMSG_SET_VECTOR && SignalOnSetVec  ) { SignalOnSetVec ( this, MMSG_SET_VECTOR, ( void* )&t, ( void* )X ) ; return 0 ; } 
	//if ( ModMsgID == MMSG_RESTORY    && SignalOnRestory ) { SignalOnRestory( this, MMSG_RESTORY, ( void* )&t, ( void* )X ) ; return 0 ; }
	//if ( ModMsgID == MMSG_START      && SignalOnStart   ) { SignalOnStart  ( this, MMSG_START, ( void* )&t, ( void* )X ) ; return 0 ; } 
	//if ( ModMsgID == MMSG_END        && SignalOnEnd     ) { SignalOnEnd    ( this, MMSG_END, ( void* )&t, ( void* )X ) ; return 0 ; }
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZMSC::OnEvent(ZMOD_EVENT& E, ZSC_SI* FSCE) 
{
	if (IsVITEvent(E.ID)) return OnVitokEvent(E, FSCE) ;
	if (IsZRVEvent(E.ID)) return OnZRVEvent(E, FSCE) ;
	if (IsUserEvent(E.ID)) return OnUserEvent(E, FSCE) ;
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZMSC::OnFullZRV(ZZRV* ZRV, ZSC_SI* FSC) 
{
	return 0 ;
}

//-------------------------------------------------------------------------------
void ZMSC::ASKtoASKbpl(double* X, double* Xbpl) 
{
	// Базовая плоскость 0 - экватор, 1 - XOZ, 2 - YOZ	
	if (KA.bpl==0) {
		memcpy(Xbpl, X, 6*sizeof(double)) ;
		return ;
	}
	if (KA.bpl==1) {
		// Z -> X,  X -> Y,  Y -> Z
		Xbpl[0]=X[2] ; Xbpl[1]=X[0] ; Xbpl[2]=X[1] ;
		Xbpl[3]=X[5] ; Xbpl[4]=X[3] ; Xbpl[5]=X[4] ;
		return ;
	}
	if ( KA.bpl == 2 ) {
		// Z -> X,  Y -> Y,  X -> -Z
		Xbpl[0]=X[2] ; Xbpl[1]=X[1] ; Xbpl[2]=-X[0] ;
		Xbpl[3]=X[5] ; Xbpl[4]=X[4] ; Xbpl[5]=-X[3] ;
		return ;
	}
}

//-------------------------------------------------------------------------------
void ZMSC::ExternalEquationsOn (double* NU) 
{
	// "Включаем" внешние уравнения
	ZCIntegrator::ExternalEquationsOn(NU) ;
	// Копируем НУ для внешних уравнений в конец интегрируемого вектора
	ZSC_SI* SI = &FSC ;
	SI->ExternalEqCount = GetExternalEquationsCount() ;
	SI->ExternalEqOn = true ;
	if (NU)	memcpy(&SI->ExternalEq, NU, GetExternalEquationsCount()*sizeof(double)) ;
	else memset(&SI->ExternalEq, 0, GetExternalEquationsCount()*sizeof(double)) ;
}

//-------------------------------------------------------------------------------
void ZMSC::ExternalEquationsOff(double* X) 
{
	// "Включаем" внешние уравнения
	ZCExternalEquations::ExternalEquationsOff(X) ;
	// Копируем НУ для внешних уравнений в конец интегрируемого вектора
	ZSC_SI* SI = &(FSR.E.ID==ModEvent_EMPTY ? FSC:FSR) ;
	SI->ExternalEqOn = false ;
	if (X)	memcpy(X, &SI->ExternalEq, GetExternalEquationsCount()*sizeof(double)) ;
}

//-------------------------------------------------------------------------------
int ZMSC::ExternalSph(double t, double* mainX, double* Xexe, double* Yexe) 
{
	int i, N = ExtEqDef.Size() ;
	if (!N) return 0 ;

	int rc = 0 ;
	double ASC_TE[6], OE_TE[6] ;
	rc = ZASC_1975toTE(t, mainX, ASC_TE, true) ; RRC ;
	ZASCtoOSC(ASC_TE, OE_TE) ;

	for (i=0; i<N; i++) {
		unsigned int IDExt = ExtEqDef.ID[i] ;
		switch(IDExt) { 
		case EXTEQ_ID_avrT_H:
			double GSK[6] ;
			rc = ZASC1975toGSC(t, mainX, GSK, 0) ;	RRC ;
			double HBL[3] ;
			ZGRtoHBL(GSK, HBL, 0, 0) ;
			Yexe[i] = 1e4*HBL[0] ;
			break ;
		case EXTEQ_ID_avrT_ex:
			Yexe[i] = OE_TE[1]*cos(OE_TE[4]) ;
			break ;
		case EXTEQ_ID_avrT_ey:
			Yexe[i] = OE_TE[1]*sin(OE_TE[4]) ;
			break ;
		case EXTEQ_ID_avrT_ix:
			Yexe[i] = tan(OE_TE[2]/2.0)*cos(OE_TE[3]) ;
			break ;
		case EXTEQ_ID_avrT_iy:
			Yexe[i] = tan(OE_TE[2]/2.0)*sin(OE_TE[3]) ;
			break ;
		}
	}
	return 0 ;
}

//-------------------------------------------------------------------------------