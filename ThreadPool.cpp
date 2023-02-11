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
            threads_.emplace_back([this, i](){
                this->work(i);
            });
        }
    };

    void ThreadPool::work(int id){
        while(isRunning_) {
            if(!pause_.load(std::memory_order_relaxed)) {
                int lastTask = lastTask_[id].field.load(std::memory_order_acquire);
                auto curTask = curTask_[id].field.load(std::memory_order_acquire);

                if (curTask != lastTask) {
                    inProcess_[id].field.store(true);
                    auto task = tasks_[id][curTask];
                    curTask_[id].field.compare_exchange_strong(curTask, curTask + 1, std::memory_order_acq_rel);
                    std::invoke(task);
                    inProcess_[id].field.store(false);
                    //std::cout << "do " << curTask.field << std::endl;
                } else {
                    inProcess_[id].field.store(false);
//                hasWork_.store(false);
//                hasWork_.wait(false);
                }
            }
        }
    }
} // my