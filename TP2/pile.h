#ifndef PILE_H
#define PILE_H

#include <memory>
#include <iostream>

template <typename T>
class Pile{
private : 
    int mMax;
    int mSommet;
    T * mPile;
public: 
    //Constructeur et destructeur
    
    Pile(int taille = 100){
            if (taille > 0) {
            mMax = taille;
        } else {
            
            mMax = 100; 
        }
        mSommet = 0; 
        
        mPile = new T[mMax];
        
        std::cout << "Constructeur: Pile creee de taille " << mMax << std::endl;
    }


    Pile(const Pile& autre){
        mMax = autre.mMax;
        mSommet = autre.mSommet;
        
        mPile = new T[mMax];

        for (int i = 0; i < mSommet; i++) {
            mPile[i] = autre.mPile[i];
        }

        std::cout << "Constructeur de copie appele." << std::endl;
    }

    ~Pile(){
        delete[] mPile;
        std::cout << "Memoire liberee" << std::endl;
    }
    

    //Le reste des méthodes

    int CompterElem(){
        return mSommet;
    }

    void EmpilerElem(T elem){
        if (mSommet < mMax) {
            mPile[mSommet] = elem;
            mSommet++;
        } else {
            std::cout << "La pile est pleine." << std::endl;
        }
    }

    T DesempilerElem(){
        if (mSommet > 0) {
            T element = mPile[mSommet - 1];
            mSommet--;
            return element;
        } else {
            std::cout << "La pile est vide." << std::endl;
            return '\0';
        }
    }

    void AfficherPile(){
        std::cout << "Contenu de la pile : ";
        std::cout << "[ ";
        for (int i = 0; i < mSommet; i++) {
            std::cout << mPile[i] << " ";
        }
        std::cout << "]" << std::endl;
    }
};



#endif
