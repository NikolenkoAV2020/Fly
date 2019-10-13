//-------------------------------------------------------------------------------
// BuildEOPStorage.cpp : 
// А. Николенко 04.04.2019
//
// Прикладная программа создания хранилища параметров вращения Земли.
// Под хранилищем понитается двоичный файл содержащий записи в виде структуры
// ПВЗ (MJD, координаты полюса X и Y, отклонение времени UTC1-UTC).
// Источником этих данных служит текстовый файл публикуемый 
// международной службой вращения Земли (IERS).
// Данные в хранилище могут формироваться полностью заново, могут дополняться 
// новыми значениями, могут удаляться полностью или частично.
// Возможна также операция проверки имеющихся в хранилище данных путём 
// сопоставления соответствующих значений с данными IERS.
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// Компоненты Fly
//#include <FlyMMath.h>		// Математические функции
//#include <FlySofa.h>		// Функции фундаментальной астрономии
//#include <FlySsp.h>			// Численные алгоритмы "пакета SSP"
//#include <FlyChebyshov.h>	// Апроксимация дискретной функции полиномами
// Чебышёва
#include <FlyCore.h>		// Собственоно сама модель движения

//---------------------------------------------------------------------------
int BuildEOPStorage()
{
	ZEOPManager EOPMng;
	int rc = 0;
	rc = EOPMng.LoadFromIersFormat(true); if (rc) return rc;
	rc = EOPMng.LoadFromBin(); if (rc) return rc;
	double t1, t2;
	t1 << DATETIMEUTC(2, 1, 2000, 0, 0, 0, 0);
	t2 << DATETIMEUTC(2, 1, 2005, 0, 0, 0, 0);
	rc = EOPMng.MakeAllEopModels(t1, t2); if (rc) return rc;
	return 0;
}

//---------------------------------------------------------------------------
int TestEOPCalc()
{
	EOPMng;
	double tn, tk, t, dt;
	int    rc = 0;
	dt = 6.0 / 24.0;
	tn << DATETIMEUTC(1, 1, 2000, 0, 0, 0, 0);
	tk << DATETIMEUTC(1, 1, 2010, 0, 0, 0, 0);
	t  = tn;

	ZEOPItem P;

	while (t < tk) {
		rc = EOPMng.Calc(t, P); if (rc) break;
		t += dt;
	}

	return 0;
}

//---------------------------------------------------------------------------

/*
class CA {
public:
CA() {
X = 0;
Y = 0;
Z = 0;
}

CA(const CA& a) {
cout << "Copy";
cout << "  X = " << X << "  a.X = " << a.X << endl;
X = a.X;
Y = a.Y;
Z = a.Z;
}

CA(CA&& a) {
cout << "Move";
cout << "  X = " << X << "  a.X = " << a.X << endl;
X = a.X;
Y = a.Y;
Z = a.Z;
//a.X = 0;
//a.Y = 0;
//a.Z = 0;
}

~CA() {
cout << "Destructir";
cout << "  X = " << X << endl;
X = 0;
Y = 0;
Z = 0;
}

CA& operator = (const CA& a) {
cout << "operator Copy";
cout << "  X = " << X << "  a.X = " << a.X << endl;
X = a.X;
Y = a.Y;
Z = a.Z;
return(*this);
}

CA& operator = (CA&& a) {
cout << "operator Move";
cout << "  X = " << X << "  a.X = " << a.X << endl;
X = a.X;
Y = a.Y;
Z = a.Z;
X = 0;
Y = 0;
Z = 0;
return(*this);
}

operator double* () {
return &X;
}

operator double () {
return X;
}

CA& operator = (const double& d) {
X = d;
Y = d;
Z = d;
return(*this);
}

protected:
double X;
double Y;
double Z;
};
CA a;
CA b;
CA c;
CA* pa = nullptr;

a = 2.0;
b = 4.0;
c = 15.0;
CA d(a);
//move(c, a, b);

c = a = b;

double* X = a;
X[0] = 12.345;
X[1] = 67.890;
X[2] = 34.567;

X[2] = b;

pa = new CA[5];
pa[0] = 1.0;
pa[1] = 15.0;

CA a1;  a1 = 1;
CA a2;  a2 = 2;
CA a3;  a3 = 3;
CA a4;  a4 = 4;
CA a5;  a5 = 5;
CA a6;  a6 = 6;
CA a7;  a7 = 7;
CA a8;  a8 = 8;
CA a9;  a9 = 9;
CA a10;  a10 = 10;
CA a11;  a11 = 11;
CA a12;  a12 = 12;
CA a13;  a13 = 13;

vector<CA> VCA;
VCA.reserve(5);
VCA.push_back(a1);
cout << endl;
VCA.push_back(a2);
cout << endl;
VCA.push_back(a3);
cout << endl;
VCA.push_back(a4);
cout << endl;
VCA.push_back(a5);
cout << endl;
VCA.push_back(a6);
cout << endl;
VCA.push_back(a7);
cout << endl;
VCA.push_back(a8);
cout << endl;
VCA.push_back(a9);
cout << endl;
VCA.push_back(a10);
cout << endl;
VCA.push_back(a11);
cout << endl;
VCA.push_back(a12);
cout << endl;
VCA.push_back(a13);
cout << endl;


VCA.resize(5);

*/