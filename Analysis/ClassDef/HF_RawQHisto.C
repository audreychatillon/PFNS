#include "HF_RawQHisto.h"


HF_RawQHisto::HF_RawQHisto()
{
  DefineHisto();
}

HF_RawQHisto::~HF_RawQHisto()
{
}

void HF_RawQHisto::DefineHisto()
{
  char name[100];
  h1_Q1 = new TH1I("HF_Q1","HF_Q1",1025,0,32800);
  h1_Q1->SetDirectory(0);
}

void HF_RawQHisto::FillHisto(vector<UShort_t> * vHF)
{
  for(UShort_t m=0; m<vHF->size();m++)
    h1_Q1->Fill(vHF->at(m));
}

TCanvas * HF_RawQHisto::DrawHisto()
{
  TCanvas * c = new TCanvas("Q1HF","Q1HF",0,0,500,500);
  c->cd();  gPad->SetLogy(); h1_Q1->Draw(); 
  return(c);
}


