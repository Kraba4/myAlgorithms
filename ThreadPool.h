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
        ThreadPool(){
            initializeThreads(std::thread::hardware_concurrency());
        }
        ThreadPool(size_t numberOfThreads){
            initializeThreads(numberOfThreads);
        }

        size_t getNumberOfThreads(){
            return threads_.size();
        }
        void doAsync(Task& task){
            std::lock_guard guard(mutex_);
            tasks_.push_back(&task);
            //tasks_[last_++] = &task;
            //std::cout << last_ << "= last\n";
        }
        void stop(){
            isRunning_ = false;
        }
        ~ThreadPool(){
            isRunning_ = false;
//            std::cout << "bye";
            for(auto& t : threads_){
                t.join();
            }
        }
    private:
        void initializeThreads(size_t numberOfThreads){
            threads_.reserve(numberOfThreads);
            isRunning_ = true;
            for(int i=0; i<numberOfThreads; ++i){
                threads_.emplace_back([this](){
                    this->work();
                });
            }
        };

        void work(){
            while(isRunning_) {
                std::unique_lock guard(mutex_);
                if(!tasks_.empty()) {

                    auto task = tasks_.front();
                    tasks_.pop_front();
                   // std::cout << "here " << tasks_.size() << std::endl;
                    guard.unlock();

                    task->mutex_.lock();
                    task->result = task->func();
                    task->hasResult = true;
                    task->hasResult.notify_one();
                    task->mutex_.unlock();
                   // std::cout << "here2 " << tasks_.size() << std::endl;
                }
            }
        }

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
