#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadTask
{
public:
	virtual void run() = 0;
};

class ThreadPool
{
public:

	ThreadPool()
	{
		for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
		{
			m_threads.emplace_back();
		}
	}

	ThreadPool(unsigned int numThreads)
	{
		for (unsigned int i = 0; i < numThreads; i++)
		{
			m_threads.emplace_back();
		}
	}

	~ThreadPool()
	{

	}

private:
	ThreadTask* getNextTask()
	{
		ThreadTask *task = nullptr;
		
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);

			if (!done)
			{
				if (m_taskQueue.size() == 0)
				{
					m_condVariable.wait(lock);
				}
			}
		}

		return task;
	}

private:
	std::queue<ThreadTask*> m_taskQueue;
	std::vector<std::thread> m_threads;

	bool done = false;
	std::mutex m_queueMutex;
	std::condition_variable m_condVariable;
};

#endif