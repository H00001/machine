cmake_minimum_required(VERSION 3.16)
project(Machine)

set(CMAKE_CXX_STANDARD 20)

add_executable(Machine main.cpp src/cpu.cc src/cpu.hh src/memory.cc src/memory.hh src/process.cc src/process.hh src/pc.cc src/pc.hh src/base.hh src/Processer.hh src/MovProcesser.cc src/MovProcesser.hh src/util.hh src/binary.hh)