//---------------------------------------------------------------------------
//  ModAstro.cpp
//  Астрономический ежегодник
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
//---------------------------------------------------------------------------
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ZNUT.h>
#include <FlyCoreSource\\ZPR.h>

//---------------------------------------------------------------------------
const double tgkqk=81.300561; //Соотношение масс Земли Луны

//---------------------------------------------------------------------------
// Переопределение варианта ежегодника
#define   DEF_Num_AstroData   1
//---------------------------------------------------------------------------
//
//-----------------------------------------------------------------------
typedef struct STInitRazm
{
  int Epoc;     // Эпохи :     0 - 2000 г . 1 - 1975 г.
  int RazmD;    // размерность 0 - дткм 1 - км      2 - м
  int RazmU;    // размерность 0 - рад  1 - град    2 - час
  int TimeSh;   // Время       0 - Всемирное 1 - Московское
} _STInitRazm;

_STInitRazm InitRazm;
//-----------------------------------------------------------------------
CModAstro::CModAstro(void)
{
    InitRazm.Epoc   = Epoc;
    InitRazm.RazmD  = RazmD;
    InitRazm.RazmU  = RazmU;
    InitRazm.TimeSh = TimeSh ;
    EpsilonNoCalc   = 1.e-10 ;
    ModeBTS = BTS_EARTH ;//ЗБЦ
    return;
}
//-----------------------------------------------------------------------
CModAstro::CModAstro(const int mode)
{     
    if (mode == BTS_SS) Epoc  = 0;  // БЦ СС
    if (mode == BTS_EARTH) Epoc  = 1; //
    //Epoc  = 1;// эпоха 1975
    RazmD = 0;// дткм
    RazmU = 0;// рад
    TimeSh= 0;// Всемирное время

    InitRazm.Epoc   = Epoc;
    InitRazm.RazmD  = RazmD;
    InitRazm.RazmU  = RazmU;
    InitRazm.TimeSh = TimeSh;
    EpsilonNoCalc   = 1.e-10;
    ModeBTS = mode;
    return;
}
//-----------------------------------------------------------------------
CModAstro::~CModAstro(void)
{
}

//-----------------------------------------------------------------------
void SET_PARAM_AE(int npl, int &beg, int &range, int &quant, int &nv);

//-----------------------------------------------------------------------
// Функция расчета и получения астроданных заданного типа на основании полиномов
//-----------------------------------------------------------------------
HRESULT CModAstro::Calc_Precesion(double dTA, AstroObject NumTypeData, double *dResult)
{
    //Проверка на совпадение с прошлым значением
	if(fabs(LastCalcMass[NumTypeData].dTA-dTA)<EpsilonNoCalc) {
		dResult[0] = LastCalcMass[NumTypeData].Params[0];
		dResult[1] = LastCalcMass[NumTypeData].Params[1];
		dResult[2] = LastCalcMass[NumTypeData].Params[2];
		return 0 ;
    }

	if(NumTypeData==Sun) {
		ANAL_AE(dTA, 0);  for(int it=0; it<3; it++) dResult[it]=Wae->qso[it];
	}
	if(NumTypeData==Moon) {
		ANAL_AE(dTA, 1);  for(int it=0; it<3; it++) dResult[it]=Wae->qlu[it];
	}
	if(NumTypeData==Jove) {
		ANAL_AE(dTA, 2);  for(int it=0; it<3; it++) dResult[it]=Wae->qup[it];
	}
	if(NumTypeData==Saturn) {
		ANAL_AE(dTA, 3);  for(int it=0; it<3; it++) dResult[it]=Wae->qsa[it];
	}
	if(NumTypeData==Uranus) {
		ANAL_AE(dTA, 4);  for(int it=0; it<3; it++) dResult[it]=Wae->qur[it];
	}
	if(NumTypeData==Venus) {
		ANAL_AE(dTA, 5);  for(int it=0; it<3; it++) dResult[it]=Wae->qwe[it];
	}
	if(NumTypeData==Mars) {
		ANAL_AE(dTA, 6);  for(int it=0; it<3; it++) dResult[it]=Wae->qma[it];
	}
	if(NumTypeData==Neptune) {
		ANAL_AE(dTA, 7);  for(int it=0; it<3; it++) dResult[it]=Wae->qne[it];
	}
	if(NumTypeData==Mercury) {
		ANAL_AE(dTA, 8);  for(int it=0; it<3; it++) dResult[it]=Wae->qme[it];
	}
	if(NumTypeData==Pluto) {
		ANAL_AE(dTA, 9);  for(int it=0; it<3; it++) dResult[it]=Wae->qpl[it];
	}
	if(NumTypeData==Nutation) {
		ANAL_AE(dTA, 12); for(int it=0; it<3; it++) dResult[it]=Wae->qnu[it];
	}

	//AstroObject { Moon = 0, Sun = 1, Mercury = 2, Venus = 3, Mars = 4,
	//		   Jove = 5, Saturn = 6, Uranus = 7, Neptune = 8, Pluto = 9,
	//		   Nutation = 10, Libration = 11, STime = 12, Precess = 13}; -- БЫЛО!
	//		Nutation = 10, Libration = 11, STime = 13, Precess = 14,
	//		Earth = 12};
	// режим БЦСС и планета земля

	//if (NumTypeData == Earth)&&(ModeBTS = BTS_SS)

	if(NumTypeData==Earth && ModeBTS==BTS_SS) {
		//ANAL_AE(dTA, 12);  for(int it=0; it<3; it++) dResult[it]=Wae->qnu[it];
	}

	LastCalcMass[NumTypeData].Params[0] = dResult[0];
	LastCalcMass[NumTypeData].Params[1] = dResult[1];
	LastCalcMass[NumTypeData].Params[2] = dResult[2];
	LastCalcMass[NumTypeData].dTA = dTA;

	return S_OK;
}

