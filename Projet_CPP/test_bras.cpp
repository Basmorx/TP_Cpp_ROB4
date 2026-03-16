#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "CBras.h"
#include "CVecteur.h" 

// 3.a. Bras vide retourne I4
TEST(BrasTest, EmptyFKReturnsIdentity) { // [cite: 79]
    CBras bras;
    Mat4 T = bras.computeFK();
    Mat4 I = Mat4::Identity(); // [cite: 79]
    
    EXPECT_TRUE(T.isApprox(I, 1e-10));
}

// 3.b. 1 joint rotatif, extraction translation
TEST(BrasTest, RevoluteFKTranslationExtraction) { // [cite: 80]
    CBras bras;
    // theta = 0, dx = 0.5m
    bras.addJoint(std::make_unique<CJointRevolute>(-M_PI, M_PI, 0.5)); // [cite: 80]
    bras.getJoint(0)->setQ(0.0); // [cite: 80]
    
    Mat4 T = bras.computeFK();
    
    // Extraction avec T.block<3, 1>(0,3)
    Eigen::Vector3d translation = T.block<3, 1>(0, 3); // [cite: 80]
    Eigen::Vector3d expected(0.5, 0.0, 0.0); // [cite: 80]
    
    EXPECT_TRUE(translation.isApprox(expected, 1e-10));
}

// 3.c. getJoint lève une exception
TEST(BrasTest, GetJointOutOfBoundsThrows) { // [cite: 81]
    CBras bras;
    bras.addJoint(std::make_unique<CJointRevolute>(-M_PI, M_PI, 0.5));
    
    EXPECT_NO_THROW(bras.getJoint(0));
    EXPECT_THROW(bras.getJoint(1), std::out_of_range); // [cite: 81]
}



// ==========================================
// TESTS SÉANCE 3
// ==========================================

// Test 3.1 : Vérification de la copie profonde (Règle des cinq)
TEST(BrasTest, DeepCopyIsIndependent) {
    CBras bras1;
    bras1.addJoint(std::make_unique<CJointRevolute>(-M_PI, M_PI));
    bras1.getJoint(0)->setQ(1.0);
    
    // On utilise le constructeur de copie
    CBras bras2 = bras1;
    
    // On modifie la copie
    bras2.getJoint(0)->setQ(2.0);
    
    // On vérifie que l'original n'a pas été affecté
    EXPECT_DOUBLE_EQ(bras1.getJoint(0)->getQ(), 1.0);
    EXPECT_DOUBLE_EQ(bras2.getJoint(0)->getQ(), 2.0);
}

// Test 3.2.b : Idempotence de set(get()) [cite: 93]
TEST(BrasTest, EigenInterfaceIdempotence) {
    CBras bras;
    bras.addJoint(std::make_unique<CJointRevolute>(-M_PI, M_PI));
    bras.addJoint(std::make_unique<CJointPrismatic>(0.0, 1.0));
    
    // On donne des valeurs initiales
    bras.getJoint(0)->setQ(0.5);
    bras.getJoint(1)->setQ(0.2);
    
    Eigen::VectorXd q_initial = bras.get();
    
    // On applique set(get())
    bras.set(bras.get());
    
    // L'état ne doit pas avoir changé
    Eigen::VectorXd q_final = bras.get();
    EXPECT_TRUE(q_initial.isApprox(q_final));
}

// Test 3.3.c : Template CVecteur [cite: 97]
TEST(VecteurTest, OperationsAndNorm) {
    CVecteur<double, 3> u{1.0, 0.0, 0.0};
    CVecteur<double, 3> v{0.0, 1.0, 0.0};
    
    // Norme de u = 1 [cite: 97]
    EXPECT_DOUBLE_EQ(u.norm(), 1.0);
    
    // L'énoncé demande "u - v = 0"[cite: 97], ce qui est mathématiquement faux car (1,0,0) - (0,1,0) = (1,-1,0). 
    // C'est sûrement une coquille pour le produit scalaire (u.v = 0) ou (u - u = 0). On teste les deux :
    EXPECT_DOUBLE_EQ(u.dot(v), 0.0); // u.v = 0 (orthogonaux)
    
    CVecteur<double, 3> diff = u - u; // u - u = 0
    EXPECT_DOUBLE_EQ(diff.norm(), 0.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}