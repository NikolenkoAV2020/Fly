//-------------------------------------------------------------------------------
// ZCIntegratorRK.cpp
// А. Николенко 03.08.2018
// Реализация одношаговых методов
//-------------------------------------------------------------------------------
#include <stdafx.h>
#include <FlyMMath.h>

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZCIntegrator.h>
//-------------------------------------------------------------------------------

int ZCIntegrator::CreateRK ( ) 
{
    switch ( m_cs.RungeType ) {
    case ID_AILER   : m_ns = 1 ; break ;
    case ID_RUNGE_4 : m_ns = 4 ; break ;
    case ID_RUNGE_8 : m_ns = 9 ; break ;
    default : return 0 ;
    }
    m_d = new double*[m_ns] ;
    for (int i=0 ; i<m_ns ; i++)  { m_d[i] = new double[m_N] ; memset(m_d[i], 0, m_N*sizeof(double)) ; }
    m_x = new double[m_N] ; memset(m_x, 0, m_N*sizeof(double)) ;
    SetEpsd(m_cs.step_eps) ;
	
	return 1 ;
}
//-------------------------------------------------------------------------------

void ZCIntegrator::Step1(double* GI, double* dq)
{
	double step = m_up*m_step ;
	int N = NE() ;

	if (GI) memcpy(m_d[0], GI, N*sizeof(double)) ;
	else SphExe(m_a, m_d[0]) ;

    for (int i=0 ; i<N ; i++)  m_a[i]+= step*m_d[0][i] ;
    if (dq) SphExe(m_a, dq) ;
}
//-------------------------------------------------------------------------------
void ZCIntegrator::StepRK4(double* GI, double* dq, double instep, bool SkipStepSelectiom )
{
	double step = instep ? m_up*instep : m_up*m_step ;
    int i ;
	int N = NE() ;

	if (GI) memcpy(m_d[0], GI, N*sizeof(double)) ;
	else SphExe(m_a, m_d[0]) ;

    for (i=0 ; i<N ; i++)  m_x[i] = m_a[i]+step*m_d[0][i]/2 ;
	SphExe( m_x, m_d[1]) ;
    for (i=0 ; i<N ; i++)  m_x[i] = m_a[i]+step*m_d[1][i]/2 ;
	SphExe( m_x, m_d[2]) ;
    for (i=0 ; i<N ; i++)  m_x[i] = m_a[i]+step*m_d[2][i] ;
	SphExe( m_x, m_d[3]) ;
    for (i=0 ; i<N ; i++)  m_a[i]+= step*(m_d[0][i]+2*m_d[1][i]+2*m_d[2][i]+m_d[3][i])/6.0 ;
    if (dq) { SphExe(m_a, dq) ; }
}
//-------------------------------------------------------------------------------
void ZCIntegrator::NewStepRK8( double* x, double* k5, double* k6, double& err, double oldstep, double& newstep )
{
    double err1 ;
    double tm1, tm2 ;
	int N = NE() ;
    int i ;
    err = 0 ;
    for (i=0 ; i<N ; i++) {
      tm1  = 1e-6>fabs(k5[i]) ? 1e-6:fabs(k5[i]) ;
      tm2  = m_eps2>fabs(x[i]) ? m_eps2:fabs(x[i]) ;
      err1 = tm1>tm2 ? tm1:tm2 ;
      tm1  = fabs(k5[i]-k6[i]) / err1 ;
      err1 = tm1>0 ? pow(tm1, 2):0 ;
      err += err1 ;
	}
    err = sqrt(err/(double)N) ;
    double step_dv ;
    tm2     = pow(err/m_eps, 0.125)/0.9 ;
    tm1     = 3.0<tm2 ? 3.0:tm2 ;
    step_dv = 1.0>6.0*tm1 ? 1.0/6.0:tm1 ;
    newstep = oldstep/step_dv ;
}

