//-----------------------------------------------------------------------------
// MagicCalcU2.cpp: 
// А. Николенко 24.10.2018
//
// Реализация методик аналитического или численно-аналитического расчёта
// параметров управления движением центра масс КА для вариантов семейста 20+,
// (то есть 20, 21,..) в рамках методики узловых точек. 
// Автор всех методик старый волшебник. 
// !!!! Номера вариантов могут не совпадать с общепринятыми !!!!
//-----------------------------------------------------------------------------
#include <stdafx.h>

//-----------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции
#include <FlyMagicLNS.h>	//

//-----------------------------------------------------------------------------
extern double k_epsdrs ;		// 10 метров
extern double k_stepu ;			// [град] шаг перебора аргумента широты
extern int    pru ;				// признак учёта времени работы КДУ
extern int    kp ;				// признак корректируемого параметра
extern double dtQ1, dtQ2 ;		// для вектора промаха
extern double PV1[3], PV2[3] ;	// для изменений а,е,w  от V1,V2

int MagicBuildORB(ZSMagicLnsSolution* MS, ZCLnsNpmClaim& PT, bool reBuild);

//-----------------------------------------------------------------------------
// Расчёт управлений по вариантам 2-импульсных коррекций
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "20" (a, e, w  для любой орбиты)
int MagicCalcU20(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& P2, 
				 ZSLoopFact& PZ, ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	PT.VarPar = SPO_VARIDPAR_a ;
	return (MagicCalcU21(MS, P1, P2, PZ, PT, U)) ;
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "21" (T, h, Bh для эл. или T, e, w для ОМИВ, ДУ)
int MagicCalcU21(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& P2, 
				 ZSLoopFact& PZ, ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
    int      rc = 0 ;
    double   u1, u2, V1, V2 ;
    double   t, S ;
    ZSLnsNpmFault  P ;
    ZSLnsNpmDiffDV DP ;

	memset(&U, sizeof(U), 0) ;
	U.n = 2 ;

	//U.isFixedArg = false ;
	//U.u0[0] = 99.86 ;
	//U.u0[1] = 279.86 ;
	//U.plane = 0 ;

	// Проектирование требуемой орбиты по периоду
	if (PT.VarPar==SPO_NOTBUILD) PT.VarPar = SPO_VARIDPAR_T ;
	rc = MagicBuildORB(MS, PT, (PT.VarOrb == SPO_ORB_OMIV || PT.VarOrb == SPO_ORB_DU) ? false : true); RRC;
	// Вычисление радиусов в узловых точках требуемой орбиты и промаха
	rc = MagicCalcMishit(PT.i, PT.p, PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; RRC ;
	//if (PT.VarPar==SPO_VARIDPAR_T || PT.VarPar==SPO_VARIDPAR_dL)
	//	P.dRs = (PT.T - PZ.T)/3./pi * sqrt(k_mu/P1.a);   //Переход от dRs к dT

    // Если заданы аргументы широты приложения импульсов
    if (U.isFixedArg) {
	    if (U.plane==1) {
			// задан u1
			V1 = U.dV[0] = P.l/8.0*(4.0*P.dRs*P.dRs-P.dR0*P.dR0)/
			     (2.0*P.dRs-P.dR1*Cos(U.u0[0])-P.dR2*Sin(U.u0[0])) ;
			V2 = U.dV[1] = P.l/2.0*P.dRs-V1 ;
			U.u0[1] = AtanSC((P.l/4.0*P.dR2-V1*Sin(U.u0[0]))/( P.l/4.*P.dR1-V1*Cos(U.u0[0])),
			     (P.l/4.0*P.dR1-V1*Cos(U.u0[0]))/V2) ;
		}
		if (U.plane==2) {
			// задан u2
			V2 = U.dV[1] = P.l/8.0*(4.0*P.dRs*P.dRs-P.dR0*P.dR0)/
			     (2.*P.dRs-P.dR1*Cos(U.u0[1])-P.dR2*Sin(U.u0[1]));
			V1 = U.dV[0]= P.l/2.0*P.dRs-V2 ;
			U.u0[0] = AtanSC((P.l/4.0*P.dR2-V2*Sin(U.u0[1]))/(P.l/4.0*P.dR1-V2*Cos(U.u0[1])),
			     (P.l/4.0*P.dR1-V2*Cos(U.u0[1]))/V1);
		}

		if (U.plane==3) {
			// заданы u1, u2
			V1 = U.dV[0] = P.l/8.0*(4.*P.dRs*P.dRs-P.dR0*P.dR0)/
			     (2.*P.dRs-P.dR1*Cos(U.u0[0])-P.dR2*Sin(U.u0[0]));
			V2 = U.dV[1]= P.l/2.0*P.dRs-V1 ;
		}
	}
	// Не заданы  аргументы приложения
	if (!U.isFixedArg) {
		if (MS->GlobCount == 1 && fabs(2.0*fabs(P.dRs) - fabs(P.dR0))<0.25) {
			//MMessageBox( "Орбиты почти касаются,возможен 1имп. манёвр" , 2, 0 ) ;
		}
		// Проверка условия пересечения орбит
		if (2.0*fabs(P.dRs) < fabs(P.dR0)) {
			// Орбиты пересекаются
			V1 = P.l/4.0*(0.5*P.dR0+P.dRs) ;
			V2 =-P.l/4.0*(0.5*P.dR0-P.dRs) ;
			if (P.dRs <0){
				t = V2 ; V2 = V1 ; V1 = t ;
			}
			u1 = P.u0 ;
			u2 = ValiA(u1+180.0, 1) ; //+(P2.Vit-P1.Vit)*(PZ.w-P1.w)/(PZ.Vit-P1.Vit) ;
			if (V1<0) {
				t = V2 ; V2 = V1 ; V1 = t ;
				t = u2 ; u2 = u1 ; u1 = t ;
			}
			rc = MagicIZM(MS, P1.a, P1.e, P1.i, P1.w, PZ.Vit - P1.Vit, u1, V1, DP); RRC;
			double p2 ;
			p2 = (P1.a+DP.da)*(P1.a+DP.da)*(1.0-(P1.e+DP.de)*(P1.e+DP.de)*(P1.e+DP.de)*(P1.e+DP.de));
			u2+= (P2.Vit-P1.Vit-(u2+u1)/360.0)*180.0/p2*k_eps/k_mu_kms*(5.0*Cos(P2.i)*Cos(P2.i)-1.0) ;
		} else {
			// Орбиты не имеют общих точек
			V1 = V2 = P.l/4.0*P.dRs ;
			//P.dRs = r;
			S = sqrt(4.0*P.dRs*P.dRs-P.dR0*P.dR0) ;
			u1= AtanSC((P.dR2*P.dR0+P.dR1*S)/(P.dR1*P.dR0-P.dR2*S), (P.dR1*P.dR0-P.dR2*S)/2.0/P.dR0/P.dRs) ;
			u2= AtanSC((P.dR2*P.dR0-P.dR1*S)/(P.dR1*P.dR0+P.dR2*S), (P.dR1*P.dR0+P.dR2*S)/2.0/P.dR0/P.dRs) ;
			//double p2 
			//p2 = (P1.a+DP.da)*(P1.a+DP.da)*(1.0-(P1.e+DP.de)*(P1.e+DP.de)*(P1.e+DP.de)*(P1.e+DP.de));
			//u2+= (P2.Vit-P1.Vit-(u2+u1)/360.0)*180.0/p2*k_eps/k_mu_kms*(5.0*Cos(P2.i)*Cos(P2.i)-1.0);
			if (MS->GlobCount>1) {
				if ((U.u0[0]>U.u0[1] && u1>u2) || (U.u0[0]<U.u0[1] && u1<u2)) {
					u1 = u1 ;
				} else { 
					t = u2 ; u2 = u1 ; u1 = t ; 
			}	}
		}
		// Искомое управление
		U.u0[0] = u1 ; U.u0[1] = u2 ;
		U.dV[0] = V1 ; U.dV[1] = V2 ;
	}
	return rc ;
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "21" для Кондор(Т, e, w )
int MagicCalcKondorU21(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& PZ, 
					   ZCLnsNpmClaim& PT, ZSLnsNpmFault& P, ZSFlightRule& U)
{
	int    rc = 0;
	double a, u2, V1, V2, S, u1;
	double up = MS->MZ.PARu();
	ZSLnsNpmDiffDV D;

	rc = MATC20(PT.i, PT.e1, PT.e2, PT.T, a);
	rc = MagicCalcMishit(PT.i, a*(1.0 - PT.e*PT.e), PT.e1, PT.e2, P1, PZ, PT.Ro, P);
	if (rc) return rc;

	U.uw[0] = U.uw[1] = 0; U.n = 2;
	if (MS->GlobCount == 1 && fabs(2.0*fabs(P.dRs) - fabs(P.dR0))<0.025) {
		//MMessageBox( "Орбиты почти касаются,возможен 1имп. манёвр" , 2, 0 ) ;
	}
	//Переход от dRs к dT
	P.dRs = (PT.T - PZ.T) / 3.0 / k_pi*sqrt(k_mu_kms / P1.a);

    // Манёвр на линии переключения
	if (U.plane) {
		if (P.u0<180.0) u2 = P.u0;
		else u2 = 360 - P.u0;
		if (u2<1.0) u2 = 1.0;
		if (u2>320.0) u2 = 320.0;
		U.u0[1] = u2;
		MagicCalcDifDV(P1.a, P1.e, P1.i, P1.w, P.u0, up, PZ.Vit - U.vit[1], D);
		//VV = ( PT.T - PZ.T) /  D.dT ; 
		U.dV[1] = V2 = P.l / 8.0*(4.0*P.dRs*P.dRs - P.dR0*P.dR0) /
					   (2.0*P.dRs - P.dR1*Cos(u2) - P.dR2*Sin(u2));
		V1 = U.dV[0] = P.l / 2.0*P.dRs - V2;
		U.u0[0] = u1 = AtanSC((P.l / 4.0*P.dR2 - V2*Sin(u2)) / 
					   (P.l / 4.*P.dR1 - V2*Cos(u2)),
					   (P.l / 4.0*P.dR1 - V2*Cos(u2)) / V1);
		// перемена импульсов местами
		if (V2>V1) {
			U.dV[1] = V2; U.dV[0] = V1; U.u0[1] = u2; U.u0[0] = u1;
		}
	} else {
		// Манёвр с равными величинами импульсов
		MagicCalcDifDV(P1.a, P1.e, P1.i, P1.w, P.u0, up, PZ.Vit - U.vit[1], D);
		U.dV[0] = U.dV[1] = V2 = V1 = (PT.T - PZ.T) / 2.0 / D.dT;
		if ((4.0*P.dRs*P.dRs - P.dR0*P.dR0)<0) S = 0;
		else S = sqrt(4.0*P.dRs*P.dRs - P.dR0*P.dR0);
		U.u0[0] = u1 = AtanSC((P.dR2*P.dR0 + P.dR1*S) / (P.dR1*P.dR0 - P.dR2*S), 
					   (P.dR1*P.dR0 - P.dR2*S) / 2.0 / P.dR0 / P.dRs);
		U.u0[1] = u2 = AtanSC((P.dR2*P.dR0 - P.dR1*S) / (P.dR1*P.dR0 + P.dR2*S),
					   (P.dR1*P.dR0 + P.dR2*S) / 2.0 / P.dR0 / P.dRs);
	}
    // Признак приращения требуемых параметров без формы орбиты
	//    PT.FalseTargetKey = MKN_PARAM_ORB_T  ; 
    // С добавлением формы орбиты
	//    if (MP.isEstimateOrb_e12) PT.FalseTargetKey|= MKN_PARAM_ORB_e1 | MKN_PARAM_ORB_e2 ;
    return rc ;
}

//-----------------------------------------------------------------------------
//Расчёт управлений по варианту "22"("T, w для эл орбиты или hs, e, w для ОМИВ,ДУ")
int MagicCalcU22(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& P2, 
				 ZSLoopFact& PZ, ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	assert(PT.VarOrb!=SPO_ORB_HBH) ;

	if (PT.VarOrb == SPO_ORB_OMIV) {
		PT.VarPar = SPO_VARIDPAR_H ;
		PT.VarH   = SPO_VARH_MID ;
		return MagicCalcU21(MS, P1, P2, PZ, PT, U);
    }
	if (PT.VarOrb == SPO_ORB_DU) {
		PT.VarPar = SPO_VARIDPAR_H ;
		PT.VarH   = SPO_VARH_MID ;
		// уход на 21 вар.для ОМИВ,ДУО
		return MagicCalcU21(MS, P1, P2, PZ, PT, U);
    }
	if (PT.VarOrb == SPO_ORB_EL) {
		PT.VarPar = SPO_VARIDPAR_H ;
		PT.VarH   = SPO_VARH_MID ;
		// уход на 21 вар.для ОМИВ,ДУО
		return MagicCalcU21(MS, P1, P2, PZ, PT, U);
    }


    // Если эллипт.орбита
    int      i, k, rc = 0 ;
    int      dN1 = PZ.Vit - P1.Vit ;
    int      dN2 = PZ.Vit - P2.Vit ;
    double   du1, du2, sdv[ 2 ] ;
    double   u1t, u1t0, u2t, dV1t=0, dV2t=0 ;
    dtQ1 = PT.T-PZ.T ;
    dtQ2 = ValiDA(PT.w-PZ.w, 1) ;
	//корр.параметры T и w
    kp   = 3 ;                              
    sdv[ 0 ] = 360.0 ;
    U.uw[ 0 ] = U.uw[ 1 ] = 0 ; U.n  = 2 ;
	//  уточнение импульсов с учётом времени работы КДУ
    pru = 0 ;  
	// не заданы  аргументы придожения
	if (U.isFixedArg == 0 && MS->GlobCount <= 2) {
		dV1t = dtQ1/6.0/k_pi/P1.a/P1.a*k_mu_kms*(1.0-P1.e*P1.e) ;
		dV2t = fabs(RadA(dtQ2)/2.0*P1.e/sqrt(P1.a/k_mu_kms)) ;
		if (MS->GlobCount == 1 && fabs(dV1t) >= fabs(dV2t)) U.plane = 11;
		if (MS->GlobCount == 1 && fabs(dV1t)< fabs(dV2t)) U.plane = 12;

		if (U.plane==12) {
			//когда 1 имп. манёвр не возможен     
			du1 = k_stepu*Sign1(dtQ1*dtQ2) ;
			du2 =-du1 ;
			// при переборе без учёта времени работы КДУ
			pru = 1 ;                   
			u1t0 = u1t = ValiA(P1.w+10.0*Sign1(du1), 1) ;
			u2t  = ValiA(P2.w+10.*Sign1(du2) , 1) ;
			while (1) {
				// уточнение V1,V2 методом трапеций
				dV1t = dV2t = 0 ;
				rc = MagicUT2(MS, dN1, dN2, kp, pru, dtQ1, dtQ2,  u1t, u2t,
					 P1, P2, &dV1t, &dV2t, PV1, PV2) ;  RRC ;
				sdv[1] = fabs(dV1t) + fabs(dV2t) ;
				//это мин.сумма импульсов
				if (sdv[1] < sdv[0]) {      
					sdv [0] = sdv[1] ;
					U.u0[0] = u1t ;  U.u0[1] = u2t ;
					U.dV[0] = dV1t ; U.dV[1] = dV2t ;
				}
				// Новое значение аргумента широты u1
				u1t = ValiA(u1t+du1, 1 ) ;
				if (fabs(ValiDA(u1t-P1.w, 1))<= 85.0 )  continue ;
				// Перебор по u1 закончен
				u1t = u1t0 ;
				// Новое значение аргумента широты u2
				u2t = ValiA( u2t + du2, 1 ) ;
				if (fabs(ValiDA(u2t-PV1[2], 1))<= 85.0) continue ;
				break ;
			}
		}
		if ( U.plane == 11) {
			//когда возможен 1 имп.манёвр
			if (MS->GlobCount == 1) MS->GlobCount = MS->GlobCount; //MMessageBox( "Возможен 1имп. манёвр" , 2, 0 ) ;
       dV1t = dV2t = 0 ;
       dV1t = dV1t - fabs(dV2t) ;
        du2 = k_stepu * Sign1( dtQ1*dtQ2 ) ;
        u1t = ValiA( P1.w +  2.*Sign1(du1) ,1) ;
        u2t = ValiA( P2.w + 50.*Sign1(du2) ,1);
      while ( 1 ) {
        //---------уточнение V1,V2 методом трапеций
       dV1t = dV2t = 0 ;
        rc = MagicUT2(MS, dN1, dN2, kp, pru, dtQ1, dtQ2,  u1t, u2t,
                        P1,  P2, &dV1t, &dV2t, PV1, PV2 ) ;  RRC ;
        sdv[ 1 ] = fabs( dV1t) + fabs( dV2t) ;
        if ( sdv[ 1 ] < sdv[ 0 ] ) {
           sdv[ 0 ] = sdv[ 1 ] ;
          U.u0[ 0 ] = u1t ;  U.u0[ 1 ] = u2t ;
          U.dV[ 0 ] = dV1t ; U.dV[ 1 ] = dV2t ;
        }
        // Новое значение аргумента широты u2
        u2t = ValiA( u2t +  du2 ,1 ) ;
       if ( fabs( ValiDA( u2t - PV1[2] , 1 ) ) <= 85.)  continue ;
       break ;
      }
     }
   }
	if (U.isFixedArg && MS->GlobCount <= 2) {     //  заданы  аргументы приложения
      k = (int)(175.0/ k_stepu) ;
    if (U.plane==1) {                        // задан u1
      du2 = - k_stepu * Sign1( dtQ1*dtQ2 );
      u2t = ValiA( P2.w + 10.*Sign1(du2) ,1); // Начальное значение u2
      pru =1 ;                     // при переборе без учёта времени работы КДУ
     for ( i = 0 ; i < k ; i++ ) {           // Начало перебора
     // Текущее значение аргумента широты u2
      u2t = ValiDA(u2t + du2 ,1) ;
      if (u2t<0) u2t+= 360.0 ;
      //----Определение V1,V2  с уточнением  методом трапеций
       dV1t = dV2t = 0 ;
      rc = MagicUT2(MS, dN1, dN2, kp, pru, dtQ1, dtQ2, U.u0[0], u2t,
                    P1, P2, &dV1t, &dV2t, PV1, PV2) ;  RRC ;
       sdv[1] = fabs(dV1t)+fabs(dV2t) ;
       if (sdv[1]<sdv[0]) {                  //это мин.сумма импульсов
          sdv[0] = sdv[1] ;
          U.dV[0]= dV1t ; U.dV[1]= dV2t ; U.u0[1] = u2t ; }
     }                                                        //Конец перебора
    }
    if (U.plane==2) {                         // задан u2
      du1 = k_stepu * Sign1( dtQ1*dtQ2 );
      u1t = ValiA( P1.w + 10.*Sign1(du1) ,1);           // Начальное значение u2
      pru =1 ;                     // при переборе без учёта времени работы КДУ
     for ( i = 0 ; i < k ; i++ ) {                           // Начало перебора
     // Текущее значение аргумента широты u1
      u1t = ValiDA(u1t + du1 ,1) ;
      if ( u1t <0 ) u1t += 360. ;
      //----Определение V1,V2  с уточнением  методом трапеций
       dV1t = dV2t = 0 ;
       rc = MagicUT2(MS, dN1, dN2, kp, pru, dtQ1, dtQ2, u1t, U.u0[1],
                       P1,  P2, &dV1t, &dV2t, PV1, PV2 ) ;  RRC ;
       sdv[ 1 ] = fabs( dV1t) + fabs( dV2t) ;
        if ( sdv[ 1 ] < sdv[ 0 ] ) {                  //это мин.сумма импульсов
           sdv[ 0 ] = sdv[ 1 ] ;
           U.dV[0]= dV1t ; U.dV[1]= dV2t ; U.u0[0] = u1t ;  }
     }                                                        //Конец перебора
    }
  }
       pru =0 ; // с учётом времени работы КДУ
       dV1t = dV2t = 0 ;
       rc = MagicUT2(MS, dN1, dN2, kp, pru, dtQ1, dtQ2, U.u0[0], U.u0[1],
               P1,  P2, &U.dV[0], &U.dV[1], PV1, PV2 ) ;  RRC ;
   // Приращения требуемых параметров всегда
//    PT.FalseTargetKey = MKN_PARAM_ORB_T | MKN_PARAM_ORB_w ;
  return rc ;
 }

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "23" (dL, e, w для простого эллипса или ОМИВ или ДУ)
 int MagicCalcU23(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& P2, 
				  ZSLoopFact& PZ, ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
    PT.VarPar = SPO_VARIDPAR_dL ;
	return(MagicCalcU21(MS, P1, P2, PZ, PT, U));
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "24" L,T + e,w или L,T + h,Bh 
 int MagicCalcU24(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& P2, 
				  ZSLoopFact& PZ, ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	ZSLnsNpmDiffDV D;
	ZSLnsNpmFault  P;
	ZSSpoOrb       SP;
	int    rc  = 0;
	int    dN1 = PZ.Vit - U.vit[0];
	int    dN2 = PZ.Vit - U.vit[1];
	double A1, A2, B1, B2;
	double a = P1.a;
	double e = P1.e;
	double w = P1.w;
	double I = P1.i;
	double up= MS->MZ.PARu();

	U.uw[0] = U.uw[1] = 0;
	U.n = 2;
	
	if (PT.VarPar == SPO_NOTBUILD) PT.VarPar = SPO_VARIDPAR_T;

	// Проектирование требуемой орбиты по заданному параметру
	if (PT.VarPar != SPO_VARIDPAR_H && PT.VarPar != SPO_VARIDPAR_dL &&
		PT.VarPar != SPO_VARIDPAR_T) return-1;
	rc = MagicBuildSpo(PT, SP); RRC;
	//if (PT.VarPar!=SPO_VARIDPAR_T) 
	PT.T = SP.T;
	PT.a = SP.a;

	// Для ОМИВ и ДУ берём геометрию требуемой орбиты
	if ((PT.VarOrb == SPO_ORB_OMIV || PT.VarOrb == SPO_ORB_DU) && MS->GlobCount == 1) {
		PT.e  = SP.e;  PT.w  = SP.w;
		PT.e1 = SP.e1; PT.e2 = SP.e2;
	}
	// Для ОМИВ и ДУО уточняем геометрию  орбиты ( при необходимости)
	if (MS->MP.isEstimateOrb != 0 && MS->GlobCount>1 && 
	   (PT.VarOrb == SPO_ORB_OMIV || PT.VarOrb == SPO_ORB_DU)) {
		SP.e1 = PT.e1; SP.e2 = PT.e2;
	}

	// Вычисление радиусов в узловых точках требуемой орбиты и промаха
	rc = MagicCalcMishit(SP.i, SP.p, SP.e1, SP.e2, P1, PZ, PT.Ro, P); RRC;

	// Если решается задача только T,L + e,w то можно и так !
	// Вычисление полуоси по периоду	- a 
    //rc = MATC20(PT.i, PT.e1, PT.e2, PT.T, a) ;
	// Вычисление промаха по узловым точкам	- P 
    //rc = MagicCalcMishit(PT.i, a*(1-PT.e*PT.e), PT.e1, PT.e2, P1, PZ, PT.Ro, P) ; if( rc ) return rc ;
	// точка приложения импульсов	- u0
    //if (P.dRs<0) P.u0 = ValiA(P.u0+180.0, 1) ;

	U.u0[0] = P.u0;
	U.u0[1] = P.u0;

restart : ;
	// Отклонения основных параметров	- T,L
	dtQ1 = PT.T - PZ.T;
	dtQ2 = ValiDA(PT.L - PZ.L, 1);

	// производные для первого импульса 
	MagicCalcDifDV(a, e, I, w, U.u0[0], up, dN1, D);
	// производные от L, T  по V1
	A1 = D.dT; B1 = D.dL;

	// производные для второго импульса 
	MagicCalcDifDV(a, e, I, w, U.u0[1], up, dN2, D);
	// производные от L, T  по V2
	A2 = D.dT; B2 = D.dL;

	// Вычисление импульсов скорости по невязкам и найденным производным 
	// Решение системы из двух уравнений с двумя неизвестными
	// V1
	U.dV[0] = (dtQ2*A2 - dtQ1*B2) / (B1*A2 - A1*B2);
	// V2
	U.dV[1] = (dtQ1*B1 - dtQ2*A1) / (B1*A2 - A1*B2);

	{
		// Коррекция аргумента приложения первого и второго импульса
		// для точного выполнения ограничений на геометрию конечной орбиты 

		// Средняя трансверсальная скорость VS вдоль траектории перелёта
		double P1p = P1.a*(1 - P1.e*P1.e);
		double ps = (P1p + SP.p) / 2.0;
		double VS = sqrt(k_mu_kms / ps) / 2.0;
		// Нормированная величина первого импульса
		double V1 = U.dV[0] / VS;
		// Нормированная величина второго импульса
		double V2 = U.dV[1] / VS;
		// Требуемые приращения компонент вектора эксцентриситета 
		// на заданном витке. Именно это приращение должно быть реализовано,
		// посредством рассчитываемых импульсов.
		double e1 = SP.e1 - PZ.e1;
		double e2 = SP.e2 - PZ.e2;
		// модуль требуемого приращения вектора эксцентриситета
		double ekv = e1*e1 + e2*e2;
		double e = sqrt(ekv);
		double emax = fabs(V1) + fabs(V2);
		double emin = fabs(V1) - fabs(V2);

		double u1, u2;
		if (emax>e && emin<e) {
			if (fabs(V1 + V2 - e) < fabs(V1 - V2 - e)) {
				u1 = u2 = P.u0;
			} else {
				u1 = P.u0;
				u2 = ValiA(u1 + 180.0);
				if (u1>u2) { u1 = u2; u2 = P.u0; }
			}
/*
			// Возможно точное решение в части касающейся геометрии конечной орбиты
			// u1 и u2 будут отличаться от P.u0 ;
			double du1u2 = (ekv-V1*V1-V2*V2)/V1/V2/2.0 ;
			assert(fabs(du1u2)<1.00001) ;
			du1u2 = Acos(du1u2) ;
			double sfi = e1/e ;
			double cfi = e2/e ;
			double fi  = Atan2(sfi, cfi, 1) ; 
			double su1f=(e+(V1*V1-V2*V2)/e)/2.0/V1 ; 
			double su2f=(e+(V2*V2-V1*V1)/e)/2.0/V2 ; 
			// По два возможных значения для каждого угла
			assert(fabs(su1f)<1.00001) ;
			assert(fabs(su2f)<1.00001) ;
			double u11 = Asin(su1f) ;
			double u12 = 180.0-u11 ;
			double u21 = Asin(su2f) ;
			double u22 = 180.0-u21 ;
			// Вычитаем fi
			u11 = ValiA(u11-fi) ; u12 = ValiA(u12-fi) ; 
			u21 = ValiA(u21-fi) ; u22 = ValiA(u22-fi) ;
			double e1r[4] ;
			double e2r[4] ;

			e1r[0] = V1*Cos(u11)+V2*Cos(u21) ;
			e2r[0] = V1*Sin(u11)+V2*Sin(u21) ;

			e1r[1] = V1*Cos(u11)+V2*Cos(u22) ;
			e2r[1] = V1*Sin(u11)+V2*Sin(u22) ;

			e1r[2] = V1*Cos(u12)+V2*Cos(u21) ;
			e2r[2] = V1*Sin(u12)+V2*Sin(u21) ;

			e1r[3] = V1*Cos(u12)+V2*Cos(u22) ;
			e2r[3] = V1*Sin(u12)+V2*Sin(u22) ;

			// Расчёт расстояния до требуемой конечной точки
			// две пары с минимальными расстояниями (примерно равны 0) будут
			// двумя альтернативными решениями (две схемы по u)
			double d1, d2, D, MinD[2] = {999999, 999999};
			int    indMin[2] ; 
			for (int i=0 ; i<4 ; i++) {
				d1 = e1r[i]-e1 ;
				d2 = e2r[i]-e2 ;
				D = sqrt(d1*d1+d2*d2) ;
				if (D<MinD[0]) {	
					MinD[1] = MinD[0] ; indMin[1] = indMin[0] ; 
					MinD[0] = D ; indMin[0] = i ; 
				} else {
					if (D<MinD[1]) {	
						MinD[1] = D ; indMin[1] = i ; 
			}	}	}
			// Выбор решения
			double u1r[2], u2r[2] ;
			for (int j=0 ; j<2 ; j++) {
				switch (indMin[j]) {
					case 0: u1r[j] = u11 ; u2r[j] = u21 ; break ;
					case 1: u1r[j] = u11 ; u2r[j] = u22 ; break ;
					case 2: u1r[j] = u12 ; u2r[j] = u21 ; break ;
					case 3: u1r[j] = u12 ; u2r[j] = u22 ; break ;
				}
			}
			if (u1r[0]<u1r[1]) {
				u1 = u1r[0] ; u2 = u2r[0] ;
			} else {
				u1 = u1r[1] ; u2 = u2r[1] ;
			}
*/
		} else {
			if (emax<e) {
				// точного решения в части касающейся геометрии конечной орбиты нет
				u1 = u2 = P.u0;
			} else {
				// точного решения в части касающейся геометрии конечной орбиты нет
				// u1-u1 = 180.0
				// u1 или u2 = P.u0 ;
				u1 = P.u0;
				u2 = ValiA(u1 + 180.0);
				if (u1>u2) { u1 = u2; u2 = P.u0; }
			} 
		}
		bool isRestart = true;
		if (fabs(ValiDA(U.u0[0] - u1))<1e-5 && fabs(ValiDA(U.u0[1] - u2))<1e-5) 
			isRestart = false;
		U.u0[0] = u1;
		U.u0[1] = u2;
		if (isRestart) goto restart;
	}
	//kp = 1 ;                                            
	//rc = MagicUT2(dN1, dN2, kp, pru, dtQ1, dtQ2, U.u0[0], U.u0[1],
    //              P1, P2, &U.dV[0], &U.dV[1], PV1, PV2) ; RRC ;
    return rc ;
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "241" u,T + e,w или u,T + h,Bh 
int MagicCalcU241(
	ZSMagicLnsSolution* MS,	// Описание решаемой задачи
	ZSLoopFact    &P1,		// Пар-ы движения на витке первого импульса
	ZSLoopFact    &P2,		// Пар-ы движения на витке второго импульса
	ZSLoopFact    &PZ,		// Пар-ы движения на заданном витке
	ZCLnsNpmClaim &PT,		// Требуемые параметры движения
	ZSFlightRule  &U)		// Искомое управление
{
	ZSLnsNpmFault  P;
	ZSSpoOrb       SP;
	ZSLnsNpmDiffDV D;
	int		rc = 0,
			dN1 = PZ.Vit - U.vit[0],
			dN2 = PZ.Vit - U.vit[1];
	double	A1, A2, B1, B2,
			a = P1.a,
			e = P1.e,
			w = P1.w,
			I = P1.i,
			up = MS->MZ.PARu();

	U.uw[0] = U.uw[1] = 0;
	U.n = 2;

	if (PT.VarPar == SPO_NOTBUILD) PT.VarPar = SPO_VARIDPAR_T;

	// Проектирование требуемой орбиты по заданному параметру
	if (PT.VarPar != SPO_VARIDPAR_H && PT.VarPar != SPO_VARIDPAR_dL &&
		PT.VarPar != SPO_VARIDPAR_T) return-1;
	rc = MagicBuildSpo(PT, SP); RRC;
	//if (PT.VarPar!=SPO_VARIDPAR_T) 
	PT.T = SP.T;
	PT.a = SP.a;

	// Для ОМИВ и ДУ берём геометрию требуемой орбиты
	if ((PT.VarOrb == SPO_ORB_OMIV || PT.VarOrb == SPO_ORB_DU) && MS->GlobCount == 1) {
		PT.e = SP.e;  PT.w = SP.w;
		PT.e1 = SP.e1; PT.e2 = SP.e2;
	}
	// Для ОМИВ и ДУО уточняем геометрию  орбиты ( при необходимости)
	if (MS->MP.isEstimateOrb != 0 && MS->GlobCount>1 &&
		(PT.VarOrb == SPO_ORB_OMIV || PT.VarOrb == SPO_ORB_DU)) {
		SP.e1 = PT.e1; SP.e2 = PT.e2;
	}

	// Вычисление радиусов в узловых точках требуемой орбиты и промаха
	rc = MagicCalcMishit(SP.i, SP.p, SP.e1, SP.e2, P1, PZ, PT.Ro, P); RRC;

	U.u0[0] = P.u0;
	U.u0[1] = P.u0;

restart:;
	// Отклонения основных параметров - T, u
	dtQ1 = PT.T - PZ.T;
	dtQ2 = ValiDA(PT.u - PZ.up, 1);

	// производные для первого импульса 
	MagicCalcDifDV(a, e, I, w, U.u0[0], up, dN1, D);
	// производные от L, T  по V1
	A1 = D.dT; B1 = D.du;

	// производные для второго импульса 
	MagicCalcDifDV(a, e, I, w, U.u0[1], up, dN2, D);
	// производные от L, T  по V2
	A2 = D.dT; B2 = D.du;

	// Вычисление импульсов скорости по невязкам и найденным производным 
	// Решение системы из двух уравнений с двумя неизвестными
	// V1
	U.dV[0] = (dtQ2*A2 - dtQ1*B2) / (B1*A2 - A1*B2);
	// V2
	U.dV[1] = (dtQ1*B1 - dtQ2*A1) / (B1*A2 - A1*B2);

	{
		// Коррекция аргумента приложения первого и второго импульса
		// для точного выполнения ограничений на геометрию конечной орбиты 

		// Средняя трансверсальная скорость VS вдоль траектории перелёта
		double P1p = P1.a*(1 - P1.e*P1.e);
		double ps = (P1p + SP.p) / 2.0;
		double VS = sqrt(k_mu_kms / ps) / 2.0;
		// Нормированная величина первого импульса
		double V1 = U.dV[0] / VS;
		// Нормированная величина второго импульса
		double V2 = U.dV[1] / VS;
		// Требуемые приращения компонент вектора эксцентриситета 
		// на заданном витке. Именно это приращение должно быть реализовано,
		// посредством рассчитываемых импульсов.
		double e1 = SP.e1 - PZ.e1;
		double e2 = SP.e2 - PZ.e2;
		// модуль требуемого приращения вектора эксцентриситета
		double ekv = e1*e1 + e2*e2;
		double e = sqrt(ekv);
		double emax = fabs(V1) + fabs(V2);
		double emin = fabs(V1) - fabs(V2);

		double u1, u2;
		if (emax>e && emin<e) {
			if (fabs(V1 + V2 - e) < fabs(V1 - V2 - e)) {
				u1 = u2 = P.u0;
			} else {
				u1 = P.u0;
				u2 = ValiA(u1 + 180.0);
				if (u1>u2) { u1 = u2; u2 = P.u0; }
			}
		} else {
			if (emax<e) {
				// точного решения в части касающейся геометрии конечной орбиты нет
				u1 = u2 = P.u0;
			} else {
				// точного решения в части касающейся геометрии конечной орбиты нет
				// u1-u1 = 180.0
				// u1 или u2 = P.u0 ;
				u1 = P.u0;
				u2 = ValiA(u1 + 180.0);
				if (u1>u2) { u1 = u2; u2 = P.u0; }
		}	}
		bool isRestart = true;
		if (fabs(ValiDA(U.u0[0] - u1))<1e-5 && fabs(ValiDA(U.u0[1] - u2))<1e-5)
			isRestart = false;
		U.u0[0] = u1;
		U.u0[1] = u2;
		if (isRestart) goto restart;
	}
	return rc;
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту 25 (L,h+Bh или L,Hs+e,w для ОМИВ, ДУ или элиипса")
int MagicCalcU25(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& P2, 
				 ZSLoopFact& PZ, ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
    if (PT.VarOrb==SPO_ORB_OMIV || PT.VarOrb==SPO_ORB_DU || PT.VarOrb==SPO_ORB_EL) {
       PT.VarPar = SPO_VARIDPAR_H ;
       PT.VarH   = SPO_VARH_MID ;
	   return (MagicCalcU24(MS, P1, P2, PZ, PT, U));
    }

    PT.VarH = SPO_VARH_MIN ;
    int      i, j, rc = 0 ;
    double   dLp ,fi ;
    double   A1, A2, D1, D2, B1, B2, E1, E2, d ;
    double   u1[2], u2[2], dV1[2], dV2[2], b1, b2 ;
    // Невязка по долготе
    dtQ1 = ValiDA(PZ.L-PT.L, 1) ;
    // Производная долготы восходящего узла по времени
    double dW =-360.0/P1.a /P1.a*k_eps/k_mu_kms*Cos(P1.i)/P1.T ;
    double  p = P1.a*(1.0-P1.e*P1.e) ;
    // Вращение Земли [гдад/сек]
	double k_omega_gs = GradA(k_omega)/86400.0 ;
    // Приведённая невязка долготы
    dLp = dtQ1*k_mu_kms/p/p/(k_omega_gs-dW) ;

    MagicAD12FromHBh(MS, P1, PZ, PT, A1, A2, D1, D2) ;
	if (MS->GlobCount == 1) {
		u1[0] = P1.w ;     
		u2[0] = ValiA(PZ.w+180.0, 1) ;
	} else { 
		u1[0] = U.u0[0] ;  
		u2[0] = U.u0[1]; 
	}
	for (j=0; j<3; j++) {
		fi = 2.0*k_pi*(PZ.Vit-U.vit[0])-RadA(u1[0]) ;
		b1 =(3.0*fi+fi*k_eps/k_mu_kms*4.0/P1.a/P1.a*(2.0-1.5*pow(Sin(PZ.i), 2))-4.0*Sin(u1[0])) ;
		fi = 2.0*k_pi*(PZ.Vit-U.vit[1])-RadA(u2[0]) ;
		b2 =(3.0*fi+fi*k_eps/k_mu_kms*4.0/P2.a/P2.a*(2.0-1.5*pow(Sin(PZ.i), 2))-4.0*Sin(u2[0])) ;
		B1 = A1*dLp+D1*b2 ;  B2 = A2*dLp+D1*b2 ;
		E1 = A1*(b2-b1) ;    E2 = A2*(b2-b1) ;
		d  =(E1*E1+E2*E2)*dLp*dLp+(B1*B1+B2*B2)*pow((b1+b2),2)+
			 2.0*(b1+b2)*(E1*B1+E2*B2)*dLp-pow((E1*B2-E2*B1),2) ;
		d  = d<0 ? 0:sqrt(d) ;

		// Расчёт управлений
		for (i=0 ; i<2 ; i++) {
			dV1[i]=(d-(E1*B1+E2*B2+(b1+b2)*dLp))/(E1*E1+E2*E2-pow(b1+b2, 2)) ;
			dV2[i]=(dLp-dV1[i]*b1)/b2 ;
			u1[i] = AtanSC((E2*dV1[i]+B2)/(E1*dV1[i]+B1), (E1*dV1[i]+B1)/((b1+b2)*dV1[i]-dLp));
			u2[i] = ValiA(u1[i]+180.0, 1) ;
			if (d!=0) d =-d ;
		}
		if (d>0 && ((fabs(dV1[0])+fabs(dV2[0])) > (fabs(dV1[1])+fabs(dV2[1])))) {
			u1[0] = u1[1] ; u2[0] = u2[1] ; 
		}
	}
	// Выбор схемы
	// 1-ая схема
    i = 0 ;                                             
	// 2-ая схема
    if (d>0 && (fabs(dV1[0])+fabs(dV2[0])>fabs(dV1[1])+fabs(dV2[1]))) i = 1 ;   
    U.plane = i+1 ;    
	U.n     = 2 ;
    U.u0[0] = u1[i] ;  U.u0[1] = u2[i] ;
    U.uw[0] = U.uw[1] = 0 ;
    U.dV[0] = dV1[i] ; U.dV[1] = dV2[i] ;
	return rc ;
}

//-----------------------------------------------------------------------------
/*
//   Попытка по новому
// Расчёт управлений по варианту "25" ("L, h + Bh для эл."
//                                 или "L, h + e, w для ОМИВ или ДУ")
int MagicCalcU25( ZSLoopFact& P1, ZSLoopFact& P2, ZSLoopFact& PZ,
                      ZCLnsNpmClaim& PT,  MKNUPR& U )
{
    if ( PT.VarOrb == SPO_ORB_OMIV || PT.VarOrb == SPO_ORB_DU ) {
       PT.VarPar = SPO_VARIDPAR_H ;
       PT.VarH   = SPO_VARH_MID ;
       return ( MagicCalcU24( P1, P2, PZ, PT, U ) ) ;   //если ОМИВ или ДУО
    }
    PT.VarH = SPO_VARH_MIN ;
    int      dN, i, j, rc = 0 ;
    double   dLp ,fi ;
    double   A1, A2, D1, D2, B1, B2, E1, E2, d ;
    double   u1[ 2 ], u2[ 2 ], dV1[ 2 ], dV2[ 2 ], b1, b2 ;
    ZSLnsNpmFault  P ;
    ZSLnsNpmDiffDV D ;
    ZSLnsNpmDiffDV DP ;
    // Невязка по долготе
    dtQ1 = ValiDA( PZ.L - PT.L, 1 ) ;
    // Производная долготы восходящего узла по времени
    double dW =-360. /  P1.a /P1.a * k_eps / k_mu * Cos( P1.i )/ P1.T ;
    // Приведённая невязка долготы
    double  p = P1.a * (1. - P1.e * P1.e ) ;
    dLp =  RadA( dtQ1 ) * k_mu  / p / p / ( RadA( k_omega ) - RadA( dW ) ) ;
    MagicAD12FromHBh( P1, PZ, PT, A1, A2, D1, D2 ) ;
    if  ( GlobCount == 1 ) { u1[0] = P1.w ;     u2[0] = ValiA( PZ.w + 180., 1);}
    else                   { u1[0] = U.u0[0] ;  u2[0] = U.u0[1]; }
  for ( j= 0 ; j< 3 ; j++ ) {
         fi =  2.* pi * ( PZ.Vit - U.vit[0] ) -  RadA( u1[0]) ;
         b1 = (3. * fi + fi * k_eps / k_mu * 4. / P1.a / P1.a *
              (2. - 1.5 * pow( Sin( PZ.i ), 2 ) ) - 4. * Sin( u1[0])) ;
         fi =  2.* pi * ( PZ.Vit - U.vit[1] ) -  RadA( u2[0]) ;
         b2 = (3. * fi + fi * k_eps / k_mu * 4. / P2.a / P2.a *
              (2. - 1.5 * pow( Sin( PZ.i ), 2 ) ) - 4. * Sin( u2[0])) ;
         B1 = A1*dLp + D1*b2 ;      B2 = A2*dLp + D1*b2 ;
         E1 = A1 * ( b2 - b1 ) ;    E2 = A2 * ( b2- b1 ) ;
         d = ( E1*E1 + E2*E2 )*dLp*dLp + (B1*B1 + B2*B2)*pow((b1+b2),2)+
             2.*( b1 + b2 )*(E1*B1 + E2*B2)*dLp - pow((E1*B2 - E2*B1),2 );
         d = d < 0 ? 0 : sqrt( d ) ;
        // Расчёт управления
     for ( i= 0 ; i< 2 ; i++ ) {
        dV1[ i ]=( d - ( E1*B1 + E2*B2 + (b1 + b2)* dLp ) ) /
                  ( E1*E1 + E2*E2 - pow( b1 + b2, 2) ) ;
        dV2[ i ]= ( dLp - dV1[ i ] * b1 ) / b2 ;
        u1[ i ] = AtanSC( ( E2 * dV1[ i ] + B2 ) / ( E1 * dV1[ i ] + B1 ),
                          ( E1 * dV1[ i ] + B1 )/((b1 + b2) * dV1[i] - dLp ));
        u2[ i ] = ValiA( u1[ i ] + 180.0, 1 ) ;
        if ( d != 0 ) d = -d ;
     }
     if ( d >0 && ((fabs(dV1[0])+ fabs(dV2[0])) > (fabs(dV1[1])+ fabs(dV2[1]))))
         { u1[ 0 ] = u1[ 1 ] ; u2[ 0 ] = u2[ 1 ] ; }
  }
//----------------------------Выбор схемы
    i = 0 ;                                             //1-ая схема
    if ( d >0 && (fabs(dV1[0])+ fabs(dV2[0]) >
                  fabs(dV1[1])+ fabs(dV2[1])) ) i= 1 ;   // 2-ая схема
    U.plane   = i + 1;    U.n       = 2 ;
    U.u0[ 0 ] = u1[ i ] ; U.u0[ 1 ] = u2[ i ] ;
    U.uw[ 0 ] = 0 ;       U.uw[ 1 ] = 0 ;
    U.dV[ 0 ] = dV1[ i ]; U.dV[ 1 ] = dV2[ i ];
    // Признак приращения требуемых параметров
   if ( MP.isEstimateOrb == 0 )
    PT.FalseTargetKey = MKN_PARAM_ORB_h | MKN_PARAM_ORB_L  ;
   if ( MP.isEstimateOrb != 0 )
    PT.FalseTargetKey = MKN_PARAM_ORB_h | MKN_PARAM_ORB_L  | MKN_PARAM_ORB_Bh ;
  return rc ;
}
*/

//-----------------------------------------------------------------------------
//Расчёт управлений по варианту 26 (L,dL+Bh для эл. или L,dL+e,w для ОМИВ, ДУ)
int MagicCalcU26(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& P2, 
				 ZSLoopFact& PZ, ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
    PT.VarPar = SPO_VARIDPAR_dL ;
    return (MagicCalcU24(MS, P1, P2, PZ, PT, U));
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту 27 (L,T + w только для эл.)
int MagicCalcU27(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& P2, 
				 ZSLoopFact& PZ, ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
    int    dN1 = PZ.Vit-P1.Vit ;
    int    dN2 = PZ.Vit-P2.Vit ;
    int    rc = 0 ;
    double dw,  sdw[ 2 ] ;
    double u1t, u2t, V1t, V2t ;

    dw   = ValiDA(PT.w-PZ.w, 1) ;
    dtQ1 = PT.T-PZ.T ;
    dtQ2 =-ValiDA(PZ.L-PT.L, 1) ;
	// корр.параметры L и Т
    kp = 1 ;     
	// уточнение V1, V2 с  учётом времени работы КДУ
    pru =0 ;     
	U.plane = U.uw[0] = U.uw[1] = 0 ; U.n = 2 ;

	// не заданы  аргументы придожения
	if (U.isFixedArg == 0 && MS->GlobCount <= 3) {
		u1t = P1.w ; u2t = P2.w ;
		// при переборе  уточнение V1, V2 без учёта времени работы КДУ
		pru =1 ;     
		sdw[0] = 360.0 ;
		while ( 1 ) {
			// уточнение V1,V2 методом трапеций
			V1t = V2t = 0;
			rc = MagicUT2(MS, dN1, dN2, kp, pru, dtQ1, dtQ2,  u1t, u2t, P1, P2, &V1t, &V2t, PV1, PV2) ;  RRC ;
			// Невязка по аргументу перигея
			sdw[1] = fabs(ValiDA(PT.w-PV2[2] ,1)) ;
			if (sdw[1]<sdw[0]) {
				U.u0[0] = u1t ; U.dV[0] = V1t ; 
				U.u0[1] = u2t ; U.dV[1] = V2t ;
				// сохраняем найденноe управление
				sdw[0] = sdw[1] ;                    
			}
			// Новое значение аргумента широты u1
			u1t = ValiA(u1t+k_stepu*Sign1(dw*V1t), 1) ;
			if (fabs(ValiDA(u1t-PV1[2],1))<=90.0) continue ;
			u1t = P1.w ;
			// Новое значение аргумента широты u2
			u2t = ValiA(u2t+k_stepu*Sign1(dw*V2t), 1) ;
			if (fabs(ValiDA(u2t-PV2[2],1))<=90.0) continue ;
			break ;
		}
	}
	// уточнение V1, V2 с  учётом времени работы КДУ
	pru = 0 ;                
	U.dV[0] = U.dV[1] = 0 ;
	rc = MagicUT2(MS, dN1, dN2, kp, pru, dtQ1, dtQ2, U.u0[0], U.u0[1], P1, P2, &U.dV[0], &U.dV[1], PV1, PV2) ; RRC ;
//	if (GlobCount>3) {
//		U.dV[0] = U.dV[1] = 0 ;
//		rc = MagicUT2(dN1, dN2, kp, pru, dtQ1, dtQ2, U.u0[0], U.u0[1], P1, P2, &U.dV[0], &U.dV[1], PV1, PV2) ;  RRC ;
//	}
/*
  if (U.isFixedArg && GlobCount <= 2 ) {       //  заданы  аргументы приложения
      k  = 175./ k_stepu ;
    if (U.plane==1) {                        // задан u1
      du2 = - k_stepu * Sign( dtQ1*dtQ2 );
//      u2t = ValiA( P2.w + 10.*Sign(du2) ,1);           // Начальное значение u2
      pru =1 ;                     // при переборе без учёта времени работы КДУ
      u2t = P2.w ;
      pru =1 ;     // при переборе  уточнение V1, V2 без учёта времени работы КДУ
      sdw[ 0 ] = 360. ;


     for ( i = 0 ; i < k ; i++ ) {                           // Начало перебора
     // Текущее значение аргумента широты u2
      u2t = ValiDA(u2t + du2 ,1) ;
      if ( u2t <0 ) u2t += 360. ;
      //----Определение V1,V2  с уточнением  методом трапеций
       dV1t = dV2t = 0 ;
      rc = MagicUT2( dN1, dN2, kp, pru, dtQ1, dtQ2,  U.u0[0], u2t,
                      P1,  P2, &dV1t, &dV2t, PV1, PV2 ) ;  RRC ;
       sdv[ 1 ] = fabs( dV1t) + fabs( dV2t) ;
       if ( sdv[ 1 ] < sdv[ 0 ] ) {                  //это мин.сумма импульсов
          sdv[ 0 ] = sdv[ 1 ] ;
          U.dV[0]= dV1t ; U.dV[1]= dV2t ;   U.u0[1] = u2t ; }
     }                                                        //Конец перебора
    }
    if (U.plane==2) {                         // задан u2
      du1 = k_stepu * Sign( dtQ1*dtQ2 );
      u1t = ValiA( P1.w + 10.*Sign(du1) ,1);           // Начальное значение u2
      pru =1 ;                     // при переборе без учёта времени работы КДУ
     for ( i = 0 ; i < k ; i++ ) {                           // Начало перебора
     // Текущее значение аргумента широты u1
      u1t = ValiDA(u1t + du1 ,1) ;
      if ( u1t <0 ) u1t += 360. ;
      //----Определение V1,V2  с уточнением  методом трапеций
       dV1t = dV2t = 0 ;
       rc = MagicUT2( dN1, dN2, kp, pru, dtQ1, dtQ2, u1t, U.u0[1],
                       P1,  P2, &dV1t, &dV2t, PV1, PV2 ) ;  RRC ;
       sdv[ 1 ] = fabs( dV1t) + fabs( dV2t) ;
        if ( sdv[ 1 ] < sdv[ 0 ] ) {                  //это мин.сумма импульсов
           sdv[ 0 ] = sdv[ 1 ] ;
           U.dV[0]= dV1t ; U.dV[1]= dV2t ; U.u0[0] = u1t ;  }
     }                                                        //Конец перебора
    }
  }
       pru =0 ; // с учётом времени работы КДУ
       dV1t = dV2t = 0 ;
       rc = MagicUT2( dN1, dN2, kp, pru, dtQ1, dtQ2, U.u0[0], U.u0[1],
               P1,  P2, &U.dV[0], &U.dV[1], PV1, PV2 ) ;  RRC ;
*/
  return rc ;
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту 28 (L,T только для эл.)
int MagicCalcU28(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& P2, 
				 ZSLoopFact& PZ, ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
	int dN1 = PZ.Vit-P1.Vit ;
    int dN2 = PZ.Vit-P2.Vit, rc = 0 ;
    // Вычисление невязок по Т,L
    dtQ1 = PT.T-PZ.T ;
    dtQ2 =-ValiDA(PZ.L-PT.L, 1) ;
	//корр.параметры L и Т
    kp = 1 ;          
	// Признак уточнения V1, V2 с учётом времени работы КДУ
    pru= 0 ;          
    U.n= 2 ;
	if (MS->GlobCount <= 1) {
		// не заданы  аргументы придожения
		if (U.isFixedArg==0) {          
			U.uw[0] = 1 ;
			U.uw[1] = 1 ;
			U.u0[0] = P1.w ; 
			U.u0[1] = P2.w ;
		}
		if (U.isFixedArg && U.plane==1) U.u0[1] = P2.w ;  //  задан u1
		if (U.isFixedArg && U.plane==2) U.u0[0] = P1.w ;  //  задан u2
	}
	// Вычисление V1, V2 с уточнением  методом трапеций
	U.dV[0] = U.dV[1] = 0;
	rc = MagicUT2(MS, dN1, dN2, kp, pru, dtQ1, dtQ2, U.u0[0],
		 U.u0[1], P1, P2, &U.dV[0], &U.dV[1], PV1, PV2);
	return rc ;
}

//-----------------------------------------------------------------------------
// Расчёт управлений по варианту "29" ("L, w только для эл.")
int MagicCalcU29(ZSMagicLnsSolution* MS, ZSLoopFact& P1, ZSLoopFact& P2, 
				 ZSLoopFact& PZ, ZCLnsNpmClaim& PT, ZSFlightRule& U)
{
    int      rc = 0 ;
    int      dN1 = PZ.Vit-P1.Vit ;
    int      dN2 = PZ.Vit-P2.Vit ;
    double   du1, du2, sdv[ 2 ] ;
    double   u1t, u1t0, u2t, dV1t=0, dV2t=0 ;

    kp   = 2 ;                                    //корр.параметры L и w
    U.uw[ 0 ] = U.uw[ 1 ] = 0 ;   U.n   = 2 ;
    dtQ1 =-ValiDA(PZ.L-PT.L, 1) ;
    dtQ2 = ValiDA(PT.w-PZ.w, 1) ;
    sdv[ 0 ] = 360.0 ;
	//  уточнение импульсов с учётом времени работы КДУ
    pru =0 ;          
	// не заданы  аргументы придожения
	//if (U.isFixedArg==0 && GlobCount<=3) { 
	if (U.isFixedArg==0) { 
		double A = 6.0*k_pi*P1.a*P1.a/k_mu_kms ;
		double p = P1.a*(1.0-P1.e*P1.e) ;
		// Производная долготы восходящего узла по времени [град/сек]
		double dW=-360.0/p/p*k_eps/k_mu_kms*Cos(P1.i)/P1.T ;
		// Вращение Земли [гдад/сек]
		double k_omega_gs = GradA(k_omega)/86400.0 ;
/*
    // Производная долготы восходящего узла по времени
    double dW =-360.0/P1.a /P1.a*k_eps/k_mu_kms*Cos(P1.i)/P1.T ;
    double  p = P1.a*(1.0-P1.e*P1.e) ;
    // Вращение Земли [гдад/сек]
	double k_omega_gs = GradA(k_omega)/86400.0 ;
    // Приведённая невязка долготы
    dLp = dtQ1*k_mu_kms/p/p/(k_omega_gs-dW) ;
*/
		//Приближённые значения импульсов
		dV1t = dtQ1/(k_omega-dW)/(dN1-P1.w/360.0)/A ;
		dV2t = fabs(RadA(dtQ2)/2.0*P1.e/sqrt(p/k_mu_kms)) ;

		if (MS->GlobCount == 1 && fabs(dV1t) >= fabs(dV2t)) U.plane = 11;
		if (MS->GlobCount == 1 && fabs(dV1t)< fabs(dV2t)) U.plane = 12;

		//когда 1 имп. манёвр не возможен
		if (U.plane == 12 ) {  
			du1 = k_stepu * Sign1( dtQ1*dtQ2 );
			du2 = - du1 ;
			u1t0= u1t = ValiA( P1.w + 10.*Sign1(du1) ,1) ;
			u2t = ValiA( P2.w + 10.*Sign1(du2) ,1);
			// при переборе без учёта времени работы КДУ
			pru =1 ;  
			while(1) {
				// уточнение V1,V2 методом трапеций
				dV1t = dV2t = 0 ;
				rc = MagicUT2(MS, dN1, dN2, kp, pru, dtQ1, dtQ2, u1t, u2t,
					          P1, P2, &dV1t, &dV2t, PV1, PV2) ;  RRC ;
				sdv[1] = fabs(dV1t)+fabs(dV2t) ;
				//это мин.сумма импульсов
				if(sdv[1]<sdv[0]) { 
					sdv [0] = sdv[1] ;
					U.u0[0] = u1t ;  U.u0[1] = u2t ;
					U.dV[0] = dV1t ; U.dV[1] = dV2t ;
				}
				// Новое значение аргумента широты u1
				u1t = ValiA( u1t + du1, 1 ) ;
				if (fabs(ValiDA(u1t-P1.w, 1))<=85.) continue ;
				// Перебор по u1 закончен
				u1t = u1t0 ;
				// Новое значение аргумента широты u2
				u2t = ValiA(u2t+du2, 1) ;
				if (fabs(ValiDA(u2t-PV1[2], 1))<=85.)  continue ;
				break ;
			}
		}

		//когда возможен 1 имп.манёвр
		if (U.plane==11) {   
			if (MS->GlobCount == 1) MS->GlobCount = MS->GlobCount; //MMessageBox( "Возможен 1имп. манёвр" , 2, 0 ) ;
			dV1t = dV2t = 0 ;
			dV1t = dV1t - fabs(dV2t) ;
			du2 = k_stepu * Sign1( dtQ1*dtQ2 ) ;
		    u1t = ValiA( P1.w +  2.*Sign1(du1) ,1) ;
			u2t = ValiA( P2.w + 50.*Sign1(du2) ,1);
			while (1) {
				// уточнение V1,V2 методом трапеций
				dV1t = dV2t = 0 ;
				rc = MagicUT2(MS, dN1, dN2, kp, pru, dtQ1, dtQ2, u1t, u2t,
							  P1, P2, &dV1t, &dV2t, PV1, PV2) ;  RRC ;
				sdv[1] = fabs(dV1t)+fabs(dV2t) ;
				if (sdv[1]<sdv[0]) {
					sdv[0] = sdv[1] ;
					U.u0[0]= u1t ;  U.u0[1] = u2t ;
					U.dV[0]= dV1t ; U.dV[1] = dV2t ;
				}
				// Новое значение аргумента широты u2
				u2t = ValiA( u2t +  du2 ,1 ) ;
				if (fabs(ValiDA(u2t-PV1[2], 1))<=85.0) continue ;
				break ;
			}
		}
	}
/*
	//  заданы  аргументы приложения
	if (U.isFixedArg && GlobCount<= 2) {       
		k  = 95.0/k_stepu ;
    if (U.plane==1) {                        // задан u1
      du2 = - k_stepu * Sign( dtQ1*dtQ2 );
      u2t = ValiA( P2.w + 10.*Sign(du2) ,1);           // Начальное значение u2
      pru =1 ;                     // при переборе без учёта времени работы КДУ
     for ( i = 0 ; i < k ; i++ ) {                           // Начало перебора
     // Текущее значение аргумента широты u2
      u2t = ValiDA(u2t + du2 ,1) ;
      if ( u2t <0 ) u2t += 360. ;
      //----Определение V1,V2  с уточнением  методом трапеций
       dV1t = dV2t = 0 ;
      rc = MagicUT2( dN1, dN2, kp, pru, dtQ1, dtQ2,  U.u0[0], u2t,
                      P1,  P2, &dV1t, &dV2t, PV1, PV2 ) ;  RRC ;
       sdv[ 1 ] = fabs( dV1t) + fabs( dV2t) ;
       if ( sdv[ 1 ] < sdv[ 0 ] ) {                  //это мин.сумма импульсов
          sdv[ 0 ] = sdv[ 1 ] ;
          U.dV[0]= dV1t ; U.dV[1]= dV2t ;   U.u0[1] = u2t ; }
     }                                                        //Конец перебора
    }
    if (U.plane==2) {                         // задан u2
      du1 = k_stepu * Sign( dtQ1*dtQ2 );
      u1t = ValiA( P1.w + 10.*Sign(du1) ,1);           // Начальное значение u2
      pru =1 ;                     // при переборе без учёта времени работы КДУ
     for ( i = 0 ; i < k ; i++ ) {                           // Начало перебора
     // Текущее значение аргумента широты u1
      u1t = ValiDA(u1t + du1 ,1) ;
      if ( u1t <0 ) u1t += 360. ;
      //----Определение V1,V2  с уточнением  методом трапеций
       dV1t = dV2t = 0 ;
       rc = MagicUT2( dN1, dN2, kp, pru, dtQ1, dtQ2, u1t, U.u0[1],
                       P1,  P2, &dV1t, &dV2t, PV1, PV2 ) ;  RRC ;
       sdv[ 1 ] = fabs( dV1t) + fabs( dV2t) ;
        if ( sdv[ 1 ] < sdv[ 0 ] ) {                  //это мин.сумма импульсов
           sdv[ 0 ] = sdv[ 1 ] ;
           U.dV[0]= dV1t ; U.dV[1]= dV2t ; U.u0[0] = u1t ;  }
     }                                                        //Конец перебора
    }
  }
*/
	pru =0 ; // с учётом времени работы КДУ
	dV1t= dV2t = 0 ;
	rc  = MagicUT2(MS, dN1, dN2, kp, pru, dtQ1, dtQ2, U.u0[0], U.u0[1],
                   P1, P2, &U.dV[0], &U.dV[1], PV1, PV2) ; RRC ;
	return rc ;
}

//-----------------------------------------------------------------------------
