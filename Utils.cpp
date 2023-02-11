//
// Created by HONOR on 07.02.2023.
#include <iomanip>
#include "Utils.h"
void utils::testFindAndPrint(std::function<
        std::vector<unsigned>::iterator(std::vector<unsigned>::iterator,std::vector<unsigned>::iterator, unsigned )> find,
                      std::initializer_list<FindTest> tests, unsigned seed, unsigned numberOfRuns, unsigned range){
    std::cout << std::fixed << std::setprecision(9) << std::left;
    constexpr int BILLION = 1'000'000'000;
    for(FindTest test : tests) {
        auto arr = utils::create_random_vector(test.size, range, seed + test.size);
        int valueToFind = arr[test.indexToBeFound];

        decltype(arr.begin()) result;
        auto [time, max_result, min_result] = utils::runAndCheckTime(numberOfRuns, [&](){
            result = find(arr.begin(), arr.end(), valueToFind);
        });
        std::cout << "size = "<< std::setw(9) << test.size << " time: " << time / BILLION << " s " << " ("
        << min_result/BILLION << " - " << max_result/BILLION << " s) |***| ";
        std::cout << "index = "<< result - arr.begin() << " (" << *result << "==" << valueToFind << ") \n";
    }
    std::cout << "\n";
}
std::vector<unsigned int> utils::create_random_vector(size_t size, unsigned int range, unsigned int seed) {
    std::vector<unsigned int> v;

    std::minstd_rand rnd(seed);

    for (int i = 0; i < size; ++i) {
        v.push_back(rnd() % range);
    }

    return std::move(v);
}
//
