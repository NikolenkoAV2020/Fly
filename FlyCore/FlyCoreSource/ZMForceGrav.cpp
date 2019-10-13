//---------------------------------------------------------------------------
// ZMForceGrav.cpp
// А. Николенко 03.08.2018
// Реализация методов класса "Модель Гравитационного Поля Земли"
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
//---------------------------------------------------------------------------
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время
#include <FlySofa.h>	// Функции фундаментальной астрономии

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ZMForceGrav.h>

//---------------------------------------------------------------------------
// Конструкторы
//---------------------------------------------------------------------------
ZMForceGrav::ZMForceGrav()
{
	GravPole.Clear();
	GarmNorm    = nullptr;
	InDimen     = false;
	IndGarmNorm = false;
	Kft_C       = nullptr;
	Kft_D       = nullptr;
	dSQRT_53    = sqrt(5. / 3.);
	dSQRT_3     = sqrt(3.);
	memset(GpzAdd, 0, sizeof(GpzAdd));
}

//---------------------------------------------------------------------------
ZMForceGrav::ZMForceGrav(long VariantGPZ)
{
	GravPole.Clear();
	GarmNorm    = nullptr ;
	InDimen     = false ;
	IndGarmNorm = false ;
	Kft_C    = nullptr ; 
	Kft_D    = nullptr ;
	dSQRT_53 = sqrt(5./3.) ;
	dSQRT_3  = sqrt(3.) ;
	Init_ZMForceGrav(VariantGPZ, 70) ;
	memset(GpzAdd, 0, sizeof(GpzAdd));
}

//---------------------------------------------------------------------------
// Деструктор
//---------------------------------------------------------------------------
ZMForceGrav::~ZMForceGrav()
{
	if(Kft_C) delete [] Kft_C;
	if(Kft_D) delete [] Kft_D;
}

//---------------------------------------------------------------------------
//Инициализация объекта
//---------------------------------------------------------------------------
HRESULT ZMForceGrav::Init_ZMForceGrav(long VariantGPZ, long SizeGPZ)
{
	// Контроль варианта загрузки ГПЗ
	if ( GravPole.VariantGPZ == VariantGPZ && GravPole.Nmax >= SizeGPZ ) return 0 ;

	int rc = ZFGravPoleLoad( &GravPole, VariantGPZ, SizeGPZ ) ; RRC ;
	InDimen     = false ;
	IndGarmNorm = false ;
    // Создание массива факториалов
	rc = MakeFactorial() ; RRC ;
	// Перевод коэффициентов во внутреннюю размерность
	rc = Change_Dimen(1) ; RRC ;

	return 0 ;
}

//---------------------------------------------------------------------------
// Реализация модели центрального поля
//---------------------------------------------------------------------------
HRESULT ZMForceGrav::Model_Grav_Pole_Center(double *Coord_Point, double *Acsel_GP)
{
	int i ;
	// Расчет величины, обратной радиус-вектору
	R = FR(Coord_Point) ;
	R = 1/R ;
	// Расчет вспомогательных величин
	Ae_R  = GravPole.AE*R ;
	B0_R1 = GravPole.B0*R ;
	B0_R2 = B0_R1*R ;
	B0_R3 =-B0_R2*R ;
	// Расчет компонентов единичного вектора радиуса
	for(i=3;--i>=0;) Comp_Ed_R[i] = Coord_Point[i]*R ;
	// Защита проекции Z от нулевого значения
	if(Comp_Ed_R[2]==0.0) Comp_Ed_R[2]=1.e-15 ;
	// Расчет ускорений
	for(i=3;--i>=0;) Acsel_GP[i] = Coord_Point[i]*B0_R3 ;
	return S_OK ;
}

//---------------------------------------------------------------------------
// Реализация модели нормального поля
//---------------------------------------------------------------------------
HRESULT ZMForceGrav::Model_Grav_Pole_Normal(double *Coord_Point, double *Acsel_GP, long Var_Priliv)
{
	int i;
	double v,ynm[5];

	double pt=1.5*Comp_Ed_R[2]*Comp_Ed_R[2]-0.5;
	double pzt=3*Comp_Ed_R[2];

	Log_Ae_R = log(Ae_R);
	ynm[0]=(-GravPole.J2)*B0_R1*exp(2*Log_Ae_R)*pt;
	ynm[1]=0.;
	ynm[2]=0.;
	ynm[3]=ynm[0]*pzt/pt;
	ynm[4]=-3*ynm[0];

	v=ynm[4];
	for(i=3;--i>=0;) v-=ynm[i+1]*Comp_Ed_R[i];
	for(i=3;--i>=0;) Acsel_GP[i]=(ynm[i+1]+v*Comp_Ed_R[i])*R;

	return S_OK ;
}

