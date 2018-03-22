# MandelBrot

Auteurs: Romain Gallay, Aurélien Siu

---
## Description des fonctionnalités du logiciel

Le calcul d’une courbe de Mandelbrot nécessite un grand nombre d’opérations effectuées pour chaque
Pixel à afficher. Il est dès lors possible de paralléliser les calculs afin de tirer profit des architectures
multi-coeurs. 

Pour rappel, la courbe de Mandelbrot est construite en appliquant itérativement la fonction Zn+1 =
Zn 2 + C, où C est le point à évaluer, et Z0 = 0. Le calcul se fait dans le plan complexe, et si une des
itérations génère un point en dehors du cercle de rayon 2 centré en (0, 0), alors le point est en dehors
de la courbe. Si tel est le cas, le nombre d’itérations pour atteindre la frontière doit être retourné.

Un nombre maximum d’itérations est défini, et si ce nombre est atteint sans que Z n’ait dépassé le
cercle de rayon 2, le point sera colorié en noir, et sinon une fonction permet de calculer sa couleur en
fonction du nombre d’itérations effectuées pour sortir du cercle.  


## Choix d'implémentation

Nous avons d'abord analysé comment le programme fonctionnait en utilisant un seul thread pour ce qui concerne la
partie calcul.
Il a ensuite fallut extraire correctement du code la partie calcul pour la placer dans une autre classe dérivant de QThread.
Une fois cette première étape effectuée, cela n'a pas fonctionné dès le début.

Nous avons du donc passer à une deuxième analyse des différentes variables qui entraient en jeu dans le calcul.  
Il y a eu notamment besoin de définir trois pointeurs sur 3 variables, respectivement
pour les booléens abort, restart ainsi que pour la Qimage.

Cela nous donne un constructeur avec un nombre conséquent de paramètres.


Comment avez-vous abordé le problème, quels choix avez-vous fait, quelle 
décomposition avez-vous choisie, quelles variables ont dû être protégées, ...

## Tests effectués

Description de chaque test, et information sur le fait qu'il ait passé ou non

Nous avons observé dans notre moniteur système que tous nos coeurs CPU tournaient à 100%.
Nous avons testé le code sur Linux et sur Windows afin de vérifier la portabilité du code

## Problèmes rencontrés

Le fait que le type int était par défaut en non signé ne nous a pas permis de séparer les lignes à calculer en bloc.
Il a donc fallut parcourir directement la qimage afin que le tout fonctionne correctement.
