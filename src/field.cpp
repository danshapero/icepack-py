
#include <icepack/field.hpp>
#include <icepack/mesh.hpp>
#include "icepack_py.hpp"

using icepack::Discretization;
using icepack::Field;
using icepack::VectorField;

namespace icepack
{
  void bind_field(py::module& module)
  {
    module.def("read_msh", &icepack::read_msh,
               "Read a .msh file into a Triangulation");

    // Note the use of the `keep_alive` call policy. This ensures that the python
    // wrapper object for discretization keeps a weak reference to the wrapper
    // for the triangulation. Without this protection, the python garbage
    // collector might clean up the triangulation before the discretization.
    py::class_<Discretization<2>>(module, "Discretization2")
      .def(py::init<const dealii::Triangulation<2>&, const unsigned int>(),
           py::keep_alive<1, 2>());

    py::class_<Field<2>>(module, "Field2")
      .def(py::init<const Discretization<2>&>(), py::keep_alive<1, 2>());

    py::class_<VectorField<2>>(module, "VectorField2")
      .def(py::init<const Discretization<2>&>(), py::keep_alive<1, 2>());
  }
}

