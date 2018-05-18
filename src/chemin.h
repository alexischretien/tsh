/**
 * UQAM - Hiver 2017 - INF3172 - Groupe 10 - TP2
 *
 * chemin.h - Fichier "header" contenant des fonctions communes a plusieurs
 *            programmes et effectuant des traitements sur les chemins.
 *
 * @Auteur    Alexis Chretien (CHRA25049209)
 * @Version   7 avril 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Constantes
 */
const char* DELIMS = "/\n";

/**
 * Fonction qui retourne le chemin absolu d'un fichier ou repertoire cible.
 *
 * @param   cheminLocal    Le chemin absolu du repertoire local.
 * @param   cible          Le chemin (absolu ou relatif par rapport
 *                         a "cheminLocal") de la cible.
 * @return                 Le chemin absolu de la cible.
 */
char* retournerCheminAbsolu(char* cheminLocal, char* cible) {

    int i;
    char* nomRep;
    char* cheminCibleAbsolu;

    // Si la cible est deja un chemin absolu
    if (cible[0] == '/') {
        cheminCibleAbsolu = malloc(strlen(cible) + 1);
        strcpy(cheminCibleAbsolu, "/");
    }
    // si la cible est un chemin relatif
    else {
        cheminCibleAbsolu = malloc(strlen(cheminLocal) + strlen(cible) + 2);
        strcpy(cheminCibleAbsolu, cheminLocal);
    }
    nomRep = strtok(cible, DELIMS);

    while (nomRep != NULL) {
        if (strcmp(nomRep, "..") == 0) {
            for (i = strlen(cheminCibleAbsolu) - 1 ; 
                    i > 0 && cheminCibleAbsolu[i] != '/' ; --i) {
                cheminCibleAbsolu[i] = '\0';
            }
            // ne pas aller effacer le caractere "/" representant le
            // repertoire racine
            if (strcmp(cheminCibleAbsolu, "/") != 0) {
                cheminCibleAbsolu[i] = '\0';
            }
        } 
        else if (strcmp(nomRep, ".") != 0) {
            
            if (strcmp(cheminCibleAbsolu, "/") != 0) {
                strcat(cheminCibleAbsolu, "/");
            } 
            strcat(cheminCibleAbsolu, nomRep);
        }
        nomRep = strtok(NULL, DELIMS);
    }
    return cheminCibleAbsolu;
}

/**
 * Fonction qui modifie le repertoire de travail courant pour le repertoire
 * parent au repertoire en argument et modifie le repertoire en argument 
 * par son chemin relatif au nouveau repertoire de travail, seulement si
 * le repertoire en argument est un chemin absolu. 
 *
 * A noter que la fonction est sans effet sur un chemin absolu. La fonction 
 * est seulement utilisee dans "size.c" et "rmall.c", avant le premier
 * appel de la fonction recursive : Les implmentations de ces fonctions sont telles
 * que le fait de travailler avec des chemins relatifs de differentes profondeurs 
 * ne cause pas de probleme. Les chemins absolus, en revenche, en causent, ce
 * que cette fonction rectifie. 
 *
 * Une verification prealable de l'existence du repertoire en argument est 
 * necessaire pour garantir le bon comportement de la fonction.  
 *
 * @param   nomRep      Le nom du repertoire (chemin relatif ou absolu). 
 * @return
 */
void deplacerVersParent(char** nomRep) {
   
    int i;
    char* repertoireParent;
 
    // si le repertoire existe et qu'il s'agit d'un chemin absolue,
    // se deplacer vers le repertoire parent
    if (*nomRep[0] == '/') {
        repertoireParent = malloc(strlen(*nomRep) + 1);
        strcpy(repertoireParent, *nomRep);
                                 
        i = strlen(repertoireParent) - 1;
        for ( ; i >  0 && repertoireParent[i] != '/' ; --i) {
            repertoireParent[i] = '\0';
        }
        repertoireParent[i] = '\0';

        // mise a jour repertoire courant - se deplacer vers le repertoire
        // parent au repertoire a afficher
        setenv("PATH", repertoireParent, 1);
        chdir(getenv("PATH"));
                                                                             
        // changer le chemin du repertoire : absolue => relatif
        (*nomRep) += strlen(repertoireParent) + 1;
        free(repertoireParent);
    }
}
