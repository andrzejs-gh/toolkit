#ifndef MTLK_TIME_EXECUTION_H
#define MTLK_TIME_EXECUTION_H

#include <time.h>
#include <stdio.h>

#define GET_EXEC_TIME(func_or_code) do { \
                                        struct timespec start_ts, end_ts; \
                                        clock_gettime(CLOCK_MONOTONIC, &start_ts); \
                                        func_or_code;  \
                                        clock_gettime(CLOCK_MONOTONIC, &end_ts); \
                                        double start = start_ts.tv_sec + start_ts.tv_nsec * 1e-9; \
                                        double end = end_ts.tv_sec + end_ts.tv_nsec * 1e-9;

#define TIME_PRINT(func_or_code) GET_EXEC_TIME(func_or_code) \
                                 printf("t = %.9f s\n", end - start); \
                                 } while (0);

#define TIME_STORE(func_or_code, variable) GET_EXEC_TIME(func_or_code) \
                                           variable = end - start; \
                                           } while (0);

#endif
