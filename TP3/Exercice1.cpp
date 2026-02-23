#include "Personne.h"

Personne::Personne() {
    nom = new char[1];
    prenom = new char[1];
    age = new int(0);
    nom[0] = '\0';
    prenom[0] = '\0';
}

void Personne::afficher() const {
    std::cout << "Nom: " << nom << ", Prenom: " << prenom << ", Age: " << *age << std::endl;
}

const char* Personne::getNom() const {
    return nom;
}

const char* Personne::getPrenom() const {
    return prenom;
}

int Personne::getAge() const {
    return *age;
}

void Personne::setNom(const char* n) {
    delete[] nom;
    nom = new char[strlen(n) + 1];
    strcpy(nom, n);
}

void Personne::setPrenom(const char* p) {
    delete[] prenom;
    prenom = new char[strlen(p) + 1];
    strcpy(prenom, p);
}

void Personne::setAge(int a) {
    *age = a;
}

