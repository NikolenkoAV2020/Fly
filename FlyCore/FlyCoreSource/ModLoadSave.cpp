// ModLoadSave.cpp 
//---------------------------------------------------------------------------
#include <stdafx.h>

#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ZMForceGrav.h>
#include <FlyCoreSource\\ZMSpaceCraft.h>

//---------------------------------------------------------------------------
int LoadNU( ZKEYNU& Key, ZNU& NU ) 
{
	int rc = 0 ;

	NU.Key    = Key ;
	NU.numlsf = 1 ;
	DATE dt(1, 1, 2011);
	TIME tm( 0, 0, 0, 0 ) ;

	TA_DMB( 1, &dt, &tm, &NU.t ) ;
	NU.vit  = 101 ;
	NU.sk   = _SK_OSK1975 ;
	NU.X[0] = TO_DTKM( 15000.0 ) ;
	NU.X[1] = 0.0012 ;
	NU.X[2] = TO_RAD( 69.0 ) ;
	NU.X[3] = TO_RAD( 250.0 ) ;
	NU.X[4] = TO_RAD( 125.0 ) ;
	NU.X[5] = TO_RAD( 15 ) ;
	NU.Sb   = 0 ;
	NU.Kp   = 0 ;
	NU.M0   = 1503 ;
	NU.MF   = 197 ;
	// сверка 1_prognoz_25-4x4.txt
	NU.sk   = _SK_ASK2000 ;
	NU.X[0] = TO_DTKM(-3579.540763 ) ;
	NU.X[1] = TO_DTKM(-14116.920150 ) ;
	NU.X[2] = TO_DTKM( 3617.127245 ) ;
	NU.X[3] = TO_DTKMSSS( 2.118713337 ) ;
	NU.X[4] = TO_DTKMSSS( 0.660510148 ) ;
	NU.X[5] = TO_DTKMSSS( 4.650420046 ) ;

/*
NU.X[0] = -3579.540764  ;
NU.X[1] = -14116.920150 ;
NU.X[2] =  3617.127245  ;
NU.X[3] =  2.118713337  ;
NU.X[4] =  0.660510148  ;
NU.X[5] =  4.650420046  ;

double mu = k_b0*1e4/8.64/8.64 ;
mu = 398600.44 ; 

double* R = &NU.X[0] ;  
double* V = &NU.X[3] ;
double C[ 3 ] ; wek_pr( R, V, C ) ; 
double CM2 = FR2( C ) ;
double CM  = FR( C ) ;
double A = CM2 / mu ;

NU.X[0] = -3649.602075  ;
NU.X[1] = -14096.714005 ;
NU.X[2] =  3625.908859  ;
NU.X[3] =  2.133664425  ;
NU.X[4] =  0.648624348  ;
NU.X[5] =  4.645254465  ;

wek_pr( R, V, C ) ; 
CM2 = FR2( C ) ;
A = CM2 / mu ; 
*/

//double  Global_m2000_75[ 9 ] = {
//	0.999981423814942,   0.00559013887986178,  0.00242947985080896,
//	-0.00559013888057073, 0.999984375028524,   -6.79033614644759e-06,
//	-0.00242947984917768,-6.79091976566359e-06, 0.9999997048786418 } ;
/*
double Pr2[3], Mpr2[9] ;
preces( 0, J_ask0, Pr2 );
Calc_Matr_Preces_B1975_J2000( Mpr2 ) ;

int i ; 
double dm2[9] ; for( i =0 ; i < 9; i++ ) dm2[i] = Mpr2[i]-Global_m2000_75[i] ;

double Pr[3], Mpr[9] ;
ZPrecess_J2000( J_ask0, Pr) ;
ZMatr_Precess( Pr, Mpr ) ;

double dm[9] ; for( i =0 ; i < 9; i++ ) dm[i] = Mpr[i]-Global_m2000_75[i] ;
double dm3[9] ; for( i =0 ; i < 9; i++ ) dm3[i] = Mpr[i]-Mpr2[i] ;
double dP[9] ; for( i =0 ; i<3; i++ ) dP[i] = Pr2[i]-Pr[i] ;
*/
//dt = DATA(10, 1, 2000);
//tm = TIME(19, 59, 28, 0) ;
//double dTime ; TA_DMB( 1, &dt, &tm, &dTime ) ;
//AEJG(dTime,12);


/*
----- НУ движения КА -  68 ---------------------------------
 Номер - 1201    Тип - 10  Мод -   0  БЦ -  0 
 Дата  11.03.2014   Время  02.12.11.65899   Виток 3902  
 Номер ЛШС  1016    Номер ПУ  0       Карта связи 0       
------------------------------------------------------------
             АСК-2000             ГСК            АСК-ИстЭ
 X,   км    3547.509260       -897.659240       3528.579176
 Y,   км    5881.353276      -6809.505751       5892.731937
 Z,   км      -4.730527         -0.000000          0.000000
 Vx, м/с   -1709.775038       1492.303964      -1723.355784
 Vy, м/с    1030.633684       -191.037238       1025.379421
 Vz, м/с    7354.209292       7351.773437       7351.773437
*/
/*
	dt = DATA(11, 3, 2014 );
	tm = TIME( 2, 12, 11, 0.659 ) ;
	TA_DMB( 1, &dt, &tm, &NU.t ) ;

	NU.sk   = IDSK_ASK2000 ;
	NU.X[0] = TO_DTKM( 3547.509260 ) ;
	NU.X[1] = TO_DTKM( 5881.353276 ) ;
	NU.X[2] = TO_DTKM(-4.730527 ) ;
	NU.X[3] = TO_DTKMSSS(-1.709775038 ) ;
	NU.X[4] = TO_DTKMSSS( 1.030633684 ) ;
	NU.X[5] = TO_DTKMSSS( 7.354209292 ) ;

ZNU NU1 = NU ;
ConvertSkNu( NU1, IDSK_GSK ) ;
DocNU( NU1, "___NU-GSK.txt" ) ;

ConvertSkNu( NU1, IDSK_ASK1975 ) ;
DocNU( NU1, "___NU-1975.txt" ) ;

ConvertSkNu( NU1, IDSK_ASK2000 ) ;
DocNU( NU1, "___NU-2000.txt" ) ;

ConvertSkNu( NU1, IDSK_GSK ) ;
DocNU( NU1, "___NU-GSK_2.txt" ) ;

ConvertSkNu( NU1, IDSK_ASK2000 ) ;
DocNU( NU1, "___NU-2000_2.txt" ) ;
*/
/*
NU1 = NU ;
ConvertSkNu( NU1, IDSK_ASKTE ) ;
DocNU( NU1, "___NU-ASKTE.txt" ) ;

NU1 = NU ;
ConvertSkNu( NU1, IDSK_ASK1975 ) ;
double R[7] ;
R[0] = NU1.t ;
memcpy( &R[1], NU1.X, 6*sizeof(double)) ;
ConvertSkNu( NU1, IDSK_ASKTE ) ;
DocNU( NU1, "___NU-ASKTE_75.txt" ) ;


ZASK1975toASKTE(R[0], &R[1], NU1.X, 1) ;
DocNU( NU1, "___NU-ASKTE_75_2.txt" ) ;

NU1 = NU ;
ConvertSkNu( NU1, IDSK_ASK1975 ) ;
DocNU( NU1, "___NU-ASK75.txt" ) ;
ConvertSkNu( NU1, IDSK_ASKTE ) ;
DocNU( NU1, "___NU-ASKTE2.txt" ) ;


ConvertSkNu( NU1, IDSK_OSK1975 ) ;
DocNU( NU1, "___NU-OSK75.txt" ) ;
NU1 = NU ;
ConvertSkNu( NU1, IDSK_ASKTE ) ;
DocNU( NU1, "___NU-ASKTE.txt" ) ;
ConvertSkNu( NU1, IDSK_ASK2000 ) ;
DocNU( NU1, "___NU-ASK2000.txt" ) ;
*/
/*	//
	dt = DATA(9, 2, 2016 );
	tm = TIME( 6, 5, 29, 0 ) ;
	TA_DMB( 1, &dt, &tm, &NU.t ) ;
	NU.vit    = 3575 ;
	NU.sk     = IDSK_ASK1975 ;
	NU.X[ 0 ] = TO_DTKM( 2619.18632 ) ;
	NU.X[ 1 ] = TO_DTKM(-6601.67831 ) ;
	NU.X[ 2 ] = TO_DTKM( 0 ) ;
	NU.X[ 3 ] = TO_DTKMSSS(-1.48268404 ) ;
	NU.X[ 4 ] = TO_DTKMSSS(-0.590555317 ) ;
	NU.X[ 5 ] = TO_DTKMSSS( 7.41570078 ) ;
	NU.Sb     = 0.0103050025 ;
	NU.Kp     = 0 ;
	NU.M0     = 1503 ;
	NU.MF     = 197 ;
*/
/*	//
	dt = DATA(18, 2, 2013 );
	tm = TIME( 10, 8, 35, 0.209 ) ;
	TA_DMB( 1, &dt, &tm, &NU.t ) ;
	NU.vit    = 6770 ;
	NU.sk     = IDSK_ASK1975 ;
	NU.X[ 0 ] = TO_DTKM( 4516.29382 ) ;
	NU.X[ 1 ] = TO_DTKM(-5717.2127 ) ;
	NU.X[ 2 ] = TO_DTKM( 0 ) ;
	NU.X[ 3 ] = TO_DTKMSSS( 1.83611362 ) ;
	NU.X[ 4 ] = TO_DTKMSSS( 1.45117076 ) ;
	NU.X[ 5 ] = TO_DTKMSSS( 6.81835168 ) ;
	NU.Sb     = 0.017 ;
	NU.Kp     = 0 ;
	NU.M0     = 1503 ;
	NU.MF     = 197 ;
*/
/*	//
	dt = DATA(31, 5, 2014 );
	tm = TIME( 6, 10, 24, 0.296 ) ;
	TA_DMB( 1, &dt, &tm, &NU.t ) ;
	NU.vit    = 398 ;
	NU.sk     = IDSK_ASK1975 ;
	NU.X[ 0 ] = TO_DTKM(-4050.029426 ) ;
	NU.X[ 1 ] = TO_DTKM( 5231.245542 ) ;
	NU.X[ 2 ] = TO_DTKM( 0 ) ;
	NU.X[ 3 ] = TO_DTKMSSS(-0.509903675 ) ;
	NU.X[ 4 ] = TO_DTKMSSS(-0.447923403 ) ;
	NU.X[ 5 ] = TO_DTKMSSS( 7.676913217 ) ;
	NU.Sb     = 0.013586915 ;
	NU.Kp     = 0 ;
	NU.M0     = 1503 ;
	NU.MF     = 197 ;
*/
	//
/*
----- НУ движения КА -  68 ---------------------------------
 Номер - 1201    Тип - 10  Мод -   0  БЦ -  0 
 Дата  11.03.2014   Время  02.12.11.65899   Виток 3902  
 Номер ЛШС  1016    Номер ПУ  0       Карта связи 0       
------------------------------------------------------------
             АСК-2000             ГСК            АСК-ИстЭ
 X,   км    3547.509260       -897.659240       3528.579176
 Y,   км    5881.353276      -6809.505751       5892.731937
 Z,   км      -4.730527         -0.000000          0.000000
 Vx, м/с   -1709.775038       1492.303964      -1723.355784
 Vy, м/с    1030.633684       -191.037238       1025.379421
 Vz, м/с    7354.209292       7351.773437       7351.773437
*/
/*
	dt = DATA(11, 3, 2014 );
	tm = TIME( 2, 12, 11, 0.659 ) ;
	TA_DMB( 1, &dt, &tm, &NU.t ) ;
	NU.vit    = 3902 ;
	NU.sk     = IDSK_ASK2000 ;
	NU.X[ 0 ] = TO_DTKM( 3547.509260 ) ;
	NU.X[ 1 ] = TO_DTKM( 5881.353276 ) ;
	NU.X[ 2 ] = TO_DTKM( -4.730527 ) ;
	NU.X[ 3 ] = TO_DTKMSSS(-1.709775038 ) ;
	NU.X[ 4 ] = TO_DTKMSSS( 1.030633684 ) ;
	NU.X[ 5 ] = TO_DTKMSSS( 7.354209292 ) ;
*/
/*
	NU.sk     = IDSK_GSK ;
	NU.X[ 0 ] = TO_DTKM(-897.659240 ) ;
	NU.X[ 1 ] = TO_DTKM(-6809.505751 ) ;
	NU.X[ 2 ] = TO_DTKM( 0 ) ;
	NU.X[ 3 ] = TO_DTKMSSS( 1.492303964 ) ;
	NU.X[ 4 ] = TO_DTKMSSS(-0.191037238 ) ;
	NU.X[ 5 ] = TO_DTKMSSS( 7.351773437 ) ;
*/
//	NU.Sb     = 0.012720 ;
//	NU.Kp     = 0 ;
//	NU.M0     = 1503 ;
//	NU.MF     = 197 ;
/*
 Номер - 14      Тип -  1  Мод -   0  БЦ -  0 
 Дата  03.03.2015   Время  04.15.22.85391   Виток 19    
 Номер ЛШС  1015    Номер ПУ  0       Карта связи 0       
------------------------------------------------------------

             АСК-2000             ГСК            АСК-ИстЭ
 X,   км    8227.733306      -8249.957537       8233.052144
 Y,   км   -1569.962762       1448.699272      -1541.875914
 Z,   км     -12.268308         -0.000000          0.000000
 Vx, м/с     509.724665       -355.787383        491.297534
 Vy, м/с    2637.284943      -2043.047793       2639.258601
 Vz, м/с    6355.440350       6356.072208       6356.072208
 */
/*
	dt = DATA(3, 3, 2015 );
	tm = TIME( 4, 15, 22, 0.85391 ) ;
	TA_DMB( 1, &dt, &tm, &NU.t ) ;
	NU.vit    = 19 ;
	NU.sk     = IDSK_GSK ;
	NU.X[ 0 ] = TO_DTKM(-8249.957537 ) ;
	NU.X[ 1 ] = TO_DTKM( 1448.699272 ) ;
	NU.X[ 2 ] = TO_DTKM( 0.0 ) ;
	NU.X[ 3 ] = TO_DTKMSSS(-0.355787383 ) ;
	NU.X[ 4 ] = TO_DTKMSSS(-2.043047793 ) ;
	NU.X[ 5 ] = TO_DTKMSSS( 6.356072208 ) ;
	NU.Sb     = 0.01 ;
	NU.Kp     = 200 ;
	NU.M0     = 1503 ;
	NU.MF     = 197 ;
	// IDSK_GSK
*/
/*
	dt = DATA(20, 6, 2018 );
	tm = TIME( 1, 12, 59, 0.468 ) ;
	TA_DMB( 1, &dt, &tm, &NU.t ) ;
	NU.vit    = 1 ;
	NU.sk     = IDSK_GSK ;
	NU.X[ 0 ] = TO_DTKM(-6037.2736063 ) ;
	NU.X[ 1 ] = TO_DTKM(-3320.8649059 ) ;
	NU.X[ 2 ] = TO_DTKM( 0 ) ;
	NU.X[ 3 ] = TO_DTKMSSS(-0.70912526776 ) ;
	NU.X[ 4 ] = TO_DTKMSSS( 1.3090186704 ) ;
	NU.X[ 5 ] = TO_DTKMSSS( 7.5435922152 ) ;
	NU.Sb     = 0.0010295921 ;
	NU.Kp     = 0 ;
	NU.M0     = 1503 ;
	NU.MF     = 197 ;
*/
	if (rc ) {
		char S[256] ;
		sprintf(S, "Ошибка загрузки НУ %d.%d.%d.%d для КА %d\0", Key.nom, Key.type, Key.mod, Key.bc, Key.ka ) ; 
		AddErrorMSG(S) ; 
	}
	return rc ;
}
/*
----- НУ движения КА -  68 ---------------------------------
 Номер - 1201    Тип - 10  Мод -   0  БЦ -  0 

 Дата  11.03.2014   Время  02.12.11.65899   Виток 3902  
 Номер ЛШС  1015    Номер ПУ  0       Карта связи 0       
------------------------------------------------------------
             АСК-2000             ГСК            АСК-ИстЭ
 X,   км    3547.509260       -897.659240       3528.579176
 Y,   км    5881.353276      -6809.505751       5892.731937
 Z,   км      -4.730527         -0.000000          0.000000
 Vx, м/с   -1709.775038       1492.303964      -1723.355784
 Vy, м/с    1030.633684       -191.037238       1025.379421
 Vz, м/с    7354.209292       7351.773437       7351.773437
------------------------------------------------------------
*/

