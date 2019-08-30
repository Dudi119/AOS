find_library(CLANG_CXX_LIBRARY_DIR NAMES libcxx.a libcxxabi.a PATHS ${AOS_SYSROOT}/${AOS_TARGET}/lib NO_DEFAULT_PATH)
find_file(CLANG_CXX_FILES_DIR NAMES iostream PATHS ${AOS_SYSROOT}/${AOS_TARGET}/include/cxx NO_DEFAULT_PATH)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CLANG_CXX REQUIRED_VARS CLANG_CXX_LIBRARY_DIR CLANG_CXX_FILES_DIR)

if(CLANG_CXX_FOUND)
    message(STATUS "Found clang cxx dir - ${Green}${CLANG_CXX_LIBRARY_DIR}${ColourReset}")
    message(STATUS "Found clang cxx include dir - ${Green}${CLANG_CXX_FILES_DIR}${ColourReset}")
else()
    message(WARNING ${Red}"CLANG_CXX not found${ColourReset}")
endif()

