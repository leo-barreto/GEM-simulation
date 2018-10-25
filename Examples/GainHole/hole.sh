#!/bin/bash

jobs=20

for i in `seq 20 10 130`
do
  for j in `seq 1 $jobs`
  do
  NAME=d$i#$j
  qsub -q griper -N $NAME -o outputs -e erros -V /GainHole/hole$i.sh -l walltime=36:00:00
  done
done
