#include <iostream>
#include <cmath>
#include <memory>
#include <iomanip>
#include "CBras.h"


// Fonction Bonus : Régulateur Proportionnel avec Pseudo-Inverse (Cinématique Inverse)
void regulateurProportionnel(CBras& bras, const Eigen::Vector2d& cible) {
    double Kp = 0.5; 
    double epsilon = 1e-4; 
    int max_iter = 1000;
    int iter = 0;

    std::cout << "--- Demarrage du Regulateur vers cible (" << cible(0) << ", " << cible(1) << ") ---\n";

    // Posture de départ pour éviter les minimums locaux (bras totalement tendu)
    for (size_t i = 0; i < bras.getNbJoints(); ++i) {
        if (bras.getJoint(i)->getTypeName() == "Revolute") {
            bras.getJoint(i)->setQ(0.1); 
        }
    }

    while (iter < max_iter) {
        Mat4 T = bras.computeFK();
        Eigen::Vector2d pos(T(0, 3), T(1, 3));
        Eigen::Vector2d e = cible - pos; 
        
        // Cible atteinte !
        if (e.norm() < 0.01) {
            std::cout << "-> Cible atteinte en " << iter << " iterations ! (Erreur finale: " << std::fixed << std::setprecision(4) << e.norm() << " m)\n";
            std::cout << "-> Configuration finale : \n" << bras << "\n";
            return;
        }

        Eigen::MatrixXd J(2, bras.getNbJoints());
        for (size_t i = 0; i < bras.getNbJoints(); ++i) {
            CJoint* joint = bras.getJoint(i);
            double q_orig = joint->getQ();
            
            // Dérivée Centrale : on regarde un peu en avant et un peu en arrière
            double q_plus = std::min(q_orig + epsilon, joint->getQMax());
            double q_minus = std::max(q_orig - epsilon, joint->getQMin());
            
            joint->setQ(q_plus);
            Eigen::Vector2d pos_plus = bras.computeFK().block<2,1>(0,3);
            
            joint->setQ(q_minus);
            Eigen::Vector2d pos_minus = bras.computeFK().block<2,1>(0,3);
            
            // Si l'articulation est totalement bloquée par ses butées
            if (q_plus - q_minus < 1e-6) {
                J.col(i) = Eigen::Vector2d(0, 0); 
            } else {
                J.col(i) = (pos_plus - pos_minus) / (q_plus - q_minus);
            }
            joint->setQ(q_orig); // On remet l'angle d'origine
        }

        // LA MAGIE EIGEN : Pseudo-Inverse (complète décomposition orthogonale)
        Eigen::VectorXd delta_theta = J.completeOrthogonalDecomposition().solve(e);
        delta_theta *= Kp; 

        // Bride de sécurité pour éviter les mouvements trop violents
        if (delta_theta.norm() > 0.1) {
            delta_theta = delta_theta.normalized() * 0.1;
        }

        // Application de la commande
        for (size_t i = 0; i < bras.getNbJoints(); ++i) {
            CJoint* joint = bras.getJoint(i);
            double new_q = joint->getQ() + delta_theta(i);
            // Saturation stricte sur les butées
            new_q = std::max(joint->getQMin(), std::min(new_q, joint->getQMax()));
            joint->setQ(new_q);
        }
        iter++;
    }
    
    std::cout << "-> Echec : Cible non atteinte apres " << max_iter << " iterations.\n";
    Mat4 T_final = bras.computeFK();
    Eigen::Vector2d pos_final(T_final(0, 3), T_final(1, 3));
    std::cout << "Distance restante : " << (cible - pos_final).norm() << " m\n";
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

    // Cible atteignable sans casser le coude !
    Eigen::Vector2d cible_xy(0.55, 0.10); 
    regulateurProportionnel(bras, cible_xy);

    return 0;
}