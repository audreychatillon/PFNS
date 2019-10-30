#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <TCanvas.h>
#include <TChain.h>
#include <TCutG.h>
#include <TF1.h>
#include <TH1.h>
#include <TGraph.h>
#include <TString.h>

#include "../../Faster2Root/ReadPid/pid.h"
#include "../RunRefs/RunRefs.h"
#include "../ClassDef/RawCoincData.h"
#include "../ClassDef/CHINU_RawToFHistos.h"
#include "../ClassDef/B3_RawToFHistos.h"
#include "../../SetupSpecific/setup_specific.h"

void run(UInt_t runFirst, UInt_t runLast, TString dataType, TString dirpath)
{
  char name[100];

  // === ========================== === //
  // === ALPHA / NEUTRON TCutG FILE === //
  // === ========================== === //
  sprintf(name,"cut/cutFC_Q2vsQ1_R103.root");
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

  // ToF FC-CHINU
#if FC>0 && CHINU>0
  CHINU_RawToFHistos htCHINU;
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++)
    for(UShort_t side=1; side<=2; side++)
      for(UShort_t ring=1; ring<=3; ring++)
	for(UShort_t rank=1; rank<=9; rank++){
	  htCHINU.DefineOneAnodeOneDetLG(anode,side,ring,rank);
	  htCHINU.DefineOneAnodeOneDetHG(anode,side,ring,rank);
	}
#endif

  // === ===================================================================================== === //
  // === DECLARATION OF ALL ToF HISTOGRAMMS FOR B3 : 11 anodes * 4 LG channels * 4 HG channels === //
  // === ===================================================================================== === //

  // ToF FC-B3NDET
#if FC>0 && B3>0
  B3_RawToFHistos htB3;
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++)
    for(UShort_t side=1; side<=2; side++)
      for(UShort_t pos=1; pos<=2; pos++){
	htB3.DefineOneAnodeOneDetLG(anode,side,pos);
	htB3.DefineOneAnodeOneDetHG(anode,side,pos);
      }
#endif


  // === ====================================================== === //
  // === LOOP OVER THE ENTRIES AND FILL THE RAW ToF HISTOGRAMMS === //
  // === ====================================================== === //
  for(ULong64_t Entry=0; Entry<nentries; Entry++){
    raw.GetEntry(Entry);
    if((Entry%1000000)==0) cout << Entry << endl;
    
#if FC>0
    if(raw.vFC_anode->size()!=1) continue;    
    if(!cut[raw.vFC_anode->at(0)-1]->IsInside(raw.vFC_Q1->at(0),raw.vFC_Q2->at(0))) continue;
#if CHINU>0
    if(!raw.vCHINUlg_det->empty()) 
      htCHINU.FillHistosLG_FCmult1(raw.vFC_anode->at(0), raw.vFC_time->at(0),raw.vCHINUlg_det,raw.vCHINUlg_time,raw.CHINUlg_mult);
    if(!raw.vCHINUhg_det->empty()) 
      htCHINU.FillHistosHG_FCmult1(raw.vFC_anode->at(0), raw.vFC_time->at(0),raw.vCHINUhg_det,raw.vCHINUhg_time,raw.CHINUhg_mult);
#endif
#if B3>0
    if(!raw.vB3lg_det->empty()) 
      htB3.FillHistosLG_FCmult1(raw.vFC_anode->at(0), raw.vFC_time->at(0),raw.vB3lg_det,raw.vB3lg_time,raw.B3lg_mult);
    if(!raw.vB3hg_det->empty()) 
      htB3.FillHistosHG_FCmult1(raw.vFC_anode->at(0), raw.vFC_time->at(0),raw.vB3hg_det,raw.vB3hg_time,raw.B3hg_mult);
#endif
#endif



  }//end of for(Entry)


  // === ========================================================================== === //
  // === FIT THE GAMMA PEAK AND WRITE THE MEAN VALUE IN AN ARRAY COPIED IN .h FILES === //
  // === ========================================================================== === //

  UShort_t det;
  Int_t    binMax;


