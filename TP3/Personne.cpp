#include "Personne.h"

// Default constructor
Personne::Personne() {
    nom = new char[1];
    nom[0] = '\0';
    prenom = new char[1];
    prenom[0] = '\0';
    age = new int(0);
}

// Parameterized constructor
Personne::Personne(const char* n, const char* p, int a) {
    nom = new char[strlen(n) + 1];
    strcpy(nom, n);
    prenom = new char[strlen(p) + 1];
    strcpy(prenom, p);
    age = new int(a);
}

// Copy constructor
Personne::Personne(const Personne& other) {
    nom = new char[strlen(other.nom) + 1];
    strcpy(nom, other.nom);
    prenom = new char[strlen(other.prenom) + 1];
    strcpy(prenom, other.prenom);
    age = new int(*other.age);
}

// Assignment operator
Personne& Personne::operator=(const Personne& other) {
    if (this != &other) {
        delete[] nom;
        nom = new char[strlen(other.nom) + 1];
        strcpy(nom, other.nom);
        delete[] prenom;
        prenom = new char[strlen(other.prenom) + 1];
        strcpy(prenom, other.prenom);
        *age = *other.age;
    }
    return *this;
}

// Destructor
Personne::~Personne() {
    delete[] nom;
    delete[] prenom;
    delete age;
    std::cout << "Destructeur appelé pour " << (nom[0] ? nom : "inconnu") << std::endl;
}

// afficher method
void Personne::afficher() const {
    std::cout << "Nom: " << nom << ", Prenom: " << prenom << ", Age: " << *age << std::endl;
}

// Getters
const char* Personne::getNom() const {
    return nom;
}

const char* Personne::getPrenom() const {
    return prenom;
}

int Personne::getAge() const {
    return *age;
}

// Setters
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