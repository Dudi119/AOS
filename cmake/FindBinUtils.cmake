find_program(BINUTILS_BINARY_DIR NAMES ${AOS_ASSEMBLER} PATHS ${AOS_SYSROOT}/bin NO_DEFAULT_PATH)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BINUTILS REQUIRED_VARS BINUTILS_BINARY_DIR)

if(BINUTILS_FOUND)
    message(STATUS "Found binutils binary dir - ${Green}${BINUTILS_BINARY_DIR}${ColourReset}")
else()
    message(WARNING ${Red}"BINUTILS not found${ColourReset}")
endif()
