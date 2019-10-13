//---------------------------------------------------------------------------
// ModTime.cpp
// А. Николенко 20.08.2018
//---------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции

//---------------------------------------------------------------------------
#include <FlyTimeSource\\ModTime.h>
#include <FlyTimeSource\\TheDataTime.h>

//---------------------------------------------------------------------------
// Функция расчета интервала времени в ССС между двумя моментами времени
//---------------------------------------------------------------------------
HRESULT DeltaTime(short Dir, DATE *dn, double tn, DATE *dk, double *tk, double *DTime)
{
	static DATE D1975(1, 1, 1975);
	DATE dt;
	TIME tm;
	double sut,tim;

	SSSTimeToCalendTime(dn,tn,&dt,&tm);
	sut = CalendTimeToSSSTime(&D1975,&dt,&tm);
	tim = GetLeapSeconds(sut);
	sut+= ((Dir)?(-1):(+1))*(k_zona+tim);
	SSSTimeToCalendTime(&D1975,sut,dk,&tm);
	*tk = CalendTimeToSSSTime(dk,dk,&tm);
	*DTime = k_zona ;

	return S_OK ;
}

//---------------------------------------------------------------------------
// Функция преобразования атомного времени из ССС в календарное ДМВ и обратно
//---------------------------------------------------------------------------
short TA_DMB(short Direct, DATE *DMB, TIME *TMB, double *TA)
{
	static DATE D1975(1, 1, 1975) ;
    TIME T0 ;
    DATE Da ;
    double DltTIME, DTIME ;
    short  rc;

    if(Direct) {
		rc = (short)DeltaTime(1,DMB,CalendTimeToSSSTime(DMB,DMB,TMB),&Da,TA,&DltTIME) ; if(rc) return rc;
        *TA+=CalendTimeToSSSTime(&D1975,&Da,&T0);
    } else {
        rc = (short)DeltaTime(0,&D1975,*TA,DMB,&DTIME,&DltTIME) ; if(rc) return rc;
        Da=*DMB;
        SSSTimeToCalendTime(&Da,DTIME,DMB,TMB);
    }
    return rc ;
}

//---------------------------------------------------------------------------
// Функция преобразования времени из ССС во всемирное календарное время
//---------------------------------------------------------------------------
void SSSTimeToCalendTime(DATE *dno, double ccc, DATE *d, TIME *t)
{
  DATE D1900(1,1,1900);
  TIME t0(0,0,0,0.), ttt;
  unsigned long sut;
  unsigned long kal[14]={0,31,59,90,120,151,181,212,243,273,304,334,365,390};
  short god,i;
  double dc = floor(ccc);

  ccc -= dc;
  *d=*dno;

  sut  = (unsigned long) (CalendTimeToSSSTime(&D1900,d,&t0) + dc);
  god  = (short)((sut+1.1)/365.25);
  sut -= (unsigned long)(god*365.249);
  if(god%4==0) { for (i=2; i<14; kal[i]++,i++); }
  for (i=0; sut>=kal[i]; i++);
  if(i==13) {god++;i=0;}
  d->d = (short)(sut-kal[i-1]+1);
  d->m = (short)i;
  d->g = god+1900;
  ccc*= 24.;                   t->h = (short) ccc;
  ccc = (ccc-t->h)*60.;        t->m = (short) ccc;
  ccc = (ccc-t->m)*60.;        t->s = (short) ccc;
                               t->d = (double) ccc-t->s;
  if(t->d>=0.999999f)
  { t->d=0.f;
    if( ++t->s >= 60 )
    { t->s=0;
      if( ++t->m >=60 )
      { t->m=0;
        if( ++t->h >=24 )
        { t->h=0;
          ccc=CalendTimeToSSSTime(&D1900,d,&t0);
          SSSTimeToCalendTime(&D1900,ccc+1.2,d,&ttt);
        }
      }
    }
  }
}

//---------------------------------------------------------------------------
// Функция преобразования всемирного календарного времени в ССС от заданной эпохи
//---------------------------------------------------------------------------
double CalendTimeToSSSTime(DATE* dno, DATE* dz, TIME* tz)
{
	DATE *d;
	double c[2]={0,0};
	short  i=0,f;

	while( i<2 && (dno->d!=dz->d || dno->m!=dz->m || dno->g!=dz->g) ) {
		d = i ? dno : dz ;
		f = d->m<3 ? 1 : ((d->g % 4) ? 3 : 2) ;
		c[i++]= d->d-f+(long)((d->g-1900)*365.249)+(long)((d->m-1)*30.58+0.5);
	}
	return (c[0]-c[1]+((tz->h*60.+tz->m)*60.+tz->s+tz->d)/86400.) ;
}

