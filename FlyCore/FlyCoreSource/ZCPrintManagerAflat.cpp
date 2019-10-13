//-------------------------------------------------------------------------------
// ZCPrintManagerAflat.cpp
// А. Николенко 03.08.2018
// Горизонтальное документирование результатов прогноза
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>

#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZCPrintManager.h>
#include <FlyCoreSource\\ZCPrintFormat.h>

//-----------------------------------------------------------------------------
int ZCPrintManager::PrintAflatMSC(ZMSC* pKA, ZSC_SI* FS, ZSC_VI* FV, FILE* fp)
{
	// Вывод информации в текстовом формате
	PrintAflatMSCtext(pKA, FS, FV, fp);
	// Вывод информации в формате html
	PrintAflatMSChtml(pKA, FS, FV);
	return 0;
}

//-----------------------------------------------------------------------------