//
// Created by HONOR on 05.02.2023.
//

#ifndef MYALGORITHMS_MYALGORITHMS_H
#define MYALGORITHMS_MYALGORITHMS_H

#include "ThreadPool.h"

namespace my {
    template<std::random_access_iterator Iterator>
    Iterator find(my::ThreadPool& pool, Iterator first, Iterator last, const std::iter_value_t<Iterator>& value){
        const int chunks = 100;
        int chunkSize = (last - first) / chunks;
        std::vector<std::promise<Iterator>> promises;
        promises.reserve(chunks);

        std::vector<std::future<Iterator>> results;
        results.reserve(chunks);
        std::atomic<int> statusResults[chunks];
        int done = 0;
        for(int i=0;i<chunks; ++i){
            promises.emplace_back();
            auto& promise = promises.back();
            results.push_back(promise.get_future());

            statusResults[i].store(0);
            auto& status = statusResults[i];
            pool.doAsync([first, i, last, value, chunkSize, &promise, &status ]() mutable {
                Iterator right = std::min(first + (i + 1) * chunkSize, last);
                Iterator result = std::find(first + i * chunkSize, right, value);
                if (result == right) {
                    promise.set_value(last);
                    status.store(1);
                }
                else {
                    promise.set_value(result);
                    status.store(2);
                }
            });
            if(done <= i){
                if(statusResults[done].load() == 2){
                    pool.clear();
                    return results[done].get();
                }else if(statusResults[done].load() == 1){
                    ++done;
                }
            }
        }
        pool.flush();
        Iterator minI = last;

        for(int i = done; i<results.size(); ++i){
            minI = results[i].get();
            if(minI != last){
                pool.clear();
                return minI;
            }
        }
        return minI;
//        return last;
    }
}


#endif //MYALGORITHMS_MYALGORITHMS_H
