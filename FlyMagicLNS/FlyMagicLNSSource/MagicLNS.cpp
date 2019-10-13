//-------------------------------------------------------------------------------
// MagicLNS.cpp: 
// А. Николенко 16.08.2018
//-------------------------------------------------------------------------------
// Модуль расчёта управлений
// Размерности:
//   Координаты - км
//   Скорости   - км/сек
//   Углы       - градусы
//------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции
#include <FlyCore.h>		// Математические функции
#include <FlyMagicLNS.h>	//

//---------------------------------------------------------------------------
ZSMagicLnsSolution* CreateMagicLnsSolution()
{
	ZSMagicLnsSolution* MS = new ZSMagicLnsSolution;
	return MS;
}

//---------------------------------------------------------------------------
void FreeMagicLnsSolution(ZSMagicLnsSolution* MS)
{
	if (MS) delete MS ;
}

//---------------------------------------------------------------------------
double  k_epsdrs= 0.01 ;	// 10 метров
double  k_stepu = 0.5 ;		// [град] шаг перебора аргумента широты
int     pru = 0 ;			// признак учёта времени работы КДУ
int     kp  = 0 ;			// признак корректируемого параметра
double  dtQ1, dtQ2 ;		// для вектора промаха
double  PV1[3], PV2[3] ;	// для изменений а,е,w  от V1,V2

int MagicCalcDpDV(double a0, double e0, double i0, double w0, double u, int dN, double dV, ZSLnsNpmDiffDV& DP) ;

