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
    Iterator find(Iterator first, Iterator last, const std::iter_value_t<Iterator>& value){

        const int chunkSize = (64/sizeof(value)) * 4000;
        const int chunks = static_cast<int>((last-first) / chunkSize) +
                           (static_cast<int>(last-first)%chunkSize != 0 ? 1:0);
        constexpr int N_THREADS = 4;
        std::vector<std::future<Iterator>> results(N_THREADS);
        std::atomic<int> chunk = 0;
        for(int i=0;i<N_THREADS;++i){
            results[i] = std::async(std::launch::async, [&first, &last, &chunk, chunks, chunkSize, &value](){
                while(true){
                    int myChunk = chunk.load();
                    if(myChunk == chunks){
                        return last;
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
                        chunk.store(chunks);
                        return myResult;
                    }

                }
            });
        }
        Iterator minI = last;
        for(int i=0;i<N_THREADS;++i){
            Iterator result = results[i].get();
            minI = std::min(result, minI);
        }
        return minI;
    }
    template<std::random_access_iterator Iterator, typename Comp = std::less<> >
    Iterator partition(Iterator first, Iterator last, std::iter_value_t<Iterator> pivot, Comp comp = Comp()){
        while(first <= last){
            while(comp(*first, pivot)){
                ++first;
            }
            while(comp(pivot, *last)){
                --last;
            }
            if(first >= last){
                break;
            }
            std::swap(*first, *last);
            ++first;
            --last;
        }
       return last;
    }
    template<std::random_access_iterator Iterator, typename Comp = std::less<> >
    void quick_sort(Iterator first, Iterator last, Comp comp = Comp()){
        if(last == first){
            return;
        }
        Iterator center = first + (last - first)/2;
        Iterator pivot = partition(first, last, *center,  comp);
        quick_sort(first, pivot);
        quick_sort(pivot+1, last);
    }
    template<std::random_access_iterator Iterator, typename Comp = std::less<> >
    void sort(Iterator begin, Iterator end, Comp comp= Comp()){
        quick_sort(begin, end-1, comp);
    }

}


#endif //MYALGORITHMS_MYALGORITHMS_H
