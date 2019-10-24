#include "CHINU_RawQHistos.h"

CHINU_RawQHistos::CHINU_RawQHistos()
  : fFlag1DLG(0)
  , fFlag1DHG(0)
  , fFlagDiscriLG(0)
  , fFlagDiscriHG(0)
{
  
}

CHINU_RawQHistos::~CHINU_RawQHistos()
{
  fFlag1DLG=0;
  fFlag1DHG=0;
  fFlagDiscriLG=0;
  fFlagDiscriHG=0;
}

UShort_t CHINU_RawQHistos::GetDet(UShort_t side, UShort_t ring, UShort_t rank)
{
//side : 1=LEFT, 2=RIGHT
//ring : 1=I, 2=II, 3=III
//rank : 1..9
//--> in order to calculate det = 1..54
  return(rank + 9*(ring-1) + 27*(side-1));
}

Bool_t CHINU_RawQHistos::TestDet(UShort_t det, ULong64_t flag)
{
  ULong64_t bit = 0;
  bit = bit | (1<<(det-1));
  return(((flag & bit) >> (det-1)) & 0x1);
}

void CHINU_RawQHistos::Define1DLG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestDet(det,fFlag1DLG)){
    sprintf(name,"Q1_%s_%s_%d_LG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h1_Q1LG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q1LG[det-1]->SetLineColor(kBlue);
    sprintf(name,"Q2_%s_%s_%d_LG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h1_Q2LG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q2LG[det-1]->SetLineColor(kBlue+3);
    fFlag1DLG = fFlag1DLG | (1<<(det-1));
  }
}
void CHINU_RawQHistos::Define1DHG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestDet(det,fFlag1DHG)){
    sprintf(name,"Q1_%s_%s_%d_HG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h1_Q1HG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q1HG[det-1]->SetLineColor(kRed);
    sprintf(name,"Q2_%s_%s_%d_HG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h1_Q2HG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q2HG[det-1]->SetLineColor(kRed+3);
    fFlag1DHG = fFlag1DHG | (1<<(det-1));
  }
}
void CHINU_RawQHistos::DefineDiscriLG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestDet(det,fFlagDiscriLG)){
    sprintf(name,"DiscriQ_%s_%s_%d_LG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h2_DiscriLG[det-1] = new TH2F(name,name,1025,0,524800,600,0,3);
    fFlagDiscriLG = fFlagDiscriLG | (1<<(det-1));
  }
}
void CHINU_RawQHistos::DefineDiscriHG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestDet(det,fFlagDiscriHG)){
    sprintf(name,"DiscriQ_%s_%s_%d_HG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h2_DiscriHG[det-1] = new TH2F(name,name,1025,0,524800,1000,0,5);
    fFlagDiscriHG = fFlagDiscriHG | (1<<(det-1));
  }
}


// attention rank, ring and side are 1-based, do not change to 0-base
void CHINU_RawQHistos::Define1DSideRingLG(UShort_t side, UShort_t ring)
{
  for(UShort_t rank=1; rank<=9; rank++)      Define1DLG(side,ring,rank);
}
void CHINU_RawQHistos::Define1DSideRingHG(UShort_t side, UShort_t ring)
{
  for(UShort_t rank=1; rank<=9; rank++)      Define1DHG(side,ring,rank);
}
void CHINU_RawQHistos::Define1DRankLG(UShort_t rank)
{
  for(UShort_t side=1; side<=2; side++)
    for(UShort_t ring=1; ring<=3; ring++)
      Define1DLG(side,ring,rank);
}
void CHINU_RawQHistos::Define1DRankHG(UShort_t rank)
{
  for(UShort_t side=1; side<=2; side++)
    for(UShort_t ring=1; ring<=3; ring++)
      Define1DHG(side,ring,rank);
}
void CHINU_RawQHistos::DefineDiscriSideRingLG(UShort_t side, UShort_t ring)
{
  for(UShort_t rank=1; rank<=9; rank++)      DefineDiscriLG(side,ring,rank);
}
void CHINU_RawQHistos::DefineDiscriSideRingHG(UShort_t side, UShort_t ring)
{
  for(UShort_t rank=1; rank<=9; rank++)      DefineDiscriHG(side,ring,rank);
}
void CHINU_RawQHistos::DefineDiscriRankLG(UShort_t rank)
{
  for(UShort_t side=1; side<=2; side++)
    for(UShort_t ring=1; ring<=3; ring++)
      DefineDiscriLG(side,ring,rank);
}
void CHINU_RawQHistos::DefineDiscriRankHG(UShort_t rank)
{
  for(UShort_t side=1; side<=2; side++)
    for(UShort_t ring=1; ring<=3; ring++)
      DefineDiscriHG(side,ring,rank);
}




void CHINU_RawQHistos::FillHistosLG(vector<UShort_t> * vDet, vector<UInt_t> * vQ1, vector<UInt_t> *vQ2)
{
  UShort_t  det;
  Float_t ratio;
  for(UShort_t m=0; m<vDet->size();m++){
    det = vDet->at(m);
    if(TestDet(det,fFlag1DLG)){
      h1_Q1LG[det-1]->Fill(vQ1->at(m));
      h1_Q2LG[det-1]->Fill(vQ2->at(m));
    }
    if(TestDet(det,fFlagDiscriLG)){
      if(vQ1->at(m)>0)      ratio = (Float_t)(vQ1->at(m)) / (Float_t)(vQ2->at(m));
      else ratio=-1;
      h2_DiscriLG[det-1]->Fill(vQ1->at(m),ratio);
    }
  }
}

void CHINU_RawQHistos::FillHistosHG(vector<UShort_t> * vDet, vector<UInt_t> * vQ1, vector<UInt_t> *vQ2)
{
  UShort_t  det;
  Float_t ratio;
  for(UShort_t m=0; m<vDet->size();m++){
    det = vDet->at(m);
    if(TestDet(det,fFlag1DHG)){
      h1_Q1HG[det-1]->Fill(vQ1->at(m));
      h1_Q2HG[det-1]->Fill(vQ2->at(m));
    }
    if(TestDet(det,fFlagDiscriHG)){
      if(vQ1->at(m)>0)      ratio = (Float_t)(vQ1->at(m)) / (Float_t)(vQ2->at(m));
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
  if(TestDet(det,fFlag1DLG)) {
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
  if(TestDet(det,fFlag1DHG)) {
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
    if(TestDet(det,fFlag1DLG)) {
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
    if(TestDet(det,fFlag1DHG)) {
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
      if(TestDet(det,fFlag1DLG)) {
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
      if(TestDet(det,fFlag1DHG)) {
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
  if(TestDet(det,fFlagDiscriLG)) h2_DiscriLG[det-1]->Draw("COL");
  return(c);
}
TCanvas * CHINU_RawQHistos::DrawHistoDiscriHG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  sprintf(name,"DiscriQ_%s_%s_%d_HG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
  TCanvas * c = new TCanvas(name,name,600,500);
  c->cd();
  if(TestDet(det,fFlagDiscriHG)) h2_DiscriHG[det-1]->Draw("COL");
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
      if(TestDet(det,fFlagDiscriLG)){
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
      if(TestDet(det,fFlagDiscriHG)){
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
      if(TestDet(det,fFlagDiscriLG)){
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
      if(TestDet(det,fFlagDiscriHG)){
	c->cd(ring+3*(side-1)); h2_DiscriHG[det-1]->Draw("COL");
      }
    }
  }
  return(c);
}
