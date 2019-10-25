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
  cout << "Number of entries : " << nentries << endl;

  // === ========================== === //
  // === DECLARATION OF HISTOGRAMMS === //
  // === ========================== === //
  sprintf(name,"../../histos/Raw/R%dtoR%d.root",runFirst,runLast);
  TFile * fsave = new TFile(name,"update");
#if FC>0
  // *** MULTIPLICITY FC *** //
  FC_RawMultHistos hmFC;

  // *** CHARGE FC *** //
  FC_RawQHistos hqFC;
  // --- define 1D Q1 and Q2 spectra for one single anode --- //
  hqFC.Define1D(6);      
  // --- define Discri (Q2vsQ1) spectra for one single anode --- //
  hqFC.DefineDiscri(6);  
  // --- the same for all anodes --- //
  //for(UShort_t anode=1; anode<=FC_nAnodes){
  //  hqFC.Define1D(anode);
  //  hqFC.DefineDiscri(anode);
  //}
#endif

#if CHINU>0
  // *** MULTIPLICITY CHI-NU *** //
  CHINU_RawMultHistos hmCHINU;

  // *** CHARGE CHI-NU *** //
  CHINU_RawQHistos hqCHINU;
  // --- define 1D and discri spectra for one single detector --- //
  //hqCHINU.Define1DLG(1,2,4);     // side, ring, rank
  //hqCHINU.Define1DHG(1,2,4);
  //hqCHINU.DefineDiscriLG(1,2,4);
  //hqCHINU.DefineDiscriHG(1,2,4);
  //--- define all spectra for one rank --- //
  //for(UShort_t side=1; side<=2; side++) {
  //  for(UShort_t ring=1; ring<=3; ring++) {
  //    hqCHINU.Define1DLG(side,ring,7);     
  //    hqCHINU.Define1DHG(side,ring,7);
  //    hqCHINU.DefineDiscriLG(side,ring,7);
  //    hqCHINU.DefineDiscriHG(side,ring,7);
  //  }
  //}
  //--- define all spectra for one ring --- //
  //for(UShort_t rank=1; rank<=9; rank++) {
  //  hqCHINU.Define1DLG(2,3,rank);     
  //  hqCHINU.Define1DHG(2,3,rank);
  //  hqCHINU.DefineDiscriLG(2,3,rank);
  //  hqCHINU.DefineDiscriHG(2,3,rank);
  //}
  // --- define all spectra --- //
  for(UShort_t side=1; side<=2; side++) {
    for(UShort_t ring=1; ring<=3; ring++) {
      for(UShort_t rank=1; rank<=9; rank++) {
	//hqCHINU.Define1DLG(side,ring,rank);     
	//hqCHINU.Define1DHG(side,ring,rank);
	hqCHINU.DefineDiscriLG(side,ring,rank);
	hqCHINU.DefineDiscriHG(side,ring,rank);
      }
    }
  }



#if FC>0
  // *** raw ToF FC-CHINU *** //
  CHINU_RawToFHistos htCHINU;
  // --- define ToFraw histos spectra for one single anode and one single detector --- //
  //htCHINU.DefineOneAnodeOneDetLG(6,1,2,4);
  //htCHINU.DefineOneAnodeOneDetHG(6,1,2,4);
  //--- define all spectra for all anodes and one det --- //
  //for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
  //htCHINU.DefineOneAnodeOneDetLG(anode,2,1,2);
  //htCHINU.DefineOneAnodeOneDetHG(anode,2,1,2);
  //}
  //--- define all spectra for one anode and one rank --- //
  //for(UShort_t side=1; side<=2; side++) {
  //for(UShort_t ring=1; ring<=3; ring++) {
  //htCHINU.DefineOneAnodeOneDetLG(6,side,ring,7);     
  //htCHINU.DefineOneAnodeOneDetHG(6,side,ring,7);
  //}
  //}
  //--- define all spectra for one anode and one ring --- //
  //for(UShort_t rank=1; rank<=9; rank++) {
  //htCHINU.DefineOneAnodeOneDetLG(6,2,3,rank);     
  //htCHINU.DefineOneAnodeOneDetHG(6,2,3,rank);
  //}
  // --- define all spectra for one anode and all detectors --- //
  for(UShort_t side=1; side<=2; side++) {
    for(UShort_t ring=1; ring<=3; ring++) {
      for(UShort_t rank=1; rank<=9; rank++) {
  	htCHINU.DefineOneAnodeOneDetLG(6,side,ring,rank);     
  	htCHINU.DefineOneAnodeOneDetHG(6,side,ring,rank);
      }
    }
  }
