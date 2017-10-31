
#include <icepack/physics/ice_shelf.hpp>
#include "icepack_py.hpp"

namespace icepack
{
  void bind_ice_shelf(py::module& module)
  {
    using boundary_ids = std::set<dealii::types::boundary_id>;

    VectorField<2> (IceShelf::*diagnostic_solve)(
      const Field<2>&,
      const Field<2>&,
      const VectorField<2>&,
      const IceShelf::SolveOptions
    ) const = &IceShelf::solve;

    Field<2> (IceShelf::*prognostic_solve)(
      const double,
      const Field<2>&,
      const Field<2>&,
      const VectorField<2>&,
      const Field<2>&
    ) const = &IceShelf::solve;

    py::class_<IceShelf>(module, "IceShelf")
      .def(py::init<const boundary_ids&, const Viscosity&, const double>(),
           "Create an object for modelling the flow of floating ice shelves",
           py::arg("dirichlet_boundary_ids") = boundary_ids{0},
           py::arg("viscosity") = Viscosity(MembraneStress()),
           py::arg("convergence_tolerance") = 1.0e-6)
      .def("solve", diagnostic_solve,
           "Solve the diagnostic equations for the ice velocity",
           py::arg("thickness"),
           py::arg("temperature"),
           py::arg("boundary_velocity"),
           py::arg("solve_options") = IceShelf::SolveOptions())
      .def("solve", prognostic_solve,
           "Solve the prognostic equations for the ice thickness",
           py::arg("dt"),
           py::arg("thickness"),
           py::arg("accumulation"),
           py::arg("velocity"),
           py::arg("inflow_thickness"));
  }
}

