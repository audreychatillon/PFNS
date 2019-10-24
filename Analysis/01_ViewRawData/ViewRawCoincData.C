#include <iostream>

#include <TCanvas.h>
#include <TChain.h>
#include <TH1.h>
#include <TString.h>

#include "../../Faster2Root/ReadPid/pid.h"
#include "../RunRefs/RunRefs.h"

#include "../ClassDef/RawCoincData.h"
#include "../ClassDef/FC_RawMultHistos.h"
#include "../ClassDef/FC_RawQHistos.h"
#include "../ClassDef/FC_RawToFHistos.h"
#include "../ClassDef/CHINU_RawMultHistos.h"
#include "../ClassDef/CHINU_RawQHistos.h"
#include "../ClassDef/CHINU_RawToFHistos.h"
#include "../ClassDef/CHINU_RawBkgToFHistos.h"
#include "../ClassDef/B3_RawMultHistos.h"
#include "../ClassDef/B3_RawQHistos.h"
#include "../ClassDef/B3_RawToFHistos.h"
#include "../ClassDef/HF_RawMultHistos.h"
#include "../ClassDef/HF_RawQHisto.h"
#include "../ClassDef/PULSER_RawMultHistos.h"
#include "../ClassDef/PULSER_RawQHisto.h"

void run(UInt_t runFirst, UInt_t runLast, TString dataType, TString dirpath)
{

  char name[100];

  UInt_t multNDETlg;
  UInt_t multNDEThg;


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
  
  // === ========================== === //
  // === DECLARATION OF HISTOGRAMMS === //
  // === ========================== === //

#if FC>0
  // MULTIPLICITY FC
  FC_RawMultHistos hmFC;
  // CHARGE FC
  FC_RawQHistos hqFC;
  hqFC.Define1D(6);
  hqFC.DefineDiscri(6);
#if HF>0
  FC_RawToFHistos htFC;
  //htFC.DefineOneAnode(6);
  htFC.DefineAllAnodes();
#endif
#endif

#if CHINU>0
  // MULTIPLICITY CHI-NU
  CHINU_RawMultHistos hmCHINU;
  // CHARGE CHI-NU
  CHINU_RawQHistos hqCHINU;
  hqCHINU.Define1DLG(1,2,4); //side, ring, rank
  hqCHINU.Define1DHG(1,2,4);
  hqCHINU.DefineDiscriLG(1,2,4);
  hqCHINU.DefineDiscriHG(1,2,4);
#if FC>0
  // raw ToF FC-CHINU
  CHINU_RawToFHistos htCHINU;
  htCHINU.DefineOneAnodeOneDetLG(6,1,2,4);
  htCHINU.DefineOneAnodeOneDetHG(6,1,2,4);
#endif
#endif  

#if B3>0
  // MULTIPLICITY B3
  B3_RawMultHistos hmB3;
  // CHARGE B3
  B3_RawQHistos hqB3;
  hqB3.Define1DLG(1,1); //side, pos
  hqB3.Define1DHG(1,1);
  hqB3.DefineDiscriLG(1,1);
  hqB3.DefineDiscriHG(1,1);
#if FC>0
  // raw ToF FC-B3
  B3_RawToFHistos htB3;
  htB3.DefineOneAnodeOneDetLG(6,1,1);
  htB3.DefineOneAnodeOneDetHG(6,1,1);
#endif
#endif  

#if HF>0
  HF_RawMultHistos hmHF;
  HF_RawQHisto hqHF;
#endif

#if PULSER>0
  PULSER_RawMultHistos hmPULSER;
  PULSER_RawQHisto hqPULSER;
#endif

  // === ===================== === //
  // === LOOP OVER THE ENTRIES === //
  // === ===================== === //
  UInt_t cpt_NDETlg=0;
  UInt_t cpt_NDEThg=0;
  for(ULong64_t Entry=0; Entry<nentries; Entry++){
    
    // --- ----------------------------- --- //
    // --- Read each entry of the TChain --- //
    // --- ----------------------------- --- //
    raw.GetEntry(Entry);
    if((Entry%100000)==0) cout << Entry << endl;

#if FC>0
    // MULTIPLICITY FC
    hmFC.FillHistos(raw.vFC_anode,raw.FC_mult);
    // CHARGE FC
    hqFC.FillHistos(raw.vFC_anode,raw.vFC_Q1,raw.vFC_Q2);
#if HF>0
    if((raw.vFC_anode->size()==1) && (raw.vHF_time->size()==1))
      htFC.FillHistos(raw.vHF_time->at(0),raw.vFC_anode->at(0),raw.vFC_time->at(0));
#endif
#endif

#if CHINU>0
    // MULTIPLICITY CHI-NU
    hmCHINU.FillHistos(raw.vCHINUlg_det,raw.vCHINUhg_det);
    // CHARGE AND TOF CHI-NU
    if(!raw.vCHINUlg_det->empty()){
      cpt_NDETlg++;
      hqCHINU.FillHistosLG(raw.vCHINUlg_det,raw.vCHINUlg_Q1,raw.vCHINUlg_Q2);
#if FC>0
      if(raw.vFC_anode->size()==1) 
	htCHINU.FillHistosLG_FCmult1(raw.vFC_anode->at(0),raw.vFC_time->at(0),raw.vCHINUlg_det,raw.vCHINUlg_time,raw.CHINUlg_mult);
#endif
    }
    if(!raw.vCHINUhg_det->empty()){
      cpt_NDEThg++;
      hqCHINU.FillHistosHG(raw.vCHINUhg_det,raw.vCHINUhg_Q1,raw.vCHINUhg_Q2);
#if FC>0
      if(raw.vFC_anode->size()==1) 
	htCHINU.FillHistosHG_FCmult1(raw.vFC_anode->at(0),raw.vFC_time->at(0),raw.vCHINUhg_det,raw.vCHINUhg_time,raw.CHINUhg_mult);
#endif
    }
#endif

#if B3>0
    // MULTIPLICITY CHI-NU
    hmB3.FillHistos(raw.vB3lg_det,raw.vB3hg_det);
    // CHARGE AND TOF CHI-NU
    if(!raw.vB3lg_det->empty()){
      cpt_NDETlg++;
      hqB3.FillHistosLG(raw.vB3lg_det,raw.vB3lg_Q1,raw.vB3lg_Q2);
#if FC>0
      if(raw.vFC_anode->size()==1) 
	htB3.FillHistosLG_FCmult1(raw.vFC_anode->at(0),raw.vFC_time->at(0),raw.vB3lg_det,raw.vB3lg_time,raw.B3lg_mult);
#endif
    }
    if(!raw.vB3hg_det->empty()){
      cpt_NDEThg++;
      hqB3.FillHistosHG(raw.vB3hg_det,raw.vB3hg_Q1,raw.vB3hg_Q2);
#if FC>0
      if(raw.vFC_anode->size()==1) 
	htB3.FillHistosHG_FCmult1(raw.vFC_anode->at(0),raw.vFC_time->at(0),raw.vB3hg_det,raw.vB3hg_time,raw.B3hg_mult);
#endif
    }
#endif    

#if HF>0
    hmHF.FillHistos(raw.vHF_label);
    hqHF.FillHisto(raw.vHF_Q1);
#endif

#if PULSER>0
    hmPULSER.FillHistos(raw.vPULSER_label);
    hqPULSER.FillHisto(raw.vPULSER_Q1);
#endif

  }// end of for(Entry)

  // === ================ === //
  // === DRAW HISTOGRAMMS === //
  // === ================ === //
  sprintf(name,"../../histos/Raw/R%dtoR%d.root",runFirst,runLast);
  TFile * fsave = new TFile(name,"update");
  fsave->cd();
#if FC>0
  TCanvas * can1 = hmFC.Draw1DHistos();                        can1->Write();
  TCanvas * can2 = hmFC.Draw2DHistos();                        can2->Write();
  TCanvas * can3 = hqFC.Draw1DHistos(6);                       can3->Write();
  TCanvas * can4 = hqFC.DrawDiscriHisto(6);                    can4->Write();
#if HF>0
  TCanvas * can5 = htFC.DrawAllAnodes();                       can5->Write();
#endif
#endif
#if CHINU>0
  TCanvas * can6 = hmCHINU.DrawHistos();                       can6->Write();
  TCanvas * can7 = hqCHINU.DrawHistos1DLG(1,2,4);              can7->Write();
  TCanvas * can8 = hqCHINU.DrawHistos1DHG(1,2,4);              can8->Write();
  TCanvas * can9 = hqCHINU.DrawHistoDiscriLG(1,2,4);           can9->Write();
  TCanvas * can10 = hqCHINU.DrawHistoDiscriHG(1,2,4);          can10->Write();
  TCanvas * can11 = hqCHINU.DrawHistos1DSideRingLG(1,2);       can11->Write();
  TCanvas * can12 = hqCHINU.DrawHistos1DSideRingHG(1,2);       can12->Write();
  TCanvas * can13 = hqCHINU.DrawHistos1DRankLG(4);             can13->Write();
  TCanvas * can14 = hqCHINU.DrawHistos1DRankHG(4);             can14->Write();
  TCanvas * can15 = hqCHINU.DrawHistosDiscriSideRingLG(1,2);   can15->Write();
  TCanvas * can16 = hqCHINU.DrawHistosDiscriSideRingHG(1,2);   can16->Write();
  TCanvas * can17 = hqCHINU.DrawHistosDiscriRankLG(4);         can17->Write();
  TCanvas * can18 = hqCHINU.DrawHistosDiscriRankHG(4);         can18->Write();
#if FC>0
  TCanvas * can19 = htCHINU.DrawOneAnodeOneDetLG(6,1,2,4);     can19->Write();
  TCanvas * can20 = htCHINU.DrawOneAnodeOneDetHG(6,1,2,4);     can20->Write();
  TCanvas * can21 = htCHINU.DrawOneAnodeSideRingLG(6,1,2);     can21->Write();
  TCanvas * can22 = htCHINU.DrawOneAnodeSideRingHG(6,1,2);     can22->Write();
  TCanvas * can23 = htCHINU.DrawAllAnodesOneDetLG(1,2,4);      can23->Write();
  TCanvas * can24 = htCHINU.DrawAllAnodesOneDetHG(1,2,4);      can24->Write();
#endif
#endif
#if B3>0
  TCanvas * can25 = hmB3.DrawHistos();                         can25->Write();
  TCanvas * can26 = hqB3.DrawHistos1DLG(1,1);                  can26->Write();
  TCanvas * can27 = hqB3.DrawHistos1DHG(1,1);                  can27->Write();
  TCanvas * can28 = hqB3.DrawHistoDiscriLG(1,1);               can28->Write();
  TCanvas * can29 = hqB3.DrawHistoDiscriHG(1,1);               can29->Write();
  TCanvas * can30 = hqB3.DrawHistos1DAllLG();                  can30->Write();
  TCanvas * can31 = hqB3.DrawHistos1DAllHG();                  can31->Write();
  TCanvas * can32 = hqB3.DrawHistosDiscriAllLG();              can32->Write();
  TCanvas * can33 = hqB3.DrawHistosDiscriAllHG();              can33->Write();
#if FC>0
  TCanvas * can34 = htB3.DrawOneAnodeOneDetLG(6,1,1);          can34->Write();
  TCanvas * can35 = htB3.DrawOneAnodeOneDetHG(6,1,1);          can35->Write();
  TCanvas * can36 = htB3.DrawAllAnodesAllDetsLG();             can36->Write();
  TCanvas * can37 = htB3.DrawAllAnodesAllDetsHG();             can37->Write();
#endif
#endif
#if HF>0
  TCanvas * can38 = hmHF.DrawHistos();                         can38->Write();
  TCanavs * can39 = hqHF.DrawHisto();                          can39->Write();
#endif 
 #if PULSER>0
  TCanvas * can40 = hmPULSER.DrawHistos();                     can40->Write();
  TCanavs * can41 = hqPULSER.DrawHisto();                      can41->Write();
#endif
 fsave->Close();
  // === ========== === //
  // === STATISTICS === //
  // === ========== === //
  cout << "number of events : " << nentries << endl;
  cout << "number of events with NDETlg : " << cpt_NDETlg << " and NDEThg : " << cpt_NDEThg << endl;
  
}
