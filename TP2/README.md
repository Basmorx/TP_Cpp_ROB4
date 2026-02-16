# TP2 : Structure de Pile en C++ (PiledeChar & Template)

 
**Auteurs :**  MUZATON Mattéo / MOREAUX Bastien

## Description
Ce projet implémente une structure de données de type **Pile** (LIFO - Last In, First Out). 
Le TP est divisé en deux parties majeures :
1. Une classe spécifique `PiledeChar` gérant une pile de caractères avec allocation dynamique.
2. Une généralisation via un **Template** `Pile<T>` pour gérer tout type de données.

## Structure des fichiers
Le code est organisé selon le principe de séparation déclaration/implémentation:

* **piledechar.h** : Déclaration de la classe `PiledeChar`.
* **piledechar.cpp** : Définition des méthodes de la classe (`EmpilerElem`, `DesempilerElem`, etc.).
* **tp2ex1.cpp** : Fichier principal contenant le `main`, les tests et les fonctions auxiliaires (`afficheinverse`, `inversemajuscule`).
* **pile.h** : (Exercice 4) Implémentation complète du Template de classe `Pile<T>`.

##  Libertée prise :

* `inversemajuscule` : Dans cette fonction le mot est inversé 2 fois (pour revenir droit) et les minuscules -> majucules ansi que les majuscules -> minuscule exemple `Bastien` -> `bASTIEN`  

## Compilation

```
g++ -Wall -Wextra -std=c++17 tp2ex1.cpp piledechar.cpp -o PiledeChar
```

Execution :

```
./PiledeChar
```

