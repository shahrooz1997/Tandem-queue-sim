cmake_minimum_required(VERSION 3.17)
project(CSE517_Eval)

set(CMAKE_CXX_STANDARD 11)

add_executable(CSE517_Eval main.cpp Event_stack.cpp Event_stack.h Job.cpp Job.h Queue.cpp Queue.h)