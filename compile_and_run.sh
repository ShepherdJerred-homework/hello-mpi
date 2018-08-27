#!/bin/bash
file="hello_name"

mpicc ${file}.c -o ${file}
mpirun -np 64 -hostfile ~/hostfile ${file}
