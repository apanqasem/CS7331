#!/bin/bash


mpi_world=$1
prog_with_args=$2

ibrun perf stat -M tma_info_system_gflops ${prog_with_args} 2> tmp.prof


flops_scalar=`cat tmp.prof | grep -m 1 "FP_ARITH_INST_RETIRED.SCALAR" | awk '{print $1}'`
flops_packed=`cat tmp.prof | grep -m 1 "FP_ARITH_INST_RETIRED.128B_PACKED_DOUBLE" | awk '{print $1}'`

#echo ${flops_scalar}
#echo ${flops_packed}

flops_scalar=`echo ${flops_scalar} | sed 's/,//g'`
flops_packed=`echo ${flops_packed} | sed 's/,//g'`


flops_scalar=`echo ${flops_scalar} ${mpi_world} | awk -v mpi_tasks=${mpi_world} '{print $1 * mpi_tasks}'`
flops_packed=`echo ${flops_packed} ${mpi_world} | awk -v mpi_tasks=${mpi_world} '{print $1 * 2 * mpi_tasks}'`


flops_total=`echo ${flops_scalar} ${flops_packed} | awk  '{print $1 + $2}'`

secs=`cat tmp.prof | grep -m 1 "elapsed" | awk '{print $1}'`
secs=`echo $secs | awk '{print $1}'` 
secs_ms=`echo $secs | awk '{printf "%3.2f", ($1 * 1000)}'` 

FLOPS=`echo ${flops_total} $secs | awk '{printf "%3.2f", ($1/1e+09)/$2}'`

echo "Total FLOP count = ${flops_total}" 
echo "Execution time = ${secs_ms} (ms)"
echo "Performance = $FLOPS GFLOPS/s"

