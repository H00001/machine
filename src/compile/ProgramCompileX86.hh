//
// Created by dos DRTT on 10/24/20.
//

#include <fstream>
#include <map>
#include "ProgramCompile.hh"

class ProgramCompileX86 : public ProgramCompile {
private:
    using compile_code = std::map<std::string, int>;
    using compile_register = std::map<std::string, int>;
    compile_code instrument_table = {{"push",        0xb},
                                     {"pop",         0x1},
                                     {"call",        0x2},
                                     {"ret",         0x3},
                                     {"mov",         0x4},
                                     {"echo",        0x77},
                                     {"add_process", 0x5},
                                     {"jmp",         0x6},
                                     {"je",          0x7},
                                     {"jne",         0x8},
                                     {"exit",        0x9},
                                     {"cmp",         0xa},
    };

    compile_register reg_replace = {{"eax", 0x1},
                                    {"ebx", 0x2},
                                    {"ecx", 0x3},
                                    {"edx", 0x4},
                                    {"esi", 0x5},
                                    {"edi", 0x6}};

public:
    std::pair<code_buffer, data_buffer> compile_load(std::string file_name) {
        byte *hd_mem = new byte[mm_size];
        std::string *hd_code_mem = new std::string[200];
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

    unsigned int compile(std::pair<code_buffer, data_buffer> p) {
        compile_data_segment(p.second.b, p.second.length);
        return compile_code_segment(p.first.b, p.first.length);
    }

    void rewrite_to_file(std::string file_name, std::string *b, int len) {
        std::ofstream os(file_name);
        for (int i = 0; i < len; ++i) {
            os << b[i] << std::endl;
        }
        os.close();

    }


private:
    unsigned int compile_code_segment(std::string *base, int length) {
        unsigned int ip = 0;
        bool in_main = false;
        bool end_ret = true;
        std::map<std::string, offset> addr_map;
        for (int i = 0; i < length; ++i) {
            std::string s = strings::trim(base[i]);
            if (s.empty()) {
                base[i] = "0 %0";
                continue;
            }
            if (s.starts_with("ret") && in_main && !end_ret) {
                base[i].replace(0, 3, "exit");
                end_ret = true;
            }
            if (base[i].ends_with(":")) {
                addr_map.insert(std::map<std::string, int>::value_type(base[i], i));
                in_main = !end_ret;
                base[i] = "0 %0";
            } else {
                auto div = strings::spilt(base[i]);
                base[i].replace(0, div[0].length(), std::to_string(instrument_table[div[0]]));
            }

            if (s.starts_with("__start:")) {
                ip = i;
                in_main = true;
                end_ret = false;
            }
        }
        for (int i = 0; i < length; ++i) {

            if (base[i].starts_with("7") || base[i].starts_with("8") || base[i].starts_with("6") ||
                base[i].starts_with("2")) {
                auto v = strings::spilt(base[i]);
                base[i] = v[0] + " %" + std::to_string(addr_map[v[1] + ":"]);
            }
        }
        return ip;
    }

    unsigned int compile_data_segment(byte *base, int length) {

    }
};