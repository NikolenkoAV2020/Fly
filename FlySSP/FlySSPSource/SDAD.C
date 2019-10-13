//---------------------------------------------------------------------------
//  SDAD.C
//    РАСЧЕТ СРЕДНЕЙ ОТНОСИТЕЛЬНОЙ ОШИБКИ И ВЕЛИЧИНЫ ОСТАТОЧНОЙ ДИСПЕРСИИ
//    ДЛЯ АНАЛИЗА КАЧЕСТВА АППРОКСИМАЦИИ ВРЕМЕННЫХ РЯДОВ
//  Автор  : Пасынков В.В.
//  Версия : 01.00 / 27.09.2000 /
//---------------------------------------------------------------------------
#include "ssp.h"
#include <math.h>

/*     ..................................................................

	SUBROUTINE SDEVP

	PURPOSE
	   РАСЧЕТ СРЕДНЕЙ ОТНОСИТЕЛЬНОЙ ОШИБКИ И ВЕЛИЧИНЫ ОСТАТОЧНОЙ
	   ДИСПЕРСИИ ПРИ АНАЛИЗЕ КАЧЕСТВА АППРОКСИМАЦИИ ВРЕМЕННЫХ РЯДОВ

	USAGE
	   SDAD(N,X,Y,SDEV,ADEV,SIG,FDATA);

	DESCRIPTION OF PARAMETERS
	   N     - число наблюдений
		   NUMBER OF OBSERVATIONS. N MUST BE > OR = TO 2.
	   X[N]  - массив исходных значений временного ряда, [n] или NULL
	   Y[N]  - массив сглаженных значений временного ряда, [n] или NULL
	   SDEV  - адрес величины остаточной дисперсии
		   sdev=1/n * SUM (x[i]-y[i])^2, i=1,...,n
	   ADEV  - адрес величины средней относительной ошибки
		   adev=1/n * SUM ( ABS(x[i]-y[i])/x[i] ), i=1,...,n
	   SIG[N]- массив индивидуальных относительных отклонений или NULL
	   FDATA(i,&xi,&yi)-функция чтения (расчета) исходного и
		   сглаженного временного ряда или NULL
	REMARKS
	   NONE

	SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   DATA(i,X[i],Y[i]) - THIS SUBROUTINE MUST BE PROVIDED BY THE USER.

	METHOD
	...............................................................

     INITIALIZATION
 */
//#define Sig(arg1, arg2)         ( fabs((arg1)-(arg2))/(arg1) )
//#define Sig(arg1, arg2)         ( fabs((arg1))/(arg2) )

int sdad(int n,double x[],double y[],
	   double *sdev,double *adev,
	   double sig[],
	   void (*fdata)(int i, double *xi, double *yi) )
{
int    i,io;
double xi,yi,a,sd,ad,fn;
  if(n<1) return -2;
  if(!x || !y)
    {
    if(!fdata) return -1;
      else io=3;
    }
    else
    if(!x) io=2;
      else
      if(!y)io=1;
	else    io=0;
  fn=1.0/(double)n;
  for(sd=0,ad=0,i=0;i<n;i++)
    {
    switch(io)
      {
      default:
      case 0:
	xi=x[i]; yi=y[i]; break;
      case 1:
	fdata(i,&xi,y);
	yi=y[i]; break;
      case 2:
	fdata(i,x,&yi);
	xi=x[i]; break;
      case 3:
	fdata(i,&xi,&yi);  break;
      }
      a=xi-yi;
      sd+=(a*a*fn);
      if(xi!=0)
	a=fabs(a)/xi;
	else
	a=0;
      ad+=(a*fn);
      if(sig)
	sig[i]=a;
    }
    if(sdev) *sdev=sd;
    if(adev) *adev=ad;
return 0;
}

