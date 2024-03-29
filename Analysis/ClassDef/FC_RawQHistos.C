#include "FC_RawQHistos.h"

FC_RawQHistos::FC_RawQHistos()
  : fFlag1D(0)
  , fFlagDiscri(0)
{
}

FC_RawQHistos::~FC_RawQHistos()
{
  fFlag1D=0;
  fFlagDiscri=0;
}

Bool_t FC_RawQHistos::TestAnode(UShort_t anode, UShort_t flag)
{
  UShort_t bit=0;
  bit = bit | (1<<(anode-1));
  return( ((flag & bit) >> (anode-1)) & 0x1);

}

void FC_RawQHistos::Define1D(UShort_t anode)
{
  char name[100];
  if(!TestAnode(anode,fFlag1D)){
    sprintf(name,"FC_Q1_anode%d",anode);
    h1_Q1[anode-1] = new TH1I(name,name,340,0,34000);
    h1_Q1[anode-1]->SetLineColor(kBlue);
    sprintf(name,"FC_Q2_anode%d",anode);
    h1_Q2[anode-1] = new TH1I(name,name,250,0,10000);
    h1_Q2[anode-1]->SetLineColor(kRed);
    fFlag1D = fFlag1D | (1<<(anode-1));    
  }
}


void FC_RawQHistos::DefineDiscri(UShort_t anode)
{
  char name[100];
  if(!TestAnode(anode,fFlagDiscri)){
    sprintf(name,"FC_Q2vsQ1_anode%d",anode);
    h2_Q2vsQ1[anode-1] = new TH2I(name,name,2500,0,50000,500,0,10000);
    fFlagDiscri = fFlagDiscri | (1<<(anode-1));    
  }
}





void FC_RawQHistos::FillHistos(vector<UShort_t>* vAnode, vector<UInt_t>* vQ1, vector<UInt_t>* vQ2)
{
  UShort_t anode;
  for(UShort_t m=0; m<vAnode->size(); m++){
    anode = vAnode->at(m);
    if(TestAnode(anode,fFlag1D)){
      h1_Q1[anode-1]->Fill(vQ1->at(m));
      h1_Q2[anode-1]->Fill(vQ2->at(m));
    }
    if(TestAnode(anode,fFlagDiscri))
      h2_Q2vsQ1[anode-1]->Fill(vQ1->at(m),vQ2->at(m));
  }
}

TCanvas * FC_RawQHistos::Draw1DHistos(UShort_t anode)
{
  char name[100];
  sprintf(name,"Q1andQ2_FC%d",anode);
  TCanvas * c = new TCanvas(name,name,1000,500);
  c->Divide(2,1);
  if(TestAnode(anode,fFlag1D)){
    c->cd(1); h1_Q1[anode-1]->Draw();h1_Q1[anode-1]->SetDirectory(0);
    c->cd(2); h1_Q2[anode-1]->Draw();h1_Q2[anode-1]->SetDirectory(0);
  }
  return(c);
}
TCanvas * FC_RawQHistos::Draw1DAllHistos()
{
  char name[100];
  sprintf(name,"Q1andQ2_FC");
  TCanvas * c = new TCanvas(name,name,1000,1000);
  c->Divide(4,3);
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    if(TestAnode(anode,fFlag1D)){
      c->cd(anode); 
      h1_Q2[anode-1]->Draw(); h1_Q2[anode-1]->SetDirectory(0);
      h1_Q1[anode-1]->Draw("same");h1_Q1[anode-1]->SetDirectory(0);
    }
  }
  return(c);
}
TCanvas * FC_RawQHistos::DrawDiscriHisto(UShort_t anode)
{
  char name[100];
  sprintf(name,"Q2vsQ1_FC%d",anode);
  TCanvas * c = new TCanvas(name,name,700,700);
  gPad->SetLogz();
  if(TestAnode(anode,fFlagDiscri)){
    h2_Q2vsQ1[anode-1]->Draw("COL");h2_Q2vsQ1[anode-1]->SetDirectory(0);
  }
  return(c);
}
TCanvas * FC_RawQHistos::DrawDiscriAllHistos()
{
  char name[100];
  sprintf(name,"Q2vsQ1_FC");
  TCanvas * c = new TCanvas(name,name,1000,1000);
  c->Divide(4,3);
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    if(TestAnode(anode,fFlagDiscri)){
      c->cd(anode);   gPad->SetLogz();
      h2_Q2vsQ1[anode-1]->Draw("COL");h2_Q2vsQ1[anode-1]->SetDirectory(0);
    }
  }
  return(c);
}