//---------------------------------------------------------------------------
// Документируем НУ
int DocNU( ZNU& NU, char* FileName, FILE* pf )
{
	FILE* PF ;
	if ( FileName ) {
		PF = fopen( FileName, "w" ) ;
	} else {
		PF = pf ;
		if ( !PF ) PF = fopen( "DocNu.txt", "w" ) ;
	}
	if ( !PF ) return-1 ; 

	DATE dt;
	TIME tm ;
	TA_DMB( 0, &dt, &tm, &NU.t ) ;
	fprintf ( PF, "\n\n\n" ) ;   
	fprintf ( PF, "   %d-%d-%d-%d\n", NU.Key.ka, NU.Key.nom, NU.Key.type, NU.sk ) ;  
	fprintf ( PF, "   %02d.%02d.%04d-%02d:%02d:%02d,%03d-%d\n", dt.d, dt.m, dt.g, tm.h, tm.m, tm.s, ( int )( 1000.0 * tm.d ), NU.vit ) ;
	for( int i = 0 ; i < 6 ; i++ ) {
		double y = NU.X[ i ] ;
		if ( NU.sk == _SK_OSK1975 || NU.sk == _SK_OSKTE || NU.sk == _SK_OSKTES || NU.sk == _SK_OSK2000 ) {
			if ( !i ) y = TO_KM( y ) ;
			if ( i > 1 ) y = TO_GRAD( y ) ;
		} else {
			y = i < 3 ? TO_KM( y ) : ( 1e3 * TO_KMS( y ) ) ;	 
		}
		fprintf ( PF, "   %.10lf\n", y ) ;
	}

	if ( FileName || ( !FileName && !pf ) ) fclose( PF ) ; PF = 0 ; 

	return 0 ;
}

