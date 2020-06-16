//
// Created by dos DRTT on 6/14/20.
//

#ifndef MACHINE_BASE_HH
#define MACHINE_BASE_HH

#endif //MACHINE_BASE_HH

#include <string>
#include <map>

namespace gunplan::cplusplus::machine {
    using cpu_register = unsigned long;
    using intstru_len = int;
    using byte = unsigned char;


    struct LDT {
        void *base;
        byte type;
        int len;
    };

    struct cache_block {
        int flag;
        unsigned long data;
        int type;
    };

    struct m_cpu {
        cpu_register cs, ss, ds, es, hs;
        cpu_register esp, ebp;
        cpu_register eax, ebx, ecx, edx;
        cpu_register esi, edi;
        cpu_register rs0, rs1, rs2;
        cpu_register bit_flags;
        std::map<std::string, int> *func_map;
        std::map<cpu_register, LDT *> *seg_divide;
        unsigned long rip;
        std::string pc;
    };
}