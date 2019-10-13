//-------------------------------------------------------------------------------
// ZMForceAtm.cpp
// А. Николенко 03.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ZHBL.h>
#include <FlyCoreSource\\ZCISAManager.h>
#include <FlyCoreSource\\ZMForceAtmSMA81.h>
#include <FlyCoreSource\\ZMForceAtmSMA62.h>
#include <FlyCoreSource\\ZMForceAtmMsis.h>
#include <FlyCoreSource\\ZMForceAtmDMAGost.h>
//
// ГОСТ 25645.101-83		"Атмосфера Земли верхняя..."
// ГОСТ 4401-81				"Межгосударственный стандарт. Атмосфера стандартная"
// ГОСТ Р 25645.000-2001	"Атмосфера Земли верхняя..." - 4ЦНИИ 
// ГОСТ Р 25645.166-2004	"Атмосфера Земли верхняя..." - 4ЦНИИ 
// ГОСТ Р 25645.___-2010	"Атмосфера Земли верхняя..." - 4ЦНИИ 

//-------------------------------------------------------------------------------
// Расчёт ускорений по найденной плотности 
int ZFCalcJAtm(double ta, double Ro, double Sb, double* Vg, double* UAsk) ;

//-------------------------------------------------------------------------------
// Атмосфера статическая
int ZFCalcSAtm(double t, double* Rgsk, ZLSF& LSF, ZNU& NU, ZMAtmSMA* pAtm, double* UAsk)
{
	// Обнуляем ускоренияя
	memset(UAsk, 0, 3*sizeof(double)) ;
	// Модель атмосферы долджна быть задана
	if (!LSF.vatm) return-21 ;

	double* Rg=&Rgsk[0] ;	// Координаты в гринвиче
	double* Vg=&Rgsk[3] ;	// Скорпости в гринвиче
	int     rc=0 ;

	// Баллистический коэффициэнт должен быть задан иначе расчёт
	// возмущающего воздействия атмосферы не имеет смысла
	if (NU.Sb<=0) {
		// Не задан баллистический коэффициент в НУ 
		AddErrorMSG( "Не задан баллистический коэффициент в НУ." ) ;
		return-22 ;
	}
	if (!pAtm) {
		AddErrorMSG( "Ошибка учета сопротивления атмосферы. Не создан объект для рассчета плотности атмосферы" ) ;
		return 1 ;
	} 

	// Плотности в кг/м3 по заданной модели (LSF.vatm) 
	double Ro ;  
	rc = pAtm->Calc(Rg, Ro) ; RRC ;
/*
	switch(LSF.vatm) {
	case ATMSMA81: {
			// СМА-81
			// Расчет плотности в кг/м3  
			// Для сравнения плотность атмосферы на высоте 270 км 
			// по ГОСТ 25645.101-83	"Атмосфера Земли верхняя..." 
			//		от 1,2e-11 до 3,7e-11 кг/м3 (для F0=65)
			// по ГОСТ 4401-81		"Межгосударственный стандарт. Атмосфера стандартная" 
			//		3.73422e-11 кг/м3
			// 
			// В нашей реализации процедура выдает значение для высоты 270 км
			//		3.5511e-11 кг/м3
			ZModAtmSMA81* AtmSMA81 = (ZModAtmSMA81*)pAtm ;
			rc = AtmSMA81->Calc(Rg, Ro) ; RRC ;
			} break ;
		case ATMSMA62: {
			// атмосфера АН-62
			// Все аналогично СМА-81
			// В нашей реализации процедура выдает значение для высоты 270 км
			//		7.6436e-011 кг/м3
			//		7.6410e-011 кг/м3
			//		что явно многовато... В чем причина не известно
			ZModAtmSMA62* AtmSMA62 = (ZModAtmSMA62*)pAtm ;
			rc = AtmSMA62->Calc(Rg, Ro) ; RRC ;
			} break ;
		case ATMSMA440181: { 
			// атмосфера СМА ГОСТ 4401.81
			ZModAtmSMA440181* AtmSMA440181 = (ZModAtmSMA440181*)pAtm ;
			rc = AtmSMA440181->Calc(Rg, Ro) ; RRC ;
			} break ;
		default: assert(0) ;
	}
*/	
	rc = ZFCalcJAtm(t, Ro, NU.Sb, Vg, UAsk);
	return rc ;
}

