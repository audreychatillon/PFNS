#include "CHINU_RawQHistos.h"

CHINU_RawQHistos::CHINU_RawQHistos()
{
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t ring=1; ring<=3; ring++){
      fFlag1DLG[ring-1+3*(side-1)]=0;
      fFlag1DHG[ring-1+3*(side-1)]=0;
      fFlagDiscriLG[ring-1+3*(side-1)]=0;
      fFlagDiscriHG[ring-1+3*(side-1)]=0;

    }
  }
}

CHINU_RawQHistos::~CHINU_RawQHistos()
{
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t ring=1; ring<=3; ring++){
      fFlag1DLG[ring-1+3*(side-1)]=0;
      fFlag1DHG[ring-1+3*(side-1)]=0;
      fFlagDiscriLG[ring-1+3*(side-1)]=0;
      fFlagDiscriHG[ring-1+3*(side-1)]=0;

    }
  }
}

UShort_t CHINU_RawQHistos::GetDet(UShort_t side, UShort_t ring, UShort_t rank)
{
//side : 1=LEFT, 2=RIGHT
//ring : 1=I, 2=II, 3=III
//rank : 1..9
//--> in order to calculate det = 1..54
  return(rank + 9*(ring-1) + 27*(side-1));
}

Bool_t CHINU_RawQHistos::TestRank(UShort_t rank, ULong64_t flag)
{
  UShort_t bit = 0;
  bit = bit | (1<<(rank-1));
  return(((flag & bit) >> (rank-1)) & 0x1);
}

void CHINU_RawQHistos::Define1DLG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestRank(rank,fFlag1DLG[ring-1+3*(side-1)])){
    sprintf(name,"Q1_%s_%s_%d_LG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h1_Q1LG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q1LG[det-1]->SetLineColor(kBlue);
    sprintf(name,"Q2_%s_%s_%d_LG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h1_Q2LG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q2LG[det-1]->SetLineColor(kBlue+3);
    fFlag1DLG[ring-1+3*(side-1)] = fFlag1DLG[ring-1+3*(side-1)] | (1<<(rank-1));
  }
}
void CHINU_RawQHistos::Define1DHG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestRank(rank,fFlag1DHG[ring-1+3*(side-1)])){
    sprintf(name,"Q1_%s_%s_%d_HG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h1_Q1HG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q1HG[det-1]->SetLineColor(kRed);
    sprintf(name,"Q2_%s_%s_%d_HG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h1_Q2HG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q2HG[det-1]->SetLineColor(kRed+3);
    fFlag1DHG[ring-1+3*(side-1)] = fFlag1DHG[ring-1+3*(side-1)] | (1<<(rank-1));
  }
}
void CHINU_RawQHistos::DefineDiscriLG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestRank(rank,fFlagDiscriLG[ring-1+3*(side-1)])){
    sprintf(name,"DiscriQ_%s_%s_%d_LG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h2_DiscriLG[det-1] = new TH2F(name,name,1025,0,524800,600,0,3);
    fFlagDiscriLG[ring-1+3*(side-1)] = fFlagDiscriLG[ring-1+3*(side-1)] | (1<<(rank-1));
  }
}
void CHINU_RawQHistos::DefineDiscriHG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestRank(rank,fFlagDiscriHG[ring-1+3*(side-1)])){
    sprintf(name,"DiscriQ_%s_%s_%d_HG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h2_DiscriHG[det-1] = new TH2F(name,name,1025,0,524800,600,0,3);
    fFlagDiscriHG[ring-1+3*(side-1)] = fFlagDiscriHG[ring-1+3*(side-1)] | (1<<(rank-1));
  }
}