//-----------------------------------------------------------------------
void CModAstro::AstroRazmSet(int Epockha, int RazmerD, int TimeSys)
{
	Epoc   = Epockha ;
	RazmD  = RazmerD ;
	TimeSh = TimeSys ;
}
//-----------------------------------------------------------------------
void CModAstro::AstroRazmGet(int *pEpockha, int *pRazmerD, int *pTimeSys)
{
	if ( pEpockha ) *pEpockha = Epoc ;
	if ( pRazmerD ) *pRazmerD = RazmD ;
	if ( pTimeSys ) *pTimeSys = TimeSh ;
}
//-----------------------------------------------------------------------
void CModAstro::AstroRazmSet(int Epockha, int RazmerD, int RazmerU, int TimeSys)
{
	CModAstro::AstroRazmSet( Epockha, RazmerD, TimeSys ) ;
	RazmU = RazmerU ;
}
//-----------------------------------------------------------------------
void CModAstro::AstroRazmGet(int *pEpockha, int *pRazmerD, int *pRazmerU, int *pTimeSys)
{
	CModAstro::AstroRazmGet( pEpockha, pRazmerD, pTimeSys ) ;
	if( pRazmerU ) *pRazmerU = RazmU ;
}
//-----------------------------------------------------------------------
void __fastcall CModAstro::AsOutMode(double * massiv)
{
  double    mp00_75 [9];
  double    rearth[3];
  short int j;

  if (Epoc == 1) { // Перевод координат в эпоху 1975.0
    // матрица перехода от фунд. эпохи 2000 к тек. эпохе 1975
    mp00_75[0] =  0.999981423814942;
    mp00_75[1] =  0.00559013887986178;
    mp00_75[2] =  0.00242947985080896;
    mp00_75[3] = -0.00559013888057073;
    mp00_75[4] =  0.999984375028524;
    mp00_75[5] = -6.79033614644759e-06;
    mp00_75[6] = -0.00242947984917768;
    mp00_75[7] = -6.79091976566359e-06;
    mp00_75[8] =  0.9999997048786418;
    UMN (mp00_75, massiv, rearth, 3, 3, 1);
    for (j=0; j<3; j++) massiv[j] = rearth[j];
  }
  if (RazmD == 0) { // Перевод в дткм
    for (j=0; j<3; j++)  massiv[j] *= 0.0001;// /= 10000.0;
  }
  if (RazmD == 2) { // Перевод в м
    for (j=0; j<3; j++)  massiv[j] *= 1000.0;
  }
}

double Get_Delta_TDT();

