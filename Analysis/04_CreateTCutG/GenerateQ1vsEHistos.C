#include "../ClassDef/CalCoincData.h"
#include "../../SetupSpecific/setup_specific.h"

#include "TH2.h"
#include "TCutG.h"
#include "TFile.h"

UShort_t GetDetCHINU(UShort_t side, UShort_t ring, UShort_t rank)
{
  return(rank + 9*(ring-1) + 27*(side-1));
}

UShort_t GetDetB3(UShort_t side, UShort_t pos)
{
    return(pos + 2*(side-1));
}

void run()
{
  UShort_t det;
  Float_t  ratio;
  UInt_t   Q1;

  // === ========== === //
  // === INPUT DATA === //
  // === ========== === //

  TChain * ch = new TChain("CalData");
  ch->Add("../../data/Cal/Cf252/R2_to_R2.root");
  CalCoincData cal(ch);
  ULong64_t nentries = ch->GetEntries();

  char name[100];

  // === ============= === //
  // === GET THE TCUTG === //
  // === ============= === //
  TFile * f_CHINUlg = new TFile("cut/cut_discri_gamma_CHINUlg.root","read");
  TFile * f_CHINUhg = new TFile("cut/cut_discri_gamma_CHINUhg.root","read");
  TCutG * cut_CHINUlg[CHINU_nDets];
  TCutG * cut_CHINUhg[CHINU_nDets];
  for(det=1; det<=CHINU_nDets; det++){
    sprintf(name,"cut_discri_gamma_%d",det);
    cut_CHINUlg[det-1] = (TCutG*)f_CHINUlg->Get(name);
    sprintf(name,"cut_discri_gamma_CHINUlg_%d",det);
    cut_CHINUlg[det-1]->SetName(name);
    cut_CHINUlg[det-1]->SetTitle(name);
    cut_CHINUlg[det-1]->ls();

    sprintf(name,"cut_discri_gamma_%d",det);
    cut_CHINUhg[det-1] = (TCutG*)f_CHINUhg->Get(name);
    sprintf(name,"cut_discri_gamma_CHINUhg_%d",det);
    cut_CHINUhg[det-1]->SetName(name);
    cut_CHINUhg[det-1]->SetTitle(name);
    cut_CHINUhg[det-1]->ls();
  }

  TFile * f_B3lg = new TFile("cut/cut_discri_gamma_B3lg.root","read");
  TFile * f_B3hg = new TFile("cut/cut_discri_gamma_B3hg.root","read");
  TCutG * cut_B3lg[B3_nDets];
  TCutG * cut_B3hg[B3_nDets];
  for(det=1; det<=B3_nDets; det++){
    sprintf(name,"cut_discri_gamma_%d",det);
    cut_B3lg[det-1] = (TCutG*)f_B3lg->Get(name);
    sprintf(name,"cut_discri_gamma_B3lg_%d",det);
    cut_B3lg[det-1]->SetName(name);
    cut_B3lg[det-1]->SetTitle(name);
    cut_B3lg[det-1]->ls();

    sprintf(name,"cut_discri_gamma_%d",det);
    cut_B3hg[det-1] = (TCutG*)f_B3hg->Get(name);
    sprintf(name,"cut_discri_gamma_B3hg_%d",det);
    cut_B3hg[det-1]->SetName(name);
    cut_B3hg[det-1]->SetTitle(name);
    cut_B3hg[det-1]->ls();
  }



  // === =========================== === //
  // === CREATE Q-DISCRI HISTOGRAMMS === //
  // === =========================== === //
  
  TFile* histofile_CHINULG = new TFile("histos/histo_Q1vsE_neutron_CHINUlg.root","recreate");
  TFile* histofile_CHINUHG = new TFile("histos/histo_Q1vsE_neutron_CHINUhg.root","recreate");

  UShort_t det_number; 
  TH2F * h2_EQ1Discri_CHINULG[CHINU_nDets];
  TH2F * h2_EQ1Discri_CHINUHG[CHINU_nDets];

  for(UShort_t det=1; det<=CHINU_nDets; det++){
    sprintf(name,"histo_Q1E_neutron_CHINUlg_%i",det);
    h2_EQ1Discri_CHINULG[det-1] = new TH2F(name,name,1000,0,20,1025,0,524800);
    sprintf(name,"histo_Q1E_neutron_CHINUhg_%i",det);
    h2_EQ1Discri_CHINUHG[det-1] = new TH2F(name,name,1000,0,20,1025,0,524800);
  }

  
  TFile* histofile_B3LG = new TFile("histos/histo_Q1vsE_neutron_B3lg.root","update");
  TFile* histofile_B3HG = new TFile("histos/histo_Q1vsE_neutron_B3hg.root","update");
  TH2F * h2_EQ1Discri_B3LG[B3_nDets];
  TH2F * h2_EQ1Discri_B3HG[B3_nDets];
  for(UShort_t det=1; det<=B3_nDets; det++){
    sprintf(name,"histo_Q1E_neutron_B3lg_%i",det);
    h2_EQ1Discri_B3LG[det-1] = new TH2F(name,name,1025,0,524800,1000,0,20);
    sprintf(name,"histo_Q1E_neutron_B3hg_%i",det);
    h2_EQ1Discri_B3HG[det-1] = new TH2F(name,name,1025,0,524800,1000,0,20);
  }
  


  // === ================== === //
  // === LOOP OVER THE DATA === //
  // === ================== === //

  for(ULong64_t Entry=0; Entry<nentries; Entry++){
    
    cal.GetEntry(Entry);

    for(UShort_t m=0; m<cal.vPFN_CHINUlg_IsGammaToF->size(); m++){
      det   = cal.vCHINUlg_det->at(m);
      Q1    = cal.vCHINUlg_Q1->at(m);
      if (Q1>0) ratio = (Float_t)cal.vCHINUlg_Q2->at(m) / (Float_t)Q1;
      else ratio=-1;
      if ( (!cut_CHINUlg[det-1]->IsInside(Q1,ratio))&&
	   (cal.vPFN_CHINUlg_ToF->at(m)>15)&&
	   (cal.FC_Q1>1700) ){
	h2_EQ1Discri_CHINULG[det-1]->Fill(cal.vPFN_CHINUlg_Ene->at(m),Q1);
      }
    }
  
    for(UShort_t m=0; m<cal.vPFN_CHINUhg_IsGammaToF->size(); m++){
      det   = cal.vCHINUhg_det->at(m);
      Q1    = cal.vCHINUhg_Q1->at(m);
      if (Q1>0) ratio = (Float_t)cal.vCHINUhg_Q2->at(m) / (Float_t)Q1;
      else ratio=-1;
      if ( (!cut_CHINUhg[det-1]->IsInside(Q1,ratio))&&
	   (cal.vPFN_CHINUhg_ToF->at(m)>15)&&
	   (cal.FC_Q1>1700) ){
	h2_EQ1Discri_CHINUHG[det-1]->Fill(cal.vPFN_CHINUhg_Ene->at(m),Q1);
      }
    }
    
    for(UShort_t m=0; m<cal.vPFN_B3lg_IsGammaToF->size(); m++){
      det   = cal.vB3lg_det->at(m);
      Q1    = cal.vB3lg_Q1->at(m);
      if (Q1>0) ratio = (Float_t)cal.vB3lg_Q2->at(m) / (Float_t)Q1;
      else ratio=-1;
      if ( (!cut_B3lg[det-1]->IsInside(Q1,ratio))&&
	   (cal.vPFN_B3lg_ToF->at(m)>15)&&
	   (cal.FC_Q1>1700) ){
	h2_EQ1Discri_B3LG[det-1]->Fill(cal.vPFN_B3lg_Ene->at(m),Q1);
      }
    }
  
    for(UShort_t m=0; m<cal.vPFN_B3hg_IsGammaToF->size(); m++){
      det   = cal.vB3hg_det->at(m);
      Q1    = cal.vB3hg_Q1->at(m);
      if (Q1>0) ratio = (Float_t)cal.vB3hg_Q2->at(m) / (Float_t)Q1;
      else ratio=-1;
      if ( (!cut_B3hg[det-1]->IsInside(Q1,ratio))&&
	   (cal.vPFN_B3hg_ToF->at(m)>15)&&
	   (cal.FC_Q1>1700) ){
	h2_EQ1Discri_B3HG[det-1]->Fill(cal.vPFN_B3hg_Ene->at(m),Q1);
      }
    }
 
  }// end of loop over the entries


  // === ================ === //
  // === SAVE HISTOTRAMMS === //
  // === ================ === //

  for(UShort_t det=0; det<CHINU_nDets; det++){
    histofile_CHINULG->cd(); h2_EQ1Discri_CHINULG[det]->Write();
    histofile_CHINUHG->cd(); h2_EQ1Discri_CHINUHG[det]->Write();
  }

  for(UShort_t det=0; det<B3_nDets; det++){
    histofile_B3LG->cd(); h2_EQ1Discri_B3LG[det]->Write();
    histofile_B3HG->cd(); h2_EQ1Discri_B3HG[det]->Write();
  }

  histofile_CHINULG->ls(); histofile_CHINULG->Close();
  histofile_CHINUHG->ls(); histofile_CHINUHG->Close();
  histofile_B3LG->ls(); histofile_B3LG->Close();
  histofile_B3HG->ls(); histofile_B3HG->Close();


}

/////////////////////////////////////////
