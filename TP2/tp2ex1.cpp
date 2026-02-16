#include <iostream>
#include <cctype>
#include "piledechar.h"
#include "pile.h"

using namespace std;

// Affiche et vide la pile (passage par valeur pour ne pas modifier l'originale)
void afficheInverse(PileDeChar p) {
    while (p.CompterElements() > 0) {
        cout << p.DesempilerElem();
    }
    cout << endl;
}

PileDeChar inversemajuscule(PileDeChar p) {

    PileDeChar pileTemp(p.CompterElements());
    while (p.CompterElements() > 0) {
        pileTemp.EmpilerElem(p.DesempilerElem());
    }

    PileDeChar nPile(pileTemp.CompterElements());

    while (pileTemp.CompterElements() > 0) {
        
        char c = pileTemp.DesempilerElem(); 

        // Logique de bascule Majuscule <-> Minuscule
        if (std::isupper(c)) {
            c = std::tolower(c); // Devient minuscule
        } else if (std::islower(c)) {
            c = std::toupper(c); // Devient majuscule
        }
        nPile.EmpilerElem(c);
    }

    return nPile;
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
    PileDeChar MApILE = inversemajuscule(maPile);
    MApILE.AffichePile();


    // -------------------------------------------------
    // TEST 1 : Pile<char> 
    // -------------------------------------------------
    cout << "=== TEST PILE<CHAR> ===" << endl;
    Pile<char> pileChar(50);
    
    cout << "Entrez un mot : ";
    // Lecture sécurisée demandée 
    cin.getline(buffer, 79); 

    // Empilage lettre par lettre
    for (int i = 0; buffer[i] != '\0'; ++i) {
        pileChar.EmpilerElem(buffer[i]);
    }
    
    cout << "Contenu de la pile char : ";
    pileChar.AfficherPile();

    // -------------------------------------------------
    // TEST 2 : Pile<int> 
    // -------------------------------------------------
    cout << "\n=== TEST PILE<INT> ===" << endl;
    Pile<int> pileInt(10); // Pile d'entiers

    // Empiler les valeurs 1, 2, 3 
    cout << "Empilage des entiers..." << endl;
    pileInt.EmpilerElem(1);
    pileInt.EmpilerElem(2);
    pileInt.EmpilerElem(3);

    // Afficher la pile 
    cout << "Etat de la pile int : ";
    pileInt.AfficherPile();

    // Dépiler et afficher le sommet obtenu 
    int sommet = pileInt.DesempilerElem();
    cout << "Valeur depilee (sommet) : " << sommet << endl;
    
    cout << "Etat final de la pile int : ";
    pileInt.AfficherPile();
    return 0;
}