//-------------------------------------------------------------------------------
// TaskOrbitDesign.cpp : 
// А. Николенко 28.12.2018
//-------------------------------------------------------------------------------
#include "stdafx.h"

//-------------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>		// Собственоно сама модель движения
#include <FlyMMath.h>		// Математические функции
#include <FlyOrbitDesign.h>	//

//-------------------------------------------------------------------------------
static double k_eps_kms = 26332713071.154318 ;

// Экваториальный радиус
double Re = 6378.136 ;
// Средний радиус Земли
double Rz = 6371.0 ;
double mu = 398600.44 ;
double eps= k_eps_kms ; // 2.634e10 ;

// Коэффициенты зональных гармоник разложения ГПЗ
double C20 =-0.0010826257 ;
double C30 = 2.5324070e-6 ;
double C40 = 1.6305600e-6 ;
double C50 = 2.2791850e-7 ;
double C60 =-5.1721630e-7 ;  
	
double J2 = 1.0826257e-3 ;
double J3 =-2.532407e-6 ;
double J5 =-0.2279185e-6 ;
double J7 =-0.350079e-6 ;

// Логическая шкала сил
// т.е. принятая модель движения
ZLSF LSF ;

char FilePrintSticker[64] = "" ;

//-------------------------------------------------------------------------------
// Для средней высоты
// 1-й этап. Нулевое приближение при заданной средней высоте
int OrbitDesignSSOStage1h(double h0, double& a0, double& e0, double& w0, double& i0) ; 

//-------------------------------------------------------------------------------
// Для драконического периода
// 1-й этап. Нулевое приближение при заданном драконическом периоде
int OrbitDesignSSOStage1T(double Tdr0, double& a0, double& e0, double& w0, double& i0) ; 
// 2-й этап для заданного драконического периода
// Уточнение значения полуоси по заданному периоду, численно в полном поле сил
int OrbitDesignSSOStage2T(ZLSF* LSF, double Tdr, double a0, double e0, double i0, double W0, double w0, double u0, double& ak) ;

//-------------------------------------------------------------------------------
// Для заданной кратности и замыкания трассы
// 1-й этап. Нулевое приближение при заданных k-суточной кратности и с замыканием
// трассы через l полных витков
int OrbitDesignSSOStage1kl(double k, double l, double& Tdr0, double& a0, double& e0, double& w0, double& i0) ; 

//-------------------------------------------------------------------------------
// Вычисление оскулирующих элементов на начало витка по заданному наклонению
// Результат решения - элементы солнечно-синхронной геометрически устойчивой орбиты
// Если isStage2 = false,	то решение по первомку этапу, то есть в нулевом 
//							приближении выполняется расчёт аналитически
// Если isStage2 = true,	то выполняется второй этап расчёта с вычислением
//							драконического периода в полном поле сил численно
//					i0,	-	Искомые параметры орбиты на начало витка
//					a0,
//					e0, w0,
//					Tdr0
int OrbitDesignSSOStage12I(bool isStage2, double i0, double& a0, double& e0, double& w0, double& Tdr0) ;

// Точное решение задачи по наклонению с учётом 7-и зональных гармоник
int OrbitDesignStage3 (double a0, double e0, double w0, double i0, double Tdr0, double& ik) ; 
// Осреднение геометрии орбиты на начало витка
int OrbitDesignStage4 (double a0, double e0, double i0, double w0, double W0, double u0, 
					   double& es, double& ws, double& e1s, double& e2s) ; 
// Проноз по времени
int OrbitDesignStage5 (double a0, double e0, double i0, double w0, double W0, double u0, 
					   double* dL=NULL, double* dLs=NULL, 
					   double* Hmin=NULL, double* Hmax=NULL, double* Hs=NULL) ; 
// Проноз по виткам с построением высотного профиля
int OrbitDesignStage6 (double a0, double e0, double i0, double w0, double W0, double u0) ;
int OrbitDesignStage7 (double a0, double e0, double i0, double w0, double W0, double u0) ;

// Расчёт драконического периода в полном поле сил
int CalcPeriodInFullForce(ZLSF* LSF, double a0, double e0, double i0, double W0, double w0, double u0, double& Tdr) ;
// Проектирование орбиты ССО для заданного периода
int OrbitDesignSSOfromPeriod(double Tdr0) ;
// Проектирование орбиты ССО для заданной кратности трассы полёта
int OrbitDesignSSOfromMultiplicity(int K, int L) ; 
// Вычисление параметров орбиты минимального изменения стационарных высот 
int CalcParamMCSHOfromA(double a, double& i, double& e, double& w) ;
// Уточнение параметров орбиты минимального изменения стационарных высот
// по условию равенства высот в диаметрально противоположенных точках орбиты
int EstParamMCSHOfromA(double a, double i, double e, double w, double& ik) ;

//-------------------------------------------------------------------------------
// Проектирование орбит
// минимального изменения высоты, в том числе и геометрически устойчивых
//-------------------------------------------------------------------------------
int TaskOrbitDesign()
{
	// Логическая шкала сил
	// т.е. принятая модель движения
	LSF.num    = 1 ;
	LSF.vgpz   = GPZ90 ;
	LSF.ngpz   = 16 ;
	LSF.mgpz   = 16 ;
	LSF.vatm   = ATMNULL ;  // ATMSMA81 ; ATMNULL
	LSF.vsvd   = 0 ;       
	LSF.varsp  = 0 ;
	LSF.vTide  = TIDENULL ; // TIDENULL, TIDESTEP2, TIDEPOSEIDON
	LSF.isDU   = 0 ;
	LSF.isSun  = 0 ;
	LSF.isMoon = 0 ;
	memset(LSF.isPlanet, 0, sizeof(LSF.isPlanet )) ;

	// ПРИМЕРЫ ПРОЕКТИРОВАНИЯ

	//---------------------------------------------------------------------------
	//double a, i, e, w ;
	//a = 6879.872 ; 
	// Выбор наклонения для проектирования ОМИВ стационарных высот
	//CalcParamMCSHOfromA(a, i, e, w) ;
	// Проектировыание ССО для заданного наклонения
	//double Tdr ;
	//OrbitDesignSSOStage12I(false, i, a, e, w, Tdr) ;
	//OrbitDesignSSOStage12I(true,  i, a, e, w, Tdr) ;
	// Построение высотного профиля д
	//OrbitDesignStage6  (a, e, i, w, 0, 0) ;
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	// Выбор наклонения для проектирования ОМИВ стационарных высот
	//CalcParamMCSHOfromA(a, i, e, w) ;
	// Построение высотного профиля д
	//sprintf(ZPrintManager::CommonFileSticker, "Поле_0700ATM\0") ; ; 
	//OrbitDesignStage7(a, e, i, w, 0, 0) ;
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	//i = 65.939 ;
	//OrbitDesignStage6  (a, e, i, w, 0, 0) ;
	//EstParamMCSHOfromA (a, i, e, w, ik) ;
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	// Для заданного периода проектируем ССО
	// OrbitDesignSSOfromPeriod(95.664*60.0) ; 
	//---------------------------------------------------------------------------

	FAST_SEMAPFORE_OPEN("TaskOrbitDesignBlock") ;

	// Множество возможных суточных кратностей
	VKLTOrb VKL ;
	// Построение множества всех возможных суточных кратностей
	// Вариация периода            от 1/16 до 1/14 суток
	// Изменение колличества суток от 1    до 14
	// Изменение числа витков 
	// замыкания трассы            от 14   до 224 
	BuildLotsOfKLT(1.0/16.0, 1.0/14.0, 1, 21, 14, 400, VKL) ;

	// Для рассчитанного набора кратностей
	int rc, N = VKL.size() ;
	N=5 ;
	for (int j=0; j<N; j++) {
		int K = VKL[j].K ;
		int L = VKL[j].L ;
		rc = OrbitDesignSSOfromMultiplicity(K, L) ;	RRC ;
	}

	FAST_SEMAPFORE_CLOSE() ;

	return 0 ;
}

