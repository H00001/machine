
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

using opFN = std::function<void(std::string)>;

class cpu {
private:
    m_cpu tss;
    memory *mm = nullptr;
    process *pc = nullptr;
    std::map<std::string, unsigned long *> regHeap = {{"eax", &tss.eax},
                                                      {"ebx", &tss.ebx},
                                                      {"ecx", &tss.ecx},
                                                      {"edx", &tss.edx}};

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
    opFN callFn = [&](const std::string &val) {
        pushStack((unsigned long) tss.rip);
        tss.rip = (*tss.flages)[val];
    };

    opFN ret = [&](const std::string &val) {
        tss.rip = popStack();
    };


    opFN echoFn = [&](const std::string &val) {
        auto k = val;
        auto o = strings::Analyze(k);
        for (auto &i:*o) {
            if (i->type == str) {
                std::cout << i->operStr << std::endl;
            } else if (i->type == reg) {
                std::cout << *regHeap[i->operReg] << std::endl;
            }
        }
    };
    opFN movFn = [&](const std::string &val) {
        auto k = val;
        auto o = strings::Analyze(k);
        int l_mv_r;
        if (o->front()->type == num) {
            l_mv_r = o->front()->operval;
        }
        if (o->back()->type == reg) {
            *regHeap[o->back()->operReg] = l_mv_r;
        }
    };

    std::map<std::string, std::function<void(std::string)>> operMap = {{"push", pushFn},
                                                                       {"pop",  popFn},
                                                                       {"call", callFn},
                                                                       {"ret",  ret},
                                                                       {"mov",  movFn},
                                                                       {"echo", echoFn}

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

    void setMm(memory *mm) {
        cpu::mm = mm;
    }

    [[nodiscard]] process *getPc() const {
        return pc;
    }

    void pushStack(unsigned long val) {
        *tss.esp = val;
        tss.esp++;
    }

    unsigned long popStack() {
        auto lang = *(tss.esp - 1);
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
            tss.pc = (*(tss.rip + tss.cs));
            if (tss.pc == "END") {
                break;
            }
            analyizer(tss.pc);
            tss.rip++;
        }
    }

    void PushProcess(const std::string &s) {
        auto t = mm->load(s);
        pc->add(t);
        task_struct *next = pc->getProcess();
        tss.rip = next->m.rip;
        tss.esp = next->m.esp;
        tss.ebp = next->m.ebp;
        tss.cs = next->m.cs;
        tss.flages = next->m.flages;
        execute();
    }

};


#endif //MACHINE_CPU_HH
