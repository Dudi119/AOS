if (NOT BINUTILS_FOUND)
    set(BINUTILS_CONFIGURE_FLAGS --with-sysroot --disable-nls --disable-werror)
    set(GCC_CONFIGURE_FLAGS --disable-nls --enable-languages=c,c++ --without-headers)
    ExternalProject_Add(BINUTILS
            DOWNLOAD_NAME       binutils-2.25.tar.gz
            URL                 https://ftp.gnu.org/gnu/binutils/binutils-2.25.tar.gz
            CONFIGURE_COMMAND   mkdir <SOURCE_DIR>/build && cd <SOURCE_DIR>/build && ../configure --target=${AOS_TARGET} --prefix=${AOS_SYSROOT} ${BINUTILS_CONFIGURE_FLAGS}
            BUILD_COMMAND       cd <SOURCE_DIR>/build && make
            INSTALL_COMMAND     cd <SOURCE_DIR>/build && make install
            TEST_COMMAND        ""
            )

    set(DEPENDENCY_ON_BINUTILS BINUTILS)

    set (BINUTILS_ROOT_DIR          ${AOS_SYSROOT})
    set (BINUTILS_BINARY_DIR        ${BINUTILS_ROOT_DIR}/bin)
    set (BINUTILS_FOUND             YES)
endif ()