//-------------------------------------------------------------------------------
// Вычисление вспомогательных величин gam2... gam7
void CalcGam2357(double a, double& gam2, double& gam3, double& gam5, double& gam7)
{
	// Отношение экваториального радиуса к большой полуоси
	double Rea = Re/a ;
	// коэффициенты "гамма"
	gam2= J2*pow(Rea,2) ;
	gam3=-J3*pow(Rea,3) ;
	gam5=-J5*pow(Rea,5) ;
	gam7=-J7*pow(Rea,7) ;
}

//-------------------------------------------------------------------------------
// Вычисление параметров орбиты минимального изменения стационарных высот 
// (она же геометрически устойчивая орбита) для заданного значения 
// большой полуоси a
// Результаты
//		i	- наклонение 
//		e	- эксцентриситет
//		w	- положение перигея орбиты
// Результат наклонение i в диапазоне от 0 до 90 градусов
// Задача имеет второе решение 180-i !!!!
//
// Функция является первым этапом проектирования ОМИСВ, 
// а именно аналитическим расчётом нулевого приближения. После чего наклонение 
// уточняется численно в полном поле сил исходя из условия равенства высот 
// в противоположенных точках орбиты, например в точках с аргументами широты
// 90 и 270 градусов.
//-------------------------------------------------------------------------------
int CalcParamMCSHOfromA(double a, double& i, double& e, double& w)
{
	// Коэффициенты gam
	double gam2, gam3, gam5, gam7 ;
	CalcGam2357(a, gam2, gam3, gam5, gam7) ;

	// Коэффициенты алг-го уравнения 3-го порядка для вычисления наклонения
	double c2 =-5.0/8.0 * gam5 ; 
	double c3 = 35.0/256.0 * gam7 ; 
	double K[4] ;
	K[0] =-429.0*c3 ;  
	K[1] = 792.0*c3+21.0*c2 ;  
	K[2] =-432.0*c3-28.0*c2-5.0/2.0*gam3 ;  
	K[3] =  64.0*c3+ 8.0*c2+2.0*gam3 ;  
	// решение кубического уравнения
	// K[0]*s3+K[1]*s2+K[2]*s+K[3] = 0 ;
	
	double Xr[3], Xi[3] ;
	int    rc, CountX ;
	rc = MSolv3DegreeEquation(K, Xr, Xi, &CountX) ;	if (rc || CountX!=1) return 1 ;    
	// квадрат синуса наклонения
	double s2= Xr[0] ; if (s2<=0) return 2 ;
	// синус наклонения
	double s = sqrt(s2) ; 
	// Наклонение орбиты в градусах
	i = Asin(s, 1) ;
	// Эксцентриситет орбиты
	// ex = e
	// ey = 0 ===> w = 0 !!!! 
	e = gam2*(1.5-s2) ;
	// Аргумент перигея орбиты 
	w = 0 ;

	return 0 ;
}

//-------------------------------------------------------------------------------
// Уточнение параметров орбиты минимального изменения стационарных высот
// по условию равенства высот в диаметрально противоположенных точках орбиты
int EstParamMCSHOfromA(double a, double i, double e, double w, double& ik)
{
	int    rc ;

	// Начальные условия движения
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a) ;
	NU.X[1] = e ;
	NU.X[2] = TO_RAD(i+1) ;
	NU.X[3] = TO_RAD(0) ;
	NU.X[4] = TO_RAD(w) ;
	NU.X[5] = TO_RAD(0) ;
	NU.Sb   = 0.0005 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

	int Vit = NU.vit ;

	// Модель движения КА
	ZMSC KA ;
	// Инициализация модели движения
	rc = KA.Init(NU, &LSF); RRC;

	// Выход на начало витка НУ 
	rc = KA.GoToVit(Vit); RRC;
	// Сохранение состояния Модели движения на момент начала витка
	int HStory = 0;
	HStory = KA.Story(HStory); if (!HStory) return-1 ;

	//---------------------------------------------------------------------------
	// Численное решение относительно i уравнения (Н2-Н1)(i) = 0 
	//---------------------------------------------------------------------------
	while (1) {
		double dH[2] ;
		double epsi = 1e-2 ; // град
		for (int j=0; j<2; j++) {
			// Моделирование до аргумента широты 90 град
			ZSC_SI* FS ;
			rc = KA.GoToVitU(Vit, 90.0) ; RRC ;
			FS = &KA.FSR ; if (!FS || FS->E.ID!=ModEvent_ONARG) return-2 ;
			double H1 = FS->THBLFR[5] ; 
			// Моделирование до аргумента широты 270 град
			rc = KA.GoToVitU(Vit, 270.0) ; RRC ;
			FS = &KA.FSR ; if (!FS || FS->E.ID!=ModEvent_ONARG) return-3 ;
			double H2 = FS->THBLFR[5] ; 
			// Восстанавливаем состояние модели движения на момент начала витка
			rc = KA.ReStory(HStory) ; RRC ;

			// Отклонение высот
			dH[j] = H2-H1 ;
			//
			if (!j) {
				NU.X[2] = RadA(i+epsi) ;
				KA.SetVector(NU.X, NU.vit, NU.t, NU.M0+NU.MF, _SK_OSKTE) ;
		}	}
		// Производная разности высот по наклонению орбиты
		double ddH = (dH[1]-dH[0])/epsi ;
		// Поправка к наклоению, обеспечивающая нулевоую разность высот в 
		// противоположенных точках орбиты
		double di = dH[0]/ddH ;
		i+= di ;
		// Проверка условия завершения итераций
		if (fabs(dH[0]) < 0.001) break ;
		// Новое значение наклонения орбиты
		NU.X[2] = RadA(i) ;
		// Новый текущий вектор
		KA.SetVector(NU.X, NU.vit, NU.t, NU.M0+NU.MF, _SK_OSKTE) ;
	}

	// Освобождение памяти, выделяемой ранее для сохранения состояния модели
	if (HStory) KA.FreeStory(&HStory) ;
	return rc ;
}

//-------------------------------------------------------------------------------
// Проектирование орбиты ССО для заданного периода
int OrbitDesignSSOfromPeriod(double Tdr0) 
{
	double a0, e0, w0, i0, W0, u0 ;
	W0 = 0.0 ;
	u0 = 0.0 ;

	sprintf(FilePrintSticker, "T=%.0lf-%.2d", Tdr0, (int)(10*(Tdr0-(int)Tdr0))) ;

	// Нулевое приближение параметров орбиты
	OrbitDesignSSOStage1T(Tdr0, a0, e0, w0, i0) ; 
	// Уточнение значения полуоси по заданному периоду
	// численно в полном поле сил
	OrbitDesignSSOStage2T(&LSF, Tdr0, a0, e0, i0, W0, w0, u0, a0) ;
	// Уточнение наклонения ССО но новому значению большой полуоси
	OrbitDesignStage3 (a0, e0, w0, i0, Tdr0, i0) ; 
	//
	OrbitDesignSSOStage2T(&LSF, Tdr0, a0, e0, i0, W0, w0, u0, a0) ;
	OrbitDesignStage3 (a0, e0, w0, i0, Tdr0, i0) ; 
	// Прогноз и осреднение
	double es, ws, e1s, e2s ;
	OrbitDesignStage4 (a0, e0, i0, w0, W0, u0, es, ws, e1s, e2s) ; 
	// В качестве геометрии оскулирующей орбиты на начало витка принимаем
	// осреднённые параметры
	e0 = es ;
	w0 = ws ;
	// Просто прогноз
	OrbitDesignStage5 (a0, e0, i0, w0, W0, u0) ; 
	// Прогноз по виткам с построением высотного профиля
	OrbitDesignStage6 (a0, e0, i0, w0, W0, u0) ;
	printf("\n") ;

	return 0 ;
}