//------------------------------------------------------------------------------
// P1 (Pv1)	- Параметры исходной орбиты на витке N1 (виток первого импульса)
// P2		- Параметры исходной орбиты на витке N2 (виток второго импульса)
// PZ (PN)	- Параметры исходной орбиты на заданном витке
// PTN		- Требуемые элементы орбиты (задание)
// PTT		- Текущие требуемые элементы орбиты (с учётом поправок от промаха)
// U		- Искомое управление
//------------------------------------------------------------------------------
// Если расчитывается коррекция для орбиты ОМИВ или ДУ, то значения
// e и w определяются с помощью функции MknBuildSpo
//------------------------------------------------------------------------------
int LNSMagicBox(
	ZSMagicLnsSolution* MS,	// Описание решаемой задачи 	
	ZSLoopFact    &P1,		// Параметры движения на витке первого импульса	
	ZSLoopFact    &P2,		// Параметры движения на витке второго импульса	
	ZSLoopFact    &PZ,		// Параметры движения на заданном витке
	ZCLnsNpmClaim &PT,		// Требуемые параметры движения
	ZSFlightRule  &U)		// Искомое управление - структура прог. управления
{
    int rc = 0 ;

	// Если заданы аргументы широты приложения импульсов
	if (MS->GlobCount==1 && U.isFixedArg) {
	    if (U.u0[0] &&!U.u0[1]) U.plane=1 ; // задан u1
		if (U.u0[1] &&!U.u0[0]) U.plane=2 ; // задан u2
		if (U.u0[0] && U.u0[1]) U.plane=3 ; // заданы u1,u2
		if (U.u0[0] && U.u0[1] && U.u0[2]) U.plane=4; // заданы u1,u2,u3
	}
	// Проверка суточного смещения и коррекция суточного числа витков
	if (MS->GlobCount == 1 && PT.VarPar == SPO_VARIDPAR_dL) {
		if (MS->GlobCount == 1) {
			double  dLmv1 =(360.-PT.dL)/(PZ.Ns -1) ;
			double  dLmv2 =(360.-PT.dL)/(PZ.Ns +1) ;
			if (fabs(PT.dL)>0.5*dLmv1 || fabs(PT.dL)>0.5*dLmv2) {
				//MMessageBox("Требуемое суточное смещение > 1/2 межвиткового интервала",2,0) ; 
				return 1 ;
			}
			if (PZ.dLs<0 && PT.dL>0 && (PZ.dLs*PT.dL)<0){
				PZ.Ns-=1;PT.N-=1;
				//MMessageBox( "Суточное число витков уменьшено на 1" , 2, 0) ;
			}
			if (PZ.dLs>0 && PT.dL<0){
				PZ.Ns+=1;PT.N+=1;
				//MMessageBox( "Суточное число витков увеличено на 1", 2, 0) ;
	}	}	}

	// Только для коррекций (h,Bh+...)
	if (PT.VarOrb==SPO_ORB_HBH) {
		if (MS->GlobCount == 1) {
#ifdef ESTIMATE_BH_AS_UH
			// Признак формирования прицельного значения аргумента широты
			// минимальной высоты и как следствие со второй итерации
			// переход на сходимость не по значению Bh а по значению uh
			PT.uH =-999.0 ;
#endif
	}	}

	// Расчёт управлений
	switch ( PT.VarKor ) {
		// Расчёт параметров управления для 1-импульсной коррекции
		case 10:  rc = MagicCalcU10 (MS, P1, PZ, PT, U) ; break ;
		case 11:  rc = MagicCalcU11 (MS, P1, PZ, PT, U); break;
		case 12:  rc = MagicCalcU12 (MS, P1, PZ, PT, U); break;
		case 13:  rc = MagicCalcU13 (MS, P1, PZ, PT, U); break;
		case 14:  rc = MagicCalcU14 (MS, P1, PZ, PT, U); break;
		case 15:  rc = MagicCalcU15 (MS, P1, PZ, PT, U); break;
		case 16:  rc = MagicCalcU16 (MS, P1, PZ, PT, U); break;
		case 17:  rc = MagicCalcU17 (MS, P1, PZ, PT, U); break;
		case 18:  rc = MagicCalcU18 (MS, P1, PZ, PT, U); break;

		// Расчёт параметров управления для 2-импульсной коррекции
		case 20:  rc = MagicCalcU20 (MS, P1, P2, PZ, PT, U); break;
		case 21:  rc = MagicCalcU21 (MS, P1, P2, PZ, PT, U); break;
		case 22:  rc = MagicCalcU22 (MS, P1, P2, PZ, PT, U); break;
		case 23:  rc = MagicCalcU23 (MS, P1, P2, PZ, PT, U); break;
		case 24:  rc = MagicCalcU24 (MS, P1, P2, PZ, PT, U); break;
		case 241: rc = MagicCalcU241(MS, P1, P2, PZ, PT, U); break;
		case 25:  rc = MagicCalcU25 (MS, P1, P2, PZ, PT, U); break;
		case 26:  rc = MagicCalcU26 (MS, P1, P2, PZ, PT, U); break;
		case 27:  rc = MagicCalcU27 (MS, P1, P2, PZ, PT, U); break;
		case 28:  rc = MagicCalcU28 (MS, P1, P2, PZ, PT, U); break;
		case 29:  rc = MagicCalcU29 (MS, P1, P2, PZ, PT, U); break;

		// Расчёт параметров управления для 3-импульсной коррекции
		case 30:  rc = MagicCalcU30 (MS, P1, PZ, PT, U); break;
		case 31:  rc = MagicCalcU31 (MS, P1, PZ, PT, U); break;
		case 311: rc = MagicCalcU311(MS, P1, PZ, PT, U); break;
		case 32:  rc = MagicCalcU32 (MS, P1, PZ, PT, U); break;
		case 33:  rc = MagicCalcU33 (MS, P1, PZ, PT, U); break;

		default:	return-100 ;
	}

	// Определение длительности работы ДУ с учётом импульса последействия 
	if (!rc) { 
		double M = MS->MZ.M();
		for (int i=0 ; i<U.n ; i++ ) { 
			double dVS = fabs(U.dV[i]) ;
			rc = ZFPUCalcTimeMain(2, M, MS->DU, NULL, &dVS, &U.dt[i], &U.dts[i], NULL, &M, U.tm[i], NULL); RRC;
	}	}
	return rc ;
}

