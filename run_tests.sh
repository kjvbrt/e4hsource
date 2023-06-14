#!/bin/bash

TEST="source"
# TEST="low_level"
OUTFILE="res_${TEST}.py"

MEM=()
CPU=()
WALLTIME=()

for I in {1..64}; do
# for I in {1..3}; do
  echo "------------------------------------------------"
  echo "                nThreads = ${I}"
  echo "------------------------------------------------"
  ./build/test_${TEST} ${I}
  /usr/bin/time -v --output=time_stat.txt ./build/test_${TEST} ${I}
  MEM+=("$(grep "Maximum resident set size (kbytes)" < time_stat.txt | cut -d' ' -f6)")
  CPU+=("$(grep "Percent of CPU this job got" < time_stat.txt | cut -d' ' -f7 | tr -d '%')")
  WALLTIME+=("$(grep "Elapsed (wall clock) time" < time_stat.txt | cut -d' ' -f8)")
done

echo "mem_${TEST} = [" > $OUTFILE
for value in "${MEM[@]}"
do
     echo "    ${value}," >> $OUTFILE
done
echo "]" >> $OUTFILE
echo "cpu_${TEST} = [" >> $OUTFILE
for value in "${CPU[@]}"
do
     echo "    ${value}," >> $OUTFILE
done
echo "]" >> $OUTFILE
echo "walltime_${TEST} = [" >> $OUTFILE
for value in "${WALLTIME[@]}"
do
     echo "    \"${value}\"," >> $OUTFILE
done
echo "]" >> $OUTFILE
