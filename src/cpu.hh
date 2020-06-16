
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
#include "util.hh"
#include "binary.hh"

namespace gunplan::cplusplus::machine {

    class cpu {
    private:
        m_cpu tss;
        memory *mm = nullptr;
        process *pc = nullptr;
        rHeap regHeap = {{"eax", &tss.eax},
                         {"ebx", &tss.ebx},
                         {"ecx", &tss.ecx},
                         {"edx", &tss.edx},
                         {"esi", &tss.esi},
                         {"edi", &tss.edi}};

        LDT *gdt[20];
        cache_block cache[64];
        opFN pushFn = [&](const std::string &val) {
            unsigned long k = 0;
            if (val.starts_with("%")) {
                k = stoi(val.substr(1, val.length()));
            }
            pushStack(k);
        };

        opFN popFn = [&](const std::string &val) {
            if (val.length() == 0) {
                popStack();
            }
        };

        // lambda
        opFN callFn = [&](const std::string &val) {
            pushStack((unsigned long) tss.rip);
            tss.rip = (*tss.func_map)[val];
        };

        opFN ret = [&](const std::string &val) {
            tss.rip = popStack();
        };

        opFN je = [&](std::string val) {
            auto o = strings::Analyze(val, &regHeap);
            if (o->size() == 1) {
                if (OperatorBitF0(tss.bit_flags)) {
                    jmp(val);
                }
            } else {
                if (o->front()->operval == 0) {
                    jmp(o->back()->operStr);
                }
            }
        };

        opFN jne = [&](std::string val) {
            auto o = strings::Analyze(val, &regHeap);
            if (o->size() == 1) {
                if (!OperatorBitF0(tss.bit_flags)) {
                    jmp(val);
                }
            } else {
                if (o->front()->operval != 0) {
                    jmp(o->back()->operStr);
                }
            }
        };

        opFN jmp = [&](std::string val) {
            auto o = strings::Analyze(val, &regHeap);
            // this is a bug operval is it not ul
            tss.cs = o->size() == 2 ? o->front()->operval : tss.cs;
            if (o->back()->type == str) {
                tss.rip = (*tss.func_map)[o->back()->operStr];
            } else {
                tss.rip = o->back()->operval;
            }
        };

        opFN echoFn = [&](std::string val) {
            auto o = strings::Analyze(val, &regHeap);
            for (auto &i:*o) {
                if (i->type == str) {
                    std::cout << i->operStr << std::endl;
                } else if (i->type == reg) {
                    std::cout << *regHeap[i->operReg] << std::endl;
                }
            }
        };

        opFN movFn = [&](std::string val) {
            auto o = strings::Analyze(val, &regHeap);
            int l_mv_r;
            if (o->front()->type == num) {
                l_mv_r = o->front()->operval;
            }
            if (o->back()->type == reg) {
                *regHeap[o->back()->operReg] = l_mv_r;
            }
        };

        opFN add = [&](std::string val) {
            auto o = strings::Analyze(val, &regHeap);
            int l = o->front()->operval;
            int r = o->back()->operval;
            tss.eax = r + l;
        };

        operatorMap operMap = {{"push", pushFn},
                               {"pop",  popFn},
                               {"call", callFn},
                               {"ret",  ret},
                               {"mov",  movFn},
                               {"echo", echoFn},
                               {"add",  add},
                               {"jmp",  jmp},
                               {"je",   je},
                               {"jne",  jne}
        };
    public:

        cpu(memory *mm, process *pc) : mm(mm), pc(pc) {}

        virtual ~cpu() {
            delete mm;
            delete pc;
        }

        [[nodiscard]] const m_cpu &getTss() const {
            return tss;
        }

        void setTss(const m_cpu &tss) {
            cpu::tss = tss;
        }

        [[nodiscard]] memory *getMm() const {
            return mm;
        }

        void pushStack(unsigned long val) {
            *(memory::transfer<cpu_register *>(tss.seg_divide, tss.ss, tss.esp)) = val;
            tss.esp++;
        }

        unsigned long popStack() {
            auto lang = *(memory::transfer<cpu_register *>(tss.seg_divide, tss.ss, tss.esp - 1));
            tss.esp--;
            return lang;
        }

        void setPc(process *p) {
            cpu::pc = p;
        }

        void analyizer(std::string basicString) {
            auto oper = strings::spilt(basicString, " ")[0];
            auto val = strings::spilt(basicString, " ")[1];
            if (operMap.count(oper) == 1) {
                operMap[oper](val);
            }
        }

        void execute() {
            while (true) {
                tss.pc = *(memory::transfer<std::string *>(tss.seg_divide, tss.cs, tss.rip - 1));
                if (tss.pc == "END") {
                    break;
                }
                analyizer(tss.pc);
                tss.rip++;
            }
        }

        void PushProcess(const std::string &s) {
            pc->add(mm->load(s));
            task_struct *next = pc->getProcess();
            memmove(&tss, next, sizeof(task_struct));
            execute();
        }

    };

}
#endif //MACHINE_CPU_HH
