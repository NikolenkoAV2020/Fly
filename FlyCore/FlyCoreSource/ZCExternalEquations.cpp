//-------------------------------------------------------------------------------
// ZCExternalEquations.cpp
// А. Николенко 22.10.2018
// 
// Реализация класса, посредством которого можно в существующую модель движения
// на этапе выполнения программы добавить дополнительные (внешние) уравнения.
// Данные внешние уравнения будут интегрироваться вместе с основными уравнениями
// движения КА, в качестве которых выступают дифференциальные уравнения для
// координат КА (x, y, z), его скоростей (Vx, Vy, Vz) и массы КА.
//
// Расширяя таким образом "вертикально" СДУ движения можно одновременно с 
// уравнениями движения центра масс интегрировать и уравнения вращения КА,
// либо интегрируя ту или иную переменную получать на некотором интервалее
// её среднеинтегральное значение.
// 
// Для увеличения числа интегрируемых уравнений необходимо перегрузить виртуальную
// функцию ExternalSph.
//
// Кроме изложенного, класс позволяет на этапе выполнения программы
// расширять модель движения "горизонтально" - добавлять учёт дополнительных 
// факторов, возмущающих движение КА. 
// Например моделировать работу той или иной двигательной установки, скажем... 
// двигателя малой тяги, характеристики которого не укладываются в рамки 
// реализованного в модели движения алгоритма учёта реактивного ускорения.
//
// Класс может оказаться очень полезным для написания исследовательского ПО
// или ПО моделирующего полёт некоторого перспективного изделия
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>
#include <FlyCoreSource\\ZCExternalEquations.h>

//-------------------------------------------------------------------------------
void operator << (ZCExternalEquationsDefinition& EED, unsigned int itemID) 
{
	EED.ID.push_back(itemID) ;
}

//-------------------------------------------------------------------------------
ZCExternalEquations::ZCExternalEquations() 
{
	CountExtEquations = 0 ;
	IsOn = false ;
	Xexe = nullptr ;
	Yexe = nullptr ;
	NUexe= nullptr ;
}

ZCExternalEquations::~ZCExternalEquations() 
{
	if (NUexe) delete[] NUexe ; NUexe = nullptr ;
}

int ZCExternalEquations::ExternalSph(double t, double* mainX, double* X, double* Y) 
{ 
	return 0 ; 
}   

int ZCExternalEquations::ExternalForces(double t, double* X, double* Y, int N) 
{ 
	return 0 ; 
}   

void ZCExternalEquations::ExternalEquationsOn (double* NU) 
{ 
	if (CountExtEquations<=0) {
		IsOn = false ; 
		return ;
	}
	IsOn = true ; 
	if (NUexe && CountExtEquations) {
		if (NU)	memcpy(NUexe, NU, CountExtEquations*sizeof(double)) ; 
		else memset(NUexe, 0, CountExtEquations*sizeof(double)) ; 
	}	
}

void ZCExternalEquations::ExternalEquationsOff(double* X) 
{ 
	if (X&&Xexe) memcpy(X, Xexe, GetExternalEquationsCount()*sizeof(double)) ;
	IsOn = false ; 
}

bool ZCExternalEquations::IsExtEqOn() 
{ 
	return IsOn ; 
}

void ZCExternalEquations::SetExternalEquationsCount(int CountExt) 
{ 
	assert(CountExt<=EXTEQ_MAXCOUNT) ;
	CountExtEquations = CountExt ;
	if (NUexe) delete[] NUexe ;
	if (CountExt>0) { 
		NUexe = new double[CountExt] ;
		memset(NUexe, 0, CountExt*sizeof(double)) ;
	} else ExternalEquationsOff() ;
}

int ZCExternalEquations::GetExternalEquationsCount() 
{ 
	return(IsOn ? CountExtEquations:0) ; 
}

//-------------------------------------------------------------------------------

