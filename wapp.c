/*24-7 app */
#include <stdio.h> /*printf*/
#include "watchdog.h" /*watch dog */
#include <stdlib.h>/*exit*/
#include <unistd.h>/*sleep*/
int main(int argc, const char **argv, const char **env)
{
  int i = 0;

  pid_t watchdog_pid = 0;

  watchdog_pid = MMI(argc, argv, env);
  printf("wapp is running\n");
  sleep(1);

  for (;i < 50; ++i)
  {
    printf("wapp printing stuff wdpid = %d %d\n",watchdog_pid,i);
    sleep(1);
  }
  sleep(1);
  /*
  exit(0);
  */
  DNR(watchdog_pid);
  printf("                          SUICIDE!!!!!!!!!!!!!!\n");
  /*
  */

  return 0;
}
