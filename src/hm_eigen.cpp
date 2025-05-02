#include "hm_eigen.hpp"

std::vector<Eigen::Matrix<double, -1, 2>> hertel_mehlhorn(const Eigen::Matrix<double, -1, 2>& bdy, const std::vector<Eigen::Matrix<double, -1, 2>>& holes)
{
    // Check if the boundary polygon is valid
    if (bdy.rows() < 3) 
    {
        throw std::invalid_argument("Boundary polygon must have at least 3 vertices.");
    }

    // Check if the holes are valid
    for (const auto& hole : holes) 
    {
        if (hole.rows() < 3) 
        {
            throw std::invalid_argument("Each hole polygon must have at least 3 vertices.");
        }
    }

    // polygon list
    TPPLPolyList polys;

    // boundary polygon
    TPPLPoly bdy_poly = make_poly(bdy, false);
    polys.push_back(bdy_poly);

    // holes
    for (const auto& hole : holes) 
    {
        TPPLPoly hole_poly = make_poly(hole, true);
        polys.push_back(hole_poly);
    }

    // remove holes
    TPPLPartition partition;
    TPPLPolyList polys_no_hole;
    int out_holes = partition.RemoveHoles(&polys, &polys_no_hole);
    if (out_holes == 0)
        throw std::runtime_error("Hole removal failed");

    TPPLPoly poly_nohole = *(polys_no_hole.begin());

    // partition
    TPPLPolyList polys_partitioned;
    int out_part = partition.ConvexPartition_HM(&poly_nohole, &polys_partitioned);
    if (out_part == 0)
        throw std::runtime_error("Partitioning failed");

    // return vector of Eigen matrices
    std::vector<Eigen::Matrix<double, -1, 2>> part;
    for (const TPPLPoly & poly : polys_partitioned) 
    {
        Eigen::Matrix<double, -1, 2> m = poly_2_eigen(poly);
        part.push_back(m);
    }
    
    return part;
}


TPPLPoly make_poly(const Eigen::Matrix<double, -1, 2>& m, bool is_hole)
{
    Eigen::Matrix<double, -1, 2> m_srt = sort_verts(m);
    TPPLPoly poly;
    poly.Init(m.rows());
    for (int i = 0; i < m_srt.rows(); i++) 
    {
        poly[i].x = m_srt(i, 0);
        poly[i].y = m_srt(i, 1);
    }
    poly.SetHole(is_hole);
    if (is_hole)
        poly.SetOrientation(TPPLOrientation::TPPL_ORIENTATION_CW);
    else
        poly.SetOrientation(TPPLOrientation::TPPL_ORIENTATION_CCW);
    return poly;
}

Eigen::Matrix<double, -1, 2> poly_2_eigen(const TPPLPoly& poly)
{
    Eigen::Matrix<double, -1, 2> m(poly.GetNumPoints(), 2);
    for (int i = 0; i < poly.GetNumPoints(); i++) 
    {
        m(i, 0) = poly[i].x;
        m(i, 1) = poly[i].y;
    }
    return m;
}

Eigen::Matrix<double, -1, 2> sort_verts(const Eigen::Matrix<double, -1, 2>& m)
{
    // convert to std::vector of Eigen::Vector2d
    std::vector<std::pair<double, double>> verts;
    for (int i = 0; i < m.rows(); i++) 
    {
        verts.push_back(std::make_pair(m(i, 0), m(i, 1)));
    }

    // get centroid
    double cx = 0.0;
    double cy = 0.0;
    for (const auto& vert : verts) 
    {
        cx += vert.first;
        cy += vert.second;
    }
    cx /= verts.size();
    cy /= verts.size();

    // sort the vertices
    std::sort(verts.begin(), verts.end(), [&](const std::pair<double, double>& a, const std::pair<double, double>& b) 
    {
        return std::atan2(a.second-cy, a.first-cx) < std::atan2(b.second-cy, b.first-cx);
    });

    // convert back to Eigen::Matrix
    Eigen::Matrix<double, -1, 2> m_srt (verts.size(), 2);
    for (int i = 0; i < verts.size(); i++) 
    {
        m_srt(i, 0) = verts[i].first;
        m_srt(i, 1) = verts[i].second;
    }
    return m_srt;
}