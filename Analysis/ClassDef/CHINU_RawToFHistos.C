#include "CHINU_RawToFHistos.h"


CHINU_RawToFHistos::CHINU_RawToFHistos()
{
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++) {
    for(UShort_t side=1; side<=2; side++){
      for(UShort_t ring=1; ring<=3; ring++){
	fFlagLG[ring-1+3*(side-1)+6*(anode-1)]=0;
	fFlagHG[ring-1+3*(side-1)+6*(anode-1)]=0;
      }
    }
  }
}

CHINU_RawToFHistos::~CHINU_RawToFHistos()
{
  for(UShort_t anode=0; anode<FC_nAnodes; anode++) {
    for(UShort_t side=1; side<=2; side++){
      for(UShort_t ring=1; ring<=3; ring++){
	fFlagLG[ring-1+3*(side-1)+6*(anode-1)]=0;
	fFlagHG[ring-1+3*(side-1)+6*(anode-1)]=0;
      }
    }
  }
}


UShort_t CHINU_RawToFHistos::GetDet(UShort_t side, UShort_t ring, UShort_t rank)
{
//side : 1=LEFT, 2=RIGHT
//ring : 1=I, 2=II, 3=III
//rank : 1..9
//--> in order to calculate det = 1..54
  return(rank + 9*(ring-1) + 27*(side-1));
}


Bool_t CHINU_RawToFHistos::TestRank(UShort_t rank, UShort_t flag)
{
  UShort_t bit = 0;
  bit = bit | (1<<(rank-1));
  return(((flag & bit) >> (rank-1)) & 0x1);
}


void CHINU_RawToFHistos::DefineOneAnodeOneDetLG(UShort_t anode, UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestRank(rank,fFlagLG[ring-1+3*(side-1)+6*(anode-1)])){
    sprintf(name,"RawToF_%s_%s_%d_LG_FC%d",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank,anode);
    h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)] = new TH1F(name,name,5000,0,500);
    h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->GetXaxis()->SetTitle("raw ToF [ns, 100ps/bin]");
    h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->GetXaxis()->SetTitleOffset(1.4);
    h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->SetLineColor(kBlue);
    h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->SetDirectory(0);
    fFlagLG[ring-1+3*(side-1)+6*(anode-1)] = fFlagLG[ring-1+3*(side-1)+6*(anode-1)] | (1<<(rank-1));
  }
}
void CHINU_RawToFHistos::DefineOneAnodeOneDetHG(UShort_t anode, UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  UShort_t det = GetDet(side,ring,rank);
  if(!TestRank(rank,fFlagHG[ring-1+3*(side-1)+6*(anode-1)])){
    sprintf(name,"RawToF_%s_%s_%d_HG_FC%d",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank,anode);
    h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)] = new TH1F(name,name,5000,0,500);
    h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->GetXaxis()->SetTitle("raw ToF [ns, 100ps/bin]");
    h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->GetXaxis()->SetTitleOffset(1.4);
    h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->SetLineColor(kRed);
    h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->SetDirectory(0);
    fFlagHG[ring-1+3*(side-1)+6*(anode-1)] = fFlagHG[ring-1+3*(side-1)+6*(anode-1)] | (1<<(rank-1));
  }
}

void CHINU_RawToFHistos::FillHistosLG_FCmult1(UShort_t FC_anode, Double_t FC_time, vector<UShort_t>* vN_det, vector<Double_t>*vN_time, UShort_t * Nmult)
{
  UShort_t det,side,ring,rank;
  for(UShort_t m=0; m<vN_det->size(); m++){
    det  = vN_det->at(m);
    side = Side_ChiNu[det-1]; 
    ring = Ring_ChiNu[det-1];
    rank = Rank_ChiNu[det-1];
    if (TestRank(rank,fFlagLG[ring-1+3*(side-1)+6*(FC_anode-1)])){
      if(Nmult[det-1]>1) continue;
      h1_RawToF_LG[(det-1)+CHINU_nDets*(FC_anode-1)]->Fill(vN_time->at(m) - FC_time);
    }
  }// end of loop over the neutron multiplicity
}
void CHINU_RawToFHistos::FillHistosHG_FCmult1(UShort_t FC_anode, Double_t FC_time, vector<UShort_t>* vN_det, vector<Double_t>*vN_time, UShort_t * Nmult)
{
  UShort_t det,side,ring,rank;
  for(UShort_t m=0; m<vN_det->size(); m++){
    det  = vN_det->at(m);
    side = Side_ChiNu[det-1]; 
    ring = Ring_ChiNu[det-1];
    rank = Rank_ChiNu[det-1];
    if (TestRank(rank, fFlagHG[ring-1+3*(side-1)+6*(FC_anode-1)])){
      if(Nmult[det-1]>1) continue;
      h1_RawToF_HG[(det-1)+CHINU_nDets*(FC_anode-1)]->Fill(vN_time->at(m) - FC_time);
    }
  }// end of loop over the neutron multiplicity
}




