#pragma once
#include <functional>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <memory>


class TaskSystem
{
private:
	uint8_t maxThreads;
	uint8_t activeThreads;

	std::vector<std::thread> workers;
	bool stopped;

	std::mutex mutex;
	std::condition_variable cv;

	std::queue<std::function<void()>> taskQueue;


	// public and private functions
public:

	TaskSystem(uint8_t toStart = 0) // toStart = worker threads to initialize, leave at 0 for max;
	{
		maxThreads = std::thread::hardware_concurrency();
		workers.reserve(maxThreads);
		stopped = false;

		if (toStart <= maxThreads && toStart > 0) activeThreads = toStart;
		else activeThreads = maxThreads;

		for (uint8_t i = 0; i < activeThreads; i++) workers.emplace_back(&TaskSystem::Worker, this);
	}

	~TaskSystem() // stops all workers threads
	{
		{
			std::unique_lock<std::mutex> lock(mutex);
			stopped = true;
		}

		cv.notify_all();
		for (uint8_t i = 0; i < workers.size(); i++) workers[i].join();
	}

	void AddWorker() // adds a single worker
	{
		if (activeThreads == maxThreads) return;
		workers.emplace_back(&TaskSystem::Worker, this);
		activeThreads++;
	}

	uint8_t MaxThreads() const // returns max thread count
	{
		return maxThreads;
	}

	uint8_t ActiveThreads() const // returns active thread count
	{
		return activeThreads;
	}

	uint8_t WorkerCount() const
	{
		return workers.size();
	}


	template <typename F, typename... Args>
	void AddTask(F&& f, Args&&... args)
	{
		taskQueue.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		cv.notify_one();
	}

private:

	void Worker()
	{
		std::function<void()> task;
		while (true)
		{
			{
				std::unique_lock<std::mutex> lock(mutex);
				cv.wait(lock, [this]() {
					return stopped || !taskQueue.empty();
				});

				if (stopped && taskQueue.empty()) break;
				if (taskQueue.empty()) continue;

				task = taskQueue.front();
				taskQueue.pop();
			}
			task();
		}
	}
};