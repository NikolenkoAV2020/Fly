//---------------------------------------------------------------------------
// MagicBuildSPO.cpp: 
// А. Николенко 16.08.2018
//---------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции
#include <FlyCore.h>		// Математические функции
#include <FlyMagicLNS.h>	//

//---------------------------------------------------------------------------
// Модуль проектирования орбит - расчёт геометрических характеристик
// орбиты (p, e, e1 = e*cos(w), e2 = e*sin(w)) по заданным :
// периоду, средней высоте или суточному смещению
//---------------------------------------------------------------------------
int MagicBuildSpo(ZCLnsNpmClaim& TZ, ZSSpoOrb& SP, char* Msg)
{
    int rc ;
	if (Msg) Msg[0] = '\0' ;
    memset(&SP, 0, sizeof(ZSSpoOrb)) ;
    switch (TZ.VarOrb) {
		// Эллиптическая орбита задаётся минимальной высотой и её широтой
		case SPO_ORB_HBH:
			//if (TZ.VarH!=SPO_VARH_MIN && TZ.VarH!=SPO_VARH_MAX) return-4 ;
			switch ( TZ.VarPar ) {
				// по большой полуоси
				case SPO_VARIDPAR_a:
					rc = MBuildEL_ahBh(TZ.i, TZ.a, TZ.Hmin, TZ.VarH, TZ.BH, TZ.VarB, SP.p, SP.e1, SP.e2, TZ.uH, Msg) ; RRC ;
					SP.a = TZ.a ; 
					SP.i = TZ.i ;
					SP.uh= TZ.uH ;
					break ;
				// по периоду
				case SPO_VARIDPAR_T:
					rc = MBuildEL_ThBh(TZ.i, TZ.T, TZ.Hmin, TZ.VarH, TZ.BH, TZ.VarB, SP.p, SP.e1, SP.e2, TZ.uH, Msg) ; RRC ;
					SP.T = TZ.T ; 
					SP.i = TZ.i ;
					SP.uh= TZ.uH ;
					break ;
				// по средней высоте
				case SPO_VARIDPAR_H:
					rc = MBuildEL_HshBh(TZ.i, TZ.Hs, TZ.Hmin, TZ.VarH, TZ.BH, TZ.VarB, SP.p, SP.e1, SP.e2, TZ.uH, Msg) ; RRC ;
					SP.i = TZ.i ;
					SP.uh= TZ.uH ;
					break ;
				// по суточному дрейфу
				case SPO_VARIDPAR_dL:
					//if (TZ.dL <0) TZ.N +=1;
					rc = MBuildEL_dLhBh(TZ.i, TZ.N, TZ.dL, TZ.Hmin, TZ.VarH, TZ.BH, TZ.VarB, SP.T, SP.p, SP.e1, SP.e2, TZ.uH, Msg) ; RRC ;
					SP.i = TZ.i ;
					SP.uh= TZ.uH ;
					break ;
				default: return-20 ;
			}
			break ;
		// Эллиптическая орбита задаётся эксцентриситетом и положением перигея
		case SPO_ORB_EL:
			switch (TZ.VarPar) {
				// по полуоси
				case SPO_VARIDPAR_a:
					rc = MBuildEL_a(TZ.i, TZ.a, TZ.e, TZ.w, SP.T, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.i = TZ.i ;
					break ;
				// по периоду
				case SPO_VARIDPAR_T:
					TZ.e1 = TZ.e*Cos(TZ.w) ;
					TZ.e2 = TZ.e*Sin(TZ.w) ;
					rc = MATC20(TZ.i, TZ.e1, TZ.e2, TZ.T, TZ.a) ; RRC ;
					SP.a = TZ.a ;
					SP.p = TZ.a*(1-TZ.e*TZ.e) ;
					SP.e1= TZ.e1 ; 
					SP.e2= TZ.e2 ;
					SP.e = TZ.e ; 
					SP.w = TZ.w ;
					SP.T = TZ.T ;
					SP.i = TZ.i ;
					break ;
				// по суточному дрейфу
				case SPO_VARIDPAR_dL:
					TZ.e1 = TZ.e*Cos(TZ.w) ;
					TZ.e2 = TZ.e*Sin(TZ.w) ;
					rc = MBuildEL_dLew (TZ.i, TZ.N, TZ.dL, TZ.e1, TZ.e2, SP.T, SP.p, SP.uh, Msg) ; RRC ;
					SP.e1= TZ.e1 ; SP.e2= TZ.e2 ;
					SP.i = TZ.i ;
					break ;
				case SPO_VARIDPAR_H:
					TZ.e1 = TZ.e*Cos(TZ.w) ;
					TZ.e2 = TZ.e*Sin(TZ.w) ;
					rc =  MBuildEL_hs(TZ.i, TZ.Hs, TZ.e1, TZ.e2, SP.T, SP.p, Msg) ; RRC ;
					SP.e1= TZ.e1 ; SP.e2= TZ.e2 ;
					SP.i = TZ.i ;
					SP.uh= 0 ;
					break ;
				default: return-20 ;
			}
			break ;
		// ОМИВ
		case SPO_ORB_OMIV :
			switch (TZ.VarPar) {
				// по полуоси
				case SPO_VARIDPAR_a:
					rc = MBuildOMIV_a(TZ.i, TZ.a, SP.T, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.i = TZ.i ;
					break ;
				// по периоду
				case SPO_VARIDPAR_T:
					rc = MBuildOMIV_T(TZ.i, TZ.T, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.T = TZ.T ; SP.i = TZ.i ;
					break ;
				// по средней высоте
				case SPO_VARIDPAR_H:
					if (TZ.VarH!=SPO_VARH_MID) return-4 ;
					rc = MBuildOMIV_hs(TZ.i, TZ.Hs, SP.T, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.i = TZ.i ;
					break ;
				// по суточному дрейфу
				case SPO_VARIDPAR_dL:
					rc = MBuildOMIV_dL(TZ.i, TZ.N, TZ.dL, SP.T, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.i = TZ.i ;
					break ;
				default: return-20 ;
			}
			break ;
		// ДУ
		case SPO_ORB_DU:
			switch (TZ.VarPar) {
				// по полуоси
				case SPO_VARIDPAR_a:
					rc = MBuildDU_a(TZ.i, TZ.a, SP.T, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.i = TZ.i ;
					break ;
				// по периоду
				case SPO_VARIDPAR_T:
					rc = MBuildDU_T(TZ.i, TZ.T, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.T = TZ.T ; SP.i = TZ.i ;
					break ;
				// по средней высоте
				case SPO_VARIDPAR_H:
					if (TZ.VarH!=SPO_VARH_MID) return-4 ;
					rc = MBuildDU_hs(TZ.i, TZ.Hs, SP.T, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.i = TZ.i ;
					break ;
				// по суточному дрейфу
				case SPO_VARIDPAR_dL :
					rc = MBuildDU_dL(TZ.i, TZ.N, TZ.dL, SP.T, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.i = TZ.i ;
					break ;
				default: return-20 ;
			}
			break ;
		// эллипс СС
		case SPO_ORB_HBHSS:
			switch (TZ.VarPar) {
				// по периоду
				case SPO_VARIDPAR_T:
					rc = MBuildSSEL_T(TZ.T, TZ.Hmin, TZ.VarH, TZ.BH, TZ.VarB, SP.i, SP.p, SP.e1, SP.e2, SP.uh, Msg) ; RRC ;
					SP.T = TZ.T ;
					break ;
				// по суточному дрейфу
				case SPO_VARIDPAR_dL:
					rc = MBuildSSEL_dL(TZ.N, TZ.dL, TZ.Hmin, TZ.VarH, TZ.BH, TZ.VarB, SP.T, SP.i, SP.p, SP.e1, SP.e2, SP.uh, Msg) ; RRC ;
					break ;
				// по наклонению
				case SPO_VARIDPAR_i :
					rc = MBuildSSEL_i(TZ.i, TZ.Hmin, TZ.VarH, TZ.BH, TZ.VarB, SP.T, SP.p, SP.e1, SP.e2, SP.uh, Msg) ; RRC ;
					SP.i = TZ.i ;
					break ;
				default: return-20 ;
			}
			break ;
		// ОМИВ СС
		case SPO_ORB_OMIVSS:
			switch (TZ.VarPar) {
				// по периоду
				case SPO_VARIDPAR_T:
					rc = MBuildSSOMIV_T(TZ.T, SP.i, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.T = TZ.T ;
					break ;
				// по средней высоте
				case SPO_VARIDPAR_H:
					if (TZ.VarH!=SPO_VARH_MID) return-4 ;
					rc = MBuildSSOMIV_hs(TZ.Hs, SP.T, SP.i, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					break ;
				// по суточному дрейфу
				case SPO_VARIDPAR_dL:
					rc = MBuildSSOMIV_dL(TZ.dL, TZ.N, SP.T, SP.i, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					break ;
				// по наклонению
				case SPO_VARIDPAR_i:
					rc = MBuildSSOMIV_i(TZ.i, SP.T, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.i = TZ.i ;
					break ;
				default: return-20 ;
			}
			break ;
		// ДУ СС
		case SPO_ORB_DUSS:
			switch(TZ.VarPar) {
				// по периоду
				case SPO_VARIDPAR_T:
					rc = MBuildSSDU_T(TZ.T, SP.i, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.T = TZ.T ;
					break ;
				// по средней высоте
				case SPO_VARIDPAR_H:
					if (TZ.VarH!=SPO_VARH_MID) return-4 ;
					rc = MBuildSSDU_hs(TZ.Hs, SP.T, SP.i, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					break ;
				// по суточному дрейфу
				case SPO_VARIDPAR_dL:
					rc = MBuildSSDU_dL(TZ.dL, TZ.N, SP.T, SP.i, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					break ;
				// по наклонению
				case SPO_VARIDPAR_i:
					rc = MBuildSSDU_i(TZ.i, SP.T, SP.p, SP.e1, SP.e2, Msg) ; RRC ;
					SP.i = TZ.i ;
					break ;
				default: return-20 ;
			}
			break ;
		default: return-21 ;
	}

    SP.e = sqrt( SP.e1*SP.e1 + SP.e2*SP.e2 ) ;
    SP.a = SP.p / ( 1.0 - SP.e*SP.e ) ;
    if ( SP.e > 1e-14 ) SP.w = AcosSS( SP.e1 / SP.e, SP.e2 ) ;
    else SP.w = 0 ;
    RRC ;

    // Вектор координат в ГСК
    SP.Q[0] = SP.p/(1+SP.e1)*Cos(TZ.L) ;
    SP.Q[1] = SP.p/(1+SP.e1)*Sin(TZ.L) ;
    SP.Q[2] = 0. ;
    double Vk = sqrt(k_mu_kms/SP.p) ;			// k_mu - км**3/ cек**2 
	double k_omega_rs = k_omega*86400.0 ;		// Угловая скорость вращения Земли - радиан в секунду  
    SP.Q[3] =-Vk*(SP.e2*Cos(TZ.L)+(1.0+SP.e1)*Sin(TZ.L)*Cos(SP.i))+k_omega_rs*SP.Q[1] ;
    SP.Q[4] =-Vk*(SP.e2*Sin(TZ.L)-(1.0-SP.e1)*Cos(TZ.L)*Cos(SP.i))-k_omega_rs*SP.Q[0] ;
    SP.Q[5] = Vk*(1.0+SP.e1)*Sin(SP.i) ;

    return 0 ;
}

//---------------------------------------------------------------------------
