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
#include "TH1.h"

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

#include "Cf_Carlson.h"


#define SPEED_OF_LIGHT_MNS 0.299792458  // m.ns-1
#define NEUTRON_MASS_MeV   939.565430   //MeV



// URGENT : CALCULER L'EFFICACITE POUR R_III_9_LG (det=54) ET R_II_5_LG (det=41)

Double_t Beta(double v_mns)
{
  return ( v_mns / SPEED_OF_LIGHT_MNS ) ;
}

Double_t Gamma(Double_t beta){
  return ( 1.0 / sqrt(1.0 - pow(beta,2)) ) ;
}

Float_t Velocity2Ene(double_t v_mns)
{
  double BETA = Beta(v_mns);
  double GAMMA = Gamma(BETA);
  return ((Float_t)(NEUTRON_MASS_MeV * (GAMMA - 1)));
}



void run(UInt_t runFirst, UInt_t runLast, TString dataType, TString dirpath)
{
  char name[500];
  UShort_t CHINUlg_mult[CHINU_nDets];
  UShort_t CHINUhg_mult[CHINU_nDets];
  UShort_t B3lg_mult[B3_nDets];
  UShort_t B3hg_mult[B3_nDets];
  UShort_t det;
  UInt_t   Q1,Q2;
  Float_t  Qratio;

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
    cout << "Data type " << fType << " unrecognized in this code" << endl;
    break;
  }
  ch->ls();
  RawCoincData raw(ch);
  ULong64_t nentries = ch->GetEntries();
  


  // === ======================================== === //
  // === GET THE TCUTG TO SELECT GAMMA ON Qdiscri === //
  // === ======================================== === //
  TFile * fG_CHINUlg = new TFile("../04_CreateTCutG/cut/cut_discri_gamma_CHINUlg.root","read");
  TFile * fG_CHINUhg = new TFile("../04_CreateTCutG/cut/cut_discri_gamma_CHINUhg.root","read");
  TCutG * cutG_CHINUlg[CHINU_nDets];
  TCutG * cutG_CHINUhg[CHINU_nDets];
  for(det=1; det<=CHINU_nDets; det++){
    sprintf(name,"cut_discri_gamma_%d",det);
    cutG_CHINUlg[det-1] = (TCutG*)fG_CHINUlg->Get(name);
    sprintf(name,"cut_discri_gamma_CHINUlg_%d",det);
    cutG_CHINUlg[det-1]->SetName(name);
    cutG_CHINUlg[det-1]->SetTitle(name);

    sprintf(name,"cut_discri_gamma_%d",det);
    cutG_CHINUhg[det-1] = (TCutG*)fG_CHINUhg->Get(name);
    sprintf(name,"cut_discri_gamma_CHINUhg_%d",det);
    cutG_CHINUhg[det-1]->SetName(name);
    cutG_CHINUhg[det-1]->SetTitle(name);
  }
  fG_CHINUlg->Close();
  fG_CHINUhg->Close();
  TFile * fG_B3lg = new TFile("../04_CreateTCutG/cut/cut_discri_gamma_B3lg.root","read");
  TFile * fG_B3hg = new TFile("../04_CreateTCutG/cut/cut_discri_gamma_B3hg.root","read");
  TCutG * cutG_B3lg[B3_nDets];
  TCutG * cutG_B3hg[B3_nDets];
  for(det=1; det<=B3_nDets; det++){
    sprintf(name,"cut_discri_gamma_%d",det);
    cutG_B3lg[det-1] = (TCutG*)fG_B3lg->Get(name);
    sprintf(name,"cut_discri_gamma_B3lg_%d",det);
    cutG_B3lg[det-1]->SetName(name);
    cutG_B3lg[det-1]->SetTitle(name);

    sprintf(name,"cut_discri_gamma_%d",det);
    cutG_B3hg[det-1] = (TCutG*)fG_B3hg->Get(name);
    sprintf(name,"cut_discri_gamma_B3hg_%d",det);
    cutG_B3hg[det-1]->SetName(name);
    cutG_B3hg[det-1]->SetTitle(name);
  }
  fG_B3lg->Close();
  fG_B3hg->Close();

  // === ================================================= === //
  // === GET THE TCUTG TO SELECT NEUTRON ON Q1 vs E discri === //
  // === ================================================= === //
  TFile * fN_CHINUlg = new TFile("../04_CreateTCutG/cut/cut_Q1vsE_CHINUlg.root","read");
  TFile * fN_CHINUhg = new TFile("../04_CreateTCutG/cut/cut_Q1vsE_CHINUhg.root","read");
  TCutG * cutN_CHINUlg[CHINU_nDets];
  TCutG * cutN_CHINUhg[CHINU_nDets];
  for(det=1; det<=CHINU_nDets; det++){
    sprintf(name,"cut_Q1vsE_neutron_CHINUlg_%d",det);
    cutN_CHINUlg[det-1] = (TCutG*)fN_CHINUlg->Get(name);
    //cutN_CHINUlg[det-1]->SetVarX("vPFN_CHINUlg_Ene");
    //cutN_CHINUlg[det-1]->SetVarY("vCHINUlg_Q1");

    sprintf(name,"cut_Q1vsE_neutron_CHINUhg_%d",det);
    cutN_CHINUhg[det-1] = (TCutG*)fN_CHINUhg->Get(name);
  }
  fN_CHINUlg->Close();
  fN_CHINUhg->Close();
  TFile * fN_B3lg = new TFile("../04_CreateTCutG/cut/cut_Q1vsE_B3lg.root","read");
  TFile * fN_B3hg = new TFile("../04_CreateTCutG/cut/cut_Q1vsE_B3hg.root","read");
  TCutG * cutN_B3lg[B3_nDets];
  TCutG * cutN_B3hg[B3_nDets];
  for(det=1; det<=B3_nDets; det++){
    sprintf(name,"cut_Q1vsE_neutron_B3lg_%d",det);
    cutN_B3lg[det-1] = (TCutG*)fN_B3lg->Get(name);
 
    sprintf(name,"cut_Q1vsE_neutron_B3hg_%d",det);
    cutN_B3hg[det-1] = (TCutG*)fN_B3hg->Get(name);
  }
  fN_B3lg->Close();
  fN_B3hg->Close();

 
  // === ===================================================== === //
  // === OUTPUT DATA CONCERNING THE TIMES oF FLIGHT AND ENERGY === //
  // === ===================================================== === //
  UInt_t Event;

  // --- BEAM
