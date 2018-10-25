#!/bin/bash

jobs=20

for i in `seq 300 50 500`
do
  for j in `seq 1 $jobs`
  do
  NAME=t$i#$j
  qsub -q griper -N $NAME -o outputs -e erros -V pot$i.sh -l walltime=36:00:00
  done
done
