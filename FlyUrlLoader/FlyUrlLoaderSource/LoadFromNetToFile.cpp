// LoadFromNetToFile.cpp
// �. ��������� 03.05.2019
//
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyUrlLoader.h>
#include <FlyUrlLoaderSource\\URLDownloadCallBack.h>

//-----------------------------------------------------------------------------
// �������� ������ �� ������ ������ �� ���������� ����. 
// ������������ ��������:
//	0 - �������� ���������� ��������
//	1 - ������� ����� ������� ��� ���������� ������������ �����.
//	2 - �������� � �������.
//	3 - ����������� ������ ��������.
//	4 - ������ �� ����������� ������ ������ ����������.
//	5 - ������ �� ����������� ������ ������ �����������.
//	6 - ����������� �������� ����������.
//	7 - �������� ����� ������ ������.
//	8 - ����������� ��������..
//	9 - ������ ���������� �� ����� �������� ������.
//	10- ���������� ��������� ������.
//	11- ��������� ��������������.
//	12- ��������� ���� �� ������.
//	
int LoadFromNetToFile(
	char* URL,			// ������ ������ �� ����������� ������� � ���������� ���� 
	char* toFile,		// ��� ������������ ���������� ����� 
	char* toDir,		// ������� � ������� ����������� ����������� ����
	bool  allowCashe,	// ���������� �������� �� ����
	string* errorMsg,	// ��������� ��������� �� ������ 
	unsigned int ID,	// ID ��������
	int(*pCallBackLFN)(unsigned int ID, float p))
{
	unsigned long rc = 0;
	string nameFile = "";

	// ������������� COM
	if (CoInitialize(NULL) != S_OK) {
		if (errorMsg) *errorMsg = "����������� ������ ��������.";
		return 3;
	}
	CUrlDownloadCallBack CBindStatusCallBack(ID, pCallBackLFN);

	// �������� ������� ���������� � Internet
	{
		DWORD dwFlags;
		rc = InternetGetConnectedState(&dwFlags, 0);
		if (!rc) { rc = 6; goto end; }
	}

	// ��� ������������� ��������� ������ �� ����
	if (!allowCashe) {
		if (!DeleteUrlCacheEntry(URL)) {
			rc = GetLastError();
			// � ���� ������� ����� ��� - ��� �� ������
			if (rc == ERROR_FILE_NOT_FOUND)	rc = 0;
			// �������� � �������
			if (rc == ERROR_ACCESS_DENIED) { rc = 2; goto end; }
			// ��� ������ ������
			if (rc) { rc = 3; goto end; }
	}	}	

	{
		// ������������ ������� ����� ������������ �����
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

		// ��������������� �������� ����� �� ���� ��
		// ������ ������
		rc = URLDownloadToFile(0, URL, nameFile.c_str(),
			// ��. http://support.microsoft.com/kb/196466
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
		//������� ��������� �������
	}
	if (CBindStatusCallBack.isWasBreak()) { rc = 13; goto end; }

	// �������� �������� ���������� �����... 
	if (GetFileAttributes(nameFile.c_str()) == INVALID_FILE_ATTRIBUTES) {
		rc = 12;
	}

end:;
	if (rc && errorMsg) {
		switch (rc) {
			case 1:
				*errorMsg = "�������� ������� ���������� ������������ �����.";
				break;
			case 2:
				*errorMsg = "�������� � �������.";
				break;
			case 3:
				*errorMsg = "����������� ������ ��������.";
				break;
			case 4:
				*errorMsg = "������ �� ����������� ������ ������ ����������.";
				break;
			case 5:
				*errorMsg = "������ �� ����������� ������ ������ �����������.";
				break;
			case 6:
				*errorMsg = "����������� �������� ����������.";
				break;
			case 7:
				*errorMsg = "�������� ����� ������ ������.";
				break;
			case 8:
				*errorMsg = "����������� ��������.";
				break;
			case 9:
				*errorMsg = "������ ���������� �� ����� �������� ������.";
				break;
			case 10:
				*errorMsg = "���������� ��������� ������.";
				break;
			case 11:
				*errorMsg = "��������� ��������������.";
				break;
			case 12:
				*errorMsg = "��������� ���� �� ������.";
				break;
			case 13:
				*errorMsg = "�������� ���� ��������. ";
				char S[64];
				sprintf(S, "��������� %.2lf%s.\0", 
				CBindStatusCallBack.WhereWasBreak(), "%");
				*errorMsg += S;
				break;
	}	}

	CBindStatusCallBack.Release();
	CoUninitialize();
	return rc;
}

//-----------------------------------------------------------------------------