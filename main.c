//gcc -o prog main.c -lpthread -lm

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

//Les paramètres
#define NBELEM 10000
#define TAILLEBLOC 200
#define NBMESURE 8
#define NBMAXTHREAD 1024

typedef struct
{
    double *a;
    double *b;
    double *array;
    int num;
    int methode;
    int nbThread;
}Data;

//Fonction pour générer un tableau aux valeurs aléatoires
double * generateArray(){
    double *tab = malloc(NBELEM * sizeof(double));
    if(tab == NULL){
        perror("malloc random array\n");
        exit(0);
    }
    for(int i=0; i<NBELEM; i++){
        tab[i] = rand() % 100; //nombre aléatoire compris entre 0 et 100
    }
    return tab;
}

//Fonction qui fait l'addition entre les tableaux selon la méthode choisie
void *add(void * data){
    Data *d = (Data*)data;
    if(d->methode == 1){ //méthode 1, on incrémente i du nombre de threads
        for(int i=d->num; i<NBELEM; i=i+d->nbThread){
            d->array[i] = d->a[i]+d->b[i];
            printf("thread n°%d || tab ind n°%d || (a)%.f + (b)%.f = (array)%.f\n", d->num, i, d->a[i], d->b[i], d->a[i]+d->b[i]);
        }
    }

    if(d->methode == 2){ //méthode 2, on incrémente i de 1, tant qu'il est dans un interval qui lui appartient
        int tmp = 0; //nombre d'éléments parcouru du bloc actuel
        for(int i=d->num * TAILLEBLOC; i<NBELEM; i++){
            tmp++;
            d->array[i] = d->a[i]+d->b[i];
            printf("thread n°%d || tab ind n°%d || (a)%.f + (b)%.f = (array)%.f\n", d->num, i, d->a[i], d->b[i], d->a[i]+d->b[i]);
            if(tmp>=TAILLEBLOC){ //si il a parcouru tous les éléments de son bloc, on change de bloc
                tmp = 0;
                i+=TAILLEBLOC*(d->nbThread-1);
            }
        }
    }

    if(d->methode == 3){ //cherche une case libre, fait le calcul pour la remplir
        for(int i=0; i<NBELEM; i++){
            if(d->array[i] == 0){
                d->array[i] = d->a[i]+d->b[i];
                printf("thread n°%d || tab ind n°%d || (a)%.f + (b)%.f = (array)%.f\n", d->num, i, d->a[i], d->b[i], d->a[i]+d->b[i]);
            }
        }
    }
    
}

//Fonction d'addition entre deux tableaux
double * additionTab(double * a, double * b, int methode, int thread, int migration){
    pthread_t *tabThread = (pthread_t *)malloc(thread*sizeof(pthread_t)); //tableau de threads
    if(tabThread == NULL){
        perror("malloc thread array\n");
        exit(0);
    }

    double *tabAdd = malloc(NBELEM * sizeof(double)); //tableau résultat de l'addition
    if(tabAdd == NULL){
        perror("malloc final array\n");
        exit(0);
    }

    printf("-------------------------\n");
    //Vérification des paramètres
    if(methode == 1){
        printf("méthode : Répartition cyclique des éléments\n");
    }else if(methode == 2){
        printf("méthode : Répartition cyclique des blocs d'éléments\n");
    }else if(methode == 3){
        printf("méthode : Répartition à la demande\n");
    }else{
        perror("erreur méthode\n");
        exit(0);
    }

    if(thread < 1){ //ramener le nombre de threads à une valeur correcte
        thread = 1;
    }else if(thread > NBMAXTHREAD){
        thread = NBMAXTHREAD;
    }
    printf("nombre de threads : %d\n", thread);

    if(migration == 0){
        printf("migration entre les coeurs : non\n");
    }else{
        printf("migration entre les coeurs : oui\n");
    }
    printf("-------------------------\n");

    //creation des threads
    for(int i=0; i<thread; i++){ 
        Data *d = malloc(sizeof(Data)); //structure avec tous les paramètres à passer à la fonction add
        d->a = a;
        d->b = b;
        d->array = tabAdd;
        d->methode = methode;
        d->num = i;
        d->nbThread = thread;
        if(pthread_create(&tabThread[i],NULL,add,(void *)d)){ //création du thread et envoi de la structure en paramètre
            perror("Creation thread\n");
            exit(0);
        }
    }

    //attendre la fin d'exécution des threads avant de terminer le programme
    for(int i=0; i<thread; i++){
        pthread_join(tabThread[i], NULL);
    }

    printf("-------------------------\n");

    free(tabThread);
}

int main(int argc, char *argv[]){

    char *filename = "data.csv";
    FILE *fp;

    if(argc !=4){
        perror("nombre d'argument invalide");
        exit(0);
    }

    int mode = atoi(argv[1]);
    int nbThread = pow(2,atoi(argv[2]));
    int migration = atoi(argv[3]);

    double *a, *b;
    struct timeval start, end;
    double avg=0;

    for(int i=0; i<NBMESURE; i++){
        gettimeofday(&start, NULL);
        a = generateArray();
        b = generateArray();
        additionTab(a, b, mode, nbThread, migration);
        gettimeofday(&end, NULL);
        double duree = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec; //durée en micro secondes
        avg+=duree/1000; //conversion en milisecondes
    }

    avg = avg/NBMESURE;
    printf("durée moyenne execution : %.2fms\n", avg);

    fp = fopen(filename, "a"); //ouverture en append -> ajout après le texte
    if (fp == NULL)
    {
        printf("Erreur ouverture fichier");
        return -1;
    }
    fprintf(fp, "%d;%d;%d;%.2f\n", mode, nbThread, migration,avg); //écriture dans le fichier CSV

    free(a);
    free(b);
    fclose(fp);
}