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
#include "ModuleVector.h"

namespace my {

    class ThreadPool {

    public:
        struct Task{
            explicit Task( std::function<std::any()>&& func) : func(func) {}
            Task(const Task& another){
                func = another.func;
                hasResult.store(another.hasResult);
                result = another.result;

            }
            std::function<std::any()> func;
            std::atomic<bool> hasResult = false;
            std::any result;
            std::mutex mutex_;
            void wait(){
                while(true){
                    std::unique_lock guard(mutex_);
                    if(hasResult) break;
                    hasResult.wait(true);
                }
            }
        };
        ThreadPool();
        ThreadPool(size_t numberOfThreads);
        ~ThreadPool();

        size_t getNumberOfThreads();
        void doAsync(Task& task);
        void stop();

    private:
        void initializeThreads(size_t numberOfThreads);
        void work();

        bool isRunning_;
        std::vector<std::thread> threads_;
        //my::ModuleVector<Task*, 300> tasks_;
        std::list<Task*> tasks_;
        std::atomic<int> front_ = 0;
        std::atomic<int> last_ = 0;
        std::mutex mutex_;

    };

} // my

#endif //MYALGORITHMS_THREADPOOL_H
