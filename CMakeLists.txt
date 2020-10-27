cmake_minimum_required(VERSION 3.16)
project(Machine)

set(CMAKE_CXX_STANDARD 20)

add_executable(Machine main.cpp src/mm/memory.hh src/proc/process.hh src/personal_computer.hh src/common/base.hh src/util/binary.hh src/compile/program_compile_x86.hh src/compile/program_compile.hh src/compile/RelocatedFilter.hh src/compile/program_compile.cc src/mm/memory.cc src/common/const.hh src/cpu/x86cpu.cc src/cpu/x86cpu.hh src/cpu/cpu1.hh src/util/strings.cc src/util/strings.hh src/proc/process.cc src/util/bitmap.hh src/util/bitmap.cc src/compile/write_instrument.cc src/compile/write_instrument.hh src/compile/standard.hh src/cpu/mmu.hh src/cpu/x86mmu.cc src/cpu/x86mmu.hh src/cpu/x86_instrument.hh)
add_executable(test_bitmap test/test_bitmap.cc)