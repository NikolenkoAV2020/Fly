//---------------------------------------------------------------------------
// Аналитический астрономический ежегодник
#include <stdafx.h>
#pragma   hdrstop
//---------------------------------------------------------------------------
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ZNUT.h>
#include <FlyCoreSource\\ZPR.h>

//---------------------------------------------------------------------------
#define razm 16
//---------------------------------------------------------------------------
// Параметры для модели движения полюса
#define radius   0.     
#define omega    0.
#define faza     0.
#define polus_x  0.
#define polus_y  0.
//---------------------------------------------------------------------------
double t_UT1( double t_TDT ) ;

//---------------------------------------------------------------------------
// Внутренние функции аналитического АЕ
short  astro_interp(double t_DNO,double *astro_koord,short vid_info);
double delta_ta(double t_TDT);
short  polus(double *p,double tau);
double S0m(double ut2000);
short  planetTIME (double tdb2000,double * rez,short planet,short sk);
double kep_f(double e,double l);
void   sunTIME(double time,double *xs,short prizn);
void   nutTIME (double tau,double *XN);
void   lunaTIME (double time,double *xl,short iprizn);
//---------------------------------------------------------------------------
short ERR_ID(char *name_function)
{

   return 18;
}
//---------------------------------------------------------------------------
// КОРНЕВОЙ БЛОК АНАЛИТИЧЕСКОГО ЕЖЕГОДНИКА
//	Тае  -  требуемое время в ссс от 1975 года в атомном времени
//	N    -  номер параметра
//---------------------------------------------------------------------------
short ANAL_AE(double Tae,short N)
{
	short const static nom_planet[10]={2,1,7,8,9,4,6,10,3,11};
	double tt,*kp;
	short i;

    if (N<=10)  goto planet;
	if (N==11) goto pvz;

	if ((i=astro_interp(Tae,Wae->qnu,53))!=0) goto error;
	for (i = 3; --i >= 0; Wae->qnu[i] /= M_SEKRAD);
vyhod:
	Wae->t[N]=Tae;
	Wae->metod[N]=1;
	return 0;
pvz:
	Wae->pvz[0]=(del_TDT-delta_ta(tt=t_TDT(DELTA_2000+Tae)))/k_cbc;
	if ((i=polus(Wae->pvz+1,tau2000(tt)))!=0) goto error;
	for (i = 2; --i >= 0; Wae->pvz[i + 1] /= M_SEKRAD);
	goto vyhod;
planet:
	kp=Wae->qso+3*N;
	if ((i=astro_interp(Tae,kp,(short)(64+nom_planet[N])))!=0) goto error;
	if (N<=1) goto perevod;
	for (i=3;--i>=0;kp[i]+=Wae->qso[i]);
perevod:
	{
	double zkp[3] ; memcpy(&zkp, kp, 3*sizeof(double)) ;
	ZASC_2000to1975(zkp, kp, false) ;
	}
	//assert(0);
	//ask1_ask2(kp,kp,1,Wae->qpr);
	goto vyhod;
error:	return i;
}
//---------------------------------------------------------------------------
// ГЛАВНЫЙ УПРАВЛЯЮЩИЙ И ИНСТРУМЕНТАЛЬНЫЙ БЛОК
//---------------------------------------------------------------------------
short astro_interp(double t_DNO,double *astro_koord,short vid_info)
/***************************************************************************
*								           *
*            vid_info += 32   -  alfa,delta,r;				   *
*	     vid_info += 64   -  X,Y,Z; 				   *
* 	     vid_info += 96   -  alfa,delta,r,X,Y,Z;			   *
*            vid_info += 128  -  t,alfa,delta,r,V_alfa,V_delta,V_r;        *
*            vid_info += 256  -  подавление интерполяции (прямой расчет);  *
*            vid_info =1      -  Луна         АСКС  геоцентрическая        *
*                     =2      -  Солнце       АСКС  геоцентрическая	   *
*                     =3      -  Меркурий     АСКС  гелиоцентрическая      *
*		      =4      -  Венера       АСКС  гелиоцентрическая      *
*		      =5      -  Земля+Луна   АСКС  гелиоцентрическая      *
*		      =6      -  Марс         АСКС  гелиоцентрическая      *
*		      =7      -  Юпитер       АСКС  гелиоцентрическая      *
*		      =8      -  Сатурн       АСКС  гелиоцентрическая      *
*		      =9      -  Уран         АСКС  гелиоцентрическая      *
*		      =10     -  Нептун       АСКС  гелиоцентрическая      *
*		      =11     -  Плутон       АСКС  гелиоцентрическая      *
*		      =20     -  Параметры прецессии       (секунды)       *
*		      =21     -  Параметры нутации         (секунды)       *
*		      =22     -  Cреднее зв. время и полюса(секунды)       *
*								           *
***************************************************************************/
{
	double t2000;
	static const double dltluna=0.25;
	static char init=0;
	char jtek=razm;
/* Здесь планировалось динамическое выделение памяти */
	static double arhiv[razm][7];
	static char   nomer[razm];
	static char   info[razm];

	short i,j,jmin,sk,jmax,metka;
	double t,tmin,tmax,ttek,dltant;
	double *arhtek, *mas;

/*   инициализация буфера в памяти */
t2000=t_DNO+DELTA_2000;
if (init != 0)goto met1;

/*	if ((arhiv=(double **)(malloc(sizeof(double)*razm*7)))==nullptr)||
	    (nomer=(char *)(malloc(sizeof(char)*razm)))==nullptr)||
	    (info=(char *)(malloc(sizeof(char)*razm)))==nullptr))
		{j=-20;goto konec;}
*/

	for (i=razm;--i>=0;
		nomer[i]=razm);
	init=1;

/*   подготовка к просмотру массивов архива   */

met1:   sk=vid_info;
	sk/=32;
	vid_info-=sk*32;
	if (sk>=8) /*  прямой расчет без интерполяции */
		{
		sk-=8;
		ttek=t2000;
		arhtek=astro_koord;
		metka=4;
		goto astrTIME;
		}

/* установка времен для интерполяционных массивов */
	if (vid_info == 1) dltant=dltluna;
		else dltant=1;
	t=floor(t2000/dltant)*dltant;
	tmin=t-dltant;
	tmax=t+dltant;
	j=jmin=jmax=i=razm;

/*   цикл поиска необходимых массивов  */
for (;--i>=0;)
	{
	jtek=nomer[i];
	nomer[i]=(char)j;
	if (jtek==razm)
		/*нужный массив не найден,есть еще незаполненные */
		{
		jtek=(char)i;
		goto met2;
		}
	if (!(info[jtek] - vid_info))
		{
		if ((ttek=arhiv[jtek][0])==t) goto met3; /* нужный массив */
		if (ttek==tmin) jmin=jtek; /* массив раньше нужного */
		if (ttek==tmax) jmax=jtek; /* массив позже нужного */
		}
	j=jtek;
	}

/*   заполнение jtek на время t  */
met2:
arhtek=(double *)(arhiv[jtek]);
*(arhtek++)=t;

if (jmin==razm) /* если нет массива раньше нужного */
	{
	ttek=t;
	metka=5;
	goto astrTIME;
met5:	arhtek+=3;
	}
   else {      /*  если есть массив раньше нужного */
	mas=(double *)(arhiv[jmin])+1;
	for (i=3;--i>=0;*(arhtek++)=
	mas[2-i]+mas[5-i]*dltant);
	if (vid_info==22) arhtek[-3]-=k_cbc*floor(arhtek[-3]/k_cbc);
	if (vid_info<20) arhtek[-3]-=k_dwapi*floor(arhtek[-3]/k_dwapi);
	}
if ((jmax==razm)||(jmax==jtek)) /* если нет массива позже нужного */
	{
	ttek=tmax;
	metka=6;
	goto astrTIME;
	}
   else { /* если есть массив позже нужного */
	mas=(double *)(arhiv[jmax])+1;
	memmove(arhtek,mas,sizeof(double)*3);
	}
met6:


/* расчет средней скорости изменения */
if (vid_info==22) *arhtek+=k_cbc;
arhtek-=3;
for (i=3;--i>=0;arhtek++) arhtek[3]=(arhtek[3]-(*arhtek))/dltant;
if ((vid_info<20)&&(*arhtek<0)) *arhtek+=k_dwapi/dltant;
if ((vid_info==22)&&(*arhtek<k_cbc)) *arhtek+=k_cbc;
info[jtek]=(char)vid_info;

/***   Формирование результата на момент t2000  ***/
met3:
	nomer[razm-1]=jtek;
	arhtek=(double *)(arhiv[jtek]);
	if ((sk>4)||(sk<1))
		{
		j=ERR_ID("astro_interp");
		goto konec;
		}
	if (sk==4)
		{
		memmove(astro_koord,arhtek,sizeof(double)*7);
		goto met7;
		}
	t=t2000-t;
	for (i=3;--i>=0;
		   //*(astro_koord++)=*(++arhtek)+arhtek[3]*t);
		   astro_koord[i]=arhtek[i+1]+arhtek[i+4]*t);
	//astro_koord-=3;
met4:	if ((sk!=1)&&(vid_info<20))
		{
  /*   Эта часть только для координат небесных тел  */
		t=cos(tmax=astro_koord[1]);
		tmin=sin(tmax);
		if (sk != 3) sk=0;
		astro_koord[1+sk]=sin(tmax=(*astro_koord))*(t=
			t*(ttek=astro_koord[2]));
		astro_koord[sk]=t*cos(tmax);
		astro_koord[2+sk]=ttek*tmin;
		}
met7:	j=0;
konec:	if (j!=0) init=0;
	return (j);
astrTIME:
	if (vid_info == 2) sunTIME(t_TDT(ttek),arhtek,1);
	if (vid_info == 1) lunaTIME(t_TDT(ttek),arhtek,0);
	if (vid_info == 20) { 
		// тут ttek отсчитывается в ссс от 0h 01.01.2000
		ZPrecessJ2000( t_TDT(ttek), arhtek, TIME1975ASD) ;
		//preces_IERS(J_ask0,tau2000(t_TDT(ttek))-J_ask0,arhtek);
	}
	if (vid_info == 21)
		{
		nutTIME(ttek=tau2000(t_TDT(ttek)),arhtek+1);
		(*arhtek)=sr_E0_IERS(ttek);
		}
	if (vid_info == 22)
		{
		(*arhtek)=S0m(t_UT1(t_TDT(ttek)));
		if ((j=polus(arhtek+1,tau2000(t_TDT(ttek))))!=0) goto konec;
		}
	if ((vid_info >=3)&&(vid_info<11))
	      if ((j=planetTIME(t_TDT(ttek),arhtek,(short)(vid_info-2),1))!=0) goto konec;
	if (vid_info<20)
		{
		ttek=arhtek[0]*arhtek[0]+arhtek[1]*arhtek[1];
		arhtek[0]=dargum(arhtek[0],arhtek[1]);
		arhtek[1]=dargum(sqrt(ttek),arhtek[2]);
		arhtek[2]=sqrt(ttek+arhtek[2]*arhtek[2]);
		}
switch(metka)
	{
	case 4 : goto met4;
	case 5 : goto met5;
	case 6 : goto met6;
	}
        return 0;
}



