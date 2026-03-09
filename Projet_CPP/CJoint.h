#pragma once
#include <Eigen/Dense>
#include <string>
#include <memory>
#include <stdexcept>
#include <cmath>

// Alias pour la matrice de base compatible avec pinocchio::SE3
using Mat4 = Eigen::Matrix4d;

// ==========================================
// 1. Classe abstraite CJoint
// ==========================================
class CJoint {
protected:
    double q_;
    double qMin_;
    double qMax_;

public:
    CJoint(double qMin, double qMax) : q_(0.0), qMin_(qMin), qMax_(qMax) {}
    
    // Destructeur virtuel indispensable
    virtual ~CJoint() = default;

    // Mutateur avec vérification des bornes
    void setQ(double q) {
        if (q < qMin_ || q > qMax_) {
            throw std::out_of_range("Erreur: q est hors des limites [qMin, qMax]");
        }
        q_ = q;
    }

    // Accesseurs const
    double getQ() const { return q_; }
    double getQMin() const { return qMin_; }
    double getQMax() const { return qMax_; }

    // Méthodes virtuelles pures
    virtual Mat4 getTransform() const = 0;
    virtual std::string getTypeName() const = 0;
    virtual std::unique_ptr<CJoint> clone() const = 0;
};

// ==========================================
// 2. Classe dérivée CJointRevolute
// ==========================================
class CJointRevolute : public CJoint {
private:
    double dx_; // Lien dx

public:
    CJointRevolute(double qMin, double qMax, double dx = 0.0) 
        : CJoint(qMin, qMax), dx_(dx) {}

    Mat4 getTransform() const override {
        Mat4 T = Mat4::Identity();
        T(0, 0) = std::cos(q_);
        T(0, 1) = -std::sin(q_);
        T(0, 3) = dx_;
        T(1, 0) = std::sin(q_);
        T(1, 1) = std::cos(q_);
        return T;
    }

    std::string getTypeName() const override {
        return "Revolute";
    }

    std::unique_ptr<CJoint> clone() const override {
        auto copy = std::make_unique<CJointRevolute>(qMin_, qMax_, dx_);
        copy->q_ = this->q_; // On copie également la position actuelle
        return copy;
    }
};

// ==========================================
// 3. Classe dérivée CJointPrismatic
// ==========================================
class CJointPrismatic : public CJoint {
public:
    CJointPrismatic(double qMin, double qMax) : CJoint(qMin, qMax) {}

    Mat4 getTransform() const override {
        Mat4 T = Mat4::Identity();
        T(2, 3) = q_; // Translation sur l'axe z selon la valeur q_
        return T;
    }

    std::string getTypeName() const override {
        return "Prismatic";
    }

    std::unique_ptr<CJoint> clone() const override {
        auto copy = std::make_unique<CJointPrismatic>(qMin_, qMax_);
        copy->q_ = this->q_;
        return copy;
    }
};