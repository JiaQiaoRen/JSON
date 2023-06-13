cmake_minimum_required(VERSION 3.25)
project(WangMing)

set(CMAKE_CXX_STANDARD 17)

add_executable(WangMing main.cpp src/json/JSON.cpp src/json/JSON.h src/json/JSONSerialization.cpp src/json/JSONSerialization.h)
