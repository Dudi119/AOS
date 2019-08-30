if (NOT NEWLIB_FOUND)
    ExternalProject_Add(NEWLIB
            DOWNLOAD_NAME       newlib-2.3.0.20160104.tar.gz
            URL                 ftp://sourceware.org/pub/newlib/newlib-2.3.0.20160104.tar.gz
            CONFIGURE_COMMAND   mkdir <SOURCE_DIR>/build && cd <SOURCE_DIR>/build && bash -c "CFLAGS=-I${AOS_SYSROOT}/${AOS_TARGET}/include/gcc/ ../configure --target=${AOS_TARGET} --prefix=${AOS_SYSROOT}"
            BUILD_COMMAND       cd <SOURCE_DIR>/build && make all
            INSTALL_COMMAND     cd <SOURCE_DIR>/build && make install
            TEST_COMMAND        ""
            DEPENDS             ${DEPENDENCY_ON_BINUTILS} ${DEPENDENCY_ON_GCC}
            )
    set(DEPENDENCY_ON_NEWLIB NEWLIB)

    set (NEWLIB_ROOT_DIR          ${AOS_SYSROOT})
    set (NEWLIB_LIBRARY_DIR       ${NEWLIB_ROOT_DIR}/bin)
    set (NEWLIB_FOUND             YES)
endif ()
