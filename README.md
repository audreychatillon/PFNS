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

* visualiser les voies des donn�es brutes
--> Analysis/01_ViewRawData

* faire le fit de tous les pics gamma des PFN (A FAIRE APRES CHAQUE T0)
--> 02_FitGammaPeak

* transformer les arbres bruts en arbres calibr�s en ToF
--> 03_Raw2Cal UNDER CONSTRUCTION  

* tracer, � partir des donn�es calibr�es en ToF, les 2D pour faire les TCutG n�cessaires pour la discri neutron/gamma 
--> 04_CreateTCutG ou macros

----------------------------------------------------------------

* convertir les arbres bruts en arbre pr�ts � tracer les �nergies des PFNs, une fois toutes les conditions d�termin�es 
--> 05_Raw2Ana
