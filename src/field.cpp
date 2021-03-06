
#include <icepack/field.hpp>
#include <icepack/mesh.hpp>
#include "icepack_py.hpp"

using icepack::Discretization;
using icepack::Field;
using icepack::VectorField;

namespace icepack
{
  template <int rank>
  void bind_field(py::module& module, const char * name)
  {
    using F = FieldType<rank, 2, primal>;
    py::class_<FieldType<rank, 2, primal>>(module, name)
      .def(py::init<std::shared_ptr<const Discretization<2>>>())
      .def_property_readonly("discretization", &F::discretization)
      .def("__add__", [](const F& u, const F& v) -> F {return u + v;})
      .def("__sub__", [](const F& u, const F& v) -> F {return u - v;})
      .def("__rmul__", [](const F& u, const double a) -> F {return a * u;})
      .def("__truediv__", [](const F& u, const double a) -> F {return u / a;})
      .def("__neg__", [](const F& u) -> F {return -u;})
      .def("write_ucd",
           [](const F& u, py::object& file)
           {
             std::ostringstream stream;
             u.write_ucd("u", stream);
             py::object write = file.attr("write");
             write(stream.str());
           });

    module.def("norm", &icepack::norm<rank, 2>,
               "Compute the L^2 norm of a field");
    module.def("dist", &icepack::dist<rank, 2>,
               "Compute the L^2-distance between two fields");
    module.def("max", &icepack::max<rank, 2>,
               "Compute the maximum absolute value of a field");

    double (*inner_product)(const FieldType<rank, 2>&, const FieldType<rank, 2>&) =
      &icepack::inner_product;
    module.def("inner_product", inner_product,
               "Compute the L^2-inner product of two fields");
  }

  void bind_field(py::module& module)
  {
    module.def("read_msh", &icepack::read_msh,
               "Read a .msh file into a Triangulation");

    py::class_<Discretization<2>, std::shared_ptr<Discretization<2>>>(
      module, "Discretization2")
      .def_property_readonly("triangulation", &Discretization<2>::triangulation);

    module.def("make_discretization", &icepack::make_discretization<2>);

    bind_field<0>(module, "Field2");
    bind_field<1>(module, "VectorField2");
  }
}

