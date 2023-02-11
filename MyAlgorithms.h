//
// Created by HONOR on 05.02.2023.
//

#ifndef MYALGORITHMS_MYALGORITHMS_H
#define MYALGORITHMS_MYALGORITHMS_H

#include "ThreadPool.h"

namespace my {
    template<std::random_access_iterator Iterator>
    Iterator find(my::ThreadPool& pool, Iterator first, Iterator last, const std::iter_value_t<Iterator>& value){
        const int chunks = std::max(static_cast<int>((last-first) / ((64/sizeof(value)) * 1500)), 1);
        int chunkSize = (last - first) / chunks;

        std::vector<Iterator> results;
        results.reserve(chunks);
        auto statusResults = new std::atomic<int>[chunks];


        Iterator result = std::find(first, first + chunkSize, value);
        if(result != first + chunkSize){
            return result;
        }

        int done = 1;
        for(int i=1;i<chunks; ++i){
            results.emplace_back(last);
            statusResults[i].store(0);
            pool.doAsync([first, i, last, value, chunkSize, &promise = results[i],
                          &status = statusResults[i]]() mutable {

                Iterator right = last;
                if(( last - (first + i*chunkSize) )>= chunkSize){
                    right = first + ((i + 1) * chunkSize);
                }
                Iterator result = std::find(first + (i * chunkSize), right, value);
                if (result == right) {
                    promise = last;
                    status.store(1);
                }
                else {
                    promise = result;
                    status.store(2);
                }
            });
            if(done <= i){
                int status = statusResults[done].load();
                if(status == 2){

                    pool.clear();
                    return results[done];
                }else if(status == 1){

                    ++done;
                }
            }
        }
        pool.flush();
        Iterator minI = last;

        for(int i = done; i<results.size(); ++i){
            int status = statusResults[i].load();
            while(status==0)
                status = statusResults[i].load();

            if(status == 2) {
                minI = results[i];
                if (minI != last) {
                    pool.clear();
                    return minI;
                }
            }
        }
        pool.clear();
        return minI;
    }


    template<std::random_access_iterator Iterator>
    Iterator find(Iterator first, Iterator last, const std::iter_value_t<Iterator>& value){
        const int chunks = 4;
        int chunkSize = (last - first) / chunks;

        std::vector<std::future<Iterator>> results;
        results.reserve(chunks);
        for(int i=0;i<chunks; ++i){
            results.push_back(std::async(std::launch::async, [first, i, last, value, chunkSize]() mutable {

                Iterator right = last;
                if(( last - (first + i*chunkSize) )>= chunkSize){
                    right = first + ((i + 1) * chunkSize);
                }
                Iterator result = std::find(first + (i * chunkSize), right, value);
                if (result == right) {
                    return last;
                }
                else {
                    return result;
                }
            }));
        }
        Iterator minI = last;
        for(int i=0; i<chunks; ++i){
            minI = std::min(minI, results[i].get());
        }
        return minI;

    }
}


#endif //MYALGORITHMS_MYALGORITHMS_H
