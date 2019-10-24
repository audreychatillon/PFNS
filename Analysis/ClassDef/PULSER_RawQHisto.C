#include "PULSER_RawQHisto.h"


PULSER_RawQHisto::PULSER_RawQHisto()
{
  DefineHisto();
}

PULSER_RawQHisto::~PULSER_RawQHisto()
{
}

void PULSER_RawQHisto::DefineHisto()
{
  char name[100];
  h1_Q1 = new TH1I("PULSER_Q1","PULSER_Q1",1025,0,32800);
  h1_Q1->SetDirectory(0);
}

void PULSER_RawQHisto::FillHisto(vector<UInt_t> * vPULSER)
{
  for(UShort_t m=0; m<vPULSER->size();m++)
    h1_Q1->Fill(vPULSER->at(m));
}

TCanvas * PULSER_RawQHisto::DrawHisto()
{
  TCanvas * c = new TCanvas("Q1PULSER","Q1PULSER",0,0,500,500);
  c->cd();  gPad->SetLogy(); h1_Q1->Draw(); 
  return(c);
}


