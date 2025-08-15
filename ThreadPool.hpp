// C++ Program to demonstrate thread pooling
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;

/*
	https://www.geeksforgeeks.org/cpp/thread-pool-in-cpp/
	Creadit to GeeksForGeeks
	This is an edited version of their threadpool
*/

// Class that represents a simple thread pool
class ThreadPool {
public:
	ThreadPool(size_t num_threads = thread::hardware_concurrency()) {
		for (size_t i = 0; i < num_threads; ++i) {
			threads_.emplace_back([this] {
				while (true) {
					function<void()> task;
					{
						unique_lock<mutex> lock(queue_mutex_);
						cv_.wait(lock, [this] {
							return !tasks_.empty() || stop_;
							});
						if (stop_ && tasks_.empty()) {
							return;
						}
						task = move(tasks_.front());
						tasks_.pop();
					}

					task();

					// Task finished, decrement the counter and notify waiting threads
					{
						unique_lock<mutex> lock(counter_mutex_);
						task_counter_--;
					}
					task_cv_.notify_all();
				}
				});
		}
	}

	~ThreadPool() {
		{
			unique_lock<mutex> lock(queue_mutex_);
			stop_ = true;
		}

		cv_.notify_all();

		for (auto& thread : threads_) {
			thread.join();
		}
	}

	void enqueue(function<void()> task) {
		{
			unique_lock<std::mutex> lock(queue_mutex_);
			tasks_.emplace(move(task));

			// Increment task counter
			unique_lock<mutex> counter_lock(counter_mutex_);
			task_counter_++;
		}
		cv_.notify_one();
	}

	// New public method to wait for all tasks to complete
	void wait_for_all_tasks() {
		unique_lock<mutex> lock(counter_mutex_);
		task_cv_.wait(lock, [this] {
			return task_counter_ == 0;
			});
	}

private:
	vector<thread> threads_;
	queue<function<void()>> tasks_;
	mutex queue_mutex_;
	condition_variable cv_;
	bool stop_ = false;

	// New member variables for task completion tracking
	int task_counter_ = 0;
	mutex counter_mutex_;
	condition_variable task_cv_;
};