#if FC>0
  UShort_t FC_anode;
  UInt_t   FC_Q1;
  UInt_t   FC_Q2;
#if HF>0
  Double_t BEAM_ToF;
  //Double_t BEAM_Ene;
#endif
  // TO DO IF PULSER
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
  std::vector<Double_t> vPFN_CHINUhg_ToF;    // PFN ToF t(CHINU) - t(FC)
  std::vector<Float_t> vPFN_CHINUlg_Ene;    // PFN Energy
  std::vector<Float_t> vPFN_CHINUhg_Ene;    // PFN Energy
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
  std::vector<Double_t> vPFN_B3hg_ToF;    // PFN ToF t(B3) - t(FC)
  std::vector<Float_t> vPFN_B3lg_Ene;    // PFN ToF
  std::vector<Float_t> vPFN_B3hg_Ene;    // PFN ToF
#endif
  // TO DO IF PULSER
#endif


  // === =========== === //
  // === OUTPUT TREE === //
  // === =========== === //
  switch (fType)
    {
    case 1:
      sprintf(name,"../../data/Ana/U235/R%d_to_R%d.root",runFirst,runLast);
      break;
    case 2:
      sprintf(name,"../../data/Ana/Cf252/R%d_to_R%d.root",runFirst,runLast);
      break;
    default:
      sprintf(name,"NO/TYPE/RECOGNIZED.root");
      break;
    }
  TFile * fsaveTree = new TFile(name,"recreate");
  fsaveTree->ls();
  sprintf(name,"Run %d File %d - FC_MULT==1, NDET_MULT>0, ONLY FIRST HIT OF NDET AND AT LEAST ONE NEUTRON IN TCutG",runFirst,runLast);
  TTree * t = new TTree("AnaData",name);

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
#if HF>0
  t->Branch("BEAM_ToF",&BEAM_ToF,"BEAM_ToF/F");
  //t->Branch("BEAM_ToF",&BEAM_Ene,"BEAM_Ene/F");
