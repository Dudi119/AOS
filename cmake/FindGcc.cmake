find_program(GCC_BINARY_DIR NAMES ${AOS_C_COMPILER} ${AOS_CXX_COMPILER} PATHS ${AOS_SYSROOT}/bin NO_DEFAULT_PATH)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GCC REQUIRED_VARS GCC_BINARY_DIR)

if(GCC_FOUND)
    message(STATUS "Found gcc binary dir - ${Green}${GCC_BINARY_DIR}${ColourReset}")
else()
    message(WARNING ${Red}"GCC not found${ColourReset}")
endif()
