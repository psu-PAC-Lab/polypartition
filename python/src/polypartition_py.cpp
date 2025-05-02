#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
namespace py = pybind11;

#include "hm_eigen.hpp"

PYBIND11_MODULE(_core, m) 
{
    m.doc() = "Python bindings for the polypartition library. Only the Hertel and Mehlhorn algorithm is being used currently.";

    m.def("hertel_mehlhorn", &hertel_mehlhorn, "Partition a polygon using the Hertel-Mehlhorn algorithm",
          py::arg("bdy"), py::arg("holes"));
}