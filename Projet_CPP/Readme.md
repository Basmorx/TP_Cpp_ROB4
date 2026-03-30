# Projet Cpp ROB4 : Simulateur de Bras Robotique Articulé
**Auteurs :** Moreaux Bastien et Muzaton Mattéo

Ce projet implémente un simulateur de bras robotique (cinématique directe et inverse) en C++17, en s'inspirant de la bibliothèque Pinocchio.

##  Prérequis et Dépendances

Pour compiler et exécuter ce projet, vous devez installer les dépendances suivantes :

- **Eigen3** et **Google Test** (`libeigen3-dev`, `libgtest-dev`)
- **Valgrind** (pour la vérification des fuites mémoire)
- **Pinocchio** (via Conda) :

```bash
  conda install pinocchio -c conda-forge 
```

## Commande de compilation:

Assurez-vous d'être dans votre environnement Conda contenant Pinocchio avant de compiler, afin que le Makefile puisse trouver les bons chemins :

```bash 
conda activate
make all
```

## Exécution et Tests

Vous pouvez utiliser les cibles du Makefile pour exécuter facilement les différents programmes :

 ``` bash

#Lancer le simulateur principal (et le régulateur) :
    make run
#Lancer les tests unitaires des articulations :
    make test_joints
#Lancer les tests du bras complet (avec vérification Valgrind) :
    make test_bras
```


