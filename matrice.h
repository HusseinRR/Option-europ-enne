#ifndef MATRICE_H
#define MATRICE_H

#include <vector>
#include "Vecteur.h"

class Matrice {
private:
    size_t n_;
    // For simplicity, store everything in a dense 2D array (row-major)
    // This is not memory-efficient for large FE systems,
    // but it is simpler for demonstration.
    std::vector<double> data_; 

    // Accessor for element (i,j)
    double &elem(size_t i, size_t j) { return data_[i*n_ + j]; }
    const double &elem(size_t i, size_t j) const { return data_[i*n_ + j]; }

public:
    // Constructors
    Matrice() : n_(0) {}
    explicit Matrice(size_t n) : n_(n), data_(n*n, 0.0) {}

    void resize(size_t n) {
        n_ = n;
        data_.assign(n*n, 0.0);
    }

    size_t size() const { return n_; }

    // (i,j) indexing
    double &operator()(size_t i, size_t j) { return elem(i,j); }
    const double &operator()(size_t i, size_t j) const { return elem(i,j); }

    // Matrix-vector product
    Vecteur operator*(const Vecteur &v) const {
        Vecteur result(n_);
        for (size_t i=0; i<n_; i++) {
            double sum = 0.0;
            for (size_t j=0; j<n_; j++) {
                sum += elem(i,j)*v[j];
            }
            result[i] = sum;
        }
        return result;
    }

    // Matrix addition: this += B
    Matrice &operator+=(const Matrice &B) {
        for (size_t i=0; i<n_*n_; i++) {
            data_[i] += B.data_[i];
        }
        return *this;
    }

    // axpy: this = this + alpha*B
    void axpy(double alpha, const Matrice &B) {
        for (size_t i=0; i<n_*n_; i++) {
            data_[i] += alpha*B.data_[i];
        }
    }

    // LU factorization (naive, no pivoting)
    // Factor in place: data_ becomes the combined L and U
    // For serious usage, partial pivoting or a robust solver is needed.
    void factorLU() {
        for (size_t k=0; k<n_; k++) {
            // No pivoting check, assume no zero pivot
            for (size_t i=k+1; i<n_; i++) {
                elem(i,k) /= elem(k,k);
                for (size_t j=k+1; j<n_; j++) {
                    elem(i,j) -= elem(i,k)*elem(k,j);
                }
            }
        }
    }

    // Solve for x in LU x = b (in-place on x)
    void solveLU(Vecteur &x) const {
        // Forward substitution for L
        for (size_t i=1; i<n_; i++) {
            double sum = x[i];
            for (size_t j=0; j<i; j++) {
                sum -= elem(i,j)*x[j];
            }
            x[i] = sum;
        }
        // Back substitution for U
        for (int i=(int)n_-1; i>=0; i--) {
            double sum = x[i];
            for (size_t j=i+1; j<n_; j++) {
                sum -= elem(i,j)*x[j];
            }
            x[i] = sum / elem(i,i);
        }
    }
};

#endif // MATRICE_H
