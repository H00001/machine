
//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_CPU_HH
#define MACHINE_CPU_HH

#include <iostream>
#include <string>
#include "memory.hh"
#include "process.hh"
#include <map>
#include "binary.hh"
#include "common.hh"
#include "compile/ProgramCompile.hh"

namespace gunplan::cplusplus::machine {
    class cpu {
    public:
        const byte CMP_FLAG = 0;
        const byte TRAP = 1;
    private:
        m_cpu tss;
        memory *mm = nullptr;
        process *pc = nullptr;
        rHeap regHeap = {{1, &tss.eax},
                         {2, &tss.ebx},
                         {3, &tss.ecx},
                         {4, &tss.edx},
                         {5, &tss.esi},
                         {6, &tss.edi}};

        segment_disruptor *gdt;
        cache_block cache[64]{};
        opFN pushFn = [&](auto val) -> int {
            pushStack(val->front()->oper_val);
            return 0;
        };

        opFN popFn = [&](auto val) -> int {
            if (val->size() == 0) {
                popStack();
            }
            return 0;
        };

        // lambda
        opFN callFn = [&](auto val) -> int {
            pushStack((unsigned long) tss.rip);
            tss.rip = val->front()->oper_val;
            return 0;
        };

        opFN ret = [&](auto val) -> int {
            tss.rip = popStack();
            if (val->size() == 1) {
                *regHeap["ecx"] = val->front()->oper_val;
            }
            return 0;
        };

        opFN je = [&](auto val) -> int {
            if (OperatorBitF(tss.bit_flags, CMP_FLAG)) {
                return jmp(val);
            }
            return 0;
        };

        opFN jne = [&](auto val) -> auto {
            if (!OperatorBitF(tss.bit_flags, CMP_FLAG)) {
                return jmp(val);
            }
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
                    std::cout << i->oper_str << std::endl;
                } else if (i->oper_type == reg) {
                    std::cout << i->oper_val << std::endl;
                }
            }
            return 0;
        };

        opFN movFn = [&](auto val) -> auto {
            int l_mv_r;
            if (val->back()->oper_type == reg) {
                *(val->back()->oper_reg) = val->front()->oper_val;
            }
            return 0;
        };

        opFN add = [&](auto val) -> int {
            int l = val->front()->oper_val;
            int r = val->back()->oper_val;
            tss.eax = r + l;
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
            return 0;
        };

        operatorMap operMap = {{0x0,  empFN},
                               {0xb,  pushFn},
                               {0x1,  popFn},
                               {0x2,  callFn},
                               {0x3,  ret},
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

        cpu(memory
            *mm,
            process *pc
        ) :

                mm(mm), pc(pc) {
            gdt = new segment_disruptor[20];
        }

        virtual ~cpu() {
            delete mm;
            delete pc;
            delete gdt;
        }

        [[nodiscard]] const m_cpu &getTss() const {
            return tss;
        }

        [[nodiscard]] memory *getMm() const {
            return mm;
        }

        void pushStack(unsigned long val) {
            mm->pushStack(val, tss.ldt_cache, tss.ss, tss.esp);
            tss.esp++;
        }

        unsigned long popStack() {
            auto data = mm->popStack(tss.ldt_cache, tss.ss, tss.esp);
            tss.esp--;
            return data;
        }


        int decode(std::string basicString) {
            auto oper = stoi(strings::spilt(basicString)[0]);
            auto val = strings::spilt(basicString)[1];
            int r;
            auto o = decode0(val, &regHeap);
            if ((operMap.count(oper) == 1)) {
                r = operMap[oper](o);
            }
            // to do exit
            if (oper == 0x9) {
                return -1;
            }
            delete o;
            return r;
        }

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
                op->oper_reg_name = (o.substr(1, o.length()));
                op->oper_val = *(*h)[op->oper_reg_name];
                op->oper_reg = (*h)[op->oper_reg_name];
            } else {
                op->oper_type = str;
            }
            return op;
        }


        void execute(segment_disruptor *ldt) {
            tss.ldt_cache = ldt;
            while (true) {
                tss.pc = memory::hd_code_mem[memory::transfer(tss.ldt_cache, segment_selector{tss.cs},
                                                              tss.rip - 1)];
                if ((decode(tss.pc) != 0)) {
                    break;
                }
                tss.rip++;
            }
        }

        void PushProcess(std::pair<std::pair<code_buffer, data_buffer>, unsigned long> p) {
            pc->add_process(mm->load(p));
            task_struct *next = pc->getProcess();
            memmove(&tss, next, sizeof(task_struct));
            execute(next->ldt);
        }

    };


}
#endif //MACHINE_CPU_HH