double t_TDB(double t_TDT)
{ // Перевод в барицентрическое время (сутки)
	double g;
	g=(35999.050l*(tau2000(t_TDT))+357.528)*k_pi/180;
	return (t_TDT+(0.001658/k_cbc)*sin(g+0.0167*sin(g)));
}


double delta_ta(double t_TDT)
{ // Поправка от земного динамического времени ко всемирному (заглушка)
	double del_ta,X0;
	static double A[12]={
		29.19159889221191-5.697624292224646E-004,
		2.34073749743402E-003,
		0.,
		3.490658709779382E-004,
		-5.952176079154015E-002,
		0.4119031429290771,
		-0.3380990326404572,
		1.408003091812134,
		-0.3036867082118988,
		0.1519049555063248,
		-4.873392730951309E-002,
		2.112182229757309E-002},

		X=-1.60709996e+04; /* кол-во ccc от 2000г до 1956г */

		X0=t_TDT-X;
		del_ta=X0*(X0*A[2]+A[1])+A[0]
		+A[4]*sin(A[3]*X0)+A[5]*cos(A[3]*X0)
		+A[6]*sin(A[3]*2*X0)+A[7]*cos(A[3]*2*X0)
		+A[8]*sin(A[3]*3*X0)+A[9]*cos(A[3]*3*X0)
		+A[10]*sin(A[3]*8*X0)+A[11]*cos(A[3]*8*X0);
	return (del_ta);
}


