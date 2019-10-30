#PFNS


* ATTENTION !!!!!

POUR UN PREMIER TELECHARGEMENT DU CODE SUR UNE NOUVELLE MACHINE
 
AVANT TOUTES CHOSES, IL FAUT COMPILER LES LIBRAIRIES DANS :

	cd Analysis/ClassDef

	root Process.C


Ce code permet de 

----------------------------------------------------------------

* convertir les fichiers .fast en fichier .root\ 
--> Faster2Root

----------------------------------------------------------------

* visualiser les voies des données brutes

--> Analysis/01_ViewRawData

* faire les TCutG de la discri Alpha/Fission (à faire après les changements de seuil des voies chambres à fission), faire le fit de tous les pics gamma des PFN et des flash gamma (A FAIRE APRES CHAQUE T0)

--> 02_AlphaDiscri_n_FitGammaPeak

* transformer les arbres bruts en arbres calibrés en ToF

--> 03_Raw2Cal UNDER CONSTRUCTION  

* tracer, à partir des données calibrées en ToF, les 2D pour faire les TCutG nécessaires pour la discri neutron/gamma 

--> 04_CreateTCutG ou macros

----------------------------------------------------------------

* convertir les arbres bruts en arbre prêts à tracer les énergies des PFNs (une fois toutes les conditions déterminées) et du faisceau de neutrons

--> 05_Raw2Ana
