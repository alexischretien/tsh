/**
 * UQAM - Hiver 2017 - INF3172 - Groupe 10 - TP2
 *
 * new.c - Fichier source d'un programme de creation de fichier
 *         vide.
 *
 *         Le programme ne prend qu'un seul argument : le chemin relatif
 *         ou absolu du fichier a creer.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char** argv) {

    // Si mauvais nombre d'arguments
    if (argc != 2) {
        fprintf(stderr, "Format invalide\n"); 
        exit(1);
    }
    else {

        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

        // ouvrir le fichier en mode ecriture, seulement s'il n'existe pas.
        // L'auteur a des droit de lecture et d'ecriture, les membres du groupes
        // et tous les autres ont seulement des droits de lecture.
        if (open(argv[1], O_WRONLY | O_CREAT | O_EXCL, mode) < 0) { 
            perror("Impossible de creer le fichier");
            exit(1);
        }
        // si creation reussie
        else {
            fprintf(stdout, "Fichier cree\n");
        }
    }
    exit(0);
}