#if FC>0 && CHINU>0
  ofstream CHINU_GammaPeakLG("../../SetupSpecific/FC_to_CHINUlg_GammaPeak.h",ios::out | ios::trunc);
  ofstream CHINU_GammaPeakHG("../../SetupSpecific/FC_to_CHINUhg_GammaPeak.h",ios::out | ios::trunc);
  ofstream CHINU_OffsetToFLG("../../SetupSpecific/CHINUlg_OffsetToF.h",ios::out | ios::trunc);
  ofstream CHINU_OffsetToFHG("../../SetupSpecific/CHINUhg_OffsetToF.h",ios::out | ios::trunc);
  Float_t GammaPeak_LG_Mean[CHINU_nDets*FC_nAnodes];
  Float_t GammaPeak_HG_Mean[CHINU_nDets*FC_nAnodes];
  Double_t RealTimeCHINU[CHINU_nDets];
  Double_t OffsetCHINULG[CHINU_nDets*FC_nAnodes];
  Double_t OffsetCHINUHG[CHINU_nDets*FC_nAnodes];
  TGraph * grCHINUlg[FC_nAnodes];
  TGraph * grCHINUhg[FC_nAnodes];
  UShort_t ptlg[FC_nAnodes];
  UShort_t pthg[FC_nAnodes];
  for(UShort_t anode=0; anode<FC_nAnodes; anode++){
    grCHINUlg[anode] = new TGraph();
    sprintf(name,"CHINUlg_FC%d_SigmaPeak",anode+1);
    grCHINUlg[anode]->SetName(name);
    grCHINUlg[anode]->SetTitle(name);
    grCHINUlg[anode]->SetMarkerColor(kBlue);
    grCHINUlg[anode]->SetMarkerStyle(20);
    grCHINUhg[anode] = new TGraph();
    ptlg[anode]=0;
    sprintf(name,"CHINUhg_FC%d_SigmaPeak",anode+1);
    grCHINUhg[anode]->SetName(name);
    grCHINUhg[anode]->SetTitle(name);
    grCHINUhg[anode]->SetMarkerColor(kRed );
    grCHINUhg[anode]->SetMarkerStyle(20);
    pthg[anode]=0;
  }
  for(UShort_t side=1; side<=2; side ++){
    for(UShort_t ring=1; ring<=3; ring++){
      for(UShort_t rank=1; rank<=9; rank++){
	det=htCHINU.GetDet(side,ring,rank);
	RealTimeCHINU[det-1] = Distance_FC_ChiNu[det-1]/SPEED_OF_LIGHT_MNS;
	  for(UShort_t anode=1; anode<=11; anode++){
	    OffsetCHINULG[det-1+CHINU_nDets*(anode-1)] = 0;
	    OffsetCHINUHG[det-1+CHINU_nDets*(anode-1)] = 0;
	    GammaPeak_LG_Mean[det-1+CHINU_nDets*(anode-1)]=0.;
	    GammaPeak_HG_Mean[det-1+CHINU_nDets*(anode-1)]=0.;
	    if((htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*(anode-1)]->Integral())>1000) {
	      binMax=htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*(anode-1)]->GetMaximumBin();
	      htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*(anode-1)]->Fit("gaus","","",htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*(anode-1)]->GetBinLowEdge(binMax)-1.5,htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*(anode-1)]->GetBinLowEdge(binMax)+1.5);
	      TF1 * fGaussianLG = (TF1*)gDirectory->FindObject("gaus");
	      sprintf(name,"FitGammaPeakLG_CHINU_%s_%s_%d_FC%d",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank,anode);
	      fGaussianLG->SetName(name);
	      fGaussianLG->SetTitle(name);
	      GammaPeak_LG_Mean[det-1+CHINU_nDets*(anode-1)]=fGaussianLG->GetParameter(1);
	      grCHINUlg[anode-1]->SetPoint(ptlg[anode-1],det,fGaussianLG->GetParameter(2));
	      OffsetCHINULG[det-1+CHINU_nDets*(anode-1)]=RealTimeCHINU[det-1]-GammaPeak_LG_Mean[det-1+CHINU_nDets*(anode-1)];
	      delete fGaussianLG;
	      ptlg[anode-1]++;
	    }// end of if(histo)
	    if((htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*(anode-1)]->Integral())>1000) {
	      binMax=htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*(anode-1)]->GetMaximumBin();
	      htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*(anode-1)]->Fit("gaus","","",htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*(anode-1)]->GetBinLowEdge(binMax)-1.5,htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*(anode-1)]->GetBinLowEdge(binMax)+1.5);
	      TF1 * fGaussianHG = (TF1*)gDirectory->FindObject("gaus");
	      sprintf(name,"FitGammaPeakHG_CHINU_%s_%s_%d_FC%d",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank,anode);
	      fGaussianHG->SetName(name);
	      fGaussianHG->SetTitle(name);
	      GammaPeak_HG_Mean[det-1+CHINU_nDets*(anode-1)]=fGaussianHG->GetParameter(1);
	      grCHINUhg[anode-1]->SetPoint(pthg[anode-1],det,fGaussianHG->GetParameter(2));
	      OffsetCHINUHG[det-1+CHINU_nDets*(anode-1)]=RealTimeCHINU[det-1]-GammaPeak_HG_Mean[det-1+CHINU_nDets*(anode-1)];
	      delete fGaussianHG;
	      pthg[anode-1]++;
	    }// end of if(histo)
	  }//end of for(rank)
      }//end of for(ring)
    }//end of for(side)
  }//end of for(anode)

  // --- write Gamma peak mean LG --- //
  CHINU_GammaPeakLG << "// File automatically generated from " << dataType.Data() << " runs (R"<< runFirst << " to R"<< runLast <<")" << endl<<endl;
  CHINU_GammaPeakLG << "#ifndef _FCCHINULG_GAMMA_PEAK__H_" << endl;
  CHINU_GammaPeakLG << "#define _FCCHINULG_GAMMA_PEAK__H_" << endl;
  CHINU_GammaPeakLG << "Float_t CHINU_GammaPeakLG["<<CHINU_nDets*FC_nAnodes<<"] = {" << endl;
  for(UShort_t anode=0; anode<FC_nAnodes-1; anode++){
    for(UShort_t det=0; det<CHINU_nDets; det++)  CHINU_GammaPeakLG << GammaPeak_LG_Mean[det+CHINU_nDets*anode] << "," ;
    CHINU_GammaPeakLG << endl;
  }
  for(UShort_t det=0; det<CHINU_nDets-1; det++)  CHINU_GammaPeakLG << GammaPeak_LG_Mean[det+CHINU_nDets*(FC_nAnodes-1)] << "," ;
  CHINU_GammaPeakHG <<  GammaPeak_LG_Mean[CHINU_nDets-1+CHINU_nDets*(FC_nAnodes-1)] << endl;
  CHINU_GammaPeakLG << "};" << endl;
  CHINU_GammaPeakLG <<  endl;
  CHINU_GammaPeakLG << "#endif //_FCCHINULG_GAMMA_PEAK__H_" << endl;

  // --- write Offset ToF LG --- //
  CHINU_OffsetToFLG << "// File automatically generated from " << dataType.Data() << " runs (R"<< runFirst << " to R"<< runLast <<")" << endl<<endl;
  CHINU_OffsetToFLG << "#ifndef _CHINULG_OFFSET_TOF__H_" << endl;
  CHINU_OffsetToFLG << "#define _CHINULG_OFFSET_TOF__H_" << endl;
  CHINU_OffsetToFLG << "Float_t CHINU_OffsetToFLG["<<CHINU_nDets*FC_nAnodes<<"] = {" << endl;
  for(UShort_t anode=0; anode<FC_nAnodes-1; anode++){
    for(UShort_t det=0; det<CHINU_nDets; det++)  CHINU_OffsetToFLG << OffsetCHINULG[det+CHINU_nDets*anode] << "," ;
    CHINU_OffsetToFLG << endl;
  }
  for(UShort_t det=0; det<CHINU_nDets-1; det++)  CHINU_OffsetToFLG << OffsetCHINULG[det+CHINU_nDets*(FC_nAnodes-1)] << "," ;
  CHINU_OffsetToFLG <<  OffsetCHINULG[CHINU_nDets-1+CHINU_nDets*(FC_nAnodes-1)] << endl;
  CHINU_OffsetToFLG << "};" << endl;
  CHINU_OffsetToFLG <<  endl;
  CHINU_OffsetToFLG << "#endif //_FCCHINULG_GAMMA_PEAK__H_" << endl;

  // --- write Gamma peak mean HG --- //
  CHINU_GammaPeakHG << "// File automatically generated from " << dataType.Data() << " runs (R"<< runFirst << " to R"<< runLast <<")" << endl;
  CHINU_GammaPeakHG << "#ifndef _FCCHINUHG_GAMMA_PEAK__H_" << endl;
  CHINU_GammaPeakHG << "#define _FCCHINUHG_GAMMA_PEAK__H_" << endl;
  CHINU_GammaPeakHG << "Float_t CHINU_GammaPeakHG["<<CHINU_nDets*FC_nAnodes<<"] = {" << endl;
  for(UShort_t anode=0; anode<FC_nAnodes-1; anode++){
    for(UShort_t det=0; det<CHINU_nDets; det++)  CHINU_GammaPeakHG << GammaPeak_HG_Mean[det+CHINU_nDets*anode] << "," ;
    CHINU_GammaPeakHG << endl;
  }
  for(UShort_t det=0; det<CHINU_nDets-1; det++)  CHINU_GammaPeakHG << GammaPeak_HG_Mean[det+CHINU_nDets*(FC_nAnodes-1)] << "," ;
  CHINU_GammaPeakHG <<  GammaPeak_HG_Mean[CHINU_nDets-1+CHINU_nDets*(FC_nAnodes-1)] << endl;
  CHINU_GammaPeakHG << "};" << endl;
  CHINU_GammaPeakHG <<  endl;
  CHINU_GammaPeakHG << "#endif //_FCCHINUHG_GAMMA_PEAK__H_" << endl;

  // --- write Offset ToF HG --- //
  CHINU_OffsetToFHG << "// File automatically generated from " << dataType.Data() << " runs (R"<< runFirst << " to R"<< runLast <<")" << endl;
  CHINU_OffsetToFHG << "#ifndef _CHINUHG_OFFSET_TOF__H_" << endl;
  CHINU_OffsetToFHG << "#define _CHINUHG_OFFSET_TOF__H_" << endl;
  CHINU_OffsetToFHG << "Float_t CHINU_OffsetToFHG["<<CHINU_nDets*FC_nAnodes<<"] = {" << endl;
  for(UShort_t anode=0; anode<FC_nAnodes-1; anode++){
    for(UShort_t det=0; det<CHINU_nDets; det++)  CHINU_OffsetToFHG << OffsetCHINUHG[det+CHINU_nDets*anode] << "," ;
    CHINU_OffsetToFHG << endl;
  }
  for(UShort_t det=0; det<CHINU_nDets-1; det++)  CHINU_OffsetToFHG << OffsetCHINUHG[det+CHINU_nDets*(FC_nAnodes-1)] << "," ;
  CHINU_OffsetToFHG <<  OffsetCHINUHG[CHINU_nDets-1+CHINU_nDets*(FC_nAnodes-1)] << endl;
  CHINU_OffsetToFHG << "};" << endl;
  CHINU_OffsetToFHG <<  endl;
  CHINU_OffsetToFHG << "#endif //_FCCHINUHG_GAMMA_PEAK__H_" << endl;
