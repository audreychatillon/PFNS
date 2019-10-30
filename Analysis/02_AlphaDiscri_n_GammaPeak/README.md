-------------------------------------------------------------------------------

#### REPERTOIRE 02_AlphaDiscri_n_FitGammaPeak ####

-------------------------------------------------------------------------------

## Pour la discri alpha / fission dans la chambre : ##

- Pour la suite de l'�tude, avant de tracer n'importe quelle observable, il faut s'assurer que la donn�e chambre � fission ne correspond pas � une particule alpha. 

- Pour cela on teste si (Q1,Q2) est � l'int�rieur d'une TCutG. 

- Ce TCutG doit �tre modifi� pour chaque nouveau r�glage de la chambre. 

Pour cela :

root [0] .L CreateAlphaDiscri.C+

root [1] run(102,102,"U","../../data/Raw/U235") : � adapter selon les runs utilis�s et le chemin vers les donn�es.

- Ce code va enregistrer les TCutG dans le r�pertoire 02_AlphaDiscri_n_FitGammaPeak/cut/cutFC_Q2vsQ1_RXXX.root avec XXX le num�ro de run


-------------------------------------------------------------------------------

## Pour les neutrons de fission prompts : ##

- ATTENTION : A CHAQUE FOIS QU'UNE CARTE EST REDEMARREE, LE SPECTRE EN TEMPS VA SE DECALER ET IL FAUT REFAIRE LE FIT DES PICS GAMMAS QUE CE SOIT POUR LES NEUTRONS DE FISSION PROMPTS OU POUR LE FAISCEAU.

- ATTENTION : �diter le fichier et bien prendre le bon fichier cutFC_Q2vsQ1_RXXX.root dont le nom est cod� en dur

- Ensuite :

root [0] .L FitGammaPeakPFN.C+

root [1] run(102,102,"U","../../data/Raw/U235") : � adapter selon les runs utilis�s et le chemin vers les donn�es.

- Ce code va cr�er quatre fichiers qui seront inclus ensuite pour calibrer le ToF et calculer l'�nergie des PFN

../../SetupSpecific/FC_to_CHINUlg_GammaPeak.h

../../SetupSpecific/FC_to_CHINUhg_GammaPeak.h

../../SetupSpecific/FC_to_B3lg_GammaPeak.h

../../SetupSpecific/FC_to_B3hg_GammaPeak.h

Modifier le code � votre convenance pour adapter les chemins.


-------------------------------------------------------------------------------

## Pour les neutrons du faisceau ##

- ATTENTION : A FAIRE A CHAQUE NOUVEAU T0

root [0] .L FitGammaFlash.C+

root [1] run(102,102,"U","../../data/Raw/U235") : � adapter selon les runs utilis�s et le chemin vers les donn�es.

- Ce code va tracer les spectres ToF bruts (un par voie de chambre � fission)

- C'est � vous de faire les fits gaussiens du flash gamma et de copier le r�sultat dans le tableau static float GammaFlash_Mean[11] qui est � l'int�rieur du fichier SetupSpecific/setup_specific.h

- puis cr�er l'offset � rajouter pour obtenir les offsets � ajouter au temps de vol brut pour avoir le temps de vol calibr�

root [0] .L CalibrateToFBeam.C+

root [1] run()

- Ce code va cr�er le fichier ../../SetupSpecific/BEAM_OffsetToF.h qui pourra �tre directement inclus pour transformer le temps brut en �nergie cin�tique

