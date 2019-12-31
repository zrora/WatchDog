/*WD_INF*/
#define _POSIX_SOURCE
#define _BSD_SOURCE

#include "../../utils/Sched.h" /*schedualer*/
#include "../../utils/Task.h"/*schadualer Tasks*/
#include <pthread.h> /*multithreads*/
#include <sys/types.h>/*Proccesses*/
#include <signal.h>/*signals between processes*/
#include <stdlib.h>/*exit func*/
#include <stdio.h>/*printg*/
#include <string.h>/*strcmp*/

static void RegisterSignalHandlers(void);
static void SignalHandler2 (int flag);
static void SignalHandler (int flag);
static void *TreadsSched(void *other_pid);
static int SendAlive (void * other_pid);
static int ReciveAlive (void * other_pid);

pid_t other_process_pid;
sig_atomic_t gotping = 0;
sig_atomic_t keep_wd_alive = 1;

struct sigaction process;
struct sigaction process2;
char* const* argv_g = NULL;

/*will terminate the other programe*/

void DNR(pid_t wd_pid)
{
  kill(wd_pid, SIGUSR2);
}

pid_t MMI(int argc, char* const *argv, const char **env)
{
  /* pid_t other_process_pid;*/
  pthread_t new_thread = 0;
  RegisterSignalHandlers();

  argv_g = argv;

  printf("                               mmi!!!!!!!!!! argv_g[0] = %s\n", argv[0]);

  /*first time running*/
  if (!getenv("RUNNING_FIRST_TIME"))
  {
    setenv ("RUNNING_FIRST_TIME", "TRUE", 1);
    printf("   first time    \n");

    /*start new process*/
    other_process_pid = fork();

    /*parent*/
    setenv ("I_AM", "APP", 1);
    if (other_process_pid != 0)
    {
      printf("parent here other_process_pid = %d \n", other_process_pid);
      /*open a new thread that will have the schedualler*/
      pthread_create(&new_thread, NULL, TreadsSched, &other_process_pid);
    }
    /*child*/
    else
    {
      printf("opening wd.out\n");
      execvp("./wd.out", argv_g);/*open wd.out as a new proccess*/
    }
  }
  /*caller is wd --> only create thread*/
  else
  {
    printf("   second time    \n");

    if(strcmp(getenv("I_AM"),"APP") == 0)
    {
      printf("   chaning i am to wd    \n");

      setenv("I_AM","WD", 1);
    }

    else if(strcmp(getenv("I_AM"),"WD") == 0)
    {
      printf("   chaning i am to app    \n");

      setenv("I_AM","APP", 1);
    }

    other_process_pid = getppid();
    printf("\n                        now opening new thread %d\n", other_process_pid);
    pthread_create(&new_thread, NULL, TreadsSched, &other_process_pid);
  }
  return getppid();
}

static void SignalHandler2 (int flag)
{
  keep_wd_alive = 0;
}

/*will ping the other programe*/
static void SignalHandler (int flag)
{
  /*I Got The Signal
  printf("ping pid = %d\n",getpid());
  */
  gotping = 1;
}

static void RegisterSignalHandlers(void)
{
  process.sa_handler = &SignalHandler;
  process2.sa_handler = &SignalHandler2;

  sigaction (SIGUSR1, &process, NULL);
  sigaction (SIGUSR2, &process2, NULL);/*will terminate the other process*/
}

static void *TreadsSched(void *other_pid)
{
  sch_t *main_s = SchCreate();
  /*
  printf("sched is being created pid = %d\n",(pid_t *)other_pid);
  */
  /*  send evevry 1 sec*/
  SchAdd(main_s, SendAlive, other_pid, 1);
  /*recive every 1 sec  */
  SchAdd(main_s, ReciveAlive, other_pid, 3);

  SchRun(main_s);

  return NULL;
}

static int SendAlive (void * other_pid)
{
  /*
  send signal to other process*/
  printf("%d sending iamnv = %s\n",getpid(),getenv("I_AM"));
  printf("%d is sending to %d\n",getpid(),*(pid_t *)other_pid);
  kill(*(pid_t *)other_pid, SIGUSR1);
  /*
  rerun
  */
  return 1;
}

static int ReciveAlive (void * other_pid)
{

  if(gotping == 1)
  {
    gotping = 0;
  }
  else
  {
    printf("other process is not responding\n");
    gotping = 0;/*to give time for other process to open - replace with semaphore?*/
    other_process_pid = fork();
    /*current process(parent)*/

    printf("%d Opening new process because no signal was recived iamnv = %s\n",getpid(),getenv("I_AM"));
    if(other_process_pid == 0)
    {
      /*open other process*/

      if(strcmp(getenv("I_AM"),"WD") == 0)
      {
        printf("                                               wapp is opend\n");
        execvp(argv_g[0], argv_g);/*open wd.out as a new proccess*/
      }

      else
      {
        printf("                                               wd is opend\n");
        execvp("./wd.out", argv_g);/*open wd.out as a new proccess*/
      }
    }
  }

  return 1;
}
