//------------------------------------------------------------------------
// ModPU.cpp
// А. Николенко 24.08.2018
//------------------------------------------------------------------------
#include <stdafx.h>

#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ModPUManager.h>

//-------------------------------------------------------------------------------
// Программа управления
//-------------------------------------------------------------------------------
ZPU::ZPU( ) 
{
	Clear() ;
}

//-------------------------------------------------------------------------------
ZPU::~ZPU( ) 
{
	Clear() ;
}

//-------------------------------------------------------------------------------
void ZPU::Clear() { V.clear() ; NumPU = 0 ; }
bool ZPU::isEmpty() { return (V.size()?false:true) ; }
int  ZPU::Size() { return((int)V.size()) ; }

//-------------------------------------------------------------------------------
double ZPU::TS() 
{
	int n = (int)V.size() ; if (!n) return 0 ;
	return (V[0].TS()) ;
}

//-------------------------------------------------------------------------------
double ZPU::TE() 
{
	int n = (int)V.size() ; if (!n) return 0 ;
	return (V[n-1].TE()) ;
}

//-------------------------------------------------------------------------------
void ZPU::SortItem() 
{
	if (!V.size()) return ;
	sort(V.begin(), V.end()) ;
}

//-------------------------------------------------------------------------------
void ZPU::operator << (const ZVPU &I) { V.push_back(I) ; }
void ZPU::Add(ZVPU &I) { V.push_back(I) ; }

//-------------------------------------------------------------------------------
ZVPU* ZPU::Add(int* ind) 
{
	ZVPU I ;
	I_ZVPU iI ;
	if (!ind || *ind==Size()) {
		V.push_back(I) ; 
		iI = V.end() ; 
	} else {
		if (*ind<0 || *ind>=Size()) return 0 ;
		iI = V.begin() ; advance(iI, *ind) ;
		V.insert(iI, I) ;
	}
	return (&iI[0]) ;
}

//-------------------------------------------------------------------------------
int ZPU::Set(ZVPU &I, int ind) 
{
	if (ind<0 || ind>=Size()) {
		AddErrorMSG( "Включение ДУ не добавлено, неверный индекс" ) ; return 1 ;
	}
	V[ind] = I ;
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZPU::Del(int ind, int Count) 
{
	int jn = ind ;
	int jk = ind+Count-1 ;
	jk = jk<Size()?jk:Size()-1 ; if (jk<jn) return 0 ;
	I_ZVPU iVn = V.begin() ; advance(iVn, jn) ;
    I_ZVPU iVk = V.begin() ; advance(iVk, jk) ;
    V.erase(iVn, iVk) ;
	return 0 ;
}

//-------------------------------------------------------------------------------
ZVPU* ZPU::Get(int ind) 
{
	if (ind<0 || ind>=Size()) {
		AddErrorMSG( "Неверный индекс Включения ДУ" ) ; return 0 ;
	}
	ZVPU* pI = &V[ind] ;
	return pI ;
}

//-------------------------------------------------------------------------------