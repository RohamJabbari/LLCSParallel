#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define LEN 51200
#define MIN(x, y) (((x) <= (y)) ? (x) : (y))
#define MAX(x, y) (((x) >= (y)) ? (x) : (y))

//SERIAL VERSION NO PARALLEL
unsigned long long llcs_serial(const char *X, const char *Y, unsigned int **M)
{
    unsigned long long entries_visited = 0;
    int i = 0;
    while (i < LEN)
    {
        int j = 0;
        while (j < LEN)
        {
            if (X[i] == Y[j]) {
                M[i + 1][j + 1] = M[i][j] + 1;
            } else if (M[i + 1][j] < M[i][j + 1]) {
                M[i + 1][j + 1] = M[i][j + 1];
            } else {
                M[i + 1][j + 1] = M[i + 1][j];
            }
            entries_visited++;
            j++;
        }
        i++;
    }
    return entries_visited;
}

//ANTI-DIAGONAL VERSION NO PARALLEL
unsigned long long llcs_antidiagonal(const char *X, const char *Y, unsigned int **M)
{
    unsigned long long entries_visited = 0;
    int row, col, diagonal;
    int t = 8192;
    for(diagonal = 2; diagonal <= LEN+LEN; diagonal+=t){
        for(row =  MIN(LEN-t+1, diagonal-1);row >= MAX(1, diagonal-LEN);row-=t)
        {
            col = diagonal - row;
            for (int i = row;i < row + t; i++)
            {
                for (int j = col; j < col + t; j++)
                {
                    if (X[i-1] == Y[j-1]) {
                        M[i][j] = M[i-1][j-1] + 1;
                    } else if (M[i][j-1] < M[i-1][j]) {
                        M[i][j] = M[i-1][j];
                    } else {
                        M[i][j] = M[i][j-1];
                    }
                    entries_visited++;
                }
            }
        }
    }
    return entries_visited;
}

#if defined(_OPENMP)

//EXPLICIT TASK VERSION WITH BEST GRANULARITY
unsigned long long llcs_parallel_tasks(const char *X, const char *Y, unsigned int **M)
{
    unsigned long long entries_visited = 0;
    int row, col, diagonal;
    int t =  512;
    
#pragma omp parallel default(shared)
    {
#pragma omp single
        {
            for(diagonal = 2; diagonal <= LEN+LEN; diagonal+=t){
                for(row =  MIN(LEN-t+1, diagonal-1);row >= MAX(1, diagonal-LEN);row-=t)
                {
                    col = diagonal - row;
                    int temp = 0;
#pragma omp task default(none) \
shared(M, entries_visited ) \
firstprivate(X, Y, temp, t, diagonal, row, col) \
depend(in:M[row+t-1][col-1], M[row-1][col+t-1]) \
depend(out:M[row+t-1][col+t-1])
                    {
                        for (int i = row;i < row + t; i++)
                        {
                            for (int j = col; j < col + t; j++)
                            {
                                if (X[i-1] == Y[j-1]) {
                                    M[i][j] = M[i-1][j-1] + 1;
                                } else if (M[i][j-1] < M[i-1][j]) {
                                    M[i][j] = M[i-1][j];
                                } else {
                                    M[i][j] = M[i][j-1];
                                }
                                temp++;
                            }
                        }
#pragma omp critical
                        {
                            entries_visited += temp;
                        }
                    }
                }
            }
        }
    }
    return entries_visited;
}

//EXPLICIT TASK VERSION WITH SECOND BEST GRANULARITY
unsigned long long llcs_parallel_tasks_v2(const char *X, const char *Y, unsigned int **M)
{
    unsigned long long entries_visited = 0;
    int row, col, diagonal;
    int t =  1024;
    
#pragma omp parallel default(shared)
    {
#pragma omp single
        {
            for(diagonal = 2; diagonal <= LEN+LEN; diagonal+=t){
                for(row =  MIN(LEN-t+1, diagonal-1);row >= MAX(1, diagonal-LEN);row-=t)
                {
                    unsigned int **tempM = M;
                    col = diagonal - row;
                    int temp = 0;
#pragma omp task default(none) \
shared(M, entries_visited ) \
firstprivate(X, Y, temp, t, diagonal, row, col) \
depend(in:M[row+t-1][col-1], M[row-1][col+t-1]) \
depend(out:M[row+t-1][col+t-1])
                    {
                        for (int i = row;i < row + t; i++)
                        {
                            for (int j = col; j < col + t; j++)
                            {
                                if (X[i-1] == Y[j-1]) {
                                    M[i][j] = M[i-1][j-1] + 1;
                                } else if (M[i][j-1] < M[i-1][j]) {
                                    M[i][j] = M[i-1][j];
                                } else {
                                    M[i][j] = M[i][j-1];
                                }
                                temp++;
                            }
                        }
#pragma omp critical
                        {
                            entries_visited += temp;
                        }
                    }
                }
            }
        }
    }
    return entries_visited;
}

