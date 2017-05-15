#include <numa.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <sys/time.h>
#include <iostream>

#include "optParser.h"

/* definition of bitmask */
// struct bitmask {
// 	unsigned long size; /* number of bits in the map */
// 	unsigned long *maskp;
// };


/* config */
int cpu_node = 1;
char* char_arr;
long long arr_size = 20 * 1024 * 1024;

void* malloc_array(int size) {
  char_arr = (char*)malloc(size);
  if (char_arr == NULL) {
    fprintf(stderr, "malloc failed");
    exit(0);
  }
  return char_arr;
}

void* numa_alloc_array(int size, int node) {
  long long node_size = numa_node_size64(node, NULL);
  assert(arr_size < node_size && "requested array too big for the NUMA node");
  char_arr = (char*)numa_alloc_onnode(arr_size, node);
  if (char_arr == NULL) {
    fprintf(stderr, "numa_alloc_onnode failed");
    exit(0);
  }
  return char_arr;
}

void* numa_alloc_array_interleaved(int size) {
  char_arr = (char*)numa_alloc_interleaved(arr_size);
  if (char_arr == NULL) {
    fprintf(stderr, "numa_alloc_interleaved failed");
    exit(0);
  }
  return char_arr;
}

void sequential_access(int mode) {
  printf("  sequential access test\n");
  struct timeval tv_start, tv_end, tv_delta;  
  gettimeofday(&tv_start, NULL);
  srand(time(NULL));

  for (long long i = 0; i < arr_size; i++) {
    if (mode == 0) {
      int dummy = char_arr[i];
    }
    else if (mode == 1) {
      char_arr[i] = 0;
    }
    else if (mode == 2) {
      ++char_arr[i];
    }
  }    
  
  gettimeofday(&tv_end, NULL);
  timersub(&tv_end, &tv_start, &tv_delta);
  printf("    elapsed: %lld.%06ld\n", (long long int)tv_delta.tv_sec, (long int)tv_delta.tv_usec);
}

void random_access(int mode) {
  printf("  random access test\n");
  struct timeval tv_start, tv_end, tv_delta;  
  gettimeofday(&tv_start, NULL);
  srand(time(NULL));
  for (long long i = 0; i < arr_size; i++) {
    int j = rand() % arr_size;
    char_arr[j] = 0;

    if (mode == 0) {
      int dummy = char_arr[j];
    }
    else if (mode == 1) {
      char_arr[j] = 0;
    }
    else if (mode == 2) {
      ++char_arr[j];
    }
  }    
  
  gettimeofday(&tv_end, NULL);
  timersub(&tv_end, &tv_start, &tv_delta);
  printf("    elapsed: %lld.%06ld\n", (long long int)tv_delta.tv_sec, (long int)tv_delta.tv_usec);
}

// void test_numa(int acc_mode) {
//   int node = 0;
//   int ret = numa_run_on_node(node); // runs the current task and its children on node 0
//   if (ret != 0) {
//     fprintf(stderr, "numa_run_on_node failed");
//     exit(0);
//   }

//   int M = 1024 * 1024;
//   long long freep;
//   for (int i = 0; i < numa_num_configured_nodes(); i++) {
//     long long node_size = numa_node_size64(i, &freep);
//     assert(arr_size < node_size && "requested array too big for the NUMA node");
//     //long long arr_size = node_size/10;
//     char* arr = (char*)numa_alloc_onnode(arr_size, i);
//     if (arr == NULL) {
//       fprintf(stderr, "numa_alloc_onnode failed");
//       exit(0);
//     }

//     struct timeval tv_start, tv_end, tv_delta;
  
//     gettimeofday(&tv_start, NULL);

//     srand (time(NULL));
//     for (long long i = 0; i < arr_size; i++) {
//       switch (acc_mode) {
//       case 0: {
//         arr[i] = 0;
//       }
//       case 1: {
//         int j = rand() % arr_size;
//         arr[j] = 'a';
//       }
//       }
      
//     }    
  
//     gettimeofday(&tv_end, NULL);
//     timersub(&tv_end, &tv_start, &tv_delta);
//     printf("Time elapsed: %lld.%06ld\n", (long long int)tv_delta.tv_sec, (long int)tv_delta.tv_usec);

//     numa_free(arr, arr_size);

//   }
  
// }

void test_malloc() {
  printf("allocate by malloc\n");
  malloc_array(arr_size);
  sequential_access(2);
  random_access(2);
}

void test_remote() {  
  for (int i = 0; i < numa_num_configured_nodes(); i++) {
    if (i == cpu_node) {
      printf("allocate on local node %d\n", i);
    }
    else {
      printf("allocate on remote node %d\n", i);
    }
    
    numa_alloc_array(arr_size, i);
    sequential_access(2);
    random_access(2);
  }
}

void test_interleaved() {
  printf("allocate interleavely on all nodes\n");
  numa_alloc_array_interleaved(arr_size);
  sequential_access(2);
  random_access(2);
}

void bind_cpu(int node) {
  int ret = numa_run_on_node(node); // runs the current task and its children on node 0
  if (ret != 0) {
    fprintf(stderr, "numa_run_on_node failed\n");
    exit(0);
  }
  printf("bind the current task to run on node %d\n", node);
}

void parse_args(int argc, char** argv) {
  GetOpt parser(argc, argv);
  parser.add_doc("Usage: ./a.out [option]");
  parser.add_doc("Description: bind the process to one cpu node, and test accessing array from the local node, remote node or interleavely.");
  parser.add_option("cpu-node", 'c', "specify which cpu node to run on", required_argument, &cpu_node, OptionArgType::INT);
  parser.add_option("array-size", 's', "specify the size of the array (in Mb)", required_argument, &arr_size, OptionArgType::INT);
  parser.parse();
}

int main(int argc, char** argv) {
  parse_args(argc, argv);
  bind_cpu(cpu_node);
  test_malloc();
  test_remote();
  test_interleaved();
  return 0;
}
