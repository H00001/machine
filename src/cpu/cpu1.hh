//
// Created by dos DRTT on 10/24/20.
//

#ifndef MACHINE_CPU1_HH
#define MACHINE_CPU1_HH

#include <utility>
#include "../common/const.hh"

namespace gunplan::cplusplus::machine {
    template<typename type>
    class cpu1 {
    public:
        virtual ~cpu1() = default;

        virtual void push_stack(type val) = 0;

        virtual type pop_stack() = 0;

        virtual int push_process(int pid) = 0;

        virtual void set_resource(memory *mm) = 0;
    };
}


#endif //MACHINE_CPU1_HH