#endif
  
#if FC>0 && B3>0
  ofstream B3_GammaPeakLG("../../SetupSpecific/FC_to_B3lg_GammaPeak.h",ios::out | ios::trunc);
  ofstream B3_GammaPeakHG("../../SetupSpecific/FC_to_B3hg_GammaPeak.h",ios::out | ios::trunc);
  ofstream B3_OffsetToFLG("../../SetupSpecific/B3lg_OffsetToF.h",ios::out | ios::trunc);
  ofstream B3_OffsetToFHG("../../SetupSpecific/B3hg_OffsetToF.h",ios::out | ios::trunc);
  Float_t GammaPeak_B3LG_Mean[B3_nDets*FC_nAnodes];
  Float_t GammaPeak_B3HG_Mean[B3_nDets*FC_nAnodes];
  Double_t RealTimeB3[B3_nDets];
  Double_t OffsetB3LG[B3_nDets*FC_nAnodes];
  Double_t OffsetB3HG[B3_nDets*FC_nAnodes];
  for(UShort_t side=1; side<=2; side ++){
    for(UShort_t pos=1; pos<=2; pos++){
      det=htB3.GetDet(side,pos);
      RealTimeB3[det-1] = Distance_FC_B3[det-1]/SPEED_OF_LIGHT_MNS;
      for(UShort_t anode=1; anode<=11; anode++){
	OffsetB3LG[det-1+B3_nDets*(anode-1)] = 0;
	OffsetB3HG[det-1+B3_nDets*(anode-1)] = 0;
	GammaPeak_B3LG_Mean[det-1+B3_nDets*(anode-1)]=0.;
	GammaPeak_B3HG_Mean[det-1+B3_nDets*(anode-1)]=0.;
	if((htB3.h1_RawToF_LG[det-1+B3_nDets*(anode-1)]->Integral())>100) {
	  binMax=htB3.h1_RawToF_LG[det-1+B3_nDets*(anode-1)]->GetMaximumBin();
	  htB3.h1_RawToF_LG[det-1+B3_nDets*(anode-1)]->Fit("gaus","","",htB3.h1_RawToF_LG[det-1+B3_nDets*(anode-1)]->GetBinLowEdge(binMax)-1.5,htB3.h1_RawToF_LG[det-1+B3_nDets*(anode-1)]->GetBinLowEdge(binMax)+1.5);
	  TF1 * fGaussianLG = (TF1*)gDirectory->FindObject("gaus");
	  sprintf(name,"FitGammaPeakLG_B3_%s_%s_FC%d",sideVal[side-1].Data(),posVal[pos-1].Data(),anode);
	  fGaussianLG->SetName(name);
	  fGaussianLG->SetTitle(name);
	  GammaPeak_B3LG_Mean[det-1+B3_nDets*(anode-1)]=fGaussianLG->GetParameter(1);
	  OffsetB3LG[det-1+B3_nDets*(anode-1)]=RealTimeB3[det-1]-GammaPeak_B3LG_Mean[det-1+B3_nDets*(anode-1)];
	  delete fGaussianLG;
	}// end of if(histo)
	if((htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->Integral())>100) {
	  binMax=htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->GetMaximumBin();
	  htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->Fit("gaus","","",htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->GetBinLowEdge(binMax)-1.5,htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->GetBinLowEdge(binMax)+1.5);
	  TF1 * fGaussianHG = (TF1*)gDirectory->FindObject("gaus");
	  sprintf(name,"FitGammaPeakHG_B3_%s_%s_FC%d",sideVal[side-1].Data(),posVal[pos-1].Data(),anode);
	  fGaussianHG->SetName(name);
	  fGaussianHG->SetTitle(name);
	  GammaPeak_B3HG_Mean[det-1+B3_nDets*(anode-1)]=fGaussianHG->GetParameter(1);
	  OffsetB3HG[det-1+B3_nDets*(anode-1)]=RealTimeB3[det-1]-GammaPeak_B3HG_Mean[det-1+B3_nDets*(anode-1)];
	  delete fGaussianHG;
	}// end of if(histo)
      }//end of for(pos)
    }//end of for(side)
  }//end of for(anode)

  // --- write Gamma peak mean LG --- //
  B3_GammaPeakLG << "// File automatically generated from " << dataType.Data() << " runs (R"<< runFirst << " to R"<< runLast <<")" << endl<<endl;
  B3_GammaPeakLG << "#ifndef _FCB3LG_GAMMA_PEAK__H_" << endl;
  B3_GammaPeakLG << "#define _FCB3LG_GAMMA_PEAK__H_" << endl;
  B3_GammaPeakLG << "Float_t B3_GammaPeakLG["<<B3_nDets*FC_nAnodes<<"] = {" << endl;
  for(UShort_t anode=0; anode<FC_nAnodes-1; anode++){
    for(UShort_t det=0; det<B3_nDets; det++)  B3_GammaPeakLG << GammaPeak_B3LG_Mean[det+B3_nDets*anode] << "," ;
    B3_GammaPeakLG << endl;
  }
  for(UShort_t det=0; det<B3_nDets-1; det++)  B3_GammaPeakLG << GammaPeak_B3LG_Mean[det+B3_nDets*(FC_nAnodes-1)] << "," ;
  B3_GammaPeakLG <<  GammaPeak_B3LG_Mean[B3_nDets-1+B3_nDets*(FC_nAnodes-1)] << endl;
  B3_GammaPeakLG << "};" << endl;
  B3_GammaPeakLG <<  endl;
  B3_GammaPeakLG << "#endif //_FCB3LG_GAMMA_PEAK__H_" << endl;

  // --- write Offset ToF B3LG --- //
  B3_OffsetToFLG << "// File automatically generated from " << dataType.Data() << " runs (R"<< runFirst << " to R"<< runLast <<")" << endl<<endl;
  B3_OffsetToFLG << "#ifndef _B3LG_OFFSET_TOF__H_" << endl;
  B3_OffsetToFLG << "#define _B3LG_OFFSET_TOF__H_" << endl;
  B3_OffsetToFLG << "Float_t B3_OffsetToFLG["<<B3_nDets*FC_nAnodes<<"] = {" << endl;
  for(UShort_t anode=0; anode<FC_nAnodes-1; anode++){
    for(UShort_t det=0; det<B3_nDets; det++)  B3_OffsetToFLG << OffsetB3LG[det+B3_nDets*anode] << "," ;
    B3_OffsetToFLG << endl;
  }
  for(UShort_t det=0; det<B3_nDets-1; det++)  B3_OffsetToFLG << OffsetB3LG[det+B3_nDets*(FC_nAnodes-1)] << "," ;
  B3_OffsetToFLG <<  OffsetB3LG[B3_nDets-1+B3_nDets*(FC_nAnodes-1)] << endl;
  B3_OffsetToFLG << "};" << endl;
  B3_OffsetToFLG <<  endl;
  B3_OffsetToFLG << "#endif //_FCB3LG_GAMMA_PEAK__H_" << endl;
  
  // --- write Gamma peak mean HG --- //
  B3_GammaPeakHG << "// File automatically generated from " << dataType.Data() << " runs (R"<< runFirst << " to R"<< runLast <<")" << endl;
  B3_GammaPeakHG << "#ifndef _FCB3HG_GAMMA_PEAK__H_" << endl;
  B3_GammaPeakHG << "#define _FCB3HG_GAMMA_PEAK__H_" << endl;
  B3_GammaPeakLG << "Float_t B3_GammaPeakLG["<<B3_nDets*FC_nAnodes<<"] = {" << endl;
  for(UShort_t anode=0; anode<FC_nAnodes; anode++){
    B3_GammaPeakHG << GammaPeak_B3HG_Mean[0] ;
    for(UShort_t det=1; det<B3_nDets; det++)  B3_GammaPeakHG << ", " <<GammaPeak_B3HG_Mean[det+B3_nDets*anode] ;
    B3_GammaPeakHG << endl;
  }
  B3_GammaPeakHG << "};" << endl;
  B3_GammaPeakHG <<  endl;
  B3_GammaPeakHG << "#endif //_FCB3HG_GAMMA_PEAK__H_" << endl;

  // --- write Offset ToF B3HG --- //
  B3_OffsetToFHG << "// File automatically generated from " << dataType.Data() << " runs (R"<< runFirst << " to R"<< runLast <<")" << endl<<endl;
  B3_OffsetToFHG << "#ifndef _B3HG_OFFSET_TOF__H_" << endl;
  B3_OffsetToFHG << "#define _B3HG_OFFSET_TOF__H_" << endl;
  B3_OffsetToFHG << "Float_t B3_OffsetToFHG["<<B3_nDets*FC_nAnodes<<"] = {" << endl;
  for(UShort_t anode=0; anode<FC_nAnodes-1; anode++){
    for(UShort_t det=0; det<B3_nDets; det++)  B3_OffsetToFHG << OffsetB3HG[det+B3_nDets*anode] << "," ;
    B3_OffsetToFHG << endl;
  }
  for(UShort_t det=0; det<B3_nDets-1; det++)  B3_OffsetToFHG << OffsetB3HG[det+B3_nDets*(FC_nAnodes-1)] << "," ;
  B3_OffsetToFHG <<  OffsetB3HG[B3_nDets-1+B3_nDets*(FC_nAnodes-1)] << endl;
  B3_OffsetToFHG << "};" << endl;
  B3_OffsetToFHG <<  endl;
  B3_OffsetToFHG << "#endif //_FCB3HG_GAMMA_PEAK__H_" << endl;
