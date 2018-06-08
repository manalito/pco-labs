#PCO - Labo 7 Server part 2

*Auteurs : Aurélien Siu et Romain Gallay*
*07.06.2018*


## Fonctionnalité du logiciel

Durant ce laboratoire nous travaillons sur un serveur de fichier utilisant comme canal de communication une WebSocket. Pour cette deuxième partie notre but sera d'utiliser un pool de threads qui se chargeront de traiter des requêtes. Les threads seront créé au début au fur et à mesure des besoins puis réutilisés lorsque le pool est plein.


## Choix d'implémentation

Nous avons gardé la 2ème implémentation de BufferN donnée dans le cours que nous avions utilisé dans la première partie du laboratoire. Nous avons ensuite choisi de créer plusieurs classes :

#### RequestDispatcher

Une classe héritant de QThread faisant dans son run les étapes suivantes :
1) Attendre une requête grâce à l'appel bloquant de BufferN::get()
2) Lorsqu'une requête est reçue, créer un nouveau RequestHandler et le passer au threadpool grâce à la méthode start.

#### RequestHandler

Une classe dérivant de Runnable qui servira à traiter les requêtes et mettre les réponses dans un BufferN.
La fonction run() crée un FileReader qui lit le chemin donné par le client. Si le chemin correspond à un fichier, RequestHandler met son contenu dans une réponse, autrement il y met simplement "File not found!". Il met ensuite cette réponse dans le BufferN de réponses.

#### ThreadWorker

Une classe héritant de QThread qui sert à exécuter la méthode run d'un runnable dans un thread. A la fin de sa fonction run, il enverra un wakeOne qui pourra être reçu par le ThreadPool et indiquera que le ThreadWorker a terminé sa tâche.

#### ThreadPool

La classe centrale de ce laboratoire. Elle se charge de créer et maintenir un "pool" contenant un nombre (défini lors de la construction) de WorkerThread. Chaque WorkerThread exécute à la demande la fonction run d'un runnable que l'on passera en paramètre à la fonction start. Cette dernière pourra au choix :
- créer un nouveau WorkerThread si le pool n'est pas plein,
- choisir un WorkerThread disponible dans le pool si celui-ci est plein.
Puis enfin exécuter le runnable dans le thread défini.

Lorsqu'un WorkerThread a terminé sa tâche, il envoie un wakeOne. Ce dernier pourra être reçu par le threadpool lorsqu'il est en attente d'un thread disponible. Dès la réception du signal, le threadpool attribuera un thread libre au runnable en attente.


## Réponses aux questions

La taille optimale du threadpool correspond au nombre de coeurs logiques de la machine utilisée. En effet plus de threads seraient inutiles car on ne pourrait pas les run en même temps que les autres et leur création et maintient prendrait des ressources. Moins de threads rendraient certains coeurs inutilisés alors que des requêtes pourraient être en attente.

Lors d'un stress test nous constatons que la stabilité s'est améliorée. Ceci est principalement dû à deux raisons :
- Nous utilisons maintenant une fonction tryPut pour mettre des éléments dans le buffer. Ainsi si le buffer est plein, il laissera tomber la requêtes et aucune ressource ne sera consommée pour de la mise en attente.
- Nous avions constaté lors de la première partie du laboratoire que l'OS stoppait le programme si trop de threads étaient créés. Avec un threadpool ce ne sera plus le cas, même si nous recevons beaucoup de requêtes, car il ne créera pas plus de threads que la taille de son pool. Ceci bien sur en supposant que la taille de son pool est correctement définie comme expliqué ci-dessus.
