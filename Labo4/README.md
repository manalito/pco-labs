# **PCO Labo4 - QTrain**

*Auteurs : Aurélien Siu, Romain Gallay*

## **Implémentation**

Nous avons commencé par définir une classe **Worker**, dérivant de **QThread**. Celle-ci représente en quelque sorte un conducteur de locomotive ayant la possibilité de communiquer avec l'autre locomotive ainsi qu'avec l'entité gérant les aiguillages. Le worker connaît le parcours de sa locomotive et s'attendra à passer par une série de contacts déterminés. Il connaît également la section "critique" de son parcours sur laquelle il devra s'assurer de ne pas entrer en collision avec l'autre locomotive.
Concrètement, nous avons défini que le worker s'occupant de la loco 1 dévie si besoin et celui s'occupant de la loco 2 s'arrête. A chaque de point de contact, le worker vérifie si la locomotive est dans la zone critique, et, si c'est le cas, dans quelle partie de la zone critique il se trouve. Il agit ensuite selon le résultat et selon son type :
- Si l'autre loco se trouve déjà en section critique, il stop sa loco ou dévie.
- Si la loco 2 est au milieu de la section critique alors que la loco 1 y entre, le worker stop la loco 2. Ceci est nécessaire pour gérer les croisements si les locos vont dans un sens différent et/ou à une vitesse différente.

Chaque worker possède une référence sur un objet **section** qui représente la zone critique au sens large. Les workers indiquent à la section lorsqu'ils entrent ou sortent de la section critique. Le worker de loco 1 quant à lui indique en plus lorsqu'il **s'approche** de la zone critique pour que le worker de la loco 2 ait le temps de s'arrêter. 
La section s'occupe également de changer les aiguillages uniquement sur appel d'un des workers. Les changements d'aiguillages sont protégés par un sémaphore pour éviter que 2 workers les modifient en même temps. Nous avons également défini un sémaphore "busy" qui indique lorsque la loco 1 est dans la section. Ce sémaphore permet de passer le signal de sortie de la loco 1 à la loco 2 pour que cette dernière puisse repartir.

### **Commandes et priorités**

Pour gérer le passage de commandes, nous avons implémenté une classe **messenger**, dérivant de **QThread**, dont le seul rôle est d'être à l'écoute et d'agir selon les commandes reçues. Ci-dessous les messages avec l'action déclanchée correspondante :
- "loco1" rend la locomotive 1 prioritaire,
- "loco2" rend la locomotive 2 prioritaire,
- "reset" remet les priorité à 0 de chaque loco.

En réalité nous avons choisi de gérer la "non priorité" d'une loco sur une autre. En effet, lorsqu'une locomotive est prioritaire elle ne fait que son parcours habituel, tandis que lorsqu'elle est non prioritaire se présentent 2 cas, selon le type de loco :
- Si loco 1 non prioritaire, alors elle emprunte toujours la voie de déviation.
- Si loco 2 non prioritaire, alors elle ne peut plus accéder au tronçon commun et s'arrête devant celui-ci.

## **Vérifications**

Nous avons commencé par laisser tourner la simulation pendant plusieurs minutes avec différentes vitesses et en changeant les priorités. La simulation fonctionne en activant l'inertie tant que la loco 2 (celle qui s'arrête) n'a pas une vitesse trop élevée. Sinon il arrive que son arrêt se finisse sur un croisement.

Ceci a été encore plus flagrant lors du test sur la maquette, mais ici un autre problème est apparu. Lors des arrêts directement après un croisement, par exemple sur les contacts 9 ou 4, l'arrière de la locomotive se trouvait encore régulièrement sur le croisement. Nous avons donc choisi d'empêcher la loco de s'arrêter sur le premier contact du tronçon commun en supprimer le conflictManagment à ce moment-là (supprimer la ligne 100 dans worker.h). Selon les modèles de locomotive, nous avons observé qu'une vitesse autour de 6 semble convenir.

