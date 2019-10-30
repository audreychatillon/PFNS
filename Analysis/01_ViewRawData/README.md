-------------------------------------------------------------------------------
** REPERTOIRE 01_ViewRawData **
-------------------------------------------------------------------------------

Ce code peut �tre utilis� pour regarder l'ensemble des donn�es brutes produites par le code fast2root dans le cas des donn�es produites en coinc (soit avec la chambre � fission Cf soit avec celle U)

-------------------------------------------------------------------------------

- Le code ViewRawCoincData.C a plusieurs include: v�rifier les chemins

- Commenter les lignes correspondant aux histos que vous ne voulez pas voir tracer

- v�rifier les chemins qui sont cod�s en durs (enregistrement des canvas dans un fichier root)

-------------------------------------------------------------------------------

root [0] .L ViewRawCoincData.C+ ou .L ViewRawSourceData.C+

root [1] run(arg1,arg2,arg3,arg4)

avec arg1: num�ro du premier run � traiter

     arg2: num�ro du dernier run � traiter

     arg3: type de donn�es: "U", "Cf", "BdF", "Cs"

     arg4: path to the root file, for example "../../data/Raw/U235"