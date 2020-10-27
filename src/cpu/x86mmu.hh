//
// Created by dos DRTT on 10/27/20.
//

#ifndef MACHINE_X86MMU_HH
#define MACHINE_X86MMU_HH

#include "../mm/memory.hh"

namespace gunplan::cplusplus::machine {
    class x86mmu {
    public:
        static address_bond transfer(segment_disruptor *ldt, segment_selector sd, address_bond offset);
    };
}


#endif //MACHINE_X86MMU_HH
