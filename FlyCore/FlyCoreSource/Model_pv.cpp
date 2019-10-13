//////////////////////////////////////////////////////////////////////////////
////    MODEL_PV.C                                                        ////
////    Функция моделирования параметров вращения Земли                   ////
////    Параметры: Time_PVZ - время расчета ПВЗ                           ////
////     Возврат: Xp - координата Xp (Если nullptr - не определяется)        ////
////              Yp - координата Yp (Если nullptr - не определяется)        ////
////              UT - UT1-UTC       (Если nullptr - не определяется)        ////
////    Возвращаемое значение: !=0 - ошибка                               ////
//////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>
#pragma   hdrstop
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
//---------------------------------------------------------------------------
static bool FlagInitPVZ = 0 ; // Флаг инициализации модели ПВЗ

short Model_PVZ( double Time_PVZ, double  *Xp, double *Yp, double *UT )
{
	int ko = 0 ;

	// ПВЗ отключены
	if (Xp != nullptr) *Xp = 0.0;
	if (Yp != nullptr) *Yp = 0.0;
	if (UT != nullptr) *UT = GetLeapSeconds(Time_PVZ);

	/*
	if( !k_Flag_Pvz ) {
		// ПВЗ отключены
		if( Xp != nullptr ) *Xp = 0.0 ;
		if( Yp != nullptr ) *Yp = 0.0 ;
		if( UT != nullptr ) *UT = Get_Delta_UTC_IAT( Time_PVZ );
	}
	// ПВЗ включены
	if( !FlagInitPVZ ) {
		// ПВЗ не проинициализированы

		// Создание класса Параметров Вращения Земли
		FlagInitPVZ = true ;
	}

	// Расчет Параметров Вращения Земли на время
	*/
	return 0 ;
}
//---------------------------------------------------------------------------
