//-----------------------------------------------------------------------------
// Fly.cpp : 
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции
#include <FlySofa.h>		// Функции фундаментальной астрономии
#include <FlySsp.h>			// Численные алгоритмы "пакета SSP"
#include <FlyChebyshov.h>	// Апроксимация дискретной функции полиномами
							// Чебышёва
#include <FlyCore.h>		// Собственоно сама модель движения
#include <FlyCoreLink.h>	// Подключение всех необходимых библиотек
#include <FlyMagicLNS.h>	// Метод узловых точек

#include <FlyUrlLoader.h>		// 
#include <FlyUrlLoaderLink.h>	//

//-----------------------------------------------------------------------------
// Примеры программ использующих модель движения КА
//-----------------------------------------------------------------------------
int TaskFlightTrack();			// Трасса полёта
int TaskCalcZRV();				// Расчёт ЗРВ и ЦУ для НКУ
int TaskTimePrediction();		// Прогноз с заданным шагом по времени
int TaskTurnsPredictionSimpl();	// Самый простой прогноз по виткам
int TaskTurnsPredictionGroup();	// Прогноз по виткам группы из 8-и КА
int TaskTurnsPredictionHAvr();	// Прогнозирование по виткам с вычислением 
								// осреднённых на витке элементов движения
//-----------------------------------------------------------------------------
// Расчёт параметров орбитального маневрирования для 1-3 импульсных
// манёвров методом узловых точек для целевых орбит различного вида
// SPO_ORB_OMIV
// SPO_ORB_DU
// SPO_ORB_EL
// SPO_ORB_HBH
// Функции по известным вариантам: 11, 12, 13, ... 20, 21, ..., 30, 31, 32
//-----------------------------------------------------------------------------
int TaskMagicFly11 (int TypeOrb);
int TaskMagicFly21 (int TypeOrb);
int TaskMagicFly24 (int TypeOrb);
int TaskMagicFly241(int TypeOrb);
int TaskMagicFly31 (int TypeOrb);
int TaskMagicFly311(int TypeOrb);

int TaskMagicFlySys3SC(int TypeOrb);

//-----------------------------------------------------------------------------
// Сверки
int MatchingPrediction();
int TestAgeValidation();

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// Вывод кирилицы в консольном приложении 
	//setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// Инициализация глобальных переменных
	// Необязательная операция, так как данная функция вызывается
	// при создании объекта vодели движения КА (ZMSC)
	FlyGlobalInit();

	// Самый простой прогноз по виткам
	//TaskTurnsPredictionSimpl();
	//return 0;

	ZNU NU;
	//LoadNU01("NU01_prognoz_000.txt", nullptr, nullptr, NU, true);
	//LoadNU01("NU01_268_Sverka_4_0005.txt", nullptr, nullptr, NU, true);
	//LoadNU50(".\\Сверка 4\\prognoz_000.txt", nullptr, nullptr, NU, true);

	// Тестирование прецессии и нутации через НУ
	TestAgeValidation();
	//return 0;
	// Сверка результатов прогнозирования
	//MatchingPrediction();

	// Одноимпульсный манёвр
	TaskMagicFly11(SPO_ORB_HBH);

	// Двухимпульсный манёвр
	TaskMagicFly21(SPO_ORB_HBH);

	// Двухимпульсный манёвр
	// для долготы
	TaskMagicFly24(SPO_ORB_HBH);

	// Двухимпульсный манёвр
	// для аргумента широты
	TaskMagicFly241(SPO_ORB_HBH);

	// Трёхимпульсный манёвр
	// для долготы
	TaskMagicFly31(SPO_ORB_HBH);

	// Трёхимпульсный манёвр
	// для аргумента широты
	TaskMagicFly311(SPO_ORB_HBH);

	// Коррекция системы КА
	// для аргумента широты
	TaskMagicFlySys3SC(SPO_ORB_HBH);

	// Создание хранилища ПВЗ 
	//BuildEOPStorage();
	// Тест вычисления ПВЗ 
	//TestEOPCalc();

	// Трасса полёта
	TaskFlightTrack();

	// Прогноз с заданным шагом по времени
	TaskTimePrediction();

	// Самый простой прогноз по виткам
	TaskTurnsPredictionSimpl();

	// Прогноз по виткам группы из 8-и КА
	TaskTurnsPredictionGroup();

	// Прогнозирование по виткам с вычислением 
	// осреднённых на витке элементов движения
	TaskTurnsPredictionHAvr();

	// Расчёт ЗРВ и ЦУ для НКУ
	TaskCalcZRV();

	return 0;
}

//-----------------------------------------------------------------------------