#endif
#endif

#if B3>0
  // *** MULTIPLICITY B3 *** //
  B3_RawMultHistos hmB3;

  // *** CHARGE B3 *** //
  B3_RawQHistos hqB3;
  // --- define 1D and discri spectra for one single detector --- //
  //hqB3.Define1DLG(1,2);     // side, pos
  //hqB3.Define1DHG(1,2);
  //hqB3.DefineDiscriLG(1,2);
  //hqB3.DefineDiscriHG(1,2);
  // --- define all spectra for one side --- //
  //for(UShort_t pos=1; pos<+2; pos++){
  //hqB3.Define1DLG(1,pos);     // side, pos
  //hqB3.Define1DHG(1,pos);
  //hqB3.DefineDiscriLG(1,pos);
  //hqB3.DefineDiscriHG(1,pos);
  //}
  // --- define all spectra for all detectors --- //
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t pos=1; pos<=2; pos++){
      //hqB3.Define1DLG(side,pos);     // side, pos
      //hqB3.Define1DHG(side,pos);
      hqB3.DefineDiscriLG(side,pos);
      //hqB3.DefineDiscriHG(side,pos);      
    }
  }
#if FC>0
  // *** ToF FC-B3 *** //
  //B3_RawToFHistos htB3;
  // --- define ToFraw histos spectra for one single anode and one single detector --- //
  //htB3.DefineOneAnodeOneDetLG(6,1,2);
  //htB3.DefineOneAnodeOneDetHG(6,1,2);
  //--- define all spectra for all anodes and one det --- //
  //for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
  //  htB3.DefineOneAnodeOneDetLG(anode,2,1);
  //  //htB3.DefineOneAnodeOneDetHG(anode,2,1);
  //}
  //--- define all spectra for one anode and all dets --- //
  //for(UShort_t side=1; side<=2; side++) {
  //  for(UShort_t pos=1; pos<=2; pos++) {
  //    htB3.DefineOneAnodeOneDetLG(6,side,pos);
  //    //htB3.DefineOneAnodeOneDetHG(6,side,pos);
  //  }
  //}
  //--- define all spectra for all anodes --- //
  //for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
  //  for(UShort_t side=1; side<=2; side++) {
  //    for(UShort_t pos=1; pos<=2; pos++) {
  //	htB3.DefineOneAnodeOneDetLG(anode,side,pos);     
  //	htB3.DefineOneAnodeOneDetHG(anode,side,pos);
  //    }
  // }
  //}
