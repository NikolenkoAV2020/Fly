//-------------------------------------------------------------------------------
// ZCAproxMod.h : 
// Диспетчер апроксимирующих моделей
//
// А. Николенко 15.03.2019
//-------------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyChebyshovSource\\ZCAproxModManager.h>

ZCAproxModManager AproxMng;

//-------------------------------------------------------------------------------
ZCAproxModManager::ZCAproxModManager()
{
}


ZCAproxModManager::~ZCAproxModManager()
{
}

//-------------------------------------------------------------------------------
int ZCAproxModManager::InitDefault(double t)
{
	return 0;
}

//-------------------------------------------------------------------------------
int ZCAproxModManager::Load(double fromTime, double toTime)
{
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZCAproxModManager::Save()
{
	return 0;
}

