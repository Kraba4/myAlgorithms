//
// Created by HONOR on 09.02.2023.
//

#ifndef MYALGORITHMS_ALIGNEDFIELD_H
#define MYALGORITHMS_ALIGNEDFIELD_H
template<typename Type, int Align>
struct AlignedField{
    alignas(Align) Type field;
};
#endif //MYALGORITHMS_ALIGNEDFIELD_H
