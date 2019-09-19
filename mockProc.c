#include "types.h"
#include "stat.h"
#include "user.h"
int seed = 7;

int rand(int max) {
  int a = 16807;
  int m = 2147483647;
  seed = (a * seed) % m;
  int random = seed / max;
  if (random < 0) {
      random *= -1;
  }
  return random % max;
}

int
main(int argc, char **argv)
{
  int i;
  int myPid;
  int prio;

  if(argc < 2){
    printf(2, "usage: mockProc pid...\n");
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
  while(wait() != -1){
      sleep(10);
  }
  exit();

  found:
  setpriority(myPid, prio);
  i = 100;
  while (i) {
      sleep(1);
      i --;
  }
  printf(1, "Terminei (%d): %d\n", myPid, getpriority(myPid));
  exit();
}