//-----------------------------------------------------------------------
// Функция расчета и получения астроданных заданного типа на основании полиномов
//-----------------------------------------------------------------------
HRESULT CModAstro::Calc_AstroData(const double dTAc, AstroObject NumTypeData, long NumSysCoord, double *dResult)
{
    //Проверка на совпадение с прошлым значением
	if(fabs(LastCalcMass[NumTypeData].dTA-dTAc)<EpsilonNoCalc) {
		dResult[0]=LastCalcMass[NumTypeData].Params[0];
		dResult[1]=LastCalcMass[NumTypeData].Params[1];
		dResult[2]=LastCalcMass[NumTypeData].Params[2];
		return 0 ;
    }
    //Перевод в число суток от 01.01.2000
    //Компенсация Московского Декретного времени
    //double dTA = dTAc - DTPCor1975 - 0.125 * TimeSh + Get_Delta_TDT( ) ;
	if ( NumTypeData == Sun ) {
		ANAL_AE( dTAc, 0 ) ; memcpy( dResult, Wae->qso, 3*sizeof( double ) ) ; 
    }
	if ( NumTypeData == Moon ) {
        ANAL_AE( dTAc, 1 ) ; memcpy( dResult, Wae->qlu, 3*sizeof( double ) ) ; 
    }
    if ( NumTypeData == Jove ){
        ANAL_AE( dTAc, 2 ) ; memcpy( dResult, Wae->qup, 3*sizeof( double ) ) ;  
    }
    if ( NumTypeData == Saturn ) {
        ANAL_AE( dTAc, 3 ) ; memcpy( dResult, Wae->qsa, 3*sizeof( double ) ) ; 
    }
    if ( NumTypeData == Uranus ) {
        ANAL_AE( dTAc, 4 ) ; memcpy( dResult, Wae->qur, 3*sizeof( double ) ) ; 
    }
    if ( NumTypeData == Venus ) {
        ANAL_AE( dTAc, 5 ) ; memcpy( dResult, Wae->qwe, 3*sizeof( double ) ) ; 
    }
    if ( NumTypeData == Mars ) {
        ANAL_AE( dTAc, 6 ) ; memcpy( dResult, Wae->qma, 3*sizeof( double ) ) ; 
    }
    if ( NumTypeData == Neptune ) {
        ANAL_AE( dTAc, 7 ) ; memcpy( dResult, Wae->qne, 3*sizeof( double ) ) ; 
    }
    if ( NumTypeData == Mercury ) {
        ANAL_AE( dTAc, 8 ) ; memcpy( dResult, Wae->qme, 3*sizeof( double ) ) ; 
    }
    if ( NumTypeData == Pluto ) {
        ANAL_AE( dTAc, 9 ) ; memcpy( dResult, Wae->qpl, 3*sizeof( double ) ) ; 
    }
    if ( NumTypeData == Nutation ) {
        ANAL_AE( dTAc, 12 ) ; memcpy( dResult, Wae->qnu, 3*sizeof( double ) ) ; 
    }

	LastCalcMass[NumTypeData].Params[0]=dResult[0];
	LastCalcMass[NumTypeData].Params[1]=dResult[1];
    LastCalcMass[NumTypeData].Params[2]=dResult[2];
    LastCalcMass[NumTypeData].dTA = dTAc;
    return 0 ;
}

//-----------------------------------------------------------------------
HRESULT CModAstro::Calc_AstroData_Startime(double dTAc, double *dResult)
{
	HRESULT hCodRet;
	// проверка на совпадение с прошлым значением
	if ( fabs( LastCalcMass[ STime ].dTA - dTAc ) < EpsilonNoCalc ) {
	    dResult[ 0 ] = LastCalcMass[STime].Params[ 0 ] ;
		dResult[ 1 ] = LastCalcMass[STime].Params[ 1 ] ;
		dResult[ 2 ] = LastCalcMass[STime].Params[ 2 ] ;
		return 0 ;
	}
	hCodRet = Calc_StarTime(dTAc, dResult); if (hCodRet) return hCodRet;
	// сохранение вновь рассчитанных параметров
	LastCalcMass[STime].Params[0] = dResult[0];
	LastCalcMass[STime].Params[1] = dResult[1];
	LastCalcMass[STime].Params[2] = dResult[2];
	LastCalcMass[STime].dTA = dTAc;
	return 0 ;
}
//-----------------------------------------------------------------------
// Прецессия
//-----------------------------------------------------------------------
//__declspec(dllexport) void Calc_Prec_Par_B1975(double Tae, double *Preces);
//-----------------------------------------------------------------------
HRESULT CModAstro::Calc_AstroData_precess(double dTAc, double *dResult)
{
    int i;
    // проверка на совпадение с прошлым значением
    if (fabs(LastCalcMass[Precess].dTA - dTAc) < EpsilonNoCalc)  {
        dResult[0] = LastCalcMass[Precess].Params[0];
        dResult[1] = LastCalcMass[Precess].Params[1];
        dResult[2] = LastCalcMass[Precess].Params[2];
        return 0 ;
    }
	ZPrecessJ2000(dTAc, dResult, TIME1975ASD) ;
    //Calc_Prec_Par_B1975(dTAc, dResult);

    if ( !RazmU ) {
        // Перевод в рад
		for (i = 3; --i >= 0; dResult[i] /= M_SEKRAD);
    } else {
        if ( RazmU == 1 ) {
            // Перевод в град
            for(i=3;--i>=0;dResult[i]/=3600.);
        } else {
            // Перевод в часы
            for(i=3;--i>=0;dResult[i]/=3600.*15.);
        }
    }
    // сохранение вновь рассчитанных параметров
    LastCalcMass[Precess].Params[0] = dResult[0];
    LastCalcMass[Precess].Params[1] = dResult[1];
    LastCalcMass[Precess].Params[2] = dResult[2];
    LastCalcMass[Precess].dTA = dTAc;
    return 0 ;
}

//-----------------------------------------------------------------------