//-------------------------------------------------------------------------------
// Проектирование орбиты ССО для заданной кратности
int OrbitDesignSSOfromMultiplicity(int K, int L) 
{
	double Tdr0 ;
	double a0, e0, w0, i0, W0, u0 ;
	W0 = 0.0 ;
	u0 = 0.0 ;

	// Нулевое приближение параметров орбиты
	// Замыкание трассы за K суток и L витков.
	OrbitDesignSSOStage1kl(K, L, Tdr0, a0, e0, w0, i0) ;

	sprintf(FilePrintSticker, "KL=%d-%d_T=%.0lf-%.2d", K, L, Tdr0, (int)(10*(Tdr0-(int)Tdr0))) ;

	// Уточнение значения полуоси по заданному периоду
	// численно в полном поле сил
	OrbitDesignSSOStage2T(&LSF, Tdr0, a0, e0, i0, W0, w0, u0, a0) ;
	// Уточнение наклонения ССО но новому значению большой полуоси
	OrbitDesignStage3 (a0, e0, w0, i0, Tdr0, i0) ; 
	//
	OrbitDesignSSOStage2T(&LSF, Tdr0, a0, e0, i0, W0, w0, u0, a0) ;
	OrbitDesignStage3 (a0, e0, w0, i0, Tdr0, i0) ; 
	// Прогноз и осреднение
	double es, ws, e1s, e2s ;
	OrbitDesignStage4 (a0, e0, i0, w0, W0, u0, es, ws, e1s, e2s) ; 
	// В качестве геометрии оскулирующей орбиты на начало витка принимаем
	// осреднённые параметры
	// Просто прогноз
	double dL, dLs, Hmin, Hmax, Hs ; 
	OrbitDesignStage5 (a0, es, i0, ws, W0, u0, &dL, &dLs, &Hmin, &Hmax, &Hs) ; 
	// Прогноз по виткам с построением высотного профиля
	OrbitDesignStage6 (a0, es, i0, ws, W0, u0) ;
	printf("\n") ;

	// Документирование полученной орбиты
	mkdir("_Результаты") ;
	mkdir("_Результаты\\Проектирование орбит") ;
	FILE* fp = fopen( "_Результаты\\Проектирование орбит\\_OrbitDesignTabl.txt", "a+" ) ;
	fprintf(fp, "\n %5d	%5d	%.3lf	%.5lf	%.3lf	%.7lf	%.3lf	%.7lf	%.7lf	%.3lf	%.3lf	%.3lf	%.3lf	%.3lf", 
		    K, L, Tdr0, i0, a0, es, ws, e1s, e2s, Hmin, Hmax, Hs, dL, dLs) ;
	fclose(fp) ; fp = NULL ; 
	//
	return 0 ;
}

//-------------------------------------------------------------------------------
// Драконический период с учётом второй зональной гармоники
double CalcTdrC20(double a, double e1, double e2, double i)
{
	double si = Sin(i, 1) ;
	double ski= si*si ;
	double p  = a*(1-e1*e1-e2*e2) ;
	double T  = a*sqrt(a/mu)-eps/mu/sqrt(p*mu)*(3.0-2.5*ski-e1*(1.0-5.0*ski)) ;
	T*= k_pi*2.0 ;
	return T ;
}

//-------------------------------------------------------------------------------
// Наклонение ССО в первом приближении
double CalcISSO(double p, double Tdr)
{
	double i = 0 ;
	// Драконический период в ссс
	Tdr/= 86400.0 ;
	// Косинус наклонения ССО с учётом только второй зональной гармоники
	i =-Tdr*p*p/365.2422*mu/eps ;
	// Наклонение ССО в градусах
	i = Acos(i, 1) ;
	return i ;
}

//-------------------------------------------------------------------------------
// Вычисление компонент вектора эксцентриситета с учётом 7-и зональных гармоник
// для солнечно синхронной стационарно-высотной орбиты
// по заданному наклонению и большой полуоси
//		i0		- заданное наклонение 
//		a		- заданная полуось
//		e1, e2	- полученные компоненты вектора эксцентриситета
//		psi		- если указатель не нулевой, то sin(i0)
void CalcE12SSOfromI(double i0, double a, double& e1, double& e2, double* psi)
{
	double si = Sin(i0, 1) ;
	double si2= si*si ;
	// Коэффициенты "гамма"
	double gam2, gam3, gam5, gam7 ;
	CalcGam2357(a, gam2, gam3, gam5, gam7) ;
	// Новые значения компонент вектора эксцентриситета
	e1 = gam2*(1.5-si*si); 
	e2 = gam3 - 5.0/4.0*gam5*(8.0-28.0*si2+21.0*si2*si2)/
		 (4.0-5.0*si2) + 35.0/128.0*gam7*
		 (64.0-432.0*si2+792.0*si2*si2-429.0*si2*si2*si2)/(4.0-5.0*si2) ;
	e2*= si/(2.0*gam2) ;
	if (psi) *psi = si ;
}

//-------------------------------------------------------------------------------
// Вычисление характеристик орбиты стационарных высот по заданному
// наклонению и драконическому периоду
// 
// Вычисляются оскулирующих элементов на начало витка
// Заданы на момент начала витка: 
//		Атомное время			t0, 
//		Драконический период	Tdr0, 
//		Наклонение				i0, 
//		Абсолютная долгота ВУ	W0,
//		Большая полуось			a0 - как нулевое приближение.
// Результат решения элементы геометрически устойчивой орбиты: 
//		Большая полуось			a, 
//		Геометрия орбиты		e, w.
// В ходе решения предполагается что все элементы орбиты заданы относительно 
// экватора истинной текущей эпохи. Геометрия орбиты высичляется по  
// аналитическим соотношениям, а большая полуось уточняется численно исходя 
// из заданных драконического периода, абсолютной долготы и времени начала витка.
int OrbitDesignOSHfromTi(ZLSF* LSF, double Tdr0, double i0, double W0, double& a0, double& e, double& w) 
{
	int rc ;
	double Tdr, e1=0, e2=0, e1r=999, e2r=999 ;
	MATC20(i0, e1, e2, Tdr0, a0);
	CalcE12SSOfromI(i0, a0, e1, e2, NULL) ;
	double a = a0 ;

	while(1) {
		CalcE12SSOfromI(i0, a, e1, e2, NULL) ;
		e = sqrt(e1*e1+e2*e2) ;
		w = Atan2(e2, e1, 1) ;
		// Расчёт периода в полном поле сил
		rc = CalcPeriodInFullForce(LSF, a, e, i0,  W0, w, 0, Tdr) ; RRC ; 
		// Невязка по периоду
		double dT = Tdr0 - Tdr ; 
		// поправка к значению большой полуоси, 
		// корректирующая значение невязки по периоду dT
		double da = dT/3.0/k_pi*sqrt(k_mu_kms/a) ;
		// Новое значение большой полуоси
		a += da ;
		a0 = a ;
		// Проверка условия завершения итерационного процесса
		double de = sqrt((e1r-e1)*(e1r-e1)+(e2r-e2)*(e2r-e2)) ;
		if (fabs(dT)<1e-4 && fabs(de)<1e-8) break ;
		e1r = e1 ;
		e2r = e2 ;
	}

	return 0 ;
}


