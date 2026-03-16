#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "CJoint.h"

class CBras {
private:
    std::vector<std::unique_ptr<CJoint>> joints_; // 

public:
    CBras() = default;

    // Ajout d'un joint
    void addJoint(std::unique_ptr<CJoint> joint) { // [cite: 43, 71]
        if (joint) {
            joints_.push_back(std::move(joint));
        }
    }

    // Nombre de joints
    size_t getNbJoints() const { // 
        return joints_.size();
    }

    // Accès à un joint spécifique
    CJoint* getJoint(size_t index) const { // 
        if (index >= joints_.size()) {
            throw std::out_of_range("Index de joint invalide"); // [cite: 81]
        }
        return joints_[index].get();
    }

    // Cinématique directe (T0 * T1 * ... * Tn-1)
    Mat4 computeFK() const { // [cite: 43, 74]
        Mat4 T = Mat4::Identity();
        for (const auto& joint : joints_) {
            T = T * joint->getTransform(); // 
        }
        return T;
    }

    // Surcharge de l'opérateur d'affichage
    friend std::ostream& operator<<(std::ostream& os, const CBras& bras) { // [cite: 43, 73]
        os << "// Bras [" << bras.getNbJoints() << " DDL]\n//\n"; // [cite: 55, 56]
        for (size_t i = 0; i < bras.joints_.size(); ++i) {
            const auto& j = bras.joints_[i];
            os << "// [" << i << "] " << j->getTypeName() << "\n"; // [cite: 57, 62, 67]
            
            os << std::fixed << std::setprecision(3);
            os << "//      q=" << j->getQ() << (j->getTypeName() == "Revolute" ? " rad\n" : " m\n"); // [cite: 58, 63, 68]
            // Formatage des bornes
            os << "//      bornes = [" << std::setw(6) << j->getQMin() << ", " 
               << std::setw(6) << j->getQMax() << "]\n//\n"; // [cite: 59, 60, 64, 65, 69, 70]
        }
        return os;
    }
};