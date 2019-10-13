//-----------------------------------------------------------
//	ATANK.CPP
//      РАСЧЕТ КРУГОВОГО АРКТАНГЕНСА
//      Автор	: Мещеряков В.М.,Махненко Ю.Ю.,Пасынков В.В.
//	Версия 	: 01.00 / 23.08.1995 /
//----------------------------------------------------------
//#include <stdio.h>
#include <math.h>
#include "ssp.h"

double atana(double sina, double cosa)
{
	double a = atan2(sina, cosa) ;
	if(a<0.0) return(a+M_PI*2.0) ;
	return a ;
}

double atank(double argtan,double signcos)
{
	double a = atan(argtan) ;
	if(signcos<0.0) return(a+M_PI) ;
	if(a<0.0)       return(a+M_PI*2.0) ;
	return a ;
}

