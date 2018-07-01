/*
**  PROGRAM: A simple serial producer/consumer program
**
**  One function generates (i.e. produces) an array of random values.  
**  A second functions consumes that array and sums it.
**
**  HISTORY: Written by Tim Mattson, April 2007.
*/
#include <omp.h>
#ifdef APPLE
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>

#define THREADS_COUNT 8

#define N        10000000

// comment it to not use openmp
#define MY_USE_OMP
// comment it to not print debug information
#define MY_DEBUG

/* Some random number constants from numerical recipies */
#define SEED       2531
#define RAND_MULT  1366
#define RAND_ADD   150889
#define RAND_MOD   714025
int randy = SEED;

/* function to fill an array with random numbers */
void fill_rand(int length, double *a)
{
   int i;
   for (i=0;i<length;i++) {
     randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;
     *(a+i) = ((double) randy)/((double) RAND_MOD);
   }   
}

/* function to sum the elements of an array */
double Sum_array(int length, double *a)
{
   int i;  double sum = 0.0;
   for (i=0;i<length;i++)  sum += *(a+i);  
   return sum; 
}
  
int main()
{
#ifdef MY_USE_OMP
    double *A, sum, runtime;
    int flag = 0, flag_dup = 0;

    A = (double *)malloc(N*sizeof(double));
    omp_set_num_threads(THREADS_COUNT);

    #pragma omp parallel
    {
        #pragma omp master
        {
            runtime = omp_get_wtime();
        }
        #pragma omp barrier

        #pragma omp sections
        {
            #pragma omp section
            {
                #ifdef MY_DEBUG
                    printf("Producer %d producing...\n", omp_get_thread_num());
                #endif
                fill_rand(N, A);
                #pragma omp flush
                flag = 1;
                #pragma omp flush(flag)
            }
            #pragma omp section
            {
                while (1)
                {
                #pragma omp flush (flag)
                #pragma omp atomic read
                    flag_dup = flag;
                    if (flag_dup == 1)
                        break;
                }
                #pragma omp flush
                #ifdef MY_DEBUG
                    printf("Consumer %d consuming...\n", omp_get_thread_num());
                #endif
                sum = Sum_array(N, A);
            }
        }

        #pragma omp master
        {
            runtime = omp_get_wtime() - runtime;
        }
    }

    printf("In %f seconds with %d threads, the sum is %f\n", runtime, THREADS_COUNT, sum);
#else
  double *A, sum, runtime;
  int flag = 0;

  A = (double *)malloc(N*sizeof(double));

  runtime = omp_get_wtime();

  fill_rand(N, A);        // Producer: fill an array of data

  sum = Sum_array(N, A);  // Consumer: sum the array

  runtime = omp_get_wtime() - runtime;

  printf(" In %f seconds, The sum is %f \n",runtime,sum);
#endif
}
 
