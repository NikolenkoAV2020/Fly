/*---------------------------------------------------------------------------
  PSI.C
	ОПТИМИЗАЦИЯ. ОТЫСКАНИЕ ГЛОБАЛЬНОГО ЭКСТРЕМУМА
	(МИНИМУМА ИЛИ МАКСИМУМА) ФУНКЦИИ НЕСКОЛЬКИХ ПЕРЕМЕННЫХ
	(Чичинадзе В.К.)
  Автор	 : составил программу Пасынков В.В.
  Версия : 01.00 / 15.04.1994 /
---------------------------------------------------------------------------*/
#include <math.h>
#include <malloc.h>
#include "ssp.h"

double INF_SUP(const double X1, const double X2, const double X3);

int    h;
long   cor;
double step(double ,int );

//#pragma argsused
int psi(int ndim,
	double *area,
	int ni,
	int l,
	int flag,
	double (*func)(double *, double *),
	double *x,
	double *z,
	double *extremum,
	double *estimate)
{
double  u1=3.14159265,u2=0.542101887;
double ps[20],ks[20],*kp;
double *xi,*bx,*bp;
double *gp,m1[4],m[4],a[9],b[3],y[3];
double si,t,inf,sup,ds,d,x1,x2,fmax,t1,t2,t3,t4,a0,a1,a2,dl,r1,r2;
int    kr,i,j,k;

// cor=coreleft();h=heapcheck();

kr=Min(20,(short)sqrt(ni));
//ps=(double *)malloc (sizeof(double)*kr);
//ks=(double *)malloc (sizeof(double)*kr);
xi=(double *)malloc(sizeof(double)*ndim*kr);
//cor=coreleft();h=heapcheck();
bx= (double *)malloc(sizeof(double)*ndim*ni);
//cor=coreleft();h=heapcheck();
bp= (double *)malloc(sizeof(double)*ndim*ni);
//cor=coreleft();h=heapcheck();
kp=(double *)malloc (sizeof(double)*ni);
//cor=coreleft();h=heapcheck();
gp=(double *)malloc(sizeof(double)*ndim);
//cor=coreleft();h=heapcheck();
if(!gp) return -1;
for (j=0;j<kr;j++)
  {
  ps[j]=ks[j]=0.0;
  for (i=0;i<ndim;i++)
    {
    xi[j*ndim+i]=0.0;
    }
  }
for (j=0;j<ni;j++)
  {
  kp[j]=0.0;
  for (i=0;i<ndim;i++)
    {
    bx[j*ndim+i]=bp[j*ndim+i]=0.0;
    }
  }
for (i=0;i<ndim;i++)
   gp[i]=area[i*2+1]-area[i*2];
i=0;
while(i<=ni)
  {
  for (j=0;j<ndim;j++)
   {
   t=u1+u2;
   u1=u2;
   if(t>4.0) t-=4.0;
   u2=t;
   bx[i*ndim+j]=area[j*2]+t*gp[j]/4.0;
   bp[i*ndim+j]=0;
   }
   kp[i]=func(&bx[i*ndim],&bp[i*ndim]);
   if(kp[i]!= 0.0) i+=1;
  }
if(flag==__MAX__)
  for (i=0;i<ni;i++)
     kp[i]=-kp[i];

for (inf=sup=kp[0],i=1;i<ni;i++)
 {
 if(sup <=kp[i]) sup=kp[i];
 if(inf > kp[i]) inf=kp[i];
 }
/* орпределяем максимальное и минимальноеное значения функции*/
si=(sup+inf)/2.0;
ds=(si-inf)/(double)kr;
for (i=0;i<kr;i++)
   ps[i]=0.0;
for (i=0;i<kr;i++)
 {
 ks[i]=inf+(double)(kr-i)*ds;
 for (j=0;j<ni;j++)
  {
  d=kp[j]-ks[i];
  if(d==0)
    if(l==0)
      ps[i]+=1;
      else;
    else
    if(d<0.0)
      ps[i]+=step(fabs(d),l);
  }
  ps[i]/=(double)ni;
 }
/* определяем по точкам значения -функции,
   коэффициенты параболы аппроксимирующей функцию */
for (i=0;i<4;i++)
{
m[i]=0.0;m1[i]=0.0;
}
dl=1.0/(double)kr;
for (i=kr-1;i>=0;i--)
{
 d=ks[i]*ks[i];
 m[0]+=(ks[i]*dl);
 m[1]+=(d*dl);
 m[2]+=(d*ks[i]*dl);
 m[3]+=(d*d*dl);
 m1[0]+=(ps[i]*dl);
 m1[1]+=(ps[i]*ks[i]*dl);
 m1[2]+=(ps[i]*d*dl);
}
a[8]=1.0;
a[5]=a[7]=m[0];
a[6]=a[4]=a[2]=m[1];
a[1]=a[3]=m[2];
a[0]=m[3];
b[0]=m1[2]; b[1]=m1[1]; b[2]=m1[0];
mgs(3,a,b,y);
d=y[1]*y[1]-4.0*y[0]*y[2];
if(d>0.0)
  {
  d=sqrt(d);
  x1=(-y[1]+d)/(2.0*y[0]);
  x2=(-y[1]-d)/(2.0*y[0]);
  if(y[0] <0)
    fmax=Min(x1,x2);       /*  выпуклость */
    else fmax=Max(x1,x2);  /* вогнутость */
  }
  else
  /* аппроксимируем прямой */
  {
  t1=m[0];t2=m[1],t3=m1[0];t4=m1[1] ;
  a0=t2-t1*t1;
  a1=t4-t1*t3;
  a2=t2*t3-t1*t4;
  a1/=a0;
  a2/=a0;
  fmax=-a2/a1;
  }
*extremum=fmax;
//определяем значения координат экстремума
for (i=0;i<ndim;i++)//по числу переменных
  {//1
  m1[0]=m1[1]=m1[2]=m1[3]=0.0;
  for (j=0;j<kr;j++)//по числу интервалов разбиения
    {//2
    r1=r2=0.0;
    for (k=0;k<ni;k++)//по числу стат. испытаний
      {//3
      d=kp[k]-ks[j];
      if(d==0.0)
	{//4
	r1+=bx[k*ndim+i];
	r2+=1.0;
	}//4
	else
	{//4
	if(d<0.0)
	  {//5
	  dl=step(fabs(d),l);
	  r1+=(bx[k*ndim+i]*dl);
	  r2+=dl;
	  }//5
	}//4
      }//3
    xi[i*kr+j]=r1/r2;
    m1[0]+=xi[i*kr+j];
    m1[1]+=(xi[i*kr+j]*ks[j]);
    m1[2]+=(xi[i*kr+j]*ks[j]*ks[j]);
    }//2
  b[0]=m1[2]/(double)kr;b[1]=m1[1]/(double)kr;b[2]=m1[0]/(double)kr;
  mgs(3,a,b,y);
  x[i]=y[0]*fmax*fmax+y[1]*fmax+y[2];
  }//1
//проверка выхода за границы интервалов неопределенности
for (i=0;i<ndim;i++)//по числу переменных
   x[i]=INF_SUP(area[i*2],x[i],area[i*2+1]);
*extremum=func(x,z);
if(flag==__MAX__)
  {
  *extremum=-(*extremum);
  *estimate=fabs(*extremum-fmax);
  *extremum=-(*extremum);
  }
  else
  *estimate=fabs(*extremum-fmax);
//cor=coreleft();h=heapcheck();
free(gp);
//cor=coreleft();h=heapcheck();
free(kp);
//cor=coreleft();h=heapcheck();
free(bp);
//cor=coreleft();h=heapcheck();
free(bx);
//cor=coreleft();h=heapcheck();
free(xi);
//cor=coreleft();h=heapcheck();
//free(ks);
//free(ps);
return 0;
}


double step(double a,int l)
 {
 int i;
 double p;
 for(p=1.0,i=0;i<l;i++)
   p *=a;

return(p);
}