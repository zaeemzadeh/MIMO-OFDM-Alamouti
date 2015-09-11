INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_MIMO mimo)

FIND_PATH(
    MIMO_INCLUDE_DIRS
    NAMES mimo/api.h
    HINTS $ENV{MIMO_DIR}/include
        ${PC_MIMO_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    MIMO_LIBRARIES
    NAMES gnuradio-mimo
    HINTS $ENV{MIMO_DIR}/lib
        ${PC_MIMO_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MIMO DEFAULT_MSG MIMO_LIBRARIES MIMO_INCLUDE_DIRS)
MARK_AS_ADVANCED(MIMO_LIBRARIES MIMO_INCLUDE_DIRS)

