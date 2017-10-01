
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

    py::class_<Discretization<2>, std::shared_ptr<Discretization<2>>>(module, "Discretization2")
      .def_property_readonly("triangulation", &Discretization<2>::triangulation);

    module.def("make_discretization", &icepack::make_discretization<2>,
               py::keep_alive<0, 1>());

    py::class_<Field<2>>(module, "Field2")
      .def(py::init<std::shared_ptr<const Discretization<2>>>())
      .def_property_readonly("discretization", &Field<2>::discretization)
      .def("write_ucd",
           [](const Field<2>& u, py::object& file)
           {
             std::ostringstream stream;
             u.write_ucd("u", stream);
             py::object write = file.attr("write");
             write(stream.str());
           });

    py::class_<VectorField<2>>(module, "VectorField2")
      .def(py::init<std::shared_ptr<const Discretization<2>>>())
      .def_property_readonly("discretization", &VectorField<2>::discretization)
      .def("write_ucd",
           [](const VectorField<2>& u, py::object& file)
           {
             std::ostringstream stream;
             u.write_ucd("u", stream);
             py::object write = file.attr("write");
             write(stream.str());
           });
  }
}

