/*------------------------------------------------------------------------
  LEPP.C
    ВЫЧИСЛЕНИЕ ЗНАЧЕНИЙ ПРИСОЕДИНЕННЫХ ФУНКЦИЙ ЛЕЖАНДРА 1-го РОДА
  Автор	 : Пасынков В.В.
  Версия : 01.00 / 10.06.1996 /
------------------------------------------------------------------------*/
#include "ssp.h"
#include <math.h>

int locp(int i,int j)
{
int irx,ix, jx;
ix=i;jx=j;
if( i > j )
   {ix=j;jx=i;}
irx = (jx+1)*jx/2;// элементов внизу
irx+=ix;         // + элемент в строке
return irx;
}


//#pragma argsused
double p00(double x)
{
x=1.0;
return x;
}
double p10(double x)
{
return x;
}

double p11(double x)
{
double Pnm;
Pnm=sqrt(1.0-x*x);
return Pnm;
}
double p20(double x)
{
double Pnm;
Pnm=0.5*(3.0*x*x-1.0);
return Pnm;
}

double p21(double x)
{
double Pnm,sqx;
sqx=sqrt(1.0-x*x);
Pnm=3.0*x*sqx;
return Pnm;
}

double p22(double x)
{
double Pnm,sqx;
sqx=sqrt(1.0-x*x);
Pnm=3.0*sqx*sqx;
return Pnm;
}

double p30(double x)
{
double Pnm;
Pnm=0.5*(5.0*x*x*x-3.0*x);
return Pnm;
}

double p31(double x)
{
double Pnm,sqx;
sqx=sqrt(1.0-x*x);
Pnm=1.5*(5.0*x*x-1.0)*sqx;
return Pnm;
}

double p32(double x)
{
double Pnm,sqx;
sqx=sqrt(1.0-x*x);
Pnm=15*x*sqx*sqx;
return Pnm;
}
double p33(double x)
{
double Pnm,sqx;
sqx=sqrt(1.0-x*x);
Pnm=15.0*sqx*sqx*sqx;
return Pnm;
}

double pnn(double x,int n,double *pnn)
{
int i;
double Pnn,coef,sqx;
sqx=sqrt(1.0-x*x);
pnn[0]=coef=Pnn = 1.0;//P(0,0)
for(i=1;i<n;i++)
  {
  coef*=(2.0*(double)i-1.0);
  Pnn *= sqx;
  if(pnn)
    pnn[i]=Pnn*coef;//диагональная форма хранения
  }
return Pnn*coef;
}

double pnm(double x,int n, int m, double Pnm[])//симметричный способ
{
int i,j,k2,k1,k,nm;
double Pnn,coef,sqx,t1,t2,t3;
if(!Pnm)
  return 0.0;
nm=(n+1)*n/2;
for(k=0;k<nm;k++)
  Pnm[k]=0.0;
if(n<0)
  return 0.0;
if(m<0)
  return 0.0;
coef= Pnm[0] = Pnn = 1.0;//P(0,0)
if(n==0 && m==0)
  return Pnm[0];
Pnm[1]=x;                //P(1,0)
if(n==1 && m==0)
  return Pnm[1];
Pnn=sqx=sqrt(1.0-x*x);
Pnm[2]=sqx;              //P(1,1)
if(n==1 && m==1)
  return Pnm[2];
for(j=2;j<n;j++)//по строкам
   {
   t1=(2.0*(double)j-1.0);
   coef*=t1;
   Pnn *= sqx;
   nm=Min((j-1),m+1);
//   nm=(j-1);//пока считается весь треугольник
   for(i=0;i<nm;i++)//по столбцам
     {
     k =locp(j,i);
     k2=locp(j-2,i);
     k1=locp(j-1,i);
     t2=(double)(i + j - 1);t3=(double)(j-i);
     Pnm[k]=(t1*x*Pnm[k1] - t2*Pnm[k2])/t3;
     }
   if( (j-1) > (m+1) )
     continue;
   k = locp(j-1,j);k1=locp(j-1,j-1);
   Pnm[k]=(t1*x*Pnm[k1]);
   if(j> (m+1))
     continue;
   k = locp(j,j);
   Pnm[k]=Pnn*coef;
   }
k = locp((n-1),(m-1));
return Pnm[k];
}
