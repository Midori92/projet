#include <stdio.h>

#include <stdlib.h>

// Structure pour représenter une paire d'opérations interdites
typedef struct {
    int operation1;
    int operation2;
} Exclusion;

struct point{
    int val;
    int indice;
};

typedef struct point point;

struct arette{
    point op1;
    point op2;
};


typedef struct arette arette;
typedef struct graphe {
    arette* sommet;
    int ordre;
    int taille;
    int* liste;
    int** adj;
}graphe;

//tableau des sommets

void tri(int* liste, int max) //tri des degre des sommet par ordre décroissant
{
    int temp;
    for(int i = 0; i< max-1; i++){
        for (int j =0; j < max -i-1; j++){
            if(liste[j] < liste[j+1]){
                temp = liste[j];
                liste[j] = liste[j+1];
                liste[j+1] = temp;

            }
        }

    }
}



graphe* exclusion(char* fichier){
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

        g->sommet = (arette*)malloc(g->taille*sizeof(arette));

        //retour au debut du texte
        fseek(ifs, 0, SEEK_SET);

        g-> liste = malloc (sizeof(int*));
        int nbs =0; // compteur nombre de sommet

       //lecture de chaque contraintes
       for (int i=0; i<g->taille; ++i) {
           int v1 = 0; //valeur ors du parcours de liste 1 si op1 deja là
           int v2 = 0; //valeur ors du parcours de liste 1 si op2 deja là
           // créer les arêtes du graphe
           fscanf(ifs, "%d%d", &op1, &op2);
           //printf("ARET %d VS %d\n\n", op1, op2);
           //printf("sommet pour l'instant: %d\n", nbs);
           g->sommet[i].op1.val = op1;
           g->sommet[i].op2.val = op2;

           if (nbs >= 1) {
               for (int j = 0; j < nbs; j++) {
                   if (g->liste[j] == op1) //si op1 deja dans la liste
                   {
                       v1 = 1;

                   }

                   if (g->liste[j] == op2) //si op2 deja dans la liste
                   {
                       v2 = 1;
                   }
               }
/*
                   if (v1 == 1 && v2 == 1) {
                      /// printf("les 2 sont deja dans liste: %d et %d  \n", op1, op2);
                   }
*/
                   if (v1 == 0) {
                       g->liste[nbs] = op1;
                       //printf("ajout du sommet %d dans la liste\n", g->liste[nbs]);
                       nbs++;
                   }

                   if (v2 == 0) {
                       g->liste[nbs] = op2;
                       //printf("ajout du sommet %d dans la liste \n", g->liste[nbs]);
                       nbs++;
                   }


           }

           else { //nbs ==0
               g->liste[nbs] = op1;
                nbs++;
               g->liste[nbs] = op2;
                nbs++;
           }
       }
      // printf("\n\n NOMBRE SOMMET FINAL/ %d\n",nbs);

       g->ordre = nbs;
       g->adj = (int**) calloc(g->ordre, sizeof(int *)); //matrice d'adjacence
       for (int i =0; i< g->ordre; i++){
           g->adj[i] = (int*) calloc(g->ordre, sizeof(int));
       }


       int s; //sommet temporaire
       int* deg = malloc(sizeof(int) * g->ordre); //tableau des dégré des sommets

       for (int k=0; k < g->ordre; k++){

           s = g->liste[k];
           int cpt = 0; //compte les degre sur sommet s

           for (int j = 0; j < g->taille; j++) {
               if (g->sommet[j].op1.val == s) //une liaion
               {
                   cpt ++;
                   printf(" 1 -une liaison du s= %d \n",s);
               }

               if (g->sommet[j].op2.val == s) //une liaion
               {
                   cpt ++;
                   printf("2 - une liaison du s= %d \n",s);
               }


           }

          // printf("DEGRE DE %d: %d\n",s, cpt);
           deg[k] = cpt;
       }

     //  printf("\n\nLISTE DEGRE 1: \n");

       for(int i=0;i< g->ordre; i++){
           //printf("Sommet : %d degre: %d \n",g->liste[i],deg[i]);
       }

        tri(deg,g->ordre);

      // printf("\n\nLISTE SOMMET RENG2\n");
       for (int i =0; i <g->ordre; i++){
           //printf("%d\n",g->liste[i]);
       }


        //printf("\n\nLISTE DEGRE 2: \n");
///sommet et indice
        for(int i=0;i< g->ordre; i++) {
           // printf("Sommet : %d degre: %d \n", g->liste[i], deg[i]);
            for (int j = 0; j < g->ordre; j++) {
                if(g->sommet[j].op1.val == g->liste[i]){
                    g->sommet[j].op1.indice = i;
                  //  printf("1- %d d'indice %d\n",g->sommet[j].op1.val,g->sommet[j].op1.indice);
                }

                if(g->sommet[j].op2.val == g->liste[i]){
                    g->sommet[j].op2.indice = i;
                   // printf("2- %d d'indice %d\n",g->sommet[j].op2.val,g->sommet[j].op2.indice);
                }
            }

        }

/////
//remplir matrice

        int indice1;
        int indice2;

        for(int i=0;i<g->taille;i++){

            indice1 = g->sommet[i].op1.indice;
            indice2 = g->sommet[i].op2.indice;

            g->adj[indice1][indice2] = 1;
            g->adj[indice2][indice1] = 1;

        }


        //well et powell

        printf("\n\nWELSH ET POWELL");
        int s1;
        int* color = (int*) calloc(g->ordre,sizeof(int*)); //0 partout si pas de couleur
        int couleur = 0;


        for(int i = 0; i<g->ordre; i++) {
            s1 = g->liste[i]; //sommet dans l'ordre

            if (color[i] == 0) { //si non colorer
                color[i] = couleur++;
               // printf("\n\nCOULEU: %d et s: %d\n", couleur, s1);


                for (int j = i; j < g->ordre; j++) { // coloration de sommet en fonction de lien avec s1 ou non

                    if (g->adj[i][j] == 0 && color[j] == 0) { // pas de lien avec s1
                        //printf("%d n'as pas de lien avec %d\n", s1, g->liste[j]);
                        color[j] = couleur;

                        for (int l = i; l < g->ordre; l++) {

                            if (g->adj[j][l] == 1 && color[l] == couleur) { // lien entre sommet de meme couleur
                                color[j] = 0;  //changment de couleur
                               // printf("erreur: couleur %d lien entre %d et %d\n", couleur, g->liste[j], g->liste[l]);
                                //printf("couleur de %d: %d\n",g->liste[j], color[j]);
                                break;
                            }

                           /* else {
                                printf("ok pour %d et %d \n", g->liste[j], g->liste[l]);
                            }
                            */

                        }
                    }


                    if (g->adj[i][j] == 1 || color[j] >0) {
                       // printf("NOPE %d: couleur : %d\n", g->liste[j],color[j]);
                    }
                }

            }
/*
            else{
                printf("\n deja colo");
            }
*/
/*
        printf("\n\n");
            for (int j=0; j<g->ordre;j++){
                if (color[j] == couleur){
                    printf("%d - ",g->liste[j]);
                }

        }*/
    }

        printf("couleur %d",couleur);

        for(int i =1; i<= couleur;i++){
            printf("\ncoueleur %d: ",i);
                for (int j=0; j<g->ordre;j++){
                    if (color[j] == i){
                        printf("%d - ",g->liste[j]);
                    }
                }
            }


        return g;
    }
}



int main() {

   exclusion("test.txt");

    return EXIT_SUCCESS;
}