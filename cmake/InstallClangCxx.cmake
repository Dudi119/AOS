if (NOT CLANG_CXX_FOUND)
    set(NEWLIB_DEFINES "-D_HAVE_LONG_DOUBLE -D_LDBL_EQ_DBL -D_POSIX_TIMERS -U__STRICT_ANSI__ -DMALLOC_PROVIDED")
    set(CLANG_CXXABI_DEFINES "-DHAVE___CXA_THREAD_ATEXIT_IMPL -D_LIBCPP_HAS_NO_ALIGNED_ALLOCATION -D_LIBCXXABI_HAS_NO_THREADS -DHAVE___CXA_THREAD_ATEXIT_IMPL")
    set(CLANG_CXX_DEFINES "-D_NEWLIB_VERSION -D_LIBCPP_HAS_NO_THREADS -D_LIBCPP_HAS_NO_MONOTONIC_CLOCK -D_LIBCPP_BUILDING_LIBRARY")
    set(CLANG_CXX_FLAGS  "-std=c++14 -fno-use-cxa-atexit -fno-threadsafe-statics -ffreestanding")
    ExternalProject_Add(LIBCXXABI
            DOWNLOAD_NAME       libcxxabi-7.1.0.src.tar.xz
            URL                 https://github.com/llvm/llvm-project/releases/download/llvmorg-7.1.0/libcxxabi-7.1.0.src.tar.xz
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       ""
            INSTALL_COMMAND     ""
            TEST_COMMAND        ""
            DEPENDS             ${DEPENDENCY_ON_GCC} ${DEPENDENCY_ON_NEWLIB}
            )

    ExternalProject_Add(LIBCXX
            DOWNLOAD_NAME       libcxx-7.1.0.src.tar.xz
            URL                 https://github.com/llvm/llvm-project/releases/download/llvmorg-7.1.0/libcxx-7.1.0.src.tar.xz
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       ""
            INSTALL_COMMAND     ""
            TEST_COMMAND        ""
            DEPENDS             ${DEPENDENCY_ON_GCC} ${DEPENDENCY_ON_NEWLIB}
            )

    message(STATUS "cxxabi compilation command - ${AOS_SYSROOT}/bin/${AOS_CXX_COMPILER} -I<SOURCE_DIR>/include -I${AOS_SYSROOT}/${AOS_TARGET}/include -I../../LIBCXX/include ${CLANG_CXX_FLAGS} ${CLANG_CXXABI_DEFINES} ${CLANG_CXX_DEFINES} ${NEWLIB_DEFINES} -c *.cpp")
    ExternalProject_Add_Step(LIBCXXABI LIBCXXABI_COMPILE
            COMMAND     cd <SOURCE_DIR>/src && bash -c "${AOS_SYSROOT}/bin/${AOS_CXX_COMPILER} -I<SOURCE_DIR>/include -I${AOS_SYSROOT}/${AOS_TARGET}/include -I../../LIBCXX/include ${CLANG_CXX_FLAGS} ${CLANG_CXXABI_DEFINES} -D_LIBCPP_HAS_NO_THREADS ${NEWLIB_DEFINES} -c *.cpp"
            DEPENDEES   install
            )

    ExternalProject_Add_Step(LIBCXXABI LIBCXXABI_ASSEMBLE
            COMMAND     cd <SOURCE_DIR>/src && bash -c "${AOS_SYSROOT}/bin/${AOS_ASSEMBLER} -rcs ${AOS_SYSROOT}/${AOS_TARGET}/lib/libcxxabi.a *.o"
            DEPENDEES   LIBCXXABI_COMPILE
            )

    ExternalProject_Add_Step(LIBCXXABI LIBCXXABI_COPY_HEADERS
            COMMAND     cp <SOURCE_DIR>/include -a ${AOS_SYSROOT}/include/cxxabi
            DEPENDEES   LIBCXXABI_ASSEMBLE
            )

    message(STATUS "cxx compilation command - ${AOS_SYSROOT}/bin/${AOS_CXX_COMPILER} -I<SOURCE_DIR>/include -I${AOS_SYSROOT}/${AOS_TARGET}/include -I../../LIBCXX/include ${CLANG_CXX_FLAGS} ${CLANG_CXXABI_DEFINES} ${CLANG_CXX_DEFINES} ${NEWLIB_DEFINES} -c *.cpp")
    ExternalProject_Add_Step(LIBCXX LIBCXX_COMPILE
            COMMAND     cd <SOURCE_DIR>/src && bash -c "${AOS_SYSROOT}/bin/${AOS_CXX_COMPILER} -I<SOURCE_DIR>/include -I${AOS_SYSROOT}/${AOS_TARGET}/include -I../../LIBCXXABI/include ${CLANG_CXX_FLAGS} ${CLANG_CXX_DEFINES} ${CLANG_CXXABI_DEFINES} ${NEWLIB_DEFINES} -c *.cpp"
            DEPENDEES   install
            )

    ExternalProject_Add_Step(LIBCXX LIBCXX_ASSEMBLE
            COMMAND     cd <SOURCE_DIR>/src && bash -c "${AOS_SYSROOT}/bin/${AOS_ASSEMBLER} -rcs ${AOS_SYSROOT}/${AOS_TARGET}/lib/libcxx.a *.o"
            DEPENDEES   LIBCXX_COMPILE
            )

    ExternalProject_Add_Step(LIBCXX LIBCXX_COPY_HEADERS
            COMMAND     cp <SOURCE_DIR>/include -a ${AOS_SYSROOT}/include/cxx
            DEPENDEES   LIBCXX_ASSEMBLE
            )

    set (CLANG_CXX_ROOT_DIR          ${AOS_SYSROOT})
    set (CLANG_CXX_LIBRARY_DIR       ${LIBCXX_ROOT_DIR}/lib)
    set (CLANG_CXX_FOUND             YES)
endif ()
