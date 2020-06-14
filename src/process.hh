//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_PROCESS_HH
#define MACHINE_PROCESS_HH


#include <string>
#include "base.hh"

struct task_struct {
    m_cpu m;
};

class process {

private:
    int now;
    task_struct *tsks[20];
public:
    void add(std::string *tsk) {
        auto *t = new task_struct{};
        t->m.cs = (tsk);
        t->m.rip = 0;
        t->m.ebp = new unsigned long[20];
        t->m.esp = t->m.ebp;

        tsks[now++] = t;
    }

    task_struct *getProcess() {
        return tsks[0];
    }
};


#endif //MACHINE_PROCESS_HH
