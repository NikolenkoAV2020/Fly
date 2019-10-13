//-----------------------------------------------------------------------
// TheUT1UTC.cpp
// А. Николенко 23.11.2018
// 
// Моделирование отклонения UTC от UT1
//-----------------------------------------------------------------------
#include "stdafx.h"
#include <FlyTimeSource\\TheUT1UTC.h>

//---------------------------------------------------------------------------
TheUT1UTC::TheUT1UTC() 
{
	isLoad    = false ;
	BMJD      =-1 ;
	EMJD      =-1 ;
	lastMJD   =-1 ;
	lastIndex =-2 ;
	CountItems= 1 ;
	VDltUTC.clear() ;
}

//---------------------------------------------------------------------------
TheUT1UTC::~TheUT1UTC()
{
	VDltUTC.clear() ;
}

//---------------------------------------------------------------------------
// Первоначальная загрузка
int TheUT1UTC::Load()
{
	VDltUTC.clear() ;
	isLoad    = false ;
	BMJD      =-1 ;
	EMJD      =-1 ;
	lastMJD   =-1 ;
	lastIndex =-2 ;
	CountItems= 0 ;

	UTCAmendmentItem I ;
	double eps ;

	FILE* pf = fopen("ModInfo\\UT1MUTCMJD.txt", "r") ; if (!pf) return-2 ;
	while(1) {
		int rc = fscanf( pf, "%lf %lf %lf\n", &I.mjd, &I.DUT1UTC, &eps ) ;
		if (rc!=3) break ;
		VDltUTC.push_back(I) ;
		if (!CountItems) BMJD = I.mjd ;
		CountItems++ ;
	}
	fclose(pf) ; pf = 0 ;
	if (CountItems)	{
		EMJD = I.mjd ;
		isLoad = true ;
	}
	else return-1 ;
	return 0 ;
}

//---------------------------------------------------------------------------
// Вычисление разницы между UTC и UT1
int TheUT1UTC::Amendment(TheDblTime& UTC, double& DUT1)
{
	if (!isLoad) return-2 ;
	if (CountItems<2) return-3 ;	
	
	int    i =-1 ;
	double M = UTC.days+UTC.sec/86400.0 ;
	double dj = M-BMJD ;
	if (dj>0) {
		int j = (int)dj;
		if (j<CountItems-1) {
			lastIndex = j ;
			if (M>=VDltUTC[j].mjd && M<VDltUTC[j+1].mjd) {
				CalcFromIndex(M, j, DUT1) ;
				return 0 ;
			}
		} else {
			lastIndex = CountItems-1 ;
			DUT1 = VDltUTC[CountItems-1].DUT1UTC ;
			return 0 ;
		}
	} else {
		lastIndex =-1 ;
		DUT1 = VDltUTC[0].DUT1UTC ;
		return 0 ;
	}

	if (lastIndex>=0) {
		// Если предыдущее значение было найдено в диапазоне с индексом lastIndex 
		i = lastIndex ;
		if (M>=VDltUTC[i].mjd && M<VDltUTC[i+1].mjd) {
			// M попадает в тот же диапазон
			CalcFromIndex(M, i, DUT1) ;
			return 0 ;
		} else {
			if (M>VDltUTC[i+1].mjd) {
				// Поиск нового диапазона вперёд
				for(i=i+1 ; i<CountItems-1 ; i++) { if (VDltUTC[i+1].mjd>M) break ; }
			} else {
				// Поиск нового диапазона назад
				for(i=i-1 ; i>=0 ; i--) { if (VDltUTC[i].mjd<M) break ; }
			}
		}
	} else {
		if (lastIndex==-1) {
			// Если предыдущее значение M раньше самого первого диапазона
			if (M<VDltUTC[0].mjd) {
				// Там же! новое значение M тоже раньше самого первого диапазона
				i =-1 ;
			} else {
				// Поиск нового диапазона вперёд от самого первого
				for(i=0 ; i<CountItems-1 ; i++) { if (VDltUTC[i+1].mjd>M) break ; }
			}
		} else {
			// Если предыдущего значения вообще не было (lastIndex==-2) 
			// то поиск нового диапазона от последнего назад
			for(i=CountItems-1 ; i>=0 ; i--) { if (VDltUTC[i].mjd<M) break ; }
		}
	}

	lastIndex = i ;
	lastMJD   = i>=0 ? VDltUTC[i].mjd : 0 ;

	if (i==CountItems-1) {
		// Вне диапазона таблицы ===> значение приближённое
		DUT1 = VDltUTC[i].DUT1UTC ;
		return 0 ;
	} else {
		if (i==-1) {
			// Вне диапазона таблицы ===> значение приближённое
			DUT1 = VDltUTC[0].DUT1UTC ;
		} else {
			// Вычисление внутри диапазона
			CalcFromIndex(M, i, DUT1) ;
	}	}	
	return 0 ;
}

//---------------------------------------------------------------------------
void TheUT1UTC::CalcFromIndex(double MJD, int I, double& DUT1) 
{
	double K = (VDltUTC[I+1].DUT1UTC-VDltUTC[I].DUT1UTC) / 
	           (VDltUTC[I+1].mjd-VDltUTC[I].mjd) ;
	DUT1 = VDltUTC[I].DUT1UTC+K*(MJD-VDltUTC[I].mjd) ;
}

//---------------------------------------------------------------------------