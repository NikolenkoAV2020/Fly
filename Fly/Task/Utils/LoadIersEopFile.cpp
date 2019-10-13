//-----------------------------------------------------------------------------
// LoadIersEopFile.cpp : 
// А. Николенко 04.04.2019
//
// Прикладная программа загрузки с сайта IERS данных о ПВЗ
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>		// Собственоно сама модель движения
#include <FlyMMath.h>		// Математические функции
#include <FlySofa.h>		// Функции фундаментальной астрономии
#include <FlyChebyshov.h>	// Апроксимация дискретной функции полиномами
							// Чебышёва
#include <FlyUrlLoader.h>	// 

//-----------------------------------------------------------------------------
int CallBackLoadFromNet(unsigned int ID, float p)
{
	cout << "\r     " << p << "%    ";
	return 0;
}

//-----------------------------------------------------------------------------
int LoadIersEopFile(
	// Имя локального файла с ПВЗ в формате IERS. Если задано (не nullptr), то
	// параметры duration, for2000, updateStorege не имеют какого-либо значения.
	// Хранилище ПВЗ обновляется данными из файла IersFileName.
	const char* IersFileName,
	// Период за который с сайта IERS загружаются данные
	//	0 - только последние данные (текущий год)
	//	1 - средний интервал (начиная с 1992 года)
	//	2 - весь интервал (начиная с 1973 года)
	char duration = 0, 
	// Модель прецесси-нутации относительно которой задаются ПВЗ
	//	0 - модель 1980 года
	//	1 - модель 2000 года
	bool for2000 = true,
	// Признак необходимости обновления данных бинарного хранилища ПВЗ
	bool updateStorege = false,
	// Признак необходимости формирования данных апостериорной оценки 
	// точности прогноза ПВЗ IERS. Имеет смысл только если
	// updateStorege = true. 
	bool updateApEst = true,
	// Признак обновления хранилища локальных моделей.
	bool updateLocMod  = false)
{
	int rc;
	string error;

	//ZCEopManager M1;
	//rc = M1.LoadFromIersFormat("IERS_EOP2000A.txt",
	//	true, true, updateLocMod);
	// return 0;

	// Возиожные прямые ссылки на ПВЗ
	char FtpSource[6][128] = {
		"https://datacenter.iers.org/data/latestVersion/12_FINALS.DAILY_IAU1980_V2013_0112.txt",
		"https://datacenter.iers.org/data/latestVersion/8_FINALS.DATA_IAU1980_V2013_018.txt",
	    "https://datacenter.iers.org/data/latestVersion/7_FINALS.ALL_IAU1980_V2013_017.txt",
		"ftp://ftp.iers.org/products/eop/rapid/daily/finals2000A.daily",
		"ftp://ftp.iers.org/products/eop/rapid/standard/finals2000A.data",
		"ftp://ftp.iers.org/products/eop/rapid/standard/finals2000A.all"};

	// Имя локального файла с данными ПВЗ полученными из IERS
	string localFileName;

	if (!IersFileName) {
		// Выбор прямой ссылки, исходя из того что задано для загрузки
		int fi = duration + 3 * for2000;
		char* sourceFileName = FtpSource[fi];

		//---------------------------------------------------------------------
		// Формирование имени, формируемого по результатам скачивания, 
		// локального файла ПВЗ
		localFileName = "IERS_EOP_";
		// Дата создания данного файла
		char SDate[16];
		time_t rawtime; time(&rawtime);
		struct tm *ptm = gmtime(&rawtime);
		ptm->tm_year -= 100;
		sprintf(SDate, "%.2d%.2d%.2d", ptm->tm_year, 
				ptm->tm_mon + 1, ptm->tm_mday);
		localFileName += SDate;
		// Тип данных (последние, сокращённые или полные)
		switch (duration) {
		case 0: localFileName += "_DAILY"; break;
		case 1: localFileName += "_DATA"; break;
		case 2: localFileName += "_ALL"; break;
		}
		// Файл имеет формат текста
		localFileName += ".txt";

		//---------------------------------------------------------------------
		cout << sourceFileName << endl;
		rc = LoadFromNetToFile(sourceFileName, (char*)localFileName.c_str(),
							   DIRIERSEOP, false, &error, 1, CallBackLoadFromNet);
		if (rc) {
			cout << "данные не загружены." << endl << error;
		} else {
			cout << "данные загружены.";
		}
		cout << endl << endl;
		if (rc) return rc;
	} else {
		updateStorege = true;
		localFileName = IersFileName;
	}

	//-------------------------------------------------------------------------
	if (!updateStorege) return 0;
	rc = CEopMng.LoadFromIersFormat((char*)localFileName.c_str(),
									updateApEst, true, updateLocMod);
	return rc;
}

//-----------------------------------------------------------------------------


