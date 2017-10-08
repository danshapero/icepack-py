
#include <icepack/physics/constants.hpp>
#include "icepack_py.hpp"

PYBIND11_MODULE(icepack_py, module)
{
  module.doc() = "icepack python bindings";

  // Add attributes for some of the physical constants, like the density of ice
  // and water, the gravitational acceleration, etc.
  module.attr("rho_ice") = py::float_(icepack::constants::rho_ice);
  module.attr("rho_water") = py::float_(icepack::constants::rho_water);
  module.attr("gravity") = py::float_(icepack::constants::gravity);

  // Python bindings for the remaining functions in icepack are in the other
  // source files in this directory.
  icepack::bind_dealii(module);
  icepack::bind_field(module);
  icepack::bind_interpolate(module);
  icepack::bind_ice_shelf(module);
  icepack::bind_mass_transport(module);
}

