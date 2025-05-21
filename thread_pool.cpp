#include "thread_pool.h"

void ThreadPool::worker_thread()
{
	while (!done)
	{
		std::packaged_task<void()> task;
		if (work_queue.try_pop(task))
		{
			task(); // Execute the task
		}
		else
		{
			std::this_thread::yield();
		}
	}
}

ThreadPool::ThreadPool()
	: done{ false }, threads{}, joiner { threads }
{
	const unsigned thread_count{ std::thread::hardware_concurrency() };
	try
	{
		for (unsigned i{ 0 }; i < thread_count; ++i)
		{
			threads.push_back(std::thread(&ThreadPool::worker_thread, this));
		}
	}
	catch (...)
	{
		done = true;
		throw;
	}
}

ThreadPool::~ThreadPool()
{
	done = true;
}

void ThreadPool::submit(std::packaged_task<void()> task)
{
	work_queue.push(std::move(task));
}