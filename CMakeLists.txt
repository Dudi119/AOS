cmake_minimum_required(VERSION 3.5.1)

set(AOS_SYSROOT "${CMAKE_CURRENT_SOURCE_DIR}/cross" CACHE STRING "cross sysroot")
set(AOS_3RD_LOC "${PROJECT_BINARY_DIR}/Third_Party" CACHE STRING "third party download location")
set(DEPENDENCY_ON_BINUTILS "")
set(DEPENDENCY_ON_GCC "")
set(DEPENDENCY_ON_NEWLIB "")
set(DEPENDENCY_ON_LIBCXX "")
set(DEPENDENCY_ON_LIBCXXABI "")

set(AOS_TARGET "i686-elf")
set(AOS_ASSEMBLER "${AOS_TARGET}-as")
set(AOS_ARCHIVER "${AOS_TARGET}-ar")
set(AOS_C_COMPILER "${AOS_TARGET}-gcc")
set(AOS_CXX_COMPILER "${AOS_TARGET}-g++")

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

set(AOS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(AOS_INCLUDE_DIRS "-I${AOS_SYSROOT}/${AOS_TARGET}/include/cxx -I${AOS_SYSROOT}/${AOS_TARGET}/include/cxxabi -I${AOS_SYSROOT}/${AOS_TARGET}/include -I${AOS_SYSROOT}/${AOS_TARGET}/include/gcc")
set(AOS_SOURCE_FILES "${AOS_SRC}/kernel/KernelMain.cpp ${AOS_SRC}/kernel/Syscalls.cpp")
set(AOS_CXX_FLAGS "-ffreestanding -g -O0 -Wall -Wextra -Wno-unused-parameter -fno-exceptions -fno-rtti")
set(AOS_LINKER_FLAGS "-ffreestanding -g -O0 -nostdlib")
set(AOS_LINKED_LIBS "-L${AOS_SYSROOT}/${AOS_TARGET}/lib -L${AOS_SYSROOT}/${AOS_TARGET}/lib/gcc/i686-elf/5.1.0 -lcxx -lcxxabi -lc -lm -lgcc")
set(AOS_CXX_DEFINES "-D_LIBCPP_HAS_NO_THREADS")

add_custom_target(aos ALL
                  COMMAND bash -c "${AOS_SYSROOT}/bin/${AOS_CXX_COMPILER} ${AOS_INCLUDE_DIRS} ${AOS_CXX_FLAGS} ${AOS_CXX_DEFINES} -c ${AOS_SOURCE_FILES}"
                  COMMAND bash -c "${AOS_SYSROOT}/bin/${AOS_ASSEMBLER} ${AOS_SRC}/kernel/BootStrap.asm -o BootStram.o"
                  COMMAND bash -c "${AOS_SYSROOT}/bin/${AOS_C_COMPILER} -T ${CMAKE_CURRENT_SOURCE_DIR}/Linker.ld -o aos.bin ${AOS_LINKER_FLAGS} *.o ${AOS_LINKED_LIBS}"
                  DEPENDS ${DEPENDENCY_ON_BINUTILS} ${DEPENDENCY_ON_GCC} ${DEPENDENCY_ON_NEWLIB}  ${DEPENDENCY_ON_LIBCXX} ${DEPENDENCY_ON_LIBCXXABI}
        )


