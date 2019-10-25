#include "FC_RawMultHistos.h"


FC_RawMultHistos::FC_RawMultHistos()
{
  DefineHistos();
}

FC_RawMultHistos::~FC_RawMultHistos()
{
}

void FC_RawMultHistos::DefineHistos()
{
  char name[100];
  h1_FC = new TH1I("FC_mult","FC_mult",10,-0.5,9.5);
  h1_FCpattern = new TH1I("FCpattern","FCpattern",13,-0.5,12.5); 
  for(UShort_t anode=0; anode<FC_nAnodes; anode++){
    sprintf(name,"MultTot_vs_MultAnode%d",anode+1);
    h2_MultTot_vs_MultAnode[anode] = new TH2I(name,name,20,-0.5,19.5,20,-0.5,19.5);
  }
}

void FC_RawMultHistos::FillHistos(vector<UShort_t> * vFC, UShort_t * mult)
{
  h1_FC->Fill(vFC->size());
  for(UShort_t m=0; m<vFC->size();m++)
    h1_FCpattern->Fill(vFC->at(m));
  for(UShort_t anode=0; anode<FC_nAnodes; anode++){
    if(mult[anode]>0) 
      h2_MultTot_vs_MultAnode[anode]->Fill(mult[anode],vFC->size());
  }
}

TCanvas * FC_RawMultHistos::Draw1DHistos()
{
  TCanvas * c1 = new TCanvas("MultFC","MultFC",0,0,1000,500);
  c1->Divide(2,1);
  c1->cd(1);  gPad->SetLogy(); h1_FC->Draw(); h1_FC->SetDirectory(0);
  c1->cd(2);  h1_FCpattern->Draw(); h1_FCpattern->SetDirectory(0);
  return(c1);
}

TCanvas * FC_RawMultHistos::Draw2DHistos()
{
  TCanvas * c = new TCanvas("MultTot_vs_MultAnode","MultTot_vs_MultAnode",0,0,1000,500);
  c->Divide(4,3);
  for(UShort_t anode=0; anode<FC_nAnodes; anode++){
    c->cd(anode+1);
    h2_MultTot_vs_MultAnode[anode]->Draw("COL");
    h2_MultTot_vs_MultAnode[anode]->SetDirectory(0);
  }    
  return(c);
}
