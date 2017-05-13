#include <numa.h>
#include <cstdio>
#include <iostream>

void show_numa_nodes() {
  int num_possible = numa_num_possible_nodes();
  printf("number of possible nodes: %d\n", num_possible);
  printf("  - the maximum number of nodes that the kernel can handle\n");

  int num_configured = numa_num_configured_nodes();
  printf("number of configured nodes: %d\n", num_configured);
  printf("  - number of memory nodes in the system\n");
  printf("  - derived from the node numbers in /sys/devices/system/node\n");

  int num_cpus = numa_num_configured_cpus();
  printf("number of cpus in the system: %d\n", num_cpus);
  printf("  - includes any cpus that are currently disabled\n");
  printf("  - derived from the cpu numbers in /sys/devices/system/cpu\n");
  // int mask_allowed = numa_get_mems_allowed();
  // printf("the mask of nodes from which the process is allowed to allocate memory in it's current cpuset context: %d\n", mask_allowed);
}

int main() {
  show_numa_nodes();
  return 0;
}
