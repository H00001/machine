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
#include "../util/strings.hh"
#include "x86mmu.hh"
#include "x86_instrument.hh"

#define NUM_FLAG_0 0b10
#define REG_FLAG_0 0b00
#define NUM_FLAG_1 0b01
#define REG_FLAG_1 0b00
#define ADDR_FLAG_0 0b1000000
#define ADDR_FLAG_1 0b0100000
namespace gunplan::cplusplus::machine {
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

        opFn pushFn = [&](auto val) -> int {
            push_stack(val->front().oper_val);
            tss.rip++;
            return 0;
        };

        opFn popFn = [&](auto val) -> int {
            cpu_register c = pop_stack();
            if (val->size() == 1) {
                *(val->front().oper_reg) = c;
            }
            tss.rip++;
            return 0;
        };

// lambda
        opFn callFn = [&](auto val) -> int {
            push_stack(tss.rip + 1);
            tss.rip = val->front().oper_val;
            return 0;
        };

        opFn retFn = [&](auto val) -> int {
            tss.rip = pop_stack();
            if (val->size() == 1) {
                *regHeap[3] = val->front().oper_val + 1;
            }
            if (tss.rip == 0) {
                return -1;
            }
            return 0;
        };

        opFn je = [&](auto val) -> int {
            if (OperatorBitF(tss.bit_flags, CMP_FLAG)) {
                return jmpFn(val);
            }
            tss.rip++;
            return 0;
        };

        opFn jne = [&](auto val) -> auto {
            if (!OperatorBitF(tss.bit_flags, CMP_FLAG)) {
                return jmpFn(val);
            }
            tss.rip++;
            return 0;
        };

        opFn jmpFn = [&](auto val) -> int {
            tss.cs = val->size() == 2 ? val->front().oper_val : tss.cs;
            tss.rip = val->back().oper_val;
            return 0;
        };

        opFn echoFn = [&](auto val) -> auto {
            for (auto &i:*val) {
                std::cout << i.oper_val;
            }
            std::cout << std::endl;
            tss.rip++;
            return 0;
        };

        opFn movFn = [&](auto val) -> auto {
            if (val->back().oper_type == reg) {
                *(val->back().oper_reg) = val->front().oper_val;
            }
            tss.rip++;
            return 0;
        };

        opFn addFn = [&](auto val) -> int {
            cpu_register l = val->front().oper_val;
            cpu_register r = val->back().oper_val;
            tss.eax = r + l;
            tss.rip++;
            return 0;
        };

        opFn exit = [&](const auto val) -> int {
            return val->front().oper_val;
        };
        opFn cmpFn = [&](const std::list<oper_code> *val) -> int {
            OperatorBitG(tss.bit_flags, CMP_FLAG, val->front().oper_val == val->back().oper_val);
            OperatorBitG(tss.bit_flags, CMP_G_FLAG, val->front().oper_val > val->back().oper_val);
            return 0;
        };

        opFn enterFn = [&](const std::list<oper_code> *val) -> int {
            push_stack(tss.ebp);
            tss.ebp = tss.esp;
            tss.rip++;
            return 0;
        };
        opFn leaveFn = [&](const std::list<oper_code> *val) -> int {
            tss.esp = tss.ebp;
            tss.ebp = pop_stack();
            tss.rip++;
            return 0;
        };

        opFn empFN = [&](const std::list<oper_code> *val) -> int {
            tss.rip++;
            return -1;
        };

        opFn subFn = [&](const std::list<oper_code> *val) -> int {
            tss.ebx = val->front().oper_val - val->back().oper_val;
            tss.rip++;
            return 0;
        };

        opFn jlFn = [&](const auto val) -> int {
            if (!OperatorBiF0(CMP_FLAG) && !OperatorBiF0(CMP_G_FLAG)) {
                return jmpFn(val);
            }
            tss.rip++;
            return 0;
        };

        opFn jgFn = [&](const auto val) -> int {
            if (!OperatorBiF0(CMP_FLAG) && OperatorBiF0(CMP_G_FLAG)) {
                return jmpFn(val);
            }
            tss.rip++;
            return 0;
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

        ~x86cpu() override;

        void push_stack(data_bond val) override;

        cpu_register pop_stack() override;

        decode_result decode(data_bond pc);

        int execute(decode_result d);

        int push_process(int pid) override;

        int boot();

        void set_resource(memory *mm) override;

        void push_stack(data_bond val, cpu_register_segment segment, data_bond offset);

        data_bond pop_stack(cpu_register_segment segment, data_bond offset);


    };

}
#endif //MACHINE_CPU1_HH

