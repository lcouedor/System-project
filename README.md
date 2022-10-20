# System-project
Mini projet de système d'exploitation de L3 pour calculer l'efficacité d'utilisation du CPU

# Fonctionnement
Le fichier script fait l'exécution un certain nombre de fois du fichier "main.c" avec des paramètres différents. 
Le fichier C fonctionne avec plusieurs threads et fait l'addition d'entiers de deux tableaux de taille identique.
Le partage des tâches entre les threads se fait selon les paramètres saisis, selon 3 méthodes : 
  1 - Les threads effectuent leur calcul l'un après l'autre.
  2 - Chaque thread possède une plage qui lui est dédié, dans laquelle il est chargé de faire tous les calculs.
  3 - A la fin de sa tâche en cours, chaque thread cherche ensuite un calcul n'ayant pas encore été traité.

# Résultats
Les résultats sont écrits dans un fichier CSV à chaque exécution du script.
Le fichier "CR.pdf" indique dans un premier temps le fonctionnement du fichier C ainsi que du script.
Il indique dans un second temps les résultats obtenus et propose une analyse succincte de ces résultats.
