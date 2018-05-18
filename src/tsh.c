/**
 * UQAM - Hiver 2017 - INF3172 - Groupe 10 - TP2
 *
 * tsh.c - Fichier source d'un shell rudimentaire. 
 *
 * Le shell supporte directement trois commandes : 
 *
 * 1 - cd : pour changer le repertoire de travail courant.
 * 2 - cdir : pour afficher le chemin absolu du repertoire de travail courant..
 * 3 - exit : pour mettre fin a l'execution du shell.
 *
 * Le shell peut egalement lancer d'autres programmes, avec ou sans argument(s).
 * Les executables de ces programmes doivent se situer dans le repertoire
 * contenant l'executable "tsh".  
 *
 * Le shell lui-meme ne procede pas a la validation des arguments passes aux
 * programmes externes : Ce sont ces programmes qui doivent proceder aux 
 * validations. 
 *
 * @Auteur      Alexis Chretien (CHRA25049209)
 * @Version     6 avril 2017
 */

#include "chemin.h"
#include <dirent.h>
#include <stdbool.h>
#include <sys/wait.h>

/**
 * Constantes
 */
#define ERR_NB_ARG        1
#define ERR_REP           2
#define ERR_EXEC          3
#define NB_ARG_MAX        20
#define TAILLE_TAMPON    256

const char* DELIMS_ARGS = " \n";

/**
 * Fonction qui affiche un message d'erreur sur stderr.
 *
 * @param   noErreur      Le code de l'erreur a afficher.
 * @return
 */
void afficherMessageErreur(int noErreur) {

    switch (noErreur) {

        case 1  : fprintf(stderr, "Format invalide\n");
                  break;
        case 2  : perror("Repertoire introuvable");
                  break;
        case 3  : perror("Programme introuvable");

        default : break;
    }
}
/**
 * Fonction qui retourne le chemin absolu du repertoire contenant l'executable
 * "tsh", peu importe s'il a ete lance dans un autre repertoire (exemple :
 * en lancant le programme via la commande "./inf3172/bin/tsh") ou s'il a ete
 * lance via son chemin absolu (exemple : "/home/USER/inf3172/../inf3172/./bin/tsh").
 *
 * C'est dans ce repertoire que l'existence des executables des autres commandes
 * sera verifiee. 
 *
 * @param   arg0   Le premier argument du programme
 * @return         Le chemin absolu du repertoire contenant l'executable "tsh". 
 */
char* retournerCheminRepExec(char* arg0) {
    
    char* rep;
    char* temp; 
    char* chemin;

    // si argv[0] est un chemin relatif
    if (arg0[0] != '/') {
        chemin = malloc(strlen(getenv("PATH")) + strlen(arg0) + 2); 
        strcpy(chemin, getenv("PATH"));
    }
    // si argv[0] est une chemin absolu
    else {
        chemin = malloc(strlen(arg0) + 1);   
        strcpy(chemin, "/"); 
    }
    rep = strtok(arg0, DELIMS); 
    temp = rep;
    rep = strtok(NULL, DELIMS);

    while (rep != NULL) {
        strcat(chemin, "/");
        strcat(chemin, temp);

         temp = rep;
         rep = strtok(NULL, DELIMS);
    }
    return chemin;
}
/**
 * Fonction qui traite une ligne de commande saisie par l'utilisateur. 
 *
 * La fonction extrait le nom de la commande et les arguments de la 
 * chaine de caracteres produit par l'utilisateur, et les insere dans
 * un tableau de chaines de caracteres. La case du tableau suivant
 * le dernier argument doit contenir NULL.
 *
 * Si la commande saisie par l'utilisateur contient plus de NB_ARG_MAX
 * mots, la fonction conserve uniquement les NB_ARG_MAX premiers mots.
 *
 * @param  ligneCommande    La chaine de caracteres saisie par l'utilisateur.
 * @param  arguments        Le tableau de chaines de caracteres dans lequel le
 *                          nom de la commande et les arguments seront conserves.
 *                          La premiere case du tableau contient le nom de 
 *                          l'executable.
 * @return                  Le nombre d'arguments (incluant le nom de la
 *                          commande) saisis par l'utilisateur.
 */
int traiterLigneCommande(char ligneCommande[TAILLE_TAMPON], 
                         char* arguments[NB_ARG_MAX + 1]){ 

    int i;
    int nbArguments;
    char* mot;
    nbArguments = 0;
   
    mot = strtok(ligneCommande, DELIMS_ARGS);

    for (i = 0 ; mot != NULL && i < NB_ARG_MAX ; ++i) {
        arguments[i] = malloc(strlen(mot) + 1);     
        strcpy(arguments[i], mot);
        ++nbArguments;

        mot = strtok(NULL, DELIMS_ARGS);
    }
    arguments[i] = NULL; 
    
    return nbArguments;
}
/**
 * Fonction qui valide les arguments entres par l'utilisateur en fonction
 * des contraintes des commandes supportees directement par le shell. 
 *
 * @param  arguments        Les arguments entrees par l'utilisateur. Le
 *                          premier argument est le nom de la commande. 
 * @param  nbArguments      Le nombre d'arguments entres par l'utilisateur, 
 *                          incluant le nom de la commande.
 * @return                  "true" si la commande entree par l'utilisateur
 *                          est une commande supportee directement par le
 *                          shell et que les arguments respectent les 
 *                          contraintes de cette commande.
 *                          "true" si la commande entree n'est pas l'une 
 *                          des commandes directement supportees par le shell
 *                          "false" sinon. 
 */
