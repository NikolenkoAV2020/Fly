//-----------------------------------------------------------------------------
// ZEOPManager.cpp
// Диспетчер параметров вращения Земли (Eath orientation parameters)
//
//-----------------------------------------------------------------------------
// Диспетчер ПВЗ объект который создаётся только однажды полсле загузки 
// библиотеки модели движения. Все объекты модели ZMSC обращаются к одному 
// и тому же объекту ZEOPManager.
// 
// Любому объекту модели движения, так же как и любой прикладной программе 
// доступна глобальная переменная ZEOPManager EOPMng. 
//
//
//
//
//
// А. Николенко 06.03.2019
//-----------------------------------------------------------------------------
#include <stdafx.h>

//-----------------------------------------------------------------------------
// Компоненты Fly
#include <FlySofa.h>		// Функции фундаментальной астрономии

//-----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <FlyCoreSource\\ZEOPManager.h>
#include <FlyCoreSource\\ZIndex.h>

//-----------------------------------------------------------------------------
ZEOPManager EOPMng;
//ZIndexes    EOPIndex;

//-----------------------------------------------------------------------------
ZEOPManager::ZEOPManager() 
{
	Clear();
	// Имена файлов-хранилищ
	PxModelHolder.SetStorageName(MOD_PX_STORAGE_FILE_NAME);
	PyModelHolder.SetStorageName(MOD_PY_STORAGE_FILE_NAME);
	DTModelHolder.SetStorageName(MOD_UT1UTC_STORAGE_FILE_NAME);
	// Интервал времени на котором строится каждая частная модель
	PxModelHolder.SetTimeFrameType(1);
	PyModelHolder.SetTimeFrameType(1);
	DTModelHolder.SetTimeFrameType(1);
}

//-----------------------------------------------------------------------------
ZEOPManager::~ZEOPManager()
{
	Clear() ;
}

//-----------------------------------------------------------------------------
void ZEOPManager::Clear()
{
	EOP.clear() ;
	memset(&EopLoadReport, 0, sizeof(EopLoadReport));

	IsForecastBuild    = false ;
	lastFindTableIndex =-1 ;
	lastFindMJDLeft    = 0 ;
	lastFindMJDRight   = 0 ;
	LastFindIAT        = 0 ;

	TimeTableFirst = 0 ; 
	TimeTableLast  = 0 ; 
	TimeForecastTo = 0 ; 

	MJDTableFirst = 0 ;
	MJDTableLast  = 0 ;

	// Имя файла внешнего источника данных ПВЗ - публикуется IERS
	IersFileName = EOP_IERS_FILE_NAME;
	// Имя файла-хранилища ПВЗ в двоичном виде 
	StorageFileName = EOP_STORAGE_FILE_NAME;
}

//-----------------------------------------------------------------------------
// Табличные ПВЗ на заданную модифицированную юлианскую дату
// Если на заданную дату ПВЗ нет то возвращаем нулевые ПВЗ с юлианской датой -1
//-----------------------------------------------------------------------------
ZEOPItem EmptyItemEOP = {-1, 0, 0, 0 };
ZEOPItem& ZEOPManager::getItemEOP(long Mjd)
{
	if (!EopLoadReport.CountItems || 
		Mjd < EopLoadReport.fromMjd || 
		Mjd > EopLoadReport.toMjd) return EmptyItemEOP;
	long i = Mjd - EopLoadReport.fromMjd;
	if (i<0 || i >= (long)EOP.size()) return EmptyItemEOP;
	return EOP[i];
}

