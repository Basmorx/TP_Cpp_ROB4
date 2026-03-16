#include <iostream>
#include <cmath>
#include <memory>
#include <iomanip>
#include "CBras.h"


// Fonction Bonus : Régulateur Proportionnel (Cinématique Inverse)
void regulateurProportionnel(CBras& bras, const Eigen::Vector2d& cible) {
    double Kp = 0.2; // Gain proportionnel
    double epsilon = 1e-4; 
    int max_iter = 1000;
    int iter = 0;

    std::cout << "--- Demarrage du Regulateur vers cible (" << cible(0) << ", " << cible(1) << ") ---\n";

    // 1. ASTUCE : On donne une posture initiale "pliée" pour éviter le minimum local
    bras.getJoint(0)->setQ(M_PI / 4.0); // On lève l'épaule
    bras.getJoint(1)->setQ(M_PI / 4.0); // On plie franchement le coude
    bras.getJoint(2)->setQ(0.0);
    bras.getJoint(3)->setQ(0.0);

    while (iter < max_iter) {
        Mat4 T = bras.computeFK();
        Eigen::Vector2d pos(T(0, 3), T(1, 3));
        Eigen::Vector2d e = cible - pos; 
        
        // Condition d'arrêt : ||e|| < 0.01 m
        if (e.norm() < 0.01) {
            std::cout << "-> Cible atteinte en " << iter << " iterations ! (Erreur finale: " << std::fixed << std::setprecision(4) << e.norm() << " m)\n";
            std::cout << "-> Configuration finale : \n" << bras << "\n";
            return;
        }

        Eigen::MatrixXd J(2, bras.getNbJoints());
        for (size_t i = 0; i < bras.getNbJoints(); ++i) {
            CJoint* joint = bras.getJoint(i);
            double q_orig = joint->getQ();
            
            // 2. AMÉLIORATION : Dérivée Centrale (plus stable sur les butées)
            double q_plus = std::min(q_orig + epsilon, joint->getQMax());
            double q_minus = std::max(q_orig - epsilon, joint->getQMin());
            
            joint->setQ(q_plus);
            Eigen::Vector2d pos_plus = bras.computeFK().block<2,1>(0,3);
            
            joint->setQ(q_minus);
            Eigen::Vector2d pos_minus = bras.computeFK().block<2,1>(0,3);
            
            if (q_plus - q_minus < 1e-6) {
                J.col(i) = Eigen::Vector2d(0, 0); // Articulation totalement bloquée
            } else {
                J.col(i) = (pos_plus - pos_minus) / (q_plus - q_minus);
            }
            joint->setQ(q_orig);
        }

        Eigen::VectorXd delta_theta = Kp * J.transpose() * e;

        // 3. SÉCURITÉ : On bride la vitesse max pour éviter les oscillations (overshoot)
        if (delta_theta.norm() > 0.05) {
            delta_theta = delta_theta.normalized() * 0.05;
        }

        // Mise à jour
        for (size_t i = 0; i < bras.getNbJoints(); ++i) {
            CJoint* joint = bras.getJoint(i);
            double new_q = joint->getQ() + delta_theta(i);
            new_q = std::max(joint->getQMin(), std::min(new_q, joint->getQMax()));
            joint->setQ(new_q);
        }
        iter++;
    }
    std::cout << "-> Configuration finale : \n" << bras << "\n";
    std::cout << "-> Echec : Cible non atteinte apres " << max_iter << " iterations.\n";
}


int main() {
    std::cout << "=== INITIALISATION DU BRAS 4-DDL ===\n";
    CBras bras;
    
    // Ajout des articulations selon le tableau fourni
    // [0] Epaule : Revolute [-PI, PI], dx = 0.30 m
    bras.addJoint(std::make_unique<CJointRevolute>(-M_PI, M_PI, 0.30));
    
    // [1] Coude : Revolute [-PI/2, PI/2], dx = 0.25 m
    bras.addJoint(std::make_unique<CJointRevolute>(-M_PI / 2.0, M_PI / 2.0, 0.25));
    
    // [2] Ext. : Prismatic [0, 0.20 m]
    bras.addJoint(std::make_unique<CJointPrismatic>(0.0, 0.20));
    
    // [3] Poignet : Revolute [-PI, PI], dx = 0.05 m
    bras.addJoint(std::make_unique<CJointRevolute>(-M_PI, M_PI, 0.05));

    // Affichage de la configuration du bras avec l'opérateur <<
    std::cout << bras << "\n";

    std::cout << "=== BALAYAGE DE L'EPAULE [-PI/2 ; PI/2] ===\n";
    // On met les autres joints à des valeurs fixes (ex: tout à 0)
    bras.getJoint(1)->setQ(0.0);
    bras.getJoint(2)->setQ(0.0);
    bras.getJoint(3)->setQ(0.0);

    // Balayage de -PI/2 à PI/2 par pas de PI/8
    double start = -M_PI / 2.0;
    double end = M_PI / 2.0;
    double step = M_PI / 8.0;
    
    std::cout << std::fixed << std::setprecision(4);
    for (double theta = start; theta <= end + 1e-6; theta += step) {
        bras.getJoint(0)->setQ(theta);
        Mat4 T = bras.computeFK();
        // Extraction de la position via T.block<3,1>(0,3).transpose()
        Eigen::RowVector3d pos = T.block<3, 1>(0, 3).transpose();
        std::cout << "Epaule q = " << std::setw(7) << theta << " rad | Effecteur (x,y,z) = " << pos << "\n";
    }

    std::cout << "\n=== TESTS DE ROBUSTESSE ===\n";
    
    // 1. Attraper std::out_of_range
    std::cout << "Test Dépassement butée (Coude à 2.0 rad) : ";
    try {
        bras.getJoint(1)->setQ(2.0); // Le coude est limité à PI/2 (1.57)
    } catch (const std::out_of_range& e) {
        std::cout << "Exception attrappée avec succès : " << e.what() << "\n";
    }

    // 2. Indépendance Copie Profonde
    std::cout << "Test Copie Profonde : ";
    bras.getJoint(0)->setQ(0.0);
    CBras copie = bras;
    copie.getJoint(0)->setQ(1.0);
    if (bras.getJoint(0)->getQ() == 0.0) {
        std::cout << "Succès (Original intact)\n";
    } else {
        std::cout << "Échec\n";
    }

    // 3. Test std::move()
    std::cout << "Test Déplacement (std::move) : ";
    CBras cible = std::move(copie);
    if (copie.getNbJoints() == 0 && cible.getNbJoints() == 4) {
        std::cout << "Succès (Copie vidée, Cible remplie)\n";
    } else {
        std::cout << "Échec\n";
    }

    std::cout << "\n=== BONUS : REGULATEUR PROPORTIONNEL ===\n";
    // On remet le bras à plat
    bras.getJoint(0)->setQ(0.0);
    bras.getJoint(1)->setQ(0.0);
    bras.getJoint(2)->setQ(0.0);
    bras.getJoint(3)->setQ(0.0);

    // On crée une cible (x*, y*) atteignable !
    Eigen::Vector2d cible_xy(0.50, 0.10); 
    regulateurProportionnel(bras, cible_xy);

    return 0;
}