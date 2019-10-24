=== ================= ===
=== REPERTOIRE RunRef ===
=== ================= ===

Remplir le fichier RunRefs.dat
    COLONNE 1: NUMERO DU RUN
    COLONNE 2: NOMBRE DE FICHIER
    COLONNE 3: type de données à choisir entre U, Cf, BdF, Cs
Execuer le script readRunRefs.py qui va créer le fichier RunRefs.h



=== ========================= ===
=== REPERTOIRE 01_ViewRawData ===
=== ========================= ===

* Les codes ViewRawCoincData.C et ViewRawSourceData.C ont plusieurs include: vérifier les chemins
* Commenter les lignes correspondant aux histos qui ne doivent pas être tracés
root [0] .L ViewRawCoincData.C+ ou .L ViewRawSourceData.C+
root [1] run(arg1,arg2,arg3,arg4)
avec arg1: numéro du premier run à traiter
     arg2: numéro du dernier run à traiter
     arg3: type de données: "U", "Cf", "BdF", "cs"
     arg4: path to the root file, for example "../data/Raw/U235"



=== ========================== ===
=== REPERTOIRE 02_FitGammaPeak ===
=== ========================== ===

root [0] .L FitGammaPeakPFN.C+
root [1] run(20,21,"Cf","../../data/Raw/Cf252/") : à adapter selon les runs utilisés et le chemin vers les données.
Ce code va créer quatre fichiers qui seront inclus ensuite pour calibrer le ToF et calculer l'énergie des PFN
../../SetupSpecific/FC_to_CHINUlg_GammaPeak.h
../../SetupSpecific/FC_to_CHINUhg_GammaPeak.h
../../SetupSpecific/FC_to_B3lg_GammaPeak.h
../../SetupSpecific/FC_to_B3hg_GammaPeak.h
Modifier le code à votre convenance pour adapter les chemins.


=== ===================== ===
=== REPERTOIRE 03_Raw2Cal ===
=== ========================== ===

Dans cette étape, création d'un arbre root avec tous les temps de vol pour les neutrons incidents et prompts. 
Je garde également les valeurs de certaines variables brutes dont les charges.

!!! !!!!!!!!! !!!
!!! ATTENTION !!!
!!! !!!!!!!!! !!!
!!! J'ai volontairement enlevé tous les évévenements où :
!!!     * la multiplicité de la chambre à fission est >1 
!!!     * aucun détecteur neutron ne répond 
!!! Je garde cependant trace du numéro d'événement originel.