//-------------------------------------------------------------------------------
// Вычисление оскулирующих элементов на начало витка
// Задана средняя высота
// Результат решения элементы солнечно-синхронной геометрически устойчивой орбиты
int OrbitDesignSSOStage1h(double h0, double& a0, double& e0, double& w0, double& i0) 
{
	// Номер итерации
	int j = 0 ;
	// средняя полуось
	double aj = Rz+h0 ;
	a0 = aj ;
	// Текущее расчитываемое наклонение
	double ij ; 
	// Драконический период
	double Tdrj = 2*k_pi*aj*sqrt(aj/k_mu_kms) ;
	// Компоненты вектора эксцентриситета e*cos(w), e*sin(w)   
	double e1j  = 0.0 ;
	double e2j  = 0.0 ;
	// Фокальный параметр орбиты 
	double pj   = aj*(1-e1j*e1j-e2j*e2j) ;
	while(1) {
		// наклонение плоскости ССО
		ij = Tdrj/86400.0/365.2422*pj*pj*k_mu_kms/k_eps_kms ;
		ij = Acos(ij, 1) ;
		// Вычисление компонент вектора эксцентриситета с учётом 7-и
		// зональных гармоник, по значению полуоси и наклонению
		double e1j, e2j, si ;
		CalcE12SSOfromI(ij, aj, e1j, e2j, &si) ;
		// Новое значение полуоси
		aj  = pj/(1.0-e1j*e1j-e2j*e2j) ;  
		// Новое значение периода
		Tdrj= 2*k_pi*(aj*sqrt(aj/k_mu_kms) - k_eps_kms/k_mu_kms/sqrt(pj*k_mu_kms)* 
			  (3.0-2.5*si*si-e1j*(1.0-5.0*si*si)));
		// Проверка условия сходимости
		if (fabs(aj-a0)<1e-5) break ;
		// Новое значение полуоси
		a0 = aj ;
		// Увеличение счётчика итераций
		j++ ;
	}
	// Завершение расчёта
	a0 = aj ;
	e0 = sqrt(e1j*e1j+e2j*e2j) ;
	w0 = Atan2(e2j, e1j, 1) ;
	i0 = ij ;
	return 0 ;
}

//-------------------------------------------------------------------------------
// Вычисление оскулирующих элементов на начало витка
// Задан драконический период
// Результат решения элементы солнечно-синхронной геометрически устойчивой орбиты
int OrbitDesignSSOStage1T(double Tdr0, double& a0, double& e0, double& w0, double& i0) 
{
	// Номер итерации
	int j = 0 ;
	// Текущий расчитываемый период
	double Tdrj = Tdr0 ;
	// Текущее расчитываемое наклонение
	double ij ; 
	// средняя полуось
	double aj = pow(Tdrj*Tdrj*k_mu_kms/4.0/k_pi/k_pi, 1.0/3.0) ;
	// Компоненты вектора эксцентриситета e*cos(w), e*sin(w)   
	double e1j = 0.0 ;
	double e2j = 0.0 ;
	a0 = aj ;
	while(1) {
		// Фокальный параметр орбиты 
		double pj = aj*(1-e1j*e1j-e2j*e2j) ;
		// наклонение плоскости ССО
		ij =-Tdrj/86400.0/365.2422*aj*aj*k_mu_kms/k_eps_kms ;
		ij = Acos(ij, 1) ;
		// Вычисление компонент вектора эксцентриситета с учётом 7-и
		// зональных гармоник, по значению полуоси и наклонению
		double e1j, e2j, si ;
		CalcE12SSOfromI(ij, aj, e1j, e2j, &si) ;
		// Новое значение периода
		Tdrj= 2*k_pi*(aj*sqrt(aj/k_mu_kms) - k_eps_kms/k_mu_kms/sqrt(pj*k_mu_kms)* 
			  (3.0-2.5*si*si-e1j*(1.0-5.0*si*si)));
		// Проверка условия сходимости
		if (fabs(Tdrj-Tdr0)<1e-4) break ;
		// Новое значение полуоси
		aj+= (Tdr0-Tdrj)/3.0/k_pi*sqrt(k_mu_kms/aj) ; ;
		// Увеличение счётчика итераций
		j++ ;
	}
	// Завершение расчёта
	a0 = aj ;
	e0 = sqrt(e1j*e1j+e2j*e2j) ;
	w0 = Atan2(e2j, e1j, 1) ;
	i0 = ij ;
	return 0 ;
}

//-------------------------------------------------------------------------------
// Вычисление оскулирующих элементов на начало витка
// Заданы k-суточной кратности и с замыканием
// трассы через l полных витков.
// Результат решения элементы солнечно-синхронной геометрически устойчивой орбиты
int OrbitDesignSSOStage1kl(double k, double l, double& Tdr0, double& a0, double& e0, double& w0, double& i0) 
{
	Tdr0 = 86400.0*(double)k/(double)l ; 
	return(OrbitDesignSSOStage1T(Tdr0, a0, e0, w0, i0)) ; 
}

//-------------------------------------------------------------------------------
// Вычисление оскулирующих элементов на начало витка по заданному наклонению
// Результат решения - элементы солнечно-синхронной геометрически устойчивой орбиты
int OrbitDesignSSOStage12I(bool isStage2, double i0, double& a0, double& e0, double& w0, double& Tdr0) 
{
	// Для ССО всегда обратные наклонения 
	if (i0<90) i0 = 180.0-i0 ;
	//
	double ci = Cos(i0, 1) ;
	double aj  ;

	if (isStage2) {
		aj = a0 ;
	} else {
		aj = 365.2422*eps*fabs(ci)/2.0/k_dwapi/sqrt(mu)*86400.0 ;
		aj = pow(aj, 2.0/7.0) ;
	}

	while(1) {
		// Вычисление компонент вектора эксцентриситета с учётом 7-и
		// зональных гармоник, по значению полуоси и наклонению
		double e1j, e2j ;
		CalcE12SSOfromI(i0, aj, e1j, e2j, NULL) ;
		// Фокальный параметр по полуоси и эксцентриситету
		double pj = aj*(1-e1j*e1j-e2j*e2j) ;
		// Драконический период по значению полуоси в нормальном поле
		double Tj ;
		if (isStage2) {
			double ej = sqrt(e1j*e1j+e2j*e2j) ;
			double wj = Atan2(e2j, e1j, 1) ;
			int rc = CalcPeriodInFullForce(&LSF, aj, ej, i0, 0, wj, 0, Tj) ; RRC ; 
		} else {
			Tj = CalcTdrC20(aj, e1j, e2j, i0) ;
		}
		// Косинус наклонения по драконическому периоду
		double cij=-Tj/365.2422/86440.0*pj*pj*mu/eps ;
		// Отклонение косинуса заданного наклонения 
		// от косинуса расчитанного наклонения
		double dci= ci-cij ;
		// Новое значение полуоси
		aj-= dci*365.2422*86400.0*eps/7.0/k_pi/aj/aj/sqrt(aj*mu) ;
		// Новое значение рассчётного наклонения
		double ij = Acos(cij, 1) ;
		// Проверка условия завершения цикла
		// Расчётное наклонение ij дожно быть равно заданному i0
		if (fabs(ij-i0) < 0.00001) {
			Tdr0 = Tj ;
			a0 = aj ;
			e0 = sqrt(e1j*e1j+e2j*e2j) ;
			w0 = Atan2(e2j, e1j, 1) ;
			break ; 
	}	}
	return 0 ;
}

