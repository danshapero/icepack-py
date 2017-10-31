
#include <icepack/numerics/optimization.hpp>
#include <icepack/physics/viscosity.hpp>
#include <icepack/physics/friction.hpp>
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


  void bind_physics(py::module& module)
  {
    bind_newton_search_options<VectorField<2>>(module, "VectorField2");

    module.def("rate_factor", &icepack::rate_factor,
               "Compute the rate factor `A` in Glen's flow law");

    py::class_<MembraneStress>(module, "MembraneStress")
      .def(py::init<const double>(),
           py::arg("n") = 3.0);

    py::class_<Viscosity>(module, "Viscosity")
      .def(py::init<const MembraneStress&>(),
           py::arg("membrane_stress") = MembraneStress());

    py::class_<BasalStress>(module, "BasalStress")
      .def(py::init<const double>(),
           py::arg("n") = 3.0);

    py::class_<Friction>(module, "Friction")
      .def(py::init<const Friction&>());
  }
}

