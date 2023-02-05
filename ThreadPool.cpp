//
// Created by HONOR on 05.02.2023.
//

#include "ThreadPool.h"

namespace my {
    ThreadPool::ThreadPool(){
        initializeThreads(std::thread::hardware_concurrency());
    }
    ThreadPool::ThreadPool(size_t numberOfThreads){
        initializeThreads(numberOfThreads);
    }

    size_t ThreadPool::getNumberOfThreads(){
        return threads_.size();
    }
    void ThreadPool::doAsync(Task& task){
        std::lock_guard guard(mutex_);
        tasks_.push_back(&task);
        //tasks_[last_++] = &task;
    }
    void ThreadPool::stop(){
        isRunning_ = false;
    }
    ThreadPool::~ThreadPool(){
        isRunning_ = false;
        for(auto& t : threads_){
            t.join();
        }
    }
    void ThreadPool::initializeThreads(size_t numberOfThreads){
        threads_.reserve(numberOfThreads);
        isRunning_ = true;
        for(int i=0; i<numberOfThreads; ++i){
            threads_.emplace_back([this](){
                this->work();
            });
        }
    };

    void ThreadPool::work(){
        while(isRunning_) {
            std::unique_lock guard(mutex_);
            if(!tasks_.empty()) {
                auto task = tasks_.front();
                tasks_.pop_front();
                guard.unlock();

                task->mutex_.lock();
                task->result = task->func();
                task->hasResult = true;
                task->hasResult.notify_one();
                task->mutex_.unlock();
            }
        }
    }
} // my