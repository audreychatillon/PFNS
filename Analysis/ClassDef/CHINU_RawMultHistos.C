#include "CHINU_RawMultHistos.h"


CHINU_RawMultHistos::CHINU_RawMultHistos()
{
  DefineHistos();
}

CHINU_RawMultHistos::~CHINU_RawMultHistos()
{
}

void CHINU_RawMultHistos::DefineHistos()
{
  char name[100];

  h1_LG = new TH1I("CHINU_LG_mult","CHINU_LG_mult",10,-0.5,9.5);
  h1_LG->SetLineColor(kBlue);
  h1_LG->SetDirectory(0);
  h1_HG = new TH1I("CHINU_HG_mult","CHINU_HG_mult",10,-0.5,9.5);
  h1_HG->SetLineColor(kRed);
  h1_HG->SetDirectory(0);

  for(UShort_t s=0; s<2; s++){
    for(UShort_t r=0; r<3; r++){
      sprintf(name,"ChiNu_LG_%s_%s_pattern",sideVal[s].Data(),ringVal[r].Data());
      h1_LGpattern[3*s+r] = new TH1I(name,name,11,-0.5,10.5);
      h1_LGpattern[3*s+r]->SetLineColor(kBlue);
      sprintf(name,"ChiNu_HG_%s_%s_pattern",sideVal[s].Data(),ringVal[r].Data());
      h1_HGpattern[3*s+r] = new TH1I(name,name,11,-0.5,10.5);
      h1_HGpattern[3*s+r]->SetLineColor(kRed);
    }
  }

}

void CHINU_RawMultHistos::FillHistos(vector<UShort_t>*vLG, vector<UShort_t>*vHG)
{
  int side,ring,rank;

  h1_LG->Fill(vLG->size());
  h1_HG->Fill(vHG->size());
  for(UShort_t m=0; m<vLG->size();m++){
    side = Side_ChiNu[vLG->at(m)-1];
    ring = Ring_ChiNu[vLG->at(m)-1];
    rank = Rank_ChiNu[vLG->at(m)-1];
    h1_LGpattern[3*(side-1)+ring-1]->Fill(rank);
  }
  for(UShort_t m=0; m<vHG->size();m++){
    side = Side_ChiNu[vHG->at(m)-1];
    ring = Ring_ChiNu[vHG->at(m)-1];
    rank = Rank_ChiNu[vHG->at(m)-1];
    h1_HGpattern[3*(side-1)+ring-1]->Fill(rank);
  }
}

TCanvas * CHINU_RawMultHistos::DrawHistos()
{
  TCanvas * c = new TCanvas("MultNDET","MultNDET",0,0,1000,1500);
  c->Divide(2,4);
  c->cd(1);  gPad->SetLogy(); h1_LG->Draw();
  c->cd(2);  gPad->SetLogy(); h1_HG->Draw();
  c->cd(3);  h1_HGpattern[0]->Draw(); h1_LGpattern[0]->Draw("same");
  c->cd(4);  h1_HGpattern[3]->Draw(); h1_LGpattern[3]->Draw("same");
  c->cd(5);  h1_HGpattern[1]->Draw(); h1_LGpattern[1]->Draw("same");
  c->cd(6);  h1_HGpattern[4]->Draw(); h1_LGpattern[4]->Draw("same");
  c->cd(7);  h1_HGpattern[2]->Draw(); h1_LGpattern[2]->Draw("same");
  c->cd(8);  h1_HGpattern[5]->Draw(); h1_LGpattern[5]->Draw("same");
  return(c);

}
