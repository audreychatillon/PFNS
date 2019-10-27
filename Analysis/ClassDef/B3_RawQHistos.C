#include "B3_RawQHistos.h"

B3_RawQHistos::B3_RawQHistos()
  : fFlag1DLG(0)
  , fFlag1DHG(0)
  , fFlagDiscriLG(0)
  , fFlagDiscriHG(0)
{
  
}

B3_RawQHistos::~B3_RawQHistos()
{
  fFlag1DLG=0;
  fFlag1DHG=0;
  fFlagDiscriLG=0;
  fFlagDiscriHG=0;
}

UShort_t B3_RawQHistos::GetDet(UShort_t side, UShort_t pos)
{
//side : 1=LEFT, 2=RIGHT
//pos : 1=BACK, 2=FRONT
//--> in order to calculate det = 1..4
  return(pos + 2*(side-1));
}

Bool_t B3_RawQHistos::TestDet(UShort_t det, UChar_t flag)
{
  UChar_t bit = 0;
  bit = bit | (1<<(det-1));
  return(((flag & bit) >> (det-1)) & 0x1);
}

void B3_RawQHistos::Define1DLG(UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det = GetDet(side,pos);
  if(!TestDet(det,fFlag1DLG)){
    sprintf(name,"Q1_B3_%s_%s_LG",sideVal[side-1].Data(),posVal[pos-1].Data());
    h1_Q1LG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q1LG[det-1]->SetLineColor(kBlue);
    sprintf(name,"Q2_B3_%s_%s_LG",sideVal[side-1].Data(),posVal[pos-1].Data());
    h1_Q2LG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q2LG[det-1]->SetLineColor(kBlue+3);
    fFlag1DLG = fFlag1DLG | (1<<(det-1));
  }
}
void B3_RawQHistos::Define1DHG(UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det = GetDet(side,pos);
  if(!TestDet(det,fFlag1DHG)){
    sprintf(name,"Q1_B3_%s_%s_HG",sideVal[side-1].Data(),posVal[pos-1].Data());
    h1_Q1HG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q1HG[det-1]->SetLineColor(kRed);
    sprintf(name,"Q2_B3_%s_%s_HG",sideVal[side-1].Data(),posVal[pos-1].Data());
    h1_Q2HG[det-1] = new TH1I(name,name,4100,0,524800);
    h1_Q2HG[det-1]->SetLineColor(kRed+3);
    fFlag1DHG = fFlag1DHG | (1<<(det-1));
  }
}
void B3_RawQHistos::DefineDiscriLG(UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det = GetDet(side,pos);
  if(!TestDet(det,fFlagDiscriLG)){
    sprintf(name,"DiscriQ_B3_%s_%s_LG",sideVal[side-1].Data(),posVal[pos-1].Data());
    h2_DiscriLG[det-1] = new TH2F(name,name,900,0,300000,350,0.5,1.2);
    fFlagDiscriLG = fFlagDiscriLG | (1<<(det-1));
  }
}
void B3_RawQHistos::DefineDiscriHG(UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det = GetDet(side,pos);
  if(!TestDet(det,fFlagDiscriHG)){
    sprintf(name,"DiscriQ_B3_%s_%s_HG",sideVal[side-1].Data(),posVal[pos-1].Data());
    h2_DiscriHG[det-1] = new TH2F(name,name,500,0,600000,350,0.5,1.2);
    fFlagDiscriHG = fFlagDiscriHG | (1<<(det-1));
  }
}





void B3_RawQHistos::FillHistosLG(vector<UShort_t> * vDet, vector<UInt_t> * vQ1, vector<UInt_t> *vQ2)
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
      if(vQ1->at(m)>0)      ratio = (Float_t)(vQ2->at(m)) / (Float_t)(vQ1->at(m));
      else ratio=-1;
      h2_DiscriLG[det-1]->Fill(vQ1->at(m),ratio);
    }
  }
}

void B3_RawQHistos::FillHistosLG(UShort_t det, UInt_t Q1, UInt_t Q2)
{
  Float_t ratio;
  if(TestDet(det,fFlag1DLG)){
    h1_Q1LG[det-1]->Fill(Q1);
    h1_Q2LG[det-1]->Fill(Q2);
  }
  if(TestDet(det,fFlagDiscriLG)){
    if(Q1>0)      ratio = (Float_t)(Q2) / (Float_t)(Q1);
    else ratio=-1;
    h2_DiscriLG[det-1]->Fill(Q1,ratio);
  }
}

