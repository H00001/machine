//
// Created by dos DRTT on 10/24/20.
//

#include "program_compile_x86.hh"

std::pair<code_buffer, data_buffer> program_compile_x86::compile_load(std::string file_name) {
    byte *hd_mem = new byte[mm_size];
    auto *hd_code_mem = new std::string[200];
    int pos = -1;
    auto *d_buff = new std::string[120];
    std::ifstream in(file_name);
    std::string line;
    int ldata = 0;
    int lcode = 0;
    while (getline(in, line)) {
        if (line == "@data") {
            pos = 0;
        } else if (line == "@code") {
            pos = 1;
        } else if (pos == 0) {
            d_buff[ldata] = line;
            ldata++;
        } else if (pos == 1) {
            hd_code_mem[lcode] = line;
            lcode++;
        }
    }
    return std::pair<code_buffer, data_buffer>(code_buffer{hd_code_mem, lcode}, data_buffer{hd_mem, ldata});
}