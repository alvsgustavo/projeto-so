#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
  setpriority(getpid(), 20);
  if (!fork()) {
    printf(1, "son priority: %d", getpriority(getpid()));
    exit();
  } else {
    wait();
    exit();
  }
  
}