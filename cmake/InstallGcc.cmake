if (NOT GCC_FOUND)
    set(GCC_CONFIGURE_FLAGS --disable-nls --enable-languages=c,c++ --without-headers --with-newlib --without-libquadmath)
    ExternalProject_Add(GCC
            DOWNLOAD_NAME       gcc-5.1.0.tar.gz
            URL                 https://ftp.gnu.org/gnu/gcc/gcc-5.1.0/gcc-5.1.0.tar.gz
            CONFIGURE_COMMAND   mkdir <SOURCE_DIR>/build && cd <SOURCE_DIR>/build && ../configure --target=${AOS_TARGET} --prefix=${AOS_SYSROOT} ${GCC_CONFIGURE_FLAGS}
            BUILD_COMMAND       cd <SOURCE_DIR>/build && make all-gcc && make all
            INSTALL_COMMAND     cd <SOURCE_DIR>/build && make install
            TEST_COMMAND        ""
            DEPENDS             ${DEPENDENCY_ON_BINUTILS}
            )

    set(DEPENDENCY_ON_GCC GCC)

    set (GCC_ROOT_DIR          ${AOS_SYSROOT})
    set (GCC_BINARY_DIR        ${GCC_ROOT_DIR}/bin)
    set (GCC_FOUND             YES)
endif ()