//---------------------------------------------------------------------------
// Реализация модели аномалийного поля
//---------------------------------------------------------------------------
HRESULT ZMForceGrav::Model_Grav_Pole_Anomal(double dTime, short N,
										 short M, long ScaleAddGarm, 
										 double* Coord_Point, double* Acsel_GP, 
										 VersionTide VPriliv)
{
	char   Scal[32], buf[32];
	double Scale;
	short  i, mt, MM, nt, NM, L;
	double v, Zu, ynm[6], at, bt, ap, bp, pt, pp, ppm, pzp, pzt, Cnm, Dnm;

	NM = MM = 0;
	Scale = ScaleAddGarm;
	memset(Scal, 0, sizeof(Scal));

	if (N >= M) { Scale = 0; NM = N; MM = M; }
	else {
		NM = MM = M;
		i = 0;
		if (Scale != 0) {
			while (Scale>1) {
				Scale /= 2;
				if (floor(Scale) != Scale) {
					buf[i] = 1;
					Scale = floor(Scale);
				}
				else buf[i] = 0;
				i++;
			}
			buf[i] = 1;
			for (int j = i; ++j <= 32; buf[j] = 0);
			memcpy(&Scal[0], &buf[0], 32);
		}
	}

	// Обнуление ускорений
	for (i = 3; --i >= 0;) { Acsel_GP[i] = ynm[i] = ynm[i + 3] = 0.0; }

	Log_Ae_R = log(Ae_R);
	v = Zu = ap = bp = 0;
	pt = pp = ppm = pzt = pzp = 0;
	at = 0;
	bt = 0;

	if ((N<0) || (M<0)) return S_OK;

	// Учет океанических приливов
	if (VPriliv != TIDENULL) {
		GeopotenzialCorrector(dTime, Coord_Point);
		Doodson_arg(dTime, Darg);
	}

	// Начало алгоритма - цикл по m
	for (mt = 0; mt <= MM; mt++) {
		// Вычисление Psi
		Zu = at*Comp_Ed_R[0] - bt*Comp_Ed_R[1];
		ap = at*mt;
		bp = bt*mt;
		bt = at*Comp_Ed_R[1] + bt*Comp_Ed_R[0];
		at = Zu;
		if (mt == 0) { at = 1; bt = 0; }

		// Цикл по n
		for (nt = mt; nt <= NM; nt++) {
			// Формирование Cnm, Dnm
			if (VPriliv != TIDENULL) {
				// Учет океанических приливов
				double CDnm[2];
				// Коррекция коэффициентов ГПЗ
				CalcCDnm(dTime, nt, mt, CDnm, VPriliv);
				Cnm = CDnm[0];
				Dnm = CDnm[1];
			} else {
				// Нет учета океанических приливов
				L   = (nt*nt + nt) / 2 + mt;
				Cnm = Kft_C[L];
				Dnm = Kft_D[L];
			}

			// Вычисление присоединенных функций Лежандра
			if (nt == mt) {
				// Pnn:  ppm - P(n-1)(n-1)
				if (nt == 0) pt = 1;
				else       pt = (2 * nt - 1)*ppm;
				ppm = pt;
			} else {
				// dP(n)m - по рекурентной форме
				Zu = (2 * nt - 1)*(pt + pzt*Comp_Ed_R[2]) - (nt + mt - 1)*pzp;
				Zu /= (nt - mt);
				pzp = pzt;
				pzt = Zu;
				// P(n)m - по рекурентной форме
				Zu = (2 * nt - 1)*pt*Comp_Ed_R[2] - (nt + mt - 1)*pp;
				Zu /= (nt - mt);
				pp = pt;
				pt = Zu;
			}

			if ((!Scal[mt]) && (nt>N))  break;

			ynm[4] = B0_R1*exp(nt*Log_Ae_R)*pt;
			if (mt != 0) {
				ynm[0] = at*Cnm + bt*Dnm;
				ynm[1] = ynm[4] * (ap*Cnm + bp*Dnm);
				ynm[2] = ynm[4] * (ap*Dnm - bp*Cnm);
			} else {
				ynm[0] = Cnm;
				ynm[1] = 0;
				ynm[2] = 0;
				if (nt<2) continue;
			}

			ynm[3] = ynm[0] * ynm[4] * pzt / pt;
			ynm[0]*= ynm[4];
			ynm[4] =-(nt + 1)*ynm[0];
			for (i = 3; --i >= 0;) Acsel_GP[i] += ynm[i + 1];
			v += ynm[4];
	    }
		pp=0; // Pnm: будет n<m 
		pzt = pzp = 0;
	}
	for (i = 3; --i >= 0;) v -= Acsel_GP[i] * Comp_Ed_R[i];
	for (i = 3; --i >= 0;) Acsel_GP[i] = (Acsel_GP[i] + v*Comp_Ed_R[i])*R;
	return S_OK ;
}

