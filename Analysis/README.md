#### CAS OU TOUTES LES COUPURES ONT ETE PREALABLEMENT DETERMINEES ####

-------------------------------------------------------------------------------

## ETAPE 1 : REPERTOIRE RunRef ##


- Remplir le fichier RunRefs.dat

    COLONNE 1: NUMERO DU RUN

    COLONNE 2: NOMBRE DE FICHIER

    COLONNE 3: type de données à choisir entre U, Cf, BdF, Cs

    COLONNE 4: numéro du dernier run où les cartes faster ont été redémarrées

- Executer le script readRunRefs.py qui va créer le fichier RunRefs.h


-------------------------------------------------------------------------------

## ETAPE 2  REPERTOIRE 05_Raw2Ana ##

Une fois toutes les coupure faites et que l'expérience est en production :

root [0] .L Raw2AnaPFN.C+

root [1] run(arg1, arg2, arg3, arg4)

avec arg1: numéro du premier run à traiter

     arg2: numéro du dernier run à traiter

     arg3: type de données: "U", "Cf", "BdF", "Cs"

     arg4: path to the root file, for example "../../data/Raw/U235"

Attention ne pas mélanger les runs ayant des T0, coupures différents

Ce code ne permet de traiter les cas BdF et Cs