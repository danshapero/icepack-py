
#include <deal.II/base/point.h>
#include <deal.II/grid/tria.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include "icepack_py.hpp"

using dealii::Point;
using dealii::Triangulation;
using dealii::types::boundary_id;

using Edge = std::tuple<unsigned int, unsigned int, bool, boundary_id>;

PYBIND11_MAKE_OPAQUE(std::vector<Point<2>>)
PYBIND11_MAKE_OPAQUE(std::vector<boundary_id>)

namespace icepack
{
  std::vector<Edge> get_all_edges(const dealii::Triangulation<2>& tria)
  {
    std::vector<Edge> edges;
    for (Triangulation<2>::active_face_iterator edge = tria.begin_active_face();
         edge != tria.end_face();
         ++edge)
    {
      const unsigned int i = edge->vertex_index(0);
      const unsigned int j = edge->vertex_index(1);
      const bool at_boundary = edge->at_boundary();
      edges.push_back(std::make_tuple(i, j, at_boundary, edge->boundary_id()));
    }

    return edges;
  }

  void bind_dealii(py::module& module)
  {
    py::class_<Point<2>>(module, "Point2")
      .def(py::init<const double, const double>())
      .def("__getitem__",
           py::overload_cast<const unsigned int>(&Point<2>::operator(), py::const_),
           "Get one of the point's coordinates");

    py::bind_vector<std::vector<Point<2>>>(module, "VectorPoint2");
    py::bind_vector<std::vector<boundary_id>>(module, "VectorBoundaryID");

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
      .def("get_boundary_ids",
           &Triangulation<2>::get_boundary_ids,
           "Return an array of all the numeric boundary IDs")
      .def("refine_global",
           &Triangulation<2>::refine_global,
           "Subdivide all the cells of a mesh for greater accuracy");

    py::bind_vector<std::vector<Edge>>(module, "VectorEdge");
    module.def("_get_all_edges", &get_all_edges,
               "Return an array of tuples consisting of the two endpoints of "
               "each triangulation edge, a boolean to indicate if the edge is "
               "on the boundary, and a boundary ID if so. This is an internal "
               "routine used to support plotting triangulations.");

  }
}

