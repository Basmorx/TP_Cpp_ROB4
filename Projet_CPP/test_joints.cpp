#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "CJoint.h"

#include <pinocchio/parsers/urdf.hpp>
#include <pinocchio/algorithm/kinematics.hpp> 


// Test 1 : setQ() hors bornes lève std::out_of_range
TEST(JointTest, SetQOutOfBoundsThrowsException) {
    CJointRevolute joint(-3.14, 3.14); // Bornes de -Pi à Pi
    
    // Test au-dessus de la limite max
    EXPECT_THROW(joint.setQ(4.0), std::out_of_range);
    
    // Test en dessous de la limite min
    EXPECT_THROW(joint.setQ(-4.0), std::out_of_range);
    
    // Vérification qu'une valeur valide ne lève pas d'exception
    EXPECT_NO_THROW(joint.setQ(1.5));
}

// Test 2 : CJointRevolute avec angle 0 et dx=0 donne l'identité
TEST(JointTest, RevoluteGetTransformIdentity) {
    CJointRevolute joint(-3.14, 3.14, 0.0); // dx = 0
    joint.setQ(0.0); // angle = 0
    
    Mat4 T = joint.getTransform();
    Mat4 I = Mat4::Identity();
    
    // On vérifie que les matrices sont approximativement égales (tolérance classique)
    EXPECT_TRUE(T.isApprox(I, 1e-10));
}

// Test 3 : CJointPrismatic avec d=1 fait une translation correcte
TEST(JointTest, PrismaticGetTransformTranslation) {
    CJointPrismatic joint(0.0, 2.0);
    joint.setQ(1.0); // d = 1
    
    Mat4 T = joint.getTransform();
    Mat4 expectedT = Mat4::Identity();
    expectedT(2, 3) = 1.0; // Translation de 1 sur l'axe Z
    
    EXPECT_TRUE(T.isApprox(expectedT, 1e-10));
}

// Test 4 : Bonus Pinocchio - Comparaison avec le UR5
TEST(JointTest, BonusPinocchioUR5) {
    // 1. Chargement du modèle UR5
    // Note : Il te faudra un vrai fichier "ur5.urdf" dans ton dossier pour que ça passe.
    // Tu peux en télécharger un facilement sur les dépôts GitHub de ROS ou d'Universal Robots.
    const std::string urdf_path = "ur5.urdf"; 
    
    pinocchio::Model model;
    try {
        pinocchio::urdf::buildModel(urdf_path, model);
    } catch (...) {
        // Si le fichier n'est pas là, on ignore le test gentiment plutôt que de tout faire crasher
        GTEST_SKIP() << "Fichier URDF UR5 introuvable, test ignore.";
    }

    // 2. Préparation des données et de la configuration (q)
    pinocchio::Data data(model);
    Eigen::VectorXd q = Eigen::VectorXd::Zero(model.nq);
    q[0] = M_PI / 4.0; // On met le premier joint (épaule) à 45 degrés

    // 3. Calcul de la cinématique avec Pinocchio
    pinocchio::forwardKinematics(model, data, q);
    
    // On extrait la matrice homogène du joint 1
    // oMi signifie "Origin to Model index i", soit la pose absolue du joint
    Mat4 pinocchio_T = data.oMi[1].toHomogeneousMatrix(); 

    // 4. Calcul avec notre propre implémentation
    // On suppose ici un joint simple (dx = 0), à adapter si le URDF du UR5 a un décalage spécifique
    CJointRevolute my_joint(-M_PI, M_PI, 0.0);
    my_joint.setQ(M_PI / 4.0);
    Mat4 my_T = my_joint.getTransform();

// 5. Affichage pour comprendre la différence
    std::cout << "--- Matrice Pinocchio (Vrai UR5 3D) ---\n" << pinocchio_T << "\n\n";
    std::cout << "--- Ma Matrice (Modele simplifie 2D) ---\n" << my_T << "\n\n";

    // 6. La confrontation ! 
    // Au lieu de comparer toute la matrice 4x4, on ne compare que le bloc de rotation 3x3
    // La méthode .block<lignes, colonnes>(ligne_depart, colonne_depart) d'Eigen est parfaite pour ça
    bool is_approx = my_T.block<3,3>(0,0).isApprox(pinocchio_T.block<3,3>(0,0), 1e-10);
    EXPECT_TRUE(is_approx);
}

// Le main() par défaut de Google Test pour lancer tous les tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}