void B3_RawQHistos::FillHistosHG(vector<UShort_t> * vDet, vector<UInt_t> * vQ1, vector<UInt_t> *vQ2)
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
      if(vQ1->at(m)>0)      ratio = (Float_t)(vQ2->at(m)) / (Float_t)(vQ1->at(m));
      else ratio=-1;
      h2_DiscriHG[det-1]->Fill(vQ1->at(m),ratio);
    }
  }
}


void B3_RawQHistos::FillHistosHG(UShort_t det, UInt_t Q1, UInt_t Q2)
{
  Float_t ratio;
  if(TestDet(det,fFlag1DHG)){
    h1_Q1HG[det-1]->Fill(Q1);
    h1_Q2HG[det-1]->Fill(Q2);
  }
  if(TestDet(det,fFlagDiscriHG)){
    if(Q1>0)      ratio = (Float_t)(Q2) / (Float_t)(Q1);
    else ratio=-1;
    h2_DiscriHG[det-1]->Fill(Q1,ratio);
  }
}

TCanvas * B3_RawQHistos::DrawHistos1DLG(UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det = GetDet(side,pos);
  sprintf(name,"Q1andQ2_B3_%s_%s_LG",sideVal[side-1].Data(),posVal[pos-1].Data());
  TCanvas * c = new TCanvas(name,name,1000,500);
  c->Divide(2,1);
  if(TestDet(det,fFlag1DLG)){
    c->cd(1); h1_Q1LG[det-1]->Draw(); h1_Q1LG[det-1]->SetDirectory(0);
    c->cd(2); h1_Q2LG[det-1]->Draw(); h1_Q2LG[det-1]->SetDirectory(0);
  }
  return(c);
}
TCanvas * B3_RawQHistos::DrawHistos1DHG(UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det = GetDet(side,pos);
  sprintf(name,"Q1andQ2_B3_%s_%s_HG",sideVal[side-1].Data(),posVal[pos-1].Data());
  TCanvas * c = new TCanvas(name,name,1000,500);
  c->Divide(2,1);
  if(TestDet(det,fFlag1DHG)){
    c->cd(1); h1_Q1HG[det-1]->Draw();h1_Q1HG[det-1]->SetDirectory(0);
    c->cd(2); h1_Q2HG[det-1]->Draw();h1_Q2HG[det-1]->SetDirectory(0);
  }
  return(c);
}
TCanvas * B3_RawQHistos::DrawHistos1DSideLG(UShort_t side)
{
  char name[100];
  UShort_t det;
  sprintf(name,"Q1andQ2_B3_%s_LG",sideVal[side-1].Data());
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(2,1);
  // attention pos is 1-based, do not change to 0-base
  for(UShort_t pos=1; pos<=2; pos++){
    det = GetDet(side,pos);
    if(TestDet(det,fFlag1DLG)){
      c->cd(pos); 
      h1_Q2LG[det-1]->Draw(); h1_Q2LG[det-1]->SetDirectory(0);
      h1_Q1LG[det-1]->Draw("same");h1_Q1LG[det-1]->SetDirectory(0);
    }
  }
  return(c);
}
TCanvas * B3_RawQHistos::DrawHistos1DSideHG(UShort_t side)
{
  char name[100];
  UShort_t det;
  sprintf(name,"Q1andQ2_B3_%s_HG",sideVal[side-1].Data());
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(2,1);
  // attention pos is 1-based, do not change to 0-base
  for(UShort_t pos=1; pos<=2; pos++){
    det = GetDet(side,pos);
    if(TestDet(det,fFlag1DHG)){
      c->cd(pos); 
      h1_Q2HG[det-1]->Draw(); h1_Q2HG[det-1]->SetDirectory(0);
      h1_Q1HG[det-1]->Draw("same");h1_Q1HG[det-1]->SetDirectory(0);
    }
  }
  return(c);
}
TCanvas * B3_RawQHistos::DrawHistos1DAllLG()
{
  char name[100];
  UShort_t det;
  sprintf(name,"Q1andQ2_B3_LG");
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(2,2);
  // attention side, and pos are 1-based, do not change to 0-base
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t pos=1; pos<=2; pos++){
      det = GetDet(side,pos);
      if(TestDet(det,fFlag1DLG)){
	c->cd(pos+2*(side-1)); 
	h1_Q2LG[det-1]->Draw(); h1_Q2LG[det-1]->SetDirectory(0);
	h1_Q1LG[det-1]->Draw("same");h1_Q1LG[det-1]->SetDirectory(0);
      }
    }
  }
  return(c);
}
TCanvas * B3_RawQHistos::DrawHistos1DAllHG()
{
  char name[100];
  UShort_t det;
  sprintf(name,"Q1andQ2_B3_HG");
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(2,2);
  // attention side, ring and rank are 1-based, do not change to 0-base
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t pos=1; pos<=2; pos++){
      det = GetDet(side,pos);
      if(TestDet(det,fFlag1DHG)){
	c->cd(pos+2*(side-1)); 
	h1_Q2HG[det-1]->Draw(); h1_Q2HG[det-1]->SetDirectory(0);
	h1_Q1HG[det-1]->Draw("same");h1_Q1HG[det-1]->SetDirectory(0);
      }
    }
  }
  return(c);
}
TCanvas * B3_RawQHistos::DrawHistoDiscriLG(UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det = GetDet(side,pos);
  sprintf(name,"DiscriQ_B3_%s_%s_LG",sideVal[side-1].Data(),posVal[pos-1].Data());
  TCanvas * c = new TCanvas(name,name,600,500);
  c->cd();
  if (TestDet(det,fFlagDiscriLG))  {
    h2_DiscriLG[det-1]->Draw("COL");h2_DiscriLG[det-1]->SetDirectory(0);
  }
  return(c);
}
TCanvas * B3_RawQHistos::DrawHistoDiscriHG(UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det = GetDet(side,pos);
  sprintf(name,"DiscriQ_%s_%s_HG",sideVal[side-1].Data(),posVal[pos-1].Data());
  TCanvas * c = new TCanvas(name,name,600,500);
  c->cd();
  if(TestDet(det,fFlagDiscriHG)) {
    h2_DiscriHG[det-1]->Draw("COL");
    h2_DiscriHG[det-1]->SetDirectory(0);
  }
  return(c);
}
TCanvas * B3_RawQHistos::DrawHistosDiscriSideLG(UShort_t side)
{
  char name[100];
  UShort_t det;
  sprintf(name,"DiscriQ_%s_LG",sideVal[side-1].Data());
  TCanvas * c = new TCanvas(name,name,1200,600);
  c->Divide(2,1);
  // attention pos is 1-based, do not change to 0-base
  for(UShort_t pos=1; pos<=2; pos++){
    det = GetDet(side,pos);				
    if(TestDet(det,fFlagDiscriLG)){
      c->cd(pos); h2_DiscriLG[det-1]->Draw("COL"); h2_DiscriLG[det-1]->SetDirectory(0);
    }
  }
  return(c);
}
TCanvas * B3_RawQHistos::DrawHistosDiscriSideHG(UShort_t side)
{
  char name[100];
  UShort_t det;
  sprintf(name,"DiscriQ_%s_HG",sideVal[side-1].Data());
  TCanvas * c = new TCanvas(name,name,1200,600);
  c->Divide(2,1);
  // attention pos is 1-based, do not change to 0-base
  for(UShort_t pos=1; pos<=2; pos++){
    det = GetDet(side,pos);
    if(TestDet(det,fFlagDiscriHG)){
      c->cd(pos); h2_DiscriHG[det-1]->Draw("COL");h2_DiscriHG[det-1]->SetDirectory(0);
    }
  }
  return(c);
}
TCanvas * B3_RawQHistos::DrawHistosDiscriAllLG()
{
  char name[100];
  UShort_t det;
  sprintf(name,"DiscriQ_B3_LG");
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(2,2);
  // attention side, ring and rank are 1-based, do not change to 0-base
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t pos=1; pos<=2; pos++){
      det = GetDet(side,pos);
      if(TestDet(det,fFlagDiscriLG)){
	c->cd(pos+2*(side-1)); h2_DiscriLG[det-1]->Draw("COL");h2_DiscriLG[det-1]->SetDirectory(0);
      }
    }
  }
  return(c);
}
TCanvas * B3_RawQHistos::DrawHistosDiscriAllHG()
{
  char name[100];
  UShort_t det;
  sprintf(name,"DiscriQ_B3_HG");
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(2,2);
  // attention side, ring and rank are 1-based, do not change to 0-base
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t pos=1; pos<=2; pos++){
      det = GetDet(side,pos);
      if(TestDet(det,fFlagDiscriHG)){
	c->cd(pos+2*(side-1)); h2_DiscriHG[det-1]->Draw("COL");h2_DiscriHG[det-1]->SetDirectory(0);
      }
    }
  }
  return(c);
}
