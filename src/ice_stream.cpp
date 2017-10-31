
#include <icepack/physics/ice_stream.hpp>
#include "icepack_py.hpp"

namespace icepack
{
  void bind_ice_stream(py::module& module)
  {
    using boundary_ids = std::set<dealii::types::boundary_id>;

    VectorField<2> (IceStream::*diagnostic_solve)(
      const Field<2>&,
      const Field<2>&,
      const Field<2>&,
      const Field<2>&,
      const VectorField<2>&,
      const IceStream::SolveOptions&
    ) const = &IceStream::solve;

    Field<2> (IceStream::*prognostic_solve)(
      const double,
      const Field<2>&,
      const Field<2>&,
      const VectorField<2>&,
      const Field<2>&
    ) const = &IceStream::solve;

    py::class_<IceStream>(module, "IceStream")
      .def(py::init<const boundary_ids&, const Viscosity&, const Friction&, const double>(),
           "Create an object for modelling the flow of grounded ice streams",
           py::arg("dirichlet_boundary_ids") = boundary_ids{0},
           py::arg("viscosity") = Viscosity(MembraneStress()),
           py::arg("friction") = Friction(BasalStress()),
           py::arg("convergence_tolerance") = 1.0e-6)
      .def("solve", diagnostic_solve,
           "Solve the diagnostic equations for the ice velocity",
           py::arg("thickness"),
           py::arg("surface"),
           py::arg("theta"),
           py::arg("beta"),
           py::arg("boundary_velocity"),
           py::arg("solve_options") = IceStream::SolveOptions())
      .def("solve", prognostic_solve,
           "Solve the prognostic equations for the ice thickness",
           py::arg("dt"),
           py::arg("thickness"),
           py::arg("accumulation"),
           py::arg("velocity"),
           py::arg("inflow_thickness"));

    module.def("compute_surface", &compute_surface,
               "Compute the surface elevation from the ice thickness and bed "
               "elevation",
               py::arg("thickness"),
               py::arg("bed"));
  }
}

