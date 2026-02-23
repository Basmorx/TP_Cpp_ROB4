#ifndef ETUDIANT_H
#define ETUDIANT_H

#include "Personne.h"

class Etudiant : public Personne {
    private:
    int* notes;
    int nbNotes;
    public :

    // Constructeurs
    Etudiant();
    Etudiant(const char* n, const char* p, int a, int nb);
    Etudiant(const Etudiant& autre);
    
    // Destructeur
    ~Etudiant();

    // Surcharge de l'affectation
    Etudiant& operator=(const Etudiant& autre);

    // Méthodes spécifiques
    int getNote(int index) const;
    void setNote(int index, int valeur);
    float calculerMoyenne() const;

    // Surcharge de l'affichage
    void afficher() const;


};



#endif