# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/generate_helper.py "
#!${PYTHON_EXECUTABLE}

import sys, os, re
sys.path.append('${GR_RUNTIME_PYTHONPATH}')
os.environ['srcdir'] = '${CMAKE_CURRENT_SOURCE_DIR}'
os.chdir('${CMAKE_CURRENT_BINARY_DIR}')

if __name__ == '__main__':
    sys.path.append('${CMAKE_CURRENT_SOURCE_DIR}/../python')
    import build_utils
    root, inp = sys.argv[1:3]
    for sig in sys.argv[3:]:
        name = re.sub ('X+', sig, root)

        d = build_utils.standard_dict(name, sig, 'Spread')
        build_utils.expand_template(d, inp, '_impl')
")


macro(expand_cc_h_impl root)
  #make a list of all the generated files
  unset(expanded_files_cc_impl)
  unset(expanded_files_h_impl)
  foreach(sig ${ARGN})
    string(REGEX REPLACE "X+" ${sig} name ${root})
    list(APPEND expanded_files_cc_impl ${CMAKE_CURRENT_BINARY_DIR}/${name}_impl.cc)
    list(APPEND expanded_files_h_impl ${CMAKE_CURRENT_BINARY_DIR}/${name}_impl.h)
    list(APPEND expanded_files_h ${CMAKE_CURRENT_BINARY_DIR}/../include/${name}.h)
  endforeach(sig)

  #create a command to generate the _impl.cc files
  add_custom_command(
    OUTPUT ${expanded_files_cc_impl}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${root}_impl.cc.t
    COMMAND ${PYTHON_EXECUTABLE} ${PYTHON_DASH_B}
    ${CMAKE_CURRENT_BINARY_DIR}/generate_helper.py
    ${root} ${root}_impl.cc.t ${ARGN}
  )

  #create a command to generate the _impl.h files
  add_custom_command(
    OUTPUT ${expanded_files_h_impl}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${root}_impl.h.t
    COMMAND ${PYTHON_EXECUTABLE} ${PYTHON_DASH_B}
    ${CMAKE_CURRENT_BINARY_DIR}/generate_helper.py
    ${root} ${root}_impl.h.t ${ARGN}
  )

  #make _impl.cc source files depend on headers to force generation
  set_source_files_properties(${expanded_files_cc_impl}
    PROPERTIES OBJECT_DEPENDS "${expanded_files_h_impl}"
  )

  #make _impl.h source files depend on headers to force generation
  set_source_files_properties(${expanded_files_h_impl}
    PROPERTIES OBJECT_DEPENDS "${expanded_files_h}"
  )

  #install rules for the generated cc files
  list(APPEND generated_sources ${expanded_files_cc_impl})
endmacro(expand_cc_h_impl)
########################################################################
# Invoke macro to generate various sources
########################################################################
expand_cc_h_impl(preamble_XX bb cc) 

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND Spread_sources
    ${generated_sources}
    synthesizer_impl.cc
    framer_impl.cc
    deframer_impl.cc
    sync_impl.cc
    cpfsk_mod_impl.cc
    cpfsk_demod_impl.cc
    msg_source_impl.cc
    rx_synthesizer_impl.cc 
    common.cc
    code_generator.cc
    ds_spreader_impl.cc
    ds_despreader_impl.cc
    rx_sync_impl.cc)

set(Spread_sources "${Spread_sources}" PARENT_SCOPE)
if(NOT Spread_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT Spread_sources)

add_library(gnuradio-Spread SHARED ${Spread_sources})
add_dependencies(gnuradio-Spread Spread_generated_includes)
target_link_libraries(gnuradio-Spread ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-Spread PROPERTIES DEFINE_SYMBOL "gnuradio_Spread_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-Spread PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-Spread
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${Boost_INCLUDE_DIR} ${CPPUNIT_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND test_Spread_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_Spread.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_Spread.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_cpfsk.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_sync.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_code_generator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_deframer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_synthesizer.cc
)

# Need to include Spread_sources since we test specific classes
add_executable(test-Spread ${test_Spread_sources} ${Spread_sources})

target_link_libraries(
  test-Spread
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  boost_thread
  ${CPPUNIT_LIBRARIES}
  gnuradio-Spread
)

GR_ADD_TEST(test_Spread test-Spread)
