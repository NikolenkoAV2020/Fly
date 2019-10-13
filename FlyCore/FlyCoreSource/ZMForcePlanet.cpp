//-------------------------------------------------------------------------------
// ZMForcePlanet.cpp
// А. Николенко 03.08.2018
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>

#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ZMForcePlanet.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ZSK.h>
#include <FlyCoreSource\\ZSun.h>

//-------------------------------------------------------------------------------
int _CalcAstroKoord( double t, int NP ) 
{
	int rc = 0 ;
	if (NP==0) {
		// Солнце
		double Q[3] ;
		rc = ZSunJ2000(t, Q, TIME1975ASD) ; 
		if (!rc) {
			// Перевод координат Солнца из АСК 2000 в АСК 1975 года
			rc = ZASC_2000to1975(Q, Wae->qso, 0) ;
		}
	} else {
		rc = AEJG( t, NP ) ;
	}
	if ( rc ) {
		switch( NP ) {
		case 0 : AddErrorMSG( "Ошибка расчёта координат Солнца" ) ; break ;
		case 1 : AddErrorMSG( "Ошибка расчёта координат Луны" ) ; break ;
		case 2 : AddErrorMSG( "Ошибка расчёта координат Юпитера" ) ; break ;
		case 3 : AddErrorMSG( "Ошибка расчёта координат Сатурна" ) ; break ;
		case 4 : AddErrorMSG( "Ошибка расчёта координат Урана" ) ; break ;
		case 5 : AddErrorMSG( "Ошибка расчёта координат Венеры" ) ; break ;
		case 6 : AddErrorMSG( "Ошибка расчёта координат Марса" ) ; break ;
		case 7 : AddErrorMSG( "Ошибка расчёта координат Нептуна" ) ; break ;
		case 8 : AddErrorMSG( "Ошибка расчёта координат Меркурия" ) ; break ;
		case 9 : AddErrorMSG( "Ошибка расчёта координат Плутона" ) ; break ;
		}
	}
	return rc ;
}

//---------------------------------------------------------------------------
// Гравитационное влияние Луны, Солнца, планет
int ZMAttractionPlanet( double t, double* RAsk, ZLSF& LSF, double* UAsk )
{
	int    i, rc = 0 ; 
	double A[3] ;

	memset( UAsk, 0, 3*sizeof(double)) ;
	for ( int jp=0 ; jp<10 ; jp++ ) {
		double  MR, MRT ;
		double  DR[ 3 ] ;
		double  GP = 0 ;
		double* RT = 0 ;
		int     NP = jp ;
		if ( jp > 1 && !LSF.isPlanet[ jp - 2 ] ) continue ;
		switch( jp ) {
			case 0 :
				if ( !LSF.isSun ) continue ;
				RT = Wae->qso ; GP = k_gs ;	break ;
			case 1 :
				if ( !LSF.isMoon ) continue ;
				RT = Wae->qlu ; GP = k_gl ;	break ;
			case 2 : RT = Wae->qup ; GP = k_g_jupiter ;	break ;
			case 3 : RT = Wae->qsa ; GP = k_g_saturn ; break ;
			case 4 : RT = Wae->qur ; GP = k_g_uran ;	break ;
			case 5 : RT = Wae->qwe ; GP = k_g_venera ; break ;
			case 6 : RT = Wae->qma ; GP = k_g_mars ;	break ;
			case 7 : RT = Wae->qne ; GP = k_g_neptun ; break ;
			case 8 : RT = Wae->qme ; GP = k_g_merkurj ; break ;
			case 9 : RT = Wae->qpl ; GP = k_g_pluton ; break ;
			default : continue ;
		}

		rc = _CalcAstroKoord( t, NP ) ; RRC ;

		for( i=0 ; i<3 ; i++ ) DR[i] = RT[i]-RAsk[i] ;
		MR  = FR( DR ) ;
		MRT = FR( RT ) ;

		for( i=0 ; i<3 ; i++ ) {
			A[i] = GP*(DR[ i ]/MR/MR/MR-RT[i]/MRT/MRT/MRT) ;
			UAsk[i]+= A[i] ;
		}
		
        if ( NP == 1 ) {
			// Косвенное влияние сжатия Земли через притяжение Луны
			double c2k = pow(RT[2] / MRT, 2) ; 
			double a = 1.5*k_c20*k_gl*k_a0*k_a0 / pow(MRT, 5) ;
			double b = a*(1.0-5.0*c2k) ;
			for ( i=0 ; i<3 ; i++ ) A[i] = b*RT[i] ;
			A[2]+= 2*a*RT[2] ;
			for( i=0 ; i<3 ; i++ ) UAsk[i]+= A[i] ;
        }

        //  Учёт приливных эффектов
		
        if( LSF.vTide!=TIDENULL && NP < 2 ) {
			double r, P, csH, F1, F2, cz, sz, x[3] ;
			cz = cos(k_ql);
			sz = sin(k_ql);
			x[0] = RT[0]*cz-RT[1]*sz;  
			x[1] = RT[0]*sz+RT[1]*cz ;  
			x[2] = RT[2] ;               

			r   = FR(RAsk) ;
			P   = k_qk2*GP*pow(k_a0, 5) / (pow(MRT, 3)*pow(r, 4)) ;
			csH = scal_pr(RAsk, x, 3) / (r*MRT) ;
			F1  = 3*csH / MRT ;
			F2  = (7.5*csH*csH-1.5) / r ;
			for ( i=0 ; i<3 ; i++ ) A[i]   = P*(F1*x[i]-F2*RAsk[i]) ;
			for ( i=0 ; i<3 ; i++ ) UAsk[i]+= A[i] ;
        }
	}
	return 0 ;
}

//---------------------------------------------------------------------------