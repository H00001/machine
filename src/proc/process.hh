//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_PROCESS_HH
#define MACHINE_PROCESS_HH


#include <string>
#include "../mm/memory.hh"
#include "../cpu/x86cpu.hh"


namespace gunplan::cplusplus::machine {
    using buf = std::tuple<segment_buffer, segment_buffer, unsigned int>;
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
        buf *task;
        int pid;
    public:
        explicit process(buf *task);

        int add_process(memory *mm);

        static task_struct *get_process(int id);

        virtual ~process();
    };


}


#endif //MACHINE_PROCESS_HH
