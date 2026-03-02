#ifndef PILE_H
#define PILE_H
#include <iostream>
using namespace std;

class CBasePile{
    protected : 
        int * donnees_;
        int capacite_;
        int nbelem_;
    public : 
    CBasePile(int elemmax = 8) : capacite_(elemmax) , nbelem_(0){
        donnees_ = new int[capacite_];
    } 
    // Virtual permet la redef des fonctions dans une classe héritée 
    virtual ~CBasePile(){
        delete[] donnees_;
    }

    virtual CBasePile& operator<(int val) = 0; 
    virtual CBasePile& operator>(int& val) = 0;


    virtual void afficher(ostream& os) const {
        for(int k = 0; k < nbelem_; ++k) {
            os << donnees_[k] << " ";
        }
    }

    friend ostream& operator<<(ostream& os, const CBasePile& p) {
        p.afficher(os);
        return os;
    }
};

class CPileLifo : public CBasePile{
    public : 
    CPileLifo(int elemmax = 8) : CBasePile(elemmax) {}

    CBasePile& operator<(int val) override {
        if (nbelem_ < capacite_) donnees_[nbelem_++] = val;
        return *this;
    }

    CBasePile& operator>(int& val) override{
        if (nbelem_>0) val = donnees_[--nbelem_];
        return * this;
    }

};

class CPileFifo : public CBasePile {
    public:
    CPileFifo(int cap = 10) : CBasePile(cap) {}

    CBasePile& operator<(int val) override {
        if(nbelem_ < capacite_) donnees_[nbelem_++] = val;
        return *this;
    }

    CBasePile& operator>(int& val) override {
        if(nbelem_ > 0) {
            val = donnees_[0];
            // Décalage des éléments vers la gauche
            for(int k = 0; k < nbelem_ - 1; ++k) {
                donnees_[k] = donnees_[k+1];
            }
            nbelem_--;
        }
        return *this;
    }
};





#endif