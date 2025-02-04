#include <cmath>
#include "Maillage.h"
#include "Matrice.h"
#include "Vecteur.h"
#include <iostream>

// Covariance matrix Xi = [ [Xi11, Xi12],[Xi21, Xi22] ]
// We define A(x) = 0.5 * [ [Xi11*x1^2, Xi12*x1*x2], [Xi21*x1*x2, Xi22*x2^2 ] ]
// We define V(x) = ( (Xi11 + 0.5*Xi21 - r)*x1, (Xi22 + 0.5*Xi12 - r)*x2 )

// For P1 local basis on a triangle with vertices (x1,y1), (x2,y2), (x3,y3), 
// the local mass/stiffness can be computed by standard formulae.

static double areaTriangle(const Point &p1, const Point &p2, const Point &p3)
{
    // Return area of triangle
    double x1 = p2.x - p1.x;
    double y1 = p2.y - p1.y;
    double x2 = p3.x - p1.x;
    double y2 = p3.y - p1.y;
    double det = x1*y2 - x2*y1;
    return 0.5 * std::fabs(det);
}

// Gradient of P1 basis function "i" in local triangle
// For node i, the gradient is constant inside the triangle.
static void localGradient(const Point &p1, const Point &p2, const Point &p3,
                          double grad[3][2])
{
    // Suppose the local shape functions are L1, L2, L3.
    // grad(L1) = [(y2-y3), (x3-x2)] / (2*area)
    // etc.  We'll compute systematically:

    double x1 = p1.x, y1 = p1.y;
    double x2 = p2.x, y2 = p2.y;
    double x3 = p3.x, y3 = p3.y;

    double A = areaTriangle(p1,p2,p3);

    // grad L1
    grad[0][0] = (y2 - y3)/(2.0*A);
    grad[0][1] = (x3 - x2)/(2.0*A);
    // grad L2
    grad[1][0] = (y3 - y1)/(2.0*A);
    grad[1][1] = (x1 - x3)/(2.0*A);
    // grad L3
    grad[2][0] = (y1 - y2)/(2.0*A);
    grad[2][1] = (x2 - x1)/(2.0*A);
}

// Evaluate the PDE data A(x) and V(x) at point x
// For clarity, pass Xi and r explicitly
static void compute_A_V(const double Xi[2][2], double r,
                        double x1, double x2,
                        double A_mat[2][2],
                        double V_vec[2])
{
    // A(x) = 0.5 * [ [Xi11*x1^2, Xi12*x1*x2],
    //                [Xi21*x1*x2, Xi22*x2^2 ] ]
    A_mat[0][0] = 0.5 * Xi[0][0] * x1*x1;
    A_mat[0][1] = 0.5 * Xi[0][1] * x1*x2;
    A_mat[1][0] = 0.5 * Xi[1][0] * x1*x2;
    A_mat[1][1] = 0.5 * Xi[1][1] * x2*x2;

    // V(x) = ( (Xi11 + 0.5*Xi21 - r)*x1, (Xi22 + 0.5*Xi12 - r)*x2 )
    double vx = (Xi[0][0] + 0.5*Xi[1][0] - r)*x1;
    double vy = (Xi[1][1] + 0.5*Xi[0][1] - r)*x2;
    V_vec[0] = vx;
    V_vec[1] = vy;
}

// Assembly of mass matrix M, diffusion matrix K, and convection matrix B
// M, K, B are all NxN, where N = mesh.nbPoints().
// Xi is covariance, r is interest, etc.
void assembleFE(const Maillage &mesh,
                Matrice &M, Matrice &K, Matrice &B,
                const double Xi[2][2], double r)
{
    size_t N = mesh.nbPoints();
    M.resize(N);
    K.resize(N);
    B.resize(N);

    // Loop over triangles
    size_t nTri = mesh.nbTriangles();
    for (size_t it=0; it<nTri; it++) {
        const Triangle &T = mesh.getTriangle(it);
        int i1 = T.i1, i2 = T.i2, i3 = T.i3;
        const Point &p1 = mesh.getPoint(i1);
        const Point &p2 = mesh.getPoint(i2);
        const Point &p3 = mesh.getPoint(i3);

        double A = areaTriangle(p1,p2,p3);
        double gradL[3][2];
        localGradient(p1,p2,p3, gradL);

        // For a constant approximation of A(x), V(x), evaluate at centroid:
        double xc = (p1.x + p2.x + p3.x)/3.0;
        double yc = (p1.y + p2.y + p3.y)/3.0;
        double A_mat[2][2], V_vec[2];
        compute_A_V(Xi, r, xc, yc, A_mat, V_vec);

        // Mass matrix local contributions: M_ij^loc = (1/12)*2A if i==j, (1/12)*2A if i!=j for P1
        // Actually, for P1 on triangle: M_loc = (A/12) [2 1 1; 1 2 1; 1 1 2].
        double M_loc[3][3];
        double factorM = A/12.0;
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                if (i==j) M_loc[i][j] = 2.0 * factorM;
                else      M_loc[i][j] = 1.0 * factorM;
            }
        }

        // Diffusion K_ij^loc = \int A(x) grad L_i . grad L_j
        // We'll approximate A(x) constant at centroid.
        // so K_ij^loc = [grad L_i]^T * A_mat * [grad L_j] * (Area).
        double K_loc[3][3];
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                double gx_i = gradL[i][0];
                double gy_i = gradL[i][1];
                double gx_j = gradL[j][0];
                double gy_j = gradL[j][1];

                // matrix-vector: (A_mat * gradL_j)
                double Ax = A_mat[0][0]*gx_j + A_mat[0][1]*gy_j;
                double Ay = A_mat[1][0]*gx_j + A_mat[1][1]*gy_j;

                double val = (gx_i*Ax + gy_i*Ay)*A; // multiply by area
                K_loc[i][j] = val;
            }
        }

        // Convection B_ij^loc = \int V(x).grad(L_j) L_i
        // approx V constant => B_ij^loc = V_vec . grad(L_j) * \int L_i
        // but actually \int L_i = A/3 on the triangle. We can do 
        // a small local integration approach:
        double B_loc[3][3];
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                double gx_j = gradL[j][0];
                double gy_j = gradL[j][1];
                double dotVg = (V_vec[0]*gx_j + V_vec[1]*gy_j); 
                // Then multiply by \int L_i => A/3
                // But a more standard approach is: 
                // B_ij^loc = \int_T (V dot grad L_j)(L_i) dT
                // We'll do midpoint rule =>  (V dot grad L_j)(centroid) * L_i(centroid)*Area
                // L_i(centroid) = 1/3 for P1 
                double Li_centroid = 1./3.;
                // So B_ij^loc ~ dotVg * Li_centroid * Area
                B_loc[i][j] = dotVg * Li_centroid * A;
            }
        }

        // Add local contributions to global
        int loc[3] = { i1, i2, i3 };
        for (int a=0; a<3; a++) {
            int Aglobal = loc[a];
            for (int b=0; b<3; b++) {
                int Bglobal = loc[b];
                M(Aglobal,Bglobal) += M_loc[a][b];
                K(Aglobal,Bglobal) += K_loc[a][b];
                B(Aglobal,Bglobal) += B_loc[a][b];
            }
        }
    }
}