//EXPLICIT TASK VERSION WITH ANOTHER GRANULARITY
unsigned long long llcs_parallel_tasks_v3(const char *X, const char *Y, unsigned int **M)
{
    unsigned long long entries_visited = 0;
    int row, col, diagonal;
    int t =  64;
    
#pragma omp parallel default(shared)
    {
#pragma omp single
        {
            for(diagonal = 2; diagonal <= LEN+LEN; diagonal+=t){
                for(row =  MIN(LEN-t+1, diagonal-1);row >= MAX(1, diagonal-LEN);row-=t)
                {
                    unsigned int **tempM = M;
                    col = diagonal - row;
                    int temp = 0;
#pragma omp task default(none) \
shared(M, entries_visited ) \
firstprivate(X, Y, temp, t, diagonal, row, col) \
depend(in:M[row+t-1][col-1], M[row-1][col+t-1]) \
depend(out:M[row+t-1][col+t-1])
                    {
                        for (int i = row;i < row + t; i++)
                        {
                            for (int j = col; j < col + t; j++)
                            {
                                if (X[i-1] == Y[j-1]) {
                                    M[i][j] = M[i-1][j-1] + 1;
                                } else if (M[i][j-1] < M[i-1][j]) {
                                    M[i][j] = M[i-1][j];
                                } else {
                                    M[i][j] = M[i][j-1];
                                }
                                temp++;
                            }
                        }
#pragma omp critical
                        {
                            entries_visited += temp;
                        }
                    }
                }
            }
        }
    }
    return entries_visited;
}

//TASKLOOP VERSION WITH BEST GRANULARITY
unsigned long long llcs_parallel_taskloop(const char *X, const char *Y, unsigned int **M)
{
    unsigned long long entries_visited = 0;
    
    int row, col, diagonal;
    int t = 32;
    #pragma omp parallel default(shared)
    {
#pragma omp single
        {
            for(diagonal = 2; diagonal <= LEN+LEN; diagonal+=t){
#pragma omp taskloop \
shared(M) \
firstprivate(X, Y, t, diagonal) \
private(row, col) \
reduction(+:entries_visited) \
grainsize(1)
                for(row =  MIN(LEN-t+1, diagonal-1);row >= MAX(1, diagonal-LEN);row-=t)
                {
                    col = diagonal - row;
                    for (int i = row;i < row + t; i++)
                    {
                        for (int j = col; j < col + t; j++)
                        {
                            if (X[i-1] == Y[j-1]) {
                                M[i][j] = M[i-1][j-1] + 1;
                            } else if (M[i][j-1] < M[i-1][j]) {
                                M[i][j] = M[i-1][j];
                            } else {
                                M[i][j] = M[i][j-1];
                            }
                            entries_visited++;
                        }
                    }
                }
            }
        }
    }
    return entries_visited;
}

//TASKLOOP VERSION WITH SECOND BEST GRANULARITY
unsigned long long llcs_parallel_taskloop_v2(const char *X, const char *Y, unsigned int **M)
{
    unsigned long long entries_visited = 0;
    int row, col, diagonal;
    int t = 256;
#pragma omp parallel default(shared)
    {
#pragma omp single
        {
            for(diagonal = 2; diagonal <= LEN+LEN; diagonal+=t){
#pragma omp taskloop \
shared(M) \
firstprivate(X, Y, t, diagonal) \
private(row, col) \
reduction(+:entries_visited) \
grainsize(1)
                for(row =  MIN(LEN-t+1, diagonal-1);row >= MAX(1, diagonal-LEN);row-=t)
                {
                    col = diagonal - row;
                    for (int i = row;i < row + t; i++)
                    {
                        for (int j = col; j < col + t; j++)
                        {
                            if (X[i-1] == Y[j-1]) {
                                M[i][j] = M[i-1][j-1] + 1;
                            } else if (M[i][j-1] < M[i-1][j]) {
                                M[i][j] = M[i-1][j];
                            } else {
                                M[i][j] = M[i][j-1];
                            }
                            entries_visited++;
                        }
                    }
                }
            }
        }
    }
    return entries_visited;
}

//TASKLOOP VERSION WITH A DIFFERENT GRAINSIZE
unsigned long long llcs_parallel_taskloop_v3(const char *X, const char *Y, unsigned int **M)
{
    unsigned long long entries_visited = 0;
    
    int row, col, diagonal;
    int t = 512;
#pragma omp parallel default(shared)
    {
#pragma omp single
        {
            for(diagonal = 2; diagonal <= LEN+LEN; diagonal+=t){
#pragma omp taskloop \
shared(M) \
firstprivate(X, Y, t, diagonal) \
private(row, col) \
reduction(+:entries_visited) \
grainsize(10)
                for(row =  MIN(LEN-t+1, diagonal-1);row >= MAX(1, diagonal-LEN);row-=t)
                {
                    col = diagonal - row;
                    for (int i = row;i < row + t; i++)
                    {
                        for (int j = col; j < col + t; j++)
                        {
                            if (X[i-1] == Y[j-1]) {
                                M[i][j] = M[i-1][j-1] + 1;
                            } else if (M[i][j-1] < M[i-1][j]) {
                                M[i][j] = M[i-1][j];
                            } else {
                                M[i][j] = M[i][j-1];
                            }
                            entries_visited++;
                        }
                    }
                }
            }
        }
    }
    return entries_visited;
}
#endif
