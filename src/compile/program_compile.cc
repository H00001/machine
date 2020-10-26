//
// Created by dos DRTT on 10/24/20.
//

#include "program_compile_x86.hh"
#include "write_instrument.hh"

ret program_compile_x86::compile_load(std::string file_name) {
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
    int ddlen;
    b = new word[lcode]{0};
    int poi = compile_data_segment(d_buff, ldata, b, ddlen);
    int ip = compile_code_segment(hd_code_mem, lcode, b, len);
    return ret(segment_buffer{b, len}, segment_buffer{b, ddlen}, ip);
}

void program_compile_x86::rewrite_to_file(std::string file_name) {
    std::ofstream os(file_name);
    for (int i = 0; i < len; ++i) {
        os << this->b[i] << std::endl;
    }
    os.close();
}


unsigned int program_compile_x86::compile_code_segment(std::string *base, int length, word *lbuf, int &ddlen) {
    unsigned int ip = 0;
    address_map addr_map;
    int j = 0;
    for (int i = 0; i < length; i++) {
        std::string s = strings::trim(base[i]);
        if (s.empty()) {
            continue;
        }
        if (base[i].ends_with(":")) {
            ip = s.starts_with("__start:") && ip == 0 ? j : ip;
            addr_map.insert(std::map<std::string, int>::value_type(base[i], j));
            continue;
        }
        auto div = strings::spilt(base[i]);
        auto adb9 = lbuf + j++;
        write_instrument::write_op_type(adb9, inf->relocate(div[0]).first);
        if (div.size() == 1) {
            continue;
        }

        auto reg = strings::spilt_reg(div[1]);
        auto left = reg[0];

        auto dtd0 = p->do_chain(left, &addr_map);
        write_instrument::write_p0(adb9, dtd0.first, dtd0.second);

        if (reg.size() == 2) {
            auto dtd1 = p->do_chain(reg[1], &addr_map);
            write_instrument::write_p0(adb9, dtd1.first, dtd1.second);
        }

    }
    ddlen = j + 1;
    delete[] base;
    return ip;
}

program_compile_x86::~program_compile_x86() = default;;