void CHINU_RawQHistos::FillHistosLG(vector<UShort_t> * vDet, vector<UInt_t> * vQ1, vector<UInt_t> *vQ2)
{
  UShort_t  det;
  Float_t ratio;
  UShort_t side,ring,rank;
  for(UShort_t m=0; m<vDet->size();m++){
    det = vDet->at(m);
    side = Side_ChiNu[det-1]; 
    ring = Ring_ChiNu[det-1];
    rank = Rank_ChiNu[det-1];
    if(TestRank(rank,fFlag1DLG[ring-1+3*(side-1)])){
      h1_Q1LG[det-1]->Fill(vQ1->at(m));
      h1_Q2LG[det-1]->Fill(vQ2->at(m));
    }
    if(TestRank(rank,fFlagDiscriLG[ring-1+3*(side-1)])){
      if(vQ1->at(m)>0)      ratio = (Float_t)(vQ2->at(m)) / (Float_t)(vQ1->at(m));
      else ratio=-1;
      h2_DiscriLG[det-1]->Fill(vQ1->at(m),ratio);
    }
  }
}

void CHINU_RawQHistos::FillHistosHG(vector<UShort_t> * vDet, vector<UInt_t> * vQ1, vector<UInt_t> *vQ2)
{
  UShort_t  det;
  Float_t ratio;
  UShort_t side,ring,rank;
  for(UShort_t m=0; m<vDet->size();m++){
    det = vDet->at(m);
    side = Side_ChiNu[det-1];
    ring = Ring_ChiNu[det-1];
    rank = Rank_ChiNu[det-1];
    if(TestRank(rank,fFlag1DHG[ring-1+3*(side-1)])){
      h1_Q1HG[det-1]->Fill(vQ1->at(m));
      h1_Q2HG[det-1]->Fill(vQ2->at(m));
    }
    if(TestRank(rank,fFlagDiscriHG[ring-1+3*(side-1)])){
      if(vQ1->at(m)>0)      ratio = (Float_t)(vQ2->at(m)) / (Float_t)(vQ1->at(m));
      else ratio=-1;
      h2_DiscriHG[det-1]->Fill(vQ1->at(m),ratio);
    }
  }
}




