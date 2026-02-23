#ifndef PERSONNE_H
#define PERSONNE_H
#include <iostream>
#include <cstring>

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
    Personne& operator=(const Personne& other) ;
    void afficher() const ;
    const char* getNom() const;
    const char* getPrenom() const;
    int getAge() const;
    void setNom(const char* n);
    void setPrenom(const char* p);
    void setAge(int a);
};



#endif