//-------------------------------------------------------------------------------
// MagicCalcMishit.cpp: 
// А. Николенко 23.08.2018
// Расчёт промаха. Используется в методе узловых точек
//-------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции
#include <FlyCore.h>		// Математические функции
#include <FlyMagicLNS.h>	//

//------------------------------------------------------------------------------
// Функция вычисления промаха
int MagicCalcMishit(double i, double p, double e1, double e2, ZSLoopFact& P1, ZSLoopFact& PZ, double* Rt, ZSLnsNpmFault& P)
{
    int rc = 0 ;
    double* Ri = &PZ.Ro[0] ;
    // Вычисление радиусов в узловых точках требуемой орбиты
    rc = MagicCalcR(i, p, e1, e2, Rt) ; RRC ;
    double R1t = Rt[0] ;
    double R2t = Rt[1] ;
    double Rst = Rt[2] ;
    double R1i = Ri[0] ;
    double R2i = Ri[1] ;
    double Rsi = Ri[2] ;
    P.dR1 = R1t-R1i ;
    P.dR2 = R2t-R2i ;
    P.dRs = Rst-Rsi ;
    P.dR0 = sqrt(P.dR1*P.dR1+P.dR2*P.dR2) ;
    P.de1 = e1-PZ.e1 ;
    P.de2 = e2-PZ.e2 ;
    double ro = 0.5*(Rst+Rsi) ;
    P.l   = sqrt(k_mu_kms/ro)/ro ;
    // u0 без уточнения
    P.u0  = AtanSC(P.dR2/P.dR1, P.dRs*P.dR1) ;
    // уточнение u0
    double deu = 360.0 ;
    double k, sn ;
    double uj = P.u0 ;
    while (fabs(deu)>0.005) {
		k    = 1+P1.e1*cos(RadA(P.u0))+P1.e2*sin(RadA(P.u0));
		sn   = k*P.de1*Sign1(P.dRs)*Rsi - P1.e1*0.5*P.dRs ;
		P.u0 = AtanSC((k*P.de2*Sign1(P.dRs)*Rsi-P1.e2*0.5*P.dRs)/sn, sn);
		deu  = P.u0-uj ;
		uj   = P.u0 ;
    }
	// Учёт прецессии линии апсид
	//if ( PT.VarOrb==SPO_ORB_OMIV || PT.VarOrb==SPO_ORB_EL ){
	double p2i = pow(P1.a*(1.0-P1.e*P1.e), 2) ;		// квадрат фокального параметра
	double du  =(PZ.Vit-P1.Vit-P.u0/360.0)*180.0/p2i*k_eps/k_mu_kms*(5.0*Cos(P1.i)*Cos(P1.i)-1.0);
	P.u0 -= du ;
	//}
	return rc ;
}

//------------------------------------------------------------------------------
// Расчёт промаха по u для Кондора
int MagicCalcMishitUKondor(double uf, double tf, double tN, double Tpr, double& du)
{
    double wpr = 360.0/Tpr ;
    double dN  = (long)((tN-tf)/Tpr*86400.0 ) ;
    du = uf+(tN-tf-dN*Tpr/86400.0)*wpr*86400.0 ;
    // Приведение рассчитанного удаления к интервалу +-180
    du = ValiDA(du, 1) ;
    return 0 ;
}

//------------------------------------------------------------------------------

