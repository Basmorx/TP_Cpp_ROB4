#include "Personne.h"

// --- EXERCICE 1 : Constructeurs et Destructeur ---

// Constructeur par défaut
Personne::Personne() {
    nom = new char[1]; nom[0] = '\0';
    prenom = new char[1]; prenom[0] = '\0';
    age = new int(0);
}

// Constructeur avec paramètres
Personne::Personne(const char* n, const char* p, int a) {
    nom = new char[strlen(n) + 1];
    strcpy(nom, n);
    prenom = new char[strlen(p) + 1];
    strcpy(prenom, p);
    age = new int(a);
}

// Constructeur de copie (copie profonde)
Personne::Personne(const Personne& other) {
    nom = new char[strlen(other.nom) + 1];
    strcpy(nom, other.nom);
    prenom = new char[strlen(other.prenom) + 1];
    strcpy(prenom, other.prenom);
    age = new int(*(other.age));
}

// Destructeur
Personne::~Personne() {
    std::cout << "Appel du destructeur Personne pour : " << nom << std::endl;
    delete[] nom;
    delete[] prenom;
    delete age;
}

// --- Méthodes ---

void Personne::afficher() const {
    std::cout << "Nom: " << nom << ", Prenom: " << prenom << ", Age: " << *age << std::endl;
}

const char* Personne::getNom() const { return nom; }
const char* Personne::getPrenom() const { return prenom; }
int Personne::getAge() const { return *age; }

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


// --- EXERCICE 3 : Opérateur d'affectation ---

Personne& Personne::operator=(const Personne& other) {
    if (this != &other) {
        // 1. Libérer l'ancienne mémoire
        delete[] nom;
        delete[] prenom;
        delete age;

        // 2. Allouer et copier (copie profonde)
        nom = new char[strlen(other.nom) + 1];
        strcpy(nom, other.nom);
        prenom = new char[strlen(other.prenom) + 1];
        strcpy(prenom, other.prenom);
        age = new int(*(other.age));
    }
    return *this;
}

