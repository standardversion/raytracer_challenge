#pragma once
#include <atomic>
#include <vector>
#include <thread>
#include <functional>
#include <future>
#include "threadsafe_queue.h"
#include "join_threads.h"

/**
 * @class ThreadPool
 * @brief A simple thread pool that runs tasks on a fixed set of worker threads.
 *
 * Tasks are submitted using std::packaged_task<void()> and processed by a background thread pool.
 * Each worker thread waits for a task to become available and then executes it.
 */
class ThreadPool
{
public:
    /**
     * @brief Constructs the thread pool and launches the worker threads.
     *
     * The number of threads is based on the hardware concurrency.
     */
    ThreadPool();

    /**
     * @brief Destructor that stops all threads and joins them.
     */
    ~ThreadPool();

    /**
     * @brief Submit a task to be executed by the thread pool.
     *
     * @param task A std::packaged_task representing a void-returning callable.
     * Ownership of the task is transferred to the pool.
     */
    void submit(std::packaged_task<void()> task);

private:
    /**
     * @brief Flag to signal the thread pool to stop processing tasks.
     */
    std::atomic_bool done;

    /**
     * @brief Thread-safe queue that holds tasks to be executed.
     */
    ThreadsafeQueue<std::packaged_task<void()>> work_queue;

    /**
     * @brief Container for the worker threads.
     */
    std::vector<std::thread> threads;

    /**
     * @brief Helper object to join all threads upon destruction.
     */
    JoinThreads joiner;

    /**
     * @brief Function executed by each worker thread to process tasks.
     */
    void worker_thread();
};