//-------------------------------------------------------------------------------
// Вычисление драконического периода в полном поле сил
int CalcPeriodInFullForce(ZLSF* LSF, double a0, double e0, double i0, double W0, double w0, double u0, double& Tdr) 
{
	int    rc ;

	// Начальные условия движения
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a0) ;
	NU.X[1] = e0 ;
	NU.X[2] = TO_RAD(i0) ;
	NU.X[3] = TO_RAD(W0) ;
	NU.X[4] = TO_RAD(w0) ;
	NU.X[5] = TO_RAD(u0) ;
	NU.Sb   = 0.0005 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

	// Осуществляется расчет драконического периода   
	// путем прогнозирования движения МКА в принятой модели движения.
	// Модель движения КА
	ZMSC KA ;
	// Инициализация модели движения
	rc = KA.Init(NU, LSF) ; RRC ;
	// Моделирование витка
	rc = KA.GoAlongVit(NU.vit) ; RRC ;
	// Значение драконического периода на смоделированном витке
	// Искомый драконический период
	Tdr = KA.FV.Td ; 
	return 0 ;
}

//-------------------------------------------------------------------------------
// Уточнение большой полуоси для заданного периода
int OrbitDesignSSOStage2T(ZLSF* LSF, double Tdr0, double a0, double e0, double i0, double W0, double w0, double u0, double& ak) 
{
	while (1) {
		double Tdr ;
		// Расчёт периода в полном поле сил
		int rc = CalcPeriodInFullForce(LSF, a0, e0, i0,  W0, w0, u0, Tdr) ; RRC ; 
		// Невязка по периоду
		double dT = Tdr0 - Tdr ; 
		// поправка к значению большой полуоси, 
		// корректирующая значение невязки по периоду dT
		double da = dT/3.0/k_pi*sqrt(k_mu_kms/a0) ;
		// Новое значение большой полуоси
		a0+= da ;
		// Проверка условия завершения итерационного процесса
		if (fabs(dT) <1e-4) break ;
	}
	ak = a0 ;	
	printf("Period = %.6lf  major semi-axis = %.6lf  ", Tdr0, ak) ;
	return 0 ;
}

//-------------------------------------------------------------------------------
// Уточнение наклонение ССО
//-------------------------------------------------------------------------------
double KW(double ps, double i)
{
	double K = 0 ;
	double ps2 = ps*ps ;
	double ps4 = ps2*ps2 ;
	double si  = Sin(i, 1) ;
	double ski = si*si ;
	K = C20+(3.0-20.0*ski)/4.0/ps2*C20*C20+
		35.0*(7.0*ski-4.0)/56.0/ps2*C40+
		35.0*(8.0-36.0*ski+33.0*ski*ski)/64.0/ps4*C60 ;
	K*= 3.0*k_pi*Cos(i, 1)/ps2 ;
	return K ;
}

double HW(double ps, double i)
{
	double H = 0 ;
	double si  = Sin(i, 1) ;
	double ski = si*si ;
	double ci  = Cos(i, 1) ;
	H = (15.0*ski-4.0)*C30+
		(8.0-84.0*ski+105*ski*ski)*5.0/4.0/ps/ps*C50 ;
	H*= 3.0*k_pi/4.0/ps/ps/ps*ci/si ;
	return H ;
}

double LW(double ps, double i)
{
	double L = 0 ;
	double si  = Sin(i, 1) ;
	double ski = si*si ;
	double ci  = Cos(i, 1) ;
	L = 6.0*k_pi*(2.0-5.0*ski)*ci*C20*C20/ps/ps/ps/ps ; 
	return L ;
}

double FW(double a, double e1, double e2, double i, double Tdr)
{
	double ps = a/(1-e1*e1-e2*e2)/Re ;
	double K = KW(ps, i) ;
	double H = HW(ps, i) ;
	double L = LW(ps, i) ;
	double F ;
	// Угловая скорость прецессии восходящего узла орбиты за виток полета 
	// (рад/виток) с учетом 6-ти зональных гармоник ГПЗ при заданных 
	// значениях элементов орбиты
	double dW = K+L*e2+H*e1 ; 
	// Драконическийпериод в ССС
	Tdr/= 86400.0 ;
	// Отклонение годичной прецессии плоскости орбиты от 2-а "пи" в радианах
	// (за период времени равный тропическому году)
	F = dW*365.2422/Tdr-2*k_pi ;
	return F ;
}

//-------------------------------------------------------------------------------
// Уточнение наклонения ССО исходя из условия синхронности в поле до 7-и
// зональных гармоник
int OrbitDesignStage3(double a0, double e0, double w0, double i0, double Tdr0, double& ik) 
{
	double e10 = e0*Cos(w0, 1) ;
	double e20 = e0*Sin(w0, 1) ;
	double p   = a0/(1-e10*e10-e20*e20) ;
	double F ;
	double si ; 
	double ij = i0 ;
	while (1) {
		// Отклонение годичной прецессии плоскости орбиты от 2-а "пи" в радианах
		F = FW(a0, e10, e20, ij, Tdr0) ;
		si = Sin(ij, 1) ;
		// Новое значение наклонения в радианах
		ij = RadA(ij) - F*mu*p*p*Tdr0/86400.0/2.0/k_pi/eps/365.2422/si ;
		// Новое значение наклонения в градусах
		ij = GradA(ij) ;
		// Проверка условия завершения итераций
		if (fabs(F)<1e-6) break ;
	}
	ik = ij ;

	printf("Inclination = %.9lf\n", ik) ;

	return 0 ;
}

