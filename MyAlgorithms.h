//
// Created by HONOR on 05.02.2023.
//

#ifndef MYALGORITHMS_MYALGORITHMS_H
#define MYALGORITHMS_MYALGORITHMS_H

#include "ThreadPool.h"

namespace my {
    template<std::random_access_iterator Iterator>
    Iterator find(my::ThreadPool& pool, Iterator first, Iterator last, const std::iter_value_t<Iterator>& value){
        const int chunkSize = (64/sizeof(value)) * 4000;
        const int chunks = static_cast<int>((last-first) / chunkSize) +
                           (static_cast<int>(last-first)%chunkSize != 0 ? 1:0);
        std::vector<Iterator> results(pool.getNumberOfThreads(), last);
        std::atomic<int> chunk = 0;
        for(int i=0;i<pool.getNumberOfThreads();++i){
            pool.doAsync([&first, &last, &chunk, chunks, &result = results[i], chunkSize, &value](){
                while(true){
                    int myChunk = chunk.load();
                    if(myChunk == chunks){
                        return ;
                    }
                    if(!chunk.compare_exchange_weak(myChunk, myChunk+1)){
                        continue;
                    }
                    Iterator left = first + myChunk*chunkSize;
                    Iterator right = last;
                    if(last - left > chunkSize){
                        right = left + chunkSize;
                    }
                    Iterator myResult = std::find(left, right, value);
                    if(myResult != right){
                        result = myResult;
                        chunk.store(chunks);
                    }

                }
            });
        }
        pool.waitAll();
        return *std::min_element(results.begin(), results.end());
    }
    template<std::random_access_iterator Iterator>
    Iterator findOld(my::ThreadPool& pool, Iterator first, Iterator last, const std::iter_value_t<Iterator>& value){
        const int chunkSize = (64/sizeof(value)) * 4000;
        const int chunks = static_cast<int>((last-first) / chunkSize) +
                           (static_cast<int>(last-first)%chunkSize != 0 ? 1:0);

        std::vector<Iterator> results;
        results.reserve(chunks);
//        auto statusResults = new std::atomic<int>[chunks];
        std::deque<std::atomic<int>> statusResults(chunks);

        results.emplace_back(std::find(first, first + chunkSize, value));
        if(results[0] != (first + chunkSize)){
            return results[0];
        }

        int done = 1;
        for(int i=1;i<chunks; ++i){
            results.emplace_back(last);
//            statusResults[i].store(0);

            Iterator right = last;
            if(static_cast<int>( last - (first + i*chunkSize) )>= chunkSize){
                right = first + ((i + 1) * chunkSize);
            }
            Iterator left = first + (i * chunkSize);
            pool.doAsync([left = std::move(left), right = std::move(right), &value, &promise = results[i],
                                 &status = statusResults[i]]() mutable {

                Iterator result = std::find(left, right, value);
                promise = result;
                if (result == right) {
                    status.store(1);
                }
                else {
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
                pool.clear();
                return minI;
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