#endif
#endif


  // === ===================== === //
  // === LOOP OVER THE ENTRIES === //
  // === ===================== === //
  UInt_t cpt_NDETlg=0;
  UInt_t cpt_NDEThg=0;
  for(ULong64_t Entry=0; Entry<20000000; Entry++){
  //  for(ULong64_t Entry=0; Entry<nentries; Entry++){
    
    // --- ----------------------------- --- //
    // --- Read each entry of the TChain --- //
    // --- ----------------------------- --- //
    raw.GetEntry(Entry);
    if((Entry%1000000)==0) cout << Entry << endl;

#if FC>0
    // MULTIPLICITY FC
    hmFC.FillHistos(raw.vFC_anode,raw.FC_mult);
    // CHARGE FC
    hqFC.FillHistos(raw.vFC_anode,raw.vFC_Q1,raw.vFC_Q2);
#endif




#if CHINU>0
    // MULTIPLICITY CHI-NU
    hmCHINU.FillHistos(raw.vCHINUlg_det,raw.vCHINUhg_det);
    // CHARGE CHI-NU LOW GAIN
    if(!raw.vCHINUlg_det->empty()){
      cpt_NDETlg++;
      hqCHINU.FillHistosLG(raw.vCHINUlg_det,raw.vCHINUlg_Q1,raw.vCHINUlg_Q2);
#if FC>0
      if(raw.vFC_anode->size()==1) 
      	htCHINU.FillHistosLG_FCmult1(raw.vFC_anode->at(0),raw.vFC_time->at(0),raw.vCHINUlg_det,raw.vCHINUlg_time,raw.CHINUlg_mult);
#endif
    }
    // CHARGE CHI-NU HIGH GAIN
    if(!raw.vCHINUhg_det->empty()){
      cpt_NDEThg++;
      hqCHINU.FillHistosHG(raw.vCHINUhg_det,raw.vCHINUhg_Q1,raw.vCHINUhg_Q2);
#if FC>0
      if(raw.vFC_anode->size()==1) 
      	htCHINU.FillHistosHG_FCmult1(raw.vFC_anode->at(0),raw.vFC_time->at(0),raw.vCHINUhg_det,raw.vCHINUhg_time,raw.CHINUhg_mult);
#endif
    }
#endif



#if B3
    // MULTIPLICITE B3
    hmB3.FillHistos(raw.vB3lg_det,raw.vB3hg_det);
    // CHARGE B3 LOW GAIN
    if(!raw.vB3lg_det->empty()){
      hqB3.FillHistosLG(raw.vB3lg_det,raw.vB3lg_Q1,raw.vB3lg_Q2);
#if FC>0
      //if(raw.vFC_anode->size()==1) 
      //htB3.FillHistosLG_FCmult1(raw.vFC_anode->at(0),raw.vFC_time->at(0),raw.vB3lg_det,raw.vB3lg_time,raw.B3lg_mult);
#endif
      
    }
    // CHARGE B3 HIGH GAIN
    if(!raw.vB3hg_det->empty()){
      hqB3.FillHistosLG(raw.vB3hg_det,raw.vB3hg_Q1,raw.vB3hg_Q2);
    }
#endif
    if(cpt_NDETlg>5000000) break;
  }// end of for(Entry)

  // === ================ === //
  // === DRAW HISTOGRAMMS === //
  // === ================ === //
  fsave->cd();

#if FC>0
  // FC mult
  TCanvas * can1 = hmFC.Draw1DHistos();                   can1->Write(); // FC multiplicity 1D spectra
  TCanvas * can2 = hmFC.Draw2DHistos();                   can2->Write(); // FC multiplicity 2D spectra
  // FC charge
  TCanvas * can3 = hqFC.Draw1DHistos(6);                  can3->Write(); // FC charge 1D spectrum for one anode
  TCanvas * can4 = hqFC.DrawDiscriHisto(6);               can4->Write(); // FC charge 2D spectrum for one anode
  //TCanvas * can5 = hqFC.Draw1DAllHistos();                can5->Write(); // FC charge 1D spectra for all anodes
  //TCanvas * can6 = hqFC.DrawDiscriAllHistos();            can6->Write(); // FC charge 2D spectra for all anodes
