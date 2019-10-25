//  std includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

//  root includes
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

//  include class of data format
#include "../ClassDef/RawCoincData.h"

//  include specific 
#include "../RunRefs/RunRefs.h"
#include "../Faster2Root/ReadPid/pid.h"
#include "../../SetupSpecific/setup_specific.h"
#if FC>0 && CHINU>0
#include "../../SetupSpecific/FC_to_CHINUlg_GammaPeak.h"
#include "../../SetupSpecific/FC_to_CHINUhg_GammaPeak.h"
#endif
#if FC>0 && B3>0
#include "../../SetupSpecific/FC_to_B3lg_GammaPeak.h"
#include "../../SetupSpecific/FC_to_B3hg_GammaPeak.h"
#endif




void run(UInt_t runFirst, UInt_t runLast, TString dataType, TString dirpath)
{
  
  // === ================ === //
  // === TYPE OF THE DATA === //
  // === ================ === //
  
  Char_t fType=0;
  if(dataType.Contains("U")) {
    cout<<"URANIUM DATA"<<endl;
    fType=1;
  }
  else if(dataType.Contains("Cf")) {
    cout<<"CALIFORNIUM DATA"<<endl;
    fType=2;
  }
  else if(dataType.Contains("BdF")) {
    cout<<"BACKGROUND DATA"<<endl;
    cout<<"ERROR ERROR ERROR ERROR : use the 01b_GenerateRawSourceHistos" << endl;
    fType=3;
  }
  else if(dataType.Contains("Cs")){
    cout<<"CESIUM DATA"<<endl;
    cout<<"ERROR ERROR ERROR ERROR : use the 01b_GenerateRawSourceHistos" << endl;
    fType=4;
  }
  else{
    cout << "DATA TYPE " << dataType.Data() << " NOT RECOGNIZED" << endl;
  }

  // === ========== === //
  // === INPUT DATA === //
  // === ========== === //

  TChain * ch = new TChain("RawCoincData");
  Int_t rankFirst=-1;
  Int_t rankLast=-1;
  UInt_t nRuns=0;
  TString filename;
  switch(fType){
  case 1:
#if U_NRUNS>0
    for(UInt_t i=0; i<U_NRUNS;i++){
      if(U_numRun[i]==runFirst)	rankFirst=i;
      if(U_numRun[i]==runLast)	rankLast=i;
    }// end of loop over the uranium runs
    if(rankFirst==-1) cout << "runFirst " << runFirst << " not found in U_numRun["<< U_NRUNS<< "]. is RunRefs.h up to date ?" << endl;
    if(rankLast==-1) cout << "runLast " << runLast << " not found in U_numRun["<< U_NRUNS<< "]. is RunRefs.h up to date ?" << endl;
    if(rankLast<rankFirst) cout << "ERROR: runFirst and runLast are not ordered in RunRefs.h" << endl;
    else nRuns=rankLast-rankFirst+1;
    cout << "Data will be loaded from the " << dirpath << " directory" << endl;
    for(Int_t i=rankFirst;i<=rankLast;i++){
      for(UInt_t j=1; j<=U_nFiles[i];j++){
	filename = Form("%s/Run%d_%04d_U.root",dirpath.Data(),U_numRun[i],j);
	cout << filename << endl;
	ch->Add(filename);
      }
    }
#endif
    break;
  case 2:
#if Cf_NRUNS>0
    ch = new TChain("RawCoincData");
    for(UInt_t i=0; i<Cf_NRUNS;i++){
      if(Cf_numRun[i]==runFirst)	rankFirst=i;
      if(Cf_numRun[i]==runLast)	rankLast=i;
    }// end of loop over the uranium runs
    if(rankFirst==-1) cout << "runFirst " << runFirst << " not found in Cf_numRun["<< Cf_NRUNS<< "]. is RunRefs.h up to date ?" << endl;
    if(rankLast==-1) cout << "runLast " << runLast << " not found in Cf_numRun["<< Cf_NRUNS<< "]. is RunRefs.h up to date ?" << endl;
    if(rankLast<rankFirst) cout << "ERROR: runFirst and runLast are not ordered in RunRefs.h" << endl;
    else nRuns=rankLast-rankFirst+1;
    cout << "Data will be loaded from the " << dirpath << " directory" << endl;
    for(Int_t i=rankFirst;i<=rankLast;i++){
      for(UInt_t j=1; j<=Cf_nFiles[i];j++){
	filename = Form("%s/Run%d_%04d_Cf.root",dirpath.Data(),Cf_numRun[i],j);
	cout << filename << endl;
	ch->Add(filename);
      }
    }
#endif
    break;

  default:
    cout << "Data type " << fType << " unrecognized" << endl;
    break;
  }
  ch->ls();
  RawCoincData raw(ch);
  ULong64_t nentries = ch->GetEntries();
  
 
  // === ========================================== === //
  // === OUTPUT DATA CONCERNING THE TIMES oF FLIGHT === //
  // === ========================================== === //


  // --- BEAM
#if HF>0
#if FC>0
  std::vector<UShort_t> vBEAM_FCanode; 
  std::vector<Double_t> vBEAM_ToFraw; // NEUTRON BEAM ToF t(FC)-t(HF)
  std::vector<Double_t> vBEAM_ToF;    // NEUTRON BEAM ToF
#endif
#if PULSER>0
  std::vector<Double_t> vBkgBEAM_ToFraw; // Background NEUTRON BEAM ToF t(PULSER)-t(HF)
  std::vector<Double_t> vBkgBEAM_ToF;    // Background NEUTRON BEAM ToF 
#endif
#endif

  // --- PROMT FISSION NEUTRONS...
  // --- ... DETECTED BY CHI-NU
#if CHINU>0
#if FC>0
  std::vector<UShort_t> vPFN_CHINUlg_det; 
  std::vector<UShort_t> vPFN_CHINUlg_anode; 
  std::vector<Double_t> vPFN_CHINUlg_ToFraw;    // PFN ToF t(CHINU) - t(FC)
  std::vector<Double_t> vPFN_CHINUlg_ToFcal;    // PFN ToF
  std::vector<UShort_t> vPFN_CHINUhg_det; 
  std::vector<UShort_t> vPFN_CHINUhg_anode; 
  std::vector<Double_t> vPFN_CHINUhg_ToFraw;    // PFN ToF t(CHINU) - t(FC)
  std::vector<Double_t> vPFN_CHINUhg_ToFcal;    // PFN ToF
#endif
#if PULSER>0
  std::vector<UShort_t> vBkgPFN_CHINUlg_det; 
  std::vector<Double_t> vBkgPFN_CHINUlg_ToFraw;    // PFN ToF t(CHINU) - t(FC)
  std::vector<Double_t> vBkgPFN_CHINUlg_ToFcal;    // PFN ToF
  std::vector<UShort_t> vBkgPFN_CHINUhg_det; 
  std::vector<Double_t> vBkgPFN_CHINUhg_ToFraw;    // PFN ToF t(CHINU) - t(FC)
  std::vector<Double_t> vBkgPFN_CHINUhg_ToFcal;    // PFN ToF
#endif
#endif
  // --- ... DETECTED BY B3 DETECTORS
#if B3>0
#if FC>0
  std::vector<UShort_t> vPFN_B3lg_det; 
  std::vector<UShort_t> vPFN_B3lg_anode; 
  std::vector<Double_t> vPFN_B3lg_ToFraw;    // PFN ToF t(B3) - t(FC)
  std::vector<Double_t> vPFN_B3lg_ToFcal;    // PFN ToF
  std::vector<UShort_t> vPFN_B3hg_det; 
  std::vector<UShort_t> vPFN_B3hg_anode; 
  std::vector<Double_t> vPFN_B3hg_ToFraw;    // PFN ToF t(B3) - t(FC)
  std::vector<Double_t> vPFN_B3hg_ToFcal;    // PFN ToF
#endif
#if PULSER>0
  std::vector<UShort_t> vBkgPFN_B3lg_det; 
  std::vector<Double_t> vBkgPFN_B3lg_ToFraw;    // PFN ToF t(B3) - t(FC)
  std::vector<Double_t> vBkgPFN_B3lg_ToFcal;    // PFN ToF
  std::vector<UShort_t> vBkgPFN_B3hg_det; 
  std::vector<Double_t> vBkgPFN_B3hg_ToFraw;    // PFN ToF t(B3) - t(FC)
  std::vector<Double_t> vBkgPFN_B3hg_ToFcal;    // PFN ToF
#endif
#endif


  // === =========== === //
  // === OUTPUT TREE === //
  // === =========== === //

  TString RunNumber=argv[3];
  TString FileNumber=argv[4];
  sprintf(name,"Run %d File %d - SORT DATA - MULT NDET>0 AND MULT FC==1",RunNumber.Atoi(),FileNumber.Atoi());
  TTree * t = new TTree("SortData",name);

  // --- ------------------ --- //
  // --- KEEP SOME RAW DATA --- //
  // --- ------------------ --- //
  // general variables
  t->Branch("Event",&raw.nevents,"Event/i");
  t->Branch("Label",&raw.label_data,"Label/i");
  // chi-nu ndet 
#if CHINU>0
  t->Branch("CHINUlg_mult",raw.CHINU_multLG,"CHINUlg_mult[54]/s");
  t->Branch("CHINUhg_mult",raw.CHINU_multHG,"CHINUhg_mult[54]/s");
  t->Branch("vCHINUlg_det",&raw.vCHINUlg_det);
  t->Branch("vCHINUhg_det",&raw.vCHINUhg_det);
  t->Branch("vCHINUlg_time",&raw.vCHINUlg_time);
  t->Branch("vCHINUhg_time",&raw.vCHINUhg_time);
  t->Branch("vCHINUlg_Q1",&raw.vCHINUlg_Q1);
  t->Branch("vCHINUhg_Q1",&raw.vCHINUhg_Q1);
#if CHINU_nQ>1
  t->Branch("vCHINUlg_Q2",&raw.vCHINUlg_Q2);
  t->Branch("vCHINUhg_Q2",&raw.vCHINUhg_Q2);
#endif
#if CHINU_nQ>2
  t->Branch("vCHINUlg_Q3",&raw.vCHINUlg_Q3);
  t->Branch("vCHINUhg_Q3",&raw.vCHINUhg_Q3);
#endif
#if CHINU_nQ>3
  t->Branch("vCHINUlg_Q4",&raw.vCHINUlg_Q4);
  t->Branch("vCHINUhg_Q4",&raw.vCHINUhg_Q4);
#endif
#endif
  // B3 ndet 
#if B3>0
  t->Branch("B3lg_mult",raw.B3_multLG,"B3lg_mult[4]/s");
  t->Branch("B3hg_mult",raw.B3_multHG,"B3hg_mult[4]/s");
  t->Branch("vB3lg_det",&raw.vB3lg_det);
  t->Branch("vB3hg_det",&raw.vB3hg_det);
  t->Branch("vB3lg_time",&raw.vB3lg_time);
  t->Branch("vB3hg_time",&raw.vB3hg_time);
  t->Branch("vB3lg_Q1",&raw.vB3lg_Q1);
  t->Branch("vB3hg_Q1",&raw.vB3hg_Q1);
#if B3_nQ>1
  t->Branch("vB3lg_Q2",&raw.vB3lg_Q2);
  t->Branch("vB3hg_Q2",&raw.vB3hg_Q2);
#endif
#if B3_nQ>2
  t->Branch("vB3lg_Q3",&raw.vB3lg_Q3);
  t->Branch("vB3hg_Q3",&raw.vB3hg_Q3);
#endif
#if B3_nQ>3
  t->Branch("vB3lg_Q4",&raw.vB3lg_Q4);
  t->Branch("vB3hg_Q4",&raw.vB3hg_Q4);
#endif
#endif
#if FC>0
  t->Branch("anode",&raw.FC_anode[0],"FC_anode/s");
  t->Branch("FC_time",&raw.FC_time[0],"FC_time/l");
  t->Branch("FC_Q1",&raw.FC_Q1[0],"FC_Q1/i");
#if FC_nQ>1
  t->Branch("FC_Q2",&raw.FC_Q2[0],"FC_Q2/i");
#endif
#if FC_nQ>2
  t->Branch("FC_Q3",&raw.FC_Q3[0],"FC_Q3/i");
#endif
#if FC_nQ>3
  t->Branch("FC_Q4",&raw.FC_Q4[0],"FC_Q4/i");
#endif
#endif
  // Pulser
#if PULSER>0
  t->Branch("PULSER_time",&raw.PULSER_time[0],"PULSER_time/l");
  t->Branch("PULSER_Q1",&raw.PULSER_Q1[0],"PULSER_Q1/i");
#endif
  // HF Beam
#if HF>0
  t->Branch("vHF_time",&raw.vHF_time[0],"HF_time/l");
  t->Branch("vHF_Q1",&raw.vHF_Q1[0],"HF_Q1/i");
#endif
  // Macro Pulse
#if MACRO>0
  t->Branch("vMACRO_time",&raw.vMACRO_time[0],"MACRO_time\l");
  t->Branch("vMACRO_Q1",&raw.vMACRO_Q1[0],"MACRO_Q1\i");
#endif
  // --- -------------------- --- //
  // --- Times of Flight data --- //
  // --- -------------------- --- //
  // --- PROMT FISSION NEUTRONS DETECTED BY CHI-NU
#if CHINU>0
#if FC>0
  t->Branch("vPFN_CHINUlg_det",&vPFN_CHINUlg_det); 
  t->Branch("vPFN_CHINUlg_ToFraw",&vPFN_CHINUlg_ToFraw);    // PFN ToF t(CHINU) - t(FC)
  t->Branch("vPFN_CHINUlg_ToFcal",&vPFN_CHINUlg_ToFcal);    // PFN ToF
  t->Branch("vPFN_CHINUhg_det",&vPFN_CHINUhg_det); 
  t->Branch("vPFN_CHINUhg_ToFraw",&vPFN_CHINUhg_ToFraw);    // PFN ToF t(CHINU) - t(FC)
  t->Branch("vPFN_CHINUhg_ToFcal",&vPFN_CHINUhg_ToFcal);    // PFN ToF
#endif
#if PULSER>0
  t->Branch("vBkgPFN_CHINUlg_det",&vBkgPFN_CHINUlg_det); 
  t->Branch("vBkgPFN_CHINUlg_ToFraw",&vBkgPFN_CHINUlg_ToFraw);    // PFN ToF t(CHINU) - t(FC)
  t->Branch("vBkgPFN_CHINUlg_ToFcal",&vBkgPFN_CHINUlg_ToFcal);    // PFN ToF
  t->Branch("vBkgPFN_CHINUhg_det",&vBkgPFN_CHINUhg_det); 
  t->Branch("vBkgPFN_CHINUhg_ToFraw",&vBkgPFN_CHINUhg_ToFraw);    // PFN ToF t(CHINU) - t(FC)
  t->Branch("vBkgPFN_CHINUhg_ToFcal",&vBkgPFN_CHINUhg_ToFcal);    // PFN ToF
#endif
#endif
  // --- PROMPT FISSION NEUTRONS DETECTED BY B3 DETECTORS
#if B3>0
#if FC>0
  t->Branch("vPFN_B3lg_det",&vPFN_B3lg_det); 
  t->Branch("vPFN_B3lg_ToFraw",&vPFN_B3lg_ToFraw);    // PFN ToF t(B3) - t(FC)
  t->Branch("vPFN_B3lg_ToFcal",&vPFN_B3lg_ToFcal);    // PFN ToF
  t->Branch("vPFN_B3hg_det",&vPFN_B3hg_det); 
  t->Branch("vPFN_B3hg_ToFraw",&vPFN_B3hg_ToFraw);    // PFN ToF t(B3) - t(FC)
  t->Branch("vPFN_B3hg_ToFcal",&vPFN_B3hg_ToFcal);    // PFN ToF
#endif
#if PULSER>0
  t->Branch("vBkgPFN_B3lg_det",&vBkgPFN_B3lg_det); 
  t->Branch("vBkgPFN_B3lg_ToFraw",&vBkgPFN_B3lg_ToFraw);    // PFN ToF t(B3) - t(FC)
  t->Branch("vBkgPFN_B3lg_ToFcal",&vBkgPFN_B3lg_ToFcal);    // PFN ToF
  t->Branch("vBkgPFN_B3hg_det",&vBkgPFN_B3lg_det); 
  t->Branch("vBkgPFN_B3hg_ToFraw",&vBkgPFN_B3lg_ToFraw);    // PFN ToF t(B3) - t(FC)
  t->Branch("vBkgPFN_B3hg_ToFcal",&vBkgPFN_B3lg_ToFcal);    // PFN ToF
#endif
#endif
  // --- NEUTRON BEAM
#if HF>0
#if FC>0
  t->Branch("vBEAM_ToFraw",&vBEAM_ToFraw); // NEUTRON BEAM ToF t(FC)-t(HF)
  t->Branch("vBEAM_ToF",&vBEAM_ToF);       // NEUTRON BEAM ToF
#endif
#if PULSER>0
  t->Branch("vBkgBEAM_ToFraw",&vBkgBEAM_ToFrawm); // Background NEUTRON BEAM ToF t(PULSER)-t(HF)
  t->Branch("vBkgBEAM_ToF",&vBkgBEAM_ToF);        // Background NEUTRON BEAM ToF 
#endif
#endif





  
}
