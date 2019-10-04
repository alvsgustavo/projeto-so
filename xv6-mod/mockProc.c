#include "types.h"
#include "stat.h"
#include "user.h"
int next = 10;

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
  int prio = 31;

  if(argc < 2){
    printf(2, "usage: mockProc num...\n");
    exit();
  }
  i = atoi(argv[1]);

  for(; i>0; i--){
      prio = rand(31);
      int lastPid = fork();
      //prio = rand(31);
      if (!lastPid){
          myPid = getpid();
          goto found;
      }
  }
  while (wait() != -1){
    sleep(100);
  }
  exit();

  found:
  setpriority(myPid, prio);
  i = 1000;
  
  /* For deterministic.
  while (1) {

  }
  */

  //Probabilistico
  while (1) {
    
  }
  printf(1, "Terminei (%d): %d\n", myPid, getpriority(myPid));
  exit();
}
