-------------------------------------------------------------------------------

#### 04_CreateTCutG ####

-------------------------------------------------------------------------------

* Cr�er les histos 2D (Qratio vs Q1) des d�tecteurs neutrons conditionn�s sur les gammas 

�diter  GenerateQdiscriHistos.C et donner le nom de l'arbre root provenant de 03_Raw2Cal

root[0] .L GenerateQdiscriHistos.C+

root[1] run()

-------------------------------------------------------------------------------

* faire les TCutG

�diter DrawCut.C et commenter (Q1vsE) et d�commenter (Q2/Q1vsQ1) les lignes 

root[0].L DrawCut.C+

root[1] run()

-------------------------------------------------------------------------------

* Cr�er les histos 2D (Q1 vs Ene) des d�tecteurs neutrons conditionn�s sur les gammas

�diter  GenerateQdiscriHistos.C et donner le nom de l'arbre root provenant de 03_Raw2Cal

root[0] .L GenerateQdiscriHistos.C+

root[1] run()

-------------------------------------------------------------------------------

* faire les TCutG 

�diter DrawCut.C et d�commenter (Q1vsE) et commenter (Q2/Q1vsQ1) 

root[0].L DrawCut.C+

root[1] run()