#include "kernel/types.h"
#include "user.h"

static int
test_self_yield(void)
{
  if(co_yield(getpid(), 1) != -1){
    printf("FAIL: self-yield did not return -1\n");
    return 1;
  }
  return 0;
}

static int
test_nonexistent_pid(void)
{
  if(co_yield(99999, 1) != -1){
    printf("FAIL: non-existent pid did not return -1\n");
    return 1;
  }
  return 0;
}

static int
test_killed_pid(void)
{
  int victim = fork();
  if(victim < 0){
    printf("fork failed\n");
    exit(1);
  }
  if(victim == 0){
    for(;;);
  }
  kill(victim);
  wait(0);
  if(co_yield(victim, 1) != -1){
    printf("FAIL: killed pid did not return -1\n");
    return 1;
  }
  return 0;
}

static void
run_error_tests(void)
{
  int fails = 0;
  fails += test_self_yield();
  fails += test_nonexistent_pid();
  fails += test_killed_pid();

  if(fails == 0)
    printf("co_test: all three error cases returned -1\n");
  else
    printf("co_test: %d error-case failure(s)\n", fails);
}

static void
run_pingpong(void)
{
  int pid1 = getpid();
  int pid2 = fork();

  if(pid2 < 0){
    printf("fork failed\n");
    exit(1);
  }

  if(pid2 == 0){
    for(;;){
      int value = co_yield(pid1, 1);
      printf("Child received: %d\n", value);
    }
  } else {
    for(;;){
      int value = co_yield(pid2, 2);
      printf("parent received: %d\n", value);
    }
  }
}

int
main(int argc, char *argv[])
{
  run_error_tests();
  run_pingpong();
  exit(0);
}