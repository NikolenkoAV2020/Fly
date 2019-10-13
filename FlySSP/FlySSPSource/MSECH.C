/*------------------------------------------------------------------------
  MSECH.C
	  ВЫЧИСЛЕНИЕ НУЛЯ ФУНКЦИИ f(x) НА ИНТЕРВАЛЕ [a,b]
	  МЕТОДОМ СЕКУЩИХ-ХОРД
  Автор   : Пасынков В.В.
  Версия  : 01.00 / 1.02.1997 /
------------------------------------------------------------------------*/
#include <math.h>


double msechord(const double a,
		const double b,
		short *iter,
		float tol,
		double (*fuser)(const double ,const void *),
		const void *userdata)
{
// Поиск корня нелинейного уравнения f(x) на интервале [a,b]
// методом секущих-хорд (в основе итерационная формула Ньютона).
// Тест: для f(x)= x**3 - 2*x - 5, корень x=2.0945514815 на интервале
//       x=[2,3]
//                         Входные параметры:
//  a    - левая граница поиска корня,
//  b    - правая граница поиска корня,
//  iter - максимальное число итераций (если iter<=0, то по умолчанию 10),
//  tol  - погрешность вычислений корня в области значений функции,
//  fuser(x,userdata)  - подпрограмма пользователя, возвращающая значение
//                       функции f(x) для значения аргумента x с
//                       использованием вспомогательных данных userdata,
//  userdata - адрес вспомогательных данных, используемых при вычислениях
//             корня, либо NULL.
//                        Входные параметры:
//  возвращаемое значение подпрограммы - корень
//  iter >  0 - фактическое число итераций для достижения заданной
//              точности tol поиска корня f(x);
//       = -1 - ошибка исходных данных: между точками а и b функция не
//              пересекает ось x;
//       =  0 - заданная точность tol не достигнута из-за ограничения по
//              числу итераций iter;


short maxiter=*iter,i;
double df,f1,f2,x1,x2,x,step;//eps,
if(maxiter<=0)
   maxiter=10;
//eps=macheps();
x1=a;
x2=b;
f1=fuser(x1,userdata);
f2=fuser(x2,userdata);
if( f1*f2 > 0 )
   {
   *iter=-1;//ошибка в ИД: кривая не пересекает ось
   x=x1;
   return x;
   }
i=0;
x=x1;
do
  {
  df=f2-f1;
  if( df)
    x=x2-(x2-x1)*f2/(f2-f1);
    else break;
  if(fabs(f1)>fabs(f2))   {step=f1;x1=x;f1=fuser(x,userdata);}
     else {step=f2;x2=x;f2=fuser(x,userdata);}
  }while( ( fabs(step)) > tol  &&  ++i <= maxiter);
if(fabs(step) < tol)
  *iter=0;
  else *iter=i;

if(fabs(f1)<fabs(f2))   x=x1;
     else x=x2;
return x;
}
/*
//  тест
double ftest(const double x,const void *data)
{
double f;
if(data==NULL)
  f=0;
f=x* (x*x-2.0) -5;
return f;
}

void main(void)
{
double x,f,a=2.0,b=3.0;
short iter=10;
float tol=1e-15;
x=msechord(a,b,&iter,tol,ftest,NULL);
f=x;
a=f;
return;
}
*/