#include <iostream>
#include "ThreadPool.h"

namespace cppTools {

ThreadPool::ThreadPool(size_t thread_number):running_(true) {
	for(size_t i = 0; i < thread_number; i++) {
		// create threads
		threads_.push_back(std::thread(std::bind(&ThreadPool::thread_process, this)));
	}
	std::this_thread::sleep_for (std::chrono::seconds(1));
	std::cout<<"ThreadPool is running..."<<std::endl;
}
ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> lock(queue_mutex_);
		running_ = false;
	}
	condition_.notify_all();
	std::cout<<"join"<<std::endl;
	for(auto &t:threads_) {
		t.join();
	}
}

void ThreadPool::thread_process() {
	while(this->running_) {
		std::cout<<"thread "<<std::this_thread::get_id()<<" is running"<<std::endl;
		std::cout<<"queue size:"<<this->tasks_.size()<<std::endl;
		Task task;
		{
			std::unique_lock<std::mutex> lock{ this->queue_mutex_ };
			// wait
			this->condition_.wait(lock, 
					[this]{
					return !this->running_ || !this->tasks_.empty();
					}
					);
			if (!this->running_ && this->tasks_.empty()) {
				return;
			}
			task = std::move(this->tasks_.front());
			this->tasks_.pop();
		}
		task();
	}
}

}
