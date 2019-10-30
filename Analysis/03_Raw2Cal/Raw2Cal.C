//  std includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "math.h"

//  root includes
#include "TCutG.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

//  include class of data format
#include "../ClassDef/RawCoincData.h"

//  include specific 
#include "../RunRefs/RunRefs.h"
#include "../../Faster2Root/ReadPid/pid.h"
#include "../../SetupSpecific/setup_specific.h"
#if FC>0 && CHINU>0
#include "../../SetupSpecific/CHINUlg_OffsetToF.h"
#include "../../SetupSpecific/CHINUhg_OffsetToF.h"
#endif
#if FC>0 && B3>0
#include "../../SetupSpecific/B3lg_OffsetToF.h"
#include "../../SetupSpecific/B3hg_OffsetToF.h"
#endif

#include "../ClassDef/CHINU_RawQHistos.h"
#include "../ClassDef/B3_RawQHistos.h"





Double_t Beta(double v_mns)
{
  return ( v_mns / SPEED_OF_LIGHT_MNS ) ;
}

Double_t Gamma(Double_t beta){
  return ( 1.0 / sqrt(1.0 - pow(beta,2)) ) ;
}

Double_t Velocity2Ene(double_t v_mns)
{
  double BETA = Beta(v_mns);
  double GAMMA = Gamma(BETA);
  return (NEUTRON_MASS_MeV * (GAMMA - 1));
}



void run(UInt_t runFirst, UInt_t runLast, TString dataType, TString dirpath)
{

  char name[500];
  
  // === ========================== === //
  // === ALPHA / NEUTRON TCutG FILE === //
  // === ========================== === //
  sprintf(name,"../02_AlphaDiscri_n_GammaPeak/cut/cutFC_Q2vsQ1_R102.root");
  cout <<"TCutG FOR ALPHA DISCRI IS : " << name << endl;
  TFile * cutfile = new TFile(name,"read");
  TCutG * cut[FC_nAnodes];
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    sprintf(name,"cut_Q2vsQ1_neutron_FC_%i",anode);
    cut[anode-1] = (TCutG*)cutfile->Get(name);
    cut[anode-1]->ls();
  }
  cutfile->Close();
  
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
  
 
  // === ===================================================== === //
  // === OUTPUT DATA CONCERNING THE TIMES oF FLIGHT AND ENERGY === //
  // === ===================================================== === //
  UInt_t Event;

  // --- BEAM
  // TO DO if HF
  // TO DO IF HF AND PULSER

#if FC>0
  UShort_t FC_anode;
  UInt_t   FC_Q1;
  UInt_t   FC_Q2;
#endif

  // --- PROMT FISSION NEUTRONS...
  // --- ... DETECTED BY CHI-NU
#if CHINU>0
  std::vector<unsigned short> vCHINUlg_det;
  std::vector<unsigned short> vCHINUhg_det;
  std::vector<unsigned int> vCHINUlg_Q1;
  std::vector<unsigned int> vCHINUhg_Q1;
  std::vector<unsigned int> vCHINUlg_Q2;
  std::vector<unsigned int> vCHINUhg_Q2;
#if FC>0
  std::vector<Double_t> vPFN_CHINUlg_ToF;    // PFN ToF t(CHINU) - t(FC)
  std::vector<Bool_t>   vPFN_CHINUlg_IsGammaToF;    
  std::vector<Bool_t>   vPFN_CHINUlg_IsGammaQdiscri;    
  std::vector<Double_t> vPFN_CHINUlg_Ene;    // PFN Energy
  std::vector<Double_t> vPFN_CHINUhg_ToF;    // PFN ToF t(CHINU) - t(FC)
  std::vector<Bool_t>   vPFN_CHINUhg_IsGammaToF;    
  std::vector<Bool_t>   vPFN_CHINUhg_IsGammaQdiscri;    
  std::vector<Double_t> vPFN_CHINUhg_Ene;    // PFN Energy
#endif
  // TO DO IF PULSER
#endif
  // --- ... DETECTED BY B3 DETECTORS
