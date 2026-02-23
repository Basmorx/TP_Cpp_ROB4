#include <iostream>
#include "Personne.h"
#include "Etudiant.h"

using namespace std;

int main() {
    cout << "====== EXERCICE 2 : TABLEAU DE PERSONNES ======" << endl;
    
    // Allocation dynamique du tableau avec initialisations variées
    Personne* tabPersonnes = new Personne[3]{
        Personne(), 
        Personne("Orieux", "Jean", 35),
        Personne("Tuili", "Silvia", 24)
    };

    for (int i = 0; i < 3; ++i) {
        tabPersonnes[i].afficher();
        cout << endl;
    }

    cout << "\nDestruction du tableau de Personnes :" << endl;
    delete[] tabPersonnes; // Provoque l'appel des destructeurs

    cout << "\n====== EXERCICE 3 : COPIE ET AFFECTATION ======" << endl;
    Personne p1("Leclerc", "Marc", 40);
    Personne p2;
    p2 = p1; // Appel de operator=

    cout << "Avant modification de p1 :" << endl;
    cout << "P1: "; p1.afficher(); cout << endl;
    cout << "P2: "; p2.afficher(); cout << endl;

    p1.setAge(99); // Preuve indep mem

    cout << "Apres modification de p1 (p2 ne doit pas changer) :" << endl;
    cout << "P1: "; p1.afficher(); cout << endl;
    cout << "P2: "; p2.afficher(); cout << endl;

    cout << "\n====== EXERCICE 5 : TABLEAU D'ETUDIANTS ======" << endl;
    
    Etudiant* tabEtudiants = new Etudiant[2]{
        Etudiant("Muzaton", "Mattéo", 20, 3),
        Etudiant("Moreaux", "Bastien", 22, 2)
    };

    // Ajout des notes
    tabEtudiants[0].setNote(0, 15);
    tabEtudiants[0].setNote(1, 12);
    tabEtudiants[0].setNote(2, 18);

    tabEtudiants[1].setNote(0, 10);
    tabEtudiants[1].setNote(1, 14);

    for (int i = 0; i < 2; ++i) {
        tabEtudiants[i].afficher();
        cout << endl;
    }

    cout << "\nDestruction du tableau d'Etudiants (Observez l'ordre des destructeurs) :" << endl;
    delete[] tabEtudiants;

    cout << "\nFin du programme." << endl;
    return 0;
}