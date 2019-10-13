// FlyFormat.cpp

#include "stdafx.h"
#include "FlyFormat.h"

#pragma message ("    Подключение библиотеки FlyTime...")
#pragma comment(lib, "FlyTime.lib")


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