//-----------------------------------------------------------------------------
// Загрузка апостериорных данных из внешненго истояника - текстового файла
// полученного с интернет-ресурса Международной службы вращения Земли IERS.
// Под апостериорными данными понимаются данные публикуемые
// IERS (уклонение полюса X,Y в угловых милисекундах 
// и разность UTC1-UTC в милисекундах)
//
// В случае неудачной загрузки массива априорных ПВЗ прогнозирование
// параметров движения будет проходить с нулевыми зщначениями уклонения
// полюса и разности между UT1 и UTC.
// Данная ситуация не является фатальной и приведёт только к незначительному
// снижению точности результатов прогнозирования. При этом полученные 
// погрешности будут иметь только периодическую составляющую, то есть с течением
// времени ошибка прогноза из-за отсутствия ПВЗ накапливаться не будет.
//
// Одновременно с загрузкой текстового файла в формате IERS формируется двоичный 
// сокращённый файл, содержащий только координаты полюса и отклонение UTC1-UTC, 
// для последующей быстрой загрузки этих данных.
//-----------------------------------------------------------------------------
int ZEOPManager::LoadFromIersFormat(bool isNeedSaveToBin)
{
	// Время начала загрузки
	time_t startTIME = clock();

	// Удаление ранее загруженной информации о ПВЗ
	Clear() ;
	// Удаление ранее сформированного индекса ПВЗ
	//EOPIndex.Clear();

	// Открытие текстового документа для считывания ПВЗ
	ifstream FileTxt;
	FileTxt.open(GetIersFileName(), ios::in);
	if (FileTxt.fail() || FileTxt.eof()) return-1;

	// Открытие двоичного документа 
	ofstream FileBin;	
	if (isNeedSaveToBin) {
		FileBin.open(GetBinaryFileName(), ios::out | ios::binary);
		if (FileBin.fail() || FileBin.eof()) return-2;
	}

	// Локальные переменные для организации процесса загрузки ПВЗ
	int  n, N, rc=0 ;
	char S[512] ;

	// Считывание колличества записей в файле
	FileTxt.getline(S, 512, '\n');
	n = sscanf(S, "%d", &N); if (n != 1) { rc = -1; goto end; }
	if (N<30)    { rc = -2; goto end; } // Слишком мало данных
	if (N>25000) { rc = -3; goto end; } // Слишком много данных 

	// Резервируем в массиве для хранения ПВЗ N записей
	EOP.reserve(N);

	{
	// Локальные переменные для организации процесса загрузки ПВЗ
	char     *s, C, text[64];
	float    F;
	ZEOPItem I;
	DATETIMEUTC UTC;

	// Цыкл считывания файла по строкам 
	while (1) {
		// Считывание строки целиком
		FileTxt.getline(S, 512, '\n'); s = &S[0];

		// Считывание даты
		memcpy(text, s, 2) ; text[2] = '\0' ; s+=2 ;
		n = sscanf(text, "%d", &UTC.G) ; if (n!=1) { rc =-10 ; goto end ; }
		memcpy(text, s, 2) ; s+=2 ;
		// Преобразование года из формата XX в формат XXXX
		if (UTC.G>50) UTC.G+= 1900 ;
		else UTC.G+= 2000 ;
		// Считывание месяца и дня
		n = sscanf(text, "%d", &UTC.M) ; if (n!=1) { rc =-10 ; goto end ; }
		memcpy(text, s, 2) ; s+=2 ;
		n = sscanf(text, "%d", &UTC.D) ; if (n!=1) { rc =-10 ; goto end ; }	

		// Считывание MJ
		n = sscanf(s, "%f", &I.MJD) ; if (n!=1) { rc =-11 ; goto end ; }
		s = SkipSpace(s) ;
		s = SkipToSpace(s) ;

		// Проверка данных о дате
		{
		double D, D2 ;  
		rc = iauDtf2d("UTC", UTC.G, UTC.M, UTC.D, UTC.h, UTC.m, 
			 UTC.s+UTC.d, &D, &D2) ;
		// По двойной JD вычисляем MJD
		D = D+D2-ZMJD0 ;
		// Собственно сама проверка  
		if (fabs(I.MJD-D)>0.001) { rc =-30 ; goto end ; }
		}

		// Считывание ключевого символа формата файла
		s = SkipSpace(s) ;
		n = sscanf(s, "%c", &C) ; if (n!=1) { rc =-12 ; goto end ; }
		if (C!='I' && C!='P') { rc =-13 ; goto end ; }
		s = SkipSpace(s) ; s++ ;

		// Считывание Xp
		n = sscanf(s, "%f", &I.Xp) ; if (n!=1) { rc =-14 ; goto end ; }
		// Перевод из секунд в милисекунды (угловые разумеется)
		I.Xp*= 1e3 ; 
		s = SkipSpace(s) ;
		s = SkipToSpace(s) ;

		// Точность Xp - она не нужна
		n = sscanf(s, "%f", &F) ; if (n!=1) { rc =-15 ; goto end ; }
		s = SkipSpace(s) ;
		s = SkipToSpace(s) ;

		// Считывание Yp
		n = sscanf(s, "%f", &I.Yp) ; if (n!=1) { rc =-16 ; goto end ; }
		// Перевод из секунд в милисекунды (угловые разумеется)
		I.Yp*= 1e3 ; 
		s = SkipSpace(s) ;
		s = SkipToSpace(s) ;

		// Точность Yp - она не нужна
		n = sscanf(s, "%f", &F) ; if (n!=1) { rc =-17 ; goto end ; }
		s = SkipSpace(s) ;
		s = SkipToSpace(s) ;

		// Считывание ключевого символа формата файла
		s = SkipSpace(s) ;
		n = sscanf(s, "%c", &C) ; if (n!=1) { rc =-18 ; goto end ; }
		if (C!='I' && C!='P') { rc =-19 ; goto end ; }
		s = SkipSpace(s) ; s++ ;

		// Считывание отклонения UT1 от UTC
		n = sscanf(s, "%f", &I.UT1mUTC); if (n != 1) { rc = -20; goto end; }
		// Перевод из секунд в милисекунды
		I.UT1mUTC *= 1e3;

		// Записываем данные в двичный файл, если это необходимо и файл открыт
		if (FileBin.is_open()) FileBin.write((const char*)(&I), sizeof(I));

		// Добавляем считанные ПВЗ для даты I.MJD в конец массива		 
		EOP.push_back(I) ;
		// Добавляем значение ключа в индекс
		//EOPIndex.AddValue(I.MJD);

		// Остальные данные даже если они и есть не нужны - символы до
		// конца строки игнорируются

		// Проверка завершения загрузки массива
		if(--N==0) break ;
	}
	}
end: ;
	// Если при загрузке данных произошла ошибка, что может быть скорее всего
	// из-за не правильного формата файла, то просто удаляем содержимое
	// диспетчера ПВЗ
	if (rc) Clear() ;
	else {
		// Время завершения загрузки  
		time_t endTIME = clock();
		// Длительность загрузки в милисекундах
		EopLoadReport.Duration  = (long)(endTIME - startTIME);
		// Источник загрузки IERS
		EopLoadReport.source    = 0;
		EopLoadReport.CountItems= EOP.size();
		EopLoadReport.fromMjd   = (long)EOP[0].MJD;
		EopLoadReport.toMjd     = (long)EOP[EopLoadReport.CountItems - 1].MJD;
	}
	EopLoadReport.ExitCode = rc;

	//Закритие текстового файла, если это необходимо
	if (FileTxt.is_open())	FileTxt.close();
	//Закритие двоичного файла, если это необходимо
	if (FileBin.is_open())	FileBin.close();

	return rc ;
}

