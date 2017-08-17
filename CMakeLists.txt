
# -------------
# Do not modify
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

FIND_PACKAGE(deal.II 9.0 QUIET
  HINTS ${deal.II_DIR} ${DEAL_II_DIR} ../ ../../ $ENV{DEAL_II_DIR}
  )
IF(NOT ${deal.II_FOUND})
  MESSAGE(FATAL_ERROR "\n"
    "*** Could not locate a (sufficiently recent) version of deal.II. ***\n\n"
    "You may want to either pass a flag -DDEAL_II_DIR=/path/to/deal.II to cmake\n"
    "or set an environment variable \"DEAL_II_DIR\" that contains this path."
    )
ENDIF()
DEAL_II_INITIALIZE_CACHED_VARIABLES()
# ---------------

project(icepack_py)

find_package(icepack REQUIRED)
find_package(pybind11 REQUIRED)

include_directories(${PYTHON_INCLUDE_DIR})
pybind11_add_module(icepack_py MODULE src/icepack.cpp)

# Hacks to make the wrapper library link with deal.II
if (CMAKE_BUILD_TYPE STREQUAL "Release")
  set (_build "RELEASE")
else()
  set (_build "DEBUG")
endif()

set_property(TARGET icepack_py APPEND PROPERTY
  INCLUDE_DIRECTORIES "${DEAL_II_INCLUDE_DIRS}")
set_property(TARGET icepack_py APPEND_STRING PROPERTY
  LINK_FLAGS " ${DEAL_II_LINKER_FLAGS} ${DEAL_II_LINKED_FLAGS_${_build}}")
get_property("OBJECT_LIBRARY" TARGET icepack_py PROPERTY TYPE)
target_link_libraries(icepack_py PRIVATE icepack)

