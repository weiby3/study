/*
 * 多生产者、多消费者源码
 */
#include <vector>
#include <iostream>
#include <cstdio>
#include <numeric>
#include <boost/circular_buffer.hpp>
#include <boost/progress.hpp>
#include <omp.h>

#define MAX_BUFFER_SIZE 100
#define MAX_THREADS_COUNT 8
#define PRODUCER_MAX_KEY 10000
#define CONSUMER_COUNT 4
#define PRODUCER_COUNT 4
#define THREADS_COUNT (CONSUMER_COUNT + PRODUCER_COUNT)

using namespace std;

boost::circular_buffer<int> cb(MAX_BUFFER_SIZE);
omp_lock_t lock;
int sum_array[CONSUMER_COUNT];

void consumer(int idx);
void producer();

int main()
{
    boost::progress_timer progress;
    omp_init_lock(&lock);
    //#pragma omp parallel for num_threads(4)
    //for (auto i = 0; i < 4; i++)
    //    printf("ID: %d, Max threads: %d, Num threads: %d \n", omp_get_thread_num(), omp_get_max_threads(), omp_get_num_threads());
    #pragma omp parallel for num_threads(MAX_THREADS_COUNT)
    for (auto i = 0; i < THREADS_COUNT; i++)
        if (i < CONSUMER_COUNT)
            consumer(i);
        else
            producer();
    omp_destroy_lock(&lock);
    printf("All sum actually is %d, expect is %d.", accumulate(sum_array, sum_array + CONSUMER_COUNT, 0), PRODUCER_MAX_KEY / 2 * (PRODUCER_MAX_KEY - 1) * PRODUCER_COUNT);
    cout << "All " << omp_get_num_threads() << " threads use ";
}

void consumer(int idx)
{
    boost::progress_timer progress;
    printf("Consumer %d prepare to consume items.\n", omp_get_thread_num());
    int sum = 0;
    for (auto i = 0; i != PRODUCER_MAX_KEY * PRODUCER_COUNT / CONSUMER_COUNT; i++)
    {
        omp_set_lock(&lock);
        if (!cb.empty())
        {
            sum += cb.front();
            cb.pop_front();
        } else
        {
            //printf("Consumer %d hit empty circular-buffer.\n", omp_get_thread_num());
            i--;
        }
        omp_unset_lock(&lock);
    }
    sum_array[idx] = sum;
    printf("Consumer %d consumed %d items, local sum is %d, use ", omp_get_thread_num(), PRODUCER_MAX_KEY * PRODUCER_COUNT / CONSUMER_COUNT, sum);
}

void producer()
{
    boost::progress_timer progress;
    printf("Producer %d prepare to produce items.\n", omp_get_thread_num());
    for (auto i = 0; i != PRODUCER_MAX_KEY; i++)
    {
        omp_set_lock(&lock);
        if (!cb.full())
        {
            cb.push_back(i);
        } else
        {
            //printf("Producer %d hit full circular-buffer.\n", omp_get_thread_num());
            i--;
        }
        omp_unset_lock(&lock);
    }
    printf("Producer %d produced %d items, use ", omp_get_thread_num(), PRODUCER_MAX_KEY);
}