#include "FC_RawToFHistos.h"


FC_RawToFHistos::FC_RawToFHistos()
  : fFlag(0)
{
  
}

FC_RawToFHistos::~FC_RawToFHistos()
{
  fFlag=0;
}




Bool_t FC_RawToFHistos::TestAnode(UShort_t anode, UShort_t flag)
{
  UShort_t bit = 0;
  bit = bit | (1<<(anode-1));
  return(((flag & bit) >> (anode-1)) & 0x1);
}


void FC_RawToFHistos::DefineOneAnode(UShort_t anode)
{
  char name[100];
  if(!TestAnode(anode,fFlag)){
    sprintf(name,"RawToF_FC%d_HF",anode);
    h1_RawToF[anode-1] = new TH1F(name,name,30000,-1000,2000);
    h1_RawToF[anode-1]->GetXaxis()->SetTitle("raw ToF [ns, 100ps/bin]");
    h1_RawToF[anode-1]->GetXaxis()->SetTitleOffset(1.4);
    h1_RawToF[anode-1]->SetLineColor(kBlue);
    fFlag = fFlag | (1<<(anode-1));
  }
}




void FC_RawToFHistos::FillHistos_FCmult1(vector<Double_t>* vHF_time, UShort_t FC_anode, Double_t FC_time)
{
  if (TestAnode(FC_anode,fFlag)){
    h1_RawToF[FC_anode-1]->Fill(FC_time - vHF_time->at(vHF_time->size()-1));
  }
}


void FC_RawToFHistos::FillHistos(vector<Double_t>* vHF_time, vector<UShort_t>* vFC_anode, vector<Double_t>* vFC_time)
{
  for(UShort_t m=0; m<vFC_anode->size(); m++){
    if (TestAnode(vFC_anode->at(m),fFlag)){
      h1_RawToF[vFC_anode->at(m)-1]->Fill((float)(vFC_time->at(m) - vHF_time->at(vHF_time->size()-1)));
      //for(UShort_t n=0; n<vHF_time->size(); n++)
      //h1_RawToF[vFC_anode->at(m)-1]->Fill((float)(vFC_time->at(m) - vHF_time->at(n)));
    }
  }
}


TCanvas * FC_RawToFHistos::DrawOneAnode(UShort_t anode)
{
  char name[100];
  sprintf(name,"BeamToFraw_FC%d_HF",anode);
  TCanvas * c = new TCanvas(name,name,700,600);
  if (TestAnode(anode,fFlag)){
    c->cd(); 
    h1_RawToF[anode-1]->Draw();
    h1_RawToF[anode-1]->SetDirectory(0);
  }
  return(c);
}

TCanvas * FC_RawToFHistos::DrawAllAnodes()
{
  char name[100];
  sprintf(name,"BeamToFraw_AllAnodes");
  TCanvas * c = new TCanvas(name,name,1200,1200);
  c->Divide(4,3);
  for(UShort_t anode=1; anode<=FC_nAnodes; anode++){
    if (TestAnode(anode,fFlag)){
      c->cd(anode); 
      h1_RawToF[anode-1]->Draw();
      h1_RawToF[anode-1]->SetDirectory(0);
    }
  }
  return(c);
}
