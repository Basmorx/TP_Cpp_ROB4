#include "Personne.h"


class Personne {
private:
    char* nom;
    char* prenom;
    int* age;
public:

    Personne() ;

    Personne(const char* nom, const char* prenom, int age) ;

    Personne(const Personne& other) ;

    ~Personne() ;

    void afficher() const ;

    const char* getNom() const;

    const char* getPrenom() const;

    int getAge() const;

    void setNom(const char* n);

    void setPrenom(const char* p);

    void setAge(int a);
};


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