//-------------------------------------------------------------------------------
// Усреднение вектора эксцентриситета
int OrbitDesignStage4(double a0, double e0, double i0, double w0, double W0, double u0, double& es, double& ws, double& e1s, double& e2s)
{
	printf("Averaging of the orbital parametrs at the turns beginning...\n") ;
	int rc = 0 ; 

	// Начальные условия движения
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a0) ;
	NU.X[1] = e0 ;
	NU.X[2] = TO_RAD(i0) ;
	NU.X[3] = TO_RAD(W0) ;
	NU.X[4] = TO_RAD(w0) ;
	NU.X[5] = TO_RAD(u0) ;
	NU.Sb   = 0.0005 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

	// Осуществляется расчет драконического периода   
	// путем прогнозирования движения МКА в принятой модели движения.
	// Модель движения КА
	ZMSC KA ;
	// Инициализация модели движения
	rc = KA.Init(NU, &LSF) ; RRC ;

	// Диспетчер документирования
	ZCPrintManager PrintMng ;
	// Добавляем стикер к имени файла документа
	PrintMng.SetPrintFileSticker(FilePrintSticker) ;
	// Отключить документирование прогноза по виткам
	PrintMng.MPK.FromVitOff() ;
	// Документирование результатов прогноза по строкам (горизонтально)
	PrintMng.MPK.AflatOn() ;
	// Параметризация ПОЛЬЗОВАТЕЛЬСКОГО докумеентирования результатов прогноза.
	// Указыватся: тип параметра, его точность (число знаков после запятой),
	// максимальное колличество символов для представления и название.
	// Значения документируемых параметров задаются далее при помощи
	// функции PMPK.UPP(0).value = ...
	// PrintMng.MPK << UserPrintParam(UPPdouble, 16, 6, "время", "часы") ;
	// PrintMng.MPK.UPP(0).value = 0 ;
	// Добавление документируемых параметров движения
	PrintMng.MPK << MODPRDATETIME ;
	PrintMng.MPK << MODPR_VIT ;
	PrintMng.MPK << MODPR_MSSV ;
	PrintMng.MPK << MODPR_T_iTE ;
	PrintMng.MPK << MODPR_T_WTE ;
	PrintMng.MPK << MODPR_T_wTE ;
	PrintMng.MPK << MODPR_T_uTE ;
	PrintMng.MPK << MODPR_H ;
	PrintMng.MPK << MODPR_Aosk ;
	PrintMng.MPK << MODPR_eosk ;
	PrintMng.MPK << MODPR_exosk ;
	PrintMng.MPK << MODPR_eyosk ;

	// Инициализация документирования
	PrintMng.PrePrint(&KA, "Проектирование орбит", "_OrbitDesign_") ;
	PrintMng.PrintHeaderDoc("\n   Прогноз параметров движения КА (TaskOrbitDesign)\n\n") ;
	// Документирования НУ
	PrintMng.PrintNU(NU, NULL, 3) ;
	// Документирование логической шкалы сил
	PrintMng.PrintLSF(LSF, 1) ;

	// Документирование параметров движения
	PrintMng.PrintMSC(&KA, NULL, NULL, NULL);

	int dVit = 100 ;
	int Vitn = NU.vit+1 ;
	int Vit  = Vitn ;
	int Vitk = Vitn + dVit ;
	// Цыкл моделирования 
	ZSC_SI SI ;
	double e1, e2, e1max=-999, e1min=999, e2max=-999, e2min=999 ;
	while(1) {
		printf("\rCompleted %.2lf%s    ", (double)(Vit-Vitn)/(double)dVit*100.0, "%") ;
		// Прогноз до начала текущего витка
		rc = KA.GoToVit(Vit, &SI); if (rc) break;
		e1 = SI.OSKTE[1]*cos(SI.OSKTE[4]) ;
		e2 = SI.OSKTE[1]*sin(SI.OSKTE[4]) ;
		if (e1>e1max) e1max = e1 ;
		if (e1<e1min) e1min = e1 ;
		if (e2>e2max) e2max = e2 ;
		if (e2<e2min) e2min = e2 ;
		// Документирование полученных параметров движения
		PrintMng.PrintMSC(&KA, &SI, NULL, NULL);
		// Проверка условия завершения численного интегрирования (прогноза)
		if (Vit==Vitk) break ;
		Vit++ ;
	}
	printf("\n") ;

	if (!rc) {
		e1s = (e1max+e1min)/2.0 ;
		e2s = (e2max+e2min)/2.0 ;
		es = sqrt(e1s*e1s+e2s*e2s) ;
		ws = Atan2(e2s, e1s, 1) ;
	}

	// Завершение документирования
	PrintMng.PostPrint() ; 
	PrintMng.ClosePrint();
	return rc ;
}

//-------------------------------------------------------------------------------
// Наследуем МД для перегрузки метода расчёта внешних дифференциальных уравнений
ZMSC_EXV_BEG(ZModHAvr)
	// Правые части внешних дифференциальных уравнений
	double GSK[6] ;
	int rc = ZASC1975toGSC(t, Xmain, GSK, 0) ;	RRC ;
	double HBL[3] ;
	ZGRtoHBL(GSK, HBL, 0, 0) ;
	Yexe[0] = 1e4*HBL[0] ;
ZMSC_END

//-------------------------------------------------------------------------------
// Простой прогноз
int OrbitDesignStage5(double a0, double e0, double i0, double w0, double W0, double u0, double* dL, double* dLs, double* Hmin, double* Hmax, double* Hs)
{
	printf("Motion prediction...\n") ;
	int rc = 0 ; 

	// Начальные условия движения
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a0) ;
	NU.X[1] = e0 ;
	NU.X[2] = TO_RAD(i0) ;
	NU.X[3] = TO_RAD(W0) ;
	NU.X[4] = TO_RAD(w0) ;
	NU.X[5] = TO_RAD(u0) ;
	NU.Sb   = 0.0005 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

	// Осуществляется расчет драконического периода   
	// путем прогнозирования движения МКА в принятой модели движения.
	// Модель движения КА
	ZMSC KA ;
	// Инициализация модели движения
	rc = KA.Init(NU, &LSF) ; RRC ;

	// Если необходим расчёт мин мах и средней высоты, 
	// то моделируем виток с учётом одного внешнего уравнения
	// для расчёта осреднённой на витке высоты (среднеинтегрального
	// значения).
	if (Hmin || Hmax || Hs || dLs || dL) {
		ZModHAvr KA ;
		// Задаём число внешних уравнений
		KA.SetExternalEquationsCount(1) ;
		// Инициализируем модель
		rc = KA.Init(NU, &LSF) ; RRC ;
		// Включаем интегрирование внешнего уравнения,
		// при этом задаются нулевые НУ для внешних уравнений
 		KA.ExternalEquationsOn(NULL) ;
		// Моделирование траектории полёта вдоль витка NU.vit
		KA.GoAlongVit(NU.vit) ;
		// Значение проинтегрированного внешнего уравнения
		double HAvr ;
		KA.ExternalEquationsOff(&HAvr) ;
		// Делим на драконический период чтобы получить 
		// среднеинтегральное значение параметра
		HAvr/= (KA.FV.Td/k_cbc) ;
		// Заменяем значение в массиве интегральных параметров витка
		KA.FV.HAvr = HAvr ;

		// Межвитковый сдвиг в градусах
		if (dL ) *dL = KA.FV.dL ;	
		// Суточное смещение трасы в градусах
		if (dLs) *dLs= KA.FV.dLs ;	

		// Выбор из локальных минимумов глобально минимальной высоты на витке
		ZSC_VP* HM1 = HMIN_VI(KA.FV) ;
		// Выбор из локальных максимумов глобально максимальной высоты на витке
		ZSC_VP* HM2 = HMAX_VI(KA.FV) ;
		if (Hmin) *Hmin = HM1->H ; 
		if (Hmax) *Hmax = HM2->H ; 
		if (Hs  ) *Hs   = KA.FV.HAvr ; 
	}

	// Диспетчер документирования
	ZCPrintManager PrintMng ;
	// Добавляем стикер к имени файла документа
	PrintMng.SetPrintFileSticker(FilePrintSticker) ;
	// Отключить документирование прогноза по виткам
	PrintMng.MPK.FromVitOff() ;
	// Документирование результатов прогноза по строкам (горизонтально)
	PrintMng.MPK.AflatOn() ;
	// Параметризация ПОЛЬЗОВАТЕЛЬСКОГО докумеентирования результатов прогноза.
	// Указыватся: тип параметра, его точность (число знаков после запятой),
	// максимальное колличество символов для представления и название.
	// Значения документируемых параметров задаются далее при помощи
	// функции PMPK.UPP(0).value = ...
	PrintMng.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "время", "часы") ;
	PrintMng.MPK.UPP(0).value = 0 ;
	// Добавление документируемых параметров движения
	PrintMng.MPK << MODPRDATETIME ;
	PrintMng.MPK << MODPR_MSSV ;
	PrintMng.MPK << MODPR_T_iTE ;
	PrintMng.MPK << MODPR_T_WTE ;
	PrintMng.MPK << MODPR_T_wTE ;
	PrintMng.MPK << MODPR_T_uTE ;
	PrintMng.MPK << MODPR_H ;
	PrintMng.MPK << MODPR_Aosk ;
	PrintMng.MPK << MODPR_eosk ;
	PrintMng.MPK << MODPR_exosk ;
	PrintMng.MPK << MODPR_eyosk ;

	// Инициализация документирования
	PrintMng.PrePrint(&KA, "Проектирование орбит", "_OrbitDesign") ;
	PrintMng.PrintHeaderDoc("\n   Прогноз параметров движения КА (TaskOrbitDesign)\n\n") ;
	// Документирования НУ
	PrintMng.PrintNU(NU, NULL, 3) ;
	// Документирование логической шкалы сил
	PrintMng.PrintLSF(LSF, 1) ;

	// Документирование параметров движения
	PrintMng.PrintMSC(&KA, NULL, NULL, NULL);
	// Интервал прогнозирования (7 суток)
	double dt= 3 ;
	// Текущее время
	double t = KA.GetTime() ;
	// Начальное время
	double tn = t ;
	// Конечное время время
	double tk= t+dt ;
	int   dir= tk>=t ? 1:-1 ;
	// Цыкл моделирования 
	ZSC_SI SI ;
	while(1) {
		printf("\rCompleted %.2lf%s    ", (t-tn)/dt*100.0, "%") ;
		// Проверка условия завершения численного интегрирования (прогноза)
		if (t>=tk) break ;
		// Один шаг интегратора с автоматическим выбором его величины, 
		// но не далее чем момент времени tk. То есть h <= tk-t, где
		// t есть текущее время
		rc = KA.GoStepA(&SI, dir, &tk) ; if (rc) break ;
		// Новое текущее время 
		t = SI.ta ;
		// Пользовательское докумеентирования результатов прогноза.
		// Задаётся значение дополнительного параметра,
		// декларируемого ранее при помощи вызова оператора 
		// PrintMng.MPK << UserPrintParam(UPPdouble...
		PrintMng.MPK.UPP(0).value = (t-tn)*24.0;
		// Документирование полученных параметров движения
		PrintMng.PrintMSC(&KA, &SI, NULL, NULL);
	}
	printf("\n") ;
	// Завершение документирования
	PrintMng.PostPrint() ; 
	PrintMng.ClosePrint();
	return 0 ;
}

