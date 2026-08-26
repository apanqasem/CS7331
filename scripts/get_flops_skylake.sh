#!/bin/bash


#prog_with_args=$1

#time ibrun perf stat -M tma_info_system_gflops ${prog_with_args} 2>> tmp.prof


flops=`cat tmp.prof | grep "FP_ARITH_INST_RETIRED.SCALAR" | awk '{print $1}'`
secs=`cat tmp.prof | grep "elapsed" | awk '{print $1}'`
secs=`echo $secs | awk '{print $1}'` 

flops=`echo $flops | sed 's/,//g'`
flops_total=0
for i in $flops; do
    flops_total=`echo $i | awk -v flops_total_awk=${flops_total} '{print $1 + flops_total_awk}'` 
done

echo ${flops_total}
echo ${secs}

FLOPS=`echo ${flops_total} $secs | awk '{printf "%3.2f", ($1/1e+09)/$2}'`

echo "Performance = $FLOPS GFLOPS/s"
