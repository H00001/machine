//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_PROCESS_HH
#define MACHINE_PROCESS_HH


#include <string>
#include "mm/memory.hh"


namespace gunplan::cplusplus::machine {
    struct task_struct {
        m_cpu m;
        segment_disruptor *ldt{};
    };

    class process {
        const int seg_code_selector = 0;
        const int seg_data_selector = 1;
        const int seg_stack_selector = 2;
        const int seg_ex_selector = 3;

    private:
        int now = 0;
        task_struct *tsks[20]{};
    public:
        int add_process(seg *tsk) {
            auto *t = new task_struct{};
            t->m.cs = seg_code_selector;
            t->m.ds = seg_data_selector;
            t->m.ss = seg_stack_selector;
            t->m.es = seg_ex_selector;
            t->m.rip = 0;
            t->m.ebp = 0;
            t->m.esp = 0;
            t->m.rip = tsk->ip;
            t->ldt = new segment_disruptor[4];
            t->ldt[0] = segment_disruptor{tsk->code >> 8, tsk->code_len, 0};
            t->ldt[1] = segment_disruptor{tsk->data >> 8, tsk->data_len, 0};
            t->ldt[2] = segment_disruptor{tsk->stack >> 8, tsk->stack_len, 0};
            tsks[now++] = t;
            return now;
        }

        task_struct *getProcess(int id) {
            return tsks[id];
        }
    };
}

#endif //MACHINE_PROCESS_HH
