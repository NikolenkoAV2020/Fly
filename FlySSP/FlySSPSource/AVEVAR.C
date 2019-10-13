//---------------------------------------------------------------------------
//  AVEVAR.C
//    РАСЧЕТ ВЫБОРОЧНЫХ СРЕДНЕГО И ДИСПЕРСИИ (несмещенная оценка)
//  Автор  : Пасынков В.В.
//  Версия : 01.00 / 25.09.2000 /
//---------------------------------------------------------------------------
#include "ssp.h"
//---------------------------------------------------------------------------
//	SUBROUTINE AVEVAR
//
//	PURPOSE
//	   COMPUTE MEANS, STANDARD DEVIATIONS.
//
//	USAGE
//	   void avevar(double data[], int n, double *ave, double *svar);
//
//	DESCRIPTION OF PARAMETERS
//	   data[n] - вектор наблюдений, размерности n
//	   n       - число наблюдений
//	   *ave    - адрес результата - выборочное среднее
//	   *svar   - адрес результата - выборочная дисперсия
//
//	REMARKS
//	   Если ave=NULL или svar=NULL, то соответсвующий результат
//	   не вычисляется
//---------------------------------------------------------------------------

void avevar(double data[], int n, double *ave, double *svar)
{
int    j;
double s,a,b;
	b=a=0;
	if(ave) (*ave)=0.0;
	if(svar) (*svar)=0.0;
	if(ave || svar)
	  {
	  for (j=0;j<n;j++) a += data[j];
	    a /= n;
	  if(ave) (*ave)=a;
	  }
	if(svar)
	  {
	  for (j=0;j<n;j++)
	    {
	    s=data[j]-a;
	    b += s*s;
	    }
	  if(n>1)
	    b /= (n-1);
	  (*svar)=b;
	  }
return;
}
