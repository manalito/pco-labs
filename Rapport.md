#PCO - Labo 7 Server part 3

**Auteurs** : *Aurélien Siu et Romain Gallay*
**Date**: *16.06.2018*


## Fonctionnalité du logiciel

Durant ce laboratoire nous travaillons sur un serveur de fichier utilisant comme canal de communication une WebSocket. Pour cette troisième partie notre but sera d'utiliser une cache qui gardera en mémoire durant un temps défini le résultat d'une requête, ceci pour éviter des accès en lecture superflus sur les fichiers.


## Choix d'implémentation

Notre implémentation du threadpool lors du labo précédent ne fonctionnant pas correctement, nous avons décidé de partir du code du labo 5 qui créait un thread par requête. Nous avons ensuite modifié les classes suivantes :

#### RequestDispatcherThread

De base à chaque requête reçue, cette classe crée un thread de type requestprocessor qui se chargera de traiter la requête. Nous l'avons modifiée comme suit :
La classe crée maintenant un ReaderWriterCache dès son initialisation. Lorsqu'elle reçoit une requête, elle vérifie d'abord si la réponse à la requête se trouve dans le cache. Le cas échéant, elle met ce résultat dans le buffer de reponses, sinon elle crée un requestprocessor pour traiter la requête.

#### RequestProcessor

Cette classe se charge de traiter les requêtes. Nous l'avons modifiée ainsi :
Lorsqu'une requête a été traitée, on met la réponse dans la cache en plus de la mettre dans le buffer de reponses.

#### ReaderWriterLock

Une simple classe de lecteur-rédacteur implémentée à l'aide de de moniteur de Hoare. Comme ce n'était pas demandé, nous avons décidé de ne pas inclure de priorité ni pour les lecteurs ni pour les rédacteurs.

#### ReaderWriterCache

Cette classe implémente un système de cache, notamment avec l'aide de ReaderWriterLock et d'une hash-table (QHash). 
La fonction putResponse fait un accès en écriture à la map, elle est donc bornée par un lockWriting et unlockWriting. A l'intérieur, elle met dans la map la réponse passée en argument avec un timestamp du moment actuel pour gérer la durée de validité d'une réponse.
La fonction tryGetCachedResponse fait un accès en lecture à la map et a donc comme borne lockReading et unlockReading. Si une réponse se trouve dans la map, on retourne une option la contenant, autrement on retourne une option vide.

## Réponses aux questions

La cache améliore les performances dans 2 situations :
- si plus d'un client lance une requête pour un même fichier,
- si un même client lance plus d'une requête pour un même fichier,

ceci durant l'intervalle de temps correspondante à staleDelaySec.

Les shémas d'utilisation suivant pourraient être problématiques :

1) Les données de la cache sont enregistrées dans la mémoire RAM. Si plusieurs fichiers de taille importante tentent d'être chargés dans la cache, il y a un risque de remplir la RAM et que celle-ci soit insuffisante pour contenir tous les fichiers.

2) La durée de validité d'un fichier est actuellement de 2 minutes. Dans un cas où les fichier changent toutes les minutes ou plus, il faudrait réduire cette durée. Si les fichiers changent trop souvent la cache peut devenir plus un problème qu'une aide, car soit le fichier en cache ne correspond plus au fichier actuel, soit il faut recharger le fichier dans le cache ce qui prend des ressources.