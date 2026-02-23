#include "Etudiant.h"
#include <iostream>

using namespace std;

// Constructeur par défaut (appelle implicitement Personne())
Etudiant::Etudiant() : nbNotes(0), notes(nullptr) {}

// Constructeur avec paramètres (appel explicite au constructeur de Personne)
Etudiant::Etudiant(const char* n, const char* p, int a, int nb) 
    : Personne(n, p, a), nbNotes(nb) {
    notes = new int[nbNotes];
    for(int i = 0; i < nbNotes; ++i) {
        notes[i] = 0; // Initialisation par défaut à 0
    }
}

// Constructeur de copie (copie profonde)
Etudiant::Etudiant(const Etudiant& autre) 
    : Personne(autre), nbNotes(autre.nbNotes) {
    notes = new int[nbNotes];
    for (int i = 0; i < nbNotes; ++i) {
        notes[i] = autre.notes[i];
    }
}

// Destructeur
Etudiant::~Etudiant() {
    cout << "  [~Etudiant] Destructeur appele pour l'etudiant : " << getNom() << endl;
    delete[] notes;
    // Le destructeur de Personne sera appelé automatiquement juste après
}

// Surcharge de l'opérateur =
Etudiant& Etudiant::operator=(const Etudiant& autre) {
    if (this != &autre) {
        Personne::operator=(autre); // Appel à l'affectation de la classe mère
        delete[] notes;
        nbNotes = autre.nbNotes;
        notes = new int[nbNotes];
        for (int i = 0; i < nbNotes; ++i) {
            notes[i] = autre.notes[i];
        }
    }
    return *this;
}

// Getter de note
int Etudiant::getNote(int index) const {
    if (index >= 0 && index < nbNotes) {
        return notes[index];
    }
    return -1; // Code d'erreur si index invalide
}

// Setter de note
void Etudiant::setNote(int index, int valeur) {
    if (index >= 0 && index < nbNotes) {
        notes[index] = valeur;
    }
}

// Calcul de la moyenne
float Etudiant::calculerMoyenne() const {
    if (nbNotes == 0) return 0.0f;
    int somme = 0;
    for (int i = 0; i < nbNotes; ++i) {
        somme += notes[i];
    }
    return static_cast<float>(somme) / nbNotes;
}

// Affichage
void Etudiant::afficher() const {
    Personne::afficher();
    cout << " | Moyenne: " << calculerMoyenne() << " (" << nbNotes << " notes)";
}