//-----------------------------------------------------------------------------
int ZEOPManager::LoadFromBin()
{
	// Время завершения загрузки  
	time_t startTIME = clock();

	unsigned long FaleSize;
	int rc = 0;
	int CountItems, dCI;
	ZEOPItem I;

	// Удаление ранее загруженной информации о ПВЗ
	Clear();
	// Удаление ранее сформированного индекса ПВЗ
	//EOPIndex.Clear();

	// Файл двоичного документа для считывания ПВЗ 
	ifstream FileBin;

	// Предварительно определяется размер файла и колличество записей в нём.
	// Для этого открываем файл с указателем чтения выставленным в конец
	// файла. Считываем текущее значение указателя чтения и тем самым 
	// определяе размер файла.
	// Далее размер файла делим на размер одной записи (структура ZEOPItem).
	// Результат данного деления есть колличество записей в файле.
	FileBin.open(GetBinaryFileName(), ios::ate | ios::in | ios::binary);
	// Открыт ли файл.
	if (FileBin.fail()) { 
		rc =-1; goto end; 
	}		
	// Размер файла в байтах - позиция указателя чтения преобразованная
	// от типа streamoff к типу unsigned long.
	FaleSize = static_cast<unsigned long>(FileBin.tellg());
	// Если файл пустой
	if (!FaleSize) { 
		rc =-2; goto end; 
	}
	// Число записей в файле.
	CountItems = FaleSize / sizeof(ZEOPItem);	
	// Проверка кратноти размера файла размеру одной записи
	dCI = FaleSize % sizeof(ZEOPItem); 
	if (dCI  || CountItems<30) {
		rc =-3; goto end; 
	}

	// Устанавливаем указатель чтения из файла в начало файла.
	FileBin.seekg(0);

	// Резервируем в массиве для хранения ПВЗ N записей
	EOP.reserve(CountItems);

	// Цыкл считывания файла по строкам 
	while (1) {
		// Чтение одной записи
		FileBin.read((char*)(&I), sizeof(I)).eof();
		// Добавляем считанные ПВЗ для даты I.MJD в конец массива		 
		// Мредполагается что координаты полюса заданы в угловых милисекундах,
		// отклонение UTC от UTC1 - в милисекундах.
		if (0) {
			// Необходима проверка считанных данных на корректность
			rc =-4;
			break;
		}
		// Полученная запись помещается в массив.
		EOP.push_back(I);
		// Добавляем значение ключа в индекс
		//EOPIndex.AddValue(I.MJD);
		// Проверка завершения загрузки массива, все ожидаемые массивы
		// загружены
		if (--CountItems == 0) break;
		// Достигнут конец файла
		if (FileBin.eof()) {
			if (CountItems > 0) {
				// Вообще при нормальной загрузке данных и 
				// правильном файле такого быть не должно
				rc =-3;
			}
			break;
	}	}
end:;
	// Если при загрузке данных произошла ошибка, что может быть скорее всего
	// из-за не правильного формата файла, то просто удаляем содержимое
	// диспетчера ПВЗ
	if (rc) Clear();
	else {
		// Время завершения загрузки  
		time_t endTIME = clock();
		// Длительность загрузки в милисекундах
		EopLoadReport.Duration  = (long)(endTIME - startTIME);
		// Источник загрузки двоичное хранилище
		EopLoadReport.source    = 1;
		EopLoadReport.CountItems= EOP.size();
		EopLoadReport.fromMjd   = (long)EOP[0].MJD;
		EopLoadReport.toMjd     = (long)EOP[EopLoadReport.CountItems - 1].MJD;
	}
	EopLoadReport.ExitCode = rc;

	//Закритие двоичного файла, если это необходимо
	if (FileBin.is_open())	FileBin.close();

	return rc;
}

