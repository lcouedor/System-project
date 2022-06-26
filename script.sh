#!/bin/bash
#sh script.sh

rm data.csv
gcc -o prog main.c -lpthread -lm #compilation

#toutes les combinaisons possibles pour les 3 paramètres
for i in 1 2 3 #les 3 modes possibles
do
    for j in `seq 1 10` #les puissances de 2 jusqu'à 10 pour 2^10 = 1024
    do
        for k in 0 1 #migration ou non
        do
            ./prog $i $j $k
        done
    done
done