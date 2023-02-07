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
#include "ModuleVector.h"
#include "BlockingQueue.h"

namespace my {

    class ThreadPool {

    public:
        ThreadPool();
        ThreadPool(size_t numberOfThreads);
        ~ThreadPool();

        size_t getNumberOfThreads();
        void clear(){
            tasks_.clear();
        }
        template<typename Function, typename ...Args>
        decltype(auto) doAsync(Function&& func, Args&&... args){
            using Result = std::invoke_result_t<Function, Args...>;
            std::function<Result()> task_function = std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
            std::shared_ptr<std::promise<Result>> task_result = std::make_shared<std::promise<Result>>();

            tasks_.push([task_function, task_result]() {
                if constexpr (std::is_void_v<Result>) {
                    std::invoke(task_function);
                    task_result->set_value();
                }else {
                    task_result->set_value(std::invoke(task_function));
                }
            });
            hasWork_.store(true);
            hasWork_.notify_one();
            return task_result->get_future();
        }
        void stop();

    private:
        void initializeThreads(size_t numberOfThreads);
        void work();

        std::vector<std::thread> threads_;
        BlockingQueue<std::function<void()>> tasks_;
        std::atomic<bool> isRunning_;
        std::atomic<bool> hasWork_;

    };

} // my

#endif //MYALGORITHMS_THREADPOOL_H
