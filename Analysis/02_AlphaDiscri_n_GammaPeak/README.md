-------------------------------------------------------------------------------

#### REPERTOIRE 02_AlphaDiscri_n_FitGammaPeak ####

-------------------------------------------------------------------------------

## Pour la discri alpha / fission dans la chambre : ##

- Pour la suite de l'étude, avant de tracer n'importe quelle observable, il faut s'assurer que la donnée chambre à fission ne correspond pas à une particule alpha. 

- Pour cela on teste si (Q1,Q2) est à l'intérieur d'une TCutG. 

- Ce TCutG doit être modifié pour chaque nouveau réglage de la chambre. 

Pour cela :

root [0] .L CreateAlphaDiscri.C+

root [1] run(102,102,"U","../../data/Raw/U235") : à adapter selon les runs utilisés et le chemin vers les données.

- Ce code va enregistrer les TCutG dans le répertoire 02_AlphaDiscri_n_FitGammaPeak/cut/cutFC_Q2vsQ1_RXXX.root avec XXX le numéro de run


-------------------------------------------------------------------------------

## Pour les neutrons de fission prompts : ##

- ATTENTION : A CHAQUE FOIS QU'UNE CARTE EST REDEMARREE, LE SPECTRE EN TEMPS VA SE DECALER ET IL FAUT REFAIRE LE FIT DES PICS GAMMAS QUE CE SOIT POUR LES NEUTRONS DE FISSION PROMPTS OU POUR LE FAISCEAU.

- ATTENTION : éditer le fichier et bien prendre le bon fichier cutFC_Q2vsQ1_RXXX.root dont le nom est codé en dur

- Ensuite :

root [0] .L FitGammaPeakPFN.C+

root [1] run(102,102,"U","../../data/Raw/U235") : à adapter selon les runs utilisés et le chemin vers les données.

- Ce code va créer quatre fichiers qui seront inclus ensuite pour calibrer le ToF et calculer l'énergie des PFN

../../SetupSpecific/FC_to_CHINUlg_GammaPeak.h

../../SetupSpecific/FC_to_CHINUhg_GammaPeak.h

../../SetupSpecific/FC_to_B3lg_GammaPeak.h

../../SetupSpecific/FC_to_B3hg_GammaPeak.h

Modifier le code à votre convenance pour adapter les chemins.


-------------------------------------------------------------------------------

## Pour les neutrons du faisceau ##

- ATTENTION : A FAIRE A CHAQUE NOUVEAU T0

root [0] .L FitGammaFlash.C+

root [1] run(102,102,"U","../../data/Raw/U235") : à adapter selon les runs utilisés et le chemin vers les données.

- Ce code va tracer les spectres ToF bruts (un par voie de chambre à fission)

- C'est à vous de faire les fits gaussiens du flash gamma et de copier le résultat dans le tableau static float GammaFlash_Mean[11] qui est à l'intérieur du fichier SetupSpecific/setup_specific.h

- puis créer l'offset à rajouter pour obtenir les offsets à ajouter au temps de vol brut pour avoir le temps de vol calibré

root [0] .L CalibrateToFBeam.C+

root [1] run()

- Ce code va créer le fichier ../../SetupSpecific/BEAM_OffsetToF.h qui pourra être directement inclus pour transformer le temps brut en énergie cinétique

