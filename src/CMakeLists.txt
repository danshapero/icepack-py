set (SOURCES
  icepack_py.cpp
  ice_shelf.cpp
  field.cpp
  interpolate.cpp
  dealii.cpp
)

pybind11_add_module(icepack_py MODULE ${SOURCES})

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

