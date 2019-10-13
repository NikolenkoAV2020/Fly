/*------------------------------------------------------------------------
  SPLINE.C

  Автор   : Перевод программы С c FORTRAN (Форсайт) выполнил Пасынков В.В.
  Версия  : 01.00 / 21.05.2000 /
------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

/* ПOДПP.BЫЧИCЛ.CПЛAЙH-ФYHKЦИИ S(X) */
/*   S(X)=Y(I)+B(I)*(X-X(I))+C(I)*(X-X(I))**2+D(I)*(X-X(I))**3
  ДЛЯ X(I).LE.X.LE.X(I+1)
       BXOД:
 N - ЧИCЛO YЗЛOB (N.GE.2)
 X - AБCЦИCCЫ YЗЛOB B CTPOГOM BOЗP.
 Y - OPДИHATЫ YЗЛOB
   BЫXOД:
 B,C,D - MACCИBЫ KOЭФФ.
 Если P символ дифференцирования, то
 Y=S(X);
 B=SP(X);
 C=SPP(X)/2;
 D=SPPP(X)/6;//правосторонняя производная
void zconr( double b[], double val, int n)
{
int i;
for(i=0;i<n;i++)
  b[i]=val;
}
 */


void spline(int n, double x[], double y[], double b[], double c[], double d[])
{
int    i, ib, nm1;
double t;


/*
zconr( b, 0.e0, n );
zconr( c, 0.e0, n );
zconr( d, 0.e0, n );
*/

for(i=0;i<n;i++)
  b[i]=c[i]=d[i]=0.0;

nm1 = n - 1;
if( n >= 2 )
	goto L_21;
/*
pria( "#SPLINE#:N<2", ADR(_i0,2) );
pric( "N=    ", n, ADR(_i0,1) );
f_exit(0L);
*/
return;
L_21:
if( n < 3 )
	goto L_50;
/*
  ПOCTP. 3-X ДИAГOH. MATPИЦЫ CЛAУ
	 B-ДИAГOHAЛЬ,D-HAДДИAГOHAЛЬ,ПOДДИAГOHAЛЬ
	 C-правые части
*/
d[0] = x[1] - x[0];
c[1] = (y[1] - y[0])/d[0];
for( i = 2; i <= nm1; i++ ){
	d[i-1] = x[i + 1-1] - x[i-1];
	b[i-1] = 2.0*(d[i - 1-1] + d[i-1]);
	c[i + 1-1] = (y[i + 1-1] - y[i-1])/d[i-1];
	c[i-1] = c[i + 1-1] - c[i-1];
	}
/*
   ГPAH.YCЛ. 4-ГO TИПA(PAB-BO 3-X ПPOИЗB-X)
   Третьи производные в точках X[1] и X[N-1] вычисляются
   с помощью разделенных разностей.
*/
b[0] = -d[0];
// b[n-1] = d[n-1]; //исправлено 10.03.97 была ошибка
b[n-1] = - d[n-2];
c[0] = 0.e0;
c[n-1] = 0.e0;
if( n == 3 )
	goto L_15;
c[0] = c[2]/(x[3] - x[1]) - c[1]/(x[2] - x[0]);
c[n-1] = c[n - 1-1]/(x[n-1] - x[n - 2-1]) - c[n - 2-1]
 /(x[n - 1-1] - x[n - 3-1]);
c[0] = (c[0]*pow(d[0], 2.0))/(x[3] - x[0]);
c[n-1] = -c[n-1]*pow(d[n - 1-1], 2.0)/(x[n-1] - x[n - 3-1]);

/*  PEШEHИE CИCT METOДOM ПPOГOHKИ ПPЯMOЙ XOД */
L_15:
for( i = 2; i <= n; i++ ){
	t = d[i - 1-1]/b[i - 1-1];
	b[i-1] = b[i-1] - t*d[i - 1-1];
	c[i-1] = c[i-1] - t*c[i - 1-1];
	}

/*           OБPATHЫЙ XOД (подстановка)*/
c[n-1] = c[n-1]/b[n-1];
for( ib = 1; ib <= nm1; ib++ ){
	i = n - ib;
	c[i-1] = (c[i-1] - d[i-1]*c[i + 1-1])/b[i-1];
	}
// в С теперь хранится величина sigma
/*  KOЭФФИЦ.CПЛAЙHA */
b[n-1] = (y[n-1] - y[nm1-1])/d[nm1-1] - d[nm1-1]*(c[nm1-1] + 2.0*c[n-1]);
for( i = 1; i <= nm1; i++ ){
	b[i-1] = (y[i + 1-1] - y[i-1])/d[i-1] - d[i-1]*(c[i + 1-1] + 2.0*
	 c[i-1]);
	d[i-1] = (c[i + 1-1] - c[i-1])/d[i-1];
	c[i-1] = 3.0*c[i-1];
	}
c[n-1] = 3.e0*c[n-1];
d[n-1] = d[n - 1-1];
return;
/*      ЛИHEЙHAЯ ИHTEPПOЛЯЦИЯ */
L_50:
b[0] = (y[1] - y[0])/(x[1] - x[0]);
c[0] = 0.e0;
d[0] = 0.e0;
b[1] = b[0];
c[1] = 0.e0;
d[1] = 0.e0;
return;
}

