// FlyMagicLNS.cpp
#include <stdafx.h>
#include <FlyMagicLNS.h>

//---------------------------------------------------------------------------
#pragma message ("    ����������� ���������� FlyMMath...")
#pragma comment(lib, "FlyMMath.lib")

#pragma message ("    ����������� ���������� FlyCore...")
#pragma comment(lib, "FlyCore.lib")

#pragma message ("    ����������� ���������� FlyTime...")
#pragma comment(lib, "FlyTime.lib")

#pragma message ("    ����������� ���������� FlyFormat...")
#pragma comment(lib, "FlyFormat.lib")

#pragma comment(user, "Copiled on" __DATE__ "at"  __TIME__)
#pragma comment(user, "����� �. ���������")
#pragma comment(compiler)
//---------------------------------------------------------------------------

FLYMAGICLNS_API int nqwe2 = 0;

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
