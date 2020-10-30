//
// Created by dos DRTT on 10/24/20.
//

#ifndef MACHINE_X86CPU_HH
#define MACHINE_X86CPU_HH
#define _vector_

#include <iostream>
#include "../mm/memory.hh"
#include "../util/binary.hh"
#include "cpu1.hh"
#include "x86mmu.hh"
#include "../proc/process.hh"
#include "x86_instrument.hh"

#define NUM_FLAG_0 0b10
#define REG_FLAG_0 0b00
#define NUM_FLAG_1 0b01
#define REG_FLAG_1 0b00
#define ADDR_FLAG_0 0b1000000
#define ADDR_FLAG_1 0b0100000
namespace gunplan::cplusplus::machine {
#define WRITE_WB(x, y)         nullwb.w.first = true;\
    nullwb.w.second.first = (x);\
    nullwb.w.second.second = (y);                    \
    return nullwb;
#define SET_NEXT(x)             nullwb.next_add = false;\
    nullwb.next = (x);                                  \
    return nullwb;
#define WRITE_WB_SET_NEXT(x, y, z)   nullwb.w.first = true;\
    nullwb.w.second.first = (x);\
    nullwb.w.second.second = (y);                        \
    nullwb.next_add = false;                               \
    nullwb.next = (z);                                  \
    return nullwb;

    class x86cpu : public cpu1<data_bond> {

        class DE {
        public:
            static decode_result decode(data_bond pc, register_heap *h) {
                decode_result dt{(op_bond) (pc >> 24u), (pc & (0b0100u)) == 0 ? 0 : (pc & (0b1000u)) == 0 ? 1 : 2,
                                 new std::list<oper_code>()};
                if (dt.op_size == 0) {
                    return dt;
                }
                oper_code op;
                op.oper_val = (pc >> 16u) & 0xffu;
                if ((pc & ADDR_FLAG_0) != 0) {
                    op.oper_type |= addr;
                }
                if ((pc & NUM_FLAG_0) != 0) {
                    op.oper_type = num;
                } else {
                    op.oper_type = reg;
                    op.oper_reg = (*h)[op.oper_val];
                    op.oper_val = *(*h)[op.oper_val];
                }
                dt.oplist->push_back(op);
                if (dt.op_size > 1) {
                    op.oper_val = (pc >> 8u) & 0xffu;
                    if ((pc & NUM_FLAG_1) != 0) {
                        op.oper_type = num;
                    } else {
                        op.oper_type = reg;
                        op.oper_reg = (*h)[op.oper_val];
                        op.oper_val = *(*h)[op.oper_val];
                    }
                    dt.oplist->push_back(op);
                }
                return dt;
            }
        };

    private:
        x86mmu mmu;
        m_cpu tss;
        memory *mm = nullptr;
        ex_ret nullwb;

        opFn pushFn = [&](auto val) -> auto {
            push_stack(val->front().oper_val);
            return nullwb;
        };

        opFn popFn = [&](auto val) -> auto {
            cpu_register c = pop_stack();
            if (val->size() == 1) {
                WRITE_WB(val->front().oper_reg, c);
            }
            return nullwb;
        };

// lambda
        opFn callFn = [&](auto val) -> auto {
            push_stack(tss.rip + 1);
            return jmpFn(val);
        };

        opFn retFn = [&](auto val) -> auto {
            int k = pop_stack();
            if (val->size() == 0) {
                SET_NEXT(k);
            }
            WRITE_WB_SET_NEXT(&tss.ecx, val->front().oper_val + 1, k);
        };

        opFn je = [&](auto val) -> auto {
            if (OperatorBitF(tss.bit_flags, CMP_FLAG)) {
                return jmpFn(val);
            }
            return nullwb;
        };

        opFn jne = [&](auto val) -> auto {
            if (!OperatorBitF(tss.bit_flags, CMP_FLAG)) {
                return jmpFn(val);
            }
            return nullwb;
        };

