#ifndef __CPPTOOLS_THREAD_POOL_H__
#define __CPPTOOLS_THREAD_POOL_H__

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <queue>

namespace cppTools{

class ThreadPool {

public:
	typedef std::function<void(void)> Task;
	ThreadPool(size_t thread_number = 4);
	~ThreadPool();
	ThreadPool(ThreadPool&) = delete;
	
	template<class F, class... Args>
	bool append(F&& f, Args&&... args) {
		// add one function to the tasks' queue
		auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		//bind the function and its args together
		{
		std::lock_guard<std::mutex> lock{ queue_mutex_ };
		tasks_.push(task);
		}
		condition_.notify_one();
		return true;
	}
	
private:
	void thread_process();

private:
	std::vector<std::thread> threads_;
	std::queue<Task> tasks_;
	std::mutex queue_mutex_;
	std::condition_variable condition_;
	bool running_;
};

}
#endif

