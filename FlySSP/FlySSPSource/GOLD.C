#include <math.h>
#include <malloc.h>

//#include <deftype.h>

/*
double fuser(const double ,const void *array,const int size);

double fintg(const double Hm,const void *array,const int size);
double macheps(void);
*/
double coeff_scaleL;
double coeff_scaleH;
/*
double macheps(void)
{
  double eps;
  eps=0.5;
  while ( ( 1.0 +eps ) != 1.0 )
    eps/= 2.0;
  return( eps);
}
*/
double gold(const double a,
	   const double b,
	   double (*finteg)(const double ,const void *,const int),
	   const void *array,
	   const int size)
{
double f1,f2;
double t1,t2,step,x0,x1,x2,x3,x;
int i=0;
int iter=1000;
double eps=0.005;
/*  eps=macheps()*fabs(b-a); */
  step=(sqrt(5.0)-1.0)/2.0;
  t1=1-step;
  t2=step;
  x0=a;x1=a+t1*(b-a);x2=a+t2*(b-a);x3=b;
  f1=finteg(x1,array,size);
  f2=finteg(x2,array,size);
  do
    {
	 if(  f2 > f1 )
       {
       step=x2-x0; x3=x2;  x2=x1;    x1=x0+t1*step;    f2=f1;
       f1=finteg(x1,array,size);
       }
       else
       {
       step=x3-x1;    x0=x1;    x1=x2;    x2=x0+t2*step;    f1=f2;
       f2=finteg(x2,array,size);
       }
    }	while( ( fabs(step)) > eps  &&  ++i <= iter);
   if(fabs(f1)<fabs(f2))   x=x1;
     else x=x2;
   return x;
}

double fuser(const double x,const void *array,const int size)
{
if(array && size>0)
  return(-exp(-x)*log(x));
 else return 0;
}
/*
double main(void)
{
int array[22];
int size;
double a=-4,b=5,x;
array[0]=1;array[1]=2;array[2]=-1;array[3]=1;
array[4]=2;array[5]=1;array[6]=0;array[7]=-1;
array[8]=-2;array[9]=1;array[10]=-1;array[11]=1;
array[12]=-2;array[13]=-3;array[14]=-1;array[15]=2;
array[16]=3;array[17]=1;array[18]=-1;array[19]=-2;
array[20]=1;array[21]=4;
size=22;
  coeff_scaleH=1;
  coeff_scaleL=1;
//x=gold(0.0,2.0,fuser,array,size);
x=gold(a,b,fintg,array,size);
return x;
}
*/
double fintg(const double Hm,const void *array,const int size)
{
int *measure;
double s;
int k;
double x1,x2;
measure=(int *)array;
x1 =((double)measure[0] - Hm)*coeff_scaleH ;
for (x1=0.0,s=0.0,k=1; k<size; k++)
  {
  x2 =((double)measure[k] - Hm)*coeff_scaleH ;
  s+=(coeff_scaleL*( x2+x1)/2.0);/*формула трапеций*/
  x1=x2;
  }
return(fabs(s));
}
