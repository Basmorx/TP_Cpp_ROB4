#ifndef PILE_H
#define PILE_H

#include <memory>
#include <iostream>

template <typename T>
class Pile{
private : 
unsigned int mMax;
unsigned int mSommet;
std::unique_ptr<T[]> mPile;

public: 
    
    Pile(int taille = 100);
    Pile(const Pile& autre);
    ~Pile();
    

    int compterElem();
    void empilerElem(T elem);
    T desempilerElem();
    AfficherPile();



};



#endif
