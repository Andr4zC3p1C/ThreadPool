/*
MIT License

Copyright (c) 2016 Andraž Čepič

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <stdio.h>

// A mutex protected printf wrapper function for debugging
std::mutex console_mutex;
template<typename... T>
void tp_printf(const char* str, T&&... args)
{
	console_mutex.lock();
	printf(str, args...);
	console_mutex.unlock();
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
	ThreadPool(unsigned int numThreads = std::thread::hardware_concurrency()) :
	    m_numThreads(numThreads)
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
					delete task;
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

	/*
		Pushes the new task to the end of the queue and notifies 1 thread about it if there is any
	*/
	void pushTask(ThreadTask *newTask)
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

	unsigned int& getNumThreads()
	{
        return m_numThreads;
	}

private:
	std::queue<ThreadTask*> m_taskQueue;
	std::vector<std::thread> m_threads;

	unsigned int m_numThreads;
	bool m_done = false;
	std::mutex m_queueMutex;
	std::condition_variable m_condVariable;
};

#endif