#endif
#if CHINU>0
  //  CHINU mult
  TCanvas * can7 = hmCHINU.DrawHistos();                     can7->Write(); // CHINU mult spectra
  // CHINU charge
  //TCanvas * can8 = hqCHINU.DrawHistos1DLG(1,2,4);            can8->Write(); // CHINU LG charge 1D for one detector
  //TCanvas * can9 = hqCHINU.DrawHistos1DHG(1,2,4);            can9->Write(); // CHINU HG charge 1D for one detector
  //TCanvas * can10 = hqCHINU.DrawHistoDiscriLG(1,2,4);        can10->Write(); // CHINU LG charge Discri for one detector
  //TCanvas * can11 = hqCHINU.DrawHistoDiscriHG(1,2,4);        can11->Write(); // CHINU HG charge Discri for one detector
  //TCanvas * can12 = hqCHINU.DrawHistos1DRankLG(7);           can12->Write(); // CHINU LG charge 1D for 6 detectors on the same rank
  //TCanvas * can13 = hqCHINU.DrawHistos1DRankHG(7);           can13->Write(); // CHINU HG charge 1D for 6 detectors on the same rank
  //TCanvas * can14 = hqCHINU.DrawHistosDiscriRankLG(7);       can14->Write(); // CHINU LG charge Discri for 6 detectors on the same rank
  //TCanvas * can15 = hqCHINU.DrawHistosDiscriRankHG(7);       can15->Write(); // CHINU HG charge Discri for 6 detectors on the same rank
  //TCanvas * can16a = hqCHINU.DrawHistos1DSideRingLG(1,1);     can16a->Write(); // CHINU LG charge 1D for 9 detectors on the same ring
  //TCanvas * can17a = hqCHINU.DrawHistos1DSideRingHG(1,1);     can17a->Write(); // CHINU HG charge 1D for 9 detectors on the same ring
  TCanvas * can18a = hqCHINU.DrawHistosDiscriSideRingLG(1,1); can18a->Write(); // CHINU LG charge Discri for 9 detectors on the same ring
  TCanvas * can19a = hqCHINU.DrawHistosDiscriSideRingHG(1,1); can19a->Write(); // CHINU HG charge Discri for 9 detectors on the same ring

  //TCanvas * can16b = hqCHINU.DrawHistos1DSideRingLG(1,2);     can16b->Write(); // CHINU LG charge 1D for 9 detectors on the same ring
  //TCanvas * can17b = hqCHINU.DrawHistos1DSideRingHG(1,2);     can17b->Write(); // CHINU HG charge 1D for 9 detectors on the same ring
  TCanvas * can18b = hqCHINU.DrawHistosDiscriSideRingLG(1,2); can18b->Write(); // CHINU LG charge Discri for 9 detectors on the same ring
  TCanvas * can19b = hqCHINU.DrawHistosDiscriSideRingHG(1,2); can19b->Write(); // CHINU HG charge Discri for 9 detectors on the same ring

  //TCanvas * can16c = hqCHINU.DrawHistos1DSideRingLG(1,3);     can16c->Write(); // CHINU LG charge 1D for 9 detectors on the same ring
  //TCanvas * can17c = hqCHINU.DrawHistos1DSideRingHG(1,3);     can17c->Write(); // CHINU HG charge 1D for 9 detectors on the same ring
  TCanvas * can18c = hqCHINU.DrawHistosDiscriSideRingLG(1,3); can18c->Write(); // CHINU LG charge Discri for 9 detectors on the same ring
  TCanvas * can19c = hqCHINU.DrawHistosDiscriSideRingHG(1,3); can19c->Write(); // CHINU HG charge Discri for 9 detectors on the same ring

  //TCanvas * can16d = hqCHINU.DrawHistos1DSideRingLG(2,1);     can16d->Write(); // CHINU LG charge 1D for 9 detectors on the same ring
  //TCanvas * can17d = hqCHINU.DrawHistos1DSideRingHG(2,1);     can17d->Write(); // CHINU HG charge 1D for 9 detectors on the same ring
  TCanvas * can18d = hqCHINU.DrawHistosDiscriSideRingLG(2,1); can18d->Write(); // CHINU LG charge Discri for 9 detectors on the same ring
  TCanvas * can19d = hqCHINU.DrawHistosDiscriSideRingHG(2,1); can19d->Write(); // CHINU HG charge Discri for 9 detectors on the same ring

  //TCanvas * can16e = hqCHINU.DrawHistos1DSideRingLG(2,2);     can16e->Write(); // CHINU LG charge 1D for 9 detectors on the same ring
  //TCanvas * can17e = hqCHINU.DrawHistos1DSideRingHG(2,2);     can17e->Write(); // CHINU HG charge 1D for 9 detectors on the same ring
  TCanvas * can18e = hqCHINU.DrawHistosDiscriSideRingLG(2,2); can18e->Write(); // CHINU LG charge Discri for 9 detectors on the same ring
  TCanvas * can19e = hqCHINU.DrawHistosDiscriSideRingHG(2,2); can19e->Write(); // CHINU HG charge Discri for 9 detectors on the same ring

  //TCanvas * can16f = hqCHINU.DrawHistos1DSideRingLG(2,3);     can16f->Write(); // CHINU LG charge 1D for 9 detectors on the same ring
  //TCanvas * can17f = hqCHINU.DrawHistos1DSideRingHG(2,3);     can17f->Write(); // CHINU HG charge 1D for 9 detectors on the same ring
  TCanvas * can18f = hqCHINU.DrawHistosDiscriSideRingLG(2,3); can18f->Write(); // CHINU LG charge Discri for 9 detectors on the same ring
  TCanvas * can19f = hqCHINU.DrawHistosDiscriSideRingHG(2,3); can19f->Write(); // CHINU HG charge Discri for 9 detectors on the same ring

