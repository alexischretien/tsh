/**
 * UQAM - Hiver 2017 - INF3172 - Groupe 10 - TP2
 *
 * rmall.c - Fichier source d'un programme procedant a la suppression 
 * recursive d'un repertoire specifie selon son chemin absolu ou
 * relatif. 
 *
 * Si le programme ne possede pas les droits necessaire pour proceder
 * a la suppression du repertoire, aucun changement ne lui est apporte.
 * Si le programme possede des droits suffisants pour le supprimer, il
 * procedera a la suppression systematique des fichiers et sous-repertoires.
 * Si un sous-repertoire ne peut etre supprimer, le processus se termine. 
 *
 * Le programme ne prend qu'un seul argument : le chemin relatif ou absolu 
 * du repertoire a supprimer.
 *
 * @Auteur      Alexis Chrétien (CHRA25049209)
 * @Version     4 avril 2017
 */

#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "chemin.h"

const int TAILLE_TAMPON = 128;

/**
 * Fonction qui valide les arguments donnees au programme. 
 *
 * Verifie que le programme a ete lance avec le bon nombre d'arguments, 
 * que le repertoire a supprimer existe et que le programme possede les
 * droits necessaires pour proceder a la suppression du repertoire. Si
 * Les arguments ne satisfaient pas ces conditions, un message d'erreur
 * est produit et l'execution du programme se termine.
 *
 * @param  nbArguments     Le nombre d'arguments donnes lors du lancement
 *                         du programme.
 * @param  arguments       Les chaines de caracteres que sont les arguments. 
 * @return
 */ 
void validerArguments(int nbArguments, char* arguments[]){

    pid_t idRmall; 
    char cheminRmall[TAILLE_TAMPON];
    struct stat statRmall;
    struct stat statRep;
    DIR* repertoire;
       
    // verifier le nombre d'arguments
    if (nbArguments != 2) {
        fprintf(stderr, "Format invalide\n");
        exit(1);
    }

    //verifier que le repertoire existe.
    repertoire = opendir(arguments[1]); 
    
    if (repertoire == NULL) {
        perror("Repertoire introuvable");
        exit(1);
    }
    closedir(repertoire);

    //verifier que le programme a les permissions d'ecriture necessaires
    //pour effecter la suppression du repertoire. 
    idRmall = getpid();
    sprintf(cheminRmall, "/proc/%d/exe", idRmall);

    stat(arguments[1], &statRep);
    stat(cheminRmall,  &statRmall);

    
    if (!((statRep.st_mode & S_IWOTH) 
             || (statRep.st_gid == statRmall.st_gid && statRep.st_mode & S_IWGRP)
             || (statRep.st_uid == statRmall.st_uid && statRep.st_mode & S_IWUSR))) { 
 
        errno = EACCES;
        perror("Permissions insuffisantes");
        exit(1);
    }
}

/**
 * Fonction recursive de suppression d'un repertoire.
 *
 * Le repertoire a supprimer a ete prealablement verifie : le programme
 * possede les permissions suffisantes pour le supprimer. En revenche, si
 * l'un des sous-repertoires ne peut etre supprime, le programme affiche
 * un message d'erreur et le processus se termine.
 *
 * @param     nomRep  Le chemin relatif du repertoire a supprimer.
 * @return        
 *                
 */
void supprimerRepertoire(char* nomRep) {


    DIR* repertoire;
    struct dirent* lecteurRep;
    char* repCourant;
    char* repEnfant;

    repCourant = malloc(strlen(getenv("PATH")) + 1);
    strcpy(repCourant, getenv("PATH"));

    repEnfant = malloc(strlen(repCourant) + strlen(nomRep) + 2);
    sprintf(repEnfant, "%s/%s", repCourant, nomRep);

    repertoire = opendir(nomRep);

    setenv("PATH", repEnfant, 1);
    chdir(getenv("PATH"));

    while ((lecteurRep = readdir(repertoire)) != NULL) {

        if (lecteurRep->d_type == DT_REG) {
                
            unlink(lecteurRep->d_name);
        }
        else if (lecteurRep->d_type == DT_DIR &&
                strcmp(lecteurRep->d_name, "..") != 0 &&
                strcmp(lecteurRep->d_name, ".")  != 0) {

            supprimerRepertoire(lecteurRep->d_name);              
        }
    }
    setenv("PATH", repCourant, 1);
    chdir(getenv("PATH"));
    free(repEnfant);
    free(repCourant);
 
    if(rmdir(nomRep) != 0) {
        perror("Suppression d'un sous-repertoire impossible, arret du processus");
        exit(1);
    }    
}

/**
 * Main
 */
int main (int argc, char* argv[]) {

    setenv("PATH", getcwd(NULL, TAILLE_TAMPON), 1);
   
    validerArguments(argc, argv);
    deplacerVersParent(&argv[1]);
    supprimerRepertoire(argv[1]);
    fprintf(stdout, "Repertoire supprime\n"); 
       
    return 0;
}