#endif



  // --- DRAW --- //s
#if CHINU>0 && FC>0

  TCanvas * canSigmaLG = new TCanvas("Sigma_GammaPeak_CHINULG","Sigma_GammaPeak_CHINULG",0,0,2000,1800);
  canSigmaLG->Divide(4,3);
  TCanvas * canSigmaHG = new TCanvas("Sigma_GammaPeak_CHINUHG","Sigma_GammaPeak_CHINUHG",0,0,2000,1800);
  canSigmaHG->Divide(4,3);
  for(UShort_t anode=1; anode<FC_nAnodes; anode++){
    canSigmaLG->cd(anode);  gPad->SetGridx();  gPad->SetGridy();  grCHINUlg[anode-1]->Draw("ALP");
    canSigmaHG->cd(anode);  gPad->SetGridx();  gPad->SetGridy();  grCHINUhg[anode-1]->Draw("ALP");
  }


  TCanvas * can1 = new TCanvas("FitGammaPeak_FC6_CHINUlg","FitGammaPeak_FC6_CHINUlg",0,0,2000,1600);
  can1->Divide(9,6);
  TCanvas * can2 = new TCanvas("FitGammaPeak_FC6_CHINUhg","FitGammaPeak_FC6_CHINUhg",0,0,2000,1600);
  can2->Divide(9,6);
  for(UShort_t det=1; det<=CHINU_nDets; det++){

    htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*5]->SetDirectory(0);
    binMax=htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*5]->GetMaximumBin();
    can1->cd(det); 
    htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*5]->Draw();  
    htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*5]->GetXaxis()->SetRangeUser(htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*5]->GetBinLowEdge(binMax)-5,htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*5]->GetBinLowEdge(binMax)+5);

    htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*5]->SetLineColor(kBlack);
    htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*5]->SetDirectory(0);
    binMax=htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*5]->GetMaximumBin();
    can2->cd(det); 
    htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*5]->Draw();  
    htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*5]->GetXaxis()->SetRangeUser(htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*5]->GetBinLowEdge(binMax)-5,htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*5]->GetBinLowEdge(binMax)+5);
  }


  det=htCHINU.GetDet(1,2,5);
  TCanvas * can5 = new TCanvas("FitGammaPeak_AllFC_CHINUlg_L_II_5","FitGammaPeak_AllFC_CHINUlg_L_II_5",0,0,2000,1600);
  can5->Divide(4,3);
  TCanvas * can6 = new TCanvas("FitGammaPeak_AllFC_CHINUhg_L_II_5","FitGammaPeak_AllFC_CHINUhg_L_II_5",0,0,2000,1600);
  can6->Divide(4,3);
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    can5->cd(anode);
    binMax=htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*(anode-1)]->GetMaximumBin();
    htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*(anode-1)]->GetXaxis()->SetRangeUser(htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*5]->GetBinLowEdge(binMax)-5,htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*(anode-1)]->GetBinLowEdge(binMax)+5);
    htCHINU.h1_RawToF_LG[det-1+CHINU_nDets*(anode-1)]->Draw();  
    can6->cd(anode);
    binMax=htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*(anode-1)]->GetMaximumBin();
    htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*(anode-1)]->SetLineColor(kBlack);  
    htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*(anode-1)]->GetXaxis()->SetRangeUser(htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*5]->GetBinLowEdge(binMax)-5,htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*(anode-1)]->GetBinLowEdge(binMax)+5);
    htCHINU.h1_RawToF_HG[det-1+CHINU_nDets*(anode-1)]->Draw();  
  }
