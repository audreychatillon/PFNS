#include "../SetupSpecific/setup_specific.h"

//=============================================================== //
// NUMBER OF CHANNELS AND NAME OF CHANNELS                        //
//=============================================================== //
// URANIUM FISSION CHAMBER                                        //
//    * 11 CHANNELS                                               //
//            FC-1..11                                            //
// CALIFORNIUM FISSION CHAMBER                                    //
//    * ONE CHANNEL                                               //
//            FC-6                                                //
// NEUTRON DETECTORS                                              //
//    * 54 NDET FROM CHINU READOUT BY A DOUBLE GAIN: 108 CHANNELS //
//            L-I-1..9-LG       L-I-1..9-HG                       //
//            L-II-1..9-LG      L-II-1..9-HG                      //
//            L-III-1..9-LG     L-III-1..9-HG                     //
//            R-I-1..9-LG       R-I-1..9-HG                       //
//            R-II-1..9-LG      R-II-1..9-HG                      //
//            R-III-1..9-LG     R-III-1..9-HG                     //
//    *  4 NDET FROM B3 READOUT BY A DOUBLE GAIN: 8 CHANNELS      //
//            L-B3-1..2-LG      L-B3-1..2-HG                      //
//            R-B3-1..2-LG      R-B3-1..2-HG                      //
// HF BEAM                                                        //
//    * 1 CHANNEL                                                 //
//           HF-Beam                                              //
// MACRO PULSER                                                   //
//    * 1 CHANNEL                                                 // 
//           MacroPulse                                           // 
// PULSER                                                         //
//    * ONE CHANNEL                                               //
//            Pulser                                              //
//=============================================================== //


//============================================================================= //
// FASTER DATA FORMAT                                                           //
//============================================================================= //
//
// 1/ BOUCLER AVEC WHILE SUR LES DATA
//    while(data = faster_file_reader_next (reader))
//
// 2/ EXTRAIRE L'ALIAS QUI INDIQUE DE QUEL TYPE DE DONNEE S'AGIT
//    alias_data = faster_data_type_alias (data)
//    * pour les runs avec trigger ou coincidence (runs avec une FC)
//      trigger = OR de toutes les voies FC et de la voie pulser
//      alias_data = GROUP_TYPE_ALIAS
//    * pour les échelles
//      alias_data = QDC_COUNTER_TYPE_ALIAS
//    * pour les runs en source : trigger less, on ne lit que les neutrons
//      alias_data = QDC_TDC_X[1, 2, 3 ou 4]_TYPE_ALIAS
//      label = [1..11] for the FC
//      label = [17..124] for the Chi-Nu Ndet
//      label = [125..132] for the B3 Ndet
//
// 3a/ GROUP_TYPE_ALIAS: 
//    *  OUVRIR LE BUFFER READER
//       lsize_data = faster_data_load (data, buffer)
//       reader_data = faster_buffer_reader_open (buffer, lsize)
//    * BOUCLER AVEC WHILE SUR LES DONNEES DU GROUPE
//      group_data = faster_buffer_reader_next (reader_data)
//    * LIRE POUR CHAQUE ITEM DU GROUPE
//      . LE LABEL 
//        label = faster_data_label (group_data)
//      . LA CLOCK
//        clock = faster_data_clock_ns  (group_data)
//      . LA DONNEE ( DE TYPE QDC/TDC DANS NOTRE CAS)
//        faster_data_load (group_data, &var) --> var.q1, var.q2, ..., var.tdc
//
// 3b/ TRIGGERLESS : utilisation directement de data
//
// NE PAS OUBLIER DE FERMER LES READERS
//============================================================================= //

//  std includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

//  root includes
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

//  fasterac includes
#include "fasterac/fasterac.h"
#include "fasterac/fast_data.h"
#include "fasterac/qdc_caras.h"

//  sample.pid
#include "ReadPid/pid.h"

//  experiment specific
#include "../SetupSpecific/setup_specific.h"


#define CHINU_mult_max 15
#define B3_mult_max 4


using namespace std;

