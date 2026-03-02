# TP 4

## Mattéo Muzaton et Bastien Moreaux

### EXERCICE 1

1. La structure de données commune à LIFO ET FIFO est le tableau alloué dynamiquement de int, les données membres de la classe de base sont un stockage de donnée (```int * donnee_```), une capacitée (```int capacite_```) et un nombre d'éléments ( ```int nbelem_```).

2. les opérateurs communs sont l'empilement ``` < ``` et le dépilement ``` > ``` et l'affichage ``` << ```

Les fonctions membres de la classe de base sont donc : ```virtual CBasePile& operator<(int val)``` , ```virtual CBasePile& operator>(int& val)``` et ```friend ostream& operator<<(ostream& os, const CBasePile& p)```.

3. Dans "Pile.h" nous avons l'implémentations des classes pour tester on peut faire :

```bash
 g++ main.cpp 
 ./a.out
```

4. Pour assurer la ligature dynamique et faire fonctionner l'appel des fonctions via un pointeur sur une classe de base (comme ```*ptPile < 0```), il faut impérativement utiliser le mot-clé ```virtual``` lors de la déclaration des fonctions membres (```operator<``` et ```operator>```) dans la classe de base ```CBasePile```. Ainsi, lors de l'exécution, le compilateur regardera le type réel de l'objet (pointé par l'adresse affectée, par exemple ```&fifo``` ou ```&lifo```) pour appeler la bonne méthode spécialisée.

### EXERCICE 2

1. On utilise ``` new ``` pour allouer dynamiquement un pointeur sans le ``` delete ``` à la fin du programme, ce qui créé une fuite de mémoire.

```cpp
void foo()
{
    CPileFifo* p = new CPileFifo(5) ; // alloue sur le tas
    *p < 0 < 1 ;
    // ... utilisation de p ...

    delete p ;

} // fin de foo

```

2. Si on appelle le destructeur et que le destructeur de CBasePile n'est pas virtuel :

```bash
delete ptPile
```

le destructeur appelé sera celui ```CBasePile::~CBasePile``` et le destructeur ```ptPile::~ptPile``` ne sera pas appelé.

Si le destructeur ```CBasePile::~CBasePile``` est virtuel, alors le compilateur reconnait le destructeur à utiliser et appelle ```ptPile::~ptPile```.

Donc si on ne déclare pas le destructeur de la classe mère comme étant ```virtual``` alors le destructeur appelé par ```delete``` sera toujours celui de la classe mère quel que soit le type de l'objet dérivé.
