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
    using cpu_register = unsigned long;
    using intstru_len = int;
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

    struct m_cpu {
        cpu_register eax, ebx, ecx, edx;
        cpu_register cs, ss, ds, es, fs, hs;
        cpu_register esi, edi, esp, ebp;
        cpu_register rs0, rs1, rs2;
        cpu_register bit_flags;
        unsigned long rip;
        std::string pc;
        segment_disruptor *ldt_cache;
    };

    enum operaType {
        num, reg, str
    };

    struct oper_code {
        unsigned long *oper_reg;
        std::string oper_str;
        unsigned short oper_reg_name;
        long oper_val{};
        operaType oper_type;
    };

    using opFN = std::function<int(std::list<oper_code *> *)>;
    using rHeap = std::map<unsigned short, unsigned long *>;
    using operatorMap = std::map<int, opFN>;

}