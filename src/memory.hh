//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_MEMORY_HH
#define MACHINE_MEMORY_HH

#include<string>
#include<fstream>
#include "base.hh"
#include <map>
#include "util.hh"
#include <regex>


namespace gunplan::cplusplus::machine {
    const unsigned long mm_size = 16 * 1024 * 1024;
    using address = unsigned long;
    const std::regex ws_re("\\s+");

    struct seg {
        unsigned long code;
        int code_len;
        unsigned long data;
        int data_len;
        unsigned long stack;
        int stack_len;
    };


    struct segment_selector {
        unsigned long key;
    };

    struct logical_address {
        segment_selector sel;
        int offset;
    };


    class memory {

    private:
        byte *analyze_code(int offset, int length) {
            auto *base = hd_code_mem + offset;
            std::map<std::string, address> addr_map;
            for (int i = 0; i < length; ++i) {
                if (base[i].ends_with(":")) {
                    addr_map.insert(std::map<std::string, int>::value_type(hd_code_mem[i], i));
                }
            }
            for (int i = 0; i < length; ++i) {
                if (base[i].starts_with("call") || base[i].starts_with("jmp") || base[i].starts_with("je") || base[i].starts_with("jne")) {
                    std::vector<std::string> v(std::sregex_token_iterator(base[i].begin(), base[i].end(), ws_re, -1),
                                               std::sregex_token_iterator());
                    base[i] = v[0] + " " + std::to_string(addr_map[v[1]+":"]);

                }
            }
            printf("ss");
        }

    public:
        static byte *hd_mem;
        static std::string *hd_code_mem;

        byte *analyze_data(std::string *s, int n) {
            auto *data_table = new std::map<std::string, int>;
//            int pos = 0;
//            for (int i = 0; i < n; i += 2) {
//                (*data_table)[s[i]] = pos;
//                auto dt = s[i + 1];
//                auto o = strings::spilt(dt, " ");
//                if (o[0] == "int") {
//                    auto r = strings::Analyze(o[1], nullptr);
//                    pos += r->size() * sizeof(int);
//                    int *p = reinterpret_cast<int *>(data);
//                    for (auto &v :*r) {
//                        *p = v->operval;
//                        p++;
//                    }
//                }
//            }
            return nullptr;
        }

        seg *load(const std::string &file) {
            int pos = -1;
            auto *d_buff = new std::string[120];
            std::ifstream in(file);
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
            byte *data = analyze_data(d_buff, ldata);
            analyze_code(0, lcode);
            return new seg{0, lcode, 0, ldata, 1024, 1024};
        }


        static address transfer(segment_disruptor *ldt, segment_selector sd, cpu_register offset) {
            if (offset > ldt[sd.key].len) {
                throw -1;
            }
            return (ldt[sd.key].start << 12) + offset;
        }

    };

    byte *memory::hd_mem = new byte[mm_size];
    std::string *memory::hd_code_mem = new std::string[200];

}
#endif //MACHINE_MEMORY_HH
