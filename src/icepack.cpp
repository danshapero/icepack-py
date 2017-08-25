
#include <icepack/utilities.hpp>
#include <icepack/discretization.hpp>
#include <icepack/field.hpp>
#include <icepack/mesh.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;
PYBIND11_MAKE_OPAQUE(std::vector<dealii::Point<2>>);

PYBIND11_MODULE(icepack_py, module)
{
  module.doc() = "icepack python bindings";

  using dealii::Point;
  py::class_<Point<2>>(module, "Point2")
    .def(py::init<const double, const double>())
    .def(
      "__getitem__",
      py::overload_cast<const unsigned int>(&Point<2>::operator(), py::const_),
      "Get one of the point's coordinates"
    );

  py::bind_vector<std::vector<Point<2>>>(module, "VectorPoint2");

  using dealii::Triangulation;
  py::class_<Triangulation<2>>(module, "Triangulation2")
    .def("n_active_cells",
         py::overload_cast<>(&Triangulation<2>::n_active_cells, py::const_),
         "Number of mesh cells (quads in 2d, hexes in 3d)")
    .def("n_vertices",
         &Triangulation<2>::n_vertices,
         "Number of mesh vertices")
    .def("get_vertices",
         &Triangulation<2>::get_vertices,
         "Get a reference to the array of mesh vertices");

  module.def("read_msh", &icepack::read_msh,
             "Read a .msh file into a Triangulation");

  using icepack::Discretization;
  py::class_<Discretization<2>>(module, "Discretization2")
    .def(py::init<const Triangulation<2>&, const unsigned int>());

  using icepack::Field;
  py::class_<Field<2>>(module, "Field2")
    .def(py::init<const Discretization<2>&>());

  using icepack::VectorField;
  py::class_<VectorField<2>>(module, "VectorField2")
    .def(py::init<const Discretization<2>&>());
}

