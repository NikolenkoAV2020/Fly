//-----------------------------------------------------------------------
// ModGlobal.cpp
// А. Николенко 03.08.2018
//-----------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop

//-----------------------------------------------------------------------
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
//------------------------------------------------------------------------------
//FLYCORE_API char DIRFLY[512]        = ".\0";
//FLYCORE_API char DIRFLYINFO[512]    = ".\\ModInfo\0";
//FLYCORE_API char DIREGF[512]        = ".\\ModInfo\\EGF\0";
//FLYCORE_API char DIRIERSEOP[512]    = ".\\ModInfo\\EopIers\0";
//FLYCORE_API char DIREOPSTORAGE[512] = ".\\ModInfo\\EopStorage\0";
//FLYCORE_API char DIRRESULT[512]     = ".\\_Результаты\0";

FLYCORE_API char DIRFLY[512]        = "";
FLYCORE_API char DIRFLYINFO[512]    = "";
FLYCORE_API char DIREGF[512]        = "";
FLYCORE_API char DIRIERSEOP[512]    = "";
FLYCORE_API char DIREOPSTORAGE[512] = "";
FLYCORE_API char DIRRESULT[512]     = "";
FLYCORE_API char DIRUOF[512]        = "";

int FlyGlobalInit()
{
	char Buf[1024];
	int  rc;

	if (!strlen(DIRFLY)) {
		rc = GetEnvironmentVariable("FLY_PATH", Buf, sizeof(Buf));
		if (!rc) sprintf(DIRFLY, ".\0");
		else	 sprintf(DIRFLY, "%s\0", Buf);
	}

	if (!strlen(DIRRESULT)) {
		//rc = ExpandEnvironmentStrings("%FLY_RESULT1%\\ЗРВ", Buf, sizeof(Buf));
		rc = GetEnvironmentVariable("FLY_RESULT", Buf, sizeof(Buf));
		if (!rc) sprintf(DIRRESULT, ".\\_Результаты\0");
		else	 sprintf(DIRRESULT, "%s\0", Buf);
	}

	if (!strlen(DIRUOF)) {
		rc = GetEnvironmentVariable("FLY_UOF", Buf, sizeof(Buf));
		if (!rc) sprintf(DIRUOF, ".\\FlyUof\0");
		else	 sprintf(DIRUOF, "%s\0", Buf);
	}

	if (!strlen(DIRFLYINFO)) {
		rc = GetEnvironmentVariable("FLY_MODINFO", Buf, sizeof(Buf));
		if (!rc) sprintf(DIRFLYINFO, ".\\FlyModInfo\0");
		else	 sprintf(DIRFLYINFO, "%s\0", Buf);
		sprintf(DIREGF,        "%s\\EGF\0", DIRFLYINFO);
		sprintf(DIRIERSEOP,    "%s\\EopIers\0", DIRFLYINFO);
		sprintf(DIREOPSTORAGE, "%s\\EopStorage\0", DIRFLYINFO);
	}

	// Инициализация каталога для считывания массива секунд
	// координации UTC-IAT
	// C:\A\CPP\FlyProject\_FlyExecut\FlyModInfo 
	SetLeapSecondsPath(DIRFLYINFO);
	rc = LeapSecondsInit(true); if (rc) return 1;

	return 0;
}

