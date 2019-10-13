//-----------------------------------------------------------------------------
// MagicCalcU1.cpp: 
// А. Николенко 23.08.2018
//
// Реализация методик аналитического или численно-аналитического расчёта
// параметров управления движением центра масс КА для вариантов семейста 10+,
// (то есть 10, 11,..) в рамках методики узловых точек. 
// Автор всех методик старый волшебник. 
// !!!! Номера вариантов могут не совпадать с общепринятыми !!!!
//-----------------------------------------------------------------------------
#include <stdafx.h>

//-----------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции
#include <FlyMagicLNS.h>	//

extern double k_epsdrs ;		// 10 метров
extern double k_stepu ;			// [град] шаг перебора аргумента широты
extern int    pru ;				// признак учёта времени работы КДУ
extern int    kp ;				// признак корректируемого параметра
extern double dtQ1, dtQ2 ;		// для вектора промаха
extern double PV1[3], PV2[3] ;	// для изменений а,е,w  от V1,V2

//-----------------------------------------------------------------------------
// Расчёт управлений по вариантам 1-импульсных коррекций
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Построение геометрии орбиты
int MagicBuildORB(ZSMagicLnsSolution* MS, ZCLnsNpmClaim& PT, bool reBuild)
{
    ZSSpoOrb SP ;
	int rc = MagicBuildSpo(PT, SP) ; RRC ;
	// на первой итерации сохраняем геометрию требуемой орбиты
	if (reBuild || MS->GlobCount == 1) {
		PT.T = SP.T ; 
		PT.a = SP.a ; 
		PT.e = SP.e ; PT.w = SP.w ; 
		PT.e1= SP.e1; PT.e2= SP.e2 ; 
		if (MS->GlobCount == 1) {
			PT.uH= SP.uh ;
			MS->MZ.PARe() = SP.e;
			MS->MZ.PARw() = SP.w;
		}
	} else {
		// на последующих итерациях используем геометрию орбиты, 
		// расчитанную на первой итерации
		SP.e1= PT.e1; SP.e2= PT.e2 ; 
		SP.e = PT.e ; SP.w = PT.w ; 
		PT.T = SP.T ; 
		PT.a = SP.a ; 
		PT.p = SP.p ; 
	}	
	PT.p = PT.a*(1-PT.e*PT.e) ;
	return 0 ; 
}

//-----------------------------------------------------------------------------
//Расчёт управлений по варианту "10"("a+h,Bh; a+e,w; a+ОМИВ; a+ДУ")
int MagicCalcU10(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	int rc = 0 ; 
	// "Нулевое" управление
	memset(&U, sizeof(U), 0) ;
    
	// Проектирование требуемой орбиты по периоду
	PT.VarPar = SPO_VARIDPAR_a ;
	rc = MagicBuildORB(MS, PT, false) ; if(rc) return rc;
    // Вычисление радиусов в узловых точках требуемой орбиты и промаха
    ZSLnsNpmFault P ;
    rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; 
	if(rc) return rc;
	// учёт прецесии линии апсид
	// P.u0 += PZ.w-P1.w ;  

	// Собственно формирование управления
	// аргумент приложения импульса
    if (!U.isFixedArg) U.u0[0] = P.u0 ;        
	U.n = 1 ; // Колличество импульсов - 1 	
    // Определение V1 с уточнением  методом трапеций
	rc = MagicUT1(MS, PZ.Vit - P1.Vit, 4, PT.a - PZ.a, P1, U.u0[0], &U.dV[0], PV1); RRC;
    return 0 ;
}

//-----------------------------------------------------------------------------
//Расчёт управлений по варианту "11"("T+h,Bh; T+e,w; T+ОМИВ; T+ДУ")
int MagicCalcU11(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	int rc = 0 ; 
	// "Нулевое" управление
	memset(&U, sizeof(U), 0) ;

	// Проектирование требуемой орбиты по периоду
	PT.VarPar = SPO_VARIDPAR_T ;
	rc = MagicBuildORB(MS, PT, false) ; RRC ;
    // Вычисление радиусов в узловых точках требуемой орбиты и промаха
    ZSLnsNpmFault P ;
    rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; if(rc) return rc;
	//учёт прецесии линии апсид
	// P.u0 += PZ.w-P1.w ;   
	
	// Собственно формирование управления
	// аргумент приложения импульса
    if (!U.isFixedArg) U.u0[0] = P.u0 ;
	U.n = 1 ; // Колличество импульсов - 1 	

    // Определение V1 с уточнением  методом трапеций
	rc = MagicUT1(MS, PZ.Vit - P1.Vit, 1, PT.T - PZ.T, P1, U.u0[0], &U.dV[0], PV1); RRC;
    return 0 ;
}

