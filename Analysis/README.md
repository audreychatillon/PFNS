-------------------------------------------------------------------------------
*** REPERTOIRE RunRef 


* Remplir le fichier RunRefs.dat

    COLONNE 1: NUMERO DU RUN

    COLONNE 2: NOMBRE DE FICHIER

    COLONNE 3: type de données à choisir entre U, Cf, BdF, Cs

    COLONNE 4: numéro du dernier run où les cartes faster ont été redémarrées

* Executer le script readRunRefs.py qui va créer le fichier RunRefs.h

-------------------------------------------------------------------------------
*** REPERTOIRE 01_ViewRawData 


* Les codes ViewRawCoincData.C et ViewRawSourceData.C ont plusieurs include: vérifier les chemins

* Commenter les lignes correspondant aux histos qui ne doivent pas être tracés

root [0] .L ViewRawCoincData.C+ ou .L ViewRawSourceData.C+

root [1] run(arg1,arg2,arg3,arg4)

avec arg1: numéro du premier run à traiter

     arg2: numéro du dernier run à traiter

     arg3: type de données: "U", "Cf", "BdF", "Cs"

     arg4: path to the root file, for example "../../data/Raw/U235"

-------------------------------------------------------------------------------
*** REPERTOIRE 02_AlphaDiscri_n_FitGammaPeak 

** Pour la discri alpha / fission dans la chambre :

* Pour la suite de l'étude, avant de tracer n'importe quelle observable, il faut s'assurer que la donnée chambre à fission ne correspond pas à une particule alpha. 

* Pour cela on teste si (Q1,Q2) est à l'intérieur d'une TCutG. 

* Ce TCutG doit être modifié pour chaque nouveau réglage de la chambre. 

Pour cela :

root [0] .L CreateAlphaDiscri.C+

root [1] run(102,102,"U","../../data/Raw/U235") : à adapter selon les runs utilisés et le chemin vers les données.

*Ce code va enregistrer les TCutG dans le répertoire 02_AlphaDiscri_n_FitGammaPeak/cut/cutFC_Q2vsQ1_RXXX.root avec XXX le numéro de run

** Pour les neutrons de fission prompts:

* ATTENTION : A CHAQUE FOIS QU'UNE CARTE EST REDEMARREE, LE SPECTRE EN TEMPS VA SE DECALER ET IL FAUT REFAIRE LE FIT DES PICS GAMMAS QUE CE SOIT POUR LES NEUTRONS DE FISSION PROMPTS OU POUR LE FAISCEAU.

* ATTENTION : éditer le fichier et bien prendre le bon fichier cutFC_Q2vsQ1_RXXX.root

* Ensuite :

root [0] .L FitGammaPeakPFN.C+

root [1] run(102,102,"U","../../data/Raw/U235") : à adapter selon les runs utilisés et le chemin vers les données.

* Ce code va créer quatre fichiers qui seront inclus ensuite pour calibrer le ToF et calculer l'énergie des PFN

../../SetupSpecific/FC_to_CHINUlg_GammaPeak.h

../../SetupSpecific/FC_to_CHINUhg_GammaPeak.h

../../SetupSpecific/FC_to_B3lg_GammaPeak.h

../../SetupSpecific/FC_to_B3hg_GammaPeak.h

Modifier le code à votre convenance pour adapter les chemins.


** Pour les neutrons du faisceau

(en cours)

root [0] .L FitGammaFlash.C+

root [1] run(102,102,"U","../../data/Raw/U235") : à adapter selon les runs utilisés et le chemin vers les données.

* Ce code va créer quatre fichiers qui seront inclus ensuite pour calibrer le ToF et calculer l'énergie des PFN

../../SetupSpecific/CHINUlg_OffsetToF.h.h

../../SetupSpecific/CHINUhg_OffsetToF.h.h

../../SetupSpecific/B3lg_OffsetToF.h.h

../../SetupSpecific/B3hg_OffsetToF.h.h

Modifier le code à votre convenance pour adapter les chemins.

-------------------------------------------------------------------------------
*** REPERTOIRE 03_Raw2Cal 


** Dans cette étape, création d'un arbre root avec tous les temps de vol et énergie pour les neutrons incidents et prompts. 

** On garde également les valeurs de certaines variables brutes dont les charges.

** ATTENTION : J'ai volontairement enlevé tous les évévenements où aucun détecteur neutron ne répond. Je garde cependant trace du numéro d'événement originel.\ 

-------------------------------------------------------------------------------
*** 04_CreateTCutG

** Créer les histos 2D (Qratio vs Q1) des détecteurs neutrons conditionnés sur les gammas

éditer  GenerateQdiscriHistos.C et donner le nom de l'arbre root provenant de 03_Raw2Cal

root[0] .L GenerateQdiscriHistos.C+

root[1] run()


** faire les TCutG

éditer DrawCut.C et commenter (Q1vsE) et décommenter (Q2/Q1vsQ1) les lignes 

root[0].L DrawCut.C+

root[1] run()


** Créer les histos 2D (Q1 vs Ene) des détecteurs neutrons conditionnés sur les gammas

éditer  GenerateQdiscriHistos.C et donner le nom de l'arbre root provenant de 03_Raw2Cal

root[0] .L GenerateQdiscriHistos.C+

root[1] run()


** faire les TCutG

éditer DrawCut.C et décommenter (Q1vsE) et commenter (Q2/Q1vsQ1) 

root[0].L DrawCut.C+

root[1] run()


-------------------------------------------------------------------------------
*** 05