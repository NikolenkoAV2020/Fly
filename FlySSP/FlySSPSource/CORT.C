#include <stdio.h>
#include <math.h>
#include "ssp.h"

  /*расчет временной корреляции*/
int  cort(int n,int m,double x[], double *sxm,double *sx)
{
int    code,i;
double sumx,sumx2,d,sum;
code=0;
if(x==NULL || sxm==NULL || sx==NULL)
  return code;
if(m<0 || n<=0)
  return code;
if(m>n)
  return code;
code=1;
for(sumx=0.0,i=0;i<n;i++)
  sumx+=x[i];
sumx/=n;
for(sumx2=0.0,i=0;i<n;i++)
  {
  d=(sumx-x[i]);
  sumx2+=(d*d);
  }
sumx2/=(n-1);
*sx=sumx2;
for(sum=0.0,i=0;i< n-m ;i++)
  {
  sum+=x[i]*x[i+m];
  }
if(sum>1e-15)
  *sxm=sum/(*sx);
  else *sxm=0.0;
return code;
}
