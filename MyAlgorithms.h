//
// Created by HONOR on 05.02.2023.
//

#ifndef MYALGORITHMS_MYALGORITHMS_H
#define MYALGORITHMS_MYALGORITHMS_H

#include "ThreadPool.h"

namespace my {
    template<std::random_access_iterator Iterator>
    Iterator find(my::ThreadPool& pool, Iterator first, Iterator last, const std::iter_value_t<Iterator>& value){
        const int chunks = 2000;
        int chunkSize = (last - first) / chunks;
//        std::vector<std::promise<Iterator>> promises;
//        promises.reserve(chunks);

        std::vector<Iterator> results;
        results.reserve(chunks);
        std::atomic<int> statusResults[chunks];
        int done = 0;
        for(int i=0;i<chunks; ++i){
//            promises.emplace_back();
//            results.push_back(promises.back().get_future());
            results.emplace_back(last);
            statusResults[i].store(0);
            pool.doAsync([first, i, last, value, chunkSize, &promise = results[i],
                          &status = statusResults[i]]() mutable {

                Iterator right = last;
                if(( last - (first + i*chunkSize) )>= chunkSize){
                    right = first + ((i + 1) * chunkSize);
                }
                //std::cout << last - right<< std::endl;
                Iterator result = std::find(first + (i * chunkSize), right, value);
                if (result == right) {
                    promise = last;
                    status.store(1);
                }
                else {
                    promise = result;
                    status.store(2);
                    //std::cout <<"find\n";
                }
            });
            if(done <= i){
//                std::cout << done << std::endl;
                if(statusResults[done].load() == 2){

                    pool.clear();
                    return results[done];
                }else if(statusResults[done].load() == 1){

                    ++done;
                }
            }
        }
        pool.flush();
        Iterator minI = last;

        for(int i = done; i<results.size(); ++i){
            while(statusResults[i].load()==0)
                ;
            //std::cout << i << std::endl;
            if(statusResults[i].load() == 2) {
                minI = results[i];
//                std::cout << statusResults[i].load() << std::endl;
                if (minI != last) {
                    pool.clear();
                    return minI;
                }
            }
        }
        pool.clear();
        return minI;
//        return last;
    }
}


#endif //MYALGORITHMS_MYALGORITHMS_H
