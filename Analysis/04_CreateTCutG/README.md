-------------------------------------------------------------------------------

#### 04_CreateTCutG ####

-------------------------------------------------------------------------------

* Créer les histos 2D (Qratio vs Q1) des détecteurs neutrons conditionnés sur les gammas 

éditer  GenerateQdiscriHistos.C et donner le nom de l'arbre root provenant de 03_Raw2Cal

root[0] .L GenerateQdiscriHistos.C+

root[1] run()

-------------------------------------------------------------------------------

* faire les TCutG

éditer DrawCut.C et commenter (Q1vsE) et décommenter (Q2/Q1vsQ1) les lignes 

root[0].L DrawCut.C+

root[1] run()

-------------------------------------------------------------------------------

* Créer les histos 2D (Q1 vs Ene) des détecteurs neutrons conditionnés sur les gammas

éditer  GenerateQdiscriHistos.C et donner le nom de l'arbre root provenant de 03_Raw2Cal

root[0] .L GenerateQdiscriHistos.C+

root[1] run()

-------------------------------------------------------------------------------

* faire les TCutG 

éditer DrawCut.C et décommenter (Q1vsE) et commenter (Q2/Q1vsQ1) 

root[0].L DrawCut.C+

root[1] run()