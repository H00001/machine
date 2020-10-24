//
// Created by dos DRTT on 10/24/20.
//

#include "memory.hh"

namespace gunplan::cplusplus::machine {
    struct seg;
    struct segment_selector;


    unsigned long *memory::hd_mem = new unsigned long[mm_size];
    std::string *memory::hd_code_mem = new std::string[240];

};