double t_UT1(double t_TDT)
{ // Расчет всемирного времени по динамическому
	return (t_TDT-delta_ta(t_TDT)/k_cbc);
}

short polus(double *p, double tau)
{ // Расчет координат полюсов (заглушка)
	p[0]=polus_x+radius*cos(tau*omega+faza);
	p[1]=polus_y+radius*sin(tau*omega+faza);
	return 0;
}

double S0m(double ut2000)
{	// Расчет среднего звездного времени по всемирному в сутках от нуля часов 01.01.2000 г.
	// Результат в часовых секундах

	// Перевод от 00h00m00s во время от J2000.0
	double s=ut2000-0.5; 
	double tau=s/36525.;
	s=(s*236.555367908+(0.093104-(6.2e-6)*tau)*tau*tau+24110.54841)/k_cbc+
		(ut2000-floor(ut2000));

	s=(s*236.555367908+(0.093104-(6.2e-6)*tau)*tau*tau + 24110.54841)/k_cbc+
		(ut2000-floor(ut2000));
	return (s-floor(s));
}

short planetTIME (double tdb2000, double *rez, short planet, short sk)
{	// tdb2000 - Барицентрическое динамическое время в сутках от 0 часов 1.01.2000г.
	// sk = 0  - asks со скоростями
	//    = 1	- asks только координаты
	//    = 29 - nsk 2
	// planet = 1  - Меркурий
	// 	 2  - Венера
	// 	 3  - Земля+Луна
	// 	 4  - Марс
	// 	 5  - Юпитер
	// 	 6  - Сатурн
	// 	 7  - Уран
	//   8  - Нептун
	//   9  - Плутон
	// rez  - Результат в радианах,дткм-ах,ссс-ах на текущее время
	//        в гелиоцентрической эклиптической СК текущей эпохи.
	// При нормальном завершении возвращает нуль
	static double Aed=1.4959787e+4l;  /* Астрономическая еденица дткм */
	//static double Ms=1.32712438e-1l*86400L*86400L;
				/* Гравит. пост. Солнца дткм,ссс */

	//static double radsek=6.283185307179586l/1296000l;/* радиан в секунде */
	static double kft[9][21]=
		{
			{ // Меркурий
			(252.*60+15)*60+3.26,538106660.097,1.0943,0.0001,
			(77.*60+27)*60+22.029,5603.318,1.0652,0.0002,
			(48.*60+19)*60+51.215,4270.279,0.6332,0.0008,
			7.*60*60+17.951,6.557,-0.0651,-0.0002,
			0.205631752,0.000020406,-0.0000000284,
			0.38709830982,0
			},
			{ // Венера
			(181.*60+58)*60+47.283,210669166.909,1.1182,0.0001,
			(131.*60+33)*60+49.346,5047.994,-3.8618,-0.0189,
			(76.*60+40)*60+47.713,3244.033,1.4639,-0.0003,
			(3.*60+23)*60+40.783,3.613,-0.0032,0,
			0.006771882,-0.0000477657,-0.0000000975,
			0.72332981996,0
			},
			{ // Земля + Луна
			(100.*60+27)*60+59.215,129602771.363,1.0932,0.0001,
			(102.*60+56)*60+14.453,6190.339,1.655,0.0014,
			0,0,0,0,
			0,0,0,0,
			0.016708617,-0.00004204,-0.0000001236,
			1.00000101778,0
			},
			{ // Марс
			(355.*60+25)*60+59.789,68910107.309,1.1195,0.0001,
			(336.*60+3)*60+36.842,6627.759,0.4864,0.001,
			(49.*60+33)*60+29.136,2779.544,0.0578,0.0082,
			(1.*60+50)*60+59.015,-2.164,0.0459,0,
			0.093400620,0.0000904825,-0.0000000805,
			1.52367934191,0
			},
			{ // Юпитер
			(34.*60+21)*60+5.342,10930690.04,0.8055,0.0001,
			(14.*60+19)*60+52.713,5805.497,3.7132,-0.0159,
			(100.*60+27)*60+51.986,3675.433,1.444,0.0021,
			(1.*60+18)*60+11.771,-19.788,0.0167,0,
			0.048494851,0.0001632427,-0.0000004719,
			5.20260319132,0.000000191323
			},
			{ // Сатурн
			(50.*60+4)*60+38.897,4404639.651,1.8703,0,
			(93.*60+3)*60+24.434,7069.538,3.015,0.0181,
			(113.*60+39)*60+55.885,3157.539,-0.4347,-0.0084,
			(2.*60+29)*60+19.961,-13.45,-0.0546,0.0003,
			0.055508622,-0.0003468187,-0.0000006457,
			9.55490959574,-0.000002138917
			},
			{ // Уран
			(314.*60+3)*60+18.018,1547510.602,1.0956,0.0001,
			(173.*60+0)*60+18.573,5350.965,0.7722,0.0015,
			(74.*60+0)*60+21.410,1876.056,4.8236,0.0666,
			46.*60+23.506,2.788,0.135,-0.0003,
			0.046295899,-0.0000273381,0.0000000791,
			19.21844606178,-0.000000037163
			},
			{ // Нептун
			(304.*60+20)*60+55.196,791579.913,1.1133,0.0001,
			(48.*60+7)*60+25.286,5134.572,1.3649,-0.0001,
			(131.*60+47)*60+2.605,3967.929,.9359,-0.0022,
			(1.*60+46)*60+11.828,-33.51,-0.0255,0.0001,
			0.008988095,0.0000064076,-0.0000000008,
			30.11038686942,-0.000000166346
			},
			{ // Плутон  (достоверность невысокая)
			(165.65659+1.452602*50)*60*60+33.15*0.25,145.2602*60*60+33.15,33.15,0,
			222.913*60*60-37.692*50-1382.95*0.25,-3769.2-1382.95,-1382.95,0,
			109.63598*60*60+0.21*50+11.72*0.25,2.1+11.72,11.72,0,
			17.08535*60*60+0.313*50+10.87*0.25,31.3+10.87,10.87,0,
			0.248248+0.0004971*0.5+0.0005632*0.25,0.0004971+0.0005632,0.0005632,
			39.5294024,0
			}
		};
	double osk[6];
	double *v;
	double *k;
	short i,j,jk=21;
	tdb2000=(tdb2000-0.5)/36525L;
	if (sk==29) v=rez;else v=osk;

	if ((planet<1)||(planet>9)) return 15 ;

	k=kft[planet-1];
	for (i=0;i<6;i++)
	{
		j=3;
		if (i == 1) j=2;
		if (i == 0) j=1;
		v[i]=k[--jk];
		for (;--j>=0;v[i]=v[i]*tdb2000+k[--jk]);
	}
	for (i=2;i<6;i++)
	{
		v[i]-=floor(v[i]/1296000l)*1296000l;
		v[i] /= M_SEKRAD;
	}
	v[0]*=Aed;
	i=1;
	if (sk==29) 
	{
		return 0;
	}
	else if ((sk==0)||(sk==1)) 
	{
		ask_nsk(rez,v,(short)(sk+1),&i,2,k_gs);
		return 0;
	}
	return ERR_ID("planetTIME");
}