//-----------------------------------------------------------------------------
int ZEOPManager::Load()
{
	// Попытка загрузки из двоичного архива
	int rc = LoadFromBin(); if (!rc) return 0;
	// Если такого архива нет (-1), ошибка открытия файла (-2), 
	// недостаточно данных (-3), неверный формат данных (-4), 
	// то загружаем данные из текстового файла в
	// формате IERS. Одновременно создаём заново двоичный архив данных.
	if (rc ==-1 || rc ==-2 || rc ==-3 || rc ==-4) {
		rc = LoadFromIersFormat(true);
	}
	return rc;
}

//-----------------------------------------------------------------------------
// Построить и сохранить в хранилище множество алгебраических моделей
// по данным ПВЗ загруженным из внешнего источника в виде дискретной функции
// Множество моделей строится для интравала заданного атомным временем.
// Хранилище моделей представляет собой двоичный файл в котором последовательно
// записаны коэффициэнты апрксимирующих полиномов. 
// Ключём для извлечения той или иной полиномиальной модели 
// является MJD начала области определения данной модели
int ZEOPManager::MakeAllEopModels(double fromTa, double toTa)
{
	if (!IsLoadStorage()) {
		EopModelBuildReport.ExitCode =-1;
		return-1;
	}

	// Время завершения построения моделей  
	time_t startTIME = clock();
	memset(&EopModelBuildReport, 0, sizeof(EopModelBuildReport));

	double fromMjd;  ZFTaToMJD(fromTa, &fromMjd);
	double toMjd;    ZFTaToMJD(toTa, &toMjd);
	// Длительность таймфрейма
	double  frame = PxModelHolder.GetTimeFrame();
	// MJD начала таймфрейма
	double  mjdb  = PxModelHolder.FindTimeFrameBeg(fromTa, false);
	// MJD конца таймфрейма
	double  mjde;
	// MJD начала области определения первой созданной модели и
	// конца области определения последней созданной модели
	double  rmjdb = 999e99; 
	double  rmjde =-999e99;
	// Признак завершения цикла построения моделей
	bool    needBreak = false;
	// Размерность буферных массивов 
	int	    M = (int)frame + 2;
	// Буферные массивы: аргумент и значение дискретной функции 
	double* T = new double[M];
	double* Y = new double[M];
	int     rc = 0;

	//-------------------------------------------------------------------------
	while (1) {
		mjde = mjdb + frame;
		// Проверка правой границы диапазона для которой необходимо построить 
		// модели.
		if (mjde > toMjd) {
			mjde = toMjd;
			needBreak = true;
		}

		//---------------------------------------------------------------------
		// Проверка допустимости юлианской даты
		if (mjdb > EopLoadReport.toMjd) {
			// Если данная дата не содержится в хранилище ПВЗ,
			// то построение модели невозможно ==> завершение цикла
			break;
		}
		if (mjde > EopLoadReport.toMjd) {
			mjde = EopLoadReport.toMjd;
			needBreak = true;
		}
		if (mjdb < EopLoadReport.fromMjd) {
			// Если данная дата не содержится в хранилище ПВЗ,
			// то необходимо перейти к первой точке из хранилища ПВЗ
			mjdb = EopLoadReport.fromMjd;
			mjde = mjdb + frame;
			if (mjde > EopLoadReport.toMjd) {
				mjde = EopLoadReport.toMjd;
				needBreak = true;
		}	}

		//---------------------------------------------------------------------
		// Построение очередной полиномиальной модели
		int jn = (int)(mjdb - EopLoadReport.fromMjd);
		int jk = (int)(mjde - EopLoadReport.fromMjd);
		int i, j;
		int N = jk - jn + 1;
		assert(N <=M);
		// Проверка достаточного колличества точек для построения 
		// полиномиальной модели
		if (N < 30) {
			break;
		}
		// Формирование массива аргумента и массива координаты полюса X
		for (j = jn; j <= jk; j++) {
			i = j - jn;
			T[i] = EOP[j].MJD - mjdb;
			Y[i] = EOP[j].Xp;
		}
		// Построение модели для координаты полюса X
		rc = PxModelHolder.BuildNewMod(T, Y, N); if (rc) break;

		// Формирование массива координаты полюса Y
		for (j = jn; j <= jk; j++) {
			i = j - jn;
			Y[i] = EOP[j].Yp;
		}
		// Построение модели для координаты полюса Y
		//rc = PyModelHolder.BuildNewMod(T, Y, N); if (rc) break;

		// Формирование массива разности UT1-UTC
		for (j = jn; j <= jk; j++) {
			i = j - jn;
			Y[i] = EOP[j].UT1mUTC;
		}
		// Построение модели для разности UT1-UTC
		//rc = DTModelHolder.BuildNewMod(T, Y, N); if (rc) break;

		// Увеличиваем счётчик созданных моделей
		EopModelBuildReport.Count++;
		if (rmjdb > mjdb) rmjdb = mjdb;
		if (rmjde < mjde) rmjde = mjde;

		//---------------------------------------------------------------------
		// Завершение цикла или переход к следующему таймфрейму массива ПВЗ
		// и к формированию следующей полиномиальной модели
		if (needBreak) break;
		mjdb = PxModelHolder.FindTimeFrameBeg(mjde, true);
	}

	EopModelBuildReport.ExitCode = rc;
	if (!rc) {
		// Сохранение расчитанных моделей в соответствующие хранилища
		//PxModelHolder.SaveToStorage();
		//PyModelHolder.SaveToStorage();
		//DTModelHolder.SaveToStorage();

		// Время завершения построения моделей  
		time_t endTIME = clock();
		// Длительность загрузки в милисекундах
		EopModelBuildReport.Duration   = (long)(endTIME - startTIME);
		// Заданный интервал на котором необходимо создать серию 
		// моделей
		EopModelBuildReport.fromTaNeed = fromTa;
		EopModelBuildReport.toTaNeed   = toTa;
		// Фактический интервал времени на котором удалось создать непрерывную
		// серию алгебраических моделей
		ZFMJDToTa(rmjdb, &EopModelBuildReport.fromTa);
		ZFMJDToTa(rmjde, &EopModelBuildReport.toTa);
	}

	delete[] T;
	delete[] Y;
	return rc;
}

