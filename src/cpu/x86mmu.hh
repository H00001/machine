//
// Created by dos DRTT on 10/27/20.
//

#ifndef MACHINE_X86MMU_HH
#define MACHINE_X86MMU_HH

#include "../mm/memory.hh"

namespace gunplan::cplusplus::machine {
    class x86mmu {
    private:
        segment_disruptor *ldt;
        segment_disruptor *gdt;
    public:
        x86mmu() = default;

        void set_ldt(segment_disruptor *ldt);

        void set_gdt(segment_disruptor *gdt);

        address_bond transfer(segment_selector sd, address_bond offset);
    };
}


#endif //MACHINE_X86MMU_HH
