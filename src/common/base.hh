//
// Created by dos DRTT on 6/14/20.
//

#ifndef MACHINE_BASE_HH
#define MACHINE_BASE_HH

#endif //MACHINE_BASE_HH

#include <string>
#include <map>
#include <list>

namespace gunplan::cplusplus::machine {
    using byte = unsigned char;

    struct cache_block {
        int flag;
        unsigned long data;
        int type;
    };

    struct segment_disruptor {
        unsigned long start;
        int len;
        byte type;
    };


    enum operaType {
        num, reg
    };


}