TCanvas * CHINU_RawToFHistos::DrawOneAnodeOneDetLG(UShort_t anode, UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  sprintf(name,"CHINU_ToFraw_FC%d_%s_%s_%d_LG",anode,sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
  TCanvas * c = new TCanvas(name,name,700,600);
  UShort_t det=GetDet(side,ring,rank);
  if (TestRank(rank,fFlagLG[ring-1+3*(side-1)+6*(anode-1)])){
    c->cd(); 
    h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->Draw();
    h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->SetDirectory(0);
  }
  return(c);
}
TCanvas * CHINU_RawToFHistos::DrawOneAnodeOneDetHG(UShort_t anode, UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  sprintf(name,"CHINU_ToFraw_FC%d_%s_%s_%d_HG",anode,sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
  TCanvas * c = new TCanvas(name,name,700,600);
  UShort_t det=GetDet(side,ring,rank);
  if (TestRank(rank,fFlagHG[ring-1+3*(side-1)+6*(anode-1)])){
    c->cd(); 
    h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->Draw();
    h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->SetDirectory(0);
  }
  return(c);
}
TCanvas * CHINU_RawToFHistos::DrawOneAnodeSideRingLG(UShort_t anode, UShort_t side, UShort_t ring)
{
  char name[100];
  UShort_t det;
  sprintf(name,"CHINU_ToFraw_FC%d_%s_%s_LG",anode,sideVal[side-1].Data(),ringVal[ring-1].Data());
  TCanvas * c = new TCanvas(name,name,700,600);
  c->Divide(3,3);
  for(UShort_t rank=1; rank<=9; rank++){
    det=GetDet(side,ring,rank);
    if (TestRank(rank,fFlagLG[ring-1+3*(side-1)+6*(anode-1)])){
      c->cd(rank); 
      h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->Draw();
      h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->SetDirectory(0);
    }
  }
  return(c);
}
TCanvas * CHINU_RawToFHistos::DrawOneAnodeSideRingHG(UShort_t anode, UShort_t side, UShort_t ring)
{
  char name[100];
  UShort_t det;
  sprintf(name,"CHINU_ToFraw_FC%d_%s_%s_HG",anode,sideVal[side-1].Data(),ringVal[ring-1].Data());
  TCanvas * c = new TCanvas(name,name,700,600);
  c->Divide(3,3);
  for(UShort_t rank=1; rank<=9; rank++){
    det=GetDet(side,ring,rank);
    if (TestRank(rank,fFlagHG[ring-1+3*(side-1)+6*(anode-1)])){
      c->cd(rank); 
      h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->Draw();
      h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->SetDirectory(0);
    }
  }
  return(c);
}
TCanvas * CHINU_RawToFHistos::DrawOneAnodeRankLG(UShort_t anode, UShort_t rank)
{
  char name[100];
  UShort_t det;
  sprintf(name,"CHINU_ToFraw_FC%d_Rank%d_LG",anode,rank);
  TCanvas * c = new TCanvas(name,name,700,600);
  c->Divide(3,2);
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t ring=1; ring<=3; ring++){
      det=GetDet(side,ring,rank);
      if (TestRank(rank,fFlagLG[ring-1+3*(side-1)+6*(anode-1)])){
	c->cd(ring+3*(side-1)); 
	h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->Draw();
	h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->SetDirectory(0);
      }
    }
  }
  return(c);
}
TCanvas * CHINU_RawToFHistos::DrawOneAnodeRankHG(UShort_t anode, UShort_t rank)
{
  char name[100];
  UShort_t det;
  sprintf(name,"CHINU_ToFraw_FC%d_Rank%d_HG",anode,rank);
  TCanvas * c = new TCanvas(name,name,700,600);
  c->Divide(3,2);
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t ring=1; ring<=3; ring++){
      det=GetDet(side,ring,rank);
      if (TestRank(rank,fFlagHG[ring-1+3*(side-1)+6*(anode-1)])){
	c->cd(ring+3*(side-1)); 
	h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->Draw();
	h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->SetDirectory(0);
      }
    }
  }
  return(c);
}
TCanvas * CHINU_RawToFHistos::DrawAllAnodesOneDetLG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  sprintf(name,"CHINU_ToFraw_AllAnodes_%s_%s_%d_LG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(4,3);
  UShort_t det=GetDet(side,ring,rank);
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    if (TestRank(rank,fFlagLG[ring-1+3*(side-1)+6*(anode-1)])){
      c->cd(anode); 
      h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->Draw();
      h1_RawToF_LG[(det-1)+CHINU_nDets*(anode-1)]->SetDirectory(0);
    }
  }
  return(c);
}
TCanvas * CHINU_RawToFHistos::DrawAllAnodesOneDetHG(UShort_t side, UShort_t ring, UShort_t rank)
{
  char name[100];
  sprintf(name,"CHINU_ToFraw_AllAnodes_%s_%s_%d_HG",sideVal[side-1].Data(),ringVal[ring-1].Data(),rank);
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(4,3);
  UShort_t det=GetDet(side,ring,rank);
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    if (TestRank(rank,fFlagHG[ring-1+3*(side-1)+6*(anode-1)])){
      c->cd(anode); 
      h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->Draw();
      h1_RawToF_HG[(det-1)+CHINU_nDets*(anode-1)]->SetDirectory(0);
    }
  }
  return(c);
}
