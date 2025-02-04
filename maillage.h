#ifndef MAILLAGE_H
#define MAILLAGE_H

#include <vector>    // for std::vector
#include <cstddef>   // for std::size_t
#include <cmath>     // for std::fabs, etc.

// ---------------------------------------------------------------------
// Basic Point and Triangle structures
struct Point {
    double x, y;
    Point(double xx=0.0, double yy=0.0) : x(xx), y(yy) {}
};

struct Triangle {
    // Indices of the 3 vertices (for P1 elements)
    int i1, i2, i3;
};

// ---------------------------------------------------------------------
// Maillage class to store a uniform mesh of [ax_, bx_] x [ay_, by_]
class Maillage {
private:
    // Vector of node coordinates and vector of triangles
    std::vector<Point> points_;
    std::vector<Triangle> triangles_;

    // Number of subdivisions in each coordinate direction
    std::size_t Nx_, Ny_;

    // Domain corners
    double ax_, bx_, ay_, by_;

public:
    // Constructor
    Maillage()
        : Nx_(0), Ny_(0), ax_(0.0), bx_(1.0), ay_(0.0), by_(1.0)
    {
    }

    // Build a uniform rectangular mesh
    void buildRectangle(double ax, double bx,
                        double ay, double by,
                        std::size_t Nx, std::size_t Ny)
    {
        ax_ = ax;  bx_ = bx;
        ay_ = ay;  by_ = by;
        Nx_ = Nx;  Ny_ = Ny;

        points_.clear();
        triangles_.clear();

        double hx = (bx_ - ax_) / static_cast<double>(Nx_);
        double hy = (by_ - ay_) / static_cast<double>(Ny_);

        // 1) Create points
        points_.reserve((Nx_+1)*(Ny_+1));
        for (std::size_t j = 0; j <= Ny_; j++) {
            for (std::size_t i = 0; i <= Nx_; i++) {
                double xx = ax_ + i * hx;
                double yy = ay_ + j * hy;
                points_.emplace_back(xx, yy);
            }
        }

        // 2) Create triangles (two per cell)
        for (std::size_t j = 0; j < Ny_; j++) {
            for (std::size_t i = 0; i < Nx_; i++) {
                // indices in the 2D grid
                int idx  = static_cast<int>( j*(Nx_+1) + i );
                int idxR = idx+1;
                int idxU = idx + static_cast<int>(Nx_+1);
                int idxUR= idxU+1;

                Triangle T1{ idx, idxU, idxR };
                Triangle T2{ idxR, idxU, idxUR };

                triangles_.push_back(T1);
                triangles_.push_back(T2);
            }
        }
    }

    // Number of points (nodes)
    std::size_t nbPoints() const { return points_.size(); }

    // Number of triangles
    std::size_t nbTriangles() const { return triangles_.size(); }

    // Access to a given point
    // (returning a const reference to internal data is usually safe if we don't reorder or resize the vector)
    const Point& getPoint(std::size_t i) const {
        return points_[i];
    }

    // Access to a given triangle
    const Triangle& getTriangle(std::size_t i) const {
        return triangles_[i];
    }
};

#endif // MAILLAGE_H
