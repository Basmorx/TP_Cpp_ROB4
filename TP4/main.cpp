#include "Pile.h"


int main() {
    CBasePile* ptPile ;
    CPileFifo fifo ; // pile First In First Out
    CPileLifo lifo ; // pile Last In First Out

    ptPile = &fifo; // ptPile pointe vers la pile FIFO
    *ptPile < 0 < 1; // on empile 0 puis 1
    cout << "pile=" << *ptPile  << endl; // a l’ecran : pile=0 1

    int i;
    *ptPile > i ; // depile dans i
    cout << "pile=" << *ptPile << "i=" << i << endl;
    // a l’ecran : pile=1 i=0

    ptPile = &lifo ; // ptPile pointe vers la pile LIFO
    *ptPile < 0 < 1 ; // on empile 0 puis 1
    cout << "pile=" << *ptPile << endl; // a l’ecran : pile=0 1

    *ptPile > i ; // depile dans i
    cout << "pile=" << *ptPile << " i=" << i << endl ; // a l’ecran : pile=0 i=1

    return 0;
}