#### CAS OU TOUTES LES COUPURES ONT ETE PREALABLEMENT DETERMINEES ####

-------------------------------------------------------------------------------

## ETAPE 1 : REPERTOIRE RunRef ##


- Remplir le fichier RunRefs.dat

    COLONNE 1: NUMERO DU RUN

    COLONNE 2: NOMBRE DE FICHIER

    COLONNE 3: type de donn�es � choisir entre U, Cf, BdF, Cs

    COLONNE 4: num�ro du dernier run o� les cartes faster ont �t� red�marr�es

- Executer le script readRunRefs.py qui va cr�er le fichier RunRefs.h


-------------------------------------------------------------------------------

## ETAPE 2  REPERTOIRE 05_Raw2Ana ##

Une fois toutes les coupure faites et que l'exp�rience est en production :

root [0] .L Raw2AnaPFN.C+

root [1] run(arg1, arg2, arg3, arg4)

avec arg1: num�ro du premier run � traiter

     arg2: num�ro du dernier run � traiter

     arg3: type de donn�es: "U", "Cf", "BdF", "Cs"

     arg4: path to the root file, for example "../../data/Raw/U235"

Attention ne pas m�langer les runs ayant des T0, coupures diff�rents

Ce code ne permet de traiter les cas BdF et Cs