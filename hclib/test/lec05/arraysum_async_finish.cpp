#include "hclib_cpp.h"
#include <sys/time.h>

#define SIZE 100000000

double array[SIZE];

long get_usecs (void)
{
   struct timeval t;
   gettimeofday(&t,NULL);
   return t.tv_sec*1000000+t.tv_usec;
}

void initialize() {
  for(int i=0; i<SIZE; i++) array[i] = (i+1)*0.1;
}

int main(int argc, char **argv) {

  hclib::launch([&]() {
  initialize();

  double sum1 =0, sum2 = 0;

  long start = get_usecs();
  hclib::finish([&]() {
  hclib::async([&]() {
  for(int i=0; i<SIZE/2; i++) {
    sum1 += 1/array[i];
  }
  });

  for(int i=SIZE/2; i<SIZE; i++) {
    sum2 += 1/array[i];
  }

  });
  double sum = sum1 + sum2;

  long end = get_usecs();
  double dur = ((double)(end-start))/1000000;
  printf("Sum = %.3f\n",sum);
  printf("Time = %f\n",dur);

  });

  return 0;
}