#endif


#if B3>0 && FC>0
  TCanvas * can3 = new TCanvas("FitGammaPeak_FC_B3lg","FitGammaPeak_FC_B3lg",0,0,2000,1000);
  can3->Divide(11,4);
  TCanvas * can4 = new TCanvas("FitGammaPeak_FC_B3hg","FitGammaPeak_FC_B3hg",0,0,2000,1000);
  can4->Divide(11,4);
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    for(UShort_t det=1; det<=B3_nDets; det++){

      htB3.h1_RawToF_LG[det-1+B3_nDets*(anode-1)]->SetDirectory(0);
      binMax=htB3.h1_RawToF_LG[det-1+B3_nDets*(anode-1)]->GetMaximumBin();
      htB3.h1_RawToF_LG[det-1+B3_nDets*(anode-1)]->GetXaxis()->SetRangeUser(htB3.h1_RawToF_LG[det-1+B3_nDets*(anode-1)]->GetBinLowEdge(binMax)-5,htB3.h1_RawToF_LG[det-1+B3_nDets*(anode-1)]->GetBinLowEdge(binMax)+5);
      can3->cd(anode+11*(det-1)); htB3.h1_RawToF_LG[det-1+B3_nDets*(anode-1)]->Draw();

      htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->SetDirectory(0);
      htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->SetLineColor(kBlack);
      binMax=htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->GetMaximumBin();
      htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->GetXaxis()->SetRangeUser(htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->GetBinLowEdge(binMax)-5,htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->GetBinLowEdge(binMax)+5);
      can4->cd(anode+11*(det-1)); htB3.h1_RawToF_HG[det-1+B3_nDets*(anode-1)]->Draw();
    }
  }
#endif



}
