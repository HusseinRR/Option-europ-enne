#ifndef VECTEUR_H
#define VECTEUR_H

#include <vector>
#include <iostream>

class Vecteur {
private:
    std::vector<double> data_;

public:
    // Constructors
    Vecteur() = default;
    explicit Vecteur(size_t n) : data_(n, 0.0) {}
    Vecteur(const Vecteur &other) = default;
    Vecteur &operator=(const Vecteur &other) = default;

    // Resize
    void resize(size_t n) { data_.resize(n, 0.0); }

    // Size
    size_t size() const { return data_.size(); }

    // Indexing
    double &operator[](size_t i) { return data_[i]; }
    const double &operator[](size_t i) const { return data_[i]; }

    // Operators
    Vecteur &operator+=(const Vecteur &rhs) {
        for (size_t i=0; i<data_.size(); ++i)
            data_[i] += rhs.data_[i];
        return *this;
    }

    Vecteur &operator-=(const Vecteur &rhs) {
        for (size_t i=0; i<data_.size(); ++i)
            data_[i] -= rhs.data_[i];
        return *this;
    }

    Vecteur &operator*=(double alpha) {
        for (auto &val : data_) {
            val *= alpha;
        }
        return *this;
    }

    // Print (for debugging)
    void print(std::ostream &os = std::cout) const {
        os << "[ ";
        for (auto v : data_) os << v << " ";
        os << "]\n";
    }
};

// Free function: dot product
inline double dot(const Vecteur &v1, const Vecteur &v2) {
    double s = 0.0;
    for (size_t i=0; i<v1.size(); i++) {
        s += v1[i]*v2[i];
    }
    return s;
}

// Scalar multiplication
inline Vecteur operator*(double alpha, const Vecteur &v) {
    Vecteur res(v);
    for (size_t i=0; i<res.size(); i++) {
        res[i] *= alpha;
    }
    return res;
}

#endif // VECTEUR_H
