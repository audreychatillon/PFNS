#include <iostream>

#include "../../SetupSpecific/setup_specific.h"
#include "../RunRefs/RunRefs.h"
#include "../ClassDef/RawCoincData.h"
#include "../ClassDef/FC_RawQHistos.h"

#include "TCanvas.h"
#include "TCutG.h"
#include "TFile.h"
#include "TH2.h"


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
  cout << "nentries=" << nentries << endl;


  // === ====================================== === //
  // === DECLARATION OF ALL Q2vsQ1 HISTOGRAMMS  === //
  // === ====================================== === //
  FC_RawQHistos hqFC;
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++) hqFC.DefineDiscri(anode);

  // === ====================================================== === //
  // === LOOP OVER THE ENTRIES AND FILL THE RAW ToF HISTOGRAMMS === //
  // === ====================================================== === //
  for(ULong64_t Entry=0; Entry<nentries; Entry++){
    raw.GetEntry(Entry);
    if((Entry%500000)==0) cout << Entry << endl;
    hqFC.FillHistos(raw.vFC_anode,raw.vFC_Q1,raw.vFC_Q2);
  }// end of loop over the data 


  // === ========== === //
  // === TCutG FILE === //
  // === ========== === //
  char name[100];
  sprintf(name,"cut/cutFC_Q2vsQ1_R%d.root",runFirst);
  TFile * cutfile = new TFile(name,"recreate");
  TCanvas * can = new TCanvas("AlphaNeutronDiscri","AlphaNeutronDiscri",0,0,1500,1200);
  can->cd(); gPad->SetLogx(); gPad->SetLogy(); gPad->SetLogz();
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    hqFC.h2_Q2vsQ1[anode-1]->Draw("COL");
    TCutG* cutg = (TCutG*)gPad->WaitPrimitive("CUTG","CutG");
    cutg->SetName(Form("cut_Q2vsQ1_neutron_FC_%i",anode));
    cutg->SetLineColor(kRed);
    cutfile->cd();
    cutg->Write();
    hqFC.h2_Q2vsQ1[anode-1]->Write(); hqFC.h2_Q2vsQ1[anode-1]->SetDirectory(0);
  }
  cutfile->Close();


}
