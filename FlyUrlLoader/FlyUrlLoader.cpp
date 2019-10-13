// FlyUrlLoader.cpp 
// А. Николенко 03.05.2019
//
// FlyUrlLoader - программный компонент, реализующий единственную функцию -
// загрузки из глобальной сети Internet файлов по заданному URL.
// Для решения задачи используются API Windows, то есть компонент FlyUrlLoader
// является зависимым от опреционной системы. 
// Основна программного кода была заимствована из OpenSource источников.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyUrlLoader.h>
#include <FlyUrlLoaderSource\\URLDownloadCallBack.h>

#pragma message ("    Подключение библиотеки Urlmon...")
#pragma comment (lib, "Urlmon.lib") 
#pragma message ("    Подключение библиотеки Wininet...")
#pragma comment (lib, "Wininet.lib") 

#pragma comment(user, "Copiled on" __DATE__ "at"  __TIME__)
#pragma comment(user, "Автор А. Николенко")
#pragma comment(compiler)

//-----------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------