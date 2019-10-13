//-------------------------------------------------------------------------------
// ZCIntegrator.cpp
// А. Николенко 03.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>
#include <FlyMMath.h>

#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZCIntegrator.h>
#include <FlyCoreSource\\ModType.h>

//-------------------------------------------------------------------------------
ZCIntegrator::ZCIntegrator(IntegratorCreateStruct* cs) : 
			  ZCExternalEquations(), 
			  ZCMemoryKeeper()
{
	m_n = 0 ;
	m_N = 0 ;
	m_a = 0 ;
    m_ok_nu = 0 ;
	m_x = 0 ;
	m_d = 0 ;
	m_IsLockBreak = false ;
	m_IsWasStepCutDown = false ;
	memset(GI, 0, (2*MAX_ADAMS_N+1)*sizeof(double*)) ;
	if (cs) Create(*cs) ;
}
//-------------------------------------------------------------------------------
ZCIntegrator::ZCIntegrator(ZCIntegrator& Runge) : 
			  ZCExternalEquations(), 
			  ZCMemoryKeeper()
{
	m_ok_nu = 0 ;
	Create(Runge.m_cs, Runge.m_a) ;
}

//-------------------------------------------------------------------------------
ZCIntegrator::~ZCIntegrator ( )
{
	Delete( ) ;
}

//-------------------------------------------------------------------------------
int ZCIntegrator::Create( IntegratorCreateStruct& cs, double* v )
{
	int rc = 1 ; 

	FAST_SEMAPFORE_OPEN("ZCIntegratorCreate") ;

	Delete ( ) ;

	m_cs = cs ;
	SetStep(m_cs.step, m_cs.step_min, m_cs.step_max) ;
	m_n = m_cs.n ;
	m_N = m_n+CountExtEquations ;
	if (m_N>0) { 
		m_a = new double[m_N] ; memset(m_a, 0, m_N*sizeof(double)) ; 
	}

    if (v && m_n>0) {
		m_ok_nu = 1 ;
		memcpy(m_a, v, m_n*sizeof(double)) ;
    } else m_ok_nu = 0 ;

    if (IsOkData(m_cs)) {
		ZCMemoryKeeper::Bind(m_a, m_N) ;
		CreateRK () ;
		CreateAdm() ;
	} else rc = 0 ;

	FAST_SEMAPFORE_CLOSE() ;
    return rc ;
}

//-------------------------------------------------------------------------------
void ZCIntegrator::Delete ( )
{
	FAST_SEMAPFORE_OPEN("ZCIntegratorDelete") ;

	int i ;
	m_n = 0 ;
	m_N = 0 ;
	if (m_x) delete[] m_x ; m_x = 0 ;
	if (m_a) delete[] m_a ; m_a = 0 ;
	if (m_d) {
		for (i=0 ; i<m_ns ; i++)  { 
			if (m_d[i]) {
				delete[] m_d[i] ; m_d[i]=0 ; 
		}	} 
		delete[] m_d ; m_d = 0 ;
    }
	for (i=0 ; i<2*MAX_ADAMS_N+1; i++) {
		if (GI[i]) { 
			delete[] GI[i] ; 
			GI[i]=0 ; 
		}
	}
	CountGI = 0 ;
	FreeStory(0) ;

	FAST_SEMAPFORE_CLOSE() ;
}

//-------------------------------------------------------------------------------
ZCIntegrator& ZCIntegrator::operator = ( double* v )
{
	if (m_n<=0 || !m_a) return *this ;
   	memcpy((void*)m_a, v, m_n*sizeof(double)) ;
	return *this ;
}

//-------------------------------------------------------------------------------
ZCIntegrator& ZCIntegrator::operator = (ZCIntegrator& Runge)
{
	Create(Runge.m_cs, Runge.m_a) ;
	return *this ;
}

