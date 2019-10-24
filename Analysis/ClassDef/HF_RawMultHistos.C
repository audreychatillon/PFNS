#include "HF_RawMultHistos.h"


HF_RawMultHistos::HF_RawMultHistos()
{
  DefineHistos();
}

HF_RawMultHistos::~HF_RawMultHistos()
{
}

void HF_RawMultHistos::DefineHistos()
{
  char name[100];
  h1_HF = new TH1I("HF_mult","HF_mult",10,-0.5,9.5);
  h1_HF->SetDirectory(0);
  h1_HFpattern = new TH1I("HFpattern","HFpattern",13,1.5,14.5); 
  h1_HFpattern->SetDirectory(0);
}

void HF_RawMultHistos::FillHistos(vector<UShort_t> * vHF)
{
  h1_HF->Fill(vHF->size());
  for(UShort_t m=0; m<vHF->size();m++)
    h1_HFpattern->Fill(vHF->at(m));
}

TCanvas * HF_RawMultHistos::DrawHistos()
{
  TCanvas * c1 = new TCanvas("MultHF","MultHF",0,0,1000,500);
  c1->Divide(2,1);
  c1->cd(1);  gPad->SetLogy(); h1_HF->Draw(); 
  c1->cd(2);  h1_HFpattern->Draw(); 
  return(c1);
}


