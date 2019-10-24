#include "CHINU_RawBkgToFHistos.h"


CHINU_RawBkgToFHistos::CHINU_RawBkgToFHistos()
  : fFlagLG(0)
  , fFlagHG(0)
{
}

CHINU_RawBkgToFHistos::~CHINU_RawBkgToFHistos()
{
  fFlagLG=0;
  fFlagHG=0;
}


UShort_t CHINU_RawBkgToFHistos::GetDet(UShort_t side, UShort_t ring, UShort_t rank)
{
//side : 1=LEFT, 2=RIGHT
//ring : 1=I, 2=II, 3=III
//rank : 1..9
//--> in order to calculate det = 1..54
  return(rank + 9*(ring-1) + 27*(side-1));
}


Bool_t CHINU_RawBkgToFHistos::TestDet(UShort_t det, ULong64_t flag)
{
  ULong64_t bit = 0;
  bit = bit | (1<<(det-1));
  return(((flag & bit) >> (det-1)) & 0x1);
}


void CHINU_RawBkgToFHistos::DefineOneDetLG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestDet(det,fFlagLG)){
    sprintf(name,"RawBkgToF_%s_%s_%d_LG_Pulser",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h1_RawToF_LG[det-1] = new TH1F(name,name,5000,0,500);
    h1_RawToF_LG[det-1]->GetXaxis()->SetTitle("raw ToF [ns, 100ps/bin]");
    h1_RawToF_LG[det-1]->GetXaxis()->SetTitleOffset(1.4);
    h1_RawToF_LG[det-1]->SetLineColor(kBlue);
    fFlagLG = fFlagLG | (1<<(det-1));
  }
}
void CHINU_RawBkgToFHistos::DefineOneDetHG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestDet(det,fFlagHG)){
    sprintf(name,"RawBkgToF_%s_%s_%d_HG_Pulser",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
    h1_RawToF_HG[det-1] = new TH1F(name,name,5000,0,500);
    h1_RawToF_HG[det-1]->GetXaxis()->SetTitle("raw ToF [ns, 100ps/bin]");
    h1_RawToF_HG[det-1]->GetXaxis()->SetTitleOffset(1.4);
    h1_RawToF_HG[det-1]->SetLineColor(kBlue);
    fFlagHG = fFlagHG | (1<<(det-1));
  }
}



void CHINU_RawBkgToFHistos::FillHistosLG_PULSERmult1(Double_t PULSER_time, vector<UShort_t>* vN_det, vector<Double_t>*vN_time, UShort_t * Nmult)
{
  for(UShort_t m=0; m<vN_det->size(); m++){
    if (TestDet(vN_det->at(m),fFlagLG)){
      if(Nmult[vN_det->at(m)-1]>1) continue;
      h1_RawToF_LG[(vN_det->at(m)-1)]->Fill(vN_time->at(m) - PULSER_time);
    }
  }// end of loop over the neutron multiplicity
}
void CHINU_RawBkgToFHistos::FillHistosHG_PULSERmult1(Double_t PULSER_time, vector<UShort_t>* vN_det, vector<Double_t>*vN_time, UShort_t * Nmult)
{
  for(UShort_t m=0; m<vN_det->size(); m++){
    if (TestDet(vN_det->at(m), fFlagHG)){
      if(Nmult[vN_det->at(m)-1]>1) continue;
      h1_RawToF_HG[(vN_det->at(m)-1)]->Fill(vN_time->at(m) - PULSER_time);
    }
  }// end of loop over the neutron multiplicity
}





TCanvas * CHINU_RawBkgToFHistos::DrawOneDetLG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  sprintf(name,"CHINU_BkgToFraw_%s_%s_%d_LG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
  TCanvas * c = new TCanvas(name,name,700,600);
  UShort_t det=GetDet(side,ring,rank);
  if (TestDet(det,fFlagLG)){
    c->cd(); h1_RawToF_LG[(det-1)]->Draw();
  }
  return(c);
}
TCanvas * CHINU_RawBkgToFHistos::DrawOneDetHG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  sprintf(name,"CHINU_BkgToFraw_%s_%s_%d_HG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
  TCanvas * c = new TCanvas(name,name,700,600);
  UShort_t det=GetDet(side,ring,rank);
  if (TestDet(det,fFlagHG)){
    c->cd(); h1_RawToF_HG[(det-1)]->Draw();
  }
  return(c);
}
TCanvas * CHINU_RawBkgToFHistos::DrawSideRingLG(UShort_t side, UShort_t ring)
{
  char name[100];
  UShort_t det;
  sprintf(name,"CHINU_BkgToFraw_%s_%s_LG",sideVal[side-1].Data(),ringVal[ring-1].Data());
  TCanvas * c = new TCanvas(name,name,700,600);
  c->Divide(3,3);
  for(UShort_t rank=1; rank<=9; rank++){
    det=GetDet(side,ring,rank);
    if (TestDet(det,fFlagLG)){
      c->cd(rank); 
      h1_RawToF_LG[(det-1)]->Draw();
    }
  }
  return(c);
}
TCanvas * CHINU_RawBkgToFHistos::DrawSideRingHG(UShort_t side, UShort_t ring)
{
  char name[100];
  UShort_t det;
  sprintf(name,"CHINU_BkgToFraw_%s_%s_HG",sideVal[side-1].Data(),ringVal[ring-1].Data());
  TCanvas * c = new TCanvas(name,name,700,600);
  c->Divide(3,3);
  for(UShort_t rank=1; rank<=9; rank++){
    det=GetDet(side,ring,rank);
    if (TestDet(det,fFlagHG)){
      c->cd(rank); 
      h1_RawToF_HG[(det-1)]->Draw();
    }
  }
  return(c);
}
TCanvas * CHINU_RawBkgToFHistos::DrawRankLG(UShort_t rank)
{
  char name[100];
  UShort_t det;
  sprintf(name,"CHINU_BkgToFraw_Rank%d_LG",rank);
  TCanvas * c = new TCanvas(name,name,700,600);
  c->Divide(3,2);
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t ring=1; ring<=3; ring++){
      det=GetDet(side,ring,rank);
      if (TestDet(det,fFlagLG)){
	c->cd(ring+3*(side-1)); 
	h1_RawToF_LG[(det-1)]->Draw();
      }
    }
  }
  return(c);
}
TCanvas * CHINU_RawBkgToFHistos::DrawRankHG(UShort_t rank)
{
  char name[100];
  UShort_t det;
  sprintf(name,"CHINU_BkgToFraw_Rank%d_HG",rank);
  TCanvas * c = new TCanvas(name,name,700,600);
  c->Divide(3,2);
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t ring=1; ring<=3; ring++){
      det=GetDet(side,ring,rank);
      if (TestDet(det,fFlagHG)){
	c->cd(ring+3*(side-1)); 
	h1_RawToF_HG[(det-1)]->Draw();
      }
    }
  }
  return(c);
}
