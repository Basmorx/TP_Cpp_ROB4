#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <Eigen/Dense>
#include "CJoint.h"

class CBras {
private:
    std::vector<std::unique_ptr<CJoint>> joints_;

public:
    // ==========================================
    // RÈGLE DES CINQ
    // ==========================================
    CBras() = default;
    
    ~CBras() = default; // 

    // Constructeur de copie avec clone() 
    CBras(const CBras& other) {
        for (const auto& joint : other.joints_) {
            joints_.push_back(joint->clone());
        }
    }

    // Constructeur de déplacement 
    CBras(CBras&& other) noexcept = default;

    // Opérateur d'affectation par copie (Copy-and-Swap) 
    // Note: 'other' est passé par valeur, ce qui déclenche une copie automatique
    CBras& operator=(CBras other) {
        std::swap(joints_, other.joints_);
        return *this;
    }

    // Opérateur d'affectation par déplacement 
    CBras& operator=(CBras&& other) noexcept = default;

    // ==========================================
    // MÉTHODES EXISTANTES
    // ==========================================
    void addJoint(std::unique_ptr<CJoint> joint) {
        if (joint) joints_.push_back(std::move(joint));
    }

    size_t getNbJoints() const { return joints_.size(); }

    CJoint* getJoint(size_t index) const {
        if (index >= joints_.size()) throw std::out_of_range("Index invalide");
        return joints_[index].get();
    }

    Mat4 computeFK() const {
        Mat4 T = Mat4::Identity();
        for (const auto& joint : joints_) T = T * joint->getTransform();
        return T;
    }

    // ==========================================
    // INTERFACE EIGEN::VECTORXD 
    // ==========================================
    
    Eigen::VectorXd get() const { 
        Eigen::VectorXd q(getNbJoints());
        for (size_t i = 0; i < getNbJoints(); ++i) {
            q[i] = joints_[i]->getQ();
        }
        return q;
    }

    void set(const Eigen::VectorXd& q) {
        if (q.size() != getNbJoints()) {
            throw std::invalid_argument("Taille du vecteur incorrecte");
        }
        for (size_t i = 0; i < getNbJoints(); ++i) {
            joints_[i]->setQ(q[i]);
        }
    }

    Eigen::VectorXd random() const {
        Eigen::VectorXd q(getNbJoints());
        for (size_t i = 0; i < getNbJoints(); ++i) {
            double min = joints_[i]->getQMin();
            double max = joints_[i]->getQMax();
            // Génération d'une valeur aléatoire entre min et max
            q[i] = min + (max - min) * ((double)rand() / RAND_MAX);
        }
        return q;
    }

    friend std::ostream& operator<<(std::ostream& os, const CBras& bras) {
        os << "Bras [" << bras.getNbJoints() << " DDL]\n";
        for (size_t i = 0; i < bras.joints_.size(); ++i) {
            const auto& j = bras.joints_[i];
            os << "  [" << i << "] " << j->getTypeName() << "\n";
            os << std::fixed << std::setprecision(3);
            os << "      q=" << j->getQ() << (j->getTypeName() == "Revolute" ? " rad\n" : " m\n");
            os << "      bornes = [" << std::setw(6) << j->getQMin() << ", " << std::setw(6) << j->getQMax() << "]\n";
        }
        return os;
    }
};