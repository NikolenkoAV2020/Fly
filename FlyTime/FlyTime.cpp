// FlyTime.cpp : 
#include "stdafx.h"

#include <FlySofa.h>
#include <FlyTime.h>

//---------------------------------------------------------------------------
//#pragma message ("    ����������� ���������� FlyMMath...")
//#pragma comment(lib, "FlyMMath.lib")
#pragma message ("    ����������� ���������� FlySofa...")
#pragma comment(lib, "FlySofa.lib")
#pragma message ("    ����������� ���������� FlyChebyshov...")
#pragma comment(lib, "FlyChebyshov.lib")

#pragma comment(user, "Copiled on" __DATE__ "at"  __TIME__)
#pragma comment(user, "����� �. ���������")
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