//---------------------------------------------------------------------------
// Функция получения номера года и дня в году по календарному дню
//---------------------------------------------------------------------------
short CalendDayToYearDay(DATE Date)
{
  DATE Date_YearBeg(31,12,2000);
  TIME Time(12,0,0,0.);
  Date_YearBeg.g = Date.g - 1;
  return (short) CalendTimeToSSSTime(&Date_YearBeg, &Date, &Time);
}

//---------------------------------------------------------------------------
// Функция преобразования времени от эпохи B1975 во время начала текущего года
double TimeB1975ToYearTime(double dTimeB1975)
{
  DATE DateN(1,1,1975);
  DATE Dt;
  TIME Tm;
  double dYearTimeB1975;
  // Получение текущих даты и времени
  SSSTimeToCalendTime(&DateN, dTimeB1975, &Dt, &Tm);
  // Формирование даты и времени начала года
  Dt.d=1; Dt.m=1; Tm.Clear();
  // Получение времени начала года в ССС
  dYearTimeB1975 = CalendTimeToSSSTime(&DateN, &Dt, &Tm);
  // Возврат времени от начала года в ССС
  return dTimeB1975 - dYearTimeB1975;
}

//---------------------------------------------------------------------------
// Функция преобразования времени от эпохи J2000 во время начала текущего года
double TimeJ2000ToYearTime(double dTimeJ2000)
{
  return TimeB1975ToYearTime(dTimeJ2000 + DeltaTime_J2000_B1975());
}

//---------------------------------------------------------------------------
// Функция преобразования времени от эпохи В1975 во время от эпохи J2000
double DeltaTime_J2000_B1975()
{
	return 9131.;
}

//---------------------------------------------------------------------------
// Функция получения календарного дня по номеру года и номеру дня в году
//---------------------------------------------------------------------------
DATE YearDayToCalendDay(short Year, short Day)
{
    DATE Date;
    DATE Date_YearBeg(31,12,2000);
    TIME Time;
    double Time_SSS = (double) Day;

    Date_YearBeg.g = Year - 1;
	SSSTimeToCalendTime(&Date_YearBeg, Time_SSS, &Date, &Time);
    return Date;
}

//---------------------------------------------------------------------------
// Функция преобразования времени из даты и времени в год с долями
//---------------------------------------------------------------------------
void DateTimeToYear(DATE Dt, TIME Tm, double *dYearTime)
{
	int i;
	unsigned long kal[14]={0,31,59,90,120,151,181,212,243,273,304,334,365,390};
	double God, SSS = 0., SEC;

	// Целая часть года
	God = (double) Dt.g;
	// Коррекция числа дней в месяце
	if(Dt.g%4==0) { for (i=2; i<14; kal[i]++,i++); }
	// Дробная часть года в ССС
	// Целые месяцы и дни месяца
	SSS = kal[Dt.m-1] + Dt.d - 1;
	SEC = (Tm.h * 60 + Tm.m) * 60 + Tm.s + Tm.d;
	// Часы, минуты, секунды, доли
	SSS += SEC/86400.;
	// Перевод в доли года
	God += SSS/kal[12];
	if(dYearTime) *dYearTime = God;
}

//---------------------------------------------------------------------------
// Функция получения даты первого дня недели N_Week (понедельника) в году N_Year
//---------------------------------------------------------------------------
DATE WeekAndYearToDateFirstDay(int N_Week, int N_Year)
{
	static DATE D1975(1, 1, 1975) ;
	TIME sT_0;
	DATE sD_2007(1, 1, 2007), sD_Tek(1, 1, N_Year), sD;

	//Контроль корректности данных
	if(N_Week<1 || N_Week>52) { sD.d=-1; sD.m=-1; sD.g=-1; return sD; }

	//Промежуточные данные
	double  dSSS1 = HMSCCC(&D1975, &sD_2007, &sT_0),
			dSSS2 = HMSCCC(&D1975, &sD_Tek,  &sT_0),
			dSSS3 = dSSS2 - ((dSSS2 > dSSS1) ? ((int)(dSSS2-dSSS1)%7-(N_Week-1)*7) : ((int)(dSSS2-dSSS1)%7+7)-N_Week*7);

	//Искомая дата понедельника
	CCCHMS(&D1975, dSSS3, &sD, &sT_0);

	return sD;
}

