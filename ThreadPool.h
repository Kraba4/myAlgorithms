//
// Created by HONOR on 05.02.2023.
//

#ifndef MYALGORITHMS_THREADPOOL_H
#define MYALGORITHMS_THREADPOOL_H

#include <utility>
#include <vector>
#include <thread>
#include <functional>
#include <deque>
#include <any>
#include <mutex>
#include <iostream>
#include <future>
#include <queue>
#include <future>

#include "ModuleVector.h"
#include "BlockingQueue.h"
#include "AlignedField.h"
namespace my {

    class ThreadPool {

    public:
        ThreadPool();
        ThreadPool(size_t numberOfThreads);
        ~ThreadPool();

        size_t getNumberOfThreads();
        void clear(){
//            std::cout << "start clear\n";
            pause_.store(true);
            for(int i=0;i<threads_.size();++i){
                auto lastTaskLoad = lastTask_[i].load();
                curTask_[i].store(lastTaskLoad);
                lastTaskCached_[i] = lastTaskLoad;
            }
            while(inProcess_[0].load() || inProcess_[1].load() ||
                  inProcess_[2].load() || inProcess_[3].load()){
//                std::cout << "wait\n";
            }
//            std::cout << "clear done \n";
        }
        void flush(){
            for(int i=0;i<threads_.size();++i){
                lastTask_[i].store(lastTaskCached_[i]);
            }
        }
        void doAsync(std::function<void()> func){
            if(pause_.load()) {
                pause_.store(false);
//                std::cout << "begin\n";
            }
            tasks_[nextThread_][lastTaskCached_[nextThread_]] = std::move(func);
            ++lastTaskCached_[nextThread_];
            if(curTask_[nextThread_].load() == lastTask_[nextThread_].load()) {
                lastTask_[nextThread_].store(lastTaskCached_[nextThread_]);
            }
//            std::cout << nextThread_ << " " << lastTaskCached_[nextThread_]<< std::endl;
            ++nextThread_;
            nextThread_ = nextThread_ % 4;
//            hasWork_.store(true);
//            hasWork_.notify_one();
        }
        void stop();

    private:
        void initializeThreads(size_t numberOfThreads);
        void work(int id);

        std::vector<std::thread> threads_;

        std::atomic<bool> isRunning_;
        std::atomic<bool> hasWork_;
        int nextThread_ = 0;
        std::atomic<bool> pause_ = false;
        std::atomic<int> curTask_[4];
        alignas(64) std::atomic<int> lastTask_[4];
        alignas(64) int lastTaskCached_[4] = {0,0,0,0};
        alignas(64) std::atomic<int> inProcess_[4];
        my::ModuleVector<std::function<void()>, 512> tasks_[4];
    };

} // my

#endif //MYALGORITHMS_THREADPOOL_H
