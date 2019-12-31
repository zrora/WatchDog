/*WD.out*/
#include <stdio.h>
#include<signal.h>
#include "watchdog.h"
extern sig_atomic_t keep_wd_alive = 1;
int main(int argc, const char **argv, const char **env)
{
  printf("WD.out opened\n");

  MMI(argc, argv, env);

  while(keep_wd_alive)
  {
    pause();
  }
  printf("           TIME TO DIE\n\n\n\n\n\n");
  return 0;
}
