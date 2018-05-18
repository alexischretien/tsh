/** UQAM - Hiver 2017 - INF3172 - Groupe 10 - TP2
 *
 * list.c - Fichier source d'un programme qui affiche les sous-repertoires ou les
 * fichiers du repertoire de travail local.
 *
 * Le programme prend au plus un argument, "-d" pour afficher les sous-repertoires. 
 * Sans l'argument "-d", le programme affiche seulement les fichiers. 
 *
 * @Auteur      Alexis Chretien (CHRA25049209)
 * @Version     7 avril 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

/**
 * Fonction qui procede a la validation de des arguments.
 *
 * Met fin a l'execution du processus si les arguments sont
 * invalides. 
 *
 * @param       nbArguments   Le nombre d'arguments, incluant
 *                            le nom de l'executable.
 * @param       arguments     Les arguments.
 * @return
 */
void validerArguments(int nbArguments, char** arguments) {

    if (nbArguments > 2 
            || (nbArguments == 2 && strcmp(arguments[1], "-d") != 0)) {
    
        fprintf(stderr, "Format invalide\n");
        exit(1);
    }
}

/**
 * Fonction qui affiche les fichiers ou les sous-repertoires du repertoire
 * local. 
 *
 * Affiche les fichiers si type == DT_REG. 
 * Affiche les sous-repertoires si type == DT_DIR.
 *
 * @param   type        Le type de fichiers a afficher.
 * @return
 */
void afficherFichiersOuRep(unsigned char type) {

    DIR* repertoire;
    struct dirent* lecteurRep;

    repertoire = opendir(getenv("PATH"));
    lecteurRep = readdir(repertoire);

    fprintf(stdout, "%s de %s\n", (type == DT_REG ? "Fichiers" : "Sous-repertoires"), 
                                  getenv("PATH"));
    
    while(lecteurRep != NULL) {
        if (lecteurRep->d_type == type 
                && strcmp(lecteurRep->d_name, ".")  != 0 
                && strcmp(lecteurRep->d_name, "..") != 0) {
    
            fprintf(stdout, "%s\n", lecteurRep->d_name);
        }
        lecteurRep = readdir(repertoire);
    }
}

/**
 * Main
 */
int main (int argc, char** argv) {

    unsigned char type; 

    validerArguments(argc, argv); 
    type = (argv[1] == NULL ? DT_REG : DT_DIR);
    afficherFichiersOuRep(type);
    exit(0);
} 
