//
// Created by HONOR on 05.02.2023.
//

#ifndef MYALGORITHMS_FINDPOOL_H
#define MYALGORITHMS_FINDPOOL_H

#include <execution>
#include <concepts>
#include "ThreadPool.h"

namespace my {
    template<std::random_access_iterator Iterator>
    class FindPool{
    public:
        FindPool(ThreadPool& pool, int nThreads, int chunkCount, Iterator first, Iterator last, const std::iter_value_t<Iterator>& value)
                : first_(first), last_(last), chunkCount_(chunkCount) {

//            searchValue_ = value;
//            result_ = last;
//            chunkSize_ = (last_ - first_) / chunkCount_;
//            extra_ = (last_ - first_) % chunkSize_;
//            //threads_.reserve(nThreads);
////            if(last_ - first < 10000){
////                finalResult_ = std::find(first_, last_, value);
////                return;
////            }
//            std::vector<ThreadPool::Task> tasks;
//            for(int i=0;i<pool.getNumberOfThreads(); ++i){
//                tasks.emplace_back(i, [this,i](){
//                    return std::find(first_ + i*chunkSize_, std::min(first_ +(i+1)*chunkSize_, last_), searchValue_);
//                });
//
//            }
//            for(auto& task : tasks){
//                pool.doAsync(task);
//            }
//            //std::cout << "im here1" << std::endl;
//            for(auto& task : tasks){
//                task.wait();
//            }
//            Iterator minI = last_;
//            for(auto& task : tasks){
//                minI = std::min(minI, any_cast<Iterator>(task.result));
//            }
//            result_ = minI;
            //std::cout << "im here2" << std::endl;
            //check time
            //auto start = std::chrono::high_resolution_clock::now();

//             wait for threads


            // don't wait for threads
//            for(int i=0;i<nThreads;++i){
//                threads_[i].detach();
//            }
//            while( endCounter_ != nThreads){
//                ;
//            }
//            for(int i=0;i<nThreads;++i){
//                threads_[i].detach();
//            }
            finalResult_ = result_;
            //check time
            //auto end = std::chrono::high_resolution_clock::now();
            // std::cout << time << std::endl;

        }
        void work(){
            for(;;) {
                if (result_ != last_) {
                    return;
                }
                auto [f, l] = getChunk(next_++);

                if (f == last_) {
                    return;
                }
                f = std::find(f, l, searchValue_);
                if (f != l) {
                    offerResult(f);
                }
            }
        }

        Iterator getResult(){
            return finalResult_;
        }
    private:

        void offerResult(const Iterator& result){
            while(resultGuard_.exchange(true)){
                ;
            }

            if(result < result_){
                result_ = result;
            }
            resultGuard_.store(false);
        }
        std::tuple<Iterator, Iterator> getChunk(int n){

            if(n >= chunkCount_){
                Iterator first = last_;
                Iterator last = last_;
                return std::move(std::make_tuple(std::move(first), std::move(last)));
            }

            if(n < extra_) {
                Iterator first = first_ + n*chunkSize_ + n;
                Iterator last = first + chunkSize_ + 1;
                return std::move(std::make_tuple(std::move(first), std::move(last)));
            } else{
                Iterator first = first_ + n*chunkSize_ + extra_;
                Iterator last = first + chunkSize_;
                return std::move(std::make_tuple(std::move(first), std::move(last)));
            }

        }
        //bool end_ = false;
        // std::atomic<bool> end_ = false;
        int endCounter_ = 0;
        std::atomic<bool> resultGuard_ = false;
        Iterator result_;
        Iterator finalResult_;
        std::iter_value_t<Iterator> searchValue_;
        int extra_;
    public:
        int chunkCount_;
        int chunkSize_;
        std::atomic_uint_fast32_t next_ = 0;
        //int next_ = 0;
        Iterator first_, last_;
    };

    template<std::random_access_iterator Iterator>
    Iterator /*double*/ find(my::ThreadPool& pool, Iterator first, Iterator last, const std::iter_value_t<Iterator>& value){
        //std::cout << "im here" << std::endl;
        std::vector<ThreadPool::Task> tasks;
        tasks.reserve(pool.getNumberOfThreads());
//        std::cout << pool.getNumberOfThreads() << "= threads \n";
        int chunkSize = (last - first) / pool.getNumberOfThreads();

        for(int i=0;i<pool.getNumberOfThreads(); ++i){
            tasks.emplace_back([first, i, last, value, chunkSize](){
//               return 2;
                return std::find(first + i*chunkSize, std::min(first +(i+1)*chunkSize, last), value);
            });

        }
        for(auto& task : tasks){
            pool.doAsync(task);
        }
//        //std::cout << "im here1" << std::endl;
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