//---------------------------------------------------------------------------
// CONVERTS JULIAN DAY NUMBER TO CALENDAR DATE
// IDATE(6)=MONTH,DAY,YEAR,HOUR,MINUTE,SECOND (INTEGER) FOR CALENDAR DATE
//
// DJED=DOUBLE PRECISION JULIAN DAY NUMBER TO BE INPUT
// FLAG=(-1,0,+1(=)JULIAN,AUTOMATIC,GREGORIAN) CALENDAR DATE OUTPUT
// AUTOMATIC MODE SELECTS JULIAN CALENDAR FOR DAYS ON AND BEFORE
//                DJED=2299160=1582OCT4,
// GREGORIAN AFTER THIS DATE = 1582OCT15.
// OTHER MODES EMPLOY CALENDAR INDICATED.
//
// **RESTRICTIONS - INPUT JULIAN CALENDAR MUST BE FOR A POSITIVE JULI
//   DAY NUMBER.  FOR GREGORIAN CALENDAR, DJED.GT.1721119 (DATES A.D.)
//
// **SEE SUBROUTINE CTOJ FOR INVERSE FUNCTIONS AND REFERENCES TO
//   CALENDAR DEFINITIONS, TO BE FOUND IN EXP.SUPP. PP.412, 436.
//---------------------------------------------------------------------------
void joulex(double djed, DATE *d_UTC, TIME *t_UTC)
{
  long   d, dy, h, j, m, md, y;
  double dt, ja, secs, secx;
  static int flag=0;

    dt=djed+0.5;
    //dt = djed + .5e0 + 0.5e0/86400.e0;
    j=(long)dt; ja = j;
    dt=(dt-ja)*24.0;
    h=(long)dt;  ja = h;
    dt=(dt-ja)*60.0;
    md=(long)dt; ja = md;
    dt=(dt-ja)*60.0;
    secx=dt; secs=secx;

	switch(flag) {
	case -1:  //JULIAN
		y=4L*(j + 105498L)/1461L-5001L;
		dy=j-(36525L*(y+5000L))/100L+105133L;
		break;
	case  0:  //AUTOMATIC
		if(j>2299160L) goto L_30;
		break;
	case  1:  //GREGORIAN
L_30:
		y=4L*(j-1720754L+3L*(4L*(j-1721119L)/146097L+1L)/4L)/1461L-1L;
		dy=j-36525L*y/100L+3L*(y/100L+1)/4L-1721119L;
		break;
	}

    m=(5*dy-3)/153+3;
    d=dy-(153*(m-3)+2)/5;
    if(m>=13) m=m-12;
    if(m<=2)	y=y+1;

    d_UTC->d=(short)d;
    d_UTC->m=(short)m;
    d_UTC->g=(short)y;

    t_UTC->h=(short)h;
    t_UTC->m=(short)md;
    t_UTC->s=(short)secs;
    t_UTC->d=secs-t_UTC->s;

    return;
}

//---------------------------------------------------------------------------
// Расчет разницы между земным динамическим (TDT) и атомным (IAT) временем 
double Get_Delta_TDT()
{
  return del_TDT / 86400. ;
}

//---------------------------------------------------------------------------
// Функция расчета бесселевой эпохи по юлианской эпохе
//---------------------------------------------------------------------------
double JD_Bessel(double JD)
{
  double JD_Bessel = 2415020.31352, TropYear = 365.242198781;
  JD_Bessel+=TropYear*floor((JD-JD_Bessel+0.5)/TropYear);
  return JD_Bessel;
}

//---------------------------------------------------------------------------
// Функция расчета бесселевой эпохи по юлианской эпохе
//---------------------------------------------------------------------------
void JD_Bessel(double *JD, double *BD)
{
  long double tau;
  tau=floor(((JD[0]-2415020.)+JD[1]-0.31352)/365.242198781)*365.242198781+2415020.31352;
  BD[0]=(long int)tau;
  BD[1]=tau-BD[0];
  return ;
}

//---------------------------------------------------------------------------
// Перевод атомных суток в земные динамические сутки
double t_TDT(double t_TAI)
{
  return (t_TAI+del_TDT/k_cbc);
}

//---------------------------------------------------------------------------
// Перевод из суток от 1.01.2000 г. в столетия от J2000.0
double tau2000(double time_sss)
{
  return (time_sss-0.5)/36525.;
}
//---------------------------------------------------------------------------