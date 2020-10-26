//
// Created by dos DRTT on 10/26/20.
//

#ifndef MACHINE_STANDARD_HH
#define MACHINE_STANDARD_HH

#include<utility>
#include <map>
#include <list>
#include "../common/const.hh"

using word = unsigned int;
using double_word = unsigned long;
using op_len = unsigned char;
using decode_result = std::pair<op_len, bool>;
using offset = unsigned int;
using address_map = std::map<std::string, offset>;
using compile_register = std::map<std::string, int>;
using compile_code = std::map<std::string, op_len>;
using ret = std::tuple<segment_buffer, segment_buffer, unsigned int>;
#endif //MACHINE_STANDARD_HH
