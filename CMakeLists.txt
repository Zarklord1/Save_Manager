cmake_minimum_required(VERSION 3.2)
project(Save_Manager)

set(DEVKITPRO $ENV{DEVKITPRO} CACHE STRING "Path to devkitPRO install")

# Check for DEVKITPRO
if(NOT DEVKITPRO)
    message(FATAL_ERROR "You must have defined DEVKITPRO before calling cmake.")
endif()

set(ENV{DEVKITPRO} ${DEVKITPRO})

# Compilation step for static resources
# Directory for all compiled in resources
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/data")
# Devkit programs for resource files
set(AS "${DEVKITPPC}/bin/powerpc-eabi-as${CMAKE_EXECUTABLE_SUFFIX}")
set(BIN2S "${DEVKITPPC}/bin/bin2s${CMAKE_EXECUTABLE_SUFFIX}")
FUNCTION(ADD_RESOURCES out_var)
  SET(result)
  FOREACH(ARG ${ARGN})
    FILE(GLOB_RECURSE RESFILES ${ARG})
    FOREACH(in_f ${RESFILES})
      FILE(RELATIVE_PATH src_f ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${in_f})
      GET_FILENAME_COMPONENT(f_n ${in_f} NAME)
      SET(out_f "${PROJECT_BINARY_DIR}/data/${f_n}.o")
      ADD_CUSTOM_COMMAND(OUTPUT ${out_f}
        COMMAND ${BIN2S} -a 32 ${src_f} | ${AS} -o ${out_f}
        DEPENDS ${in_f}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "${out_f}"
        VERBATIM
        )
      LIST(APPEND result ${out_f})
    ENDFOREACH()
    SET(${out_var} "${result}" PARENT_SCOPE)
  ENDFOREACH()
ENDFUNCTION()

set(LIBOGC_INC "${DEVKITPRO}/libogc/include")
set(LIBOGC_LIB "${DEVKITPRO}/libogc/lib/wii")
set(PORTLIBS "${DEVKITPRO}/portlibs/ppc")

set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/src) 
set(DATADIR data) 

set(SRCINCLUDES "${SRCDIR}")

set(INCLUDES "${SRCINCLUDES}" "${CMAKE_CURRENT_SOURCE_DIR}/build" "${PORTLIBS}/include" "${PORTLIBS}/include/freetype2")
set(LINKFLAGS "-L${PROJECT_SOURCE_DIR} -L${DEVKITPPC}/ -L${DEVKITPPC}/lib/gcc/powerpc-eabi/4.8.2 -L${LIBOGC_LIB} -L${PORTLIBS}/lib")

file(GLOB_RECURSE C_FILES ${SRCDIR}/*.c)
file(GLOB_RECURSE CPP_FILES ${SRCDIR}/*.cpp)
file(GLOB_RECURSE H_FILES ${SRCDIR}/.h)
file(GLOB_RECURSE HPP_FILES ${SRCDIR}/*.hpp)
ADD_RESOURCES(BINFILES ${DATADIR}/*.*)

add_rpx(Save_Manager ${C_FILES} ${CPP_FILES} ${H_FILES} ${HPP_FILES} ${BINFILES})
# Add extralibs stuff to building libs
set_target_properties(Save_Manager PROPERTIES LINK_FLAGS "${RPX_LINKER_FLAGS} ${LINKFLAGS}")
target_include_directories(Save_Manager PRIVATE "${INCLUDES}")
target_link_libraries(Save_Manager
    coreinit proc_ui nsysnet sndcore2 vpad gx2 sysapp gd png z freetype mad vorbisidec iosuhax)