//---------------------------------------------------------------------------
int LoadKA( long numKA, ZKA& KA )
{
	int rc = 0 ;
	KA.num      = numKA ; 
	KA.M0       = 1500 ;
	KA.MF       = 200 ;
	KA.tstart   = 10.0 ;
	KA.bpl      = 0 ;
	KA.min_step = 1.0 ;
	KA.max_step = 600.0 ;

	if ( rc ) { 
		char S[256] ;
		sprintf( S, "Ошибка загрузки данных по КА %d\0", numKA ) ; 
		AddErrorMSG( S ) ; 
	}
	return rc ;
}

//---------------------------------------------------------------------------
int LoadLSF( long numLSF, ZLSF& LSF )
{
	int rc = 0 ;
	// Условный номер шкалы
	LSF.num     = numLSF ;
	// Номер модели ГПЗ
	//   1 - 85 год 
	//   2 - 90 год 
	//   3 - 96 год EGM96
	LSF.vgpz    = GPZEGM96 ;
	// Колличество гармоник N*M
	LSF.ngpz    = 4 ;
	LSF.mgpz    = 4 ;
	// Номер модели атмосферы
	//   0 - атмосфера не учитывается
	//   1 - атмосфера статическая 81 года ГОСТ 4401-81 
	//   2 - атмосфера АН-62
	//   3 - атмосфера динамическая 2004 года ГОСТ Р25645.166-2004 
	LSF.vatm    = ATMNULL ;
	// Номер модели светового давления
	//   0 - световое давление на поверхность КА не учитывается
	//   1 - простая "каппа" 
	//   2 - 
	LSF.vsvd    = 0 ;       
	// 
	LSF.varsp   = 0 ;
	// Признак учета работы ДУ
	//   0 - нет тяги ДУ
	//   1 - реализация импульсных коррекций орбиты
	//   2 - моделирование работы ДУ с двигателем конечной тяги (протяженный активный участок) 
	LSF.isDU    = 0 ;
	// Признак учета приливов
	LSF.vTide   = TIDENULL ;
	// Признаки учета гравитационных возмущений обусловленных притяжением Луной и Солнцем
	LSF.isSun   = 0 ;
	LSF.isMoon  = 0 ;
	// Признаки учета гравитационных возмущений обусловленных притяжением планет
	LSF.isPlanet[0] = 0 ;
	LSF.isPlanet[1] = 0 ;
	LSF.isPlanet[2] = 0 ;
	LSF.isPlanet[3] = 0 ;
	LSF.isPlanet[4] = 0 ;
	LSF.isPlanet[5] = 0 ;
	LSF.isPlanet[6] = 0 ;
	LSF.isPlanet[7] = 0 ;
	
	if ( rc ) {
		char S[256] ;
		sprintf( S, "Ошибка загрузки ЛШС %d\0", numLSF ) ; 
		AddErrorMSG(S) ; 
	}
	return rc ;
}

