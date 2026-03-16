#pragma once
#include <array>
#include <cmath>
#include <iostream>
#include <Eigen/Dense>

template<typename T, std::size_t N> // 
class CVecteur {
private:
    std::array<T, N> data_; // 

public:
    // Initialisation
    CVecteur() { data_.fill(T{}); }
    CVecteur(std::initializer_list<T> init) {
        std::copy(init.begin(), init.begin() + std::min(init.size(), N), data_.begin());
    }

    // Accès
    T& operator[](std::size_t i) { return data_[i]; } // 
    const T& operator[](std::size_t i) const { return data_[i]; }

    // Opérations
    CVecteur operator+(const CVecteur& other) const { // 
        CVecteur res;
        for (std::size_t i = 0; i < N; ++i) res[i] = data_[i] + other.data_[i];
        return res;
    }

    CVecteur operator-(const CVecteur& other) const { // 
        CVecteur res;
        for (std::size_t i = 0; i < N; ++i) res[i] = data_[i] - other.data_[i];
        return res;
    }

    CVecteur operator*(T scalaire) const { // 
        CVecteur res;
        for (std::size_t i = 0; i < N; ++i) res[i] = data_[i] * scalaire;
        return res;
    }

    T dot(const CVecteur& other) const { // 
        T sum = 0;
        for (std::size_t i = 0; i < N; ++i) sum += data_[i] * other.data_[i];
        return sum;
    }

    T norm() const { // 
        return std::sqrt(this->dot(*this));
    }

    // Conversion Eigen via Map (Zéro copie)
    Eigen::Matrix<T, N, 1> toEigen() const { // 
        return Eigen::Map<const Eigen::Matrix<T, N, 1>>(data_.data());
    }

    // Affichage
    friend std::ostream& operator<<(std::ostream& os, const CVecteur& v) { // 
        os << "(";
        for (std::size_t i = 0; i < N; ++i) {
            os << v.data_[i] << (i < N - 1 ? ", " : "");
        }
        os << ")";
        return os;
    }
};