bool validerLigneCommande(char** arguments, int nbArguments) {

    if (nbArguments == 0) {   

       return false;
    } 
    else if ((strcmp(arguments[0], "cd") == 0 && nbArguments != 2) 
                || (strcmp(arguments[0], "cdir") == 0 && nbArguments != 1)
                || (strcmp(arguments[0], "exit") == 0 && nbArguments != 1)) {

            afficherMessageErreur(ERR_NB_ARG);
            return false;
    }
    return true;
}

/**
 * Fonction qui execute la commande "cd" en changeant le repertoire de travail 
 * courant du shell pour le repertoire cible.
 *
 * Le repertoire cible peut etre sous forme de chemin absolu ou de chemin 
 * relatif par rapport au repertoire de travail courant.
 *
 * Fait appel a une fonction definie dans "chemin.h" (retournerCheminAbsolu(...))
 *
 * @param   cible   Le chemin (absolu ou relatif) du prochain repertoire
 *                  de travail.
 * @return
 */
void cdExecuter(char* cible) {
   
    DIR* repertoire;
    char* cheminCibleAbsolu;
    
    cheminCibleAbsolu = retournerCheminAbsolu(getenv("PATH"), cible);

    repertoire = opendir(cheminCibleAbsolu);

    // si le repertoire cible n'existe pas
    if (repertoire == NULL) {
        afficherMessageErreur(ERR_REP);
    }
    // si le repertoire cible existe
    else { 
        closedir(repertoire);
        setenv("PATH", cheminCibleAbsolu, 1);
        chdir(getenv("PATH"));
    }
    free(cheminCibleAbsolu);
}

/**
 * Fonction qui affiche sur stdout le chemin de travail courant. 
 *
 * @return
 */
void cdirExecuter() {
  fprintf(stdout, "repertoire courant : %s\n", getenv("PATH"));
}

/**
 * Fonction qui libere la memoire allouee lors de la saisie d'une commande
 * par l'utilisateur.
 *
 * @param   arguments       Les chaines de caracteres allouees dynamiquement.
 * @param   nbArguments     Le nombre de chaines de caracteres.
 * @return
 */
void libererMemoire(char* arguments[NB_ARG_MAX], int nbArguments) {

    int i;

    for (i = 0 ; i < nbArguments ; ++i) {
        free(arguments[i]);
    }
}

/**
 * Main
 */ 
int main (int argc, char** argv) {

    int nbArguments;
    bool termine;
    char ligneCommande[TAILLE_TAMPON];
    char* arguments[NB_ARG_MAX];
    char* cheminRepExec;
    char* cheminExec;
    pid_t idEnfant;

    setenv("PATH", getcwd(NULL, TAILLE_TAMPON), 1);

    cheminRepExec = retournerCheminRepExec(argv[0]); 
  
    idEnfant = 0;
    termine = false;

    // Tant que la commande exit (sans arguments) n'a pas ete entre
    while(termine == false) {
        fprintf(stdout, "tsh> ");
        fgets(ligneCommande, TAILLE_TAMPON, stdin);
        nbArguments = traiterLigneCommande(ligneCommande, arguments); 
  
        if (validerLigneCommande(arguments, nbArguments)) {     
            if (strcmp(arguments[0], "cd") == 0) {
                cdExecuter(arguments[1]);
            }
            else if (strcmp(arguments[0], "cdir") == 0) {
                cdirExecuter();
            }
            else if (strcmp(arguments[0], "exit") == 0) {
                termine = true;
            }
            else {
                idEnfant = fork();

                cheminExec = malloc(strlen(cheminRepExec) + strlen(arguments[0]) + 2);
                sprintf(cheminExec, "%s/%s", cheminRepExec, arguments[0]);
                
                // processus enfant
                if (idEnfant == 0) {
                   // message d'erreur si le processus enfant n'a pas ete lance.
                   if (execvp(cheminExec, arguments) == -1) {
                        afficherMessageErreur(ERR_EXEC);    
                   }         
                   exit(1);       
                }
                free(cheminExec);
            }
            if (idEnfant != 0) {
                wait(&idEnfant);
                idEnfant = 0;
            }
        }
        libererMemoire(arguments, nbArguments);
    }
    exit(0);
}



