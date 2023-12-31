#include <stdio.h>

#include <stdlib.h>

// Structure pour représenter une paire d'opérations interdites
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

void tri(int* deg, int max, int* liste) //tri decroissant des degre des sommet par ordre décroissant
{
    int temp;
    int temp1;
    for(int i = 0; i< max-1; i++){
        for (int j =0; j < max -i-1; j++){
            if(deg[j] < deg[j+1]){
                temp = deg[j];
                deg[j] = deg[j+1];
                deg[j+1] = temp;

                temp1 = liste[j];
                liste[j] = liste[j+1];
                liste[j+1] = temp1;
            }
        }

    }
}


graphe* Graphe(FILE* ifs,graphe* g){ //creation du graphe
    int op1, op2;
//lecture nombre de lignes
    int c;
    int nbLigne = 1;
    while((c= fgetc(ifs)) != EOF){
        if (c=='\n'){
            nbLigne++;
        }
    }

    printf("orde %d\n",g->ordre);
    g->taille = nbLigne;
    printf("taille %d\n",g->taille);
    g->sommet = (arette*)malloc(g->taille*sizeof(arette));
    g-> liste = malloc (sizeof(int) * g->ordre);

    //retour au debut du texte
    fseek(ifs, 0, SEEK_SET);

    int nbs =0; // compteur nombre de sommet

    //lecture de chaque contraintes
    for (int i=0; i<g->taille; ++i) {
        int v1 = 0; //valeur lors du parcours de liste 1 si op1 deja là
        int v2 = 0; //valeur lors du parcours de liste 1 si op2 deja là
        // créer les arêtes du graphe
        fscanf(ifs, "%d%d", &op1, &op2);

        g->sommet[i].op1.val = op1;
        g->sommet[i].op2.val = op2;
        printf("VALEUR: %d\n",g->sommet[i].op1.val);
        printf("VALEUR: %d\n",g->sommet[i].op2.val);
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

            if (v1 == 0) {
                g->liste[nbs] = op1;
                nbs++;
            }

            if (v2 == 0) {
                g->liste[nbs] = op2;
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

   //g->ordre = nbs;
    printf("2 -orde %d\n",g->ordre);
    

    int s; //sommet temporaire
    int* deg = malloc(sizeof(int) * g->ordre); //tableau des dégré des sommets

    for (int k=0; k < g->ordre; k++){

        int f =0; //valeur sommet relié à 0 ou non
        s = g->liste[k];
        int cpt = 0; //compte les degre sur sommet s

        for (int j = 0; j < g->taille; j++) {
            if (g->sommet[j].op1.val == s) //une liaion
            {
                cpt ++;
            }

            if (g->sommet[j].op2.val == s) //une liaion
            {
                cpt ++;
            }

            if (g->sommet[j].op2.val == 0 && g->sommet[j].op1.val == s )
            {
                deg[k] = 0;
                printf("OK %d et %d %d\n",g->sommet[j].op2.val,g->sommet[j].op2.val,deg[k]);
                f = 1;
            }

            if (g->sommet[j].op2.val == s && g->sommet[j].op1.val == 0 ) //une liaion
            {
                deg[k] = 0;
                printf("OK %d et %d %d\n",g->sommet[j].op2.val,g->sommet[j].op2.val,deg[k]);
                f = 1;
            }

            if (s == 0 ) //somme inutile
            {
                f = 1;
            }


        }

        if(f== 0) {
            deg[k] = cpt;
        }
        else{
            deg[k] =0;
        }

    }

    tri(deg,g->ordre,g->liste);

    for (int i=0; i<g->ordre; i++){
        printf("def de %d: %d\n",g->liste[i],deg[i]);
    }

    if(g->taille > g->ordre) {

        for (int i = 0; i < g->ordre; i++) {
            for (int j = 0; j < g->ordre; j++) {
                if (g->sommet[j].op1.val == g->liste[i]) {
                    printf("VAL %d;\n", g->sommet[j].op1.val);
                    g->sommet[j].op1.indice = i;
                }

                if (g->sommet[j].op2.val == g->liste[i]) {
                    printf("VAL %d;\n", g->sommet[j].op2.val);
                    g->sommet[j].op2.indice = i;
                }
            }

        }

    }

    else{

        for (int i = 0; i < g->ordre; i++) {
            for (int j = 0; j < g->taille; j++) {
                if (g->sommet[j].op1.val == g->liste[i]) {
                    printf("VAL %d;\n", g->sommet[j].op1.val);
                    g->sommet[j].op1.indice = i;
                }

                if (g->sommet[j].op2.val == g->liste[i]) {
                    printf("VAL %d;\n", g->sommet[j].op2.val);
                    g->sommet[j].op2.indice = i;
                }
            }

        }

    }


    //remplir matrice


    g->adj = malloc(g->ordre*sizeof(int*)); //matrice d'adjacence
    for (int i =0; i< g->ordre; i++) {
        g->adj[i] = calloc(g->ordre, sizeof(int));

        if (!g->adj[i]) {
            perror("Allocation de mémoire pour la matrice d'adjacence a échoué");
            exit(EXIT_FAILURE);
        }

    }


for (int i=0;i<g->taille;i++){
printf("\n");
    for(int j = 0;j<g->taille;j++) {
        //printf("%d OK\n",j);
        int val1 = g->sommet[j].op1.val;
        int val2 = g->sommet[j].op2.val;
        //printf("inde %d\n", g->sommet[j].op1.val);
        //printf(" inde %d\n\n", g->sommet[j].op2.val);

        g->adj[val1][val2] = 1;
        g->adj[val2][val1] = 1;

    }
}

///////////////////

    for (int i =0;i<g->ordre;i++){
        for(int j=0;j<g->ordre;j++){
            printf("%d -",g->adj[i][j]);
        }
        printf("\n");
    }
    printf(" OK\n");


    ///////////


    //well et powell

    printf("\n\nWELSH ET POWELL");
    int s1;
    int* color = (int*) calloc(g->ordre,sizeof(int)); //0 partout si pas de couleur
    int couleur = 1;

//dans ADJ = nbre= indice
    for(int i = 0; i<g->ordre; i++) {
        s1 = g->liste[i]; //sommet dans l'ordre
        printf("\nS1 = %d\n",s1);
        printf("COULEUR S1 %d\n",color[s1]);

        if (color[s1] == 0) { //si non colorer
            color[s1] = couleur;
            printf("2 -COULEUR S1 %d\n",color[s1]);

            for (int j = i+1; j < g->ordre; j++) { // coloration de sommet en fonction de lien avec s1 ou non

                int s2 = g->liste[j];
                if (g->adj[s1][s2] == 0 && color[s2] == 0) { // pas de lien avec s1
                    printf("%d n'as pas de lien avec %d\n", s1, s2);
                    color[s2] = couleur;
                    printf("Couleur de %d == %d indice%d\n",s2,color[s2],j);

                    for (int l = 0; l < j; l++) {
                        int s3 = g->liste[l];
                        if (g->adj[s2][s3] == 1 && color[s3] == color[s2] ) { // lien entre sommet de meme couleur
                            color[s3] = 0;  //changment de couleur
                            printf("NOPE1");
                            break;
                        }

                         else {
                             printf("ok pour %d (c%d) et %d(c%d) adj == %d \n", s2,color[s2], s3,color[s3],g->adj[s2][s3]);
                         }


                    }
                }

            }
            couleur++;
        }
    }

    printf("couleur %d",couleur-1);


    for(int i =0; i< couleur;i++){
        printf("\ncoueleur %d: ",i);
        for (int j=0; j<g->ordre;j++){
            if (color[j] == i){
                printf("%d - ",j);
            }
        }
    }

    free(color);

    return g;
}




int main() {
    graphe * g=(graphe*)malloc(sizeof(graphe));
    g->sommet = NULL;
    g->liste = NULL;
    g->adj = NULL;
    char fichier[100];
    printf("ENTREZ LE NOM DU FICHIER (avec le .txt):");
    scanf("%s",fichier);
    FILE * ifs = fopen(fichier,"r");

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    g->ordre = 31;

    Graphe(ifs,g);


    free(g->sommet);
    free(g->liste);
    for(int i=0; i<g->ordre; i++){
        free(g->adj[i]);
    }
    free(g);
    fclose(ifs);

    return 0;
}