//vector<string>	ErrorsMsg ;
//vector<string>	WarningsMsg ;
/*
const double DTPCor1975 = 9131.;				 // Дата 1 января 1975 года по отношению к 1 января 2000 года

const double k_mu_kms = 398600.44;				 //  Гравитационная постоянная,           [км**3/cек**2]												 
const double k_b0     = +2.97553634058240e+03;   //  Гравитационная постоянная,           [дткм**3/ccc**2]
const double k_c20    = -1.08262570000000e-03;   //  Коэфф.разложения потенциала Земли,   [дткм**2/ccc**2]
const double k_omega  = +6.30038809698000e+00;   //  Угловая скорость вращения Земли,     [рад/ccc]
const double k_rcp    = +6.37100000000000e-01;   //  Средний радиус Земли,                [дткм]
const double k_a0     = +6.37813600000000e-01;   //  Большая полуось ОЗЭ ,                [дткм]
const double k_alfa0  = +3.35282440000000e-03;   //  Полярное сжатие ОЗЭ ,
const double k_alfak  = +3.35232986926000e-03;   //  Полярное сжатие ЭК ,
const double k_ak     = +6.37824500000000e-01;   //  Большая полуось ЭК ,                 [дткм]
const double k_ekb    = +6.69342162180000e-03;   //  Квадрат эксцентриситета ЭК,
const double k_gekw   = +7.32062499840000e+03;   //  Ускорение силы тяжести на экваторе,  [дткм/ccc**2]
const double k_g0     = 9.80665;                 //  Ускорение силы тяжести на экваторе,  [м/c**2]
const double k_eps    = 1.96572649767644139 ;	 //	 = 1.5*1,31048433178429426  км**5/ сек**2

const double k_dx     = +2.50000000000000e-06;   //  Смещение центра ЭК отн.центра ОЗЭ ,  [дткм]
const double k_dy     = -1.41000000000000e-05;   //              --
const double k_dz     = -8.00000000000000e-06;   //              --
const double k_cfak   = +2.59020683712400e+06;   //  Скорость света фактическая,          [дткм/ccc]
const double k_cnom   = +2.59200000000000e+06;   //  Скорость света номинальная,          [дткм/ccc]
const double k_tmt    = +2.73790926300000e-03;   //  Коэффициенты перехода от солнечного времени к
const double k_tmf    = +2.73781190600000e-03;   //    звездному в случае фиксированной точки весны
const double k_rekw   = +6.37816000000000e-01;   //  Экваториальный радиус Земли,         [дткм]
const double k_threeh = +1.25000000000000e-01;   //  3 часа,                              [ccc]
const double k_pi     = M_PI;        // +3.14159265358979e+00;   //  Число Пи
const double k_polpi  = M_PI/2.0;    // +1.57079632679000e+00;   //  Число Пи/2
const double k_dwapi  = M_PI*2.0;    // +6.28318530717000e+00;   //  Число Пи*2
const double k_t2pi   = M_PI*1.5;    // +4.71238897037000e+00;   //  Число Пи*3/2
const double k_cbc    = +8.64000000000000e+04;   //  Секунд в сутках                      [сек]
const double k_cbm    = +1.44000000000000e+03;   //  Минут в сутках                       [мин]
const double k_e0     = +1.36825000000000e+03;   //  Пост. энер. осв.на пов-ти Земли [(кг дткм**2)/(m**2 ccc**3]
const double k_gs     = +9.90693071599657e+08;   //  Гелиоцентрическая гравит. пост.      [дткм**3/ссс**2]
const double k_gl     = +3.65992110957681e+01;   //  Селеноцентрическая гравит. пост.     [дткм**3/ссс**2]
const double k_qk     = +8.13005600000000e+01;   //  Отношение массы Земли к массе Луны   [ - ]
const double k_qk2    = +3.00000000000000e-01;   //  Число Лява                           [ - ]
const double k_ql     = +4.36332312998585e-02;   //  Угол запаздывания приливной волны    [радиан]
const double k_ae     = +1.49597900000000e+04;   //  Астрономическая единица              [дткм]
const double k_rcp_c  = +6.95980000000000e+01;   //  Средний радиус Солнца,               [дткм]
const double k_rcp_l  = +1.73800000000000e-01;   //  Средний радиус Луны,                 [дткм]
const double k_zona   = +1.25000000000000e-01;   //  Разница зонального времени от Гринвича [ссс]
const double k_vek    =   21;                    //  Текуший век (сегодня  =  20)
const double k_vekcme =   50;                    //  Год контроля смены века
const double k_dwx    = +0.00000000000000e+00;   //  Орентировка оси X ЭК относительно ОЗЭ [рад]
const double k_dwy    = +1.69684788388338E-06;   //  Орентировка оси Y ЭК относительно ОЗЭ [рад]
const double k_dwz    = +3.19977029532295E-06;   //  Орентировка оси Z ЭК относительно ОЗЭ [рад]

const bool k_Flag_Pril =        0;             //  Флаг учета приливов в координатах ИС
const bool k_Flag_Tekt =        0;             //  Флаг учета движения тектонических плит в координатах ИС
const bool k_Flag_Pvz  =        0;             //  Флаг неучета ПВЗ при переводах АСК-ГСК
const bool k_Flag_Pril_Pvz  =   0;             //  Флаг учета ежесуточных приливных поправок ПВЗ
const double k_g_jupiter = 945906.3002000;     //  Гравит. пост. Юпитера  [дткм**3/ссс**2]
const double k_g_saturn  = 283225.4230000;     //  Гравит. пост. Сатурна  [дткм**3/ссс**2]
const double k_g_uran    =  43256.1020000;     //  Гравит. пост. Урана    [дткм**3/ссс**2]
const double k_g_venera  =   2425.0579000;     //  Гравит. пост. Венеры   [дткм**3/ссс**2]
const double k_g_mars    =    319.7118400;     //  Гравит. пост. Марса    [дткм**3/ссс**2]
const double k_g_neptun  =  51034.4830000;     //  Гравит. пост. Нептуна  [дткм**3/ссс**2]
const double k_g_merkurj =    164.4687000;     //  Гравит. пост. Меркурия [дткм**3/ссс**2]
const double k_g_pluton  =      7.3276157;     //  Гравит. пост. Плутона  [дткм**3/ссс**2]
// Разница в ссс между датой начала отсчёта времени и эпохой 2000 (01.01.2000)
const long   DELTA_2000  = -9131L ;
*/
//---------------------------------------------------------------------------
// Дата начала отсчёта времени
//DATE   D1975( 1, 1, 1975 ) ;
//---------------------------------------------------------------------------
// Матрица перехода из J2000 в B1975
static double  Gl_mJ2000_B1975[9] ;
double*        mJ2000_B1975 = Gl_mJ2000_B1975 ;
//---------------------------------------------------------------------------
// Mатрица перехода из гринвичской замороженной СК в АСК
static double  Gl_mgrzab[9] ;
double*        mgrzab = Gl_mgrzab ;
//---------------------------------------------------------------------------
POLE_AE POLEAE ;
POLE_AE* Wae = &POLEAE ;
//---------------------------------------------------------------------------
void ClearErrorsMSG( ) 
{
	//ErrorsMsg.clear( ) ;
	//WarningsMsg.clear( ) ;
}
//---------------------------------------------------------------------------
void AddErrorMSG( char* Msg ) 
{
	//ErrorsMsg.push_back( Msg ) ;
}
//---------------------------------------------------------------------------
void AddWarningMSG( char* Msg ) 
{
	//WarningsMsg.push_back( Msg ) ;
}
//---------------------------------------------------------------------------
int CountErrorsMSG( ) 
{
	//return ( ( int )ErrorsMsg.size( ) ) ;
	return 0 ;
}
//---------------------------------------------------------------------------
int CountWarningsMSG( ) 
{
	//return ( ( int )WarningsMsg.size( ) ) ;
	return 0 ;
}
//---------------------------------------------------------------------------
const char* GetErrorMsg( int i ) 
{
	//if ( i < 0 || i >= ( int )ErrorsMsg.size( ) ) return 0 ;
	//return ( ErrorsMsg[ i ].c_str( ) ) ;
	return "" ;
}
//---------------------------------------------------------------------------
const char* GetWarningMsg( int i ) 
{
	//if ( i < 0 || i >= ( int )WarningsMsg.size( ) ) return 0 ;
	//return ( WarningsMsg[ i ].c_str( ) ) ;
	return "" ;
}
//---------------------------------------------------------------------------