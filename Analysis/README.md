-------------------------------------------------------------------------------
*** REPERTOIRE RunRef 


* Remplir le fichier RunRefs.dat

    COLONNE 1: NUMERO DU RUN

    COLONNE 2: NOMBRE DE FICHIER

    COLONNE 3: type de donn�es � choisir entre U, Cf, BdF, Cs

    COLONNE 4: num�ro du dernier run o� les cartes faster ont �t� red�marr�es

* Executer le script readRunRefs.py qui va cr�er le fichier RunRefs.h

-------------------------------------------------------------------------------
*** REPERTOIRE 01_ViewRawData 


* Les codes ViewRawCoincData.C et ViewRawSourceData.C ont plusieurs include: v�rifier les chemins

* Commenter les lignes correspondant aux histos qui ne doivent pas �tre trac�s

root [0] .L ViewRawCoincData.C+ ou .L ViewRawSourceData.C+

root [1] run(arg1,arg2,arg3,arg4)

avec arg1: num�ro du premier run � traiter

     arg2: num�ro du dernier run � traiter

     arg3: type de donn�es: "U", "Cf", "BdF", "Cs"

     arg4: path to the root file, for example "../../data/Raw/U235"

-------------------------------------------------------------------------------
*** REPERTOIRE 02_AlphaDiscri_n_FitGammaPeak 

** Pour la discri alpha / fission dans la chambre :

* Pour la suite de l'�tude, avant de tracer n'importe quelle observable, il faut s'assurer que la donn�e chambre � fission ne correspond pas � une particule alpha. 

* Pour cela on teste si (Q1,Q2) est � l'int�rieur d'une TCutG. 

* Ce TCutG doit �tre modifi� pour chaque nouveau r�glage de la chambre. 

Pour cela :

root [0] .L CreateAlphaDiscri.C+

root [1] run(102,102,"U","../../data/Raw/U235") : � adapter selon les runs utilis�s et le chemin vers les donn�es.

*Ce code va enregistrer les TCutG dans le r�pertoire 02_AlphaDiscri_n_FitGammaPeak/cut/cutFC_Q2vsQ1_RXXX.root avec XXX le num�ro de run

** Pour les neutrons de fission prompts:

* ATTENTION : A CHAQUE FOIS QU'UNE CARTE EST REDEMARREE, LE SPECTRE EN TEMPS VA SE DECALER ET IL FAUT REFAIRE LE FIT DES PICS GAMMAS QUE CE SOIT POUR LES NEUTRONS DE FISSION PROMPTS OU POUR LE FAISCEAU.

* ATTENTION : �diter le fichier et bien prendre le bon fichier cutFC_Q2vsQ1_RXXX.root

* Ensuite :

root [0] .L FitGammaPeakPFN.C+

root [1] run(102,102,"U","../../data/Raw/U235") : � adapter selon les runs utilis�s et le chemin vers les donn�es.

* Ce code va cr�er quatre fichiers qui seront inclus ensuite pour calibrer le ToF et calculer l'�nergie des PFN

../../SetupSpecific/FC_to_CHINUlg_GammaPeak.h

../../SetupSpecific/FC_to_CHINUhg_GammaPeak.h

../../SetupSpecific/FC_to_B3lg_GammaPeak.h

../../SetupSpecific/FC_to_B3hg_GammaPeak.h

Modifier le code � votre convenance pour adapter les chemins.


** Pour les neutrons du faisceau

(en cours)

root [0] .L FitGammaFlash.C+

root [1] run(102,102,"U","../../data/Raw/U235") : � adapter selon les runs utilis�s et le chemin vers les donn�es.

* Ce code va cr�er quatre fichiers qui seront inclus ensuite pour calibrer le ToF et calculer l'�nergie des PFN

../../SetupSpecific/CHINUlg_OffsetToF.h.h

../../SetupSpecific/CHINUhg_OffsetToF.h.h

../../SetupSpecific/B3lg_OffsetToF.h.h

../../SetupSpecific/B3hg_OffsetToF.h.h

Modifier le code � votre convenance pour adapter les chemins.

-------------------------------------------------------------------------------
*** REPERTOIRE 03_Raw2Cal 


** Dans cette �tape, cr�ation d'un arbre root avec tous les temps de vol et �nergie pour les neutrons incidents et prompts. 

** On garde �galement les valeurs de certaines variables brutes dont les charges.

** ATTENTION : J'ai volontairement enlev� tous les �v�venements o� aucun d�tecteur neutron ne r�pond. Je garde cependant trace du num�ro d'�v�nement originel.\ 

-------------------------------------------------------------------------------
*** 04_CreateTCutG

** Cr�er les histos 2D (Qratio vs Q1) des d�tecteurs neutrons conditionn�s sur les gammas

�diter  GenerateQdiscriHistos.C et donner le nom de l'arbre root provenant de 03_Raw2Cal

root[0] .L GenerateQdiscriHistos.C+

root[1] run()


** faire les TCutG

�diter DrawCut.C et commenter (Q1vsE) et d�commenter (Q2/Q1vsQ1) les lignes 

root[0].L DrawCut.C+

root[1] run()


** Cr�er les histos 2D (Q1 vs Ene) des d�tecteurs neutrons conditionn�s sur les gammas

�diter  GenerateQdiscriHistos.C et donner le nom de l'arbre root provenant de 03_Raw2Cal

root[0] .L GenerateQdiscriHistos.C+

root[1] run()


** faire les TCutG

�diter DrawCut.C et d�commenter (Q1vsE) et commenter (Q2/Q1vsQ1) 

root[0].L DrawCut.C+

root[1] run()


-------------------------------------------------------------------------------
*** 05