        opFn jmpFn = [&](auto val) -> auto {
            SET_NEXT(val->back().oper_val);
        };

        opFn echoFn = [&](auto val) -> auto {
            for (auto &i:*val) {
                std::cout << i.oper_val;
            }
            std::cout << std::endl;
            return nullwb;
        };

        opFn movFn = [&](auto val) -> auto {
            if (val->back().oper_type == reg) {
                WRITE_WB(val->back().oper_reg, val->front().oper_val);
            }
            return nullwb;
        };

        opFn addFn = [&](auto val) -> auto {
            WRITE_WB(&tss.eax, val->front().oper_val + val->back().oper_val);
        };

        opFn exit = [&](const auto val) -> auto {
            return nullwb;
        };

        opFn cmpFn = [&](const std::list<oper_code> *val) -> auto {
            OperatorBitG(tss.bit_flags, CMP_FLAG, val->front().oper_val == val->back().oper_val);
            OperatorBitG(tss.bit_flags, CMP_G_FLAG, val->front().oper_val > val->back().oper_val);
            return nullwb;
        };

        opFn enterFn = [&](const std::list<oper_code> *val) -> auto {
            push_stack(tss.ebp);
            tss.ebp = tss.esp;
            return nullwb;
        };
        opFn leaveFn = [&](const std::list<oper_code> *val) -> auto {
            tss.esp = tss.ebp;
            tss.ebp = pop_stack();
            return nullwb;
        };

        opFn empFN = [&](const std::list<oper_code> *val) -> auto { return nullwb; };

        opFn subFn = [&](auto val) -> auto {
            WRITE_WB(&tss.ebx, val->front().oper_val - val->back().oper_val);
        };

        opFn jlFn = [&](const auto val) -> auto {
            if (!OperatorBiF0(CMP_FLAG) && !OperatorBiF0(CMP_G_FLAG)) {
                return jmpFn(val);
            }
            return nullwb;
        };

        opFn jgFn = [&](const auto val) -> auto {
            if (!OperatorBiF0(CMP_FLAG) && OperatorBiF0(CMP_G_FLAG)) {
                return jmpFn(val);
            }
            return nullwb;
        };
    public:
        const byte CMP_FLAG = 0;
        const byte CMP_G_FLAG = 0;
        const byte TRAP = 2;
    private:

        register_heap regHeap = {{1, &tss.eax},
                                 {2, &tss.ebx},
                                 {3, &tss.ecx},
                                 {4, &tss.edx},
                                 {5, &tss.esi},
                                 {6, &tss.edi}};

        segment_disruptor *gdt;
        cache_block cache[64]{};


        operatorMap operMap = {{0x0,  empFN},
                               {0xb,  pushFn},
                               {0x1,  popFn},
                               {0x2,  callFn},
                               {0x3,  retFn},
                               {0x4,  movFn},
                               {0x77, echoFn},
                               {0x5,  addFn},
                               {0x6,  jmpFn},
                               {0x7,  je},
                               {0x8,  jne},
                               {0x9,  exit},
                               {0xa,  cmpFn},
                               {0x13, enterFn},
                               {0x10, subFn},
                               {0x11, jlFn},
                               {0x12, jgFn},
                               {0xc,  leaveFn}
        };
    public:

        x86cpu() = default;

        x86cpu(memory *mm);

        ~x86cpu()

        override;

        void push_stack(data_bond val)

        override;

        cpu_register pop_stack()

        override;

        decode_result decode(data_bond pc);

        ex_ret execute(decode_result d);

        int push_process(int pid)

        override;

        int boot(int k);

        void set_resource(memory *mm)
        override;

        void push_stack(data_bond val, cpu_register_segment segment, data_bond offset);

        data_bond pop_stack(cpu_register_segment segment, data_bond offset);


        int write_back(wb_data pair);
    };

}
#endif //MACHINE_CPU1_HH

