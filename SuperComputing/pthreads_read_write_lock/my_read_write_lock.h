#ifndef SUPER_COMPUTING_HOMEWORK_MY_READ_WRITE_LOCK_H
#define SUPER_COMPUTING_HOMEWORK_MY_READ_WRITE_LOCK_H

#include <pthread.h>

class my_read_write_lock_read_prefer
{
public:
    my_read_write_lock_read_prefer()
    {
        pthread_mutex_init(&read_write_mutex, nullptr);
        pthread_cond_init(&read_condition, nullptr);
        pthread_cond_init(&write_condition, nullptr);
        count = 0;
        try_read_count = 0;
        try_write_count = 0;
    }
    ~my_read_write_lock_read_prefer()
    {
        pthread_mutex_destroy(&read_write_mutex);
        pthread_cond_destroy(&read_condition);
        pthread_cond_destroy(&write_condition);
    }
    void read_lock()
    {
        pthread_mutex_lock(&read_write_mutex);
        while (count < 0)
        {
            try_read_count++;
            pthread_cond_wait(&read_condition, &read_write_mutex);
            try_read_count--;
        }
        count++;
        pthread_mutex_unlock(&read_write_mutex);
    }
    void write_lock()
    {
        pthread_mutex_lock(&read_write_mutex);
        while (count != 0)
        {
            try_write_count++;
            pthread_cond_wait(&write_condition, &read_write_mutex);
            try_write_count--;
        }
        count--;
        pthread_mutex_unlock(&read_write_mutex);
    }
    void unlock()
    {
        pthread_mutex_lock(&read_write_mutex);
        if (count == -1)
            count = 0;
        else
            count--;
        if (count == 0)
        {
            if (try_read_count > 0)
                pthread_cond_signal(&read_condition);
            else if (try_write_count > 0)
                pthread_cond_broadcast(&write_condition);
        }
        pthread_mutex_unlock(&read_write_mutex);
    }

private:
    pthread_mutex_t read_write_mutex;
    pthread_cond_t read_condition;
    pthread_cond_t write_condition;
    int count;
    int try_read_count;
    int try_write_count;
};


class my_read_write_lock_write_prefer
{
public:
    my_read_write_lock_write_prefer()
    {
        pthread_mutex_init(&read_write_mutex, nullptr);
        pthread_cond_init(&read_condition, nullptr);
        pthread_cond_init(&write_condition, nullptr);
        count = 0;
        try_read_count = 0;
        try_write_count = 0;
    }
    ~my_read_write_lock_write_prefer()
    {
        pthread_mutex_destroy(&read_write_mutex);
        pthread_cond_destroy(&read_condition);
        pthread_cond_destroy(&write_condition);
    }
    void read_lock()
    {
        pthread_mutex_lock(&read_write_mutex);
        while (count < 0)
        {
            try_read_count++;
            pthread_cond_wait(&read_condition, &read_write_mutex);
            try_read_count--;
        }
        count++;
        pthread_mutex_unlock(&read_write_mutex);
    }
    void write_lock()
    {
        pthread_mutex_lock(&read_write_mutex);
        while (count != 0)
        {
            try_write_count++;
            pthread_cond_wait(&write_condition, &read_write_mutex);
            try_write_count--;
        }
        count--;
        pthread_mutex_unlock(&read_write_mutex);
    }
    void unlock()
    {
        pthread_mutex_lock(&read_write_mutex);
        if (count == -1)
            count = 0;
        else
            count--;
        if (count == 0)
        {
            if (try_write_count > 0)
                pthread_cond_signal(&write_condition);
            else if (try_read_count > 0)
                pthread_cond_broadcast(&read_condition);
        }
        pthread_mutex_unlock(&read_write_mutex);
    }

private:
    pthread_mutex_t read_write_mutex;
    pthread_cond_t read_condition;
    pthread_cond_t write_condition;
    int count;
    int try_read_count;
    int try_write_count;
};

#endif //SUPER_COMPUTING_HOMEWORK_MY_READ_WRITE_LOCK_H
