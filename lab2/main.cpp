#include <vector>
#include <iostream>
#include <chrono>
#include <random>

#include "ThreadPool.h"
#include "TaskQueue.h"

const int THREAD_COUNT = 4;
const int TASK_MIN_DURATION = 4;
const int TASK_MAX_DURATION = 10;
const int QUEUE_INTERVAL = 45;
const int INITIAL_NUMBER_OF_TASKS = 5;
int task_counter = 0;
long long overall_task_time = 0;

void execute_task() {
    std::random_device rd;
    std::mt19937 rng(rd());
    int time = 4000 + rng() % 6001;
    std::this_thread::sleep_for(std::chrono::milliseconds(time));

    std::cout << "Task " << task_counter++ << " took " << time << " milliseconds" << '\n';
    overall_task_time += time;    
}

void add_task_to_pool(threadPool& pool, std::function<void()> task) {
    pool.add_task(task);

    std::this_thread::sleep_for(std::chrono::seconds(48));
    pool.add_task(task);
}

void terminate_threads(threadPool& pool)
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "terminate called" << '\n';
    pool.terminate_immidiately();
}

int main()
{
    threadPool pool;
    pool.initilize(THREAD_COUNT);

    std::thread threads[INITIAL_NUMBER_OF_TASKS];
    for (int i = 0; i < INITIAL_NUMBER_OF_TASKS; i++)
    {
        threads[i] = std::thread(add_task_to_pool, std::ref(pool), execute_task);
    }

    for (int i = 0; i < INITIAL_NUMBER_OF_TASKS; i++)
        threads[i].join();

    //std::thread thread = std::thread(terminate_threads, std::ref(pool));

    pool.terminate();

    //thread.join();

    std::cout << "Average task execution time: " << 1.0 * overall_task_time / task_counter << " ms" << '\n';
    std::cout << "Average thread waiting time: " << 1.0 * pool.overallWaitingTime / THREAD_COUNT << " ms" << '\n';
    std::cout << "Overall number of threads: " << INITIAL_NUMBER_OF_TASKS + THREAD_COUNT << '\n';
    return 0;
}