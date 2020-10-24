//
// Created by dos DRTT on 10/24/20.
//


//
// Created by dos DRTT on 6/13/20.
//



#include <string>
#include "../mm/memory.hh"
#include "process.hh"


namespace gunplan::cplusplus::machine {
    task_struct **tsks = new task_struct *[20];


    process::process(buf *task) : task(task) {

    }

    int process::add_process() {
        static int now = 0;

        auto *t = new task_struct{};
        t->m.cs = seg_code_selector;
        t->m.ds = seg_data_selector;
        t->m.ss = seg_stack_selector;
        t->m.es = seg_ex_selector;
        t->m.rip = 0;
        t->m.ebp = 0;
        t->m.esp = 0;
        t->m.rip = task->second;
        t->ldt = memory::load(task->first);
        tsks[now++] = t;
        return now - 1;
    }


    process::~process() {
        delete task;
    }

    task_struct *process::get_process(int id) {
        return tsks[id];
    }
}


