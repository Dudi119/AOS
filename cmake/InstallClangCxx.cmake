if (NOT CLANG_CXX_FOUND)
    set(NEWLIB_DEFINES "-D_HAVE_LONG_DOUBLE -D_LDBL_EQ_DBL -D_POSIX_TIMERS -U__STRICT_ANSI__ -DMALLOC_PROVIDED")
    set(CLANG_CXXABI_DEFINES "-DHAVE___CXA_THREAD_ATEXIT_IMPL -D_LIBCPP_HAS_NO_ALIGNED_ALLOCATION -D_LIBCXXABI_HAS_NO_THREADS -DHAVE___CXA_THREAD_ATEXIT_IMPL")
    set(CLANG_CXX_DEFINES "-D_NEWLIB_VERSION -D_LIBCPP_HAS_NO_THREADS -D_LIBCPP_HAS_NO_MONOTONIC_CLOCK -D_LIBCPP_BUILDING_LIBRARY -D_LIBCPP_NO_EXCEPTIONS -DLIBCXX_BUILDING_LIBCXXABI")
    set(CLANG_C_HEADERS "-I${AOS_SYSROOT}/${AOS_TARGET}/include -I${AOS_SYSROOT}/${AOS_TARGET}/include/gcc")
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

    set(LIBCXXABI_SOURCE "${AOS_3RD_LOC}/src/LIBCXXABI")
    set(LIBCXX_SOURCE "${AOS_3RD_LOC}/src/LIBCXX")


    add_custom_target(LIBCXXABI_COMPILE ALL
            COMMAND cd ${LIBCXXABI_SOURCE}/src && bash -c "${AOS_SYSROOT}/bin/${AOS_CXX_COMPILER} -I${LIBCXXABI_SOURCE}/include -I${LIBCXX_SOURCE}/include ${CLANG_C_HEADERS} ${CLANG_CXX_FLAGS} ${CLANG_CXXABI_DEFINES} -D_LIBCPP_HAS_NO_THREADS ${NEWLIB_DEFINES} -c *.cpp"
            COMMAND cd ${LIBCXXABI_SOURCE}/src && bash -c "${AOS_SYSROOT}/bin/${AOS_ARCHIVER} -rcs ${AOS_SYSROOT}/${AOS_TARGET}/lib/libcxxabi.a *.o"
            COMMAND cp ${LIBCXXABI_SOURCE}/include -a ${AOS_SYSROOT}/${AOS_TARGET}/include/cxxabi
            DEPENDS LIBCXX LIBCXXABI
            )

    add_custom_target(LIBCXX_COMPILE ALL
            COMMAND     cd ${LIBCXX_SOURCE}/src && bash -c "${AOS_SYSROOT}/bin/${AOS_CXX_COMPILER} -I${LIBCXX_SOURCE}/include -I${LIBCXXABI_SOURCE}/include ${CLANG_C_HEADERS} ${CLANG_CXX_FLAGS} ${CLANG_CXX_DEFINES} ${CLANG_CXXABI_DEFINES} ${NEWLIB_DEFINES} -c *.cpp"
            COMMAND     cd ${LIBCXX_SOURCE}/src && bash -c "${AOS_SYSROOT}/bin/${AOS_ARCHIVER} -rcs ${AOS_SYSROOT}/${AOS_TARGET}/lib/libcxx.a *.o"
            COMMAND     cp ${LIBCXX_SOURCE}/include -a ${AOS_SYSROOT}/${AOS_TARGET}/include/cxx
            DEPENDS LIBCXX LIBCXXABI
            )

    set(DEPENDENCY_ON_LIBCXX LIBCXX_COMPILE)
    set(DEPENDENCY_ON_LIBCXXABI LIBCXXABI_COMPILE)
    set (CLANG_CXX_ROOT_DIR          ${AOS_SYSROOT})
    set (CLANG_CXX_LIBRARY_DIR       ${LIBCXX_ROOT_DIR}/lib)
    set (CLANG_CXX_FOUND             YES)
endif ()
