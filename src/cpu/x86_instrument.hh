//
// Created by dos DRTT on 10/27/20.
//

#ifndef MACHINE_X86_INSTRUMENT_HH
#define MACHINE_X86_INSTRUMENT_HH

#include<list>
#include<functional>
#include <map>

namespace gunplan::cplusplus::machine {
#define instrument_length  sizeof(cpu_register)
    enum operaType {
        num = 0, reg = 1, addr = 2
    };

    using cpu_register = unsigned int;
    using cpu_register_segment = unsigned short;
    using cpu_register_point = cpu_register *;
    using decode_register = unsigned char;
    using op_bond = unsigned char;

    struct oper_code {
        cpu_register_point oper_reg{0};
        cpu_register oper_val{0};
        decode_register oper_type;

    };

    struct m_cpu {
        cpu_register eax{0}, ebx{0}, ecx{0}, edx{0};
        cpu_register_segment cs{0}, ss{0}, ds{0}, es{0}, fs{0}, hs{0};
        cpu_register esi{0}, edi{0}, esp{0}, ebp{0};
        cpu_register rs0{0}, rs1{0}, rs2{0};
        cpu_register bit_flags{0};
        cpu_register rip{0};
        cpu_register pc{0};
#ifdef _vector_
        cpu_register t0{0}, t1{0}, t2{0}, t3{0}, t4{0}, t5{0}, t6{0}, t7{0};
        cpu_register r0{0}, r1{0}, r2{0}, r3{0}, r4{0}, r5{0}, r6{0}, r7{0};
#endif

    };

    using opFn = std::function<int(std::list<oper_code> *)>;

    using register_heap = std::map<decode_register, cpu_register *>;
    using operatorMap = std::map<op_bond, opFn>;

    struct decode_result {
        op_bond cmd;
        int op_size;
        std::list<oper_code> *oplist;
    };
}
#endif //MACHINE_X86_INSTRUMENT_HH