#if FC>0
  //TCanvas * can20 = htCHINU.DrawOneAnodeOneDetLG(6,1,2,4);   can20->Write(); //FC-CHINUlg ToFraw for one anode and one det
  //TCanvas * can21 = htCHINU.DrawOneAnodeOneDetHG(6,1,2,4);   can21->Write(); //FC-CHINUhg ToFraw for one anode and one det
  //TCanvas * can22 = htCHINU.DrawAllAnodesOneDetLG(2,1,2);    can22->Write(); //FC-CHINUlg ToFraw for all anodes and one det
  //TCanvas * can23 = htCHINU.DrawAllAnodesOneDetHG(2,1,2);    can23->Write(); //FC-CHINUhg ToFraw for all anodes and one det
  TCanvas * can24a = htCHINU.DrawOneAnodeSideRingLG(6,1,1);   can24a->Write(); //FC-CHINUlg ToFraw for one anode and 9 dets on one ring
  TCanvas * can25a = htCHINU.DrawOneAnodeSideRingHG(6,1,1);   can25a->Write(); //FC-CHINUhg ToFraw for one anode and 9 dets on one ring
  TCanvas * can24b = htCHINU.DrawOneAnodeSideRingLG(6,1,2);   can24b->Write(); //FC-CHINUlg ToFraw for one anode and 9 dets on one ring
  TCanvas * can25b = htCHINU.DrawOneAnodeSideRingHG(6,1,2);   can25b->Write(); //FC-CHINUhg ToFraw for one anode and 9 dets on one ring
  TCanvas * can24c = htCHINU.DrawOneAnodeSideRingLG(6,1,3);   can24c->Write(); //FC-CHINUlg ToFraw for one anode and 9 dets on one ring
  TCanvas * can25c = htCHINU.DrawOneAnodeSideRingHG(6,1,3);   can25c->Write(); //FC-CHINUhg ToFraw for one anode and 9 dets on one ring
  TCanvas * can24d = htCHINU.DrawOneAnodeSideRingLG(6,2,1);   can24d->Write(); //FC-CHINUlg ToFraw for one anode and 9 dets on one ring
  TCanvas * can25d = htCHINU.DrawOneAnodeSideRingHG(6,2,1);   can25d->Write(); //FC-CHINUhg ToFraw for one anode and 9 dets on one ring
  TCanvas * can24e = htCHINU.DrawOneAnodeSideRingLG(6,2,2);   can24e->Write(); //FC-CHINUlg ToFraw for one anode and 9 dets on one ring
  TCanvas * can25e = htCHINU.DrawOneAnodeSideRingHG(6,2,2);   can25e->Write(); //FC-CHINUhg ToFraw for one anode and 9 dets on one ring
  TCanvas * can24f = htCHINU.DrawOneAnodeSideRingLG(6,2,3);   can24f->Write(); //FC-CHINUlg ToFraw for one anode and 9 dets on one ring
  TCanvas * can25f = htCHINU.DrawOneAnodeSideRingHG(6,2,3);   can25f->Write(); //FC-CHINUhg ToFraw for one anode and 9 dets on one ring
  //TCanvas * can26 = htCHINU.DrawOneAnodeRankLG(6,7);         can26->Write(); //FC-CHINUlg ToFraw for one anode and 6 dets on one rank
  //TCanvas * can27 = htCHINU.DrawOneAnodeRankHG(6,7);         can27->Write(); //FC-CHINUhg ToFraw for one anode and 6 dets on one rank
