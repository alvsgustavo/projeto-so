#include "types.h"
#include "stat.h"
#include "user.h"
int next = 7;

int rand(int max)
{
    next = next * 1103515243 + 12345;
    if (max != 0)
      return (unsigned int)(next / 65536) % 32768 % max;
    else
      return (unsigned int)(next / 65536) % 32768;
}

int
main(int argc, char **argv)
{
  int i;
  int myPid;
  int prio;

  if(argc < 2){
    printf(2, "usage: mockProc num...\n");
    exit();
  }
  i = atoi(argv[1]);

  for(; i>0; i--){
      int lastPid = fork();
      prio = rand(31);
      if (!lastPid){
          myPid = getpid();
          goto found;
      }
  }
  exit();

  found:
  setpriority(myPid, prio);
  i = 1000;
  while (1) {
      //sleep(1);
      //i --;
  }
  printf(1, "Terminei (%d): %d\n", myPid, getpriority(myPid));
  exit();
}
