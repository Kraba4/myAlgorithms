//
// Created by HONOR on 07.02.2023.
//

#ifndef MYALGORITHMS_BLOCKINGQUEUE_H
#define MYALGORITHMS_BLOCKINGQUEUE_H


#include <queue>
#include <mutex>
#include <optional>
template<typename ItemType>
class BlockingQueue {
public:
    void push(const ItemType& item) {
        std::lock_guard lock(mutex_);
        queue.push(item);
    }

    void push(ItemType&& item) {
        std::lock_guard lock(mutex_);
        queue.push(std::move(item));
    }
    std::optional<ItemType> pop(){
        std::unique_lock lock(mutex_);

        if(queue.empty()){
            return {};
        }
        auto elem = std::move(queue.front());
        queue.pop();
        return std::move(elem);
    }
    void clear(){
        std::lock_guard lock(mutex_);
        for(int i=0;i<queue.size();++i){
            queue.pop();
        }
    }
private:
    std::queue<ItemType> queue;
    std::mutex mutex_;
};


#endif //MYALGORITHMS_BLOCKINGQUEUE_H
