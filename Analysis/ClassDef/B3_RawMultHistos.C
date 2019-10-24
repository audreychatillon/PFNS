#include "B3_RawMultHistos.h"


B3_RawMultHistos::B3_RawMultHistos()
{
  DefineHistos();
}

B3_RawMultHistos::~B3_RawMultHistos()
{
}

void B3_RawMultHistos::DefineHistos()
{
  char name[100];

  h1_LG = new TH1I("B3_LG_mult","B3_LG_mult",10,-0.5,9.5);
  h1_LG->SetLineColor(kBlue);
  h1_LG->SetDirectory(0);
  h1_HG = new TH1I("B3_HG_mult","B3_HG_mult",10,-0.5,9.5);
  h1_HG->SetLineColor(kRed);
  h1_HG->SetDirectory(0);

  for(UShort_t s=0; s<2; s++){
      sprintf(name,"B3_LG_%s_pattern",sideVal[s].Data());
      h1_LGpattern[s] = new TH1I(name,name,11,-0.5,10.5);
      h1_LGpattern[s]->SetLineColor(kBlue);
      sprintf(name,"B3_HG_%s_pattern",sideVal[s].Data());
      h1_HGpattern[s] = new TH1I(name,name,11,-0.5,10.5);
      h1_HGpattern[s]->SetLineColor(kRed);
  }

}

void B3_RawMultHistos::FillHistos(vector<UShort_t>*vLG, vector<UShort_t>*vHG)
{
  int side,pos;

  h1_LG->Fill(vLG->size());
  h1_HG->Fill(vHG->size());
  for(UShort_t m=0; m<vLG->size();m++){
    side = Side_B3[vLG->at(m)-1];
    pos = Pos_B3[vLG->at(m)-1];
    h1_LGpattern[(side-1)]->Fill(pos);
  }
  for(UShort_t m=0; m<vHG->size();m++){
    side = Side_B3[vHG->at(m)-1];
    pos = Pos_B3[vHG->at(m)-1];
    h1_HGpattern[side-1]->Fill(pos);
  }
}

TCanvas * B3_RawMultHistos::DrawHistos()
{
  TCanvas * c = new TCanvas("MultNDETB3","MultNDETB3",0,0,1000,900);
  c->Divide(2,2);
  c->cd(1);  gPad->SetLogy(); h1_LG->Draw(); 
  c->cd(2);  gPad->SetLogy(); h1_HG->Draw(); 
  c->cd(3);  h1_HGpattern[0]->Draw(); h1_LGpattern[0]->Draw("same");
  c->cd(4);  h1_HGpattern[1]->Draw(); h1_LGpattern[1]->Draw("same");
  return(c);

}
