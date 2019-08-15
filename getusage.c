#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
  int pid;
  int usage;

  if(argc < 2){
    printf(2, "usage: getusage pid...\n");
    exit();
  }
  pid = atoi(argv[1]);
  usage = getusage(pid);
  if (usage == -1){
    printf(2, "Something went wrong. Possibly process id invalid.\n");
  } else {
    printf(2, "The process with the pid(%d) has taken control over the cpu %d times.\n", pid, usage);
  }
  exit();
}