//-------------------------------------------------------------------------------
// Прогноз высотного профиля
int OrbitDesignStage6(double a0, double e0, double i0, double w0, double W0, double u0)
{
	printf("Height frofile Model forcing...\n") ;
	int rc = 0 ; 

	// Начальные условия движения
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a0) ;
	NU.X[1] = e0 ;
	NU.X[2] = TO_RAD(i0) ;
	NU.X[3] = TO_RAD(W0) ;
	NU.X[4] = TO_RAD(w0) ;
	NU.X[5] = TO_RAD(u0) ;
	NU.Sb   = 0.001 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

	// Осуществляется расчет драконического периода   
	// путем прогнозирования движения МКА в принятой модели движения.
	// Модель движения КА
	ZMSC KA ;
	// Инициализация модели движения
	rc = KA.Init(NU, &LSF) ; RRC ;

	int CountStep = 180 ; 

	// Диспетчер документирования
	ZCPrintManager PrintMng ;
	// Добавляем стикер к имени файла документа
	PrintMng.SetPrintFileSticker(FilePrintSticker) ;
	// Отключить документирование прогноза по виткам
	PrintMng.MPK.FromVitOff() ;
	// Документирование результатов прогноза по строкам (горизонтально)
	PrintMng.MPK.AflatOn() ;
	// Добавление документируемых параметров движения
	//PrintMng.MPK << MODPR_VIT ;
	// Параметризация ПОЛЬЗОВАТЕЛЬСКОГО докумеентирования результатов прогноза.
	// Указыватся: тип параметра, его точность (число знаков после запятой),
	// максимальное колличество символов для представления и название.
	// Значения документируемых параметров задаются далее при помощи
	// функции PMPK.UPP(0).value = ...
	// PrintMng.MPK << UserPrintParam(UPPdouble, 16, 6, "время", "часы") ;
	// PrintMng.MPK.UPP(0).value = 0 ;
	for (int i=0 ; i<CountStep+1 ; i++) {  
		PrintMng.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "h", "км") ;
	}

	// Инициализация документирования
	PrintMng.PrePrint(&KA, "Проектирование орбит", "_OrbitDesignHProf") ;
	PrintMng.PrintHeaderDoc("\n   Прогноз параметров движения КА (TaskOrbitDesign)\n\n") ;
	// Документирования НУ
	PrintMng.PrintNU(NU, NULL, 3) ;
	// Документирование логической шкалы сил
	PrintMng.PrintLSF(LSF, 1) ;

	// Цыкл моделирования 
	ZSC_SI SI ;
	double U ;
	double du = 360.0/CountStep ; 
	int    dVit = 2500 ;
	int    Vitn = NU.vit+1 ;
	int    Vit  = Vitn ;
	int    Vitk = Vitn + dVit ;
	// Прогноз до начального витка
	rc = KA.GoToVit(Vitn, &SI); RRC;
	// Цикл моделирования движения по виткам
	while(1) {
		printf("\rTurn %d from %d   ", Vit-Vitn+1, dVit) ;
		// Высота для первой точки витка (начало витка)
		PrintMng.MPK.UPP(0).value = SI.THBLFR[1] ;
		U = du ;
		// Моделирование движения в пределах витка с заданным шагом du
		// по аргументу широты
		for (int i=1 ; i<CountStep ; i++) {  
			rc = KA.GoToVitU(Vit, U, &SI); if (rc) break;
			// Пользовательское докумеентирования результатов прогноза.
			// Задаётся значение дополнительного параметра,
			// декларируемого ранее при помощи вызова оператора 
			// PrintMng.MPK << UserPrintParam(UPPdouble...
			PrintMng.MPK.UPP(i).value = SI.THBLFR[1] ;
			U += du ;
		}
		// Если в цикле выше произошла ошибка то текущий цикл 
		// также прекращается
		if (rc) break ;
		// Увеличение номера витка
		Vit++ ;
		// Моделирование до начала следующего (конец текущего) витка
		rc = KA.GoToVit(Vit, &SI); if (rc) break;
		PrintMng.MPK.UPP(CountStep).value = SI.THBLFR[1] ;
		// Документирование полученных параметров движения
		// Высота для последней точки витка (конец витка)
		PrintMng.PrintMSC(&KA, &SI, NULL, NULL);
		// Проверка условия завершения численного интегрирования (прогноза)
		if (Vit==Vitk) break ;
	}
	printf("\n") ;
	// Завершение документирования
	PrintMng.PostPrint() ; 
	PrintMng.ClosePrint();
	return rc ;
}

