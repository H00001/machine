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
        t->m.ss = new unsigned long[100];
        t->m.ebp = 0;
        t->m.esp = 0;
        t->m.flages = new std::map<std::string, int>();

        for (int i = 0; *(tsk + i) != "END"; ++i) {
            if ((*(tsk + i)) == "__start:") {
                t->m.rip = i;
            }
            if ((*(tsk + i)).ends_with(":")) {
                (*(t->m.flages))[(*(tsk + i)).substr(0, (*(tsk + i)).length() - 1)] = i;
            }

        }

        tsks[now++] = t;
    }

    task_struct *getProcess() {
        return tsks[0];
    }
};


#endif //MACHINE_PROCESS_HH
