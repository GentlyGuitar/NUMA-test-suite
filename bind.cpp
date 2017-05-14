#include <numa.h>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include <iostream>

/* definition of bitmask */
// struct bitmask {
// 	unsigned long size; /* number of bits in the map */
// 	unsigned long *maskp;
// };

void test_bind() {
  int node = 0;
  int ret = numa_run_on_node(node); // runs the current task and its children on node 0
  if (ret != 0) {
    fprintf(stderr, "numa_run_on_node failed");
    exit(0);
  }

  int M = 1024 * 1024;
  long long freep;
  for (int i = 0; i < numa_num_configured_nodes(); i++) {
    long long node_size = numa_node_size64(i, &freep);
    long long arr_size = node_size/1000;
    char* arr = (char*)numa_alloc_onnode(arr_size, i);
    if (arr == NULL) {
      fprintf(stderr, "numa_alloc_onnode failed");
      exit(0);
    }

    struct timeval tv_start, tv_end, tv_delta;
  
    gettimeofday(&tv_start, NULL);

    srand (time(NULL));
    for (long long i = 0; i < arr_size; i++) {
      //arr[i] = 0;
      int j = rand() % arr_size;
      arr[j] = 'a';
    }

    
    
  
    gettimeofday(&tv_end, NULL);
    timersub(&tv_end, &tv_start, &tv_delta);
    printf("Time elapsed: %lld.%06ld\n", (long long int)tv_delta.tv_sec, (long int)tv_delta.tv_usec);

    numa_free(arr, arr_size);

  }
  
}

int main() {
  test_bind();
  return 0;
}