//---------------------------------------------------------------------------
int LoadGravPole(ZMForceGrav* GPZ, int vgpz, int ng, int mg )
{
	int rc = 0 ;
	if (!GPZ) return-1 ;
	if (GPZ->GravPole.VariantGPZ == vgpz) return 0 ; // Поле уже загружено

	//-----------------------------------------------------------------------
	// Блокирование нижеследующего кода на случай многопоточного приложения.
	// !!!! Операции чтения/записи в файл не являются потокобезопасными !!!!
	//-----------------------------------------------------------------------
	FAST_SEMAPFORE_OPEN("LoadGravPoleFromFile") ;

	//-----------------------------------------------------------------------
	// Имя файла, содержащего параметры разложения ГПЗ 
	// по сферическим функциям
	char filename[ 512 ] ;
	switch(vgpz) {
		case GPZ85:    
			sprintf(filename, "%s\\GravPolePZ85.dat\0", DIREGF); 
			break;
		case GPZ90:    
			sprintf(filename, "%s\\GravPolePZ90.dat\0", DIREGF); 
			break;
		case GPZ9011:  
			sprintf(filename, "%s\\GravPolePZ9011.dat\0", DIREGF); 
			break;
		case GPZEGM96: 
			sprintf(filename, "%s\\GravPoleEGM96.dat\0", DIREGF); 
			break;
		case GPZGEMT3:
			sprintf(filename, "%s\\GEMT3.dat\0", DIREGF);
			break;
		case GPZJGM3:
			sprintf(filename, "%s\\JGM3.dat\0", DIREGF);
			break;
		default: { rc =-1 ; goto end ; }
	}
	// Открытие текстового файла
	FILE* fp ;
	fp = fopen( filename, "r" ) ; if (!fp) { rc =-2 ; goto end ; }
	// Условный номер модели ГПЗ (заданный)
	GPZ->GravPole.VariantGPZ = vgpz ;
	// Макс. степень разложения ГПЗ (заданное число зональных гармоник)
	GPZ->GravPole.Nmax       = ng ;
	// Максимально возможное число зональных гармоник в выбранной модели ГПЗ
	rc = fscanf(fp, "%hd", &GPZ->GravPole.Nmax_BD);
	if (rc!=1)  { rc =-3 ; goto end ; }
	if ( GPZ->GravPole.Nmax_BD <= 0 ) 
	{ rc =-4 ; goto end ; }
	// Если требуемое число гармоник ng больше чем максимально возможное 
	// (которое записано в загружаемом текстовом файле) Nmax_BD, 
	// то генерируется ошибка 
	if ( GPZ->GravPole.Nmax_BD < ng ) { rc =-5 ; goto end ; }
	// Экваториальный радиус
	rc = fscanf( fp, "%lf", &GPZ->GravPole.AE ) ; 
	if (rc!=1)  { rc =-3 ; goto end ; }
	GPZ->GravPole.AE /= 1e7 ;
	// Константа характеризующая центральное ГПЗ (C00, mu, ...)
	rc = fscanf( fp, "%lf", &GPZ->GravPole.B0 ) ; 
	if (rc!=1)  { rc =-3 ; goto end ; }
	GPZ->GravPole.B0km = GPZ->GravPole.B0 / 1e9 ;
	GPZ->GravPole.B0  *= (8.64*8.64 / 1e13) ;
	// Константа характеризующая полярное сжатие Земли (C20, J20)
	rc = fscanf( fp, "%lf", &GPZ->GravPole.J2 ) ; 
	if (rc!=1)  { rc =-3 ; goto end ; }

	//-----------------------------------------------------------------------
	int    k, km, i, j ;
	double d ;
	// Модель ГПЗ должна содержать пары коэффициентов Cnm и Dnm 
	// для следующих индексов
	// 00
	// 10 11
	// 20 21 22
	// 30 31 32 33
	// 40 41 42 43 44
	// 50 51 52 53 54 55
	// ---------------------
	// N0 N1 N2 ---------------- NN
	// Модель движения может учитывать только часть из этих слогаемых,
	// то есть разложение ГПЗ по сферическим функциям может быть учечённым 

	// Вычисление колличества коэффициентов которые должен содержать файл
	km = GPZ->GravPole.Nmax+1 ;
	km = (1+km)*km ;
	// Выделение памяти для хранения коэффициентов Cnm и Dnm 
	// разложения ГПЗ по сферическим функциям 
	if (GPZ->GravPole.Garm) { 
		delete[] GPZ->GravPole.Garm ; 
		GPZ->GravPole.Garm = nullptr ; 
	}
	GPZ->GravPole.Garm = new double[km] ;

	// Цыкл считывания коэффициентов
	k = 0 ;
	while ( 1 ) {
		// Индекс n
		rc = fscanf( fp, "%d", &i ) ; 
		if (rc!=1)  break ;
		// Индекс m
		rc = fscanf( fp, "%d", &j ) ; 
		if (rc!=1)  break ;
		// Коэффициент Cnm
		rc = fscanf( fp, "%lf", &d) ; 
		if (rc!=1)  { rc =-3 ; goto end ; }
		GPZ->GravPole.Garm[k] = d ; k++ ; 
		if (k>=km) { rc =-3 ; goto end ; }
		// Коэффициент Dnm
		rc = fscanf( fp, "%lf", &d) ; 
		if (rc!=1)  { rc =-3 ; goto end ; }
		GPZ->GravPole.Garm[k] = d ; k++ ; 
		if (k>=km) break ;
	}
	GPZ->InDimen = false ;

    // Создание массива факториалов
    rc = GPZ->MakeFactorial( ) ; 
	if (rc) { rc =-6 ; goto end ; }
	// Перевод коэффициентов во внутреннюю размерность
	rc = GPZ->Change_Dimen(1) ;  
	if (rc) { rc =-7 ; goto end ; }
end : ;
	if ( fp ) fclose( fp ) ; 

	//-----------------------------------------------------------------------
	// Открытие части (выше по тексту) программного кода для других потоков
	//-----------------------------------------------------------------------
	FAST_SEMAPFORE_CLOSE() ;

	//-----------------------------------------------------------------------
	if ( rc ) {
		char S[256] ;
		sprintf(S, "Ошибка загрузки ГПЗ, вариант %d\0", vgpz ) ;
		AddErrorMSG(S) ; 
	}
	return rc ;
}

