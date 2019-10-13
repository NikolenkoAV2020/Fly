// LoadFromNetToFile.cpp
// А. Николенко 03.05.2019
//
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyUrlLoader.h>
#include <FlyUrlLoaderSource\\URLDownloadCallBack.h>

//-----------------------------------------------------------------------------
// Загрузка данных по прямой ссылке из глобальной сети. 
// Возвращаемое значение:
//	0 - Успешное выполнение загрузки
//	1 - Неверно задан каталог для сохранения загруженного файла.
//	2 - Отказано в доступе.
//	3 - Неизвестная ошибка загрузки.
//	4 - Данные по указанномой прямой ссылке недоступны.
//	5 - Данные по указанномой прямой ссылке отсутствуют.
//	6 - Отсутствует интернет соединение.
//	7 - Неверный адрес прямой ссылки.
//	8 - Неизвестный протокол..
//	9 - Разрыв соединения во время загрузки данных.
//	10- Невозможно загрузить данные.
//	11- Требуется аутентификация.
//	12- Локальный файл не создан.
//	
int LoadFromNetToFile(
	char* URL,			// Прямая ссылка на загружаемые даанные в глобальной сети 
	char* toFile,		// Имя загруженного локального файла 
	char* toDir,		// Каталог в котором формируется загружаемый файл
	bool  allowCashe,	// Разрешение загрузки из кэша
	string* errorMsg,	// Текстовое сообщение об ошибке 
	unsigned int ID,	// ID загрузки
	int(*pCallBackLFN)(unsigned int ID, float p))
{
	unsigned long rc = 0;
	string nameFile = "";

	// Инициализация COM
	if (CoInitialize(NULL) != S_OK) {
		if (errorMsg) *errorMsg = "Неизвестная ошибка загрузки.";
		return 3;
	}
	CUrlDownloadCallBack CBindStatusCallBack(ID, pCallBackLFN);

	// Проверка наличия соединения с Internet
	{
		DWORD dwFlags;
		rc = InternetGetConnectedState(&dwFlags, 0);
		if (!rc) { rc = 6; goto end; }
	}

	// При необходимости удаляются данные из кеша
	if (!allowCashe) {
		if (!DeleteUrlCacheEntry(URL)) {
			rc = GetLastError();
			// В кэше данного файла нет - это не ошибка
			if (rc == ERROR_FILE_NOT_FOUND)	rc = 0;
			// Отказано в доступе
			if (rc == ERROR_ACCESS_DENIED) { rc = 2; goto end; }
			// Все другие ошибки
			if (rc) { rc = 3; goto end; }
	}	}	

	{
		// Формирование полного имени загружаемого файла
		if (toDir) {
			if (mkdir(toDir) < 0) {
				if (errno != EEXIST) {
					rc = 1; goto end;
				}
			}
			nameFile = toDir;
			nameFile += "\\";
		}
		nameFile += toFile;

		// Непосредственно загрузка файла из сети по
		// прямой ссылке
		rc = URLDownloadToFile(0, URL, nameFile.c_str(),
			// см. http://support.microsoft.com/kb/196466
			BINDF_GETNEWESTVERSION,
			(LPBINDSTATUSCALLBACK)&CBindStatusCallBack);
	}
	if (rc == INET_E_DOWNLOAD_FAILURE  ) { rc = 3; goto end; }
	if (rc == INET_E_DATA_NOT_AVAILABLE) { rc = 4; goto end; }
	if (rc == INET_E_RESOURCE_NOT_FOUND) { rc = 5; goto end; }
	if (rc == INET_E_OBJECT_NOT_FOUND  ) { rc = 5; goto end; }
	if (rc == INET_E_CANNOT_CONNECT    ) { rc = 6; goto end; }
	if (rc == INET_E_INVALID_URL       ) { rc = 7; goto end; }
	if (rc == INET_E_UNKNOWN_PROTOCOL  ) { rc = 8; goto end; }
	if (rc == INET_E_TERMINATED_BIND   ) { rc = 9; goto end; }
	if (rc == INET_E_CANNOT_LOAD_DATA  ) { rc = 10; goto end; }
	if (rc == INET_E_AUTHENTICATION_REQUIRED) { rc = 11; goto end; }
	if (rc == S_OK) {
		//Закачка выполнена успешна
	}
	if (CBindStatusCallBack.isWasBreak()) { rc = 13; goto end; }

	// Проверка создания локального файла... 
	if (GetFileAttributes(nameFile.c_str()) == INVALID_FILE_ATTRIBUTES) {
		rc = 12;
	}

end:;
	if (rc && errorMsg) {
		switch (rc) {
			case 1:
				*errorMsg = "Неверный каталог сохранения загруженного файла.";
				break;
			case 2:
				*errorMsg = "Отказано в доступе.";
				break;
			case 3:
				*errorMsg = "Неизвестная ошибка загрузки.";
				break;
			case 4:
				*errorMsg = "Данные по указанномой прямой ссылке недоступны.";
				break;
			case 5:
				*errorMsg = "Данные по указанномой прямой ссылке отсутствуют.";
				break;
			case 6:
				*errorMsg = "Отсутствует интернет соединение.";
				break;
			case 7:
				*errorMsg = "Неверный адрес прямой ссылки.";
				break;
			case 8:
				*errorMsg = "Неизвестный протокол.";
				break;
			case 9:
				*errorMsg = "Разрыв соединения во время загрузки данных.";
				break;
			case 10:
				*errorMsg = "Невозможно загрузить данные.";
				break;
			case 11:
				*errorMsg = "Требуется аутентификация.";
				break;
			case 12:
				*errorMsg = "Локальный файл не создан.";
				break;
			case 13:
				*errorMsg = "Загрузка была прервана. ";
				char S[64];
				sprintf(S, "Загружено %.2lf%s.\0", 
				CBindStatusCallBack.WhereWasBreak(), "%");
				*errorMsg += S;
				break;
	}	}

	CBindStatusCallBack.Release();
	CoUninitialize();
	return rc;
}

//-----------------------------------------------------------------------------