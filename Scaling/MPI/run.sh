#!/bin/sh
# run mpi_pi.c with np from 1 to 8

mpicc -o pi mpi_pi.c

#for i in $(seq 1 8)
for ((i=1; i<=16; i=i*2))
do
mpirun -np $i ./pi $1
done