//-------------------------------------------------------------------------------
double& ZCIntegrator::operator [] (int    i) { assert(i>=0 && i<m_N) ; return m_a[i] ; }
double& ZCIntegrator::operator [] (short  i) { assert(i>=0 && i<m_N) ; return m_a[i] ; }
double& ZCIntegrator::operator [] (long   i) { assert(i>=0 && i<m_N) ; return m_a[i] ; }
double& ZCIntegrator::operator [] (float  i) { assert(i>=0 && i<m_N) ; return m_a[(long)i] ; }
double& ZCIntegrator::operator [] (double i) { assert(i>=0 && i<m_N) ; return m_a[(long)i] ; }

//-------------------------------------------------------------------------------
int ZCIntegrator::IsOkData( IntegratorCreateStruct& cs ) { return ( cs.n <= 0 ? 0 : cs.n ) ; }
int ZCIntegrator::IsOkCreate( ) { return( m_n <= 0 ? 0 : m_n ) ; }

//-------------------------------------------------------------------------------
int ZCIntegrator::SetVector (double* x)
{
	if (m_n<=0) return 1 ;
    m_ok_nu = 1 ;
    (*this) = x ;
	Restart() ;
    SetStart() ;
    return 0 ;
}

//-------------------------------------------------------------------------------
int ZCIntegrator::SetVector (double t, double* x, double m)
{
	if (m_n<=0) return 1 ;
    m_ok_nu = 1 ;
   	memcpy((void*)(&m_a[1]), x, (m_n-2)*sizeof(double)) ;
	m_a[0] = t ;
	m_a[m_n-1] = m ;
	Restart() ;
    SetStart() ;
    return 0 ;
}

//-------------------------------------------------------------------------------
int ZCIntegrator::SetVectorExe(double* xexe)
{
	if (m_n<=0) return 1 ;
	int N = NE() ; if (N<=m_n) return 0 ; 
	memcpy(&m_a[m_n], xexe, (N-m_n)*sizeof(double));
	Restart() ;
    SetStart() ;
    return 0 ;
}

//-------------------------------------------------------------------------------
void ZCIntegrator::GetVector(double* x) 
{ 
	memcpy(x, m_a, m_n*sizeof(double)) ; 
}

//-------------------------------------------------------------------------------
void ZCIntegrator::GetVector(double& t, double* x, double* m) 
{ 
	memcpy(x, (void*)(&m_a[1]), (m_n-2)*sizeof(double)) ;
	t = m_a[0] ;
	if (m) *m=m_a[m_n-1] ;
}

//-------------------------------------------------------------------------------
void ZCIntegrator::GetVectorExe(double* xexe)
{
	int N = NE() ; if (N<=m_n) return ; 
	memcpy(xexe, &m_a[m_n], (N-m_n)*sizeof(double));
}

//-------------------------------------------------------------------------------
int  ZCIntegrator::GetDirect(void) { return (( int )m_up ); }
void ZCIntegrator::SetDirect(int up) { m_up = up < 0 ? -1. : 1. ; }

//-------------------------------------------------------------------------------
void ZCIntegrator::SetStart ( double str_step )
{
	if ( str_step != 0.0 ) {
		m_up      = str_step < 0 ? -1. : 1. ;
		m_cs.step = fabs( str_step ) ;
	}
	m_step = m_cs.step ;
}

//-------------------------------------------------------------------------------
void ZCIntegrator::SetStep (double step) { SetStep ( step, step/10.0, step*10.0) ; }
void ZCIntegrator::SetStep (double step, double min_step, double max_step)
{
	m_up            = step >= 0 ? 1. : -1. ;
	m_cs.fixed_step = fabs(m_cs.fixed_step) ;
    m_cs.step       = fabs( step ) ;
    m_cs.step_min   = fabs( min_step ) ;
    m_cs.step_max   = fabs( max_step ) ;
    m_step          = m_cs.fixed_step>0 ? m_cs.fixed_step:fabs( step ) ;
}

