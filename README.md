## A propos du projet

Le but de ce projet est de concevoir un algorithme nous permettant de dessiner des enveloppes convexes dans un plan.

### Logiciels utilisés

* 🐙 GitHub (pour le partage de fichiers entre nous)
* 💻 Visual Studio Code


## Lancer le programme

Notre programme est conçu pour se lancer avec une seule ligne de commande, grâce a notre Makefile et a notre fichier "run.sh" qui combine "make all", "make clean" et "./main".

Pour lancer le programme, il suffit donc d'entrer la commande "./run.sh", et tout se compilera et se lancera automatiquement.


## Mode d'emploi pour l'utilisateur

Une fois le programme lancé, le terminal affichera un menu, permettant de choisir le mode d’affichage exact que l’on souhaite.

1. Le choix du mode de génération :
Vous aurez d’abord à choisir entre une génération manuelle, où vous saisirez les points à la main ; ou bien une génération automatique, qui  affichera dynamiquement, un par un, chaque point créé aléatoirement.


2. Le choix du type d’enveloppe convexe :
Vous pourrez alors choisir entre une enveloppe convexe simple ou des enveloppes convexes emboîtées.


3. Le choix de la forme de l’enveloppe :
Enfin, dans le cas d’une génération automatique, vous aurez à choisir la forme que prendra l’enveloppe, soit en carré, soit en cercle.

Dans le cas d’une génération automatique, une fois la fenêtre ouverte, votre premier clic définira le point de départ de la figure (et donc son centre), et votre deuxième clic définira son rayon, c’est à dire la distance jusqu’à laquelle la figure grandira.


## Auteurs

* Amal ABDALLAH TD1/TP1
* Jade ANTONI  TD1/TP1
