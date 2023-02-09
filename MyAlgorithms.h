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
        std::vector<std::future<decltype(first)>> results;

        for(int i=0;i<chunks; ++i){
            results.push_back(pool.doAsync([first, i, last, value, chunkSize]() {
                Iterator right = std::min(first + (i + 1) * chunkSize, last);
                Iterator result = std::find(first + i * chunkSize, right, value);
                if (result == right) return last;
                return result;
            }));
        }
        pool.flush();
        Iterator minI = last;
//        for(int i=0; i<results.size(); ++i){
//            results[i].wait();
//            minI = results[i].get();
//            if(minI != last){
//                pool.clear();
////                std::cout << "out\n";
//                return minI;
//            }
//
//        }
        for(auto& result : results){
            minI = std::min(minI, result.get());
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
