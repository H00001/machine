//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_PROCESS_HH
#define MACHINE_PROCESS_HH


#include <string>
#include "../mm/memory.hh"


namespace gunplan::cplusplus::machine {
    using buf = std::pair<std::pair<code_buffer, data_buffer>, unsigned long>;
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
    public:
        explicit process(buf *task);

        int add_process();

        static task_struct *get_process(int id);

        virtual ~process();
    };


}


#endif //MACHINE_PROCESS_HH
