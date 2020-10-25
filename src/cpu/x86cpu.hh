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

namespace gunplan::cplusplus::machine {
    using cpu_register = unsigned int;
    using cpu_register_point = cpu_register *;
    using decode_register = unsigned short;
#define instrument_length  sizeof(cpu_register)

    struct oper_code {
        cpu_register_point oper_reg;
        std::string oper_str;
        decode_register oper_reg_name;
        cpu_register oper_val{};
        operaType oper_type;
    };



    using opFn = std::function<int(std::list<oper_code *> *)>;
    using register_heap = std::map<decode_register, cpu_register *>;
    using operatorMap = std::map<int, opFn>;

    struct m_cpu;

    struct m_cpu {
        cpu_register eax, ebx, ecx, edx;
        cpu_register cs, ss, ds, es, fs, hs;
        cpu_register esi, edi, esp, ebp;
        cpu_register rs0, rs1, rs2;
        cpu_register bit_flags;
        unsigned long rip;
        std::string pc;
        segment_disruptor *ldt_cache;
#ifdef _vector_
        cpu_register t0, t1, t2, t3, t4, t5, t6, t7;
        cpu_register r0, r1, r2, r3, r4, r5, r6, r7;
#endif

    };


    class x86cpu : public cpu1<cpu_register> {
    public:
        const byte CMP_FLAG = 0;
        const byte TRAP = 1;
    private:
        m_cpu tss;
        memory *mm = nullptr;
        register_heap regHeap = {{1, &tss.eax},
                                 {2, &tss.ebx},
                                 {3, &tss.ecx},
                                 {4, &tss.edx},
                                 {5, &tss.esi},
                                 {6, &tss.edi}};

        segment_disruptor *gdt;
        cache_block cache[64]{};
        opFn pushFn = [&](auto val) -> int {
            push_stack(val->front()->oper_val);
            tss.rip++;
            return 0;
        };

        opFn popFn = [&](auto val) -> int {
            cpu_register c = pop_stack();
            if (val->size() == 1) {
                *(val->front()->oper_reg) = c;
            }
            tss.rip++;
            return 0;
        };

        // lambda
        opFn callFn = [&](auto val) -> int {
            push_stack((unsigned long) tss.rip + 1);
            tss.ebp = tss.esp;
            tss.rip = val->front()->oper_val;
            return 0;
        };

        opFn retFn = [&](auto val) -> int {
            tss.rip = pop_stack();
            if (val->size() == 1) {
                *regHeap[3] = val->front()->oper_val + 1;
            }
            std::cout << "pop:" << tss.rip << std::endl;
            if (tss.rip == 32767) {
                // exit
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

        opFn jmpFn = [&](auto val) -> auto {
            tss.cs = val->size() == 2 ? val->front()->oper_val : tss.cs;
            tss.rip = val->back()->oper_val;
            return 0;
        };

        opFn echoFn = [&](auto val) -> auto {
            for (auto &i:*val) {
                if (i->oper_type == str) {
                    std::cout << i->oper_str;
                } else if (i->oper_type == reg) {
                    std::cout << i->oper_val;
                }
            }
            std::cout << std::endl;
            tss.rip++;
            return 0;
        };

        opFn movFn = [&](auto val) -> auto {
            if (val->back()->oper_type == reg) {
                *(val->back()->oper_reg) = val->front()->oper_val;
            }
            tss.rip++;
            return 0;
        };

        opFn addFn = [&](auto val) -> int {
            cpu_register l = val->front()->oper_val;
            cpu_register r = val->back()->oper_val;
            tss.eax = r + l;
            tss.rip++;
            return 0;
        };

        opFn exit = [&](const auto val) -> int {
            return val->front()->oper_val;
        };
        opFn cmpFn = [&](const std::list<oper_code *> *val) -> int {
            OperatorBitG(tss.bit_flags, CMP_FLAG, val->front()->oper_val == val->back()->oper_val);
            return 0;
        };

        opFn enterFn = [&](const std::list<oper_code *> *val) -> int {
            push_stack(tss.ebp);
            tss.ebp = tss.esp;
            tss.rip++;
            return 0;
        };
        opFn leaveFn = [&](const std::list<oper_code *> *val) -> int {
            tss.esp = tss.ebp;
            tss.ebp = pop_stack();
            tss.rip++;
            return 0;
        };

        opFn empFN = [&](const std::list<oper_code *> *val) -> int {
            tss.rip++;
            return 0;
        };


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
                               {0xb,  enterFn},
                               {0xc,  leaveFn}
        };
    public:
        x86cpu() = default;

        x86cpu(memory *mm);

        ~x86cpu() override;

        void push_stack(unsigned long val) override;

        cpu_register pop_stack() override;

        int decode(std::string basicString);

        static std::list<oper_code *> *decode0(std::string &s, register_heap *h) {
            auto *list = new std::list<oper_code *>;
            if (strings::trim(s).empty()) {
                return list;
            }
            int opIndex = s.find(',');
            if (opIndex < 0) {
                list->push_back(operAna(s, h));
            } else {
                sp(s, h, list);
            }
            return list;
        }


        static void sp(std::string &s, register_heap *h, std::list<oper_code *> *list) {
            auto op = strings::spilt_reg(s);
            list->push_back(operAna(op[0], h));
            auto c = operAna(op[1], h);
            int k = c->oper_str.find(',');
            if (k > 0) {
                sp(c->oper_str, h, list);
            } else {
                list->push_back(c);
            }
        }

        static oper_code *operAna(std::string &o, register_heap *h) {
            auto *op = new oper_code;
            char flag = o.c_str()[0];
            op->oper_str = o;
            if (flag == '%') {
                op->oper_type = num;
                op->oper_val = std::stoi(o.substr(1, o.length()));
            } else if (flag == '$') {
                op->oper_type = reg;
                op->oper_reg_name = stoi((o.substr(1, o.length())));
                op->oper_val = *(*h)[op->oper_reg_name];
                op->oper_reg = (*h)[op->oper_reg_name];
            } else {
                op->oper_type = str;
            }
            return op;
        }


        int execute();

        int push_process(int pid) override;

        void set_resource(memory *mm) override;
    };

}
#endif //MACHINE_CPU1_HH

