
#ifndef ICEPACK_PY_HPP
#define ICEPACK_PY_HPP

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace icepack
{
  void bind_dealii(py::module& module);
  void bind_field(py::module& module);
  void bind_grid_data(py::module& module);
  void bind_interpolate(py::module& module);
  void bind_physics(py::module& module);
  void bind_ice_shelf(py::module& module);
  void bind_ice_stream(py::module& module);
  void bind_mass_transport(py::module& module);
}

#endif

