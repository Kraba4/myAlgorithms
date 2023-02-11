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
                auto lastTaskLoad = lastTask_[i].field.load();
                curTask_[i].field.store(lastTaskLoad);
                lastTaskCached_[i].field = lastTaskLoad;
            }
            while(inProcess_[0].field.load() || inProcess_[1].field.load() ||
                  inProcess_[2].field.load() || inProcess_[3].field.load()){
//                std::cout << "wait\n";
            }
//            std::cout << "clear done \n";
        }
        void flush(){
            for(int i=0;i<threads_.size();++i){
                lastTask_[i].field.store(lastTaskCached_[i].field);
            }
        }
        void doAsync(std::function<void()>&& func){
            if(pause_.load()) {
                pause_.store(false);
//                pause_.notify_all();
//                std::cout << "begin\n";
            }
            tasks_[nextThread_][lastTaskCached_[nextThread_].field] = std::move(func);
            ++lastTaskCached_[nextThread_].field;
            if(curTask_[nextThread_].field.load() == lastTask_[nextThread_].field.load()) {
                lastTask_[nextThread_].field.store(lastTaskCached_[nextThread_].field);
            }
            ++nextThread_;
            nextThread_ = nextThread_ % 4;

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
        AlignedField<std::atomic<int>, 64> curTask_[4];
        AlignedField<std::atomic<int>, 64> lastTask_[4];
        AlignedField<int, 64> lastTaskCached_[4] = {0,0,0,0};
        AlignedField<std::atomic<int>,64> inProcess_[4];
        alignas(64) my::ModuleVector<std::function<void()>, 2048> tasks_[4];
    };

} // my

#endif //MYALGORITHMS_THREADPOOL_H
