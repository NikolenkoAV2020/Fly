#include "ssp.h"

int type=2;
void overfl( int *n )
{
*n=type;
return;
}
/*
int matherr (struct exception *a)
{
  if (a->type == OVERFLOW)
    type=1;
    else type=2;
  if(type==0)
    return 1;
  return 0;
}
*/