//-------------------------------------------------------------------------------
void ZCIntegrator::SetAutoStep (bool autostep )
{
    if (autostep) m_cs.flag|= F_RUNA_AUTOSTEP ;
	else		  m_cs.flag&=~F_RUNA_AUTOSTEP ;
}

//-------------------------------------------------------------------------------
void ZCIntegrator::SetEpsd ( double epsd )
{
	m_cs.step_eps = epsd ;
    m_eps  = m_cs.step_eps > 13.0 * 1e-15 ? m_cs.step_eps : 13.0 * 1e-15 ;
    m_eps2 = 2 * 1e-15 / m_eps ;
}

//-------------------------------------------------------------------------------
void ZCIntegrator::SetData(void* data) { m_cs.data = data ; }
void ZCIntegrator::SetType(int type) { m_cs.RungeType = type ; SetStart() ; }

//-------------------------------------------------------------------------------
void ZCIntegrator::SetSph(int (*Sph)(double*, double*, void*)) { m_cs.sph = Sph; SetStart() ; }
void ZCIntegrator::SetUserBreak(int (*user_break)(double*, double*)) { m_cs.user_break = user_break ; }

//-------------------------------------------------------------------------------
// Интегрирование СДУ до заданного значения независимой переменной
int ZCIntegrator::GoToArg ( double arg )
{
	// Если не заданы НУ или заданы не корректно то аварийный выход 
    if (!m_ok_nu) return 1 ;
	// Сохраняем значение направления интегрирования
	double old_up = m_up ;
	// Устанавливаем значение направления интегрирования
    if ((*this)[m_cs.arg_index]>arg) m_up =-1 ;
	else m_up = 1.0 ;
	// Интегрирование СДУ с автоматическим выбором шага (если это задано в признаках)
	int rc = 0 ;
    while (m_up*(*this)[m_cs.arg_index]<=m_up*arg) {
		rc = StepSelf() ; if(rc) goto end ; // выход по break
	}
	// Отшагивание назад частично (в пределах последнего шага)
	// с целью точного выхода на заданное значение независимой переменной
    rc = StepAdjusted(arg-(*this)[m_cs.arg_index]) ;
	//
end : ;
	// Восстанавливаем значение направления интегрирования
    m_up = old_up ;
    return rc ;
}

//-------------------------------------------------------------------------------
// Самостоятельный шаг модели движения - то есть с возможностью как разгона,
// так и сокращения шага. 
int ZCIntegrator::StepSelf(double* dq)
{
	// Если не заданы НУ или заданы не корректно то аварийный выход 
	if (!m_ok_nu) return 1 ;
	// Обнуление признака ограничения шага максимально допустимым значением
	m_IsWasStepCutDown = false ;
	bool isRemoveF_RUNA_AUTOSTEP = false ;

	// Если задан фиксированный шаг интегрирования
	if ( m_cs.fixed_step>0) {
		// Максимальный шаг считаем равным заданному фиксированному шагу
		if (m_cs.step_max < fabs(m_cs.fixed_step)) m_cs.step_max = fabs(m_cs.fixed_step) ;
		// Шаг полагаем равным фиксированному
		m_step = m_cs.fixed_step ;
		// Временно удаляем признак автоматического выбора шага
		if ( m_cs.flag & F_RUNA_AUTOSTEP) {
			isRemoveF_RUNA_AUTOSTEP = true ;
			m_cs.flag&=~F_RUNA_AUTOSTEP ;
	}	}

	// Выбор метода интегрирования
	if (m_cs.AdamsType>0 && (m_cs.flag & F_RUNA_AUTOSTEP)) {
		// Один шаг методом Адамса
		switch (m_cs.AdamsType) {
			case ID_ADAMS_4 :
			case ID_ADAMS_5 :
			case ID_ADAMS_6 :
			case ID_ADAMS_8 :
			case ID_ADAMS_10: StepAdams(dq) ; break ;
			default: assert(0) ;
		}	
	} else {
		double* dqrk = dq ;
		double* girk = nullptr ;
		if (m_cs.AdamsType==0) {
			// Если интегрирование только РК!
			// РК не используется для разгона а используется как основной метод интегрирования
			// Запоминаем группы интегрирования
			if (CountGI>0) { 
				//memcpy(GI[2*MAX_ADAMS_N-1], GI[2*MAX_ADAMS_N], NE()*sizeof(double)) ;
				//girk = GI[2*MAX_ADAMS_N-1] ;
			}
			//dqrk = GI[2*MAX_ADAMS_N] ; 
		}
		// Один шаг методом РК
		switch (m_cs.RungeType) {
			case ID_AILER   : Step1  (girk, dqrk) ; break ;
			case ID_RUNGE_4 : StepRK4(girk, dqrk) ; break ;
			case ID_RUNGE_8 : StepRK8(girk, dqrk) ; break ;
			default: assert(0) ;
		}
		if (m_cs.AdamsType==0) {
			// Если интегрирование только РК!
			// РК не используется для разгона а используется как основной метод интегрирования
			if (CountGI<2) CountGI++;
			//if (dq) memcpy(dq, dqrk, NE()*sizeof(double)) ;
		}
	}

	// Если задан фиксированный шаг интегрирования
	if ( m_cs.fixed_step>0) {
		// Восстанавливаем признак автоматического выбора шага, если такой был
		if (isRemoveF_RUNA_AUTOSTEP) m_cs.flag|= F_RUNA_AUTOSTEP ;
	}

	// При необходимости вызывается пошаговая пользовательская функция
    return(((m_cs.flag & F_RUNA_USEBREAK) && !m_IsLockBreak) ? UserBreak(m_a, dq):0) ;
}

