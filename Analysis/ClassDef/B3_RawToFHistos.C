#include "B3_RawToFHistos.h"


B3_RawToFHistos::B3_RawToFHistos()
{
  for(UShort_t anode=0; anode<FC_nAnodes; anode++) {
    fFlagLG[anode]=0;
    fFlagHG[anode]=0;
  }
}

B3_RawToFHistos::~B3_RawToFHistos()
{
  for(UShort_t anode=0; anode<FC_nAnodes; anode++) {
    fFlagLG[anode]=0;
    fFlagHG[anode]=0;
  }
}


UShort_t B3_RawToFHistos::GetDet(UShort_t side, UShort_t pos)
{
//side : 1=LEFT, 2=RIGHT
//pos  : 1=BACK, 2=FRONT
//--> in order to calculate det = 1..4
  return(pos + 2*(side-1));
}


Bool_t B3_RawToFHistos::TestDet(UShort_t det, UChar_t flag)
{
  UChar_t bit = 0;
  bit = bit | (1<<(det-1));
  return(((flag & bit) >> (det-1)) & 0x1);
}


void B3_RawToFHistos::DefineOneAnodeOneDetLG(UShort_t anode, UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det = GetDet(side,pos);
  if(!TestDet(det,fFlagLG[anode-1])){
    sprintf(name,"RawToF_%s_%s_LG_FC%d",sideVal[side-1].Data(),posVal[pos-1].Data(),anode);
    h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)] = new TH1F(name,name,5000,0,500);
    h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)]->GetXaxis()->SetTitle("raw ToF [ns, 100ps/bin]");
    h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)]->GetXaxis()->SetTitleOffset(1.4);
    h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)]->SetLineColor(kBlue);
    fFlagLG[anode-1] = fFlagLG[anode-1] | (1<<(det-1));
  }
}
void B3_RawToFHistos::DefineOneAnodeOneDetHG(UShort_t anode, UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det = GetDet(side,pos);
  if(!TestDet(det,fFlagHG[anode-1])){
    sprintf(name,"RawToF_%s_%s_HG_FC%d",sideVal[side-1].Data(),posVal[pos-1].Data(),anode);
    h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)] = new TH1F(name,name,5000,0,500);
    h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)]->GetXaxis()->SetTitle("raw ToF [ns, 100ps/bin]");
    h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)]->GetXaxis()->SetTitleOffset(1.4);
    h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)]->SetLineColor(kRed);
    fFlagHG[anode-1] = fFlagHG[anode-1] | (1<<(det-1));
  }
}

void B3_RawToFHistos::FillHistosLG_FCmult1(UShort_t FC_anode, Double_t FC_time, vector<UShort_t>* vN_det, vector<Double_t>*vN_time, UShort_t * Nmult)
{
  for(UShort_t m=0; m<vN_det->size(); m++){
    if (TestDet(vN_det->at(m),fFlagLG[FC_anode-1])){
      if(Nmult[vN_det->at(m)-1]>1) continue;
      h1_RawToF_LG[(vN_det->at(m)-1)+B3_nDets*(FC_anode-1)]->Fill((float)(vN_time->at(m) - FC_time));
    }
  }// end of loop over the neutron multiplicity
}
void B3_RawToFHistos::FillHistosHG_FCmult1(UShort_t FC_anode, Double_t FC_time, vector<UShort_t>* vN_det, vector<Double_t>*vN_time, UShort_t * Nmult)
{
  for(UShort_t m=0; m<vN_det->size(); m++){
    if (TestDet(vN_det->at(m),fFlagHG[FC_anode-1])){
      if(Nmult[vN_det->at(m)-1]>1) continue;
      h1_RawToF_HG[(vN_det->at(m)-1)+B3_nDets*(FC_anode-1)]->Fill((float)(vN_time->at(m) - FC_time));
    }
  }// end of loop over the neutron multiplicity
}




