#include <iostream>
#include <cctype>
#include "piledechar.h"

using namespace std;

// Affiche et vide la pile (passage par valeur pour ne pas modifier l'originale)
void afficheInverse(PileDeChar p) {
    while (p.CompterElements() > 0) {
        cout << p.DesempilerElem();
    }
    cout << endl;
}

int main() {
    char buffer[80];
    cout << "Entrez un mot : ";
    cin.getline(buffer, 79); // Utilisation de getline pour éviter les résidus 

    PileDeChar maPile;
    for (int i = 0; buffer[i] != '\0'; ++i) {
        maPile.EmpilerElem(buffer[i]);
        maPile.AffichePile(); // Surveillance de l'évolution 
    }

    cout << "Message a l'envers : ";
    afficheInverse(maPile);

    return 0;
}