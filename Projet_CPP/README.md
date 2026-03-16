# Projet CPP

## Question théoriques

### Scéance 1

1. Réponses aux questions théoriques

    1.a. Destructeur virtuel : Le destructeur de CJoint doit être virtual pour garantir que, lors de la destruction d'un objet dérivé (ex. CJointRevolute) via un pointeur de la classe de base (CJoint*), le destructeur de la classe dérivée soit bien appelé en premier. Sans cela, seul le destructeur de la classe de base serait exécuté, entraînant un comportement indéfini comme des fuites de mémoire (les attributs spécifiques à la classe dérivée ne seraient pas nettoyés).

    1.b. Const-correctness : La méthode setQ() modifie l'attribut q_ de l'objet, elle ne peut donc pas être déclarée const (qui garantit qu'une méthode ne modifie pas l'état de l'objet). À l'inverse, les accesseurs comme getQ() se contentent de lire les attributs sans les altérer, ils doivent donc être const pour pouvoir être appelés sur des instances constantes de la classe.

    2.a. Ligature dynamique : Grâce au mot-clé virtual, l'appel à getTransform() via un pointeur CJoint* va dynamiquement résoudre le type réel de l'objet pointé à l'exécution et appeler la bonne redéfinition (ex: CJointRevolute::getTransform()). Si la méthode n'était pas virtual, c'est la méthode de la classe de base (le type du pointeur) qui serait appelée par défaut (ligature statique), ce qui ici poserait problème car elle est virtuelle pure (ce qui empêcherait d'ailleurs l'instanciation ou créerait une erreur de compilation).

    2.b. Clonage polymorphique : La méthode clone() est indispensable pour effectuer une copie profonde d'objets polymorphiques. La classe CBras stockera des pointeurs vers la classe de base (CJoint), et le constructeur de copie standard ne connaîtrait pas le type dérivé exact à instancier lors de la copie du bras, d'où la nécessité du pattern Prototype (clone).

### Scéance 2

1.a. Pourquoi ```addJoint()``` prend un unique_ptr par valeur et requiert `std::move()` à l'appel ?
Un unique_ptr garantit une propriété exclusive (il ne peut y avoir qu'un seul propriétaire de la ressource en mémoire). Le passer par valeur avec `std::move()` force le développeur à transférer explicitement cette propriété à la classe CBras, évitant ainsi toute copie accidentelle et garantissant qu'il n'y aura pas de fuite mémoire.

1.b. Pourquoi CBras n'est-elle pas copiable par défaut ? La classe contient un `std::vector<std::unique_ptr<CJoint>>`. Puisque les unique_ptr interdisent la copie par essence (pour maintenir la propriété unique), le vecteur qui les contient, et par extension la classe CBras, voient leur constructeur de copie désactivé par défaut par le compilateur.

1.c. Type de retour de `operator<<` et pourquoi retourner `std::ostream&` ? 
Il retourne une référence vers le flux d'entrée (`std::ostream&`). Cela permet le chaînage des appels, ce qui te laisse écrire des choses comme `cout << bras << endl;` sur une seule ligne .

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
