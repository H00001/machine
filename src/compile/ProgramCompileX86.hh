//
// Created by dos DRTT on 10/24/20.
//

#include <fstream>
#include <map>
#include "ProgramCompile.hh"
#include "RelocatdFilter.hh"

class ProgramCompileX86 : public ProgramCompile {
private:


    std::vector<RelocatedFilter *> empty_list{new EmptyFilter()};
    std::vector<RelocatedFilter *> reg_list{new RegisterFilter()};
    std::vector<RelocatedFilter *> ins_list{new InstrumentFilter()};


public:

    ProgramCompileX86() = default;

    std::pair<code_buffer, data_buffer> compile_load(std::string file_name) override {

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

    unsigned int compile(std::pair<code_buffer, data_buffer> p) override {
        compile_data_segment(p.second.b, p.second.length);
        return compile_code_segment(p.first.b, p.first.length);
    }

    void rewrite_to_file(std::string file_name, std::string *b, int len) override {
        std::ofstream os(file_name);
        for (int i = 0; i < len; ++i) {
            os << b[i] << std::endl;
        }
        os.close();

    }


private:
    unsigned int compile_code_segment(std::string *base, int length) {
        unsigned int ip = 0;
        std::map<std::string, offset> addr_map;
        for (int i = 0; i < length; ++i) {
            std::string s = strings::trim(base[i]);
            if (s.empty()) {
                for (auto &iter : empty_list) {
                    s = iter->relocate(s);
                }
                base[i] = s;
                continue;
            }
            if (base[i].ends_with(":")) {
                ip = s.starts_with("__start:") && ip == 0 ? i : ip;
                addr_map.insert(std::map<std::string, int>::value_type(base[i], i + 1));
                base[i] = "0 %0";
                continue;
            }
            auto div = strings::spilt1(base[i], " ");
            auto op = div[0];
            for (auto &iter : ins_list) {
                op = iter->relocate(op);
            }
            if (div.size() == 1) {
                base[i] = op;
                continue;
            }
            auto ins = strings::spilt_reg(div[1]);
            std::string c3d4 = " ";
            for (auto dt:ins) {
                for (auto &iter : reg_list) {
                    if (iter->match(dt[0])) {
                        dt = iter->relocate(dt);
                    }
                }
                c3d4.append(dt + ",");
            }
            base[i] = op + c3d4.substr(0, c3d4.length() - 1);

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
        return 0;
    }
};