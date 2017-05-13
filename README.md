## NUMA Test Suite

This suite contains a set of simple programs to test numa interfaces provided by libnuma.

## Prerequisites

Make sure you have `numactl`, if you don't, do

`sudo yum install numactl`

`sudo yum install numactl-devel`

or use other package package management tool for your platform.

## Check NUMA Support

To check if your machine has NUMA, do

`numactl --hardware`

The first line shows the number of NUMA nodes in the system. It needs to be more than one node.

