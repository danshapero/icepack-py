
#include <icepack/field.hpp>
#include "icepack_py.hpp"

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


  void bind_interpolate(py::module& module)
  {
    Field<2> (*scalar_interp)(const Discretization<2>&, const py::function&) =
      &icepack::py_interpolate;
    module.def("interpolate", scalar_interp,
               "Interpolate an analytically defined scalar field to the finite"
               " element basis");

    VectorField<2> (*vector_interp)(
      const Discretization<2>&,
      const py::function&,
      const py::function&
    ) = &icepack::py_interpolate;
    module.def("interpolate", vector_interp,
               "Interpolate an analytically defined vector field to the finite"
               " element basis");
  }

}