TCanvas * B3_RawToFHistos::DrawOneAnodeOneDetLG(UShort_t anode, UShort_t side, UShort_t pos)
{
  char name[100];
  sprintf(name,"ToFraw_FC%d_%s_B3_%s_LG",anode,sideVal[side-1].Data(),posVal[pos-1].Data());
  TCanvas * c = new TCanvas(name,name,700,600);
  UShort_t det=GetDet(side,pos);
  if (TestDet(det,fFlagLG[anode-1])){
    c->cd(); 
    h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)]->Draw();
    h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)]->SetDirectory(0);
  }
  return(c);
}
TCanvas * B3_RawToFHistos::DrawOneAnodeOneDetHG(UShort_t anode, UShort_t side, UShort_t pos)
{
  char name[100];
  sprintf(name,"ToFraw_FC%d_%s_B3_%s_HG",anode,sideVal[side-1].Data(),posVal[pos-1].Data());
  TCanvas * c = new TCanvas(name,name,700,600);
  UShort_t det=GetDet(side,pos);
  if (TestDet(det,fFlagHG[anode-1])){
    c->cd(); 
    h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)]->Draw();
    h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)]->SetDirectory(0);
  }
  return(c);
}
TCanvas * B3_RawToFHistos::DrawAllAnodesOneDetLG(UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det;
  sprintf(name,"ToFraw_FCall_%s_B3_%s_LG",sideVal[side-1].Data(),posVal[pos-1].Data());
  TCanvas * c = new TCanvas(name,name,1200,1100);
  c->Divide(4,3);
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    det=GetDet(side,pos);
    if (TestDet(det,fFlagLG[anode-1])){
      c->cd(anode); 
      h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)]->Draw();
      h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)]->SetDirectory(0);
    }
  }
    return(c);
}
TCanvas * B3_RawToFHistos::DrawAllAnodesOneDetHG(UShort_t side, UShort_t pos)
{
  char name[100];
  UShort_t det;
  sprintf(name,"ToFraw_FCall_%s_B3_%s_HG",sideVal[side-1].Data(),posVal[pos-1].Data());
  TCanvas * c = new TCanvas(name,name,1200,1100);
  c->Divide(4,3);
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    det=GetDet(side,pos);
    if (TestDet(det,fFlagHG[anode-1])){
      c->cd(anode); 
      h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)]->Draw();
      h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)]->SetDirectory(0);
    }
  }
    return(c);
}

TCanvas * B3_RawToFHistos::DrawOneAnodeAllDetsLG(UShort_t anode)
{
  char name[100];
  UShort_t det;
  sprintf(name,"ToFraw_FC%d_B3_AllDets_LG",anode);
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(2,2);
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t pos=1; pos<=2; pos++){
      det=GetDet(side,pos);
      if (TestDet(det,fFlagLG[anode-1])){
	c->cd(pos+2*(side-1)); 
	h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)]->Draw();
	h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)]->SetDirectory(0);	
      }
    }
  }
  return(c);
}
TCanvas * B3_RawToFHistos::DrawOneAnodeAllDetsHG(UShort_t anode)
{
  char name[100];
  UShort_t det;
  sprintf(name,"B3_ToFraw_FC%d_B3_AllDets_HG",anode);
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(2,2);
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t pos=1; pos<=2; pos++){
      det=GetDet(side,pos);
      if (TestDet(det,fFlagHG[anode-1])){
	c->cd(pos+2*(side-1)); 
	h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)]->Draw();
	h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)]->SetDirectory(0);
      }
    }
  }
  return(c);
}
TCanvas * B3_RawToFHistos::DrawAllAnodesAllDetsLG()
{
  char name[100];
  sprintf(name,"B3_ToFraw_AllAnodes_AllDet_LG");
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(11,4);
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    for(UShort_t det=1; det<=4; det++){
      if (TestDet(det,fFlagLG[anode-1])){
	c->cd(anode+FC_nAnodes*(det-1)); 
	h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)]->Draw();
	h1_RawToF_LG[(det-1)+B3_nDets*(anode-1)]->SetDirectory(0);
      }
    }
  }
  return(c);
}
TCanvas * B3_RawToFHistos::DrawAllAnodesAllDetsHG()
{
  char name[100];
  sprintf(name,"B3_ToFraw_AllAnodes_AllDet_HG");
  TCanvas * c = new TCanvas(name,name,2000,1200);
  c->Divide(11,4);
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    for(UShort_t det=1; det<=4; det++){
      if (TestDet(det,fFlagHG[anode-1])){
	c->cd(anode+FC_nAnodes*(det-1)); 
	h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)]->Draw();
	h1_RawToF_HG[(det-1)+B3_nDets*(anode-1)]->SetDirectory(0);
      }
    }
  }
  return(c);
}
