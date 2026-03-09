# Projet CPP

## Question théoriques

### Scéance 1

1. Réponses aux questions théoriques

    1.a. Destructeur virtuel : Le destructeur de CJoint doit être virtual pour garantir que, lors de la destruction d'un objet dérivé (ex. CJointRevolute) via un pointeur de la classe de base (CJoint*), le destructeur de la classe dérivée soit bien appelé en premier. Sans cela, seul le destructeur de la classe de base serait exécuté, entraînant un comportement indéfini comme des fuites de mémoire (les attributs spécifiques à la classe dérivée ne seraient pas nettoyés).

    1.b. Const-correctness : La méthode setQ() modifie l'attribut q_ de l'objet, elle ne peut donc pas être déclarée const (qui garantit qu'une méthode ne modifie pas l'état de l'objet). À l'inverse, les accesseurs comme getQ() se contentent de lire les attributs sans les altérer, ils doivent donc être const pour pouvoir être appelés sur des instances constantes de la classe.

    2.a. Ligature dynamique : Grâce au mot-clé virtual, l'appel à getTransform() via un pointeur CJoint* va dynamiquement résoudre le type réel de l'objet pointé à l'exécution et appeler la bonne redéfinition (ex: CJointRevolute::getTransform()). Si la méthode n'était pas virtual, c'est la méthode de la classe de base (le type du pointeur) qui serait appelée par défaut (ligature statique), ce qui ici poserait problème car elle est virtuelle pure (ce qui empêcherait d'ailleurs l'instanciation ou créerait une erreur de compilation).

    2.b. Clonage polymorphique : La méthode clone() est indispensable pour effectuer une copie profonde d'objets polymorphiques. La classe CBras stockera des pointeurs vers la classe de base (CJoint), et le constructeur de copie standard ne connaîtrait pas le type dérivé exact à instancier lors de la copie du bras, d'où la nécessité du pattern Prototype (clone).