#if B3>0
  std::vector<unsigned short> vB3lg_det;
  std::vector<unsigned short> vB3hg_det;
  std::vector<unsigned int> vB3lg_Q1;
  std::vector<unsigned int> vB3hg_Q1;
  std::vector<unsigned int> vB3lg_Q2;
  std::vector<unsigned int> vB3hg_Q2;
#if FC>0
  std::vector<Double_t> vPFN_B3lg_ToF;    // PFN ToF t(B3) - t(FC)
  std::vector<Bool_t>   vPFN_B3lg_IsGammaToF;    
  std::vector<Bool_t>   vPFN_B3lg_IsGammaQdiscri;    
  std::vector<Double_t> vPFN_B3lg_Ene;    // PFN ToF
  std::vector<Double_t> vPFN_B3hg_ToF;    // PFN ToF t(B3) - t(FC)
  std::vector<Bool_t>   vPFN_B3hg_IsGammaToF;    
  std::vector<Bool_t>   vPFN_B3hg_IsGammaQdiscri;    
  std::vector<Double_t> vPFN_B3hg_Ene;    // PFN ToF
#endif
  // TO DO IF PULSER
#endif


  // === =========== === //
  // === OUTPUT TREE === //
  // === =========== === //
  sprintf(name,"../../data/Cal/Cf252/R%d_to_R%d.root",runFirst,runLast);
  TFile * fsaveTree = new TFile(name,"recreate");
  fsaveTree->ls();
  sprintf(name,"Run %d File %d - Cal Data - ATTENTION ONLY EVENTS WITH MULT NDET>0",runFirst,runLast);
  TTree * t = new TTree("CalData",name);

  // --- ------------------ --- //
  // --- KEEP SOME RAW DATA --- //
  // --- ------------------ --- //
  // general variables
  t->Branch("Event",&Event,"Event/i");

  // fission chamber
#if FC>0
  t->Branch("FC_anode",&FC_anode,"FC_anode/s");
  t->Branch("FC_Q1",&FC_Q1,"FC_Q1/s");
  t->Branch("FC_Q2",&FC_Q2,"FC_Q2/s");
#endif

  // chi-nu ndet 
#if CHINU>0
  t->Branch("vCHINUlg_det",&vCHINUlg_det);
  t->Branch("vCHINUhg_det",&vCHINUhg_det);
  t->Branch("vCHINUlg_Q1",&vCHINUlg_Q1);
  t->Branch("vCHINUhg_Q1",&vCHINUhg_Q1);
  t->Branch("vCHINUlg_Q2",&vCHINUlg_Q2);
  t->Branch("vCHINUhg_Q2",&vCHINUhg_Q2);
#if FC>0
  t->Branch("vPFN_CHINUlg_ToF",&vPFN_CHINUlg_ToF); //calibrated ToF
  t->Branch("vPFN_CHINUhg_ToF",&vPFN_CHINUhg_ToF);
  t->Branch("vPFN_CHINUlg_IsGammaToF",&vPFN_CHINUlg_IsGammaToF);
  t->Branch("vPFN_CHINUhg_IsGammaToF",&vPFN_CHINUhg_IsGammaToF);
  t->Branch("vPFN_CHINUlg_IsGammaQdiscri",&vPFN_CHINUlg_IsGammaQdiscri);
  t->Branch("vPFN_CHINUhg_IsGammaQdiscri",&vPFN_CHINUhg_IsGammaQdiscri);
  t->Branch("vPFN_CHINUlg_Ene",&vPFN_CHINUlg_Ene);
  t->Branch("vPFN_CHINUhg_Ene",&vPFN_CHINUhg_Ene);
#endif
#endif

  // B3 ndet 
#if B3>0
  t->Branch("vB3lg_det",&vB3lg_det);
  t->Branch("vB3hg_det",&vB3hg_det);
  t->Branch("vB3lg_Q1",&vB3lg_Q1);
  t->Branch("vB3hg_Q1",&vB3hg_Q1);
  t->Branch("vB3lg_Q2",&vB3lg_Q2);
  t->Branch("vB3hg_Q2",&vB3hg_Q2);
