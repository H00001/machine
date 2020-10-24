//
// Created by dos DRTT on 10/24/20.
//

#ifndef MACHINE_CPU1_HH
#define MACHINE_CPU1_HH

#include <utility>
#include "../common/const.hh"

namespace gunplan::cplusplus::machine {
    class cpu1 {
    public:
        virtual ~cpu1() = default;

        virtual void push_stack(unsigned long val) = 0;

        virtual unsigned long pop_stack() = 0;

        virtual void push_process(std::pair<std::pair<code_buffer, data_buffer>, unsigned long> p) = 0;

        virtual void set_resource(memory *mm) = 0;
    };
}


#endif //MACHINE_CPU1_HH
