// FlyChebyshov.cpp
#include "stdafx.h"
#include <FlyChebyshov.h>

#pragma message ("    Подключение библиотеки FlyMMath...")
#pragma comment(lib,  "FlyMMath.lib")

#pragma message ("    Подключение библиотеки FlySSP...")
#pragma comment(lib,  "FlySSP.lib")

#pragma comment(user, "Copiled on" __DATE__ "at"  __TIME__)
#pragma comment(user, "Автор А. Николенко")
#pragma comment(compiler)

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
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