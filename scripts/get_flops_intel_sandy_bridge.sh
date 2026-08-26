#!/bin/bash

prog_with_args=$1

# dump perf out to tmp file 
perf stat -M GFLOPS ${prog_with_args} 2> tmp.prof

gflops=`cat tmp.prof | grep "scalar_single" | awk '{print $4}'`

# print the result
echo "Performance = $gflops GFLOPS/s"

