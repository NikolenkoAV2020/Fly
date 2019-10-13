//-------------------------------------------------------------------------------
// ZCPrintManagerAflat.cpp
// �. ��������� 03.08.2018
// �������������� ���������������� ����������� ��������
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>

#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZCPrintManager.h>
#include <FlyCoreSource\\ZCPrintFormat.h>

//-----------------------------------------------------------------------------
int ZCPrintManager::PrintAflatMSC(ZMSC* pKA, ZSC_SI* FS, ZSC_VI* FV, FILE* fp)
{
	// ����� ���������� � ��������� �������
	PrintAflatMSCtext(pKA, FS, FV, fp);
	// ����� ���������� � ������� html
	PrintAflatMSChtml(pKA, FS, FV);
	return 0;
}

//-----------------------------------------------------------------------------