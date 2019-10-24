#include "PULSER_RawMultHistos.h"


PULSER_RawMultHistos::PULSER_RawMultHistos()
{
  DefineHistos();
}

PULSER_RawMultHistos::~PULSER_RawMultHistos()
{
}

void PULSER_RawMultHistos::DefineHistos()
{
  char name[100];
  h1_PULSER = new TH1I("PULSER_mult","PULSER_mult",10,-0.5,9.5);
  h1_PULSER->SetDirectory(0);
  h1_PULSERpattern = new TH1I("PULSERpattern","PULSERpattern",13,1.5,14.5); 
  h1_PULSERpattern->SetDirectory(0);
}

void PULSER_RawMultHistos::FillHistos(vector<UShort_t> * vPULSER)
{
  h1_PULSER->Fill(vPULSER->size());
  for(UShort_t m=0; m<vPULSER->size();m++)
    h1_PULSERpattern->Fill(vPULSER->at(m));
}

TCanvas * PULSER_RawMultHistos::DrawHistos()
{
  TCanvas * c1 = new TCanvas("MultPULSER","MultPULSER",0,0,1000,500);
  c1->Divide(2,1);
  c1->cd(1);  gPad->SetLogy(); h1_PULSER->Draw(); 
  c1->cd(2);  h1_PULSERpattern->Draw(); 
  return(c1);
}


