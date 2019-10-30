-------------------------------------------------------------------------------
** REPERTOIRE 01_ViewRawData **
-------------------------------------------------------------------------------

Ce code peut être utilisé pour regarder l'ensemble des données brutes produites par le code fast2root dans le cas des données produites en coinc (soit avec la chambre à fission Cf soit avec celle U)

-------------------------------------------------------------------------------

- Le code ViewRawCoincData.C a plusieurs include: vérifier les chemins

- Commenter les lignes correspondant aux histos que vous ne voulez pas voir tracer

- vérifier les chemins qui sont codés en durs (enregistrement des canvas dans un fichier root)

-------------------------------------------------------------------------------

root [0] .L ViewRawCoincData.C+ ou .L ViewRawSourceData.C+

root [1] run(arg1,arg2,arg3,arg4)

avec arg1: numéro du premier run à traiter

     arg2: numéro du dernier run à traiter

     arg3: type de données: "U", "Cf", "BdF", "Cs"

     arg4: path to the root file, for example "../../data/Raw/U235"