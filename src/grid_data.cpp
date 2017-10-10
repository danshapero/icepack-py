
#include <icepack/grid_data.hpp>
#include "icepack_py.hpp"

namespace icepack
{
  void bind_grid_data(py::module& module)
  {
    py::class_<GridData<2>>(module, "GridData2")
      .def("__call__",
           [](const GridData<2>& data, const dealii::Point<2>& x)
           {
             return data(x).get();
           });

    module.def("read_arc_ascii_grid",
               [](py::object& file)
               {
                 py::object read = file.attr("read");
                 py::str py_text = read();
                 std::string text(py_text);
                 std::istringstream stream(text);
                 return read_arc_ascii_grid(stream);
               },
               "Read a gridded data set stored in the ESRI Arc/Info format");
  }
}

