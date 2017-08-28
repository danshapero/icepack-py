
#include <icepack/physics/ice_shelf.hpp>
#include <icepack/mesh.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>

namespace py = pybind11;
PYBIND11_MAKE_OPAQUE(std::vector<dealii::Point<2>>);

namespace icepack
{
  template <int dim>
  class PyFunction : public dealii::Function<dim>
  {
  public:
    PyFunction(const py::function& f) : f_(f)
    {}

    double value(const dealii::Point<dim>& x, const unsigned int = 0) const
    {
      py::object q = f_(x);
      return q.cast<double>();
    }

  protected:
    const py::function& f_;
  };


  Field<2> py_interpolate(
    const Discretization<2>& discretization,
    const py::function& f
  )
  {
    const PyFunction<2> F(f);
    return interpolate(discretization, F);
  }


  VectorField<2> py_interpolate(
    const Discretization<2>& discretization,
    const py::function& u,
    const py::function& v
  )
  {
    const PyFunction<2> U(u);
    const PyFunction<2> V(v);
    return interpolate(discretization, U, V);
  }

}


using dealii::Point;
using dealii::Triangulation;
using icepack::Discretization;
using icepack::Field;
using icepack::VectorField;

PYBIND11_MODULE(icepack_py, module)
{
  module.doc() = "icepack python bindings";

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
         "Refine all cells of the triangulation for better resolution");

  module.def("read_msh", &icepack::read_msh,
             "Read a .msh file into a Triangulation");


  // Note the use of the `keep_alive` call policy. This ensures that the python
  // wrapper object for discretization keeps a weak reference to the wrapper
  // for the triangulation. Without this protection, the python garbage
  // collector might clean up the triangulation before the discretization.
  py::class_<Discretization<2>>(module, "Discretization2")
    .def(py::init<const Triangulation<2>&, const unsigned int>(),
         py::keep_alive<1, 2>());

  py::class_<Field<2>>(module, "Field2")
    .def(py::init<const Discretization<2>&>(), py::keep_alive<1, 2>());

  py::class_<VectorField<2>>(module, "VectorField2")
    .def(py::init<const Discretization<2>&>(), py::keep_alive<1, 2>());

  Field<2> (*scalar_interp)(const Discretization<2>&, const py::function&) =
    &icepack::py_interpolate;
  module.def("interpolate", scalar_interp,
             "Interpolate a scalar field to the finite element basis",
             py::keep_alive<0, 1>());

  VectorField<2> (*vector_interp)(const Discretization<2>&, const py::function&, const py::function&) =
    &icepack::py_interpolate;
  module.def("interpolate", vector_interp,
             "Interpolate a vector field to the finite element basis",
             py::keep_alive<0, 1>());

  py::class_<icepack::MembraneStress>(module, "MembraneStress")
    .def(py::init<const double>(),
         py::arg("n") = 3.0);

  py::class_<icepack::Viscosity>(module, "Viscosity")
    .def(py::init<const icepack::MembraneStress&>(),
         py::keep_alive<1, 2>(),
         py::arg("membrane_stress") = icepack::MembraneStress());

  py::class_<icepack::IceShelf>(module, "IceShelf")
    .def(py::init<const std::set<dealii::types::boundary_id>&, const icepack::Viscosity&, const double>(),
         "Create an object for modelling the flow of floating ice shelves",
         py::keep_alive<1, 3>(),
         py::arg("dirichlet_boundary_ids") = std::set<dealii::types::boundary_id>{0},
         py::arg("viscosity") = icepack::Viscosity(icepack::MembraneStress()),
         py::arg("convergence_tolerance") = 1.0e-6)
    .def("solve", &icepack::IceShelf::solve);
}

