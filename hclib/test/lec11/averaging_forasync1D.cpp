#include"hclib_cpp.h"
#include<algorithm>
#include<stdio.h>
#include<string.h>
#include<cmath>
#include<sys/time.h>

/*
 * Ported from HJlib
 *
 * Author: Vivek Kumar
 *
 */

//#define VERIFY
//48 * 256 * 2048
#define SIZE 25165824
#define ITERATIONS 64

double* myNew, *myVal, *initialOutput;
int n;

long get_usecs () {
  struct timeval t;
  gettimeofday(&t,NULL);
  return t.tv_sec*1000000+t.tv_usec;
}
 
int ceilDiv(int d) {
  int m = SIZE / d;
  if (m * d == SIZE) {
    return m;
  } else {
    return (m + 1);
  }
}

void validateOutput() {
  for (int i = 0; i < SIZE + 2; i++) {
    double init = initialOutput[i];
    double curr = myVal[i];
    double diff = std::abs(init - curr);
    if (diff > 1e-20) {
      printf("ERROR: validation failed!\n");
      printf("Diff: myVal[%d]=%.3f != initialOutput[%d]=%.3f",i,curr,i,init);
      break;
    }
  }
}

void runSequential() {
  for (int iter = 0; iter < ITERATIONS; iter++) {
    for(int j=1; j<=SIZE; j++) {
        myNew[j] = (initialOutput[j - 1] + initialOutput[j + 1]) / 2.0;
    }
    double* temp = myNew;
    myNew = initialOutput;
    initialOutput = temp;
  }
}

void runParallel() {
  // note we are not using ceil(...) as we did in async-finish version.
  // If there are some remainder iterations after division then forasync1D
  // will take care of it
  // Also, note that we have highBound=SIZE+1 (matching the runSequential) 
  hclib::loop_domain_t loop = {1, SIZE+1, 1, SIZE/hclib::num_workers()}; 
  for (int iter = 0; iter < ITERATIONS; iter++) {
    hclib::finish([&]() {
      hclib::forasync1D(&loop, [=](uint64_t j) {
        myNew[j] = (myVal[j - 1] + myVal[j + 1]) / 2.0;
      }, FORASYNC_MODE_RECURSIVE);
    });
    double* temp = myNew;
    myNew = myVal;
    myVal = temp;
  }
}

int main(int argc, char** argv) {
  hclib::launch([&]() {
  myNew = new double[(SIZE + 2)];
  myVal = new double[(SIZE + 2)];
  initialOutput = new double[(SIZE + 2)];
  memset(myNew, 0, sizeof(double) * (SIZE + 2));
  memset(myVal, 0, sizeof(double) * (SIZE + 2));
  memset(initialOutput, 0, sizeof(double) * (SIZE + 2));
  initialOutput[SIZE + 1] = 1.0;
  myVal[SIZE + 1] = 1.0;
#ifdef VERIFY
  runSequential();
#endif
  long start = get_usecs();
  runParallel();
  long end = get_usecs();
  double dur = ((double)(end-start))/1000000;
#ifdef VERIFY
  validateOutput();
#endif
  printf("Time = %.3f\n",dur);
  delete(myNew);
  delete(myVal);
  delete(initialOutput);
  });
}