//------------------------------------------------------------------------------
void DefaultParam(ZSLnsNpmPar& P) {
  P.maxIter = 50;
  P.epsT    = 0.001;   // сек
  P.epsL    = 0.001;   // град
  P.epsdL   = 0.0001;  // град / сут
  P.epsu    = 0.001;   // град
  P.epsH    = 0.0001;  // км
  P.epsBH   = 0.01;    // град
  P.epsa    = 0.00001; // км
  P.epse    = 0.0001;  //
  P.epsw    = 0.1;     // град
  P.isEstimateOrb = 0;
  P.isEstimateOrb_e12 = 1;
  P.isEstimateOrb_e   = 1;
  P.isEstimateOrb_w   = 1;
  P.isEstimateOrb_h   = 1;
  P.isEstimateOrb_Bh  = 1;
  //
  P.Epoha     = 2 ;
  //
  P.ViewDimT  = 0 ;
  P.ViewDimL  = 1 ;
  P.ViewDimdL = 1 ;
  P.ViewDimw  = 1 ;
  P.ViewDimBh = 1 ;
  P.ViewDimu  = 1 ;
  P.ViewDimdV = 0 ;
  P.ViewDimdt = 0 ;
  //
  P.RezSaveNU          = 2 ;
  P.RezSaveNUNom       = 1 ;
  P.RezSaveNUMod       = 0 ;
  P.RezSavePULast      = 1 ;
  P.RezUseLHS          = 0 ;
  P.RezSaveFOFrom      = 1 ;
  P.RezSaveFODbl       = 0 ;
  P.RezSaveDocFull     = 1 ;
  P.RezSaveDocDsk      = 1 ;
  P.RezSaveDocDU       = 0 ;
  P.RezSaveDocLHS      = 1 ;
  P.RezSaveDocLHSShort = 0 ;
  P.RezSaveDocINU      = 1 ;
  P.RezSaveDocINUShort = 0 ;
  P.RezSaveDocNUR      = 1 ;
  P.RezSaveDocNURShort = 0 ;
  P.RezSaveDocRV       = 1 ;
  P.RezSaveDocPU       = 1 ;
  P.RezSaveDocFO       = 1 ;
  P.RezSaveDocSVER     = 1 ;
  //
  P.RezUseLHSNum       = 0 ;
} ;

//------------------------------------------------------------------------------
void operator << (ZSLoopFact& P, ZSC_VI& FV) 
{
	ZSC_VP* Hmin = HMIN_VI(FV) ;
	ZSC_VP* Hmax = HMAX_VI(FV) ;

	P.Vit   = FV.FSUN.VitTE ;
	P.t     = FV.FSUN.ta ;
	P.Dt    = FV.FSUN.Data ;
	P.Tt    = FV.FSUN.Time ;
	P.T		= FV.Td ;			// Период драконический [сек]
	P.Tss	= FV.Td ;			// Период драконический среднесуточный [сек]
	P.dTv	= 0  ;				// Падение драконического периода за виток [сек]
	P.Hs	= FV.HAvr ;         // Средняя высота орбиты на витке [км]
	P.dLs	= FV.dLs ;			// Суточное смещение трассы полёта [град]
	P.dLv	= FV.dL ;			// Межвитковое расстояние [град]
	P.L		= FV.UN.L ;			// Долгота начала витка [град]
	P.Ns	= FV.Ks ;			// кратность орбиты
	P.Nvs	= 1 ;				// Номер суточного витка
	P.hmin	= Hmin->H ;			// Минимальная высота [км]
	P.Bhmin = Hmin->B ;			// Широта минимальной высоты [град]
	P.Uhmin = Hmin->ute ;		// Аргумент широты минимальной высоты [град]
	P.hmax  = Hmax->H ;			// Минимальная высота [км]
	P.Bhmax = Hmax->B ;			// Широта минимальной высоты [град]
	P.Uhmax = Hmax->ute ;		// Аргумент широты максимальной высоты [град]
	P.a		= TO_KM(FV.FSUN.OSKTE[0]) ;		// большая полуось на момент начала витка [км]
	P.e		= FV.FSUN.OSKTE[1] ;			// Эксцентриситет на момент начала витка
	P.w		= GradA(FV.FSUN.OSKTE[4]) ;		// Положение перигея на момент начала витка [град]
	P.e1	= P.e*cos(FV.FSUN.OSKTE[4]) ;	//
	P.e2	= P.e*sin(FV.FSUN.OSKTE[4]) ;	//
	P.i		= GradA(FV.FSUN.OSKTE[2]) ;		// наклонение на момент начала витка
	P.u		= 0 ;

	// радиусы в узловых точках
    MagicCalcR(P.i, P.a*(1-P.e*P.e), P.e1, P.e2, P.Ro) ; 
}

void operator << (ZSLoopFact& P, ZSC_VI* FV) { P << *FV ; }

//------------------------------------------------------------------------------
