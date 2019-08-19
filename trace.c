#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
  int pid;
  int sysNumber;
  int callCounts;

  if(argc < 3){
    printf(2, "usage: trace pid sysNumber...\n");
    exit();
  }
  pid = atoi(argv[1]);
  sysNumber = atoi(argv[2]);
  callCounts = trace(pid, sysNumber);
  
  if (callCounts == -1){
    printf(2, "Something went wrong. Possibly process id invalid, syscall number invalid or system isn't tracking this syscall yet.\n");
  } else {
    printf(1, "The process with the pid(%d) has made %d syscalls of type(%d).\n", pid, callCounts, sysNumber);
  }

  exit();
}