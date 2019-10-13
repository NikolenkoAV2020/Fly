// FlyUrlLoader.cpp 
// �. ��������� 03.05.2019
//
// FlyUrlLoader - ����������� ���������, ����������� ������������ ������� -
// �������� �� ���������� ���� Internet ������ �� ��������� URL.
// ��� ������� ������ ������������ API Windows, �� ���� ��������� FlyUrlLoader
// �������� ��������� �� ����������� �������. 
// ������� ������������ ���� ���� ������������ �� OpenSource ����������.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyUrlLoader.h>
#include <FlyUrlLoaderSource\\URLDownloadCallBack.h>

#pragma message ("    ����������� ���������� Urlmon...")
#pragma comment (lib, "Urlmon.lib") 
#pragma message ("    ����������� ���������� Wininet...")
#pragma comment (lib, "Wininet.lib") 

#pragma comment(user, "Copiled on" __DATE__ "at"  __TIME__)
#pragma comment(user, "����� �. ���������")
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