//---------------------------------------------------------------------------
// Формирование "нулевого" гравитационного поля
//---------------------------------------------------------------------------
HRESULT ZMForceGrav::Make_nullptrPole(short N, short M,  long ScaleAddGarm)
{
	char   Scal[32], buf[32];
	double Scale;
	short  i, mt, MM, nt, NM, L;

	// Необходима проверка на наличие загруженного поля
	if (!GravPole.Garm) return 1;

	NM = MM = 0;
	Scale = ScaleAddGarm;
	memset(Scal, 0, sizeof(Scal));

	if (N >= M) { Scale = 0; NM = N;  MM = M; }
	else { 
		NM = MM = M;
		i = 0;
		if (Scale != 0) {
			while (Scale>1) {
				Scale /= 2.0;
				if (floor(Scale) != Scale) {
					buf[i] = 1;
					Scale = floor(Scale);
				} else buf[i] = 0;
				i++;
			}
			buf[i] = 1;
			for (int j = i; ++j <= 32; buf[j] = 0);
			memcpy(Scal, buf, 32);
	}	}
	for (mt = 0; mt <= MM; mt++) {
		for (nt = mt; nt <= NM; nt++) {
			L = nt*nt + nt + 2 * mt;
			if ((Scal[mt] == 0) && (nt>N)) {
				GravPole.Garm[L]     = 0.0;
				GravPole.Garm[L + 1] = 0.0;
	}	}	}
	return S_OK;
}

//---------------------------------------------------------------------------
// Приведение ко внутренней размерности (и обратно)
//---------------------------------------------------------------------------
HRESULT ZMForceGrav::Change_Dimen(short Param)
{
	if (!GravPole.Garm) return 1;

	bool bDirect = (Param ? true : false);
	int  i;

	// в безразмерные, ненормированные, дткм/ссс
	if (bDirect && !InDimen) {
		int k = (GravPole.Nmax*GravPole.Nmax + GravPole.Nmax) / 2 + GravPole.Nmax + 1;
		Kft_C = new double[k];
		Kft_D = new double[k];

		for (int n = GravPole.Nmax; n >= 0; n--) {
			for (int m = n; m >= 0; m--) {
				i = n*n + n + 2 * m;
				//GravPole.vGarm.at(i)  /= vGarmNorm.at(i/2) ; 
				//GravPole.vGarm.at(i+1)/= vGarmNorm.at(i/2) ; 
				GravPole.Garm[i]     /= GarmNorm[i / 2];
				GravPole.Garm[i + 1] /= GarmNorm[i / 2];
				//Kft_C[i/2] = GravPole.vGarm.at(i) ;
				//Kft_D[i/2] = GravPole.vGarm.at(i+1) ;
				Kft_C[i / 2] = GravPole.Garm[i];
				Kft_D[i / 2] = GravPole.Garm[i + 1];
			}
		}
		InDimen  = true;
		dParJ2   = -GravPole.J2 / sqrt(5.0);
		dSQRT_53 = sqrt(5.0 / 3.0);
		dSQRT_3  = sqrt(3.0);
		// Формирование C20
		if (GravPole.Nmax >= 2) Kft_C[3] = -GravPole.J2;
	}
	// в безразмерные, нормированные, м/с
	if (!bDirect && InDimen) {
		for (int n = 0; n <= GravPole.Nmax; n++) {
			for (int m = 0; m <= n; m++) {
				i = n*n + n + 2 * m;
				//GravPole.vGarm.at(i)  *= vGarmNorm.at(i/2) ; 
				//GravPole.vGarm.at(i+1)*= vGarmNorm.at(i/2) ;
				GravPole.Garm[i]    *= GarmNorm[i / 2];
				GravPole.Garm[i + 1]*= GarmNorm[i / 2];
			}
		}
		GravPole.AE *= 1.e7;
		GravPole.B0 *= 1.e7 / 86400.*1.e7 / 86400.*1.e7;
		InDimen = false;
	}
	return S_OK;
}