//-------------------------------------------------------------------------------
// Атмосфера динамическая
int ZFCalcDAtm(double t, double* Rgsk, ZLSF& LSF, ZNU& NU, ZCISAManager* ISAManager, double* UAsk, double* atmro)
{
	if (!LSF.vatm) return 0 ;
	if (UAsk) memset(UAsk, 0, 3*sizeof(double)) ;

	double* Rg=&Rgsk[0] ;
	double* Vg=&Rgsk[3] ;
	int     rc=0 ;

	if (NU.Sb <= 0) {
		// Не задан баллистический коэффициент в НУ 
		AddErrorMSG( "Не задан баллистический коэффициент в НУ." ) ;
		return 1 ;
	}
	if (!ISAManager) {
		AddErrorMSG( "Ошибка учета сопротивления атмосферы. Не создан диспетчер индексов СА" ) ;
		return 1 ;
	} 

	// плотности в кг/м3  
	double Ro ;  

	switch(LSF.vatm) {
		case ATMMSIS1990: 
		case ATMMSIS2000: {
			double HBL[3] ;
			ZGRtoHBL(Rgsk, HBL, 0, 0, nullptr, nullptr) ;
			HBL[0] = TO_KM(HBL[0]) ;
			HBL[1] = TO_GRAD(HBL[1]) ;
			HBL[2] = TO_GRAD(HBL[2]) ;
			rc = CalcModAtm_DMA_MSIS(LSF.vatm, t, HBL, ISAManager, &Ro) ; }
			break ;
		case ATMDMAGOST1990: 
		case ATMDMAGOST2001: 
		case ATMDMAGOST2004: 
			rc = CalcModAtm_DMA_GOST(LSF.vatm, t, Rgsk, ISAManager, &Ro) ; 
			break ;
		case ATMDMAGOST1977: 
		case ATMDMAGOST1984: 
		default: assert(0) ;
	}
	RRC ;

	if (atmro) *atmro = Ro ; 
	if (UAsk) rc = ZFCalcJAtm(t, Ro, NU.Sb, Vg, UAsk) ;

	return rc ;
}

//-------------------------------------------------------------------------------
int ZFCalcJAtm(double ta, double Ro, double Sb, double* Vg, double* UAsk)
{
	// Модуль возмущающего ускорения
	double J ;
	// Модуль скорости 
	double Vm = FR(Vg) ;

	// Расчет возмущающих ускорений в ГСК
	//   - Полагаем NU.Sb = Cx*Sm / (2*m) - размерность м2/кг
	//     именно в такой разменрности Sb должен храниться в НУ
	//	 - Нужно помнить, что если КА активно расходует запас топлива на борту
	//	   (например перед его затоплением выдается тормозной импульс) то
	//	   и Sb так же при этом меняется.....  
	//   - Домножаем на 1e7 чтоб получить ускорения в дткм/ссс2  
	//	   (Ro и Sb заданы в метрах поэтому появляется множитель 1e7)
	J = 1e7*Ro*Vm*Sb ;
	// Компоненты ускорения в ГСК
	// !!!! Не забывать поставить минус перед скоростью (-Vg[i]) 
	// потому как ускорение поверхностной силы направлено против скорости КА
	double Jg[3] ;
	for (int i=0 ; i<3 ; i++) Jg[i] =-Vg[i]*J ;
	int rc = ZGSCtoASC1975(ta, Jg, UAsk, 1) ; 
	return rc ;
}

//-------------------------------------------------------------------------------

