//-----------------------------------------------------------------------------
// ZFUofNU01.h
// �. ��������� 15.03.2019
//-----------------------------------------------------------------------------
#ifndef _Z_UOF_NU01_H
#define _Z_UOF_NU01_H

// ��������� �� �� ��� ��01
FLYCORE_API int LoadNU01(
			char* shortFileName,			// �������� ��� �����, ��� ����, 
			FILE* pf,						// ���� �������� ����� � ��,
			char* Text,						// ���� ��������� ������ � ��.
			ZNU& NU,						// �� ������� ��������� 
			vector<string>* rows=nullptr,	// ��������� ����� ���
			bool PrintToCout=false);		// ������� ����������������
											// ����������� �������� � ��������
// ��������� �� ��� ��� ��01
FLYCORE_API int SaveNU01(ZNU& NU, ZLSF* lhs,	// ����������� �� � ����� ���
						 char* shortFileName,	// �������� ��� �����, 
						 FILE* pf = nullptr,	// ���� �������� ����,
						 char* Text = nullptr);	// ���� ��������� ������.

//-----------------------------------------------------------------------------

#endif // #ifndef _Z_UOF_NU01_H