//-----------------------------------------------------------------------------
//Расчёт управлений по варианту "11" для Кондор( T+ e, w )
int MagicCalcKondorU11(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
					   ZCLnsNpmClaim& PT, ZSLnsNpmFault& P, ZSFlightRule& U)
{
	int    rc = 0 ; 
    double a ;
	memset(&U, sizeof(U), 0) ;
     
    rc = MATC20(PT.i, PT.e1, PT.e2, PT.T, a) ;

	// Определение аргумента широты импульса
    rc = MagicCalcMishit(PT.i, a*(1-PT.e*PT.e), PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; RRC ;
	U.u0[0] = P.u0 ;
	U.n    = 1 ;

    // Определение V1 с уточнением  методом трапеций
    dtQ1 = (PT.T-PZ.T); kp = 1 ;
	rc = MagicUT1(MS, PZ.Vit - P1.Vit, kp, dtQ1, P1, U.u0[0], &U.dV[0], PV1); RRC;

    return rc ;
}

//-----------------------------------------------------------------------------
//Расчёт управлений по варианту "12"(L+h,Bh для эл. или L+e,w для ОМИВ, ДУ)
int MagicCalcU12(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
    int      rc = 0, count=0;
	int      dN = PZ.Vit-P1.Vit ;
    ZSLnsNpmFault  P ;
    ZSLnsNpmDiffDV DP ;
	// Требуемое изменение долготы на заданном витке
    dtQ1 = ValiDA(PT.L-PZ.L, 1) ;
	// "Нулевое" управление 
	memset(&U, sizeof(U), 0) ;
	//
	if (!U.isFixedArg) {
		//аргумент прил. импульса не задан
		if (MS->GlobCount == 1) U.u0[0] = P1.w;
		// Уточнение арг. широты импульса
		while ( 1 ) {
			// Определение V1 с уточнением  методом трапеций
			rc = MagicUT1(MS, dN, 2, dtQ1, P1, U.u0[0], &U.dV[0], PV1); if (rc) return rc;
			// расчёт отклонений от трансверсального импульса
			rc = MagicIZM(MS, P1.a, P1.e, P1.i, P1.w, dN, U.u0[0], U.dV[0], DP); RRC;
			// Период фазируещей орбиты
			PT.T = P1.T+DP.dT ;
			// Проектирование требуемой орбиты по периоду
			PT.VarPar = SPO_VARIDPAR_T ;
			rc = MagicBuildORB(MS, PT, false); RRC;
			// Вычисление радиусов в узловых точках требуемой орбиты и промаха
			rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; RRC ;
			U.u0[0] = P.u0 ;
			// учёт прецесии линии апсид
			//U.u0[0]+= PZ.w - P1.w ;                
			if (count==3) break ; count++ ;
		}
		// Расчёт аргумента широты приложения импульса
		rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; RRC ;
		U.u0[0] = P.u0 ;	 
		U.n    = 1 ;	// Колличество импульсов - 1 	
	}
	// Расчёт величины импульса скорости
	rc = MagicUT1(MS, dN, 2, dtQ1, P1, U.u0[0], &U.dV[0], PV1); RRC;
    return 0 ;
}

//-----------------------------------------------------------------------------
//Расчёт управлений по варианту "13"(dL+h,Bh; dL+e,w; dL+ОМИВ; dL+ДУ;)
int MagicCalcU13(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	int rc = 0 ; 
	// "Нулевое" управление
	memset(&U, sizeof(U), 0) ;

	// Проектирование требуемой орбиты по периоду
	PT.VarPar = SPO_VARIDPAR_dL ;
	rc = MagicBuildORB(MS, PT, true); RRC;

    // Вычисление радиусов в узловых точках требуемой орбиты и промаха
	ZSLnsNpmFault P ;
    rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; RRC ;
	// P.u0 += PZ.w-P1.w ;

	// Собственно формирование управления
	// аргумент приложения импульса
    if (!U.isFixedArg) U.u0[0] = P.u0 ;	
	U.n = 1 ;

    // ОпределениеV1 с уточнением  методом трапеций
	rc = MagicUT1(MS, PZ.Vit - P1.Vit, 1, PT.T - PZ.T, P1, U.u0[0], &U.dV[0], PV1); RRC;
    return rc ;
}

//-----------------------------------------------------------------------------
// Функция пересчёта h,Bh
int MagicAD12FromHBh(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
					 ZCLnsNpmClaim& PT, double& A1, double& A2, double& D1, double& D2)
{
    double rhe, sb, si, d1, d2, k1, k2 ;
    double p = P1.a*(1.-P1.e*P1.e) ;
    sb = sin(RadA(PT.BH)-k_alfa0*Sin(2.*PT.BH)) ;
    si = Sin(PT.i) ;
    d2 = sb/si ;
    d1 = (PT.VarB ? -1.0:1.0)*sqrt(1.0-d2*d2) ;
    rhe= PT.Hmin+1e4*k_a0*(1.0-k_alfa0*sb*sb) ;
    k1 = rhe+k_eps/k_mu_kms/rhe*(1.0-d1+1.0/6.0*si*si*(4.0*d1-d1*d1+d2*d2-3.0)) ;
    k2 = k_eps/k_mu_kms/rhe*(d2+2.0/3.0*si*si*(d1*d2-d2))-2.0*k_a0*k_alfa0*d1*d2*si*si ;
    A1 = p*(k2/k1*d2+d1)/k1 ;
    A2 = p*(d2-k2/k1*d1)/k1 ;
    D1 = sqrt(k_mu_kms/p)*(A1-d1-P1.e*Cos(P1.w))/2.0 ;
    D2 = sqrt(k_mu_kms/p)*(A2-d2-P1.e*Sin(P1.w))/2.0 ;
    return 0 ;
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "14"("hmin + Bh для эл." 
// или "hср + e, w для ОМИВ, ДУ")
int MagicCalcU14(
	ZSMagicLnsSolution* MS,	// Описание решения задачи.
	ZSLoopFact&    P1,		// Эл-ы движ. на витке первого импульса.
	ZSLoopFact&	   PZ,		// Эл-ы движ. на заданном витке в пассивном полёте
	ZCLnsNpmClaim& PT,		// Требуемые элементы движения
	ZSFlightRule&  U)		// Искомое управление 
{
	ZSLnsNpmFault  P ;
    ZSLnsNpmDiffDV D ;
    int    dN = PZ.Vit - P1.Vit, rc = 0 ;
	double up = MS->MZ.PARu();
	memset(&U, sizeof(U), 0) ;
	U.n = 1 ;

	if (PT.VarOrb == SPO_ORB_HBH) {
		// для эллиптическойорбиты
		// hmin + Bhmin
		// Проектирование орбиты не требуется
		if (U.isFixedArg) {            
			//аргумент приложения задан
			rc = MagicCalcDifDV(P1.a, P1.e, P1.i, P1.w, U.u0[0], up, dN, D) ; RRC ;
			U.dV[0] = (PT.Hmin-PZ.hmin)/D.dhp ;
		}
		if (!U.isFixedArg) {           
			//аргумент приложения не задан
			double A1, A2, D1, D2 ;
			// пересчёт h,Bh
			MagicAD12FromHBh(MS, P1, PZ, PT, A1, A2, D1, D2) ;       
			double R1 = A1*D1+A2*D2 ;
			double R2 = sqrt(D1*D1+D2*D2-pow(A1*D2-A2*D1, 2)) ;
			double R3 = A1*A1+A2*A2-1 ;
			double dV1=-(R1-R2)/R3 ;
			double dV2=-(R1+R2)/R3 ;
			double dhe= (PT.VarH ? PT.Hmax-PZ.hmax : PT.Hmin-PZ.hmin) ;
			if (dV1*dhe>0 && dV2*dhe<=0) U.dV[0] = dV1 ;
			else {
				if (dV2*dhe>0 && dV1*dhe<=0) U.dV[0] = dV2 ;
				else U.dV[0] = fabs(dV1)<fabs(dV2) ? dV1:dV2 ;
			}
			U.u0[0] = AtanSC((A2*U.dV[0]+D2)/(A1*U.dV[0]+D1), A1+D1/U.dV[0]) ;
		}
	} else {
		// для ОМИВ или ДУ
		// hср + e, w`
		// Проектирование требуемой орбиты по средней высоте для ОМИВ или ДУО
		PT.VarPar = SPO_VARIDPAR_H ; 
		PT.VarH   = SPO_VARH_MID ;
		rc = MagicBuildORB(MS, PT, true); RRC;
		// Вычисление радиусов в узловых точках требуемой орбиты и промаха
		rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P); RRC ;
		// P.u0+= (PZ.w-P1.w) ; 
		//аргумент не  задан
		if (!U.isFixedArg) U.u0[0] = P.u0 ; 
		U.dV[0] = 0.5*P.l*P.dRs ;
	}
	return rc ;
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "15" (T+w для любой орбиты)
int MagicCalcU15(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	// !!!! Для корректной работы необходимо указать тип орбиты SPO_ORB_EL
	assert(MS->MZ.ORB() == SPO_ORB_EL);

    int    i, k, rc = 0 ;
    double dw, du, dsw[2] ;
    double u1t, V1t = 0 ;
    int    dN = PZ.Vit-P1.Vit ;

	memset(&U, sizeof(U), 0) ;
	U.n = 1 ;
	k_stepu = 1.0 ;
    dtQ1 = PT.T-PZ.T ;    
	if (!U.isFixedArg) {
		//аргумент приложения не задан
		dw = ValiDA(PT.w-PZ.w, 1) ;
		k  = (int)(180.0/k_stepu) ;
		dsw[0] = 360.0 ;
		// Начальное значение аргумента широты
		u1t = P1.w ;
		du  = k_stepu*Sign1(dw*dtQ1) ;
		// Начало перебора
		for (i=0 ; i<k ; i++) {
			// Текущее значение аргумента широты
			u1t = ValiDA(u1t+du, 1) ;
			if (u1t<0) u1t+= 360.0 ;
			// Определение V1 с уточнением  методом трапеций
			V1t = 0 ;
			rc = MagicUT1(MS, dN, 1, dtQ1, P1, u1t, &V1t, PV1); RRC;
			dsw[1] = ValiDA(PT.w-PV1[2], 1) ;
			// Проверка условия  перебора
			if (fabs(dsw[1])<fabs(dsw[0])) {
				U.u0[0]  = u1t ; 
				U.dV[0] = V1t ;   
				dsw[0]  = dsw[1] ; 
	}	}	}
	rc = MagicUT1(MS, dN, 1, dtQ1, P1, U.u0[0], &U.dV[0], PV1);
	return rc ;
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "16" (L+w для любой орбиты)
int MagicCalcU16(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	// !!!! Для корректной работы необходимо указать тип орбиты SPO_ORB_EL
	assert(MS->MZ.ORB() == SPO_ORB_EL);

    int    dN, k , i, rc = 0 ;
    double dw, du, dsw[ 2 ] ;
    double u1t, V1t = 0   ;
    dN = PZ.Vit-P1.Vit ;

	memset(&U, sizeof(U), 0) ;
	U.n = 1 ;
	k_stepu = 1.0 ;

    dtQ2 =-ValiDA(PZ.L-PT.L, 1) ;                    
	if (!U.isFixedArg) {
		//аргумент приложения не задан
		dw = ValiDA(PT.w-PZ.w, 1) ;
		dsw[0] = 360. ;
		du =-k_stepu*Sign1(dw*dtQ2) ;
		k  = (int)(180.0/k_stepu) ;
		// Начальное значение аргумента широты
		u1t = P1.w ;    
		for (i=0 ; i<k ; i++) {
			// Текущее значение аргумента широты
			u1t = ValiDA(u1t+du, 1) ;
			if (u1t<0) u1t+= 360.0 ;
			// Определение V1 с уточнением  методом трапеций
			V1t = 0   ;
			rc = MagicUT1(MS, dN, 2, dtQ2, P1, u1t, &V1t, PV1); RRC;
			dsw[1] = ValiDA(PT.w-PV1[2], 1) ;
			// Проверка условия  перебора
			if (fabs(dsw[1])<fabs(dsw[0])) {
				U.u0[0]  = u1t ; 
				U.dV[0] = V1t ; 
				dsw[0]  = dsw[1] ;
	}	}	}
	rc = MagicUT1(MS, dN, 2, dtQ2, P1, U.u0[0], &U.dV[0], PV1); RRC;
	return rc ;
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "17" (T для любой орбиты)
int MagicCalcU17(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	int rc = 0 ;
	int dN = PZ.Vit-P1.Vit ;
	dtQ1 = PT.T-PZ.T ;
	
	memset(&U, sizeof(U), 0) ;
	U.n = 1 ;
	U.uw[0] = false ;
	// если  аргумент приложения не задан
	if (!U.isFixedArg) { 
		U.u0[0] = P1.w ; 
		U.uw[0]= 1 ; 
	}
	// Определение V1 с уточнением  методом трапеций
	rc = MagicUT1(MS, dN, 1, dtQ1, P1, U.u0[0], &U.dV[0], PV1);
	return rc ;
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "18" (L для любой орбиты)
int MagicCalcU18(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
				 ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	int rc = 0 ;
	int dN = PZ.Vit-P1.Vit ;
    dtQ2 =-ValiDA(PZ.L-PT.L, 1) ;                    

	memset(&U, sizeof(U), 0) ;
	U.n = 1 ;
	U.uw[0] = false ; 
	// если  аргумент приложения не задан
	if (!U.isFixedArg) { 
		U.u0[0] = P1.w ; 
		U.uw[0] = true ; 
	}
	// Определение V1 с уточнением  методом трапеций
	rc = MagicUT1(MS, dN, 2, dtQ2, P1, U.u0[0], &U.dV[0], PV1) ;
	return rc ;
}

//-----------------------------------------------------------------------------