#if FC>0
  t->Branch("vPFN_B3lg_ToF",&vPFN_B3lg_ToF);
  t->Branch("vPFN_B3hg_ToF",&vPFN_B3hg_ToF);
  t->Branch("vPFN_B3lg_IsGammaToF",&vPFN_B3lg_IsGammaToF);
  t->Branch("vPFN_B3hg_IsGammaToF",&vPFN_B3hg_IsGammaToF);
  t->Branch("vPFN_B3lg_IsGammaQdiscri",&vPFN_B3lg_IsGammaQdiscri);
  t->Branch("vPFN_B3hg_IsGammaQdiscri",&vPFN_B3hg_IsGammaQdiscri);
  t->Branch("vPFN_B3lg_Ene",&vPFN_B3lg_Ene);
  t->Branch("vPFN_B3hg_Ene",&vPFN_B3hg_Ene);
#endif
#endif


  // === ===================== === //
  // === LOOP OVER THE ENTRIES === //
  // === ===================== === //
  Double_t ToF, Velocity, Ene;
  //for(ULong64_t Entry=0; Entry<20000000; Entry++){
  for(ULong64_t Entry=0; Entry<nentries; Entry++){
    
    // --- ----------------------------------------------------- --- //
    // --- Initialization of the vector dedicated to ToF and Ene --- //
    // --- ----------------------------------------------------- --- //
    Event=0;
    FC_anode=0;
    FC_Q1=0;
    FC_Q2=0;
    vCHINUlg_det.clear();
    vCHINUhg_det.clear();
    vCHINUlg_Q1.clear();
    vCHINUhg_Q1.clear();
    vCHINUlg_Q2.clear();
    vCHINUhg_Q2.clear();
    vPFN_CHINUlg_ToF.clear();
    vPFN_CHINUhg_ToF.clear();
    vPFN_CHINUlg_IsGammaToF.clear();
    vPFN_CHINUhg_IsGammaToF.clear();
    vPFN_CHINUlg_IsGammaQdiscri.clear();
    vPFN_CHINUhg_IsGammaQdiscri.clear();
    vPFN_CHINUlg_Ene.clear();
    vPFN_CHINUhg_Ene.clear();
    vB3lg_det.clear();
    vB3hg_det.clear();
    vB3lg_Q1.clear();
    vB3hg_Q1.clear();
    vB3lg_Q2.clear();
    vB3hg_Q2.clear();
    vPFN_B3lg_ToF.clear();
    vPFN_B3hg_ToF.clear();
    vPFN_B3lg_IsGammaToF.clear();
    vPFN_B3hg_IsGammaToF.clear();
    vPFN_B3lg_IsGammaQdiscri.clear();
    vPFN_B3hg_IsGammaQdiscri.clear();
    vPFN_B3lg_Ene.clear();
    vPFN_B3hg_Ene.clear();

    // --- ----------------------------- --- //
    // --- Read each entry of the TChain --- //
    // --- ----------------------------- --- //
    raw.GetEntry(Entry);
    if((Entry%1000000)==0) cout << Entry << endl;

    if((raw.vB3lg_det->size()==0) && (raw.vB3hg_det->size()==0) && (raw.vCHINUlg_det->size()==0) && (raw.vCHINUhg_det->size()==0)) continue;
    if (raw.vFC_anode->size()!=1) continue;
    if(!cut[raw.vFC_anode->at(0)-1]->IsInside(raw.vFC_Q1->at(0),raw.vFC_Q2->at(0))) continue;
    
    Event = raw.Event;
    FC_anode = raw.vFC_anode->at(0);
    FC_Q1 = raw.vFC_Q1->at(0);
    FC_Q2 = raw.vFC_Q2->at(0);
    
    for(UShort_t m=0; m<raw.vCHINUlg_det->size(); m++) {
      vCHINUlg_det.push_back(raw.vCHINUlg_det->at(m));
      vCHINUlg_Q1.push_back(raw.vCHINUlg_Q1->at(m));
      vCHINUlg_Q2.push_back(raw.vCHINUlg_Q2->at(m));
      ToF = (raw.vCHINUlg_time->at(m)-raw.vFC_time->at(0))+CHINU_OffsetToFLG[raw.vCHINUlg_det->at(m)-1+CHINU_nDets*(FC_anode-1)];
      vPFN_CHINUlg_ToF.push_back(ToF);
      Velocity = Distance_FC_ChiNu[raw.vCHINUlg_det->at(m)-1] / ToF;
      Ene = Velocity2Ene(Velocity);
      vPFN_CHINUlg_Ene.push_back(Ene);
      if((CHINU_GammaToFmin<=ToF) && (ToF<=CHINU_GammaToFmax)) 	
	vPFN_CHINUlg_IsGammaToF.push_back(kTRUE);
      else 	
	vPFN_CHINUlg_IsGammaToF.push_back(kFALSE);
    }
    for(UShort_t m=0; m<raw.vCHINUhg_det->size(); m++){
      vCHINUhg_det.push_back(raw.vCHINUhg_det->at(m));
      vCHINUhg_Q1.push_back(raw.vCHINUhg_Q1->at(m));
      vCHINUhg_Q2.push_back(raw.vCHINUhg_Q2->at(m));
      ToF = (raw.vCHINUhg_time->at(m)-raw.vFC_time->at(0))+CHINU_OffsetToFHG[raw.vCHINUhg_det->at(m)-1+CHINU_nDets*(FC_anode-1)];
      vPFN_CHINUhg_ToF.push_back(ToF);
      Velocity = Distance_FC_ChiNu[raw.vCHINUhg_det->at(m)-1] / ToF;
      Ene = Velocity2Ene(Velocity);
      vPFN_CHINUhg_Ene.push_back(Ene);
      if((CHINU_GammaToFmin<=ToF) && (ToF<=CHINU_GammaToFmax)) 
	vPFN_CHINUhg_IsGammaToF.push_back(kTRUE);
      else 
	vPFN_CHINUhg_IsGammaToF.push_back(kFALSE);

    }
    
    for(UShort_t m=0; m<raw.vB3lg_det->size(); m++) {
      vB3lg_det.push_back(raw.vB3lg_det->at(m));
      vB3lg_Q1.push_back(raw.vB3lg_Q1->at(m));
      vB3lg_Q2.push_back(raw.vB3lg_Q2->at(m));
      ToF = (raw.vB3lg_time->at(m)-raw.vFC_time->at(0))+B3_OffsetToFLG[raw.vB3lg_det->at(m)-1+B3_nDets*(FC_anode-1)];
      vPFN_B3lg_ToF.push_back(ToF);
      Velocity = Distance_FC_B3[raw.vB3lg_det->at(m)-1] / ToF;
      Ene = Velocity2Ene(Velocity);
      vPFN_B3lg_Ene.push_back(Ene);
      if((B3_GammaToFmin<=ToF) && (ToF<=B3_GammaToFmax)) 
	vPFN_B3lg_IsGammaToF.push_back(kTRUE);
      else 
	vPFN_B3lg_IsGammaToF.push_back(kFALSE);

    }
    for(UShort_t m=0; m<raw.vB3hg_det->size(); m++){
      vB3hg_det.push_back(raw.vB3hg_det->at(m));
      vB3hg_Q1.push_back(raw.vB3hg_Q1->at(m));
      vB3hg_Q2.push_back(raw.vB3hg_Q2->at(m));
      ToF = (raw.vB3hg_time->at(m)-raw.vFC_time->at(0))+B3_OffsetToFHG[raw.vB3hg_det->at(m)-1+B3_nDets*(FC_anode-1)];
      vPFN_B3hg_ToF.push_back(ToF);
      Velocity = Distance_FC_B3[raw.vB3hg_det->at(m)-1] / ToF;
      Ene = Velocity2Ene(Velocity);
      vPFN_B3hg_Ene.push_back(Ene);
      if((B3_GammaToFmin<=ToF) && (ToF<=B3_GammaToFmax)) 
	vPFN_B3hg_IsGammaToF.push_back(kTRUE);
      else 
	vPFN_B3hg_IsGammaToF.push_back(kFALSE);
    }
    
    t->Fill();
  }    

  fsaveTree->cd();
  t->Write();
  fsaveTree->ls();
  fsaveTree->Close();


}
