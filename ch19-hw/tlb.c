#include <assert.h>
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

const int PAGESIZE = 4096;
#define ARR_TYPE uint32_t

double Time_GetMicroSeconds() {
  struct timeval t;
  int rc = gettimeofday(&t, NULL);
  assert(rc == 0);
  return (double)((double)t.tv_sec * 1e6 + (double)t.tv_usec);
}

// Program that allocates an array of ints of certain size,
// and then proceeeds to update each int in a loop, forever.
int main(int argc, char *argv[]) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "usage: tlb <num pages> <num trials>\n");
    exit(1);
  }
  int silent = 0;
  if (argc > 3) {
    if (strcmp(argv[3], "-s") == 0) {
      silent = 1;
    } else {
      fprintf(stderr, "Unknown option\n");
      exit(1);
    }
  }
  long long int num_pages = (long long int)atoi(argv[1]);
  long long int size_in_bytes = num_pages * PAGESIZE; // * 4kb

  int num_trials = atoi(argv[2]);

  if (!silent)
    printf("allocating %lld bytes (%.2f MB)\n", size_in_bytes,
           size_in_bytes / (1024 * 1024.0));

  // the big memory allocation happens here
  ARR_TYPE *x = malloc(size_in_bytes);
  if (x == NULL) {
    fprintf(stderr, "memory allocation failed\n");
    exit(1);
  }
  for (unsigned long long i = 0; i < size_in_bytes / sizeof(ARR_TYPE); i++) {
    x[i] = 0; // We zero out the array to make sure that all memory is actually
              // allocated.
  }

  long long int num_ints = size_in_bytes / sizeof(ARR_TYPE);
  if (!silent)
    printf("  number of chars in array: %lld\n", num_ints);

  // now the main loop: each time through, touch each integer
  // (and increment its value by 1).

  int jump = PAGESIZE / sizeof(ARR_TYPE);
  int num_jumps = num_pages * jump;
  double t = Time_GetMicroSeconds();
  for (int trial = 0; trial < num_trials; trial++) {
    int ind = trial % num_ints;
    for (int i = 0; i < num_jumps; i += 1) {
      x[ind] += trial;
      ind = (ind + jump) % num_ints;
    }
  }
  double run = Time_GetMicroSeconds() - t;

  unsigned hash = 0;
  for (int ind = 0; ind < num_ints;) {
    hash += x[ind++];
  }
  if (!silent) {
    printf("%f seconds minimum to read through %lld pages.\n\tHash = "
           "%d\n\tThats %f microseconds "
           "per access.\n",
           (run / num_trials) * 1e6, num_pages, hash,
           run / num_jumps / num_trials);
  } else
    printf("%f\n", run / num_jumps / num_trials);
  return 0;
}
