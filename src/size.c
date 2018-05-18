/**
 * UQAM - Hiver 2017 - INF3172 - Groupe 10 - TP2
 *
 * size.c - Fichier source d'un programme affichant le nombre et la 
 * somme totale des tailles des fichiers et contenus dans un 
 * repertoire. 
 *
 * Cette somme est differente de la taille reellement occupee et ne
 * prend pas en consideration les octets non-occupes des blocs 
 * allouees a ces fichiers.  
 *
 * Le programme ne prend qu'un seul argument : le chemin absolu ou 
 * relatif du repertoire dont on veut connaitre les informations.
 *
 * @Auteur   Alexis Chretien (CHRA25049209)
 * @Version  7 avril 2017
 */

#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "chemin.h"

const int TAILLE_TAMPON = 128;

/**
 * Fonction qui valide les arguments de lancement du programme.
 *
 * Met fin a l'execution du processus si les arguments sont invalides.
 *
 * @param   nbArguments     Le nombre d'arguments.
 * @param   arguments       Les arguments de lancement du programme
 * @return
 */
void validerArguments(int nbArguments, char* arguments[]) {

    DIR* repertoire;
    
    // verification sur le nombre d'arguments
    if (nbArguments != 2) {
        fprintf(stderr, "Format invalide\n");
        exit(1);
    }

    // verifier que le repertoire existe.
    repertoire = opendir(arguments[1]);

    if (repertoire == NULL) {
        perror("Repertoire introuvable");
        exit(1);
    }
    closedir(repertoire);
}

/**
 * Fonction qui calcule le nombre et la somme (en octets) des tailles des 
 * fichiers contenus dans un repertoire.
 *
 * @param  nomRep    Le chemin relatif du repertoire dont on veut afficher 
 *                   les informations.
 * @param  pair      Couple d'entier servant a enregistrer les valeurs de 
 *                   sorties. 
 *                   pair[0] => nombre de fichiers.
 *                   pair[1] => somme des tailles des fichiers.
 * @return
 */
void calculerNombreEtTaille(char* nomRep, int pair[2]) {

    DIR* repertoire;
    struct dirent* lecteurRep;
    struct stat statFichier;
    char* cheminFichier;
    char* repCourant;
    char* repEnfant;
    
    repCourant = malloc(strlen(getenv("PATH"))+1);
    strcpy(repCourant, getenv("PATH"));

    repEnfant = malloc(strlen(repCourant) + strlen(nomRep) + 2);
    sprintf(repEnfant, "%s/%s", repCourant, nomRep);
 
    repertoire = opendir(nomRep);
    
    setenv("PATH", repEnfant, 1);
    chdir(getenv("PATH"));
  
    while ((lecteurRep = readdir(repertoire)) != NULL) {
        
        if ((lecteurRep->d_type != DT_DIR) ||
                (lecteurRep->d_type == DT_DIR 
                && strcmp(lecteurRep->d_name, "..") != 0 
                && strcmp(lecteurRep->d_name, "." ) != 0)) {
                
            cheminFichier = malloc(strlen(repEnfant) + strlen(lecteurRep->d_name) + 2);
            sprintf(cheminFichier, "%s/%s", repEnfant, lecteurRep->d_name);
            stat(cheminFichier, &statFichier);

            if (lecteurRep->d_type != DT_DIR) {
                pair[0] += 1;
                pair[1] += statFichier.st_size;
            }
            else if (lecteurRep->d_type == DT_DIR ) {
                calculerNombreEtTaille(lecteurRep->d_name, pair);    
            }
        }
    }
    setenv("PATH", repCourant, 1);
    chdir(getenv("PATH"));
    free(repEnfant);
    free(repCourant);
}  

/**
 * Main
 */
int main (int argc, char* argv[]) {

    int donnees[2] = {0,0};

    setenv("PATH", getcwd(NULL, TAILLE_TAMPON), 1);
    validerArguments(argc, argv);
    deplacerVersParent(&argv[1]);
    calculerNombreEtTaille(argv[1], donnees); 

    fprintf(stdout, "Le repertoire %s/%s contient %d fichiers pour "
                    "un total de %d octets.\n", getenv("PATH"), argv[1], donnees[0], donnees[1]);

    exit(0);
}