//---------------------------------------------------------------------------
// Создание массива факториалов
//---------------------------------------------------------------------------
HRESULT ZMForceGrav::MakeFactorial()
{
	double x;
	int    i, n, m, k;

	if (GarmNorm) delete[] GarmNorm; GarmNorm = nullptr;
	k = GravPole.Nmax + 1;
	k = (1 + k)*k / 2;
	GarmNorm = new double[k];
	k = 0;
	i = 0;

	for (n = 0; n <= GravPole.Nmax; n++) {
		for (m = 0; m <= n; m++) {
			x = 1.0;
			k = 0;
			if (n >= 0 && m>0) for (k = n - m + 1; k <= n + m; k++) x *= k;
			if (m != 0) x/= 2.0;
			else        x = 1.0;
			GarmNorm[i] = sqrt(x / (2 * n + 1));
			i++;
	}	}
	IndGarmNorm = true;
	return S_OK ;
}

//---------------------------------------------------------------------------
// Коррекция коэффициентов ГПЗ Cnm и Dnm из-за приливов от Луны и Солнца
//---------------------------------------------------------------------------
HRESULT ZMForceGrav::GeopotenzialCorrector (double dTime, double *Gri)
{
	int    rc;
	double gkoo[4], akoo[4];
	struct SOLID_TIDE sd;
	double Knm[] = {
		//  Re          Im           +
		0.30190   ,  0.00000   , -0.00089*1.341641,   // 2,0
		0.29830/3 , -0.00144/3 , -0.00080*0.244949,   // 2,1
		0.30102/12, -0.00130/12, -0.00057*0.057735,   // 2,2
		0.093     ,                                   // 3,0
		0.093 /6  ,                                   // 3,1
		0.093 /60 ,                                   // 3,2
		0.094 /360                                    // 3,3
		};

	akoo[0] = dTime;
	memset(GpzAdd, 0, sizeof(GpzAdd));

	double* add = GpzAdd;
	for (int k = 0; k<2; k++) {
		double MassRatio = (k ? k_gl : k_gs) / GravPole.B0;
		// Обращаемся к астрономическому ежегоднику где по полиномам вычисляются 
		// ккординаты Солнца. 
		// Координаты Солнца лежат в Wae->qso
		// Координаты Луны лежат в Wae->qlu
		rc = AEJG(akoo[0], (short)k); RRC;
		memcpy(akoo + 1, (k ? Wae->qlu : Wae->qso), 3 * sizeof(akoo[0]));
		rc = ZASC1975toGSC(dTime, &akoo[1], &gkoo[1], 1); if (rc) return rc;

		LegendreInit(&sd.l, gkoo[3] / (sd.r0[0] = FR(gkoo + 1)));
		sd.r0[2] = sd.r0[0] = GravPole.AE / sd.r0[0];
		sd.lon = atan2(gkoo[2], gkoo[1]);

		while (sd.l.m <= 3) {
			double csml = cos(sd.l.m*sd.lon);
			double snml = sin(sd.l.m*sd.lon);
			while (1) {
				if (sd.l.n >= 2) {
					double fc = MassRatio*sd.l.val*sd.r0[sd.l.n - 1];
					double ReKnm, ImKnm;
					if (sd.l.n == 2) {
						double* Base = Knm + 3 * sd.l.m;
						ReKnm = Base[0];
						ImKnm = Base[1];
						add[4] = fc*Base[2] * csml;
						add[5] = fc*Base[2] * snml;
					} else {
						ReKnm = Knm[9 + sd.l.m];
						ImKnm = 0;
					}
					add[0]+= fc*(ReKnm*csml + ImKnm*snml);
					add[1]+= fc*(ReKnm*snml - ImKnm*csml);
					add   += 2;
				}
				if (sd.l.n >= 3) break;
				if (!sd.l.m) {
					sd.r0[2] *= sd.r0[0];
					if (sd.l.n == 1) sd.r0[1] = sd.r0[2];
				}
				LegendreNext(&sd.l);
			}
			add += 2;
			LegendreNextRow(&sd.l);
	}	}
	return rc;
}

