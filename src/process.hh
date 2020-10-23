//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_PROCESS_HH
#define MACHINE_PROCESS_HH


#include <string>
#include "memory.hh"


namespace gunplan::cplusplus::machine {
    struct task_struct {
        m_cpu m;
        segment_disruptor *ldt;
    };

    class process {
        const int seg_code_selector = 0;
        const int seg_data_selector = 1;
        const int seg_stack_selector = 2;
        const int seg_ex_selector = 3;

    private:
        int now = 0;
        task_struct *tsks[20];
    public:
        void add_process(seg *tsk) {
            auto *t = new task_struct{};
            t->m.cs = seg_code_selector;
            t->m.ds = seg_data_selector;
            t->m.ss = seg_stack_selector;
            t->m.es = seg_ex_selector;
            t->m.rip = 0;
            t->m.ebp = 0;
            t->m.esp = 0;
            t->ldt = new segment_disruptor[4];
            t->ldt[0] = segment_disruptor{tsk->code >> 12,tsk->code_len,0};
            t->ldt[1] = segment_disruptor{tsk->data >> 12,tsk->data_len,0};
            t->ldt[2] = segment_disruptor{tsk->stack >> 12,tsk->stack_len,0};


            for (int i = 0;; ++i) {
                std::string s = memory::hd_code_mem[memory::transfer(t->ldt,segment_selector{t->m.cs}, i)];
                if (s == "END") {
                    break;
                }
                if (s == "__start:") {
                    t->m.rip = i;
                }
                if (s.ends_with(":")) {

                }

            }
            tsks[now++] = t;
        }

        task_struct *getProcess() {
            return tsks[0];
        }
    };
}

#endif //MACHINE_PROCESS_HH
