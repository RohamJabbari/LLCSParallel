# LLCSParallel
Length of the Longest Common Subsequence using OpenMP

This code is written in C. 

Four different implementations of LLCS function. (located in implementation.h)

X.in , Y.in: two large input strings

Serial version: normal implementation.

Anti-diagonal version: a different serial approach

Explicit tasks: parallel LLCS using OpenMp with Explicit tasks

Taskloop: parallel LLCS using OpenMp with Taskloop

To compile and run the code do:

make CC=/opt/global/gcc-11.2.0/bin/gcc

OMP_NUM_THREADS=16 srun --nodes=1 ./llcs-parallel-<version>
  
(change OMP_NUM_THREADS for different thread count)
