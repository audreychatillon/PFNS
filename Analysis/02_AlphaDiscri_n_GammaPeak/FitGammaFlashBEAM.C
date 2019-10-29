#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <TCanvas.h>
#include <TChain.h>
#include <TF1.h>
#include <TH1.h>
#include <TString.h>

#include "../../Faster2Root/ReadPid/pid.h"
#include "../RunRefs/RunRefs.h"
#include "../ClassDef/RawCoincData.h"
#include "../ClassDef/FC_RawToFHistos.h"

#define SPEED_OF_LIGHT_MNS       0.299792458       // m.ns-1

void run(UInt_t runFirst, UInt_t runLast, TString dataType, TString dirpath)
{
  char name[100];

  // === ========================== === //
  // === ALPHA / NEUTRON TCutG FILE === //
  // === ========================== === //
  sprintf(name,"cut/cutFC_Q2vsQ1_R102.root");
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

  // === =========================================================================================== === //
  // === DECLARATION OF ALL ToF HISTOGRAMMS FOR CHI-NU : 11 anodes * 54 LG channels * 54 HG channels === //
  // === =========================================================================================== === //

  // ToF BEAM
#if FC>0 && HF>0
  FC_RawToFHistos htFC;
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++)
    htFC.DefineOneAnode(anode);
#endif




  // === ====================================================== === //
  // === LOOP OVER THE ENTRIES AND FILL THE RAW ToF HISTOGRAMMS === //
  // === ====================================================== === //
  for(ULong64_t Entry=0; Entry<nentries; Entry++){
    raw.GetEntry(Entry);
    if((Entry%1000000)==0) cout << Entry << endl;
    
#if FC>0 && HF>0
    if((raw.vFC_anode->size()!=1) || (raw.vHF_label->size())) continue;    
    htFC.FillHistos_FCmult1(raw.vHF_time->at(0),raw.vFC_anode->at(0),raw.vFC_time->at(0));
#endif
  }//end of for(Entry)


  // === ========================================================================== === //
  // === FIT THE GAMMA PEAK AND WRITE THE MEAN VALUE IN AN ARRAY COPIED IN .h FILES === //
  // === ========================================================================== === //

  char name[100];
  UShort_t det;
  Int_t    binMax;


#if FC>0 && HF>0
  ofstream BEAM_GammaFlash("../../SetupSpecific/FC_to_HF_GammaFlash.h",ios::out | ios::trunc);
  ofstream BEAM_OffsetToF("../../SetupSpecific/BEAM_OffsetToF.h",ios::out | ios::trunc);
  Float_t GammaFlash_Mean[FC_nAnodes];
  Double_t RealTimeFC = Distance_FC_Target4 / SPEED_OF_LIGHT_MNS;
  Double_t OffsetFC[FC_nAnodes];
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    GammaFlash_Mean[anode-1] = 0;
    OffsetFC[anode-1] = 0;
    if( htFC.h1_RawToF[anode-1]->Integral()>100) {
    }
  }

  // --- write Gamma Flash mean  --- //
  BEAM_GammaFlash << "// File automatically generated from " << dataType.Data() << " runs (R"<< runFirst << " to R"<< runLast <<")" << endl<<endl;
  BEAM_GammaFlash << "#ifndef _FCHF_GAMMA_FLASH__H_" << endl;
  BEAM_GammaFlash << "#define _FCHF_GAMMA_FLASH__H_" << endl;
  BEAM_GammaFlash << "Float_t BEAM_GammaFlash["<<FC_nAnodes<<"] = {" << endl;
  for(UShort_t anode=0; anode<FC_nAnodes-1; anode++){
    BEAM_GammaFlash << GammaFlash_Mean[anode] << "," ;
  }
  BEAM_GammaFlash << GammaFlash_Mean[FC_nAnodes-1] << endl ;
  BEAM_GammaFlash << "};" << endl;
  BEAM_GammaFlash <<  endl;
  BEAM_GammaFlash << "#endif //_FCHF_GAMMA_FLASH__H_" << endl;

  // --- write Offset ToF LG --- //
  BEAM_OffsetToF << "// File automatically generated from " << dataType.Data() << " runs (R"<< runFirst << " to R"<< runLast <<")" << endl<<endl;
  BEAM_OffsetToF << "#ifndef _BEAM_OFFSET_TOF__H_" << endl;
  BEAM_OffsetToF << "#define _BEAM_OFFSET_TOF__H_" << endl;
  BEAM_OffsetToF << "Float_t BEAM_OffsetToF["<<FC_nAnodes<<"] = {" << endl;
  for(UShort_t anode=0; anode<FC_nAnodes-1; anode++){
    BEAM_OffsetToF << OffsetFC[anode] << "," ;
  }
  BEAM_OffsetToF << OffsetFC[FC_nAnodes-1] << endl ;
  BEAM_OffsetToF << "};" << endl;
  BEAM_OffsetToF <<  endl;
  BEAM_OffsetToF << "#endif //_BEAM_OFFSET_TOF__H_" << endl;


#endif
  


  // --- DRAW --- //
  TCanvas * can = htFC.DrawAllAnodes();

}