//-----------------------------------------------------------------------------
char* ZEOPManager::SkipSpace(char* s) 
{
	while (*s==' ') {
		if (*s=='\n' || *s=='\r' || *s=='\0') break ;
		s++ ;
	}
	return s ;
}

//-----------------------------------------------------------------------------
char* ZEOPManager::SkipToSpace(char* s) 
{
	while (*s!=' ') {
		if (*s=='\n' || *s=='\r' || *s=='\0') break ;
		s++ ;
	}
	return s ;
}

//-----------------------------------------------------------------------------
// Проверка загружена ли априорная информация
bool ZEOPManager::IsLoadStorage()
{
	return (EOP.size()?true:false) ;
}

//-----------------------------------------------------------------------------
// Вычисление ПВЗ либо методом интерполяции по загруженным
// апостериорным данным, либо по построенной прогностической модели.
int ZEOPManager::Calc(double t, ZEOPItem& P, TIMESCALE Scale)
{
	int rc = 0;
	rc = PxModelHolder.Calc(t, P.Xp); 
	// Проверка необходимости загрузки табличных ПВЗ и формирование
	// новой локальной модели актуальной для времени t.
	if (rc == 100) {
		// Загрузка табличных ПВЗ
		double *T, *Y;
		int     N;

		// Создание новых локальных моделей
		rc = PxModelHolder.BuildNewMod(T, Y, N); if (rc) return rc;
		rc = PyModelHolder.BuildNewMod(T, Y, N); if (rc) return rc;
		rc = DTModelHolder.BuildNewMod(T, Y, N); if (rc) return rc;

		// Повторная попытка вычисления
		rc = PxModelHolder.Calc(t, P.Xp);
		if (rc) {
			assert(0);
		}
	}
	if (rc) return rc;
	rc = PyModelHolder.Calc(t, P.Yp); if (rc) return rc;
	rc = DTModelHolder.Calc(t, P.UT1mUTC); if (rc) return rc;
	return 0 ;
}

