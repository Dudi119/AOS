cmake_minimum_required(VERSION 3.5.1)

set(AOS_SYSROOT "${CMAKE_CURRENT_SOURCE_DIR}/cross" CACHE STRING "cross sysroot")
set(AOS_3RD_LOC "${PROJECT_BINARY_DIR}/Third_Party" CACHE STRING "third party download location")
set(DEPENDENCY_ON_BINUTILS "")
set(DEPENDENCY_ON_GCC "")
set(DEPENDENCY_ON_NEWLIB "")

set(AOS_TARGET "i686-elf")
set(AOS_ASSEMBLER "${AOS_TARGET}-ar")
set(AOS_C_COMPILER "${AOS_TARGET}-gcc")
set(AOS_CXX_COMPILER "${AOS_TARGET}-g++")

set(ENV{PATH} "$ENV{PATH}:${AOS_SYSROOT}/bin" CACHE STRING "PATH env")

message(STATUS "-------------------------------------")
message(STATUS "SYSROOT=${AOS_SYSROOT}")
message(STATUS "3RD_LOC=${AOS_3RD_LOC}")
message(STATUS "TARGET=${AOS_TARGET}")
message(STATUS "C_COMPILER=${AOS_C_COMPILER}")
message(STATUS "CXX_COMPILER=${AOS_CXX_COMPILER}")
message(STATUS "ENV:PATH=$ENV{PATH}")
message(STATUS "-------------------------------------")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(BinUtils)
find_package(Gcc)
find_package(NewLib)
find_package(ClangCxx)

include(installThirdParty)