#endif
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
  t->Branch("vPFN_B3lg_Ene",&vPFN_B3lg_Ene);
  t->Branch("vPFN_B3hg_Ene",&vPFN_B3hg_Ene);
#endif
#endif



  // === ========== === //
  // === HISTOGRAMS === //
  // === ========== === //
  //kinetic energy
  TH1F * h1_Ene_CHINUlg[CHINU_nDets*FC_nAnodes];
  TH1F * h1_Ene_CHINUhg[CHINU_nDets*FC_nAnodes];
  TH1F * h1_Ene_B3lg[B3_nDets*FC_nAnodes];
  TH1F * h1_Ene_B3hg[B3_nDets*FC_nAnodes];
  //efficiency (kinetic energy / Manhart)
  TH1F * h1_Eff_CHINUlg[CHINU_nDets*FC_nAnodes];
  TH1F * h1_Eff_CHINUhg[CHINU_nDets*FC_nAnodes];
  TH1F * h1_Eff_B3lg[B3_nDets*FC_nAnodes];
  TH1F * h1_Eff_B3hg[B3_nDets*FC_nAnodes];
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    for(UShort_t side=1; side<=2; side++){
      for(UShort_t ring=1; ring<=3; ring++){
	for(UShort_t rank=1; rank<=9; rank++){
	  det=rank + 9*(ring-1) + 27*(side-1);
	  sprintf(name,"ENERGY_CHINU_LOW_GAIN_%s_%s_%d_fromFC%d",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank,anode);
	  h1_Ene_CHINUlg[det-1+CHINU_nDets*(anode-1)] = new TH1F(name,name,nBinsEne,binEdges);
	  h1_Ene_CHINUlg[det-1+CHINU_nDets*(anode-1)]->SetDirectory(0);
	  sprintf(name,"ENERGY_CHINU_HIGH_GAIN_%s_%s_%d_fromFC%d",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank,anode);
	  h1_Ene_CHINUhg[det-1+CHINU_nDets*(anode-1)] = new TH1F(name,name,nBinsEne,binEdges);
	  h1_Ene_CHINUhg[det-1+CHINU_nDets*(anode-1)]->SetDirectory(0);
	  sprintf(name,"EFFICIENCY_CHINU_LOW_GAIN_%s_%s_%d_fromFC%d",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank,anode);
	  h1_Eff_CHINUlg[det-1+CHINU_nDets*(anode-1)] = new TH1F(name,name,nBinsEne,binEdges);
	  h1_Eff_CHINUlg[det-1+CHINU_nDets*(anode-1)]->SetDirectory(0);
	  sprintf(name,"EFFICIENCY_CHINU_HIGH_GAIN_%s_%s_%d_fromFC%d",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank,anode);
	  h1_Eff_CHINUhg[det-1+CHINU_nDets*(anode-1)] = new TH1F(name,name,nBinsEne,binEdges);
	  h1_Eff_CHINUhg[det-1+CHINU_nDets*(anode-1)]->SetDirectory(0);
	}
      }
      for(UShort_t pos=1; pos<=2; pos++){
	det=pos + 2*(side-1);
	sprintf(name,"ENERGY_B3_LOW_GAIN_%s_%d_fromFC%d",sideVal[side-1].Data(),pos,anode);
	h1_Ene_B3lg[det-1+B3_nDets*(anode-1)] = new TH1F(name,name,nBinsEne,binEdges);
	h1_Ene_B3lg[det-1+B3_nDets*(anode-1)]->SetDirectory(0);
	sprintf(name,"ENERGY_B3_HIGH_GAIN_%s_%d_fromFC%d",sideVal[side-1].Data(),pos,anode);
	h1_Ene_B3hg[det-1+B3_nDets*(anode-1)] = new TH1F(name,name,nBinsEne,binEdges);
	h1_Ene_B3hg[det-1+B3_nDets*(anode-1)]->SetDirectory(0);
	sprintf(name,"EFFICIENCY_B3_LOW_GAIN_%s_%d_fromFC%d",sideVal[side-1].Data(),pos,anode);
	h1_Eff_B3lg[det-1+B3_nDets*(anode-1)] = new TH1F(name,name,nBinsEne,binEdges);
	h1_Eff_B3lg[det-1+B3_nDets*(anode-1)]->SetDirectory(0);
	sprintf(name,"EFFICIENCY_B3_HIGH_GAIN_%s_%d_fromFC%d",sideVal[side-1].Data(),pos,anode);
	h1_Eff_B3hg[det-1+B3_nDets*(anode-1)] = new TH1F(name,name,nBinsEne,binEdges);
	h1_Eff_B3hg[det-1+B3_nDets*(anode-1)]->SetDirectory(0);
      }
    }
  }

  // === ===================== === //
  // === LOOP OVER THE ENTRIES === //
  // === ===================== === //
  Double_t ToF, Velocity;
  Float_t Ene;
  cout << "NUMBER OF ENTRIES: " << nentries << endl;
  for(ULong64_t Entry=0; Entry<nentries; Entry++){

    // --- ------------------------------------------------ --- //
    // --- Keep only the first hit of each neutron detector --- //
    // --- ------------------------------------------------ --- //
    for(det=0; det<CHINU_nDets; det++){
      CHINUlg_mult[det]=0;
      CHINUhg_mult[det]=0;      
    }
    for(det=0; det<B3_nDets; det++){
      B3lg_mult[det]=0;
      B3hg_mult[det]=0;      
    }

    // --- ----------------------------------------------------- --- //
    // --- Initialization of the vector dedicated to ToF and Ene --- //
    // --- ----------------------------------------------------- --- //
    Event=0;
    FC_anode=0;
    FC_Q1=0;
    FC_Q2=0;
    BEAM_ToF=0;
    //    BEAM_Ene=0;
    vCHINUlg_det.clear();
    vCHINUhg_det.clear();
    vCHINUlg_Q1.clear();
    vCHINUhg_Q1.clear();
    vCHINUlg_Q2.clear();
    vCHINUhg_Q2.clear();
    vPFN_CHINUlg_ToF.clear();
    vPFN_CHINUhg_ToF.clear();
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
    vPFN_B3lg_Ene.clear();
    vPFN_B3hg_Ene.clear();

    // --- ----------------------------- --- //
    // --- Read each entry of the TChain --- //
    // --- ----------------------------- --- //
    raw.GetEntry(Entry);
    if((Entry%1000000)==0)    cout << Entry << endl;

    if (raw.vFC_anode->size()!=1) continue;
    if((raw.vB3lg_det->size()==0) && (raw.vB3hg_det->size()==0) && (raw.vCHINUlg_det->size()==0) && (raw.vCHINUhg_det->size()==0)) continue;
    if(FC_Q1<FC_CutAlpha[FC_anode-1]) continue;

    Event = raw.Event;

    // === FISSION CHAMBER === //
    // get some raw data
    FC_anode = raw.vFC_anode->at(0);
    FC_Q1 = raw.vFC_Q1->at(0);
    FC_Q2 = raw.vFC_Q2->at(0);
    // test if the data is below the alpha cut
    if (raw.vFC_anode->size()==1) {
      BEAM_ToF = raw.vFC_time->at(0) - raw.vHF_time->at(0); // this is ToFraw = FC_time-HF_time, TO DO transform into ToFcal
      // TO DO: calculate the beam energy 
    }

    // === CHINU - LOW GAIN === //
    for(UShort_t m=0; m<raw.vCHINUlg_det->size(); m++) {
      if(CHINUlg_mult[raw.vCHINUlg_det->at(m)-1]>0) continue; // if this detector has alredy a hit recorded, do not take it into account
      // get some raw data
      det = raw.vCHINUlg_det->at(m);
      Q1  = raw.vCHINUlg_Q1->at(m);
      Q2  = raw.vCHINUlg_Q2->at(m);
      ToF = (raw.vCHINUlg_time->at(m)-raw.vFC_time->at(0))+CHINU_OffsetToFLG[det-1+CHINU_nDets*(FC_anode-1)];
      Velocity = Distance_FC_ChiNu[raw.vCHINUlg_det->at(m)-1] / ToF;
      Ene = Velocity2Ene(Velocity);

      // test if ToF is below the prompt ToFcut
      if(ToF<ToFcutPFN) 	continue;

      // test if the Qdiscri is in the Gamma TCutG
      if (Q1>0) Qratio = (Float_t)Q2 /(Float_t)Q1;
      else Qratio=-1;
      if (cutG_CHINUlg[det-1]->IsInside(Q1,Qratio)) continue;

      // test if Q1vsE is not in the neutron TCutG
      if (!cutN_CHINUlg[det-1]->IsInside(Ene,Q1)) continue;

      vCHINUlg_det.push_back(det);
      vCHINUlg_Q1.push_back(Q1);
      vCHINUlg_Q2.push_back(Q2);
      vPFN_CHINUlg_ToF.push_back(ToF);
      vPFN_CHINUlg_Ene.push_back(Ene);
      h1_Ene_CHINUlg[det-1+CHINU_nDets*(FC_anode-1)]->Fill(Ene);
      // flag that neutron detecteur number det has been taken into account
      CHINUlg_mult[det-1]++;
    }

    // === CHINU - HIGH GAIN === //
    for(UShort_t m=0; m<raw.vCHINUhg_det->size(); m++) {
      if(CHINUhg_mult[raw.vCHINUhg_det->at(m)-1]>0) continue; // if this detector has alredy a hit recorded, do not take it into account
      // get some raw data
      det = raw.vCHINUhg_det->at(m);
      Q1  = raw.vCHINUhg_Q1->at(m);
      Q2  = raw.vCHINUhg_Q2->at(m);
      ToF = (raw.vCHINUhg_time->at(m)-raw.vFC_time->at(0))+CHINU_OffsetToFHG[det-1+CHINU_nDets*(FC_anode-1)];
      Velocity = Distance_FC_ChiNu[raw.vCHINUhg_det->at(m)-1] / ToF;
      Ene = Velocity2Ene(Velocity);

      // test if ToF is inside the prompt gamma peak
      if((CHINU_GammaToFmin<=ToF) && (ToF<=CHINU_GammaToFmax)) 	continue;

      // test if the Qdiscri is in the Gamma TCutG
      if (Q1>0) Qratio = (Float_t)Q2 /(Float_t)Q1;
      else Qratio=-1;
      if (cutG_CHINUhg[det-1]->IsInside(Q1,Qratio)) continue;

      // test if Q1vsE is not in the neutron TCutG
      if (!cutN_CHINUhg[det-1]->IsInside(Ene,Q1)) continue;

      vCHINUhg_det.push_back(det);
      vCHINUhg_Q1.push_back(Q1);
      vCHINUhg_Q2.push_back(Q2);
      vPFN_CHINUhg_ToF.push_back(ToF);
      vPFN_CHINUhg_Ene.push_back(Ene);
      h1_Ene_CHINUhg[det-1+CHINU_nDets*(FC_anode-1)]->Fill(Ene);
      // flag that neutron detecteur number det has been taken into account
      CHINUhg_mult[det-1]++;
    }

  

    // === B3 - LOW GAIN === //
    for(UShort_t m=0; m<raw.vB3lg_det->size(); m++) {
      if(B3lg_mult[raw.vB3lg_det->at(m)-1]>0) continue; // if this detector has alredy a hit recorded, do not take it into account
      // get some raw data
      det = raw.vB3lg_det->at(m);
      Q1  = raw.vB3lg_Q1->at(m);
      Q2  = raw.vB3lg_Q2->at(m);
      ToF = (raw.vB3lg_time->at(m)-raw.vFC_time->at(0))+B3_OffsetToFLG[det-1+B3_nDets*(FC_anode-1)];
      Velocity = Distance_FC_ChiNu[raw.vB3lg_det->at(m)-1] / ToF;
      Ene = Velocity2Ene(Velocity);

      // test if ToF is inside the prompt gamma peak
      if((B3_GammaToFmin<=ToF) && (ToF<=B3_GammaToFmax)) 	continue;

      // test if the Qdiscri is in the Gamma TCutG
      if (Q1>0) Qratio = (Float_t)Q2 /(Float_t)Q1;
      else Qratio=-1;
      if (cutG_B3lg[det-1]->IsInside(Q1,Qratio)) continue;

      // test if Q1vsE is not in the neutron TCutG
      if (!cutN_B3lg[det-1]->IsInside(Ene,Q1)) continue;

      vB3lg_det.push_back(det);
      vB3lg_Q1.push_back(Q1);
      vB3lg_Q2.push_back(Q2);
      vPFN_B3lg_ToF.push_back(ToF);
      vPFN_B3lg_Ene.push_back(Ene);
      h1_Ene_B3lg[det-1+B3_nDets*(FC_anode-1)]->Fill(Ene);
      // flag that neutron detecteur number det has been taken into account
      B3lg_mult[det-1]++;
    }

    // === B3 - HIGH GAIN === //
    for(UShort_t m=0; m<raw.vB3hg_det->size(); m++) {
      if(B3hg_mult[raw.vB3hg_det->at(m)-1]>0) continue; // if this detector has alredy a hit recorded, do not take it into account
      // get some raw data
      det = raw.vB3hg_det->at(m);
      Q1  = raw.vB3hg_Q1->at(m);
      Q2  = raw.vB3hg_Q2->at(m);
      ToF = (raw.vB3hg_time->at(m)-raw.vFC_time->at(0))+B3_OffsetToFHG[det-1+B3_nDets*(FC_anode-1)];
      Velocity = Distance_FC_ChiNu[raw.vB3hg_det->at(m)-1] / ToF;
      Ene = Velocity2Ene(Velocity);

      // test if ToF is inside the prompt gamma peak
      if((B3_GammaToFmin<=ToF) && (ToF<=B3_GammaToFmax)) 	continue;

      // test if the Qdiscri is in the Gamma TCutG
      if (Q1>0) Qratio = (Float_t)Q2 /(Float_t)Q1;
      else Qratio=-1;
      if (cutG_B3hg[det-1]->IsInside(Q1,Qratio)) continue;

      // test if Q1vsE is not in the neutron TCutG
      if (!cutN_B3hg[det-1]->IsInside(Ene,Q1)) continue;

      vB3hg_det.push_back(det);
      vB3hg_Q1.push_back(Q1);
      vB3hg_Q2.push_back(Q2);
      vPFN_B3hg_ToF.push_back(ToF);
      vPFN_B3hg_Ene.push_back(Ene);
      h1_Ene_B3hg[det-1+B3_nDets*(FC_anode-1)]->Fill(Ene);
      // flag that neutron detecteur number det has been taken into account
      B3hg_mult[det-1]++;
    }

    // --- ------------- --- //
    // --- FILL THE TREE --- //
    // --- ------------- --- //
    t->Fill();
  }    

  fsaveTree->cd();
  t->Write();
  fsaveTree->ls();
  fsaveTree->Close();

  switch (fType)
    {
    case 1:
      sprintf(name,"../../histos/U235_PFN_Ene_Eff_R%d_to_R%d.root",runFirst,runLast);
      break;
    case 2:
      sprintf(name,"../../histos/Cf252_PFN_Ene_Eff_R%d_to_R%d.root",runFirst,runLast);
      break;
    default:
      sprintf(name,"NO/TYPE/RECOGNIZED.root");
      break;
    }
  TFile * fHistos = new TFile(name,"recreate");
  fHistos->cd();
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    for(det=1; det<=CHINU_nDets; det++){
      for(UShort_t bin=1; bin<=h1_Ene_CHINUlg[det-1+CHINU_nDets*(anode-1)]->GetNbinsX();bin++){
	h1_Eff_CHINUlg[det-1+CHINU_nDets*(anode-1)]->SetBinContent(bin,h1_Ene_CHINUlg[det-1+CHINU_nDets*(anode-1)]->GetBinContent(bin)/binContent[bin-1]);
	h1_Ene_CHINUlg[det-1+CHINU_nDets*(anode-1)]->SetBinContent(bin,h1_Ene_CHINUlg[det-1+CHINU_nDets*(anode-1)]->GetBinContent(bin)/h1_Ene_CHINUlg[det-1+CHINU_nDets*(anode-1)]->GetBinWidth(bin));      
	h1_Eff_CHINUhg[det-1+CHINU_nDets*(anode-1)]->SetBinContent(bin,h1_Ene_CHINUhg[det-1+CHINU_nDets*(anode-1)]->GetBinContent(bin)/binContent[bin-1]);
	h1_Ene_CHINUhg[det-1+CHINU_nDets*(anode-1)]->SetBinContent(bin,h1_Ene_CHINUhg[det-1+CHINU_nDets*(anode-1)]->GetBinContent(bin)/h1_Ene_CHINUhg[det-1+CHINU_nDets*(anode-1)]->GetBinWidth(bin));   
      }
      h1_Ene_CHINUlg[det-1+CHINU_nDets*(anode-1)]->Write();      
      h1_Eff_CHINUlg[det-1+CHINU_nDets*(anode-1)]->Write();      
      h1_Ene_CHINUhg[det-1+CHINU_nDets*(anode-1)]->Write();      
      h1_Eff_CHINUhg[det-1+CHINU_nDets*(anode-1)]->Write();      
    }
    for(det=1; det<=B3_nDets; det++){
      for(UShort_t bin=1; bin<=h1_Ene_B3lg[det-1+B3_nDets*(anode-1)]->GetNbinsX();bin++){
	h1_Eff_B3lg[det-1+B3_nDets*(anode-1)]->SetBinContent(bin,h1_Ene_B3lg[det-1+B3_nDets*(anode-1)]->GetBinContent(bin)/binContent[bin-1]);
	h1_Ene_B3lg[det-1+B3_nDets*(anode-1)]->SetBinContent(bin,h1_Ene_B3lg[det-1+B3_nDets*(anode-1)]->GetBinContent(bin)/h1_Ene_B3lg[det-1+B3_nDets*(anode-1)]->GetBinWidth(bin));    
	h1_Eff_B3hg[det-1+B3_nDets*(anode-1)]->SetBinContent(bin,h1_Ene_B3hg[det-1+B3_nDets*(anode-1)]->GetBinContent(bin)/binContent[bin-1]);
	h1_Ene_B3hg[det-1+B3_nDets*(anode-1)]->SetBinContent(bin,h1_Ene_B3hg[det-1+B3_nDets*(anode-1)]->GetBinContent(bin)/h1_Ene_B3hg[det-1+B3_nDets*(anode-1)]->GetBinWidth(bin));   
      }
      h1_Ene_B3lg[det-1+B3_nDets*(anode-1)]->Write();      
      h1_Eff_B3lg[det-1+B3_nDets*(anode-1)]->Write();      
      h1_Ene_B3hg[det-1+B3_nDets*(anode-1)]->Write();      
      h1_Eff_B3hg[det-1+B3_nDets*(anode-1)]->Write();      
    }
  }  

  fHistos->ls();
  fHistos->Close();

}
