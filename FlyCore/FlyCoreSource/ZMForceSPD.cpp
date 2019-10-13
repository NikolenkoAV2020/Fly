//---------------------------------------------------------------------------
// ZMForceSPD.cpp
// А. Николенко 03.08.2018
// Модель светового давления
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
//---------------------------------------------------------------------------
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ZMForceGrav.h>
#include <FlyCoreSource\\ZMForceSPD.h>
//---------------------------------------------------------------------------
// Расчёт светового давления - вариант 4 ЦНИИ МО РФ
// Коэффициент светового давления безразмерный, доля от ГП Солнца)
//---------------------------------------------------------------------------
int ZMForceSPD_DSD(double *Cin_Obj_Ask, double *Cin_Sun_Ask, double dKSD, double* A)
{
	int i;
	double dUKA, dRKA, dXKA[3];
	// Координаты КА относительно Солнца
	for( i = 0, dRKA = 0. ; i < 3 ; i++ ) {
		dXKA[ i ] = Cin_Obj_Ask[ i ] - Cin_Sun_Ask[ i ] ; dRKA+= dXKA[ i ] * dXKA[ i ] ;
	}
	dRKA = sqrt( dRKA ) ;
	// Модуль ускорения КА от СД
	dUKA = dKSD * k_gs / dRKA/dRKA;
	// Проекции ускорения
	for( i = 0 ; i < 3 ; i++ ) A[ i ] = dUKA * dXKA[ i ] / dRKA ;
	return 0 ;
}

//---------------------------------------------------------------------------
// Расчёт светового давления - вариант НПО ПМ
// Коэффициент светового давления - потенциал в км3/с2
//---------------------------------------------------------------------------
int ZMForceSPD_PSD(double *Cin_Obj_Ask, double *Cin_Sun_Ask, double dTempKSD, double *A)
{
	int i;
	double dUKA, dXKA[3], dRKA, dKSD;
	// Перевод КСД в дткм3/ссс2 
	dKSD = dTempKSD * 86400. *86400. / 1.e12 ;
	// Координаты КА относительно Солнца
	for(i=0, dRKA=0.; i<3; i++) {
		dXKA[ i ] = Cin_Obj_Ask[ i ] - Cin_Sun_Ask[ i ] ; dRKA+= dXKA[ i ] * dXKA[ i ] ;
	}
	dRKA = sqrt( dRKA ) ;
	// Модуль ускорения КА от СД
	dUKA = dKSD / dRKA / dRKA ;
	// Проекции ускорения
	for( i = 0 ; i < 3 ; i++ ) A[i] = dUKA * dXKA[i] / dRKA ;
	return 0 ;
}
//---------------------------------------------------------------------------
// Расчёт светового давления - вариант ЦНИИМаш
// Коэффициент светового давления - коэффициент в м2/кг
//---------------------------------------------------------------------------
int ZMForceSPD_KSD(double *Cin_Obj_Ask, double *Cin_Sun_Ask, double dTempKSD, double *Usk)
{
	int i;
	double dUKA, dXKA[3], dRKA, dKSD, dUSD;
	// Мощьность солнечной радиации, приходящаяся на 1 см2 в кг/с3
	double dP = 1360;
	// Скорость света
	double dC = 299790000.;
	// Световое давление солнца в районе орбиты Земли, кг/м/с2
	//double dQ = 4.5365e-6; // Q = P/C
	// Средний радиус орбиты Земли, дткм
	double dR = 149000000.;
	// Коэффициент светового давления 
	dKSD = dTempKSD;
	// Ускорение от светового давления в районе орбиты Земли, дткм/ссс2 
	dUSD = dKSD * dP / dC * dR * dR ;
	dKSD = dUSD * 8.64 * 8.64 / 1.e7 ;
	// Координаты КА относительно Солнца
	for( i = 0, dRKA = 0. ; i < 3 ; i++ ) {
		dXKA[ i ] = Cin_Obj_Ask[ i ] - Cin_Sun_Ask[ i ] ; dRKA += dXKA[ i ] * dXKA[ i ] ;
	}
	dRKA = sqrt(dRKA);
	// Модуль ускорения КА от СД
	dUKA = dKSD / dRKA / dRKA ;
	// Проекции ускорения
	for( i = 0 ; i < 3 ; i++ ) Usk[ i ] = dUKA * dXKA[ i ] / dRKA ;
	return 0 ;
}
//---------------------------------------------------------------------------




/*

//---------------------------------------------------------------------------
// ?aaeecaoey ooieoee ?an?aoa naaoiaiai aaaeaiey
//---------------------------------------------------------------------------
HRESULT _SP_SVDAV_ETALON(double *Cin_Obj_Ask, double *Cin_Sun_Ask, double *Usk)
{
  HRESULT hCodRet;
  int i;
  unsigned short adr;
  // Eiyooeoeaio io?a?aiey iiaa?oiinoe niooieea, aac?acia?iay aaee?eia
  double dK = 1.25;
  // Ieiuaau iiia?a?iiai na?aiey EA, ii?iaeuiiai e naaoiaiio iioieo, i2
  //double dS = 50.0; // Aeiaon-1I
  //double dS = 20.5;   // Yeaeo?i-E
  double dS = 30.0; // EacNao-2
  // Oaeouay ianna EA, ea
  //double dM = 2630; // Aeiaon-1I
  //double dM = 1830;   // Yeaeo?i-E
  double dM = 1260; // EacNao-2
  // Iiuiinou nieia?iie ?aaeaoee, iiiaaa?uae ia 1 ni iiaa?oiinoe, ea/n3
  double dP = 1360;
  // Cei?inou naaoa, i/n
  double dC = 299790000.;
  // Naaoiaia aaaeaiea Nieioa a ?aeiia i?aeou Caiee, ea/i/n2
  //double dQ = 4.5365e-6; // Q = P/C
  // N?aaiee ?aaeon i?aeou Caiee, aoei
  double dR = 149000000.;
  // Onei?aiea io NA a ?aeiia Caiee, i/n2
  double dUSD = dK*dP*dS/dM/dC;
  // Eiyooeoeaio caaoiaiai aaaeaiey, aoei3/nnn2 (Iioaioeae NA)
  double dKSD = dUSD*dR*dR/1000.;
  // Iaioeaiea onei?aiey
  memset (Usk,0,3*sizeof(double));
  // Onei?aiea io NA a ?aeiia Caiee, aoei/nnn2
  dKSD *= 86400.*86400./1.e12;
  // Onei?aiey io NA ia i?aeoa EA, aoei/nnn2
  double dUKA;
  // Eii?aeiaou EA ioiineoaeuii Nieioa, aoei
  double dXKA[3], dRKA;
  // Eii?aeiaou EA ioiineoaeuii Nieioa
  for(i=0, dRKA=0.; i<3; i++)
  { dXKA[i]=Cin_Obj_Ask[i]-Cin_Sun_Ask[i]; dRKA+=dXKA[i]*dXKA[i];
  }
  dRKA = sqrt(dRKA);
  // Iiaoeu onei?aiey EA io NA
  dUKA = dKSD/dRKA/dRKA;
  // I?iaeoee onei?aiey EA io NA
  for(i=0; i<3; i++) Usk[i]=dUKA*dXKA[i]/dRKA;
  return S_OK;
}


*/