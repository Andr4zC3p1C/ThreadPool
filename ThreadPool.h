#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <string>
#include <iostream>

// A mutex protected printf function for debugging
std::mutex console_mutex;
template<typename... T>
void tp_printf(const char* str, T... args) {
	std::unique_lock<std::mutex> console_lock(console_mutex);
	printf(str, args...);
}

/*
	An interface for the task that will be run by any thread in the pool
*/
class ThreadTask
{
public:
	virtual void run() = 0;
};

/*
	The thread pool containing the threads and a queue of tasks to be executed by the threads
*/
class ThreadPool
{
public:
	/*
		Creates either as many threads as the user specifies, or as many as there are logical CPU cores if not specified
	*/
	ThreadPool(unsigned int numThreads = std::thread::hardware_concurrency())
	{
		for (unsigned int i = 0; i < numThreads; i++)
		{
			m_threads.emplace_back([this] {
				while (true)
				{
					ThreadTask *task = nullptr;

					{
						std::unique_lock<std::mutex> lock(m_queueMutex);

						m_condVariable.wait(lock, [this] { return m_done || !m_taskQueue.empty(); });

						if (m_done && m_taskQueue.empty())
						{
							return;
						}

						task = m_taskQueue.front();
						m_taskQueue.pop();
					}

					task->run();
				}
			});
		}
	}

	/*
		Notifies all threads that they should stop and joins them 
	*/
	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			m_done = true;
		}
		
		m_condVariable.notify_all();

		for(unsigned int i = 0; i < m_threads.size(); i++)
		{
			m_threads[i].join();
		}
	}

	void addTask(ThreadTask *newTask)
	{
		if(!m_done)
		{
			{
				std::unique_lock<std::mutex> lock(m_queueMutex);
				m_taskQueue.push(newTask);
			}

			m_condVariable.notify_one();
		}
	}

private:
	std::queue<ThreadTask*> m_taskQueue;
	std::vector<std::thread> m_threads;

	bool m_done = false;
	std::mutex m_queueMutex;
	std::condition_variable m_condVariable;
};

#endif