#!/bin/bash

prog_with_args=$1



# dump perf out to tmp file 
perf stat -e r538010 ${prog_with_args} 2> tmp.prof

# extract flop count
flops=`cat tmp.prof | grep r538010  | awk '{printf $1}' | sed 's/,//g'`
flops=`echo $flops | awk '{print $1 / 2.5}'`


# extract number of seconds 
secs=`cat tmp.prof | grep "elapsed"  | awk '{print $1}'`

# calculate FLOPS
FLOPS=`echo $flops $secs | awk '{printf "%3.2f", ($1/1e+09)/$2}'`

# print the result
echo "Performance = $FLOPS GFLOPS/s"
