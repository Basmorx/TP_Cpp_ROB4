# Projet CPP

## Question théoriques

### Scéance 1

1. Réponses aux questions théoriques

    1.a. Destructeur virtuel : Si l'on détruit un objet dérivé via un pointeur de sa classe de base (ex: CJoint* ptr = new CJointRevolute(); delete ptr;), l'absence de destructeur virtual fera que seul le destructeur de la classe de base (CJoint) sera appelé. Les données spécifiques à la classe dérivée (comme dx_ pour un joint rotatif) ne seront pas libérées, provoquant une fuite de mémoire (un comportement indéfini en C++). Le rendre virtual garantit que le destructeur dérivé est appelé en premier.

    1.b. Pourquoi setQ() ne peut pas être const, et pourquoi les accesseurs doivent l'être ? Le mot-clé const à la fin d'une méthode promet au compilateur que la fonction ne modifiera aucun attribut de l'objet. La méthode setQ() modifie l'attribut q_, elle viole donc cette promesse et ne peut pas être const. À l'inverse, les accesseurs (comme getQ()) se contentent de lire les données. Les marquer const est indispensable pour qu'on puisse les appeler sur un objet déclaré constant (ex: const CJoint joint;).

    2.a. L'importance de virtual pour getTransform() (Ligature dynamique) : Lorsqu'on appelle ptr->getTransform() via un pointeur CJoint*, le mot-clé virtual indique au programme de regarder le type réel de l'objet en mémoire au moment de l'exécution (via la vtable), et d'appeler la bonne implémentation (ex: CJointRevolute::getTransform()). Si la méthode n'était pas virtuelle, le programme se baserait uniquement sur le type statique du pointeur et essaierait d'appeler CJoint::getTransform(), ce qui planterait car c'est une méthode purement virtuelle.

    2.b. Pourquoi clone() doit-elle être virtual ? Le bras (CBras) stocke des pointeurs vers la classe abstraite CJoint. S'il veut se copier (copie profonde), il ne connaît pas le type dérivé exact de chaque joint qu'il possède. En appelant une méthode clone() virtuelle, il délègue la création de la copie à l'objet lui-même, qui sait exactement comment instancier la bonne classe dérivée (make_unique<CJointRevolute>(this))

