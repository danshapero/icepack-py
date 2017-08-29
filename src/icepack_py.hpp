
#ifndef ICEPACK_PY_HPP
#define ICEPACK_PY_HPP

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace icepack
{
  void bind_dealii(py::module& module);
  void bind_field(py::module& module);
  void bind_interpolate(py::module& module);
  void bind_ice_shelf(py::module& module);
}

#endif