#endif
#endif

#if B3>0
  //  B3 mult
  TCanvas * can28 = hmB3.DrawHistos();                  can28->Write(); // B3 mult spectra
  // B3 charge
  //TCanvas * can29 = hqB3.DrawHistos1DLG(1,2);           can29->Write(); // B3 LG charge 1D for one detector
  //TCanvas * can30 = hqB3.DrawHistos1DHG(1,2);           can30->Write(); // B3 HG charge 1D for one detector
  //TCanvas * can31 = hqB3.DrawHistoDiscriLG(1,2);        can31->Write(); // B3 LG charge Discri for one detector
  //TCanvas * can32 = hqB3.DrawHistoDiscriHG(1,2);        can32->Write(); // B3 HG charge Discri for one detector
  //TCanvas * can33 = hqB3.DrawHistos1DSideLG(1);         can33->Write(); // B3 LG charge 1D for 2 detectors on one side
  //TCanvas * can34 = hqB3.DrawHistos1DSideHG(1);         can34->Write(); // B3 HG charge 1D for 2 detectors on one side
  //TCanvas * can35 = hqB3.DrawHistosDiscriSideLG(1);     can35->Write(); // B3 LG charge Discri for 2 detectors on one side
  //TCanvas * can36 = hqB3.DrawHistosDiscriSideHG(1);     can36->Write(); // B3 HG charge Discri for 2 detectors on one side
  //TCanvas * can37 = hqB3.DrawHistos1DAllLG();           can37->Write(); // B3 LG charge 1D for all detectors
  //TCanvas * can38 = hqB3.DrawHistos1DAllHG();           can38->Write(); // B3 HG charge 1D for all detectors
  TCanvas * can39 = hqB3.DrawHistosDiscriAllLG();       can39->Write(); // B3 LG charge Discri for all detectors
  //TCanvas * can40 = hqB3.DrawHistosDiscriAllHG();       can40->Write(); // B3 HG charge Discri for all detectors
#if FC>0
  //TCanvas * can41 = htB3.DrawOneAnodeOneDetLG(6,1,2);   can41->Write();  //FC-B3lg ToFraw for one anode and one det
  //TCanvas * can42 = htB3.DrawOneAnodeOneDetHG(6,1,2);   can42->Write();  //FC-B3hg ToFraw for one anode and one det
  //TCanvas * can43 = htB3.DrawAllAnodesOneDetLG(1,2);    can43->Write();  //FC-B3lg ToFraw for all anode and one det
  //TCanvas * can44 = htB3.DrawAllAnodesOneDetHG(1,2);    can44->Write();  //FC-B3hg ToFraw for all anode and one det
  //TCanvas * can45 = htB3.DrawOneAnodeAllDetsLG(6);      can45->Write();  //FC-B3lg ToFraw for one anode and all dets
  //TCanvas * can46 = htB3.DrawOneAnodeAllDetsHG(6);      can46->Write();  //FC-B3hg ToFraw for one anode and all dets
  //TCanvas * can47 = htB3.DrawAllAnodesAllDetsLG();      can47->Write();  //FC-B3lg ToFraw for all anodesxs and all dets
  //TCanvas * can48 = htB3.DrawAllAnodesAllDetsHG();      can48->Write();  //FC-B3hg ToFraw for all anodes and all dets
#endif
#endif



 fsave->Close();
  // === ========== === //
  // === STATISTICS === //
  // === ========== === //
  cout << "number of events : " << nentries << endl;
  cout << "number of events with NDETlg : " << cpt_NDETlg << " and NDEThg : " << cpt_NDEThg << endl;
  
}
