//
// Created by dos DRTT on 10/24/20.
//


//
// Created by dos DRTT on 6/13/20.
//



#include <string>
#include "../mm/memory.hh"
#include "process.hh"
#include <tuple>
#include <array>
#include <iostream>
#include <utility>

namespace gunplan::cplusplus::machine {
    task_struct **tsks = new task_struct *[20];


    process::process(buf *task) : task(task) {

    }

    int process::add_process(memory *mm) {
        static int now = 0;
        auto *t = new task_struct{};
        t->m.cs = seg_code_selector;
        t->m.ds = seg_data_selector;
        t->m.ss = seg_stack_selector;
        t->m.es = seg_ex_selector;
        t->m.rip = 0;
        t->m.ebp = 0;
        t->m.esp = 0;
        t->m.rip = std::get<2>(*task);
        t->ldt = mm->load(std::pair<segment_buffer, segment_buffer>(std::get<0>(*task), std::get<1>(*task)));
        tsks[now++] = t;
        pid = now - 1;
        return pid;
    }


    process::~process() {
        delete tsks[pid];
    }

    task_struct *process::get_process(int id) {
        return tsks[id];
    }
}


