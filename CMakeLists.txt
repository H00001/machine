cmake_minimum_required(VERSION 3.16)
project(Machine)

set(CMAKE_CXX_STANDARD 20)

add_executable(Machine main.cpp src/mm/memory.hh src/process.hh src/personal_computer.hh src/base.hh src/util/binary.hh src/compile/program_compile_x86.hh src/compile/program_compile.hh src/compile/RelocatdFilter.hh src/compile/program_compile.cc src/mm/memory.cc src/common/const.hh src/cpu/x86cpu.cc src/cpu/x86cpu.hh src/cpu/cpu1.hh src/util/strings.cc src/util/strings.hh)