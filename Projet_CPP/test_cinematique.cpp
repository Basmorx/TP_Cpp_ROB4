#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "CBras.h"
#include "CJoint.h"

// a. computeFK() sur un bras vide retourne I4 
TEST(CBrasTest, ComputeFKEmptyArm) {
    CBras bras;
    Mat4 T = bras.computeFK();
    Mat4 I4 = Mat4::Identity();
    
    // On vérifie que la matrice retournée est proche de l'identité
    EXPECT_TRUE(T.isApprox(I4, 1e-10));
}

// b. 1 joint rotatif (theta=0, dx=0.5m) : vérifier l'effecteur en (0.5, 0, 0) 
TEST(CBrasTest, ComputeFKOneRevoluteJoint) {
    CBras bras;
    
    // Ajout d'un joint rotatif avec des bornes arbitraires et dx = 0.5
    // Par défaut, q est initialisé à 0.0 dans le constructeur de CJoint.
    bras.addJoint(std::make_unique<CJointRevolute>(-M_PI, M_PI, 0.5));
    
    Mat4 T = bras.computeFK();
    
    // Extraire la translation avec T.block<3, 1>(0,3) 
    Eigen::Vector3d translation = T.block<3, 1>(0, 3);
    Eigen::Vector3d expected(0.5, 0.0, 0.0);
    
    // On vérifie que la position de l'effecteur est bien celle attendue
    EXPECT_TRUE(translation.isApprox(expected, 1e-10));
}

// c. getJoint(i) lève std::out_of_range pour i >= N 
TEST(CBrasTest, GetJointOutOfRange) {
    CBras bras;
    
    // Le bras est vide (N=0), donc l'indice 0 est déjà >= N 
    EXPECT_THROW(bras.getJoint(0), std::out_of_range);
    
    // Ajout d'un joint (N=1)
    bras.addJoint(std::make_unique<CJointRevolute>(-M_PI, M_PI));
    
    // L'indice 0 doit fonctionner maintenant, mais l'indice 1 (>= N) doit lever une exception 
    EXPECT_NO_THROW(bras.getJoint(0));
    EXPECT_THROW(bras.getJoint(1), std::out_of_range);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}