### Scéance 2

    1.a. Pourquoi ```addJoint()``` prend un unique_ptr par valeur et requiert `std::move()` à l'appel ?
    Un unique_ptr garantit une propriété exclusive (il ne peut y avoir qu'un seul propriétaire de la ressource en mémoire). Le passer par valeur avec `std::move()` force le développeur à transférer explicitement cette propriété à la classe CBras, évitant ainsi toute copie accidentelle et garantissant qu'il n'y aura pas de fuite mémoire.

    1.b. Pourquoi CBras n'est-elle pas copiable par défaut ? La classe contient un `std::vector<std::unique_ptr<CJoint>>`. Puisque les unique_ptr interdisent la copie par essence (pour maintenir la propriété unique), le vecteur qui les contient, et par extension la classe CBras, voient leur constructeur de copie désactivé par défaut par le compilateur.

    1.c. Le type de retour std::ostream& pour l'opérateur << : La méthode retourne une référence vers la classe de base des flux de sortie, std::ostream&. C'est le polymorphisme en action : puisque std::cout (affichage console), std::ofstream (fichier) et std::ostringstream (chaîne de caractères) héritent tous de std::ostream, notre opérateur fonctionnera universellement avec tous ces outils ! Si on retournait std::ofstream&, on ne pourrait utiliser cet opérateur que pour écrire dans des fichiers, et cout << bras déclencherait une erreur de compilation.

    2.a. Comparaison A∗B (Eigen) vs boucle triple manuelle : 
    Lisibilité : Le code est beaucoup plus clair et concis (une ligne au lieu de boucles imbriquées).

    Performance : Eigen est fortement optimisée et utilise la vectorisation SIMD du processeur pour faire les calculs mathématiques beaucoup plus rapidement qu'une boucle standard.

    2.b. Équivalent Matrix4d dans Pinocchio et comparaison : La méthode de pinocchio::SE3 qui retourne l'équivalent est toHomogeneousMatrix(). Pour comparer numériquement les deux résultats, on vérifie que l'erreur de Frobenius entre les deux matrices est inférieure à une certaine tolérance (ex: <10−10), ou on utilise la méthode isApprox() d'Eigen.

### Scéance 3

    1.a. Copie et clone() : Un unique_ptr est par nature incopiable pour garantir un propriétaire unique. Pour copier le bras, on doit copier son contenu. De plus, comme le vecteur contient des pointeurs vers la classe de base (CJoint), appeler le constructeur de copie standard ne ferait que "couper" les objets dérivés (Object Slicing). La méthode clone() permet de faire une copie profonde (Deep Copy) en recréant dynamiquement le bon type dérivé.

    1.b. L'idiome Copy-and-Swap : Il consiste à créer une copie temporaire de l'objet, puis à échanger (swap) ses données avec l'objet actuel. Cela offre la garantie d'exception forte : si la copie plante (ex: plus de mémoire), l'objet original n'est absolument pas modifié ni corrompu.

    1.c. Déplacement (= default) : Oui, c'est suffisant. Transférer (déplacer) un std::vector de unique_ptr ne fait que déplacer les pointeurs internes d'un objet à l'autre sans réallouer de mémoire. C'est rapide et totalement sécurisé.

    1.d. Destructeur (= default) : Oui, c'est suffisant. À la destruction de CBras, son std::vector est détruit. Le vecteur va détruire ses éléments dans l'ordre inverse de leur création (du dernier au premier). Les unique_ptr libéreront alors automatiquement la mémoire de chaque joint.

    2.a. Pourquoi Eigen::VectorXd ? Pinocchio utilise les types d'Eigen plutôt que std::vector<double> pour bénéficier d'opérations mathématiques directes (comme norm(), dot()) et des optimisations matérielles (vectorisation SIMD) qui accélèrent drastiquement les calculs.

    3.a. Templates dans le .h : Un template n'est pas du code exécutable, c'est un "plan de construction" pour le compilateur. Pour instancier la classe avec un type spécifique (ex: double), le compilateur doit avoir accès à l'intégralité du code source au moment de la compilation.

    3.b. typename T vs size_t N : T est un paramètre de type (il indique si l'on stocke des int, double, etc.), tandis que N est un paramètre non-type (une valeur constante évaluée à la compilation pour définir la taille exacte du tableau en mémoire).

### Scéance 4

    2.b. Pinocchio découple Model (lecture seule) et Data (mutable). Quel intérêt en multithreading ? 
    Le Model contient la description physique du robot (longueurs, masses) qui ne change jamais. Puisqu'il est en lecture seule (const), plusieurs threads (ex: un pour l'affichage 3D, un pour la physique, un pour le calcul de trajectoire) peuvent le lire en même temps sans risque de blocage ni besoin de mutex. À l'inverse, Data contient les résultats de calculs temporaires qui changent à chaque instant. En donnant une instance Data propre à chaque thread, ils peuvent tous calculer en parallèle sans écraser les données des autres (absence de data race).

    2.c. Différence entre modèle cinématique et modèle de collision (hpp-fcl) : 
    Le modèle cinématique n'est qu'un "squelette" mathématique (des points et des axes) utilisé pour calculer des positions. Le modèle de collision, basé sur hpp-fcl, enveloppe ce squelette avec des volumes 3D réels (des cylindres, des maillages de la carrosserie du robot). La méthode computeCollisions()  sert donc à vérifier si ces volumes 3D se rentrent dedans (auto-collision) ou percutent l'environnement, ce qui est indispensable avant d'envoyer la commande aux moteurs réels pour ne pas casser le robot.