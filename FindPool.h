//
// Created by HONOR on 05.02.2023.
//

#ifndef MYALGORITHMS_FINDPOOL_H
#define MYALGORITHMS_FINDPOOL_H

#include "ThreadPool.h"

namespace my {
    template<std::random_access_iterator Iterator>
    Iterator find(my::ThreadPool& pool, Iterator first, Iterator last, const std::iter_value_t<Iterator>& value){
        std::vector<my::ThreadPool::Task> tasks;
        tasks.reserve(pool.getNumberOfThreads());
        int chunkSize = (last - first) / pool.getNumberOfThreads();

        for(int i=0;i<pool.getNumberOfThreads(); ++i){
            tasks.emplace_back([first, i, last, value, chunkSize](){
                Iterator right =  std::min(first +(i+1)*chunkSize, last);
                Iterator result = std::find(first + i*chunkSize, right, value);
                if(result == right) return last;
                return result;
            });

        }
        for(auto& task : tasks){
            pool.doAsync(task);
        }
        for(auto& task : tasks){
            task.wait();
        }
        Iterator minI = last;
        for(auto& task : tasks){
            minI = std::min(minI, any_cast<Iterator>(task.result));
        }
        return minI;
    }
}


#endif //MYALGORITHMS_FINDPOOL_H
