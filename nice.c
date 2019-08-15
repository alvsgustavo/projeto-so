#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
  int pid;
  int prio;
  int oldprio;

  if(argc < 3){
    printf(2, "usage: nice pid priority...\n");
    exit();
  }
  pid = atoi(argv[1]);
  prio = atoi(argv[2]);
  oldprio = getpriority(pid);
  if (setpriority(pid, prio) == prio){
    printf(2, "Priority of process with pid(%d) was: %d. New priority is set to: %d\n", pid, oldprio, prio);
    exit();    
  } else {
    printf(2, "Some error has occured. Pid(%d), oldprio(%d)\n", pid, oldprio);
    exit();
  }
}