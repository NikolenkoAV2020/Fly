// FlyCore.cpp
#include <stdafx.h>

#include <FlyCore.h>

//---------------------------------------------------------------------------
#pragma message ("    Подключение библиотеки FlyMMath...")
#pragma comment(lib, "FlyMMath.lib")
#pragma message ("    Подключение библиотеки FlySSP...")
#pragma comment(lib, "FlySSP.lib")
#pragma message ("    Подключение библиотеки FlySofa...")
#pragma comment(lib, "FlySofa.lib")
#pragma message ("    Подключение библиотеки FlyChebyshov...")
#pragma comment(lib, "FlyChebyshov.lib")
#pragma message ("    Подключение библиотеки FlyTime...")
#pragma comment(lib, "FlyTime.lib")
#pragma message ("    Подключение библиотеки FlyFormat...")
#pragma comment(lib, "FlyFormat.lib")


#pragma comment(user, "Copiled on" __DATE__ "at"  __TIME__)
#pragma comment(user, "Автор А. Николенко")
#pragma comment(compiler)

//---------------------------------------------------------------------------
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

