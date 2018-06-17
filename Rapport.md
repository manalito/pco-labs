#PCO - Labo 7 Server part 3

**Auteurs** : *Aurélien Siu et Romain Gallay*
**Date**: *16.06.2018*


## Fonctionnalité du logiciel

Durant ce laboratoire nous travaillons sur un serveur de fichier utilisant comme canal de communication une **WebSocket**. Pour cette troisième partie notre but sera d'utiliser une **cache** qui gardera en mémoire durant un temps défini le résultat d'une requête, ceci pour éviter des accès en lecture superflus sur les fichiers.


## Choix d'implémentation

Notre implémentation du threadpool lors du labo précédent ne fonctionnant pas correctement, nous avons décidé de partir du code du labo 5 qui créait un thread par requête. Nous avons ensuite modifié les classes suivantes :

#### RequestDispatcherThread

De base à chaque requête reçue, cette classe crée un thread de type **RequestProcessor** qui se chargera de traiter la requête. Nous l'avons modifiée comme suit :
La classe crée maintenant un `ReaderWriterCache` dès son initialisation. Lorsqu'elle reçoit une requête, elle vérifie d'abord si la réponse à la requête se trouve dans le cache. Le cas échéant, elle met ce résultat dans le buffer de reponses, sinon elle crée un `RequestProcessor` pour traiter la requête.

#### RequestProcessor

Cette classe se charge de traiter les requêtes. Nous l'avons modifiée ainsi :
Lorsqu'une requête a été traitée, on met la réponse dans la cache en plus de la mettre dans le buffer de reponses.

#### ReaderWriterLock

Une simple classe de **lecteur-rédacteur** implémentée à l'aide d'un **moniteur de Hoare**. Comme ce n'était pas demandé, nous avons décidé de ne pas inclure de priorité ni pour les lecteurs ni pour les rédacteurs.

#### ReaderWriterCache

Cette classe implémente un système de cache, notamment avec l'aide de **ReaderWriterLock** et d'une hash-table (`QHash`). 
La fonction `putResponse` fait un accès en écriture à la map, elle est donc bornée par les fonctions `lockWriting` et un `unlockWriting`. À l'intérieur, elle met dans la map la réponse passée en argument avec un **timestamp** du moment actuel pour gérer la durée de validité d'une réponse.
La fonction `tryGetCachedResponse` fait un accès en lecture à la map et a donc comme borne `lockReading` et `unlockReading`. Si une réponse se trouve dans la map, on retourne une **option** la contenant, autrement on retourne une option vide.

## Réponses aux questions

La cache améliore les performances dans 2 situations :
- si plus d'un client lance une requête pour un même fichier,
- si un même client lance plus d'une requête pour un même fichier,

ceci durant l'intervalle de temps correspondante à staleDelaySec.

Les schémas d'utilisation suivant pourraient être problématiques :

1) Les données de la cache sont enregistrées dans la mémoire RAM. Si plusieurs fichiers de taille importante tentent d'être chargés dans la cache, il y a un risque de remplir la RAM et que celle-ci soit insuffisante pour contenir tous les fichiers.

2) La durée de validité d'un fichier est actuellement de 2 minutes. Dans un cas où les fichiers changent toutes les minutes ou plus, il faudrait réduire cette durée. Si les fichiers changent trop souvent la cache peut devenir plus un problème qu'une aide, car soit le fichier en cache ne correspond plus au fichier actuel, soit il faut recharger le fichier dans le cache ce qui prend des ressources.

---

Les classes  en lien avec le cache ont pu être testées à l'aide de la classe `tst_readerwriterchache` prévue à cet effet. Tous les tests ont passés sur Windows. Dès que nous sommes passés sur Linux, nous avons rencontrés un soucis au niveau du 3 ème test. Nous n'avons cependant pas trouvé l'origine de l'erreur. La voici:

	********* Start testing of ReaderWriterCacheTest *********
	PASS   : ReaderWriterCacheTest::initTestCase()
	PASS   : ReaderWriterCacheTest::testSingleReadWrite()

	========= Received signal, dumping stack ==============
	GNU gdb (Ubuntu 7.11.1-0ubuntu1~16.5) 7.11.1
	Copyright (C) 2016 Free Software Foundation, Inc.
	License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
	This is free software: you are free to change and redistribute it.
	There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
	and "show warranty" for details.
	This GDB was configured as "x86_64-linux-gnu".
	Type "show configuration" for configuration details.
	For bug reporting instructions, please see:
	<http://www.gnu.org/software/gdb/bugs/>.
	Find the GDB manual and other documentation resources online at:
	<http://www.gnu.org/software/gdb/documentation/>.
	For help, type "help".
	Type "apropos word" to search for commands related to "word".
	Attaching to process 4232
	(gdb) 
	========= End of stack trace ==============
	QFATAL : ReaderWriterCacheTest::testSingleReadWriteWithStale() Received signal 11
	Function time: 6001ms Total time: 7002ms
	FAIL!  : ReaderWriterCacheTest::testSingleReadWriteWithStale() Received a fatal error.
		Loc: [Unknown file(0)]
	Totals: 2 passed, 1 failed, 0 skipped, 0 blacklisted, 7147ms
	********* Finished testing of ReaderWriterCacheTest *********
