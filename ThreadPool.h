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
            for(int i=0;i<threads_.size();++i){
                auto lastTaskLoad = lastTask_[i].load();
                curTask_[i].store(lastTaskLoad);
                lastTaskCached_[i] = lastTaskLoad;
            }
            while(inProcess_[0].load().field || inProcess_[1].load().field ||
                  inProcess_[2].load().field || inProcess_[3].load().field){
//                std::cout << "wait\n";
            }
        }
        void flush(){
            for(int i=0;i<threads_.size();++i){
                lastTask_[i].store(lastTaskCached_[i]);
            }
        }
        template<typename Function, typename ...Args>
        decltype(auto) doAsync(Function func, Args... args){
            using Result = std::invoke_result_t<Function, Args...>;
            std::function<Result()> task_function = std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
            std::shared_ptr<std::promise<Result>> task_result = std::make_shared<std::promise<Result>>();

            tasks_[nextThread_][lastTaskCached_[nextThread_].field] = [task_function, task_result]() {
                if constexpr (std::is_void_v<Result>) {
                    std::invoke(task_function);
                    task_result->set_value();
                }else {
                    task_result->set_value(std::invoke(task_function));
                }
            };
            ++lastTaskCached_[nextThread_].field;
            if(curTask_[nextThread_].load().field == lastTask_[nextThread_].load().field) {
                lastTask_[nextThread_].store(lastTaskCached_[nextThread_]);
            }
//            std::cout << nextThread_ << " " << lastTaskCached_[nextThread_]<< std::endl;
            ++nextThread_;
            nextThread_ = nextThread_ % 4;

//            hasWork_.store(true);
//            hasWork_.notify_one();
            return task_result->get_future();
        }
        void stop();

    private:
        void initializeThreads(size_t numberOfThreads);
        void work(int id);

        std::vector<std::thread> threads_;

        std::atomic<bool> isRunning_;
        std::atomic<bool> hasWork_;
        int nextThread_ = 0;

        std::atomic<AlignedField<int, 64>> curTask_[4];
        alignas(64) std::atomic<AlignedField<int, 64>> lastTask_[4];
        alignas(64) AlignedField<int, 64> lastTaskCached_[4];
        alignas(64) std::atomic<AlignedField<bool, 64>> inProcess_[4];
        my::ModuleVector<std::function<void()>, 256> tasks_[4];
    };

} // my

#endif //MYALGORITHMS_THREADPOOL_H
