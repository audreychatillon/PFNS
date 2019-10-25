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


using namespace std;

int main (int argc, char** argv) {

  char name[100];
  UInt_t nevents_coinc=0;
  UInt_t nevents_source=0;
  UInt_t nevents_counters=0;
  UInt_t detType, detSubType;

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
  UShort_t FC_mult[FC_nAnodes];
  std::vector<UShort_t> vFC_label;
  std::vector<UShort_t> vFC_anode;
  //std::vector<ULong64_t> vFC_clock;
  //std::vector<Double_t> vFC_tdc;
  std::vector<Double_t> vFC_time;
  std::vector<UInt_t> vFC_Q1;
#if FC_nQ>1
  std::vector<UInt_t> vFC_Q2;
#endif
#if FC_nQ>2
  std::vector<UInt_t> vFC_Q3;
#endif
#if FC_nQ>3
  std::vector<UInt_t> vFC_Q4;
#endif
#endif

  // --- Chi-Nu Neutron detectors
#if CHINU>0
  UShort_t CHINU_multLG[CHINU_nDets];
  UShort_t CHINU_multHG[CHINU_nDets];
  std::vector<UShort_t> vCHINUlg_label;
  std::vector<UShort_t> vCHINUhg_label;
  std::vector<UShort_t> vCHINUlg_det;
  std::vector<UShort_t> vCHINUhg_det;
  //std::vector<ULong64_t> vCHINUlg_clock;
  //std::vector<ULong64_t> vCHINUhg_clock;
  //std::vector<Double_t> vCHINUlg_tdc;
  //std::vector<Double_t> vCHINUhg_tdc;
  std::vector<Double_t> vCHINUlg_time;
  std::vector<Double_t> vCHINUhg_time;
  std::vector<UInt_t> vCHINUlg_Q1;
  std::vector<UInt_t> vCHINUhg_Q1;
#if CHINU_nQ>1
  std::vector<UInt_t> vCHINUlg_Q2;
  std::vector<UInt_t> vCHINUhg_Q2;
#endif
#if CHINU_nQ>2
  std::vector<UInt_t> vCHINUlg_Q3;
  std::vector<UInt_t> vCHINUhg_Q3;
#endif
#if CHINU_nQ>3
  std::vector<UInt_t> vCHINUlg_Q4;
  std::vector<UInt_t> vCHINUhg_Q4;
#endif
#endif

  // --- B3 Neutron detectors
#if B3>0
  UShort_t B3_multLG[B3_nDets];
  UShort_t B3_multHG[B3_nDets];
  std::vector<UShort_t> vB3lg_label;
  std::vector<UShort_t> vB3hg_label;
  std::vector<UShort_t> vB3lg_det;
  std::vector<UShort_t> vB3hg_det;
  //std::vector<ULong64_t> vB3lg_clock;
  //std::vector<ULong64_t> vB3hg_clock;
  //std::vector<Double_t> vB3lg_tdc;
  //std::vector<Double_t> vB3hg_tdc;
  std::vector<Double_t> vB3lg_time;
  std::vector<Double_t> vB3hg_time;
  std::vector<UInt_t> vB3lg_Q1;
  std::vector<UInt_t> vB3hg_Q1;
#if B3_nQ>1
  std::vector<UInt_t> vB3lg_Q2;
  std::vector<UInt_t> vB3hg_Q2;
#endif
#if B3_nQ>2
  std::vector<UInt_t> vB3lg_Q3;
  std::vector<UInt_t> vB3hg_Q3;
#endif
#if B3_nQ>3
  std::vector<UInt_t> vB3lg_Q4;
  std::vector<UInt_t> vB3hg_Q4;
#endif
#endif

  // --- Pulser
#if PULSER>0
  std::vector<UShort_t> vPULSER_label;
  //std::vector<ULong64_t> vPULSER_clock;
  //std::vector<Double_t> vPULSER_tdc;
  std::vector<Double_t> vPULSER_time;
  std::vector<UInt_t> vPULSER_Q1;
#endif

  // --- HF Beam
#if HF>0
  std::vector<UShort_t> vHF_label;
  //std::vector<ULong64_t> vHF_clock;
  //std::vector<Double_t> vHF_tdc;
  std::vector<Double_t> vHF_time;
  std::vector<UInt_t> vHF_Q1;
#endif

  // --- Macro Pulse
#if MACRO>0
  std::vector<UShort_t> vMACRO_label;
  //std::vector<ULong64_t> vMACRO_clock;
  //std::vector<Double_t> vMACRO_tdc;
  std::vector<Double_t> vMACRO_time;
  std::vector<UInt_t> vMACRO_Q1;
#endif

  // === =========== === //
  // === OUTPUT TREE === //
  // === =========== === //
  TString rootfilename = argv[2];
  TFile * fsave = new TFile(rootfilename,"recreate");
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
  t->Branch("CHINUlg_mult",CHINU_multLG,"CHINUlg_mult[54]/s");
  t->Branch("CHINUhg_mult",CHINU_multHG,"CHINUhg_mult[54]/s");
  t->Branch("vCHINUlg_label",&vCHINUlg_label);
  t->Branch("vCHINUhg_label",&vCHINUhg_label);
  t->Branch("vCHINUlg_det",&vCHINUlg_det);
  t->Branch("vCHINUhg_det",&vCHINUhg_det);
  //t->Branch("vCHINUlg_clock",&vCHINUlg_clock);
  //t->Branch("vCHINUhg_clock",&vCHINUhg_clock);
  //t->Branch("vCHINUlg_tdc",&vCHINUlg_tdc);
  //t->Branch("vCHINUhg_tdc",&vCHINUhg_tdc);
  t->Branch("vCHINUlg_time",&vCHINUlg_time);
  t->Branch("vCHINUhg_time",&vCHINUhg_time);
  t->Branch("vCHINUlg_Q1",&vCHINUlg_Q1);
  t->Branch("vCHINUhg_Q1",&vCHINUhg_Q1);
#if CHINU_nQ>1
  t->Branch("vCHINUlg_Q2",&vCHINUlg_Q2);
  t->Branch("vCHINUhg_Q2",&vCHINUhg_Q2);
#endif
#if CHINU_nQ>2
  t->Branch("vCHINUlg_Q3",&vCHINUlg_Q3);
  t->Branch("vCHINUhg_Q3",&vCHINUhg_Q3);
#endif
#if CHINU_nQ>3
  t->Branch("vCHINUlg_Q4",&vCHINUlg_Q4);
  t->Branch("vCHINUhg_Q4",&vCHINUhg_Q4);
#endif
#endif
  // B3 ndet 
#if B3>0
  t->Branch("B3lg_mult",B3_multLG,"B3lg_mult[4]/s");
  t->Branch("B3hg_mult",B3_multHG,"B3hg_mult[4]/s");
  t->Branch("vB3lg_label",&vB3lg_label);
  t->Branch("vB3hg_label",&vB3hg_label);
  t->Branch("vB3lg_det",&vB3lg_det);
  t->Branch("vB3hg_det",&vB3hg_det);
  //t->Branch("vB3lg_clock",&vB3lg_clock);
  //t->Branch("vB3hg_clock",&vB3hg_clock);
  //t->Branch("vB3lg_tdc",&vB3lg_tdc);
  //t->Branch("vB3hg_tdc",&vB3hg_tdc);
  t->Branch("vB3lg_time",&vB3lg_time);
  t->Branch("vB3hg_time",&vB3hg_time);
  t->Branch("vB3lg_Q1",&vB3lg_Q1);
  t->Branch("vB3hg_Q1",&vB3hg_Q1);
#if B3_nQ>1
  t->Branch("vB3lg_Q2",&vB3lg_Q2);
  t->Branch("vB3hg_Q2",&vB3hg_Q2);
#endif
#if B3_nQ>2
  t->Branch("vB3lg_Q3",&vB3lg_Q3);
  t->Branch("vB3hg_Q3",&vB3hg_Q3);
#endif
#if B3_nQ>3
  t->Branch("vB3lg_Q4",&vB3lg_Q4);
  t->Branch("vB3hg_Q4",&vB3hg_Q4);
#endif
#endif
#if TRIGGER_MODE==1 
  // dans le cas des données prises en coinc, on rajoute toutes les voies à l'arbre
  // fission chamber
#if FC>0
  t->Branch("FC_mult",FC_mult,"FC_mult[11]/s");
  t->Branch("vFC_label",&vFC_label);
  t->Branch("vFC_anode",&vFC_anode);
  //t->Branch("vFC_clock",&vFC_clock);
  //t->Branch("vFC_tdc",&vFC_tdc);
  t->Branch("vFC_time",&vFC_time);
  t->Branch("vFC_Q1",&vFC_Q1);
#if FC_nQ>1
  t->Branch("vFC_Q2",&vFC_Q2);
#endif
#if FC_nQ>2
  t->Branch("vFC_Q3",&vFC_Q3);
#endif
#if FC_nQ>3
  t->Branch("vFC_Q4",&vFC_Q4);
#endif
#endif
  // Pulser
#if PULSER>0
  t->Branch("vPULSER_label",&vPULSER_label);
  //t->Branch("vPULSER_clock",&vPULSER_clock);
  //t->Branch("vPULSER_tdc",&vPULSER_tdc);
  t->Branch("vPULSER_time",&vPULSER_time);
  t->Branch("vPULSER_Q1",&vPULSER_Q1);
#endif
  // HF Beam
#if HF>0
  t->Branch("vHF_label",&vHF_label);
  //t->Branch("vHF_clock",&vHF_clock);
  //t->Branch("vHF_tdc",&vHF_tdc);
  t->Branch("vHF_time",&vHF_time);
  t->Branch("vHF_Q1",&vHF_Q1);
#endif
  // Macro Pulse
#if MACRO>0
  t->Branch("vMACRO_label",&vMACRO_label);
  //t->Branch("vMACRO_clock",&vMACRO_clock);
  //t->Branch("vMACRO_tdc",&vMACRO_tdc);
  t->Branch("vMACRO_time",&vMACRO_time);
  t->Branch("vMACRO_Q1",&vMACRO_Q1);
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
    for(UShort_t anode=0; anode<FC_nAnodes; anode++) FC_mult[anode]=0;
    vFC_label.clear();
    vFC_anode.clear();
    //vFC_clock.clear();
    //vFC_tdc.clear();
    vFC_time.clear();
    vFC_Q1.clear();
#if FC_nQ>1
    vFC_Q2.clear();
#endif
#if FC_nQ>2
    vFC_Q3.clear();
#endif
#if FC_nQ>3
    vFC_Q4.clear();
#endif
#endif
    // *** Chi-Nu Neutron Detectors *** //
#if CHINU>0
    for(UShort_t det=0; det<CHINU_nDets; det++) {CHINU_multLG[det]=0; CHINU_multHG[det]=0;}
    vCHINUlg_label.clear();
    vCHINUhg_label.clear();
    vCHINUlg_det.clear();
    vCHINUhg_det.clear();
    //vCHINUlg_clock.clear();
    //vCHINUhg_clock.clear();
    //vCHINUlg_tdc.clear();
    //vCHINUhg_tdc.clear();
    vCHINUlg_time.clear();
    vCHINUhg_time.clear();
    vCHINUlg_Q1.clear();
    vCHINUhg_Q1.clear();
#if CHINU_nQ>1
    vCHINUlg_Q2.clear();
    vCHINUhg_Q2.clear();
#endif
#if CHINU_nQ>2
    vCHINUlg_Q3.clear();
    vCHINUhg_Q3.clear();
#endif
#if CHINU_nQ>3
    vCHINUlg_Q4.clear();
    vCHINUhg_Q4.clear();
#endif
#endif
    // *** B3 Neutron Detectors *** //
#if B3>0
    for(UShort_t det=0; det<B3_nDets; det++) {B3_multLG[det]=0; B3_multHG[det]=0;}
    vB3lg_label.clear();
    vB3hg_label.clear();
    vB3lg_det.clear();
    vB3hg_det.clear();
    //vB3lg_clock.clear();
    //vB3hg_clock.clear();
    //vB3lg_tdc.clear();
    //vB3hg_tdc.clear();
    vB3lg_time.clear();
    vB3hg_time.clear();
    vB3lg_Q1.clear();
    vB3hg_Q1.clear();
#if B3_nQ>1
    vB3lg_Q2.clear();
    vB3hg_Q2.clear();
#endif
#if B3_nQ>2
    vB3lg_Q3.clear();
    vB3hg_Q3.clear();
#endif
#if B3_nQ>3
    vB3lg_Q4.clear();
    vB3hg_Q4.clear();
#endif
#endif
    // *** Pulser *** //
#if PULSER>0
    vPULSER_label.clear();
    //vPULSER_clock.clear();
    //vPULSER_tdc.clear();
    vPULSER_time.clear();
    vPULSER_Q1.clear();
#endif
    // *** HF Beam *** //
#if HF>0
    vHF_label.clear();
    //vHF_clock.clear();
    //vHF_tdc.clear();
    vHF_time.clear();
    vHF_Q1.clear();
#endif
    // *** Macro Pulse *** //
#if MACRO>0
    vMACRO_label.clear();
    //vMACRO_clock.clear();
    //vMACRO_tdc.clear();
    vMACRO_time.clear();
    vMACRO_Q1.clear();
#endif

    // --- -------------------------------------- --- //
    // --- IDENTIFICATION OF THE TYPE OF THE DATA --- //
    // --- -------------------------------------- --- //
    alias_data  = faster_data_type_alias (data);
    label_data  = faster_data_label (data);
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
	label = faster_data_label (group_data);
	clock = faster_data_clock_ns (group_data);
	detType = DetType[label-1];
	switch (detType)
	  {
#if FC>0
	  case 1: //--- FISSION CHAMBER --- //
	    faster_data_load(group_data,&FCdata);
	    vFC_label.push_back(label);
	    vFC_anode.push_back(Label2Ch[label-1]); FC_mult[Label2Ch[label-1]-1]++;
	    //vFC_clock.push_back(clock);
	    //vFC_tdc.push_back(FCdata.tdc);
	    vFC_time.push_back((double)(clock)+(double)(qdc_conv_dt_ns(FCdata.tdc)));
	    vFC_Q1.push_back(FCdata.q1);
#if FC_nQ>1
	    vFC_Q2.push_back(FCdata.q2);
#endif
#if FC_nQ>2
	    vFC_Q3.push_back(FCdata.q3);
#endif
#if FC_nQ>3
	    vFC_Q4.push_back(FCdata.q4);
#endif
	    break;
#endif
#if CHINU>0
	  case 2: //--- CHI-NU NEUTRON DETECTORS --- //
	    faster_data_load(group_data,&CHINUdata);
	    detSubType = DetSubType[label-1];
	    switch(detSubType)
	      {
	      case 21: // LOW GAIN 
		CHINU_multLG[Label2Ch[label-1]]++;
		vCHINUlg_label.push_back(label);
		vCHINUlg_det.push_back(Label2Ch[label-1]);
		//vCHINUlg_clock.push_back(clock);
		//vCHINUlg_tdc.push_back(CHINUdata.tdc);
		vCHINUlg_time.push_back((double)(clock)+(double)(qdc_conv_dt_ns(CHINUdata.tdc)));
		vCHINUlg_Q1.push_back(CHINUdata.q1);
#if CHINU_nQ>1
		vCHINUlg_Q2.push_back(CHINUdata.q2);
#endif
#if CHINU_nQ>2
		vCHINUlg_Q3.push_back(CHINUdata.q3);
#endif
#if CHINU_nQ>3
		vCHINUlg_Q4.push_back(CHINUdata.q4);
#endif
		break;
	      case 22: // HIGH GAIN
		CHINU_multHG[Label2Ch[label-1]]++;
		vCHINUhg_label.push_back(label);
		vCHINUhg_det.push_back(Label2Ch[label-1]);
		//vCHINUhg_clock.push_back(clock);
		//vCHINUhg_tdc.push_back(CHINUdata.tdc);
		vCHINUhg_time.push_back((double)(clock)+(double)(qdc_conv_dt_ns(CHINUdata.tdc)));
		vCHINUhg_Q1.push_back(CHINUdata.q1);
#if CHINU_nQ>1
		vCHINUhg_Q2.push_back(CHINUdata.q2);
#endif
#if CHINU_nQ>2
		vCHINUhg_Q3.push_back(CHINUdata.q3);
#endif
#if CHINU_nQ>3
		vCHINUhg_Q4.push_back(CHINUdata.q4);
#endif
		break;
	      default:
		break;
	      }//end of switch(detSubType)
	    break;
#endif
#if B3>0
	  case 3: //--- B3 NEUTRON DETECTORS --- //
	    faster_data_load(group_data,&B3data);
	    detSubType = DetSubType[label-1];
	    switch(detSubType)
	      {
	      case 21: // LOW GAIN 
		B3_multLG[Label2Ch[label-1]]++;
		vB3lg_label.push_back(label);
		vB3lg_det.push_back(Label2Ch[label-1]);
		//vB3lg_clock.push_back(clock);
		//vB3lg_tdc.push_back(B3data.tdc);
		vB3lg_time.push_back((double)(clock)+(double)(qdc_conv_dt_ns(B3data.tdc)));
		vB3lg_Q1.push_back(B3data.q1);
#if B3_nQ>1
		vB3lg_Q2.push_back(B3data.q2);
#endif
#if B3_nQ>2
		vB3lg_Q3.push_back(B3data.q3);
#endif
#if B3_nQ>3
		vB3lg_Q4.push_back(B3data.q4);
#endif
		break;
	      case 22: // HIGH GAIN
		B3_multHG[Label2Ch[label-1]]++;
		vB3hg_label.push_back(label);
		vB3hg_det.push_back(Label2Ch[label-1]);
		//vB3hg_clock.push_back(clock);
		//vB3hg_tdc.push_back(B3data.tdc);
		vB3hg_time.push_back((double)(clock)+(double)(qdc_conv_dt_ns(B3data.tdc)));
		vB3hg_Q1.push_back(B3data.q1);
#if B3_nQ>1
		vB3hg_Q2.push_back(B3data.q2);
#endif
#if B3_nQ>2
		vB3hg_Q3.push_back(B3data.q3);
#endif
#if B3_nQ>3
		vB3hg_Q4.push_back(B3data.q4);
#endif
		break;
	      default:
		break;
	      }//end of switch(detSubType)
	    break;
#endif
#if PULSER>0
	  case 4: //--- PULSER --- //
	    faster_data_load(group_data,&PULSERdata);
	    vPULSER_label.push_back(label);
	    //vPULSER_clock.push_back(clock);
	    //vPULSER_tdc.push_back(PULSERdata.tdc);
	    vPULSER_time.push_back((double)(clock)+(double)(qdc_conv_dt_ns(PULSERdata.tdc)));
	    vPULSER_Q1.push_back(PULSERdata.q1);
	    break;
#endif
#if HF>0
	  case 5: //--- HF BEAM--- //
	    faster_data_load(group_data,&HFdata);
	    vHF_label.push_back(label);
	    //vHF_clock.push_back(clock);
	    //vHF_tdc.push_back(HFdata.tdc);
	    vHF_time.push_back((double)(clock)+(double)(qdc_conv_dt_ns(HFdata.tdc)));
	    vHF_Q1.push_back(HFdata.q1);
	    break;
#endif
#if MACRO>0
	  case 6: //--- MACRO PULSE --- //
	    faster_data_load(group_data,&MACROdata);
	    vMACRO_label.push_back(label);
	    //vMACRO_clock.push_back(clock);
	    //vMACRO_tdc.push_back(MACROdata.tdc);
	    vMACRO_time.push_back((double)(clock)+(double)(qdc_conv_dt_ns(MACROdata.tdc)));
	    vMACRO_Q1.push_back(MACROdata.q1);
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
      
#if (CHINU_nQ==B3_nQ)
      
#if CHINU_nQ==1
    case QDC_TDC_X1_TYPE_ALIAS:
#elif CHINU_nQ==2
    case QDC_TDC_X2_TYPE_ALIAS:
#elif CHINU_nQ==3
    case QDC_TDC_X3_TYPE_ALIAS:
#else
    case QDC_TDC_X4_TYPE_ALIAS:
#endif
      nevents_source++; cout << nevents_source << endl;
      if(TRIGGER_MODE!=0) cout << "ERROR, QDC_TDC_X"<<CHINU_nQ<<"_TYPE_ALIAS FOUND WITH TRIGGER_MODE = "<< TRIGGER_MODE << " DIFFERENT OF THE EXPECTED 0 VALUE" << endl;
      detType = DetType[label_data-1];
      switch (detType)
	{
#if CHINU>0
	case 2:
	  faster_data_load(data,&CHINUdata);
	  detSubType = DetSubType[label_data-1];
	  switch(detSubType)
	    {
	    case 21: // LOW GAIN 
	      CHINU_multLG[Label2Ch[label_data-1]]++;
	      vCHINUlg_label.push_back(label_data);
	      vCHINUlg_det.push_back(Label2Ch[label_data-1]);
	      //vCHINUlg_clock.push_back(clock_data);
	      //vCHINUlg_tdc.push_back(CHINUdata.tdc);
	      vCHINUlg_time.push_back((double)(clock_data)+(double)(qdc_conv_dt_ns(CHINUdata.tdc)));
	      vCHINUlg_Q1.push_back(CHINUdata.q1);
#if CHINU_nQ>1
	      vCHINUlg_Q2.push_back(CHINUdata.q2);
#endif
#if CHINU_nQ>2
	      vCHINUlg_Q3.push_back(CHINUdata.q3);
#endif
#if CHINU_nQ>3
	      vCHINUlg_Q4.push_back(CHINUdata.q4);
#endif
	      break;
	    case 22: // HIGH GAIN
	      CHINU_multHG[Label2Ch[label_data-1]]++;
	      vCHINUhg_label.push_back(label_data);
	      vCHINUhg_det.push_back(Label2Ch[label_data-1]);
	      //vCHINUhg_clock.push_back(clock_data);
	      //vCHINUhg_tdc.push_back(CHINUdata.tdc);
	      vCHINUhg_time.push_back((double)(clock_data)+(double)(qdc_conv_dt_ns(CHINUdata.tdc)));
	      vCHINUhg_Q1.push_back(CHINUdata.q1);
#if CHINU_nQ>1
	      vCHINUhg_Q2.push_back(CHINUdata.q2);
#endif
#if CHINU_nQ>2
	      vCHINUhg_Q3.push_back(CHINUdata.q3);
#endif
#if CHINU_nQ>3
	      vCHINUhg_Q4.push_back(CHINUdata.q4);
#endif
	      break;
	    default:
	      break;
	    }//end of switch(detSubType)
	  break;
#endif
#if B3>0
	case 3:
	  faster_data_load(data,&B3data);
	  detSubType = DetSubType[label_data-1];
	  switch(detSubType)
	    {
	    case 21: // LOW GAIN 
	      B3_multLG[Label2Ch[label_data-1]]++;
	      vB3lg_label.push_back(label_data);
	      vB3lg_det.push_back(Label2Ch[label_data-1]);
	      //vB3lg_clock.push_back(clock_data);
	      //vB3lg_tdc.push_back(B3data.tdc);
	      vB3lg_time.push_back((double)(clock_data)+(double)(qdc_conv_dt_ns(B3data.tdc)));
	      vB3lg_Q1.push_back(B3data.q1);
#if B3_nQ>1
	      vB3lg_Q2.push_back(B3data.q2);
#endif
#if B3_nQ>2
	      vB3lg_Q3.push_back(B3data.q3);
#endif
#if B3_nQ>3
	      vB3lg_Q4.push_back(B3data.q4);
#endif
	      break;
	    case 22: // HIGH GAIN
	      B3_multHG[Label2Ch[label_data-1]]++;
	      vB3hg_label.push_back(label_data);
	      vB3hg_det.push_back(Label2Ch[label_data-1]);
	      //vB3hg_clock.push_back(clock_data);
	      //vB3hg_tdc.push_back(B3data.tdc);
	      vB3hg_time.push_back((double)(clock_data)+(double)(qdc_conv_dt_ns(B3data.tdc)));
	      vB3hg_Q1.push_back(B3data.q1);
#if B3_nQ>1
	      vB3hg_Q2.push_back(B3data.q2);
#endif
#if B3_nQ>2
	      vB3hg_Q3.push_back(B3data.q3);
#endif
#if B3_nQ>3
	      vB3hg_Q4.push_back(B3data.q4);
#endif
	      break;
	    default:
	      break;
	    }//end of switch(detSubType)
	  break;
#endif
	default:
	  //cout << "ERROR TRIGGERLESS MODE: CHI-NU DETECTOR DATA TYPE (2) OR B3 DETECTOR DATA TYPE (3) EXPECTED BUT GET " << detType << endl;	  
	  break;
	}// end of switch(detType)
      t->Fill();
      break; // END OF TRIGGERLESS DATA FOR CHI-NU AND B3 NEUTRON DETECTORS
      
#else // CHINU_nQ DIFFERS FROM B3_nQ
#if CHINU>0
#if CHINU_nQ==1
    case QDC_TDC_X1_TYPE_ALIAS:
#elif CHINU_nQ==2
    case QDC_TDC_X2_TYPE_ALIAS:
#elif CHINU_nQ==3
    case QDC_TDC_X3_TYPE_ALIAS:
#else
    case QDC_TDC_X4_TYPE_ALIAS:
#endif
      nevents_source++;
      detType = DetType[label_data-1];  
      switch(detType)
	{
	case 2:
	  if(TRIGGER_MODE!=0) cout << "ERROR QDC_TDC_X"<<CHINU_nQ<<"_TYPE_ALIAS FOUND WITH TRIGGER_MODE = "<< TRIGGER_MODE << " DIFFERENT OF THE EXPECTED 0 VALUE" << endl;
	  faster_data_load(data,&CHINUdata);
	  detSubType = DetSubType[label_data-1];
	  switch(detSubType)
	    {
	    case 21: // LOW GAIN 
	      CHINU_multLG[Label2Ch[label_data-1]]++;
	      vCHINUlg_label.push_back(label_data);
	      vCHINUlg_det.push_back(Label2Ch[label_data-1]);
	      //vCHINUlg_clock.push_back(clock_data);
	      //vCHINUlg_tdc.push_back(CHINUdata.tdc);
	      vCHINUlg_time.push_back((double)(clock_data)+(double)(qdc_conv_dt_ns(CHINUdata.tdc)));
	      vCHINUlg_Q1.push_back(CHINUdata.q1);
#if CHINU_nQ>1
	      vCHINUlg_Q2.push_back(CHINUdata.q2);
#endif
#if CHINU_nQ>2
	      vCHINUlg_Q3.push_back(CHINUdata.q3);
#endif
#if CHINU_nQ>3
	      vCHINUlg_Q4.push_back(CHINUdata.q4);
#endif
	      break;
	    case 22: // HIGH GAIN
	      CHINU_multHG[Label2Ch[label_data-1]]++;
	      vCHINUhg_label.push_back(label_data);
	      vCHINUhg_det.push_back(Label2Ch[label_data-1]);
	      //vCHINUhg_clock.push_back(clock_data);
	      //vCHINUhg_tdc.push_back(CHINUdata.tdc);
	      vCHINUhg_time.push_back((double)(clock_data)+(double)(qdc_conv_dt_ns(CHINUdata.tdc)));
	      vCHINUhg_Q1.push_back(CHINUdata.q1);
#if CHINU_nQ>1
	      vCHINUhg_Q2.push_back(CHINUdata.q2);
#endif
#if CHINU_nQ>2
	      vCHINUhg_Q3.push_back(CHINUdata.q3);
#endif
#if CHINU_nQ>3
	      vCHINUhg_Q4.push_back(CHINUdata.q4);
#endif
	      break;
	    default:
	      break;
	    }//end of switch(detSubType)
	  
	  break;
	default:
	  //cout << "ERROR TRIGGERLESS MODE: CHI-NU DETECTOR TYPE DATA EXPECTED (2) BUT GET " << detType << endl;
	  break;
	}// end of switch(detType)
      t->Fill();
      break; // END OF TRIGGERLESS DATA FROM CHI-NU NDET
#endif
#if B3>0
#if B3_nQ==1
    case QDC_TDC_X1_TYPE_ALIAS:
#elif B3_nQ==2
    case QDC_TDC_X2_TYPE_ALIAS:
#elif B3_nQ==3
    case QDC_TDC_X3_TYPE_ALIAS:
#else
    case QDC_TDC_X4_TYPE_ALIAS:
#endif
      nevents_source++;
      detType = DetType[label_data-1];    
      switch(detType)
	{
	case 3:
	  if(TRIGGER_MODE!=0) cout << "ERROR QDC_TDC_X"<<B3_nQ<<"_TYPE_ALIAS FOUND WITH TRIGGER_MODE = "<< TRIGGER_MODE << " DIFFERENT OF THE EXPECTED 0 VALUE" << endl;
	  faster_data_load(data,&B3data);
	  detSubType = DetSubType[label_data-1];
	  switch(detSubType)
	    {
	    case 21: // LOW GAIN 
	      B3_multLG[Label2Ch[label_data-1]]++;
	      vB3lg_label.push_back(label_data);
	      vB3lg_det.push_back(Label2Ch[label_data-1]);
	      //vB3lg_clock.push_back(clock_data);
	      //vB3lg_tdc.push_back(B3data.tdc);
	      vB3lg_time.push_back((double)(clock_data)+(double)(qdc_conv_dt_ns(B3data.tdc)));
	      vB3lg_Q1.push_back(B3data.q1);
#if B3_nQ>1
	      vB3lg_Q2.push_back(B3data.q2);
#endif
#if B3_nQ>2
	      vB3lg_Q3.push_back(B3data.q3);
#endif
#if B3_nQ>3
	      vB3lg_Q4.push_back(B3data.q4);
#endif
	      break;
	    case 22: // HIGH GAIN
	      B3_multHG[Label2Ch[label_data-1]]++;
	      vB3hg_label.push_back(label_data);
	      vB3hg_det.push_back(Label2Ch[label_data-1]);
	      //vB3hg_clock.push_back(clock_data);
	      //vB3hg_tdc.push_back(B3data.tdc);
	      vB3hg_time.push_back((double)(clock_data)+(double)(qdc_conv_dt_ns(B3data.tdc)));
	      vB3hg_Q1.push_back(B3data.q1);
#if B3_nQ>1
	      vB3hg_Q2.push_back(B3data.q2);
#endif
#if B3_nQ>2
	      vB3hg_Q3.push_back(B3data.q3);
#endif
#if B3_nQ>3
	      vB3hg_Q4.push_back(B3data.q4);
#endif
	      break;
	    default:
	      break;
	    }//end of switch(detSubType)
	  break;
	default:
	  //cout << "TRIGGERLESS MODE: B3 DETECTOR TYPE DATA EXPECTED (3) BUT GET " << detType << endl;
	  break;
	}//end of switch(detType)
      t->Fill();
      break; // END OF TRIGGERLESS DATA FOR B3 NDET
#endif
      
#endif // COMPARISON OF NDET_nQ AND B3_nQ FOR TRIGGERLESS MODE
      
    default: 
      break;
    }// end of switch(alias)
    
    

  }//end of while data


  // === ======================== === //
  // === CLOSE FASTER FILE READER === //
  // === ======================== === //
  faster_file_reader_close (file_reader);


  // === ====================== === //
  // === SAVE TREE IN ROOT FILE === //
  // === ====================== === //
  fsave->cd();
  t->Write();
  fsave->ls();
  fsave->Close();

  // === =============== === //
  // === SOME STATISTICS === //
  // === =============== === //
  cout << "TOTAL NUMBER OF EVENTS: " << nevents << endl;
  cout << "  . COINC NUMBER OF EVENTS: " << nevents_coinc << endl;
  cout << "  . SOURCE NUMBER OF EVENTS: " << nevents_source << endl;
  cout << "  . COUNTERS NUMBER OF EVENTS: " << nevents_counters << endl;

}//end of main
