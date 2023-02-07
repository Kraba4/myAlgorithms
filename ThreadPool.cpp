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
            threads_.emplace_back([this](){
                this->work();
            });
        }
    };

    void ThreadPool::work(){
        while(isRunning_) {
            auto task = tasks_.pop();
            if(task) {
                std::invoke(task.value());
            }else{
                hasWork_.store(false);
                hasWork_.wait(false);
            }
        }
    }
} // my