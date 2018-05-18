# tsh

## Description 

Shell rudimentaire *tsh*. 

Le shell supporte directement trois commandes : 

* `cd` : pour changer le repertoire de travail courant.
* `cdir` : pour afficher le chemin absolu du repertoire de travail courant.
* `exit` : pour mettre fin a l'exécution du shell.
 
## Compilation et exécution

```
$ make
$ ./bin/tsh
```
Le shell peut également lancer d'autres programmes, avec ou sans argument(s).
Les executables de ces programmes doivent se situer dans le repertoire
contenant l'executable *tsh*. Le projet inclut 6 programmes pouvant être
utilisés par ce shell : *fin*,  *list*, *new*,  *newdir*,  *rmall* et *size*.

 
# fin
## Description
Permet d'afficher les `NB` dernières lignes d'un fichier. 
## Utilisation
```
tsh> fin [NB] [NOM_FICHIER]
```
# list
## Description
Permet d'afficher le contenu du répertoire courrant.
## Utilisation
```
tsh> list
```
# new
## Description 
Permet de créer un nouveau fichier.

## Utilisation
```
tsh> new [NOM_FICHIER]
```
# newdir
## Description
 Permet de créer un nouveau répertoire.
## Utilisation
```
tsh> newdir [NOM_REPERTOIRE]
```
# rmall
## Description
Permet de procéder à la suppression récursive d'un répertoire.
## Utilisation
```
tsh> rmall [NOM_REPERTOIRE]
```
# size 
## Description
Affiche le nombre et la somme totale des tailles des fichiers contenus dans 
un répertoire. 
## Utilisation
```
tsh> size [NOM_REPERTOIRE]
```
