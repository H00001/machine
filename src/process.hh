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
    };

    class process {
        static const int seg_code_selector = 0;
        static const int seg_data_selector = 1;
        static const int seg_stack_selector = 2;
        static const int seg_heap_selector = 3;

    private:
        int now;
        task_struct *tsks[20];
    public:
        void add(seg *tsk) {
            auto *t = new task_struct{};
            t->m.cs = seg_code_selector;
            t->m.ds = seg_data_selector;
            t->m.ss = seg_stack_selector;
            t->m.hs = seg_heap_selector;
            t->m.rip = 0;
            t->m.ebp = 0;
            t->m.esp = 0;
            t->m.seg_divide = new std::map<cpu_register, LDT *>();
            (*(t->m.seg_divide))[t->m.cs] = new LDT{tsk->code, 0, tsk->clen};
            (*(t->m.seg_divide))[t->m.ds] = new LDT{tsk->data, 0, tsk->dlen};
            (*(t->m.seg_divide))[t->m.ss] = new LDT{new unsigned long[200], 0, 200};
            (*(t->m.seg_divide))[t->m.hs] = new LDT{new unsigned long[200], 0, 200};

            t->m.func_map = new std::map<std::string, int>();

            for (int i = 0;; ++i) {
                std::string s = *((std::string *) (memory::transfer<std::string *>(t->m.seg_divide, t->m.cs, i)));
                if (s == "END") {
                    break;
                }
                if (s == "__start:") {
                    t->m.rip = i;
                }
                if (s.ends_with(":")) {
                    (*(t->m.func_map))[(*(tsk->code + i)).substr(0, (*(tsk->code + i)).length() - 1)] = i;
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
