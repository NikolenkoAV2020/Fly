//------------------------------------------------------------------------------
#include <stdafx.h>
#include <math.h>
#include <string.h>
#include <FlyMMathSource\\MMathDef.h>
#pragma hdrstop

//------------------------------------------------------------------------------
//  Svd.cpp
//------------------------------------------------------------------------------
int USV(double* a, double* w, double* v, double* work, int m, int n, bool notNegativSp )
{
    short ierr;
    double g,scale,anorm,s,f,h,c,y,z,x;
    //double *work;
    short  i,j,k,i1,l1,ii,kk,ll,k1,l,its,mn;
    // массивы a(m,n),w(n),v(n,n)
    // необходима пам€ть под рабочий массив work[n]
    // work=(double *)malloc(sizeof(double)*max(m,n));
    ierr=0 ;      
	g=0.0;      
	scale=0.0;      
	anorm=0.0;
    //’аусхолдерово приведение к двухдиагональной форме
    for(i=0;i<m;i++) *(w+i)=0.0;
    for(i=1;i<=n;i++) {
      l=i+1;      
	  work[i-1]=scale*g;      
	  g=0.0;      
	  s=0.0;      
	  scale=0.0;
      if(i>m) goto l210;
      for(k=i;k<=m;k++) scale+=fabs(a[(k-1)*n+i-1]);
      if(scale==0.) goto l210;
      for(s=0.0,k=i;k<=m;k++) {
        a[(k-1)*n+i-1]=a[(k-1)*n+i-1]/scale;
        s+=(a[(k-1)*n+i-1]*a[(k-1)*n+i-1]);
      }
      f=a[(i-1)*n+i-1];      
	  g=-sqrt(s)*Sign1(f);
      h=f*g-s;      
	  a[(i-1)*n+i-1]=f-g;
      if(i==n) goto l190;
      for(j=l;j<=n;j++)  {
        s=0.0;
        for(k=i;k<=m;k++) s+=(a[(k-1)*n+i-1]*a[(k-1)*n+j-1]);
        f=s/h;
        for(k=i;k<=m;k++) a[(k-1)*n+j-1]=a[(k-1)*n+j-1]+f*a[(k-1)*n+i-1];
      }
l190: for(k=i;k<=m;k++) a[(k-1)*n+i-1]=scale*a[(k-1)*n+i-1];
l210: w[i-1]=scale*g;
      g=0.0;      s=0.0;      scale=0.0;
      if(i>m || i==n) goto l290;
      for(k=l;k<=n;k++) scale+=fabs(a[(i-1)*n+k-1]);
      if(scale==0.0) goto l290;
      for(k=l;k<=n;k++) {
        a[(i-1)*n+k-1]=a[(i-1)*n+k-1]/scale;
        s+=(a[(i-1)*n+k-1]*a[(i-1)*n+k-1]);
      }
      f=a[(i-1)*n+l-1];      g=-sqrt(s)*Sign1(f);
      h=f*g-s;      a[(i-1)*n+l-1]=f-g;
      for(k=l;k<=n;k++) work[k-1]=a[(i-1)*n+k-1]/h ;
      if(i==m) goto l270;
      for(j=l;j<=m;j++) {
        s=0.0;
        for(k=l;k<=n;k++) s+=(a[(j-1)*n+k-1]*a[(i-1)*n+k-1]);
        for(k=l;k<=n;k++) a[(j-1)*n+k-1]=a[(j-1)*n+k-1]+s*work[k-1];
      }
l270: for(k=l;k<=n;k++) a[(i-1)*n+k-1]=scale*a[(i-1)*n+k-1];
l290: anorm=Max(anorm,fabs(w[i-1])+fabs(work[i-1]));
    }
    //Ќакопление правосторонних преобразований
    for(ii=1;ii<=n;ii++) {
      i=n+1-ii;
      if(i==n) goto l390 ;
      if(g==0.0) goto l360;
      //двойное деление обходит машинный нуль
      for(j=l;j<=n;j++) v[(j-1)*n+i-1]=(a[(i-1)*n+j-1]/a[(i-1)*n+l-1])/g;
	for(j=l;j<=n;j++) {
	  for(s=0.0,k=l;k<=n;k++) s+=(a[(i-1)*n+k-1]*v[(k-1)*n+j-1]);
	  for(k=l;k<=n;k++) v[(k-1)*n+j-1]=v[(k-1)*n+j-1]+s*v[(k-1)*n+i-1];
      }
l360: for(j=l;j<=n;j++)	{
	v[(i-1)*n+j-1]=0.0;      v[(j-1)*n+i-1]=0.0;
      }
l390: v[(i-1)*n+i-1]=1.0;	 g=work[i-1];
      l=i;
    }
    //Ќакопление левосторонних преобразований
    mn=Min(n,m) ;
    for(ii=1;ii<=mn;ii++) {
      i=mn+1-ii;      l=i+1;
      g=w[i-1];
      if(i==n)  goto l430;
      for(j=l;j<=n;j++)	a[(i-1)*n+j-1]=0.0;
l430: if(g==0.0) goto l475;
      if(i==mn) goto l460;
      for(j=l;j<=n;j++)	{
        for(s=0.0,k=l;k<=m;k++)	s+=(a[(k-1)*n+i-1]*a[(k-1)*n+j-1]);
	f=(s/a[(i-1)*n+i-1])/g;
	for(k=i;k<=m;k++) a[(k-1)*n+j-1]+=(f*a[(k-1)*n+i-1]);
      }
l460: for(j=i;j<=m;j++) a[(j-1)*n+i-1]/=g;
      goto l490;
l475: for(j=i;j<=m;j++) a[(j-1)*n+i-1]=0.0;
l490: a[(i-1)*n+i-1]+=1.0;
    }
    //диагонализаци€ двухдиагональной формы
    for(kk=1;kk<=n;kk++) {
      k1=n-kk;      k=k1+1;      its=0;
l520:
      //проверка возможности расщеплени€
      for(ll=1;ll<=k;ll++) {
	l1=k-ll;      l=l1+1;
	if((fabs(work[l-1])+anorm)==anorm) goto l565;
	// work[0] всегда равно 0,поэтому выхода через конец цикла не будет
	if((fabs(w[l1-1])+anorm)==anorm)  goto l540;
      }
l540: // если l>1,то work[l-1]=0
      c=0.0;      s=1.0;
      for(i=l;i<=k;i++)	{
	f=s*work[i-1];      work[i-1]=c*work[i-1];
	if((fabs(f)+anorm)==anorm)  goto l565;
	g=w[i-1];
	h=sqrt(fabs(f))*sqrt(fabs(g))*sqrt(2.0);
	w[i-1]=fabs(g)+fabs(f);
	h=sqrt(fabs(h+w[i-1]))*sqrt(fabs(w[i-1]-h));
	w[i-1]=h;      c=g/h;      s=-f/h;
	for(j=1;j<=m;j++) {
	  y=a[(j-1)*n+l1-1];	    z=a[(j-1)*n+i-1];
	  a[(j-1)*n+l1-1]=y*c+z*s;  a[(j-1)*n+i-1]=-y*s+z*c;
	}
      }
l565: // проверка сходимости
      z=w[k-1];
      if(l==k) goto l650;
      if(its==30) goto l1000;
      its=its+1;
      x=w[l-1];      y=w[k1-1];
      g=work[k1-1];  h=work[k-1];
      f=((y+z)/h*(1.0-z/y)+(g+h)/y*(g/h-1.0))/2.0;
      g=1.0+fabs(f);
      c=sqrt(2.0)*sqrt(fabs(f));
      g=sqrt(fabs(g+c))*sqrt(fabs(g-c));
	  f = (x-z)*(z/x+1.0)+h*(y/(f+g*Sign1(f))-h)/x;
      //следующее QR-обращение
      c=1.0;      s=1.0;
      for(i1=l;i1<=k1;i1++) {
	i=i1+1;     g=work[i-1];   y=w[i-1];
	h=s*g;      g=c*g;         z=fabs(f)+fabs(h);
	c=sqrt(fabs(f))*sqrt(fabs(h))*sqrt(2.0);
	z=sqrt(fabs(z+c))*sqrt(fabs(z-c));
	if(z==0.0) goto l600;
	work[i1-1]=z;
	c=f/z;	    s=h/z;
	f=x*c+g*s;  g=-x*s+g*c;
	h=y*s;      y=y*c;
	for(j=1;j<=n;j++) {
	  x=v[(j-1)*n+i1-1];         z=v[(j-1)*n+i-1];
	  v[(j-1)*n+i1-1]=x*c+z*s ;  v[(j-1)*n+i-1]=-x*s+z*c;
	}
	z=sqrt(fabs(f))*sqrt(fabs(h))*sqrt(2.0);
	w[i1-1]=fabs(f)+fabs(h);
	z=sqrt(fabs(z+w[i1-1]))*sqrt(fabs(w[i1-1]-z));
	w[i1-1]=z;
	// вращение может быть произведено если z=0
	if(z==0.0) goto l580;
	c=f/z;      s=h/z;
l580:	f=c*g+s*y;  x=-s*g+c*y;
	for(j=1;j<=m;j++) {
	  y=a[(j-1)*n+i1-1];      z=a[(j-1)*n+i-1];
	  a[(j-1)*n+i1-1]=y*c+z*s;      a[(j-1)*n+i-1]=-y*s+z*c;
	}
l600:;
      }
      work[l-1]=0.0;      work[k-1]=f;      w[k-1]=x;
      goto l520;
l650: // сходимость
      if ( notNegativSp ) {
        if(z< 0.0) {
	  w[k-1]=-z; //w[k-1] делаетс€ неотрицательным
          // соответствующий орт направл€ем в противоположенную сторону
	  for(j=1;j<=n;j++) v[(j-1)*n+k-1]=-v[(j-1)*n+k-1];
      } }
    }
    goto l1010;
l1000:
    ierr=k;
   // printf("ќшибка пpи нахождении %4d-го синг. числа",ierr);
l1010:
return ierr ;
}

int USVTestS( double* U, double* S, double* V, int n, int m, double* W )
{
    int k, i, j, l, N = Min( n, m ) ;
    double  d ;
    double* w = ( W ? W : new double[ N*N ] ) ;
    memset( w, 0, N*N*sizeof( double ) ) ;
    for( k = N - 1 ; k > 0; k-- ) {
      if ( fabs( S[ k ] ) > fabs( S[ k - 1 ] ) ) {
        // ћен€ем местами сингул€рные числа
        d = S[ k ] ; S[ k ] = S[ k - 1 ] ; S[ k - 1 ] = d ;
        // мен€ем местами орты i и i-1
        for( i = 0 ; i < n ; i++ ) {
          for( j = 0 ; j < m ; j++ ) {
            w[ i*m + j ] = 0 ;
            for( l = 0 ; l < n ; l++ ) {
              d = ( l == i ? 1.0 : 0.0 ) ;
              if ( l == k || l == k - 1 ) d = 0 ;
              w[ i*m + j ] += d*V[ l*m + j ] ;
            }
        } }
      }
    }
    if ( w && !W ) delete[] w ; w = 0 ;
    return 0 ;
}



