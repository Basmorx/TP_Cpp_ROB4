#include "piledechar.h"


PileDeChar::PileDeChar(int max){
    if (max > 0) {
        mMax = max;
    } else {
        
        mMax = 100; 
    }
    mSommet = 0; 
    
    mPile = new char[mMax];
    
    std::cout << "Constructeur: Pile creee de taille " << mMax << std::endl;
}

PileDeChar::PileDeChar(const PileDeChar& other) {
    mMax = other.mMax;
    mSommet = other.mSommet;
    
    mPile = new char[mMax];

    for (int i = 0; i < mSommet; i++) {
        mPile[i] = other.mPile[i];
    }

    std::cout << "Constructeur de copie appele." << std::endl;

}

PileDeChar::~PileDeChar() {
    delete[] mPile;
    std::cout << "Memoire liberee" << std::endl;
}


int PileDeChar::CompterElements(){
    return mSommet;
}

void PileDeChar::AffichePile() {
    std::cout << "Contenu de la pile : ";
    for (int i = 0; i < mSommet; i++) {
        std::cout << mPile[i] << " ";
    }
    std::cout << std::endl;
}

void PileDeChar::EmpilerElem(char element) {
    if (mSommet < mMax) {
        mPile[mSommet] = element;
        mSommet++;
    } else {
        std::cout << "La pile est pleine." << std::endl;
    }
}

char PileDeChar::DesempilerElem() {
    if (mSommet > 0) {
        char element = mPile[mSommet - 1];
        mSommet--;
        return element;
    } else {
        std::cout << "La pile est vide." << std::endl;
        return '\0';
    }
}