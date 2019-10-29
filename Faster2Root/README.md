1/ copier le fichier NAME.pid correspondant au fichier .fast dans  ReadPid


2/ Aller dans le r�pertoire ReadPid
 
           cd ReadPid


3/ Executer le script qui va cr�er pid.h:

           ./readPid.py NAME.pid

   REMARQUE : Si vous ne mettez pas d'argument par d�faut il produira le pid.h bas� sur sampleFake.pid


4/ Aller dans le r�pertoire SetupSpecific

           cd ../../SetupSpecific

   et �diter setup_specific.h afin de changer si besoin les valeurs suivantes :

#define TRIGGER_MODE 1   : 0=TRIGGER LESS MODE  1=COINCIDENCE MODE

#define LABEL_GROUP 3000 : valeur du label pour le groupe 

#define FC_nQ     i      : i=1, 2, 3 ou 4 en fonction du type qdc_t_xi  

#define NDET_nQ   j      : j=1, 2, 3 ou 4 en fonction du type qdc_t_xj  

#define B3NDET_nQ k      : k=1, 2, 3 ou 4 en fonction du type qdc_t_xk  

#define PULSER_nQ l      : l=1, 2, 3 ou 4 en fonction du type qdc_t_xl  

#define HF_nQ     m      : m=1, 2, 3 ou 4 en fonction du type qdc_t_xm  

#define MACRO_nQ  n      : n=1, 2, 3 ou 4 en fonction du type qdc_t_xn  

Pour savoir le type de donn�es : 

faster_disfast -I file.fast

faster_disfast -l3000 file.fast


5/ retourner dans le r�pertoire Faster2Root et compiler :

	   cd ..

           make clean

           make


6/ Ex�cuter : il faut indiquer 4 arguments

           ./fast2root PATH/TO/THE/FAST/FILE/FAST_NAME.fast PATH/TO/THE/RAW/ROOT/FILE/ROOT_NAME.root RUN_NUMBER FILE_NUMBER   
     
   REMARQUE : ce programme va cr�er un arbre root dont le nom sera RawCoincData dans le cas ou le trigger mode est 1 our RawSourceData dans le cas ou le trigger mode est 0 