TCanvas * CHINU_RawQHistos::DrawHistos1DLG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  sprintf(name,"Q1andQ2_%s_%s_%d_LG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
  TCanvas * c = new TCanvas(name,name,1000,500);
  c->Divide(2,1);
  if(TestRank(rank,fFlag1DLG[ring-1+3*(side-1)])) {
    c->cd(1); h1_Q1LG[det-1]->Draw();
    c->cd(2); h1_Q2LG[det-1]->Draw();
  }
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistos1DHG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  sprintf(name,"Q1andQ2_%s_%s_%d_HG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
  TCanvas * c = new TCanvas(name,name,1000,500);
  c->Divide(2,1);
  if(TestRank(rank,fFlag1DHG[ring-1+3*(side-1)])) {
    c->cd(1); h1_Q1HG[det-1]->Draw();
    c->cd(2); h1_Q2HG[det-1]->Draw();
  }
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistos1DSideRingLG(UShort_t side, UShort_t ring)
{
  char name[100];
  UShort_t det;
  sprintf(name,"Q1andQ2_%s_%s_LG",sideVal[side-1].Data(),ringVal[ring-1].Data());
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(3,3);
  // attention rank is 1-based, do not change to 0-base
  for(UShort_t rank=1; rank<=9; rank++){
    det = GetDet(side,ring,rank);
    if(TestRank(rank,fFlag1DLG[ring-1+3*(side-1)])) {
      c->cd(rank); h1_Q2LG[det-1]->Draw(); h1_Q1LG[det-1]->Draw("same");
    }
  }
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistos1DSideRingHG(UShort_t side, UShort_t ring)
{
  char name[100];
  UShort_t det;
  sprintf(name,"Q1andQ2_%s_%s_HG",sideVal[side-1].Data(),ringVal[ring-1].Data());
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(3,3);
  // attention rank is 1-based, do not change to 0-base
  for(UShort_t rank=1; rank<=9; rank++){
    det = GetDet(side,ring,rank);
    if(TestRank(rank,fFlag1DHG[ring-1+3*(side-1)])) {
      c->cd(rank); h1_Q2HG[det-1]->Draw(); h1_Q1HG[det-1]->Draw("same");
    }
  }
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistos1DRankLG(UShort_t rank)
{
  char name[100];
  UShort_t det;
  sprintf(name,"Q1andQ2_%d_LG",rank);
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(3,2);
  // attention side, ring and rank are 1-based, do not change to 0-base
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t ring=1; ring<=3; ring++){
      det = GetDet(side,ring,rank);
      if(TestRank(rank,fFlag1DLG[ring-1+3*(side-1)])) {
	c->cd(ring+3*(side-1)); h1_Q2LG[det-1]->Draw(); h1_Q1LG[det-1]->Draw("same");
      }
    }
  }
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistos1DRankHG(UShort_t rank)
{
  char name[100];
  UShort_t det;
  sprintf(name,"Q1andQ2_%d_HG",rank);
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(3,2);
  // attention side, ring and rank are 1-based, do not change to 0-base
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t ring=1; ring<=3; ring++){
      det = GetDet(side,ring,rank);
      if(TestRank(rank,fFlag1DHG[ring-1+3*(side-1)])) {
	c->cd(ring+3*(side-1)); h1_Q2HG[det-1]->Draw(); h1_Q1HG[det-1]->Draw("same");
      }
    }
  }
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistoDiscriLG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  sprintf(name,"DiscriQ_%s_%s_%d_LG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
  TCanvas * c = new TCanvas(name,name,600,500);
  c->cd();
  if(TestRank(rank,fFlagDiscriLG[ring-1+3*(side-1)])) h2_DiscriLG[det-1]->Draw("COL");
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistoDiscriHG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  sprintf(name,"DiscriQ_%s_%s_%d_HG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
  TCanvas * c = new TCanvas(name,name,600,500);
  c->cd();
  if(TestRank(rank,fFlagDiscriHG[ring-1+3*(side-1)])) h2_DiscriHG[det-1]->Draw("COL");
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistosDiscriSideRingLG(UShort_t side, UShort_t ring)
{
  char name[100];
  UShort_t det;
  sprintf(name,"DiscriQ_%s_%s_LG",sideVal[side-1].Data(),ringVal[ring-1].Data());
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(3,3);
  // attention rank is 1-based, do not change to 0-base
  for(UShort_t rank=1; rank<=9; rank++){
    det = GetDet(side,ring,rank);
      if(TestRank(rank,fFlagDiscriLG[ring-1+3*(side-1)])){
	c->cd(rank); h2_DiscriLG[det-1]->Draw("COL"); 
      }
  }
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistosDiscriSideRingHG(UShort_t side, UShort_t ring)
{
  char name[100];
  UShort_t det;
  sprintf(name,"DiscriQ_%s_%s_HG",sideVal[side-1].Data(),ringVal[ring-1].Data());
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(3,3);
  // attention rank is 1-based, do not change to 0-base
  for(UShort_t rank=1; rank<=9; rank++){
    det = GetDet(side,ring,rank);
      if(TestRank(rank,fFlagDiscriHG[ring-1+3*(side-1)])){
	c->cd(rank); h2_DiscriHG[det-1]->Draw("COL");
      }
  }
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistosDiscriRankLG(UShort_t rank)
{
  char name[100];
  UShort_t det;
  sprintf(name,"DiscriQ_Rank%d_LG",rank);
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(3,2);
  // attention side, ring and rank are 1-based, do not change to 0-base
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t ring=1; ring<=3; ring++){
      det = GetDet(side,ring,rank);
      if(TestRank(rank,fFlagDiscriLG[ring-1+3*(side-1)])){
	c->cd(ring+3*(side-1)); h2_DiscriLG[det-1]->Draw("COL");
      }
    }
  }
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistosDiscriRankHG(UShort_t rank)
{
  char name[100];
  UShort_t det;
  sprintf(name,"DiscriQ_Rank%d_HG",rank);
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(3,2);
  // attention side, ring and rank are 1-based, do not change to 0-base
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t ring=1; ring<=3; ring++){
      det = GetDet(side,ring,rank);
      if(TestRank(rank,fFlagDiscriHG[ring-1+3*(side-1)])){
	c->cd(ring+3*(side-1)); h2_DiscriHG[det-1]->Draw("COL");
      }
    }
  }
  return(c);
}
