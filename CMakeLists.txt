
# -------------
# Do not modify
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

FIND_PACKAGE(deal.II 8.5 QUIET
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

## Build the python bindings for the core icepack library.
find_package(pybind11 REQUIRED)

include_directories(${PYTHON_INCLUDE_DIR})
add_library(icepack_py MODULE src/icepack.cpp)
DEAL_II_SETUP_TARGET(icepack_py)
target_link_libraries(icepack_py icepack pybind11::module)

set_target_properties(icepack_py PROPERTIES
  OUTPUT_NAME "_icepack_py"
  PREFIX "${PYTHON_MODULE_PREFIX}"
  SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
