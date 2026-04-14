#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("memory size before malloc: %d\n", memsize());

  char *p = malloc(20000);

  printf("memory size after malloc: %d\n", memsize());

  free(p);

  printf("memory size after free: %d\n", memsize());

  exit(0);
}