void sunTIME(double time,double *xs,short prizn)
{ //	time - время в TDT в сутках от 0 часов 1.01.2000 г.
	//	xs   - массив результатов
	//	prizn = 0 -альфа,дельта,радиус
	//	     != 0 -X,Y,Z
	static const double koef[13]={
		1.0932,129602771.363,361679.215,
		1.655, 6190.339,     370574.453,
		-0.1236e-6,-.4204e-4,0.016708617,
		-20.49,648000.0,
		1.4959787e+4,1.00000101778
		};
	const double *kf;
	double arg[3],x,y,t;
	short i;
	t=(time-0.5)/36525.;
	kf=koef;
	for (i=0;i<3;kf+=3)
		arg[i++]=(kf[0]*t+kf[1])*t+kf[2];
	(*arg) = kep_f(arg[2], (arg[0] - arg[1]) / M_SEKRAD);
	x=1-arg[2]*arg[2];
	y=1+arg[2]*cos(arg[0]);
	arg[1] = arg[0] + (arg[1] + kf[0] * y + kf[1]) / M_SEKRAD;
	x*=kf[2]*kf[3]/y;
	y = sr_E0_IERS(t) / M_SEKRAD;
	xs[0]=x*cos(arg[1]);
	xs[1]=(t=x*sin(arg[1]))*cos(y);
	xs[2]=t*sin(y);
	if (prizn!=0) goto ret;
	t=atan2(xs[2],sqrt(xs[0]*xs[0]+xs[1]*xs[1]));
	xs[2]=x;
	xs[0]=atan2(xs[1],xs[0]);
	xs[1]=t;
ret:	return;
}

