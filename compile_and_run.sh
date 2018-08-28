#!/bin/bash
file="avg_dist"

mpicc ${file}.c -o ${file}
mpirun -np 2 -hostfile ~/hostfile ${file}
