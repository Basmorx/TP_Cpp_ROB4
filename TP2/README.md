# TP2 : Structure de Pile en C++ (PiledeChar & Template)

**Cours :** Introduction au C++ / Programmation Orientée Objet  
**Contexte :** Polytech Sorbonne - InfoRob (Février 2026)  
**Auteurs :** [VOTRE NOM] & [NOM DE VOTRE BINÔME]

## Description
Ce projet implémente une structure de données de type **Pile** (LIFO - Last In, First Out). 
Le TP est divisé en deux parties majeures :
1. [cite_start]Une classe spécifique `PiledeChar` gérant une pile de caractères avec allocation dynamique[cite: 6, 7].
2. [cite_start]Une généralisation via un **Template** `Pile<T>` pour gérer tout type de données[cite: 63].

## Structure des fichiers
[cite_start]Le code est organisé selon le principe de séparation déclaration/implémentation[cite: 14, 19]:

* **piledechar.h** : Déclaration de la classe `PiledeChar`.
* **piledechar.cpp** : Définition des méthodes de la classe (`EmpilerElem`, `DesempilerElem`, etc.).
* **tp2ex1.cpp** : Fichier principal contenant le `main`, les tests et les fonctions auxiliaires (`afficheinverse`, `inversemajuscule`).
* [cite_start]**pile.h** : (Exercice 4) Implémentation complète du Template de classe `Pile<T>`[cite: 65].

## Compilation

```
g++ -Wall -Wextra -std=c++17 tp2ex1.cpp piledechar.cpp -o PiledeChar
```

Execution :

```
./PiledeChar
```

