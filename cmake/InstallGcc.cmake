if (NOT GCC_FOUND)
    set(GCC_VERSION "5.1.0" CACHE STRING "gcc version")
    set(GCC_CONFIGURE_FLAGS --disable-nls --enable-languages=c,c++ --without-headers --with-newlib --without-libquadmath)
    ExternalProject_Add(GCC
            DOWNLOAD_NAME       gcc-5.1.0.tar.gz
            URL                 https://ftp.gnu.org/gnu/gcc/gcc-5.1.0/gcc-5.1.0.tar.gz
            CONFIGURE_COMMAND   mkdir <SOURCE_DIR>/build && cd <SOURCE_DIR>/build && ../configure --target=${AOS_TARGET} --prefix=${AOS_SYSROOT} --includedir=${AOS_SYSROOT}/${AOS_TARGET}/include --libdir=${AOS_SYSROOT}/${AOS_TARGET}/lib ${GCC_CONFIGURE_FLAGS}
            BUILD_COMMAND       cd <SOURCE_DIR>/build && make all-gcc && make all-target-libgcc
            INSTALL_COMMAND     cd <SOURCE_DIR>/build && make install-gcc && make install-target-libgcc
            TEST_COMMAND        ""
            DEPENDS             ${DEPENDENCY_ON_BINUTILS}
            )

    ExternalProject_Add_Step(GCC GCC_COPY_HEADERS
            COMMAND     mv ${AOS_SYSROOT}/${AOS_TARGET}/lib/gcc/i686-elf/${GCC_VERSION}/include ${AOS_SYSROOT}/${AOS_TARGET}/include/gcc
            DEPENDEES   install
            )

    set(DEPENDENCY_ON_GCC GCC)

    set (GCC_ROOT_DIR          ${AOS_SYSROOT})
    set (GCC_BINARY_DIR        ${GCC_ROOT_DIR}/bin)
    set (GCC_FOUND             YES)
endif ()