//-----------------------------------------------------------------------------
// Вычисление параметров прогностической модели по загруженным данным.
int ZEOPManager::BuildForecast(double toTime, TIMESCALE Scale)
{

	return 0 ;
}

//-----------------------------------------------------------------------------
// Если априорные данные не загружены, то все 
// три функции GetTimeXXX возвращают нулевое значение
// Время начала даннных ПВЗ - время начала таблицы априорных данных
double ZEOPManager::GetTimeFirst(TIMESCALE Scale)
{
	return (double)TimeTableFirst ;
}

//-----------------------------------------------------------------------------
// Время конца даннных ПВЗ - время до которого построена прогностическая
// модель или время конца таблицы априорных данных, если прогностическая
// модель не построена
double ZEOPManager::GetTimeLast(TIMESCALE Scale)
{
	return (double)(IsForecastBuild ? TimeForecastTo:TimeTableLast) ;
}

//-----------------------------------------------------------------------------
// Время конца таблицы априорных данных
double ZEOPManager::GetTimeTable(TIMESCALE Scale)
{
	return (double)TimeTableLast ;
}

//-----------------------------------------------------------------------------
// Прогнозирование ПВЗ, применение прогностической модели.
int ZEOPManager::Forecast(double t, TIMESCALE Scale) 
{



	return 0 ;
}

//-----------------------------------------------------------------------------