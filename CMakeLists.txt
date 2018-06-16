cmake_minimum_required(VERSION 3.10)

project(fusion_xml)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-fexceptions")

file(GLOB headers "./rapidxml/*.hpp")
add_executable(fusion_xml main.cpp ${headers} FusionXML.cpp FusionXML.h )

