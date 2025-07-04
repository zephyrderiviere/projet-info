# Projet d'informatique sur les instructions **Intel Intrinsics**

Ce projet consiste en l'étude des Intel Intrinsics pour grandement augmenter les performances de calculs sur des vecteurs.

Concrètement, ils permettent à l'aide d'instructions au niveau du CPU, de réaliser dans un même temps des opérations élémentaires (d'addition, de multiplication etc...) sur plusieurs entiers, plusieurs nombre flottants à la fois. 

Cependant, un processeur donné ne supporte pas tous les sets d'instructions. Alors que les librairies grand public de calcul scientifique telles que Eigen choisissent le meilleur jeu d'instructions à la compilation, notre objectif dans ce projet est de créer un système s'adaptant au runtime au meilleur jeu d'instructions disponible sur la machine sur laquelle il est exécuté. Une fois le set d'instruction optimal déterminé, la librairie permet de réaliser des calculs élémentaires plus rapidement. Cela est particulièrement utile pour les petits laboratoires ne disposant pas de cluster de calculs dont les processeurs présentent des caractéristiques homogènes. 

Le projet se porte en particulier sur les sets d'instruction SSE42, AVX et AVX2. Il devrait également pouvoir être utilisé avec le set AVX512 mais ce-dernier n'a pas pu être testé n'étant pas compréhensible par les processeurs des ordinateurs utilisés. 

Au niveau de l'implémentation, on a choisi de créer une interface élémentaire pour communiquer avec les différents jeux d'instructions tandis qu'un calculateur utilise ces interfaces pour faire les calculs. 

Pour exécuter le projet, nous avons utilisé Boost chrono, test et Program Options. Pour obtenir une repésentation graphique, on peut utiliser le programme "generationGraph.cpp" de la facon suivante : $./generationGraph [n] > data.txt avec n la taille des vecteurs que l'on veut additionner. Sont alors écrit les durées d'addition pour des vecteurs de tailles 0 à n dans le fichier data.txt. Il suffit alors de faire $gnuplot tracer.plt pour créer un graphique en png.