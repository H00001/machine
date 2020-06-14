
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
    std::map<std::string, int> flages;
    std::map<std::string, unsigned long *> regHeap = {{"eax", &tss.eax},
                                                      {"ebx", &tss.ebx},
                                                      {"ecx", &tss.ecx},
                                                      {"edx", &tss.edx}};

    opFN pushFn = [&](const std::string &val) {
        unsigned long k = 0;
        if (val.starts_with("%")) {
            k = stoi(val.substr(1, val.length()));
        }
        std::cout << "push:" << k << std::endl;
        pushStack(k);
    };
    opFN popFn = [&](const std::string &val) {
        if (val.length() == 0) {
            popStack();
        }
    };
    std::map<std::string, std::function<void(std::string)>> operMap = {{"push", pushFn},
                                                                       {"pop",  popFn}

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

    memory *getMm() const {
        return mm;
    }

    void setMm(memory *mm) {
        cpu::mm = mm;
    }

    process *getPc() const {
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

    void setPc(process *pc) {
        cpu::pc = pc;
    }

    void analyizer(std::string basicString) {
        auto oper = strings::spilt(basicString, " ")[0];
        auto val = strings::spilt(basicString, " ")[1];
        if (operMap.count(oper) == 1) {
            operMap[oper](val);
        }
        if (basicString.starts_with("mov")) {

        } else {
            std::cout << tss.pc << std::endl;
        }
    }

    void execute() {
        bool inmain = false;
        while (true) {
            tss.pc = (*(tss.rip + tss.cs));

            if (tss.pc == "__start:") {
                inmain = true;
            }
            if (!inmain && tss.pc.ends_with(":")) {
                flages[tss.pc.substr(0, tss.pc.length() - 1)] = tss.rip;
            }

            if (inmain) {
                if (tss.pc.starts_with("fun") && tss.pc.ends_with("()")) {
                    pushStack((unsigned long) tss.rip + 1);
                    tss.rip = flages[tss.pc.substr(0, tss.pc.length() - 2)] + 1;
                    continue;
                }
                if (tss.pc == "ret") {
                    tss.rip = popStack();
                    continue;
                }
                analyizer(tss.pc);

                if (tss.pc == "END") {
                    break;
                }
            }

            tss.rip++;
        }
    }

    void PushProcess(const std::string &s) {
        auto t = mm->load(s);
        pc->add(t);
        tss.rip = pc->getProcess()->m.rip;
        tss.esp = pc->getProcess()->m.esp;
        tss.ebp = pc->getProcess()->m.ebp;
        tss.cs = pc->getProcess()->m.cs;
        execute();
    }

};


#endif //MACHINE_CPU_HH