int main (int argc, char** argv) {

  char     name[100];
  UInt_t   nevents_coinc=0;
  UInt_t   nevents_source=0;
  UInt_t   nevents_counters=0;
  UShort_t detType, detSubType, detId;

  // === ================ === //
  // === FASTER VARIABLES === //
  // === ================ === //
  // --- FILE
  faster_file_reader_p   file_reader;
  // --- DATA
  faster_data_p          data;
  UInt_t                 lsize_data;
  UChar_t                buffer [3000];
  UChar_t                alias_data;
  UShort_t               label_data;
  ULong64_t              clock_data;
  // --- ONLY FOR COINC
  faster_buffer_reader_p buffer_reader;
  faster_data_p          group_data;
  UShort_t               label;
  ULong64_t              clock;
#if FC>0
#if   FC_nQ==1
  qdc_t_x1               FCdata;
#elif FC_nQ==2
  qdc_t_x2               FCdata;
#elif FC_nQ==3
  qdc_t_x3               FCdata;
#else
  qdc_t_x4               FCdata;
#endif
#endif
#if PULSER>0
#if PULSER_nQ==1
  qdc_t_x1               PULSERdata;
#elif PULSER_nQ==2
  qdc_t_x2               PULSERdata;
#elif PULSER_nQ==3
  qdc_t_x3               PULSERdata;
#else 
  qdc_t_x4               PULSERdata;
#endif
#endif
#if HF>0
#if HF_nQ==1
  qdc_t_x1               HFdata;
#elif HF_nQ==2
  qdc_t_x2               HFdata;
#elif HF_nQ==3
  qdc_t_x3               HFdata;
#else 
  qdc_t_x4               HFdata;
#endif
#endif
#if MACRO>0
#if MACRO_nQ==1
  qdc_t_x1               MACROdata;
#elif MACRO_nQ==2
  qdc_t_x2               MACROdata;
#elif MACRO_nQ==3
  qdc_t_x3               MACROdata;
#else 
  qdc_t_x4               MACROdata;
#endif
#endif
  // --- FOR COINC AND TRIGGER LESS
#if CHINU>0
#if   CHINU_nQ==1
  qdc_t_x1               CHINUdata;
#elif CHINU_nQ==2
  qdc_t_x2               CHINUdata;
#elif CHINU_nQ==3
  qdc_t_x3               CHINUdata;
#else
  qdc_t_x4               CHINUdata;
#endif
#endif
#if B3>0
#if   B3_nQ==1
  qdc_t_x1               B3data;
#elif B3_nQ==2
  qdc_t_x2               B3data;
#elif B3_nQ==3
  qdc_t_x3               B3data;
#else
  qdc_t_x4               B3data;
#endif
#endif
  
  
  // === ================ === //
  // === OUTPUT VARIABLES === //
  // === ================ === //

  // --- General 
  UInt_t nevents=0;;
  
  // --- Fission Chamber
#if FC>0
  UShort_t FC_mult_tot;
  UShort_t FC_mult[FC_nAnodes];
  UShort_t FC_label;
  UShort_t FC_anode;
  Double_t FC_time;
  UInt_t   FC_Q1;
  //ULong64_t clock;
  //ULong64_t tdc;
#if FC_nQ>1
  UInt_t FC_Q2;
#endif
#if FC_nQ>2
  UInt_t FC_Q3;
#endif
#if FC_nQ>3
  UInt_t FC_Q4;
#endif
#endif

  // --- Chi-Nu Neutron detectors
#if CHINU>0
  UShort_t CHINU_multLG_tot;
  UShort_t CHINU_multHG_tot;
  UShort_t CHINU_multLG[CHINU_nDets];
  UShort_t CHINU_multHG[CHINU_nDets];
  UShort_t CHINUlg_label[CHINU_mult_max];
  UShort_t CHINUhg_label[CHINU_mult_max];
  UShort_t CHINUlg_det[CHINU_mult_max];
  UShort_t CHINUhg_det[CHINU_mult_max];
  //ULong64_t CHINUlg_clock[CHINU_mult_max];
  //ULong64_t CHINUhg_clock[CHINU_mult_max];
  //Double_t CHINUlg_tdc[CHINU_mult_max];
  //Double_t CHINUhg_tdc[CHINU_mult_max];
  Double_t CHINUlg_time[CHINU_mult_max];
  Double_t CHINUhg_time[CHINU_mult_max];
  UInt_t CHINUlg_Q1[CHINU_mult_max];
  UInt_t CHINUhg_Q1[CHINU_mult_max];
#if CHINU_nQ>1
  UInt_t CHINUlg_Q2[CHINU_mult_max];
  UInt_t CHINUhg_Q2[CHINU_mult_max];
#endif
#if CHINU_nQ>2
  UInt_t CHINUlg_Q3[CHINU_mult_max];
  UInt_t CHINUhg_Q3[CHINU_mult_max];
#endif
#if CHINU_nQ>3
  UInt_t CHINUlg_Q4[CHINU_mult_max];
  UInt_t CHINUhg_Q4[CHINU_mult_max];
#endif
#endif

  // --- B3 Neutron detectors
#if B3>0
  UShort_t B3_multLG_tot;
  UShort_t B3_multHG_tot;
  UShort_t B3_multLG[B3_nDets];
  UShort_t B3_multHG[B3_nDets];
  UShort_t B3lg_label[B3_mult_max];
  UShort_t B3hg_label[B3_mult_max];
  UShort_t B3lg_det[B3_mult_max];
  UShort_t B3hg_det[B3_mult_max];
  //ULong64_t B3lg_clock[B3_mult_max];
  //ULong64_t B3hg_clock[B3_mult_max];
  //Double_t B3lg_tdc[B3_mult_max];
  //Double_t B3hg_tdc[B3_mult_max];
  Double_t B3lg_time[B3_mult_max];
  Double_t B3hg_time[B3_mult_max];
  UInt_t B3lg_Q1[B3_mult_max];
  UInt_t B3hg_Q1[B3_mult_max];
#if B3_nQ>1
  UInt_t B3lg_Q2[B3_mult_max];
  UInt_t B3hg_Q2[B3_mult_max];
#endif
#if B3_nQ>2
  UInt_t B3lg_Q3[B3_mult_max];
  UInt_t B3hg_Q3[B3_mult_max];
#endif
#if B3_nQ>3
  UInt_t B3lg_Q4[B3_mult_max];
  UInt_t B3hg_Q4[B3_mult_max];
#endif
#endif

  // --- Pulser
#if PULSER>0
  UShort_t PULSER_mult;
  UShort_t PULSER_label;
  //ULong64_t PULSER_clock;
  //ULong64_t PULSER_tdc;
  Double_t PULSER_time;
  UInt_t PULSER_Q1;
#endif

  // --- HF Beam
#if HF>0
  UShort_t HF_mult;
  UShort_t HF_label;
  //ULong64_t HF_clock;
  //ULong64_t HF_tdc;
  Double_t HF_time;
  UInt_t HF_Q1;
#endif

  // --- Macro Pulse
#if MACRO>0
  UShort_t MACRO_mult;
  UShort_t MACRO_label;
  //ULong64_t MACRO_clock;
  //ULong64_t MACRO_tdc;
  Double_t MACRO_time;
  UInt_t MACRO_Q1;
#endif

  // === =========== === //
  // === OUTPUT TREE === //
  // === =========== === //
  TString rootfilename = argv[2];
  TFile * fsave = new TFile(rootfilename,"recreate");
  fsave->ls();
  TString RunNumber=argv[3];
  TString FileNumber=argv[4];
#if TRIGGER_MODE==1
  sprintf(name,"Run %d File %d - COINC MODE",RunNumber.Atoi(),FileNumber.Atoi());
  TTree * t = new TTree("RawCoincData",name);
#else
  sprintf(name,"Run %d File %d - TRIGGERLESS MODE: ONLY NEUTRON DETECTORS",RunNumber.Atoi(),FileNumber.Atoi());
  TTree * t = new TTree("RawSourceData",name);
#endif
  // general variables
  t->Branch("Event",&nevents,"Event/i");
  //t->Branch("Label",&label_data,"Label/s");
  //t->Branch("Clock",&clock_data,"Clock/l");
  // chi-nu ndet 
#if CHINU>0
  t->Branch("CHINUlg_mult_tot",&CHINU_multLG_tot,"CHINUlg_mult_tot/s");
  t->Branch("CHINUhg_mult_tot",&CHINU_multHG_tot,"CHINUhg_mult_tot/s");
  t->Branch("CHINUlg_mult",CHINU_multLG,"CHINUlg_mult[54]/s");
  t->Branch("CHINUhg_mult",CHINU_multHG,"CHINUhg_mult[54]/s");
  t->Branch("CHINUlg_label",CHINUlg_label,"CHINUlg_label[15]/s");
  t->Branch("CHINUhg_label",CHINUhg_label,"CHINUhg_label[15]/s");
  t->Branch("CHINUlg_det",CHINUlg_det,"CHINUlg_det[15]/s");
  t->Branch("CHINUhg_det",CHINUhg_det,"CHINUhg_det[15]/s");
  //t->Branch("CHINUlg_clock",CHINUlg_clock,"CHINUlg_clock[15]/l");
  //t->Branch("CHINUhg_clock",CHINUhg_clock,"CHINUhg_clock[15]/l");
  //t->Branch("CHINUlg_tdc",CHINUlg_tdc,"CHINUlg_tdc[15]/l");
  //t->Branch("CHINUhg_tdc",CHINUhg_tdc,"CHINUhg_tdc[15]/l");
  t->Branch("CHINUlg_time",CHINUlg_time,"CHINUlg_time[15]/D");
  t->Branch("CHINUhg_time",CHINUhg_time,"CHINUhg_time[15]/D");
  t->Branch("CHINUlg_Q1",CHINUlg_Q1,"CHINUlg_Q1[15]/i");
  t->Branch("CHINUhg_Q1",CHINUhg_Q1,"CHINUhg_Q1[15]/i");
#if CHINU_nQ>1
  t->Branch("CHINUlg_Q2",CHINUlg_Q2,"CHINUlg_Q2[15]/i");
  t->Branch("CHINUhg_Q2",CHINUhg_Q2,"CHINUhg_Q2[15]/i");
#endif
#if CHINU_nQ>2
  t->Branch("CHINUlg_Q3",CHINUlg_Q3,"CHINUlg_Q3[15]/i");
  t->Branch("CHINUhg_Q3",CHINUhg_Q3,"CHINUhg_Q3[15]/i");
#endif
#if CHINU_nQ>3
  t->Branch("CHINUlg_Q4",CHINUlg_Q4,"CHINUlg_Q4[15]/i");
  t->Branch("CHINUhg_Q4",CHINUhg_Q4,"CHINUhg_Q4[15]/i");
#endif
#endif
  // B3 ndet 
#if B3>0
  t->Branch("B3lg_mult_tot",&B3_multLG_tot,"B3lg_mult_tot/s");
  t->Branch("B3hg_mult_tot",&B3_multHG_tot,"B3hg_mult_tot/s");
  t->Branch("B3lg_mult",B3_multLG,"B3lg_mult[4]/s");
  t->Branch("B3hg_mult",B3_multHG,"B3hg_mult[4]/s");
  t->Branch("B3lg_label",B3lg_label,"B3lg_label[4]/s");
  t->Branch("B3hg_label",B3hg_label,"B3hg_label[4]/s");
  t->Branch("B3lg_det",B3lg_det,"B3lg_det[4]/s");
  t->Branch("B3hg_det",B3hg_det,"B3hg_det[4]/s");
  //t->Branch("B3lg_clock",B3lg_clock,"B3lg_clock[4]/l");
  //t->Branch("B3hg_clock",B3hg_clock,"B3hg_clock[4]/l");
  //t->Branch("B3lg_tdc",B3lg_tdc,"B3lg_tdc[4]/l");
  //t->Branch("B3hg_tdc",B3hg_tdc,"B3hg_tdc[4]/l");
  t->Branch("B3lg_time",B3lg_time,"B3lg_time[4]/D");
  t->Branch("B3hg_time",B3hg_time,"B3hg_time[4]/D");
  t->Branch("B3lg_Q1",B3lg_Q1,"B3lg_Q1[4]/i");
  t->Branch("B3hg_Q1",B3hg_Q1,"B3hg_Q1[4]/i");
#if B3_nQ>1
  t->Branch("B3lg_Q2",B3lg_Q2,"B3lg_Q2[4]/i");
  t->Branch("B3hg_Q2",B3hg_Q2,"B3hg_Q2[4]/i");
#endif
#if B3_nQ>2
  t->Branch("B3lg_Q3",B3lg_Q3,"B3lg_Q3[4]/i");
  t->Branch("B3hg_Q3",B3hg_Q3,"B3hg_Q3[4]/i");
#endif
#if B3_nQ>3
  t->Branch("B3lg_Q4",B3lg_Q4,"B3lg_Q4[4]/i");
  t->Branch("B3hg_Q4",B3hg_Q4,"B3hg_Q4[4]/i");
#endif
#endif
#if TRIGGER_MODE==1 
  // dans le cas des données prises en coinc, on rajoute toutes les voies à l'arbre
  // fission chamber
#if FC>0
  t->Branch("FC_mult_tot",&FC_mult_tot,"FC_mult_tot/s");
  t->Branch("FC_mult",FC_mult,"FC_mult[11]/s");
  t->Branch("FC_label",&FC_label,"FC_label/s");
  t->Branch("FC_anode",&FC_anode,"FC_anode/s");
  //t->Branch("FC_clock",FC_clock,"FC_clock/l");
  //t->Branch("FC_tdc",FC_tdc,"FC_tdc/l");
  t->Branch("FC_time",&FC_time,"FC_time/D");
  t->Branch("FC_Q1",&FC_Q1,"FC_Q1/i");
#if FC_nQ>1
  t->Branch("FC_Q2",&FC_Q2,"FC_Q2/i");
#endif
#if FC_nQ>2
  t->Branch("FC_Q3",&FC_Q3,"FC_Q3/i");
#endif
#if FC_nQ>3
  t->Branch("FC_Q4",&FC_Q4,"FC_Q4/i");
#endif
#endif
  // Pulser
#if PULSER>0
  t->Branch("PULSER_mult",&PULSER_mult,"PULSER_mult/i");
  t->Branch("PULSER_label",&PULSER_label,"PULSER_label/i");
  //t->Branch("PULSER_clock",&PULSER_clock,"PULSER_time/D");
  //t->Branch("PULSER_tdc",&PULSER_tdc,"PULSER_tdc/l");
  t->Branch("PULSER_time",&PULSER_time,"PULSER_time/D");
  t->Branch("PULSER_Q1",&PULSER_Q1,"PULSER_Q1/i");
#endif
  // HF Beam
#if HF>0
  t->Branch("HF_mult",&HF_mult,"HF_mult/s");
  t->Branch("HF_label",&HF_label,"HF_label/s");
  //t->Branch("HF_clock",&HF_clock,"HF_clock/D");
  //t->Branch("HF_tdc",&HF_tdc,"HF_tdc/D");
  t->Branch("HF_time",&HF_time,"HF_time/D");
  t->Branch("HF_Q1",&HF_Q1,"HF_Q1/i");
#endif
  // Macro Pulse
#if MACRO>0
  t->Branch("MACRO_mult",&MACRO_mult,"MACRO_mult/s");
  t->Branch("MACRO_label",&MACRO_label,"MACRO_label/s");
  //t->Branch("MACRO_clock",&MACRO_clock,"MACRO_clock/D");
  //t->Branch("MACRO_tdc",vMACRO_tdc,"MACRO_tdc/D");
  t->Branch("MACRO_time",&MACRO_time,"MACRO_time/D");
  t->Branch("MACRO_Q1",&MACRO_Q1,"MACRO_Q1/i");
#endif
#endif 


  // === ======================= === //
  // === OPEN FASTER FILE READER === //
  // === ======================= === //
  TString datafilename = argv[1];
  file_reader = faster_file_reader_open (datafilename);
  if (file_reader == NULL) {
    printf ("error opening file %s\n", datafilename.Data());
    return EXIT_FAILURE;
  }
  printf ("opening Faster file %s\n", datafilename.Data());
  
  // === ================== === //
  // === LOOP OVER THE DATA === //
  // === ================== === //
  while((data = faster_file_reader_next (file_reader))){
    
    nevents++; 
    if ((nevents%5000000)==0) cout << nevents << endl;

    // --- ---------------------------------- --- // 
    // --- INITIALIZATION OF OUTPUT VARIABLES --- //
    // --- ---------------------------------- --- // 


    // *** Fission Chamber *** //
#if FC>0
    FC_mult_tot=0;
    for(UShort_t anode=0; anode<FC_nAnodes; anode++) FC_mult[anode]=0;
    FC_label=0;
    FC_anode=0;
    //FC_clock=0;
    //FC_tdc=0;
    FC_time=0;
    FC_Q1=0;
#if FC_nQ>1
    FC_Q2=0;
#endif
#if FC_nQ>2
    FC_Q3=0;
#endif
#if FC_nQ>3
    FC_Q4=0;
#endif
#endif


    // *** Chi-Nu Neutron Detectors *** //
#if CHINU>0
    CHINU_multLG_tot=0;
    CHINU_multHG_tot=0;
    for(UShort_t det=0; det<CHINU_nDets; det++) 
      {
	CHINU_multLG[det]=0; 
	CHINU_multHG[det]=0;
      }
    for(UShort_t m=0; m<CHINU_mult_max; m++){
      CHINUlg_label[m]=0;      CHINUhg_label[m]=0;
      CHINUlg_det[m]=0;        CHINUhg_det[m]=0;
      //CHINUlg_clock[m]=0;    CHINUhg_clock[m]=0;
      //CHINUlg_tdc[m]=0;      CHINUhg_tdc[m]=0;
      CHINUlg_time[m]=0;       CHINUhg_time[m]=0;
      CHINUlg_Q1[m]=0;         CHINUhg_Q1[m]=0;
#if CHINU_nQ>1
      CHINUlg_Q2[m]=0;         CHINUhg_Q2[m]=0;
#endif
#if CHINU_nQ>2
      CHINUlg_Q3[m]=0;         CHINUhg_Q3[m]=0;
#endif
#if CHINU_nQ>3
      CHINUlg_Q4[m]=0;         CHINUhg_Q4[m]=0;
#endif
    }
#endif


    // *** B3 Neutron Detectors *** //
#if B3>0
    B3_multLG_tot=0;
    B3_multHG_tot=0;
    for(UShort_t det=0; det<B3_nDets; det++) 
      {
	B3_multLG[det]=0; 
	B3_multHG[det]=0;
      }
    for(UShort_t m=0; m<B3_mult_max; m++){
      B3lg_label[m]=0;      B3hg_label[m]=0;
      B3lg_det[m]=0;        B3hg_det[m]=0;
      //B3lg_clock[m]=0;    B3hg_clock[m]=0;
      //B3lg_tdc[m]=0;      B3hg_tdc[m]=0;
      B3lg_time[m]=0;       B3hg_time[m]=0;
      B3lg_Q1[m]=0;         B3hg_Q1[m]=0;
#if B3_nQ>1
      B3lg_Q2[m]=0;         B3hg_Q2[m]=0;
#endif
#if B3_nQ>2
      B3lg_Q3[m]=0;         B3hg_Q3[m]=0;
#endif
#if B3_nQ>3
      B3lg_Q4[m]=0;         B3hg_Q4[m]=0;
#endif
    }
#endif


    // *** Pulser *** //
#if PULSER>0
    PULSER_mult=0;
    PULSER_label=0;
    //PULSER_clock=0;
    //PULSER_tdc=0;
    PULSER_time=0;
    PULSER_Q1=0;
#endif


    // *** HF Beam *** //
#if HF>0
    HF_mult=0;
    HF_label=0;
    //HF_clock=0;
    //HF_tdc=0;
    HF_time=0;
    HF_Q1=0;
#endif


    // *** Macro Pulse *** //
#if MACRO>0
    MACRO_mult=0;
    MACRO_label=0;
    //MACRO_clock=0;
    //MACRO_tdc=0;
    MACRO_time=0;
    MACRO_Q1=0;
#endif

    // --- -------------------------------------- --- //
    // --- IDENTIFICATION OF THE TYPE OF THE DATA --- //
    // --- -------------------------------------- --- //
    alias_data  = faster_data_type_alias (data);
    label_data  = (UShort_t)(faster_data_label (data));
    clock_data  = faster_data_clock_ns(data);
    
    switch(alias_data){

    case QDC_COUNTER_TYPE_ALIAS:
      // COUNTERS DATA
      nevents_counters++;
      break; // end of COUNTERS DATA
      
    case GROUP_TYPE_ALIAS:      
      // COINC DATA
      if(TRIGGER_MODE!=1) cout << "ERROR, GROUP_TYPE_ALIAS FOUND WITH TRIGGER_MODE = "<< TRIGGER_MODE << " DIFFERENT OF THE EXPECTED 1 VALUE, CHECK setup_specific.h" << endl;
      nevents_coinc++;
      // --- ----------------------------- --- //
      // --- OPEN THE FASTER BUFFER READER --- //
      // --- ----------------------------- --- //
      lsize_data  = faster_data_load (data, buffer);
      buffer_reader = faster_buffer_reader_open (buffer, lsize_data);

      // --- ---------------- --- //
      // --- READOUT THE data --- //
      // --- ---------------- --- //
      if(label_data!=LABEL_GROUP) cout << "WARNING !!! SEEMS TO HAVE A MISMATCH BETWEEN GROUP_ALIAS AND GROUP_LABEL" << endl;
      while ((group_data = faster_buffer_reader_next (buffer_reader)) != NULL){
	label = (UShort_t)(faster_data_label (group_data));
	clock = faster_data_clock_ns (group_data);
	detType = (UShort_t)(DetType[label-1]);
	switch (detType)
	  {
#if FC>0
	  case 1: //--- FISSION CHAMBER --- //
	    faster_data_load(group_data,&FCdata);
	    FC_mult_tot++;
	    FC_label = label;
	    FC_anode = (UShort_t)(Label2Ch[label-1]); 
	    FC_mult[FC_anode-1]++;
	    //FC_clock=clock;
	    //vFC_tdc=FCdata.tdc;
	    FC_time = (double)(clock)+(double)(qdc_conv_dt_ns(FCdata.tdc));
	    FC_Q1=(UInt_t)(FCdata.q1);
#if FC_nQ>1
	    FC_Q2=(UInt_t)(FCdata.q2);
#endif
#if FC_nQ>2
	    FC_Q3=(UInt_t)(FCdata.q3);
#endif
#if FC_nQ>3
	    FC_Q4=(UInt_t)(FCdata.q4);
#endif
	    break;
#endif
#if CHINU>0
	  case 2: //--- CHI-NU NEUTRON DETECTORS --- //
	    faster_data_load(group_data,&CHINUdata);
	    detSubType = DetSubType[label-1];
	    detId      = (UShort_t)Label2Ch[label-1];
	    switch(detSubType)
	      {
	      case 21: // LOW GAIN 
		CHINU_multLG[detId-1]++;
		if(CHINU_multLG_tot<CHINU_mult_max){
		  CHINUlg_label[CHINU_multLG_tot] = label;
		  CHINUlg_det[CHINU_multLG_tot] = detId;
		  //CHINUlg_clock[CHINU_multLG_tot] = clock;
		  //CHINUlg_tdc[CHINU_multLG_tot] = CHINUdata.tdc;
		  CHINUlg_time[CHINU_multLG_tot] = (double)(clock)+(double)(qdc_conv_dt_ns(CHINUdata.tdc)) ;
		  CHINUlg_Q1[CHINU_multLG_tot] = CHINUdata.q1;
#if CHINU_nQ>1
		  CHINUlg_Q2[CHINU_multLG_tot] = CHINUdata.q2;
#endif
#if CHINU_nQ>2
		  CHINUlg_Q3[CHINU_multLG_tot] = CHINUdata.q3;
#endif
#if CHINU_nQ>3
		  CHINUlg_Q4[CHINU_multLG_tot] = CHINUdata.q4;
#endif
		}
		CHINU_multLG_tot++;
		break;
	      case 22: // HIGH GAIN
		CHINU_multHG[detId-1]++;
		if(CHINU_multHG_tot<CHINU_mult_max){
		  CHINUhg_label[CHINU_multHG_tot] = label;
		  CHINUhg_det[CHINU_multHG_tot] = detId;
		  //CHINUhg_clock[CHINU_multHG_tot] = clock;
		  //CHINUhg_tdc[CHINU_multHG_tot] = CHINUdata.tdc ;
		  CHINUhg_time[CHINU_multHG_tot] = (double)(clock)+(double)(qdc_conv_dt_ns(CHINUdata.tdc));
		  CHINUhg_Q1[CHINU_multHG_tot] = CHINUdata.q1;
#if CHINU_nQ>1
		  CHINUhg_Q2[CHINU_multHG_tot] = CHINUdata.q2;
#endif
#if CHINU_nQ>2
		  CHINUhg_Q3[CHINU_multHG_tot] = CHINUdata.q3;
#endif
#if CHINU_nQ>3
		  CHINUhg_Q4[CHINU_multHG_tot] = CHINUdata.q4;
#endif
		}
		CHINU_multHG_tot++;
		break;
	      default:
		break;
	      }//end of switch(detSubType)	    
	    break;
#endif
#if B3>0
	  case 3: //--- B3 NEUTRON DETECTORS --- //
	    faster_data_load(group_data,&B3data);
	    detId      = (UShort_t)Label2Ch[label-1];
	    detSubType = DetSubType[label-1];
	    switch(detSubType)
	      {
	      case 21: // LOW GAIN 
		B3_multLG[detId-1]++;
		if(B3_multLG_tot<B3_mult_max){
		  B3lg_label[B3_multLG_tot]=label;
		  B3lg_det[B3_multLG_tot]=detId;
		  //B3lg_clock[B3_multLG_tot]=clock;
		  //B3lg_tdc[B3_multLG_tot]=B3data.tdc;
		  B3lg_time[B3_multLG_tot]=(double)(clock)+(double)(qdc_conv_dt_ns(B3data.tdc));
		  B3lg_Q1[B3_multLG_tot]=B3data.q1;
#if B3_nQ>1
		  B3lg_Q2[B3_multLG_tot]=B3data.q2;
#endif
#if B3_nQ>2
		  B3lg_Q3[B3_multLG_tot]=B3data.q3;
#endif
#if B3_nQ>3
		  B3lg_Q4[B3_multLG_tot]=B3data.q4;
#endif
		}
		B3_multLG_tot++;
		break;
	      case 22: // HIGH GAIN
		B3_multHG[detId-1]++;
		if(B3_multHG_tot<B3_mult_max){
		  B3hg_label[B3_multHG_tot]=label;
		  B3hg_det[B3_multHG_tot]=detId;
		  //B3hg_clock[B3_multHG_tot]=cloc);
		  //B3hg_tdc[B3_multHG_tot]=B3data.tdc;
		  B3hg_time[B3_multHG_tot]=(double)(clock)+(double)(qdc_conv_dt_ns(B3data.tdc));
		  B3hg_Q1[B3_multHG_tot]=B3data.q1;
#if B3_nQ>1
		  B3hg_Q2[B3_multHG_tot]=B3data.q2;
#endif
#if B3_nQ>2
		  B3hg_Q3[B3_multHG_tot]=B3data.q3;
#endif
#if B3_nQ>3
		  B3hg_Q4[B3_multHG_tot]=B3data.q4;
#endif
		}
		B3_multHG_tot++;
		break;
	      default:
		break;
	      }//end of switch(detSubType)
	    break;
#endif
#if PULSER>0
	  case 4: //--- PULSER --- //
	    faster_data_load(group_data,&PULSERdata);
	    PULSER_mult++;
	    PULSER_label=label;
	    //PULSER_clock=clock;
	    //PULSER_tdc=PULSERdata.tdc;
	    PULSER_time=(double)(clock)+(double)(qdc_conv_dt_ns(PULSERdata.tdc));
	    PULSER_Q1=PULSERdata.q1;
	    break;
#endif
#if HF>0
	  case 5: //--- HF BEAM--- //
	    faster_data_load(group_data,&HFdata);
	    HF_label=label;
	    //HF_clock=clock;
	    //HF_tdc=HFdata.tdc;
	    HF_time=(double)(clock)+(double)(qdc_conv_dt_ns(HFdata.tdc));
	    HF_Q1=HFdata.q1;
	    break;
#endif
#if MACRO>0
	  case 6: //--- MACRO PULSE --- //
	    faster_data_load(group_data,&MACROdata);
	    MACRO_label=label;
	    //vMACRO_clock=clock;
	    //vMACRO_tdc=MACROdata.tdc;
	    MACRO_time=(double)(clock)+(double)(qdc_conv_dt_ns(MACROdata.tdc));
	    MACRO_Q1=MACROdata.q1;
	    break;
#endif
	  default:
	    cout << "WARNING : DETECTOR TYPE " << detType << " FOUND BUT DO NOT MATCH ANY CASES " << endl;
	    break;
	  }//end of switch(detType)

      }// end of while(group)
      // --- ------------------------------ --- //
      // --- CLOSE THE FASTER BUFFER READER --- //
      // --- ------------------------------ --- //
      faster_buffer_reader_close(buffer_reader);
      // --- ------------- --- //
      // --- FILL THE TREE --- //
      // --- ------------- --- //

      t->Fill();
      break;// END OF COINC DATA
      
    default: 
      nevents_source++;
      break;
    }// end of switch(alias)
    
    

  }//end of while data

  cout << "END OF LOOP OVER THE DATA, READY TO SAVE " << endl;

  // === ====================== === //
  // === SAVE TREE IN ROOT FILE === //
  // === ====================== === //
  fsave->ls();
  fsave->cd();
  t->Write();
  fsave->ls();
  fsave->Close();

  // === ======================== === //
  // === CLOSE FASTER FILE READER === //
  // === ======================== === //
  //faster_file_reader_close (file_reader);
  cout << "faster_file_reader_close (file_reader)" << endl;


  // === =============== === //
  // === SOME STATISTICS === //
  // === =============== === //
  cout << "TOTAL NUMBER OF EVENTS: " << nevents << endl;
  cout << "  . COINC NUMBER OF EVENTS: " << nevents_coinc << endl;
  cout << "  . SOURCE NUMBER OF EVENTS: " << nevents_source << endl;
  cout << "  . COUNTERS NUMBER OF EVENTS: " << nevents_counters << endl;

}//end of main
