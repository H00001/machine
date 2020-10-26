//
// Created by dos DRTT on 10/24/20.
//

#ifndef MACHINE_CONST_HH
#define MACHINE_CONST_HH

#include <string>

const int mm_size = 1024;
using byte = unsigned char;
using offset = unsigned int;


struct segment_buffer {
    offset *b;
    int length;
};

#endif //MACHINE_CONST_HH
