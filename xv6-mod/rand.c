#include "types.h"
#include "stat.h"
#include "user.h"

long seed = 1;

int rand(int max) {
  int a = 16807;
  int m = 2147483647;
  seed = (a * seed) % m;
  printf(1, "seed: %d", seed);
  int random = seed / max;
  printf(1, "rnd: %d\n", random);
  if (random < 0) {
      random *= -1;
  }
  return random % max;
}

int
main(int argc, char **argv)
{
  for (int i = 0; i < 100; i++){
      printf(1, "result: %d\n", rand(10));
  }
  exit();
}