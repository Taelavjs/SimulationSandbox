#ifndef THREADPOOL
#define THREADPOOL

#include <condition_variable> 
#include <functional> 
#include <iostream> 
#include <mutex> 
#include <queue> 
#include <thread> 
#include <future>  // Include for std::future and std::packaged_task
using namespace std; 
  
class ThreadPool { 
public: 
    ThreadPool(size_t num_threads = std::thread::hardware_concurrency()) 
    { 
        for (size_t i = 0; i < num_threads; ++i) { 
            threads_.emplace_back([this] { 
                while (true) { 
                    function<void()> task; 
                    { 
                        unique_lock<mutex> lock(queue_mutex_); 
                        cv_.wait(lock, [this] { return !tasks_.empty() || stop_; }); 
                        if (stop_ && tasks_.empty()) { 
                            return; 
                        } 
                        task = move(tasks_.front()); 
                        tasks_.pop(); 
                    } 
                    task(); 
                } 
            }); 
        } 
    } 
  
    ~ThreadPool() 
    { 
        { 
            unique_lock<mutex> lock(queue_mutex_); 
            stop_ = true; 
        } 
        cv_.notify_all(); 
        for (auto& thread : threads_) { 
            thread.join(); 
        } 
    } 
  
    template<class F>
    std::future<void> enqueue(F&& f) 
    { 
        auto task = std::make_shared<std::packaged_task<void()>>(std::forward<F>(f)); 
        std::future<void> res = task->get_future(); 
        { 
            unique_lock<std::mutex> lock(queue_mutex_); 
            if (stop_) 
                throw std::runtime_error("enqueue on stopped ThreadPool"); 
            tasks_.emplace([task]() { (*task)(); }); 
        } 
        cv_.notify_one(); 
        return res; 
    } 
  
private: 
    vector<std::thread> threads_; 
    queue<std::function<void()>> tasks_; 
    mutex queue_mutex_; 
    condition_variable cv_; 
    bool stop_ = false; 
}; 

#endif /* THREADPOOL */
