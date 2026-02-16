#include "memory"
#include "iostream"

class PileDeChar {
private:
    int mMax ;
    int mSommet ;
    char * mPile ;

public:
    PileDeChar(int max= 100) ;

    PileDeChar(const PileDeChar& other) ;

    ~PileDeChar() { delete[] mPile; }

    int CompterElements();

    void AffichePile() ;

    void EmpilerElem(char element) ;

    char DesempilerElem() ;
};

