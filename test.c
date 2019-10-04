#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
  int pid = getpid();
  ps();

  setpriority(pid, 10);
  printf(2, "FATHER(%d) priority: %d\n", pid, getpriority(pid));
  if (!fork()){
    ps();
    printf(2, "CHILD(%d) priority: %d\n", getpid(), getpriority(getpid()));
    setpriority(getpid(), 10);
    printf(2, "CHILD(%d) USAGE: %d\n", getpid(), trace(getpid(), 24));
  } else {
    wait();
    printf(2, "FATHER(%d) usage: %d\n", getpid(), trace(getpid(), 24));
    for(int i = 0; i < 10; i++){
      getusage(1);
    }
    printf(2, "FATHER(%d) usage: %d, pid: %d\n\n", getpid(), trace(getpid(), 24));
  }

  exit();

}