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
    void ThreadPool::doAsyncOld(Task& task){
        std::lock_guard guard(old_mutex_);
        old_tasks_.push(&task);
    }
    void ThreadPool::stop(){
        isRunning_.store(false);
        hasWork_.store(true);
        hasWork_.notify_all();
    }
    ThreadPool::~ThreadPool(){
        isRunning_.store(false);
        hasWork_.store(true);
        hasWork_.notify_all();
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
            auto task = tasks_.pop();
            if(task) {
                task.value()();
            }else{
                hasWork_.store(false);
                hasWork_.wait(false);
            }
        }
    }
    void ThreadPool::workOld(){
        while(isRunning_) {
            std::unique_lock guard(old_mutex_);
            if(!old_tasks_.empty()) {
                auto task = old_tasks_.front();
                old_tasks_.pop();
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