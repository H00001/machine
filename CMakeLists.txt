cmake_minimum_required(VERSION 3.16)
project(Machine)

set(CMAKE_CXX_STANDARD 20)

add_executable(Machine main.cpp src/cpu.hh src/memory.hh src/process.hh src/pc.hh src/base.hh src/util.hh src/binary.hh src/compile/program_compile_x86.hh src/compile/program_compile.hh src/compile/RelocatdFilter.hh src/compile/program_compile.cc)