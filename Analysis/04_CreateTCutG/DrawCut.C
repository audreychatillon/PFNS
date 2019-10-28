#include "TCanvas.h"
#include "TCutG.h"
#include "TFile.h"
#include "TH1.h"

#include "../../SetupSpecific/setup_specific.h"
//////////////////////////////////////////
void DrawCut()
{
  // === ================== === //
  // === EITHER Q2/Q1 vs Q1 === //
  // === ================== === //
  //TFile* histofile = new TFile("histos/histo_Qdiscri_gamma_CHINUlg.root");
  //TFile* histofile = new TFile("histos/histo_Qdiscri_gamma_CHINUhg.root");
  //TFile* histofile = new TFile("histos/histo_Qdiscri_gamma_B3lg.root");
  //TFile* histofile = new TFile("histos/histo_Qdiscri_gamma_B3hg.root");
  // === ========================== === //
  // === TCutG file for Q2/Q1 vs Q1 === //
  // === ========================== === //
  //TFile* cutfile = new TFile("cut/cut_discri_gamma_CHINUlg.root","update");
  //TFile* cutfile = new TFile("cut/cut_discri_gamma_CHINUhg.root","update");
  //TFile* cutfile = new TFile("cut/cut_discri_gamma_B3lg.root","update");
  //TFile* cutfile = new TFile("cut/cut_discri_gamma_B3hg.root","update");


  // === =============== === //
  // === OR Q1 vs ENERGY === //
  // === =============== === //
  //TFile* histofile = new TFile("histos/histo_Q1vsE_neutron_CHINUlg.root");
  //TFile* histofile = new TFile("histos/histo_Q1vsE_neutron_CHINUhg.root");
  TFile* histofile = new TFile("histos/histo_Q1vsE_neutron_B3lg.root");
  //TFile* histofile = new TFile("histos/histo_Q1vsE_neutron_B3hg.root");
  // === =========================== === //
  // === TCutG file for Q1 vs ENERGY === //
  // === =========================== === //
  //TFile* cutfile = new TFile("cut/cut_Q1vsE_CHINUlg.root","update");
  //TFile* cutfile = new TFile("cut/cut_Q1vsE_CHINUhg.root","update");
  TFile* cutfile = new TFile("cut/cut_Q1vsE_B3lg.root","update");
  //TFile* cutfile = new TFile("cut/cut_Q1vsE_B3hg.root","update");

  histofile->ls();

  TCanvas* c1 = new TCanvas("c1","c1",1200,1200);
  c1->cd(); 
  gPad->SetLogx(1);
  gPad->SetLogy(1);
  //for(int det_number=1; det_number<=CHINU_nDets; det_number++){
  for(int det_number=1; det_number<=B3_nDets; det_number++){
    
    // === ================================== === //
    // === Name of the histos for Q2/Q1 vs Q1 === //
    // === ================================== === //
    //TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Qdiscri_gamma_CHINUlg_%i",det_number));
    //TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Qdiscri_gamma_CHINUhg_%i",det_number));
    //TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Qdiscri_gamma_B3lg_%i",det_number));
    //TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Qdiscri_gamma_B3hg_%i",det_number));

    // === =================================== === //
    // === Name of the histos for Q1 vs Energy === //
    // === =================================== === //
    //TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Q1E_neutron_CHINUlg_%i",det_number));
    //TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Q1E_neutron_CHINUhg_%i",det_number));
    TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Q1E_neutron_B3lg_%i",det_number));
    //TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_Q1E_neutron_B3hg_%i",det_number));

    h->Draw("colz");

    TCutG* cutg;
    cutg = (TCutG*)gPad->WaitPrimitive("CUTG","CutG");
    //cutg->SetName(Form("cut_discri_gamma_%i",det_number));
    
    //cutg->SetName(Form("cut_Q1vsE_neutron_CHINUlg_%i",det_number));
    //cutg->SetName(Form("cut_Q1vsE_neutron_CHINUhg_%i",det_number));
    cutg->SetName(Form("cut_Q1vsE_neutron_B3lg_%i",det_number));
    //cutg->SetName(Form("cut_Q1vsE_neutron_B3hg_%i",det_number));
    cutfile->cd();
    cutg->Write();
  }
  histofile->Close();
  cutfile->Close();

}

/////////////////////////////////////////
