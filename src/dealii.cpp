
#include <deal.II/base/point.h>
#include <deal.II/grid/tria.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include "icepack_py.hpp"

PYBIND11_MAKE_OPAQUE(std::vector<dealii::Point<2>>);

using dealii::Point;
using dealii::Triangulation;

namespace icepack
{
  void bind_dealii(py::module& module)
  {
    py::class_<Point<2>>(module, "Point2")
      .def(py::init<const double, const double>())
      .def("__getitem__",
           py::overload_cast<const unsigned int>(&Point<2>::operator(), py::const_),
           "Get one of the point's coordinates");

    py::bind_vector<std::vector<Point<2>>>(module, "VectorPoint2");

    py::class_<Triangulation<2>>(module, "Triangulation2")
      .def("n_active_cells",
           py::overload_cast<>(&Triangulation<2>::n_active_cells, py::const_),
           "Number of mesh cells (quads in 2d, hexes in 3d)")
      .def("n_vertices",
           &Triangulation<2>::n_vertices,
           "Number of mesh vertices")
      .def("get_vertices",
           &Triangulation<2>::get_vertices,
           "Get a reference to the array of mesh vertices")
      .def("refine_global",
           &Triangulation<2>::refine_global,
           "Subdivide all the cells of a mesh for greater accuracy");
  }

}

