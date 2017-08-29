
#include <icepack/physics/ice_shelf.hpp>
#include <pybind11/stl.h>
#include "icepack_py.hpp"

namespace icepack
{
  void bind_ice_shelf(py::module& module)
  {
    module.def("rate_factor", &icepack::rate_factor,
               "Compute the rate factor `A` in Glen's flow law");

    py::class_<icepack::MembraneStress>(module, "MembraneStress")
      .def(py::init<const double>(),
           py::arg("n") = 3.0);

    py::class_<icepack::Viscosity>(module, "Viscosity")
      .def(py::init<const icepack::MembraneStress&>(),
           py::keep_alive<1, 2>(),
           py::arg("membrane_stress") = icepack::MembraneStress());

    py::class_<icepack::IceShelf>(module, "IceShelf")
      .def(py::init<const std::set<dealii::types::boundary_id>&, const icepack::Viscosity&, const double>(),
           "Create an object for modelling the flow of floating ice shelves",
           py::keep_alive<1, 3>(),
           py::arg("dirichlet_boundary_ids") = std::set<dealii::types::boundary_id>{0},
           py::arg("viscosity") = icepack::Viscosity(icepack::MembraneStress()),
           py::arg("convergence_tolerance") = 1.0e-6)
      .def("solve", &icepack::IceShelf::solve);
  }
}

