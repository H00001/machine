//
// Created by dos DRTT on 10/24/20.
//

#ifndef MACHINE_X86CPU_HH
#define MACHINE_X86CPU_HH


#include <iostream>
#include "../mm/memory.hh"
#include "../util/binary.hh"
#include "../process.hh"
#include "cpu1.hh"
#include "../util/strings.hh"

namespace gunplan::cplusplus::machine {
    class x86cpu : public cpu1 {
    public:
        const byte CMP_FLAG = 0;
        const byte TRAP = 1;
    private:
        m_cpu tss;
        memory *mm = nullptr;
        rHeap regHeap = {{1, &tss.eax},
                         {2, &tss.ebx},
                         {3, &tss.ecx},
                         {4, &tss.edx},
                         {5, &tss.esi},
                         {6, &tss.edi}};

        segment_disruptor *gdt;
        cache_block cache[64]{};
        opFN pushFn = [&](auto val) -> int {
            push_stack(val->front()->oper_val);
            tss.rip++;
            return 0;
        };

        opFN popFn = [&](auto val) -> int {
            if (val->size() == 0) {
                pop_stack();
            }
            tss.rip++;
            return 0;
        };

        // lambda
        opFN callFn = [&](auto val) -> int {
            push_stack((unsigned long) tss.rip + 1);
            tss.ebp = tss.esp;
            tss.rip = val->front()->oper_val;
            return 0;
        };

        opFN retFn = [&](auto val) -> int {
            tss.rip = pop_stack();
            if (val->size() == 1) {
                *regHeap[3] = val->front()->oper_val + 1;
            }
            printf("rip:%d\n", tss.rip);
            if (tss.rip == 32767) {
                // exit
                return -1;
            }
            return 0;
        };

        opFN je = [&](auto val) -> int {
            if (OperatorBitF(tss.bit_flags, CMP_FLAG)) {
                return jmp(val);
            }
            tss.rip++;
            return 0;
        };

        opFN jne = [&](auto val) -> auto {
            if (!OperatorBitF(tss.bit_flags, CMP_FLAG)) {
                return jmp(val);
            }
            tss.rip++;
            return 0;
        };

        opFN jmp = [&](auto val) -> auto {
            tss.cs = val->size() == 2 ? val->front()->oper_val : tss.cs;
            tss.rip = val->back()->oper_val;
            return 0;
        };

        opFN echoFn = [&](auto val) -> auto {
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

        opFN movFn = [&](auto val) -> auto {
            if (val->back()->oper_type == reg) {
                *(val->back()->oper_reg) = val->front()->oper_val;
            }
            tss.rip++;
            return 0;
        };

        opFN add = [&](auto val) -> int {
            int l = val->front()->oper_val;
            int r = val->back()->oper_val;
            tss.eax = r + l;
            tss.rip++;
            return 0;
        };

        opFN exit = [&](const auto val) -> int {
            return val->front()->oper_val;
        };
        opFN cmpFn = [&](const std::list<oper_code *> *val) -> int {
            OperatorBitG(tss.bit_flags, CMP_FLAG, val->front()->oper_val == val->back()->oper_val);
            return 0;
        };

        opFN empFN = [&](const std::list<oper_code *> *val) -> int {
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
                               {0x5,  add},
                               {0x6,  jmp},
                               {0x7,  je},
                               {0x8,  jne},
                               {0x9,  exit},
                               {0xa,  cmpFn}
        };
    public:
        x86cpu() = default;

        x86cpu(memory *mm);

        ~x86cpu() override;

        void push_stack(unsigned long val) override;

        unsigned long pop_stack() override;

        int decode(std::string basicString);

        static std::list<oper_code *> *decode0(std::string &s, rHeap *h) {
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


        static void sp(std::string &s, rHeap *h, std::list<oper_code *> *list) {
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

        static oper_code *operAna(std::string &o, rHeap *h) {
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


        void execute(segment_disruptor *ldt);

        void push_process(std::pair<std::pair<code_buffer, data_buffer>, unsigned long> p) override;

        void set_resource(memory *mm);
    };


}
#endif //MACHINE_CPU1_HH

