#include "polypartition.h"
#include "Eigen/Dense"
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <utility>

// function to partition boundary polygon (V-rep) given a set of hole polygons
std::vector<Eigen::Matrix<double, -1, 2>> hertel_mehlhorn(const Eigen::Matrix<double, -1, 2>& bdy, const std::vector<Eigen::Matrix<double, -1, 2>>& holes);

// internal functions
TPPLPoly make_poly(const Eigen::Matrix<double, -1, 2>& m, bool is_hole);
Eigen::Matrix<double, -1, 2> poly_2_eigen(const TPPLPoly& poly);
Eigen::Matrix<double, -1, 2> sort_verts(const Eigen::Matrix<double, -1, 2>& m);