//-------------------------------------------------------------------------------
// Один шаг интегрирования на заданную величину независимой переменной
// Шаг фиксированной длинны - выбор величины шага не допускается
int ZCIntegrator::StepAdjusted(double step, double* dq)
{
	// Если шаг нулевой то интегрирования не требуется	
	if (step == 0) return 0 ;
	// Сохраняются значения: шага, направления, признаков, и величина фиксированного шага
    double old_step = m_step ;
    double old_up   = m_up ;
    long   old_flag = m_cs.flag ;
	double old_fixed_step = m_cs.fixed_step ;
	// Задаются временные значения: шага, направления, признаков, 
	// и величина фиксированного шага
	m_cs.fixed_step = 0 ;
    m_cs.flag = m_cs.flag & F_RUNA_AUTOSTEP ? m_cs.flag - F_RUNA_AUTOSTEP : m_cs.flag ;
    m_cs.flag = !( m_cs.flag & F_RUNA_NOTMAXSTEP ) ? m_cs.flag + F_RUNA_NOTMAXSTEP : m_cs.flag ;
    m_up      = step>0 ? 1.0:-1.0 ;
    m_step    = fabs(step) ;
	
	// Собственно сам шаг интегрирования
    int rc = StepSelf(dq) ;

	// Восстанавливаются прежние значения: шага, направления, признаков, 
	// и величина фиксированного шага
	m_cs.fixed_step = old_fixed_step ;
    m_step    = old_step ;
    m_cs.flag = old_flag ;
    m_up      = old_up ;
    return rc ;
}

//-------------------------------------------------------------------------------
// Проверка шага на максимально и минимально допустимые значения
void ZCIntegrator::ValiStep ( double& step )
{
	if (m_cs.flag & F_RUNA_NOTMAXSTEP) return ;
	// Знак шага необходимо учесть для коректного интегрирования
	// "назад", то есть с отрицательным шагом
	double ss = step<0 ? -1.0:1.0 ;  
	// Фиксируем признак ограничения шага максимальным значением
	m_IsWasStepCutDown = m_cs.step_max<ss*step ? true:false;
	// Собственно сама проверка
    step = m_cs.step_max<ss*step ? (ss*m_cs.step_max):step ;
    step = m_cs.step_min>ss*step ? (ss*m_cs.step_min):step ;
}

//-------------------------------------------------------------------------------


