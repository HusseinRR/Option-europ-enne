#include <iostream>
#include <fstream>
#include <cmath>
#include "Maillage.h"
#include "Matrice.h"
#include "Vecteur.h"

// Declaration of assembler from assemble.cpp:
void assembleFE(const Maillage &mesh,
                Matrice &M, Matrice &K, Matrice &B,
                const double Xi[2][2], double r);

int main()
{
    // ------------------
    // 1) Set PDE/option parameters
    double Xi[2][2] = { {0.04, -0.024},
                        {-0.024, 0.04} };
    double r   = 0.05;  // interest rate
    double K   = 100.0; // strike (example)
    double T   = 2.0;   // maturity in years
    double a   = 200.0; // domain cutoff: we do [0,a] x [0,a]

    // Time stepping
    int    Nt = 100;  // number of time steps
    double dt = T / Nt;

    // ------------------
    // 2) Build mesh
    Maillage mesh;
    size_t Nx = 50;  // 50 subdivisions in x
    size_t Ny = 50;  // 50 subdivisions in y
    mesh.buildRectangle(0.0, a, 0.0, a, Nx, Ny);

    std::cout << "Mesh created: Npoints=" << mesh.nbPoints() 
              << ", Ntri=" << mesh.nbTriangles() << std::endl;

    // ------------------
    // 3) Assemble FE matrices
    ::Matrice M, Kmat, B;
    assembleFE(mesh, M, Kmat, B, Xi, r);

    size_t N = mesh.nbPoints();
    std::cout << "Matrices assembled, dimension = " << N << std::endl;

    // 4) Build D = K + B + r*M
    Matrice D(N);
    for (size_t i=0; i<N; i++) {
        for (size_t j=0; j<N; j++) {
            D(i,j) = Kmat(i,j) + B(i,j) + r*M(i,j);
        }
    }

    // 5) Build the initial condition vector Q0:  (x1 + x2 - K)^+
    // We interpret "K" as the strike, so let's rename "strike" to avoid confusion
    double strike = K;
    Vecteur Q0(N);
    for (size_t i=0; i<N; i++) {
        double x1 = 0.0; 
        double x2 = 0.0;
        // get the node coordinate
        x1 = mesh.getPoint(i).x;
        x2 = mesh.getPoint(i).y;
        double payoff = x1 + x2 - strike;
        if (payoff < 0.0) payoff = 0.0;
        Q0[i] = payoff;
    }

    // 6) We solve PDE in forward time 
    // PDE: M dP/dt + D P = 0 
    // or dP/dt + M^{-1}D P = 0
    // We'll do the backward Euler:  (M + dt D) P^{k+1} = M P^k
    // We'll factor E = (M + dt D) once.

    // Build E = M + dt D
    Matrice E(N);
    for (size_t i=0; i<N; i++) {
        for (size_t j=0; j<N; j++) {
            E(i,j) = M(i,j) + dt*D(i,j);
        }
    }

    // Factor E in place (naive LU)
    E.factorLU();

    // Initialize solution at time 0
    Vecteur Pk = Q0;

    // 7) Time loop
    for (int k=0; k<Nt; k++) {
        // RHS = M P^k
        Vecteur Rk = M * Pk;

        // Solve E * P^(k+1) = Rk
        E.solveLU(Rk); // Rk becomes P^(k+1)

        Pk = Rk;

        // Optionally write to disk at some intervals
        if ((k+1) % 10 == 0) { 
            std::ofstream fout("Sol_time_" + std::to_string(k+1) + ".dat");
            for (size_t i=0; i<N; i++) {
                double xx = mesh.getPoint(i).x;
                double yy = mesh.getPoint(i).y;
                fout << xx << " " << yy << " " << Pk[i] << "\n";
            }
            fout.close();
            std::cout << "Wrote solution at time step " << (k+1) 
                      << " to file.\n";
        }
    }

    // Done
    std::cout << "Finished time-stepping.\n";

    return 0;
}
