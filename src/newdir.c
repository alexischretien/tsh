/**
 * UQAM - Hiver 2017 - INF3172 - Groupe 10 - TP2
 *
 * newdir.c - Fichier source d'un programme qui procede a la creation
 * d'un repertoire vide via un chemin relatif ou absolu. 
 *
 * Le programme ne prend qu'un seul argument : le chemin relatif ou absolu
 * du repertoire a creer.
 *
 * @Auteur   Alexis Chretien (CHRA25049209)
 * @Version  7 avril 2017
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

const int TAILLE_TAMPON = 128;

/**
 * Fonction qui verifie si le programme a ete lance avec le bon nombre
 * d'arguments. 
 *
 * Met fin a l'execution du programme si ce n'est pas le cas.
 *
 * @param  nbArguments    Le nombre d'arguments avec lesquels le programme
 *                        a ete lance.
 * @return
 */
void validerArguments(int nbArguments) {

    // si mauvais nombre d'arguments
    if (nbArguments != 2) {
        fprintf(stderr, "Format invalide\n");
        exit(1);
    }
}

/**
 * Main
 */
int main (int argc, char* argv[]) {

    setenv("PATH", getcwd(NULL, TAILLE_TAMPON), 1);

    validerArguments(argc);
 
    if (mkdir(argv[1], S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0) {
        perror("Impossible de creer le repertoire");
        exit(1);
    }
    fprintf(stdout, "Repertoire cree\n");
    exit(0);
}
