// NRRHeader.h
// Артём Николаев 28.08.2014
//----------------------------------------------------------------------------------------------------
#if !defined(NRR_HEADER)
#define NRR_HEADER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct {
	char Name[ 32 ] ;
	char Text[ 64 ] ;
} RHiaderItem ;

typedef struct { 
	char s[ 256 ] ; // Само слово
	int  n ;		// Длинна слова
	int  nspace ;	// Колличество последующих пробелов
} RWord ;

typedef vector<RWord> VRWord ; 

class NRHeader {
public :
	NRHeader( ) { Clear( ) ; }
	void Clear( ) {
		// 0 - раздел отсутствует, 1 - раздел найден пустым, 2 - раздел найден, 3 - раздел найден, не верный формат
		is_RINEX_VERSION_TYPE	= 0 ; 
		Version = 0 ;
		memset( TypeFile, 0, sizeof( TypeFile ) ) ;
		memset( TypeData, 0, sizeof( TypeData ) ) ;

		is_PGM_RUNBYDATE		= 0 ;
		memset( ProgName,  0, sizeof( ProgName ) ) ;
		memset( ProgVer,   0, sizeof( ProgVer ) ) ;
		memset( &BuildDate, 0, sizeof( BuildDate ) ) ;
		memset( &BuildTime, 0, sizeof( BuildTime ) ) ;

		is_COMMENT				= 0 ; 
		Comments.clear( ) ;

		is_MARKER_NAME			= 0 ;
		memset( Marker, 0, sizeof( Marker ) ) ;

		is_OBSERVER_AGENCY		= 0 ;
		memset( ObservAgency, 0, sizeof( ObservAgency ) ) ;

		is_APPROX_POSITION		= 0 ;
		apX = apY = apZ = 0 ;

		is_ANTENNA_DELTA		= 0 ;
		aH = aE = aN = 0 ;

		is_WAVELENGTH_FACT		= 0 ;
		WL1 = WL2 = 0 ;

		is_ANALYSIS_CENTER		= 0 ;
		memset( CenrerID, 0, sizeof( CenrerID ) ) ;
		memset( CenterName, 0, sizeof( CenterName ) ) ;

		is_TYPES_OF_DATA		= 0 ;

		is_TYPES_OF_OBSERV		= 0 ;
		CountParams  = 0 ;
		NameParam.clear( ) ;

		is_INTERVAL				= 0 ;
		dt = 0 ;

		isTIME_OF_FIRST_OBS	= 0 ;
		memset( &FirstDate, 0, sizeof( FirstDate ) ) ;
		memset( &FirstTime, 0, sizeof( FirstTime ) ) ;
		memset( TimeScale, 0, sizeof( TimeScale ) ) ;

		is_LEAP_SECONDS			= 0 ;
		LeapSec = 0 ;
	}

	// "RINEX VERSION / TYPE"	Версия формата / тип файла (Observation Data "O" - измерения "C" - часы ) тип данных
	short	is_RINEX_VERSION_TYPE ; // 0 - раздел отсутствует, 1 - раздел найден пустым, 2 - раздел найден, 3 - раздел найден, не верный формат
	float	Version ;
	char	TypeFile[ 60 ] ;
	char	TypeData[ 12 ] ;
	// "PGM / RUN BY / DATE"	Имя программы, сформировавшей файл / Версия программы, сформировавшей файл / Дата формирования файла
	short	is_PGM_RUNBYDATE ;
	char	ProgName[ 64 ] ;
	char	ProgVer[ 64 ] ;
	DATE	BuildDate;
	TIME	BuildTime ;
	// "COMMENT"
	short	is_COMMENT ; 
	vector<string> Comments ;
	// "MARKER NAME"			Уникальное имя маркера
	short	is_MARKER_NAME ;
	char	Marker[ 64 ] ;		
	// "OBSERVER / AGENCY"
	short	is_OBSERVER_AGENCY ;
	char	ObservAgency[ 64 ] ;
	// "APPROX POSITION XYZ"
	short	is_APPROX_POSITION ;
	double	apX, apY, apZ ;
	// "ANTENNA: DELTA H/E/N"
	short	is_ANTENNA_DELTA ;
	double	aH, aE, aN ;
	// "WAVELENGTH FACT L1/2"   Коэффициент кратности передаваемых данных для фазовых измерений на L1 и L2 (1 - полные циклы, 2 - половины циклов)
	short	is_WAVELENGTH_FACT ;
	short	WL1, WL2 ;
	// "ANALYSIS CENTER"		Центр анализа данных Идентификатор и полное название
	short	is_ANALYSIS_CENTER ;
	char	CenrerID[ 16 ] ;
	char	CenterName[ 64 ] ;
	// "# / TYPES OF DATA"		Количество различных типов данных представленных в файле и Список типов данных (для СВОЭВП) 
	// "AR" - поправки к ШВ приемников относительно фиксируемой ШВ
	// "AS" - поправки к ШВ спутников относительно фиксируемой ШВ
	short	is_TYPES_OF_DATA ;
	// "# / TYPES OF OBSERV"	Количество измеряемых параметров Типы измеряемых параметров
	// "С1" "С2" - Псевдодальность (ПТ код ГЛОНАСС, С/А-код для GPS) на частоте f1 (f2)
	// "L1" "L2" - Уход фазы на частоте f1 (f2)
	// "P1" "P2"
	short			is_TYPES_OF_OBSERV ;
	short			CountParams ;
	vector<string>	NameParam ;
	// "INTERVAL"				Интервал между измерениями, сек
	short	is_INTERVAL ;
	float	dt ; 
	// "TIME OF FIRST OBS"		Время первого измерения (дата и время)
	short	isTIME_OF_FIRST_OBS ;
	DATE	FirstDate;
	TIME	FirstTime ;
	char	TimeScale[ 6 ] ;	// Шкала времени: GLO (UTC)
	// "LEAP SECONDS"			Количество "скачущих секунд", начиная с	6 января 1980 года (для перехода к ШВ UTC)                      
	short	is_LEAP_SECONDS ;
	float	LeapSec ;

	// "REC # / TYPE / VERS"
	
	// "ANT # / TYPE"

	// "END OF HEADER"
	
	// Неизвестные записи заголовка
	vector<RHiaderItem> Unknown ;
} ;

#endif // #if !defined(NRR_HEADER)
