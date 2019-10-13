//---------------------------------------------------------
//  CMPLX.C
//    ОПЕРАЦИИ НАД КОМПЛЕКСНЫМИ ЧИСЛАМИ
//  Автор  : Пасынков В.В.
//  Версия : 01.00 / 01.10.2000 /
//---------------------------------------------------------
#include "ssp.h"

//присвоить
void cmplxeq(double alpha[],double a[])
{
a[0]=alpha[0]; //real
a[1]=alpha[1]; //imag
return;
}
//сложение 
void cmplxadd(double alpha[],double betta[],double a[])
{
a[0]=alpha[0]+betta[0]; //real
a[1]=alpha[1]+betta[1]; //imag
return;
}

//вычитание
void cmplxsub(double alpha[],double betta[],double a[])
{
a[0]=alpha[0]-betta[0]; //real
a[1]=alpha[1]-betta[1]; //imag
return;
}

//умножение
void cmplxmpy(double alpha[],double betta[],double a[])
{
a[0]=alpha[0]*betta[0]-alpha[1]*betta[1]; //real
a[1]=alpha[0]*betta[1]+alpha[1]*betta[0]; //imag
return;
}

//деление
void cmplxdev(double alpha[],double betta[],double a[])
{
double d;
d=betta[0]*betta[0]+betta[1]*betta[1];
a[0]=alpha[0]*betta[0]+alpha[1]*betta[1]; //real
a[1]=alpha[1]*betta[0]-alpha[0]*betta[1]; //imag
if(d!=0)
  {
  a[0]/=d;
  a[1]/=d;
  }
return;
}