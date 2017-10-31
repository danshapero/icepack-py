
#include <icepack/physics/mass_transport.hpp>
#include "icepack_py.hpp"

namespace icepack
{
  void bind_mass_transport(py::module& module)
  {
    using boundary_ids = std::set<dealii::types::boundary_id>;

    module.def("compute_timestep", &compute_timestep,
               "Compute a timestep that gives a max Courant number for the "
               "given velocity field");

    py::class_<MassTransport>(module, "MassTransport")
      .def(py::init<const boundary_ids&>(),
           "Create an object for modelling the thickness evolution of floating"
           " ice shelves")
      .def("solve", &MassTransport::solve,
           "Solve the prognostic equations for the ice thickness",
           py::arg("dt"),
           py::arg("thickness"),
           py::arg("accumulation"),
           py::arg("velocity"),
           py::arg("inflow_thickness"));

  }
}
