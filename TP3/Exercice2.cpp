#include "Personne.h"

int main() {
    const int taille = 3;
    Personne* personnes = new Personne[taille];

    // Initialisations différentes
    personnes[0] = Personne("Dupont", "Jean", 25);
    // personnes[1] reste avec le constructeur par défaut
    personnes[2] = Personne("Martin", "Marie", 30);

    // Affichage de tous les objets
    for(int i = 0; i < taille; i++) {
        personnes[i].afficher();
    }

    // Destruction du tableau
    delete[] personnes;

    return 0;
}