//-------------------------------------------------------------------------------
void ZCIntegrator::StepRK8(double* GI, double* dq, double instep, bool SkipStepSelectiom )
{
    static double A2       = 1.0/18.0 ;
    static double A3 [ 2 ] = { 1.0/48.0, 1.0/16.0 } ;
    static double A4 [ 2 ] = { 1.0/32.0, 3.0/32.0 } ;
    static double A5 [ 3 ] = { 5.0/16.0, -75.0/64.0, 75.0/64.0 } ;
    static double A6 [ 3 ] = { 3.0/80.0,  3.0/16.0,  3.0/20.0 } ;
    static double A7 [ 4 ] = { 29443841.0/614563906.0, 77736538.0/692538347.0, -28693883.0/1125e6, 23124283.0/18e8 } ;
    static double A8 [ 5 ] = { 16016141.0/946692911.0, 61564180.0/158732637.0, 22789713.0/633445777.0, 545815736.0/2771057229.0, -180193667.0/1043307555.0 } ;
    static double A9 [ 6 ] = { 39632708.0/573591083.0, -433636366.0/683701615.0, -421739975.0/2616292301.0, 100302831.0/723423059.0, 790204164.0/839813087.0, 800635310.0/3783071287.0 } ;
    static double A10[ 7 ] = { 246121993.0/1340847787.0,-37695042795.0/15268766246.0, -309121744.0/1061227803.0, -12992083.0/490766935.0, 6005943493.0/2108947869.0, 393006217.0/1396673457.0, 123872331.0/1001029789.0 } ;
    static double A11[ 8 ] = {-1028468189.0/846180014.0, 8478235783.0/508512852.0, 1311729495.0/1432422823.0, -10304129995.0/1701304382.0, -48777925059.0/3047939560.0, 15336726248.0/1032824649.0, -45442868181.0/3398467696.0, 3065993473.0/597172653.0 } ;
    static double A12[ 9 ] = { 185892177.0/718116043.0, -3185094517.0/667107341.0, -477755414.0/1098053517.0, -703635378.0/230739211.0, 5731566787.0/1027545527.0, 5232866602.0/850066563.0, -4093664535.0/808688257.0, 3962137247.0/1805957418.0, 65686358.0/487910083.0 } ;
    static double A13[ 9 ] = { 403863854.0/491063109.0, -5068492393.0/434740067.0, -411421997.0/543043805.0, 652783627.0/914296604.0, 11173962825.0/925320556.0, -13158990841.0/6184727034.0, 3936647629.0/1978049680.0, -160528059.0/685178525.0, 248638103.0/1413531060.0 } ;
    static double B  [ 9 ] = { 14005451.0/335480064.0, -59238493.0/1068277825.0, 181606767.0/758867731.0, 561292985.0/797845732.0, -1041891430.0/1371343529.0, 760417239.0/1151165299.0, 118820643.0/751138087.0, -528747749.0/2220607170.0, 1.0/4.0 } ;
    static double BH [ 8 ] = { 13451932.0/455176623.0, -808719846.0/976000145.0, 1757004468.0/5645159321.0, 656045339.0/265891186.0, -3867574721.0/1518517206.0, 465885868.0/322736535.0, 53011238.0/667516719.0, 2.0 / 45.0 } ;
	int N = NE() ;
    int i ;

	if (GI) memcpy(m_d[0], GI, N*sizeof(double)) ;
	else SphExe(m_a, m_d[0]) ;

	double step = m_step ; 
met : ;
    ValiStep(step) ;
	step = m_up*(instep ? instep:step) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*A2*m_d[0][i] ;
    SphExe(m_x, m_d[1]) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*(A3[0]*m_d[0][i]+A3[1]*m_d[1][i]) ;
    SphExe(m_x, m_d[2]) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*(A4[0]*m_d[0][i]+A4[1]*m_d[2][i]) ;
    SphExe(m_x, m_d[3]) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*(A5[0]*m_d[0][i]+A5[1]*m_d[2][i]+A5[2]*m_d[3][i]) ;
    SphExe(m_x, m_d[4]) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*(A6[0]*m_d[0][i]+A6[1]*m_d[3][i]+A6[2]*m_d[4][i]) ;
    SphExe(m_x, m_d[5]) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*(A7[0]*m_d[0][i]+A7[1]*m_d[3][i]+A7[2]*m_d[4][i]+A7[3]*m_d[5][i]) ;
    SphExe(m_x, m_d[6]) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*(A8[0]*m_d[0][i]+A8[1]*m_d[3][i]+A8[2]*m_d[4][i]+A8[3]*m_d[5][i]+A8[4]*m_d[6][i]) ;
    SphExe(m_x, m_d[1]) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*(A9[0]*m_d[0][i]+A9[1]*m_d[3][i]+A9[2]*m_d[4][i]+A9[3]*m_d[5][i]+A9[4]*m_d[6][i]+A9[5]*m_d[1][i]) ;
    SphExe(m_x, m_d[2]) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*(A10[0]*m_d[0][i]+A10[1]*m_d[3][i]+A10[2]*m_d[4][i]+A10[3]*m_d[5][i]+A10[4]*m_d[6][i]+A10[5]*m_d[1][i]+A10[6]*m_d[2][i]) ;
    //
    for (i=0 ; i<N ; i++) m_d[7][i] = A11[0]*m_d[0][i]+A11[1]*m_d[3][i]+A11[2]*m_d[4][i]+A11[3]*m_d[5][i]+A11[4]*m_d[6][i]+A11[5]*m_d[1][i]+A11[6]*m_d[2][i] ;
    for (i=0 ; i<N ; i++) m_d[8][i] = A12[0]*m_d[0][i]+A12[1]*m_d[3][i]+A12[2]*m_d[4][i]+A12[3]*m_d[5][i]+A12[4]*m_d[6][i]+A12[5]*m_d[1][i]+A12[6]*m_d[2][i] ;
    for (i=0 ; i<N ; i++) m_d[3][i] = A13[0]*m_d[0][i]+A13[1]*m_d[3][i]+A13[2]*m_d[4][i]+A13[3]*m_d[5][i]+A13[4]*m_d[6][i]+A13[5]*m_d[1][i]+A13[6]*m_d[2][i] ;
    for (i=0 ; i<N ; i++) m_d[4][i] =   B[0]*m_d[0][i]+  B[1]*m_d[5][i]+  B[2]*m_d[6][i]+  B[3]*m_d[1][i]+  B[4]*m_d[2][i] ;
    for (i=0 ; i<N ; i++) m_d[5][i] =  BH[0]*m_d[0][i]+ BH[1]*m_d[5][i]+ BH[2]*m_d[6][i]+ BH[3]*m_d[1][i]+ BH[4]*m_d[2][i] ;
    memcpy(m_d[1], m_d[7], N*sizeof(double)) ;
    memcpy(m_d[2], m_d[8], N*sizeof(double)) ;
    //
    SphExe(m_x, m_d[6]) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*(m_d[1][i]+A11[7]*m_d[6][i]) ;
    SphExe(m_x, m_d[1]) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*(m_d[2][i]+A12[7]*m_d[6][i]+A12[8]*m_d[1][i]) ;
    SphExe(m_x, m_d[2]) ;
    for (i=0 ; i<N ; i++) m_x[i] = m_a[i]+step*(m_d[3][i]+A13[7]*m_d[6][i]+A13[8]*m_d[1][i]) ;
    SphExe(m_x, m_d[3]) ;
    //
    for (i=0 ; i<N ; i++) m_d[4][i] = m_a[i]+step*(m_d[4][i]+B[5]*m_d[6][i]+B[6]*m_d[1][i]+B[7]*m_d[2][i]+B[8]*m_d[3][i]) ;

	//-----------------------------------------------------------------------------------
	// Контроль точнисти шага,
	// при необходимости уменьшение шага или его увеличение, при наличии такой возможности
    if (!SkipStepSelectiom && (m_cs.flag&F_RUNA_AUTOSTEP)) {
		double err, newstep ;
		for (i=0 ; i<N ; i++) m_d[5][i] = m_a[i]+step*(m_d[5][i]+BH[5]*m_d[6][i]+BH[6]*m_d[1][i]+BH[7]*m_d[2][i]) ;
		NewStepRK8(m_a, m_d[4], m_d[5], err, step, newstep) ;
		step = newstep ;
		// Шаг неудачный
		if (err>m_eps && m_cs.step_min<newstep) goto met ;
    }
	m_step = fabs(step) ;

	//-----------------------------------------------------------------------------------
	// Новое значение интегрируемого вектора
    memcpy(m_a, m_d[4], N*sizeof(double)) ;
    if (dq) SphExe(m_a, dq) ;                     
}
//-------------------------------------------------------------------------------

