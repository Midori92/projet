#include <stdio.h>

#include <stdlib.h>

// Structure pour représenter une paire d'opérations interdites
typedef struct {
    int operation1;
    int operation2;
} Exclusion;


typedef struct sommet{
    int nom;
    int couleur;
}sommet;

struct arette{
    int op1;
    int op2;
};

typedef struct arette arette;
typedef struct graphe {
    arette* arbre;
    int ordre;
    int taille;
}graphe;

//tableau des sommets

graphe* lecture(char* fichier){
    {

        graphe * g=(graphe*)malloc(sizeof(graphe));
        FILE * ifs = fopen(fichier,"r");
        int op1, op2;


        if (!ifs)
        {
            printf("Erreur de lecture fichier\n");
            exit(-1);
        }

//lecture nombre de lignes
        int c;
        int nbLigne = 0;
        while((c= fgetc(ifs)) != EOF){
            if (c=='\n'){
                nbLigne++;
                printf("nbLigne == %d\n", nbLigne);
            }
        }

        g->taille = nbLigne;

        g->arbre = (arette*)malloc(g->taille*sizeof(arette));

        //retour au debut du texte
        fseek(ifs, 0, SEEK_SET);

        int* liste[50];
        int nbs =0; // compteur nombre de sommet

       //lecture de chaque contraintes
       for (int i=0; i<g->taille; ++i) {
           int v1 = 0; //valeur ors du parcours de liste 1 si op1 deja là
           int v2 = 0; //valeur ors du parcours de liste 1 si op2 deja là
           // créer les arêtes du graphe
           fscanf(ifs, "%d%d", &op1, &op2);
           printf("ARET %d VS %d\n\n", op1, op2);
           printf("sommet pour l'instant: %d\n", nbs);
           g->arbre[i].op1 = op1;
           g->arbre[i].op2 = op2;

           if (nbs >= 1) {
               for (int j = 0; j < nbs; j++) {
                   if (liste[j] == op1) //si op1 deja dans la liste
                   {
                       v1 = 1;
                       printf("op1 == %d deja dans liste \n", op1);
                   }

                   if (liste[j] == op2) //si op2 deja dans la liste
                   {
                       v2 = 1;
                       printf("op2 == %d deja dans liste \n", op2);
                   }

               }

                   if (v1 == 1 && v2 == 1) {
                       printf("les 2 sont deja dans liste: %d et %d  \n", op1, op2);
                   }

                   if (v1 == 0) {
                       liste[nbs] = op1;
                       nbs++;
                       printf("ajout du sommet %d dans la liste\n", op1);
                   }

                   if (v2 == 0) {
                       liste[nbs] = op2;
                       nbs++;
                       printf("ajout du sommet %d dans la liste \n", op2);
                   }


           }
           else { //nbs ==0
                liste[nbs] = op1;
                printf("op1 ajout %d\n",op1);
                nbs++;
                liste[nbs] = op2;
                printf("op2 ajout %d\n", op2);
                nbs++;
           }
       }
       printf("\n\n NOMBRE SOMMET FINAL/ %d",nbs);

       g->ordre = nbs;
       int s; //sommet temporaire

       int*deg = malloc(sizeof(int*) * g->ordre); //tableau des dégré des sommets

       printf("Boucle\n");
       for (int i=0;i<g->ordre;i++){
           s = liste[i];
           printf("s= %s\n",s);
           int cpt = 0; //compte les degre sur sommet s

           for (int j =0; j<g->taille;j++) {
               if (g->arbre[j].op1 == s || g->arbre[j].op2 == s) //une liaion
               {
                   cpt ++;
                   printf("une liaison du s= %d \n",s);
               }
           }

           printf("DEGRE DE %d: %d\n",s, cpt);
           deg[i] = cpt;
       }

        return g;
    }
}

/*
// Fonction pour lire les contraintes d'exclusion à partir d'un fichier
Exclusion* lecture(int* nombreContraintes) {

    char nom[50];
    printf("Entrez le nom du fichier (avec le .txt): \n");
    scanf("%s",nom);

    FILE* ifs = fopen(nom, "r");
    if (!ifs) {
        printf( "Erreur ouverture du fichier\n");
        exit(-1);
    }

    int tailleTableau = 20;

    Exclusion* contraintes = malloc(tailleTableau * sizeof(Exclusion));

    int nbContraintes = 0;

    while (fscanf(ifs, "%d %d", &contraintes[nbContraintes].operation1, &contraintes[nbContraintes].operation2) == 2) {
        nbContraintes++;

        if (nbContraintes == tailleTableau) {
            tailleTableau *= 2;
            contraintes = realloc(contraintes, tailleTableau * sizeof(Exclusion));
        }
    }

    fclose(ifs);

    printf("Liste des contraintes:\n");

    for(int i =0; i<nbContraintes;i++)
    {
        printf("%d %d \n",contraintes[i].operation1, contraintes[i].operation2);
    }

    *nombreContraintes = nbContraintes;
    return contraintes;
}
*/


// Fonction pour vérifier si une paire d'opérations est autorisée sur une station
int estAutoriseeSurStation(int station, int* affectations, Exclusion* contraintes, int nombreContraintes, int operation) {
    for (int i = 0; i < nombreContraintes; i++) {
        if ((contraintes[i].operation1 == operation || contraintes[i].operation2 == operation) &&
            affectations[contraintes[i].operation1 - 1] == station && affectations[contraintes[i].operation2 - 1] == station) {
            return 0; // La paire d'opérations interdites est affectée à la même station
        }
    }
    return 1; // La paire d'opérations est autorisée sur la station
}

// Fonction pour répartir les opérations sur les stations en respectant les contraintes
void repartirOperations(int nombreOperations, int nombreStations, int nombreContraintes, Exclusion* contraintes) {

    int* affectations = calloc(nombreOperations, sizeof(int));

    for (int i = 0; i < nombreOperations; i++) {
        for (int j = 1; j <= nombreStations; j++) {
            if (estAutoriseeSurStation(j, affectations, contraintes, nombreContraintes, i + 1)==1) {
                affectations[i] = j;
                break;
            }
        }
    }

    // Affichage des affectations
    printf("\nRepartition des operations:\n");
    for (int i = 0; i < nombreOperations; i++) {
        printf("Operation %d affectee a la station %d\n", i + 1, affectations[i]);
    }

    free(affectations);
}


//graphe
// nombre chromatique



int main() {

   /* int nombreContraintes;
    Exclusion* contraintes = lecture( &nombreContraintes);

    int nombreOperations = 35;  // Nombre total d'opérations
    int nombreStations = 20;    // Nombre total de stations

    repartirOperations(nombreOperations, nombreStations, nombreContraintes, contraintes);

    // Libération de la mémoire
    free(contraintes);
    */
   lecture("test.txt");

    return EXIT_SUCCESS;
}