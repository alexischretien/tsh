/**
 * UQAM - Hiver 2017 - INF3172 - Groupe 10 - TP2
 *
 * fin.c - Fichier source d'un programme qui affiche les dernieres
 * lignes d'un fichier. 
 *
 * Le programme prend deux arguments : le nombre de lignes a afficher et 
 * le chemin absolu ou relatif du fichier. Si le fichier contient moins
 * de lignes que le nombre en argument, le programme affiche le fichier
 * au complet. 
 *
 * @Auteur    Alexis Chretien (CHRA25049209)
 * @Version   7 avril 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int TAILLE_TAMPON = 128;

/**
 * Fonction qui valide les arguments de lancement du programme et 
 * retourne le nombre de lignes a afficher. 
 *
 * Met fin a l'execution du processus si les arguments sont invalides.
 *
 * @param  nbArguments      Le nombre d'arguments. 
 * @param  arguments        Les arguments de lancement du programme. 
 * @return                  Si les arguments sont valides, retourne 
 *                          le nombre de lignes a afficher.
 */
int validerArguments(int nbArguments, char* arguments[]) {

    int i;
    int nbLignes;
    FILE* fichier;

    // si mauvais nombre d'arguments
    if (nbArguments != 3) {
        fprintf(stderr, "Format invalide\n");
        exit(1);
    }

    // si le premier argument contient des caracteres non numerique
    for (i = 0 ; i < strlen(arguments[1]) ; ++i) {
        if (arguments[1][i] < '0' || arguments[1][i] > '9') {
            fprintf(stderr, "Argument invalide\n");
            exit(1);
        }    
    }
   
    fichier = fopen(arguments[2], "r");

    // si le fichier n'existe pas ou ne peut etre lu. 
    if (fichier == NULL) {
        perror("Fichier introuvable");
        exit(1);
    }
    fclose(fichier);
    
    nbLignes = atoi(arguments[1]);

    // si le premier argument n'est pas strictement positif.
    if (nbLignes < 1) {
        fprintf(stderr, "Argument invalide\n");
        exit(1);
    }
    return nbLignes;
}

/** Fonction qui affiche les "nbLignesAfficher" dernieres lignes du fichier
 * "nomFichier".
 *
 * @param   nbLignesAfficher        Le nombre de lignes a afficher.
 * @param   nomFichier              Le chemin relatif ou absolu du fichier a afficher.
 * @return 
 */
void afficherFin(int nbLignesAfficher, char* nomFichier) {

    int nbLignes;
    char ligne[TAILLE_TAMPON];
    FILE* fichier = fopen(nomFichier, "r");

    fseek(fichier, 0, SEEK_END);
    
    nbLignes = 0;

    // Parcourir a partir de la fin en comptant les caracteres '\n'
    do {
        fseek(fichier, -2, SEEK_CUR);
        if((fgetc(fichier)) == '\n') {
           ++nbLignes;
        }
    }
    while (nbLignes < nbLignesAfficher && ftell(fichier) > 1);

    // Si on a pas suffisamment de lignes, repositionner en debut de fichier.
    if (nbLignes != nbLignesAfficher) {
        fseek(fichier, 0, SEEK_SET);
    }
    
    // afficher les lignes
    while (fgets(ligne, TAILLE_TAMPON, fichier) != NULL) {
        fprintf(stdout, "%s", ligne);
    }
}

/**
 * Main
 */
int main (int argc, char* argv[]) {

    int nbLignes;

    setenv("PATH", getcwd(NULL, TAILLE_TAMPON), 1); 

    nbLignes = validerArguments(argc, argv);
    afficherFin(nbLignes, argv[2]);

    exit(0);
}
