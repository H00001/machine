//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_PROCESS_HH
#define MACHINE_PROCESS_HH


#include <string>
#include "memory.hh"

struct task_struct {
    m_cpu m;
};

class process {

private:
    int now;
    task_struct *tsks[20];
public:
    void add(seg *tsk) {
        auto *t = new task_struct{};
        t->m.cs = 0;
        t->m.ds = 1;
        t->m.rip = 0;
        t->m.ss = 2;
        t->m.ebp = 0;
        t->m.esp = 0;
        t->m.seg_divide = new std::map<cpu_register, LDT *>();
        (*(t->m.seg_divide))[0] = new LDT{tsk->code, 0, tsk->clen};
        (*(t->m.seg_divide))[1] = new LDT{tsk->data, 0, tsk->dlen};
        (*(t->m.seg_divide))[2] = new LDT{new unsigned long[200], 0, 200};
        t->m.flages = new std::map<std::string, int>();

        for (int i = 0;; ++i) {
            std::string s = *((std::string *) (memory::transfer<std::string *>(t->m.seg_divide, t->m.cs, i)));
            if (s == "END") {
                break;
            }
            if (s == "__start:") {
                t->m.rip = i;
            }
            if (s.ends_with(":")) {
                (*(t->m.flages))[(*(tsk->code + i)).substr(0, (*(tsk->code + i)).length() - 1)] = i;
            }

        }

        tsks[now++] = t;
    }

    task_struct *getProcess() {
        return tsks[0];
    }
};


#endif //MACHINE_PROCESS_HH
