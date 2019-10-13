//------------------------------------------------------------------------
// ModPUSwitchOn.cpp
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
// Включение программы управления
//-------------------------------------------------------------------------------
ZVPU::ZVPU( ) 
{
	Clear( ) ;
}

//-------------------------------------------------------------------------------
ZVPU::~ZVPU( ) 
{
	Clear( ) ;
}

//-------------------------------------------------------------------------------
void ZVPU::Clear()
{
	U.clear() ;
	NumDU = 0 ;
}

//-------------------------------------------------------------------------------
bool ZVPU::isEmpty() 
{	return (U.size()?false:true) ; }

//-------------------------------------------------------------------------------
int ZVPU::Size() 
{	return ((int)U.size()) ; }

//-------------------------------------------------------------------------------
double ZVPU::TS() 
{
	int n = (int)U.size() ; if (!n) return 0 ;
	return( U[0].t ) ;
}

//-------------------------------------------------------------------------------
double ZVPU::TE() 
{
	int n = (int)U.size() ; if (!n) return 0 ;
	return( U[n-1].t+U[n-1].dt/k_cbc) ;
}

//-------------------------------------------------------------------------------
void ZVPU::SortItem( ) 
{
	if (!U.size()) return ;
	sort(U.begin(), U.end()) ;
}

//-------------------------------------------------------------------------------
void ZVPU::operator << (ZPUItem &I) { U.push_back(I) ; }
void ZVPU::Add(ZPUItem &I) { U.push_back(I) ; }

//-------------------------------------------------------------------------------
ZPUItem* ZVPU::Add(int* ind) 
{
	ZPUItem I ;
	I_ZPUItem iI ;
	if (!ind || *ind==Size()) {
		U.push_back(I) ; 
		iI = U.end() ; 
	} else {
		if (*ind<0 || *ind>=Size()) return 0 ;
		iI = U.begin() ; advance(iI, *ind) ;
		U.insert(iI, I) ;
	}
	return (&iI[0]) ;
}

//-------------------------------------------------------------------------------
int ZVPU::Set(ZPUItem &I, int ind) 
{
	if (ind<0 || ind>=Size()) {
		AddErrorMSG( "Участок работы ДУ не добавлен, неверный индекс" ) ; return 1 ;
	}
	U[ind] = I ;
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZVPU::Del(int ind, int Count) 
{
	int jn = ind ;
	int jk = ind+Count-1 ;
	jk = jk<Size()?jk:Size()-1 ; if (jk<jn) return 0 ;
	I_ZPUItem iUn = U.begin() ; advance(iUn, jn) ;
    I_ZPUItem iUk = U.begin() ; advance(iUk, jk) ;
    U.erase(iUn, iUk) ;
	return 0 ;
}

//-------------------------------------------------------------------------------
ZPUItem* ZVPU::Get( int ind ) 
{
	if (ind<0 || ind>=Size()) {
		AddErrorMSG( "Неверный индекс участока работы ДУ" ) ; return 0 ;
	}
	ZPUItem* pI = &U[ind] ;
	return pI ;
}

//-------------------------------------------------------------------------------
bool operator < (ZVPU &V1, ZVPU &V2) { return (V1.TS()<V2.TS()?true:false) ; }

//-------------------------------------------------------------------------------