#!/bin/bash
file="avg_dist"

mpicc ${file}.c -o ${file}
mpirun -np 64 -hostfile ~/hostfile ${file}