//---------------------------------------------------------------------------
// Расчет коэффициентов С21 и D21, определяемых средним полюсом
// и положением текущего полюса в соответствии со стандартом IERS
//---------------------------------------------------------------------------
HRESULT ZMForceGrav::CalcCD21(double t, double J2, double* CD21)
{
	int RetCode;
	double arg, xp, yp, x, y, m1, m2;

	arg= (t + DELTA_2000) / 365.25;
	xp = 2.62e-7 + arg*4.02e-9;
	yp = 1.730e-6 + arg*1.915e-8;

	RetCode = Model_PVZ(t, &x, &y, nullptr);
	m1 = x - xp;
	m2 = yp - y;

	CD21[0] =-J2*xp - 1.721e-9*(m1 + 0.0155*m2);
	CD21[1] = J2*yp - 1.721e-9*(m2 - 0.0155*m1);

	return RetCode;
}

//---------------------------------------------------------------------------
// Формирование текущей пары коэффициентов ГПЗ Cnm и Dnm
//---------------------------------------------------------------------------
HRESULT ZMForceGrav::CalcCDnm (double dTime, int n, int m, double* CDnm, VersionTide VPriliv)
{
	// Расчет индекса смещения
	int L = (n*n + n) / 2 + m;
	// Формирование Cnm, Dnm
	CDnm[0] = Kft_C[L];
	CDnm[1] = Kft_D[L];
	if (n == 2 && m == 1) CalcCD21(dTime, GravPole.J2, CDnm);
	if ((n >= 2) && (n <= 4) && (m<4)) {
		L = 6 * m + 2 * (n - 2);
		if (m>2) L -= 2;
		CDnm[0] += GpzAdd[L];
		CDnm[1] += GpzAdd[L + 1];
	}
	switch(VPriliv) {
		case TIDEPOSEIDON:	O_Tide_GPZ(Darg, CDnm, n, m); break;
		case TIDESTEP2:		Step2_GP(Darg, CDnm, n, m); break;
		default: assert(0);
	}
	return S_OK;
}

//---------------------------------------------------------------------------
// Учет океанических приливов в ГПЗ 
// !!!! Результаты не корректные !!!!
//---------------------------------------------------------------------------
HRESULT ZMForceGrav::Calc_OceanTide(double Time, double *Gri, short N, short M, VersionTide VPriliv, double *Usk)
{
	struct LEGENDRE Leg;
	double longitude, mas[5], ae_r0, A, r;

	memset(mas, 0, sizeof(mas));
	// Инициализация полинома Лежандра
	LegendreInit(&Leg, Gri[2] * R);
	ae_r0 = B0_R2; r = Ae_R;
	longitude = atan2(Gri[1], Gri[0]);

	GeopotenzialCorrector(Time, Gri);
	Doodson_arg(Time, Darg);

	// Цикл по столбцам
	while (Leg.m <= M) {
		double tml[2], ae_r = ae_r0;
		A = Leg.m*longitude;
		tml[0] = cos(A);
		tml[1] = sin(A);
		if (Leg.m == 1) memcpy(mas + 3, tml, sizeof(tml));
		while (1) {
			double CDnm[2], y1, xx;
			CalcCDnm(Time, Leg.n, Leg.m, CDnm, VPriliv);

			xx = CDnm[0] * tml[0] + CDnm[1] * tml[1];
			y1 = ae_r*Leg.val;

			mas[1] += xx*(Leg.bval*(Leg.m + Leg.n) - Leg.n*Leg.val*Leg.cst)*ae_r;
			mas[2] += (CDnm[1] * tml[0] - CDnm[0] * tml[1])*Leg.m*y1;
			mas[0] -= (Leg.n + 1)*xx*y1;

			if (Leg.n >= N) break;

			ae_r *= r;
			if (Leg.n == Leg.m) ae_r0 = ae_r;
			LegendreNext(&Leg);
		}
		LegendreNextRow(&Leg);
	}

	// перевод ускорений в Гринвич
	Usk[2] = (mas[0] * Leg.cst + mas[1]);
	A = mas[0] * Leg.snt - mas[1] * Leg.cst / Leg.snt;
	Usk[0] = (A*mas[3] - (mas[2] /= Leg.snt)*mas[4]);
	Usk[1] = (A*mas[4] + mas[2] * mas[3]);

	return S_OK;
}

//---------------------------------------------------------------------------