void nutTIME (double tau,double *XN)
{ //	Расчет параметров нутации на tau от эпохи J2000.0 в столетиях
	//	XN[0] - нутация по долготе в угловых секундах
	//	XN[1] - нутация наклона в угловых секундах

	static const float am[37]={
			 712.0e0f,-7.0e0f,1426.0e0f,54.0e0f,-386.0e0f,
			 200.0e0f,63.0e0f,-2.0e0f,-171996.0e0f,92025.0e0f,
			 -174.2e0f,129.0e0f,-70.0e0f,-13187.0e0f,5736.0e0f,
			 -517.0e0f,224.0e0f,217.0e0f,-95.0e0f,2062.0e0f,
			 -895.0e0f,-51.0e0f,27.0e0f,-2274.0e0f,977.0e0f,
			 -301.0e0f,129.0e0f,123.0e0f,-53.0e0f,-59.0e0f,
			 26.0e0f,-158.0e0f,-1.0e0f,63.0e0f,-33.0e0f,
			 -58.0e0f,32.0e0f};
		float S[6],C[6];
		double U[5],XND,XNN,X,Y;
	static const short LOGIK[13]={
			 01346,0656,0260,01620,0550,
			 0130,0353,0130,01313,0340,
			 01140,0150,01510};
		short I,I1,I2,I3,I4,J;
	#ifdef NUT_OTL
		short K1[7][5],K;
	#endif
		short METKA;
	#ifdef NUT_OTL
				FILE *file1;
		if ((file1=fopen("NUTD.REZ","wt"))==nullptr) goto error;
	#endif
	fargTIME(tau,U);
				U[2]=2*U[2]+U[4];
				U[3]=2*U[3];
	#ifdef NUT_OTL
				for (I=0;I<7;I++)
				for (J=0;J<5;J++)
		K1[I][J]=0;
				K1[0][0]=1;
				K1[1][1]=1;
				K1[4][4]=1;
				K1[2][4]=1;
				K1[3][3]=2;
				K1[2][2]=2;
	#endif
				XNN=0;
				XND=0;
				I=-1;
				J=0;
				METKA=9;
			met9: I=I+1;
				if (I==5) goto met15;
				X=U[I];
				Y=sin(X);
				X=cos(X);
			met1: XND=am[J]*Y+XND;
				J=J+1;
				XNN=am[J]*X+XNN;
				J=J+1;
				if (I<0) goto met8;
				S[I]=(float)Y;
				C[I]=(float)X;
				if (METKA==8) goto met8;
		else goto met9;
		 met15:
				XND=am[J]*tau*S[4]+XND;
				J=J+1;
				METKA=8;
				I4=-1;
			met8: I4=I4++;
				if (I4==13) goto met7;
				I=-1;
				I2=-1;
				I1=LOGIK[I4];
			met2: I3=I1/8;
				I1=I1-I3*8;
				if (I>=0) goto met3;
				I=I2;
				I2=I1;
				I1=I3;
				goto met2;
			met3: if (I3>0) goto met4;
				Y=-1;
				goto met5;
			met4: Y=1;
			met5:I1--;
		 I2--;
		 I--;
		 X=C[I1]*C[I2]+Y*S[I2]*S[I1];
	#ifdef NUT_OTL
				for (K=0;K<5;K++)
				K1[6][K]=K1[I1][K]-(short)(Y)*K1[I2][K];
			for (K=0;K<5;K++)
			if (fprintf(file1,"%5d",K1[6][K])==EOF) goto error;
			if (fprintf(file1," %10f %10f \n",am[J],am[J+1])==EOF) goto error;
	#endif
				if(I==-1) goto met6;
	#ifdef NUT_OTL
				for (K=0;K<5;K++)
				K1[I][K]=K1[6][K];
	#endif
			met6:Y=S[I1]*C[I2]-Y*S[I2]*C[I1];
				goto met1;
			met7:
				XN[0]=XND*1.0e-4;
				XN[1]=XNN*1.0e-4;
	#ifdef NUT_OTL
	if (fclose(file1)==EOF) goto error;
	#endif
			 return;
	#ifdef NUT_OTL
	 error: fprintf(stderr,form_err);
		exit(1);
	#endif
}