//-------------------------------------------------------------------------------
int LoadDU(int nka, int ndu, ZCDU& DU)
{
	DU.Clear() ;

	DU.ka = nka ;
	DU.n  = ndu ;
	DU.k  = 4 ;		// Число участков моделирования
	DU.ip = 1 ;		// Признак импульса последействия
	DU.mu = 2 ;		// Номер основного участка
	DU.Var= 0 ;
	DU.mp = 1.0 ;	// Тяга основного участка
	DU.mt = 1000 ;	// Максимальная длительность основного участка
	DU.alfa = 0 ;
	DU.beta = 0 ;
	DU.isDimKmS = true ;
	DU.i = new ZDUItem[DU.k] ;

	DU.k  = 1 ;		// Число участков моделирования
	DU.ip = 0 ;		// Признак импульса последействия
	DU.mu = 0 ;
	DU.i[0].p  = 300.102481 ; 
	DU.i[0].pu = 307.919719 ; 
	DU.i[0].ms = 0 ; 
	DU.i[0].t  = 1000 ;

/*
	DU.i[0].p = 0.1 ; DU.i[0].pu = 200 ; DU.i[0].ms = 0 ; DU.i[0].t = 1 ;
	DU.i[1].p = 0.5 ; DU.i[1].pu = 250 ; DU.i[1].ms = 0 ; DU.i[1].t = 0.5 ;
	DU.i[2].p = 1.0 ; DU.i[2].pu = 300 ; DU.i[2].ms = 0 ; DU.i[2].t = 3000 ;
	DU.i[3].p = 0.3 ; DU.i[3].pu = 280 ; DU.i[3].ms = 0 ; DU.i[3].t = 0.3 ;
*/
	for (int i=0 ; i<DU.k ; i++) DU.i[i].ms = DU.i[i].p/DU.i[i].pu ; 

	return 0 ;
}

//---------------------------------------------------------------------------
int SaveNU( ZKEYNU& Key, ZNU& NU )
{
	return 0 ;
}
//---------------------------------------------------------------------------
int SaveKA( long numKA, ZKA& KA )
{
	return 0 ;
}
//---------------------------------------------------------------------------
int SaveLSF( long numLSF, ZLSF& LSF )
{
	return 0 ;
}
//---------------------------------------------------------------------------