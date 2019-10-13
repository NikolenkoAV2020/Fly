/*---------------------------------------------------------------------------
  MGS.C
    ……… ‘‹“ …’„ ƒ€“‘‘€ ‘ ‚› €‘€‹ƒ ‹……’€
  €Άβ®ΰ   :  αλ­ª®Ά ‚.‚.
  ‚¥ΰα¨ο  : 01.00 / 10.08.1997 /
---------------------------------------------------------------------------*/
#include <malloc.h>
#include <math.h>
#include "ssp.h"

/*
//’¥αβ
int main(void)
{
double a[9],b[3], y[3];
int    ier;
a[0]=1.0;a[1]=2.0;a[2]=3.0;
a[3]=4.0;a[4]=5.0;a[5]=6.0;
a[6]=7.0;a[7]=8.0;a[8]=1.0;
b[0]=-1 ;b[1]=2;b[2]=3.0;
//ΰ¥θ¥­¨¥ 3.25 -2.5 0.25 ier=0;
ier=mgs(3,a,b,y);
return ier;
}
*/

int  mgs(int n,double *a,double *b,double *x)
{
double   c,*y,*f,d;
int i,k,l,j,m,r;
  if(n<1) return -2;
  if(!a || !b || !x) return -1;
  k=n+1;      l=n-1;
  y=(double *)malloc (sizeof(double)*k);
  f=(double *)malloc (sizeof(double)*k*n);
  if(!y || !f) return -3;


  for(i=0;i<n;i++)
    {
    for(j=0;j<n;j++)
      *(f+(i*k)+j)=*(a+(i*n)+j);
    *(f+(i*k)+n)=b[i];
    x[i]=0;
    }
  for(j=0;j<l;j++)
    {
    for(d=0,i=j;i<n;i++)
       {
       c=fabs(*(f+(i*k)+j));
       if(d<c)
	 {
	 d=c;   m=i;
	 }
       }
    if (d==0) return (-3);
    for(i=0;i<k;i++)
      y[i]=*(f+(m*k)+i) /  *(f+(m*k)+j);
    for(i=0;i<k;i++)
      *(f+(m*k)+i)=*(f+(j*k)+i);
    for(i=0;i<k;i++)
      *(f+(j*k)+i)=y[i];
    for(i=j+1;i<n;i++)
      {
      for(r=0;r<k;r++)
	*(y+r)=*(f+(j*k)+r)* *(f+(i*k)+j);
      for(r=0;r<k;r++)
	*(f+(i*k)+r)-=*(y+r);
      }
    }
  *(x+l)=*(f+(l*k)+n) / *(f+(l*k)+l);
  for(j=l-1;j>=0;j--)
     {
     for(d=0.0,i=j;i<n;i++)
       d+=(*(f+(j*k)+i) * x[i]);
     *(x+j)=*(f+(j*k)+n)-d;
     }
  free( f );  free( y );
return 0;
}
