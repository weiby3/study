/*
 * 多生产者、多消费者源码
 */
#include <vector>
#include <iostream>
#include <boost/circular_buffer.hpp>
#include <omp.h>

#define MAX_BUFFER_SIZE 10

using namespace std;

boost::circular_buffer<unsigned long long int> cb(MAX_BUFFER_SIZE);

int main()
{
    for (auto i = 0; i < MAX_BUFFER_SIZE + 1; i++)
    {
        cb.push_back(i);
    }
    cout << cb.full() << endl;
    for (auto i = 0; i < MAX_BUFFER_SIZE; i++)
    {
        cout << cb[i] << endl;
    }
}