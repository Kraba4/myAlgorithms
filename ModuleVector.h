//
// Created by HONOR on 05.02.2023.
//

#ifndef MYALGORITHMS_MODULEVECTOR_H
#define MYALGORITHMS_MODULEVECTOR_H

#include <array>
namespace my {
    template<typename ItemType, int Size>
    class ModuleVector {
    public:
        ItemType &operator[](int index) {
            return array[index % Size];
        }

    private:
        std::array<ItemType, Size> array;
    };
}

#endif //MYALGORITHMS_MODULEVECTOR_H