//-------------------------------------------------------------------------------
// Прогноз высотного профиля упрощённый
int OrbitDesignStage7(double a0, double e0, double i0, double w0, double W0, double u0)
{
	printf("Height frofile Model forcing...\n") ;
	int i, rc = 0 ; 
	int CountStep = 720 ; 

	//---------------------------------------------------------------------------
	// Начальные условия движения
	ZNU NU ; 
	NU.t << DATETIMEDMB(1, 12, 2019, 0, 0, 0, 0);
	NU.Key  = ZKEYNU(1, 12, 4, 0, 1) ;
	NU.vit  = 1 ;
	NU.sk   = _SK_OSKTE ;
	NU.X[0] = TO_DTKM(a0) ;
	NU.X[1] = e0 ;
	NU.X[2] = TO_RAD(i0) ;
	NU.X[3] = TO_RAD(W0) ;
	NU.X[4] = TO_RAD(w0) ;
	NU.X[5] = TO_RAD(u0) ;
	NU.Sb   = 0.001 ;
	NU.Kp   = 0 ;
	NU.M0   = 1000 ;
	NU.MF   = 200 ;

	// Модель движения КА
	ZMSC KA ;
	// Инициализация модели движения
	rc = KA.Init(NU, &LSF) ; RRC ;

	//---------------------------------------------------------------------------
	// Настройка документирования основного
	// Диспетчер документирования
	ZCPrintManager PrintMng ;
	// Отключить документирование прогноза по виткам
	PrintMng.MPK.FromVitOff() ;
	// Документирование результатов прогноза по строкам (горизонтально)
	PrintMng.MPK.AflatOn() ;
	// Добавление документируемых параметров движения
	//PrintMng.MPK << MODPR_VIT ;
	// Параметризация ПОЛЬЗОВАТЕЛЬСКОГО докумеентирования результатов прогноза.
	// Указыватся: тип параметра, его точность (число знаков после запятой),
	// максимальное колличество символов для представления и название.
	// Значения документируемых параметров задаются далее при помощи
	// функции PMPK.UPP(0).value = ...
	// PrintMng.MPK << UserPrintParam(UPPdouble, 16, 6, "время", "часы") ;
	// PrintMng.MPK.UPP(0).value = 0 ;
	for (i=0 ; i<=CountStep ; i++) {  
		PrintMng.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "h", "км");
	}

	// Инициализация документирования
	PrintMng.PrePrint(&KA, "Проектирование орбит", "_OrbitDesignHProf") ;
	PrintMng.PrintHeaderDoc("\n   Высотный профиль (TaskOrbitDesign)\n\n") ;
	// Документирования НУ
	PrintMng.PrintNU(NU, NULL, 3) ;
	// Документирование логической шкалы сил
	PrintMng.PrintLSF(LSF, 1) ;

	//---------------------------------------------------------------------------
	// Настройка документирования вариаций минимальных и максимальных высот
	ZCPrintManager PrintMngH ;
	PrintMngH.MPK.FromVitOff() ;
	PrintMngH.MPK.AflatOn() ;
	PrintMngH.MPK << ZSUserPrintParam(UPPint, 16, 6, "Виток", "");
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmin1", "км");
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmin2", "км");
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmax1", "км");
	PrintMngH.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "Hmax2", "км");
	PrintMngH.PrePrint(&KA, "Проектирование орбит", "_OrbitDesignHminmax") ;
	PrintMngH.PrintHeaderDoc("\n   Зависимости минимальных/максимальных высот от времени полёта (TaskOrbitDesign)\n\n") ;
	// Документирования НУ
	PrintMngH.PrintNU(NU, NULL, 3) ;
	// Документирование логической шкалы сил
	PrintMngH.PrintLSF(LSF, 1) ;

	//---------------------------------------------------------------------------
	// Цыкл моделирования 
	ZSC_SI SI ;
	double U ;
	double du = 360.0/CountStep ; 
	// Интервал моделирования в сутках
	double maxTime = 366 ; 
	// Пропускать витков между витками на которых контролируютсся 
	// экстремальные высоты
	int    SkipVit = 15 ;
	// Первый виток моделирования будет виток следующий 
	// за витком начальных условий
	int    Vit = NU.vit+1 ;
	// Документирование значений аргумента широты для профиля высот
	U = 0 ;
	for(i=0 ; i<=CountStep ; i++) {
		PrintMng.MPK.UPP(i).value = U ;
		U+= du ;
	}
	PrintMng.PrintMSC(&KA, &SI, NULL, NULL);

	//---------------------------------------------------------------------------
	// Цикл моделирования движения на заданном интервале
	while(1) {
		// Моделирование до начало текущего витка
		rc = KA.GoToVit(Vit, &SI); if (rc) break;
		printf("\rTime %.1lf from %.1lf  ", SI.ta-NU.t, maxTime) ;
		// Высота для первой точки витка (начало витка)
		PrintMng.MPK.UPP(0).value = SI.THBLFR[1] ;
		// Инициализация отслеживания событий витка (мин и макс высоты)
		KA.RunUpVit() ;
		//-----------------------------------------------------------------------
		// Моделирование движения в пределах ПРИМЕРНО витка 
		// с заданным шагом du по аргументу широты
		U = du ;
		// Цикл моделирования превышающий виток для гарантированого
		// определения 2-х локально минимальных и 2-х
		// локально максимальных высот
		// моделирования прерывается в любом случае м моент когда
		// найдены все 4-е экстремальные высоты
		// Характерной особенностью околокруговых орбит с перицентром
		// в окрестности экватора является то что на витке может быть
		// максимальных или минимальных высот от 1-й до 3-х
		for (int i=1 ; i<=2*CountStep ; i++) {  
			rc = KA.GoToVitU(Vit, U, &SI); if (rc) break;
			if (i<=CountStep) {
				// Пользовательское докумеентирования результатов прогноза.
				// Задаётся значение дополнительного параметра,
				// декларируемого ранее при помощи вызова оператора 
				// PrintMng.MPK << UserPrintParam(UPPdouble...
				PrintMng.MPK.UPP(i).value = SI.THBLFR[1] ;
			}
			U += du ;
			// Проверка обнаружения всех 4-х высот 
			// и одновременно моделирования всего витка
			// Если виток закончен а одна из высот не найдена то
			// будет реализовано 1-2-3 шага интегрирования и найдено ближайшее
			// недостающее значение экстремальной высоты
			if (KA.FV.Hmin.isFind && KA.FV.Hmin2.isFind &&
				KA.FV.Hmax.isFind && KA.FV.Hmax2.isFind &&
				i>=CountStep) {
				break ;
			}
		}
		// Завершение отслеживания событий витка
		KA.CompletVit() ;
		// Если в цикле выше произошла ошибка то текущий цикл 
		// также прекращается
		if (rc) break ;

		// Документирование полученных параметров движения
		// Записывается в документ одна строка профиля высот
		PrintMng.PrintMSC(&KA, &SI, NULL, NULL);

		//-----------------------------------------------------------------------
		// Документирование результатов рассчёта экстремальных высот
		if (KA.FV.Hmin.ute>100.0 && KA.FV.Hmin.ute<260.0) {
			ZSC_VP HM   = KA.FV.Hmin ;
			KA.FV.Hmin  = KA.FV.Hmin2 ;
			KA.FV.Hmin2 = HM ;
		}
		PrintMngH.MPK.UPP(0).value = Vit ;
		PrintMngH.MPK.UPP(1).value = KA.FV.Hmin.H ; 
		PrintMngH.MPK.UPP(2).value = KA.FV.Hmin2.H ; 
		PrintMngH.MPK.UPP(3).value = KA.FV.Hmax.H ; 
		PrintMngH.MPK.UPP(4).value = KA.FV.Hmax2.H ; 
		PrintMngH.PrintMSC(&KA, &SI, NULL, NULL);

		//-----------------------------------------------------------------------
		// Проверка условия завершения численного интегрирования (прогноза)
		if (SI.ta > NU.t+maxTime) break ;

		//-----------------------------------------------------------------------
		// Увеличение номера витка
		Vit++ ;
		// Пропукаем 15 витков
		Vit+= SkipVit ;
	}
	printf("\n") ;

	//---------------------------------------------------------------------------
	// Завершение документирования
	PrintMng.PostPrint(); 
	PrintMng.ClosePrint();
	PrintMngH.PostPrint(); 
	PrintMngH.ClosePrint();

	return rc ;
}

//-------------------------------------------------------------------------------
