INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_SPREAD Spread)

FIND_PATH(
    SPREAD_INCLUDE_DIRS
    NAMES Spread/api.h
    HINTS $ENV{SPREAD_DIR}/include
        ${PC_SPREAD_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    SPREAD_LIBRARIES
    NAMES gnuradio-Spread
    HINTS $ENV{SPREAD_DIR}/lib
        ${PC_SPREAD_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SPREAD DEFAULT_MSG SPREAD_LIBRARIES SPREAD_INCLUDE_DIRS)
MARK_AS_ADVANCED(SPREAD_LIBRARIES SPREAD_INCLUDE_DIRS)

