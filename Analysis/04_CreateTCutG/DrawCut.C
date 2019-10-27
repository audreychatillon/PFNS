#include "TCanvas.h"
#include "TCutG.h"
#include "TFile.h"
#include "TH1.h"

#include "../../SetupSpecific/setup_specific.h"
//////////////////////////////////////////
void DrawCut()
{
  // === =========== === //
  // === Q2/Q1 vs Q1 === //
  // === =========== === //
  //TFile* histofile = new TFile("histos/histo_Qdiscri_gamma_CHINUlg.root");
  //TFile* histofile = new TFile("histos/histo_Qdiscri_gamma_CHINUhg.root");
  //TFile* histofile = new TFile("histos/histo_Qdiscri_gamma_B3lg.root");
  TFile* histofile = new TFile("histos/histo_Qdiscri_gamma_B3hg.root");

  // === ========================== === //
  // === TCutG file for Q2/Q1 vs Q1 === //
  // === ========================== === //
  //TFile* cutfile = new TFile("cut/cut_discri_gamma_CHINUlg.root","update");
  //TFile* cutfile = new TFile("cut/cut_discri_gamma_CHINUhg.root","update");
  //TFile* cutfile = new TFile("cut/cut_discri_gamma_B3lg.root","update");
  TFile* cutfile = new TFile("cut/cut_discri_gamma_B3hg.root","update");

  TCanvas* c1 = new TCanvas("c1","c1",1200,1200);


  //for(int det_number=1; det_number<=CHINU_nDets; det_number++){
  for(int det_number=1; det_number<=B3_nDets; det_number++){
    
    // === ================== === //
    // === Name of the histos === //
    // === ================== === //
    //TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Qdiscri_gamma_CHINUlg_%i",det_number));
    //TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Qdiscri_gamma_CHINUhg_%i",det_number));
    //TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Qdiscri_gamma_B3lg_%i",det_number));
    TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Qdiscri_gamma_B3hg_%i",det_number));

    h->Draw("colz");

    TCutG* cutg;
    cutg = (TCutG*)gPad->WaitPrimitive("CUTG","CutG");
    cutg->SetName(Form("cut_discri_gamma_%i",det_number));
    cutfile->cd();
    cutg->Write();
  }
  histofile->Close();
  cutfile->Close();

}

/////////////////////////////////////////