void lunaTIME (double time,double *xl,short iprizn)
{ //	time   -  время в шкале TDT в сутках от 0 часов 1.01.2000-го года
	//	xl     -  массив результатов
	//	iprizn =  1  : xl - средняя эклиптическая долгота,широта,радиус
	//	      !=  1  : xl - X,Y,Z  в дткм в средней абсолютной
	//				   экваториальной СК текущей эпохи
	static const float am[82]={
	 186.540E0f, -0.978E0f,  22639.50E0f,-668.11E0f,-125.15E0f,
	 18461.48E0f,-411.61E0f, -6.30E0f,   1010.18E0f,-999.70E0f,
	 -45.10E0f,  -0.714E0f,  39.53E0f,   10.166E0f, 769.02E0f,
	 61.91E0f,   -31.76E0f,  -4.00E0f,   0.621E0f,  36.12E0f,
	 3.98E0f,    -6.49E0f,   4.86E0f,    -0.949E0f, -109.67E0f,
	 -5.33E0f,   5.10E0f,    1.153E0f,   147.69E0f, 6.76E0f,
	 -5.66E0f,   -7.65E0f,   9.70E0f,    -7.49E0f,  -5.36E0f,
	 4.80E0f,    -8.47E0f,   18.61E0f,   18.02E0f,  28.233E0f,
	 2369.90E0f, 117.26E0f,  -623.66E0f, -5.74E0f,  -55.17E0f,
	 3.086E0f,   191.95E0f,  15.12E0f,   -33.36E0f, -6.38E0f,
	 34.312E0f,  -4586.43E0f,-166.58E0f, 199.49E0f, 9.37E0f,
	 14.39E0f,   -211.66E0f, -15.57E0f,  -13.19E0f, -24.42E0f,
	 12.14E0f,   28.48E0f,   1.918E0f,   -165.15E0f,-29.69E0f,
	 8.00E0f,    1.444E0f,   -205.96E0f, -7.46E0f,  8.90E0f,
	 -8.63E0f,   -7.41E0f,   14.58E0f,   -8.10E0f,  13.90E0f,
	 -3.68E0f,   0.601E0f,   -38.43E0f,  -6.58E0f,  -4.39E0f,
	 -30.77E0f,  3422.700E0f
	 };
	float s[7],c[7],x,y;
	double u[5],p,v,b;
	double ukrug;			// число угловых секунд в круге */
	#define pi2  k_dwapi    // 6.283185307179586;  Два пи */
	#define ae   k_a0		//  =0.637814;  Большая полуось Земли */
	static const short logik[64]={
	 02533, 01035, 01013, 011031,02015,
	 016015,06711, 01073, 011037,02057,
	 06617, 01036, 01023, 011032,06612,
	 01036, 011036,016612,01036, 011036,
	 02072, 012072,02022, 01034, 011034,
	 02014, 012014,02024, 06644, 01036,
	 011036,02056, 012056,06716, 01037,
	 011037,012075,016416,01034, 011034,
	 012045,02017, 02714, 01037, 02017,
	 02526, 011035,012015,016526,01035,
	 011035,06415, 01034, 011034,02014,
	 02024, 012015,02025, 02566, 011035,
	 016415,01034, 02024, 02014
	 };
	short i,i1,i2,i3,l,log,j,ip;
	ukrug=k_cbc*15;
#ifdef OTLADKA_LUNA
	FILE *filep,*filev,*fileb;
	if ((filep=fopen("lunap.rez","wt"))==nullptr) goto error;
	if ((filev=fopen("lunav.rez","wt"))==nullptr) goto error;
	if ((fileb=fopen("lunab.rez","wt"))==nullptr) goto error;

	for (i=0;i<8;i++)
		for (j=0;j<4;j++)
			{
			if (i==j) k=1;else k=0;
			k1[i][j]=k;
			}
#endif

	fargTIME(time=tau2000(time),u);
	v=(u[2]+u[4])/pi2*ukrug;
	for (i=0;i<4;i++)
		{
		s[i]=(float)(sin(p=u[i]));
		c[i]=(float)(cos(p));
		}
	p=c[0]*am[0]+c[3]*am[1];

#ifdef OTLADKA_LUNA
	vyvod_otl_luna(filep,0,am[0]);
	vyvod_otl_luna(filep,3,am[1]);
#endif

	v=s[0]*am[2]+s[1]*am[3]+s[3]*am[4]+v;

#ifdef OTLADKA_LUNA
	vyvod_otl_luna(filev,0,am[2]);
	vyvod_otl_luna(filev,1,am[3]);
	vyvod_otl_luna(filev,3,am[4]);
#endif

	b=s[2]*am[5];

#ifdef OTLADKA_LUNA
	vyvod_otl_luna(fileb,2,am[5]);
#endif

	j=6;
	for (i=0;i<64;i++)
		{
		log=logik[i];
		i1=-1;
		met2: ip=log/8;
		i3=log-ip*8;
		log=ip/8;
		l=ip-log*8;
		if (i1>=0) goto met1;
		i2=i3-1;
		i1=l-1;
		goto met2;
		met1: i3--;
		if (log>0) y=1.0f;else y=-1.0f;

#ifdef OTLADKA_LUNA
		for (k=0;k<4;k++)
			k1[7][k]=k1[i1][k]-k1[i2][k]*(short)(y);
#endif

		x=s[i1]*c[i2]-y*s[i2]*c[i1];
		y=c[i1]*c[i2]+y*s[i2]*s[i1];
		if (i3<0) goto met5;

#ifdef OTLADKA_LUNA
		for (k=4;--k>=0;k1[i3][k]=k1[7][k]);
#endif

		s[i3]=x;
		c[i3]=y;
		met5: if (l<4) goto met6;

#ifdef OTLADKA_LUNA
		vyvod_otl_luna(filep,7,am[j]);
#endif

		p+=am[j]*y;
		l-=4;
		j++;
		met6: if (l<2) goto met7;

#ifdef OTLADKA_LUNA
		vyvod_otl_luna(filev,7,am[j]);
#endif

		v+=am[j]*x;
		l-=2;
		j++;
		met7: if (l<1) goto met8;

#ifdef OTLADKA_LUNA
		vyvod_otl_luna(fileb,7,am[j]);
#endif

		b+=am[j]*x;
		j++;
		met8: ;
		}
	p=(p+am[j])/ukrug*pi2;
	v=v/ukrug*pi2;
	b=b/ukrug*pi2;
	if (iprizn != 1)
		{
		u[0]=sr_E0_IERS(time)/ukrug*pi2;
		u[1]=ae/p;
		u[2]=cos(b);
		xl[0]=u[1]*cos(v)*u[2];
		u[3]=sin(b);
		p=sin(u[0]);
		b=cos(u[0]);
		v=sin(v);
		xl[1]=u[1]*(v*b*u[2]-u[3]*p);
		xl[2]=u[1]*(v*p*u[2]+u[3]*b);
		}
		else {
		xl[0]=v;
		xl[1]=b;
		xl[2]=ae/p;
		}

	#ifdef OTLADKA_LUNA
	if ((i=fclose(filep))==EOF) goto error;
	if ((i=fclose(filev))==EOF) goto error;
	if ((i=fclose(fileb))==EOF) goto error;
	#endif

	return;

#ifdef OTLADKA_LUNA
	error:	fprintf(stderr,form_err);exit(1);
#endif
}

double kep_f(double e,double l)
{	// Возвращает f-истинную аномалию по эксцентриситету e
	// и средней аномалии l . Только для почти круговых орбит.
	static const double kfc[9]={
		5./96.,-0.25,2.0,
		-11./12.,1.25,
		-43./64.,13./12,
		103./96.,1097./960
		};
	double x1,y1,x2,y2,x4,y4,e2;
	double kepf=l;
	x1=e*cos(l);
	y1=e*sin(l);
	x2=x1*x1-y1*y1;
	y2=x1*y1*2;
	x4=x2*x2-y2*y2;
	y4=y2*x2*2;
	e2=e*e;
	kepf+=((kfc[0]*e2+kfc[1])*e2+kfc[2])*y1+
		(kfc[3]*e2+kfc[4])*y2+
		(kfc[5]*e2+kfc[6])*(y2*x1+x2*y1)+
		kfc[7]*y4+kfc[8]*(y4*x1+y1*x4);
	return(kepf);
}