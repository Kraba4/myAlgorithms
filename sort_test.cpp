#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <execution>
#include <thread>
#include <numeric>

namespace my {
    template<std::random_access_iterator Iterator>
    class FindPool{
    public:
        FindPool(int nThreads, int chunkCount, Iterator first, Iterator last, const std::iter_value_t<Iterator>& value)
                : first_(first), last_(last), chunkCount_(chunkCount) {

            searchValue_ = value;
            result_ = last;
            chunkSize_ = (last_ - first_) / chunkCount_;
            extra_ = (last_ - first_) % chunkSize_;
            //threads_.reserve(nThreads);
//            if(last_ - first < 10000){
//                finalResult_ = std::find(first_, last_, value);
//                return;
//            }
            for(int i=0;i<nThreads;i++){
                threads_[i] = std::thread([this]() {
                   work();
                });
            }
            //check time
            //auto start = std::chrono::high_resolution_clock::now();

//             wait for threads
            for(int i=0;i<nThreads;++i){
                threads_[i].join();
            }

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
        std::array<std::thread, 4> threads_;
        std::atomic_uint_fast32_t next_ = 0;
        //int next_ = 0;
        Iterator first_, last_;
    };

    template<std::random_access_iterator Iterator>
    Iterator /*double*/ find(Iterator first, Iterator last, const std::iter_value_t<Iterator>& value){
        my::FindPool<Iterator> findPool(4, 100, first, last, value);
        //std::cout << findPool.chunkCount_ << " " << findPool.next_ << std::endl;
        return findPool.getResult(); //findPool.time;
    }
}

using std::cout;

std::vector<unsigned int>  create_random_vector(size_t size, unsigned int range, unsigned int seed){
    std::vector<unsigned int> v;

    std::minstd_rand rnd(seed);

    for(int i=0;i<size;++i){
        v.push_back(rnd() % range);
    }

    return std::move(v);
}
template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v){
    out << &v << "=[ ";
    for(auto& elem : v) {
        out << elem << ' ';
    }
    out << "] \n";
    return out;
}
template<typename Function, typename ...Args>
double callAndCheckTime(int n, const Function& fn){
    std::vector<int> results;
    for(int i=0;i<n;i++) {
        auto start = std::chrono::high_resolution_clock::now();
        fn();
        auto end = std::chrono::high_resolution_clock::now();
        results.push_back((end - start).count());
    }
    return std::reduce(results.begin(), results.end()) / results.size();
}

int main(){
    std::cout << std::fixed << std::setprecision(9) << std::left;
    const unsigned int SEED =1675442720;// time(NULL);
    const unsigned int RANGE = 1000'000'000;
    constexpr int N_CHECKS = 60;
    constexpr int BILLION = 1'000'000'000;

    cout << "SEED = " << SEED; //1675442720
    cout << std::endl;
    cout << "seq find: \n";
    for(int size=5000; size<= 5*std::pow(10, 7); size*=100) {
        auto arr = create_random_vector(size, RANGE, SEED + size);
        std::minstd_rand rnd(SEED + size);
//        int valueToFind = arr[arr.size()-1];
//        int valueToFind = arr[100];
        int valueToFind = arr[rnd() % size];
        //std::sort(std::execution::par, arr.begin(), arr.end());
        decltype(arr.begin()) result;
        double time = callAndCheckTime(N_CHECKS, [&](){
            result = std::find(arr.begin(), arr.end(), valueToFind);
        });

        cout << "size = "<< std::setw(9) << size << " time: " << time / BILLION<< " s |***| ";
        cout << "index = "<< result - arr.begin() << " (" << *result << "==" << valueToFind << ")\n";
    }
    cout << std::endl;
    cout << "my find: \n";
    for(int size=5000; size<= 5 * std::pow(10, 7); size*=100) {
        auto arr = create_random_vector(size, RANGE, SEED + size);
        std::minstd_rand rnd(SEED + size);
//        int valueToFind = arr[arr.size()-1];
//        int valueToFind = arr[100];
        int valueToFind = arr[rnd() % size];
        decltype(arr.begin()) result;
        double time2 = 0;
        double time = callAndCheckTime(N_CHECKS, [&](){
            result = my::find(arr.begin(), arr.end(), valueToFind);
        });
        cout << "size = "<< std::setw(9) << size << " time: " << time / BILLION << " s |***| ";// << "--> time2: "<< time2 << " s  <--  \n";
        cout << std::endl;
        //cout << "index = "<< result - arr.begin() << " (" << *result << "==" << valueToFind << ") \n";
    }
    cout << std::endl;
    cout << "par find: \n";
    for(int size=5000; size<= 5*std::pow(10, 7); size*=100) {
        auto arr = create_random_vector(size, RANGE, SEED + size);
        //std::sort(std::execution::par, arr.begin(), arr.end());
        std::minstd_rand rnd(SEED + size);
//        int valueToFind = arr[arr.size()-1];
//        int valueToFind = arr[100];
        int valueToFind = arr[rnd() % size];
        decltype(arr.begin()) result;
        double time = callAndCheckTime(N_CHECKS, [&](){
            result = std::find(std::execution::par, arr.begin(), arr.end(), valueToFind);
        });

        cout << "size = "<< std::setw(9) << size << " time: " << time / BILLION << " s |***| ";
        cout << "index = "<< result - arr.begin() << " (" << *result << "==" << valueToFind << ") \n";
    }


//    cout << std::endl;
//    for(int size=5000; size<= 5*std::pow(10, 7); size*=100) {
//        auto arr = create_random_vector(size, RANGE, SEED + size);
//        //std::sort(std::execution::par, arr.begin(), arr.end());
//        double time = callAndCheckTime(N_CHECKS, [&](){
//            for(auto it=arr.begin(); it!=arr.end(); ++it){
//                if(*it==4) {
//                    break;
//                }
//            }
//        });
//        cout << "handmade find size = "<< std::setw(9) << size << " time: " << time / BILLION<< " s \n";
//    }


}