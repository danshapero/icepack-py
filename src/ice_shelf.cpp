
#include <icepack/physics/ice_shelf.hpp>
#include <pybind11/stl.h>
#include "icepack_py.hpp"

namespace icepack
{
  template <typename T>
  void bind_newton_search_options(py::module& module, const std::string& name)
  {
    using numerics::LineSearchOptions;
    using numerics::NewtonSearchOptions;
    using Callback = typename LineSearchOptions<T>::Callback;

    const std::string callback_name = "Callback" + name;
    py::class_<Callback>(module, callback_name.c_str());

    const std::string line_search_name = "LineSearchOptions" + name;
    py::class_<LineSearchOptions<T>>(module, line_search_name.c_str())
      .def(py::init<const double, const double, const Callback>(),
           py::arg("armijo") = py::float_(LineSearchOptions<T>::armijo_default),
           py::arg("wolfe") = py::float_(LineSearchOptions<T>::wolfe_default),
           py::arg("callback") = Callback([](const double, const T&){}));

    const std::string newton_search_name = "NewtonSearchOptions" + name;
    py::class_<NewtonSearchOptions<T>>(module, newton_search_name.c_str())
      .def(py::init<const Callback, const LineSearchOptions<T>>(),
           py::arg("callback") = Callback([](const double, const T&){}),
           py::arg("line_search_options") = LineSearchOptions<T>());
  }


  void bind_ice_shelf(py::module& module)
  {
    module.def("rate_factor", &icepack::rate_factor,
               "Compute the rate factor `A` in Glen's flow law");

    py::class_<MembraneStress>(module, "MembraneStress")
      .def(py::init<const double>(),
           py::arg("n") = 3.0);

    py::class_<Viscosity>(module, "Viscosity")
      .def(py::init<const MembraneStress&>(),
           py::arg("membrane_stress") = MembraneStress());

    bind_newton_search_options<VectorField<2>>(module, "VectorField2");

    using boundary_ids = std::set<dealii::types::boundary_id>;

    VectorField<2> (IceShelf::*diagnostic_solve)(const Field<2>&, const Field<2>&, const VectorField<2>&, const IceShelf::SolveOptions) const = &IceShelf::solve;
    Field<2> (IceShelf::*prognostic_solve)(const double dt, const Field<2>&, const Field<2>&, const VectorField<2>&, const Field<2>&) const = &IceShelf::solve;

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

