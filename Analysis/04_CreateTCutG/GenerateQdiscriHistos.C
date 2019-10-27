#include "../ClassDef/CalCoincData.h"
#include "../ClassDef/CHINU_RawQHistos.h"
#include "../ClassDef/B3_RawQHistos.h"
#include "../../SetupSpecific/setup_specific.h"

#include "TH2.h"
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

  // === ========== === //
  // === INPUT DATA === //
  // === ========== === //

  TChain * ch = new TChain("CalData");
  ch->Add("../../data/Cal/Cf252/R2_to_R2.root");
  CalCoincData cal(ch);
  ULong64_t nentries = ch->GetEntries();

  char name[100];

 
  // === =========================== === //
  // === CREATE Q-DISCRI HISTOGRAMMS === //
  // === =========================== === //
  
  TFile* histofile_CHINULG = new TFile("histos/histo_Qdiscri_gamma_CHINUlg.root","recreate");
  TFile* histofile_CHINUHG = new TFile("histos/histo_Qdiscri_gamma_CHINUhg.root","recreate");

  CHINU_RawQHistos hqCHINU;
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t ring=1; ring<=3; ring++){
      for(UShort_t rank=1; rank<=9; rank++){
	det = hqCHINU.GetDet(side,ring,rank);

	hqCHINU.DefineDiscriLG(side,ring,rank);
	sprintf(name,"histo_Qdiscri_gamma_CHINUlg_%d",det);
	hqCHINU.h2_DiscriLG[det-1]->SetName(name);
	hqCHINU.h2_DiscriLG[det-1]->SetTitle(name);
	hqCHINU.h2_DiscriLG[det-1]->GetYaxis()->SetRangeUser(0,1.5);

	hqCHINU.DefineDiscriHG(side,ring,rank);
	sprintf(name,"histo_Qdiscri_gamma_CHINUhg_%d",det);
	hqCHINU.h2_DiscriHG[det-1]->SetName(name);
	hqCHINU.h2_DiscriHG[det-1]->SetTitle(name);	
	hqCHINU.h2_DiscriHG[det-1]->GetYaxis()->SetRangeUser(0,1.5);
      }
    }
  }


  TFile* histofile_B3LG = new TFile("histos/histo_Qdiscri_gamma_B3lg.root","recreate");
  TFile* histofile_B3HG = new TFile("histos/histo_Qdiscri_gamma_B3hg.root","recreate");

  B3_RawQHistos hqB3;
  for(UShort_t side=1; side<=2; side++){
    for(UShort_t pos=1; pos<=2; pos++){
      det = hqB3.GetDet(side,pos);
      
      hqB3.DefineDiscriLG(side,pos);
      sprintf(name,"histo_Qdiscri_gamma_B3lg_%d",det);
      hqB3.h2_DiscriLG[det-1]->SetName(name);
      hqB3.h2_DiscriLG[det-1]->SetTitle(name);
      hqB3.h2_DiscriLG[det-1]->GetYaxis()->SetRangeUser(0,1.5);
      
      hqB3.DefineDiscriHG(side,pos);
      sprintf(name,"histo_Qdiscri_gamma_B3hg_%d",det);
      hqB3.h2_DiscriHG[det-1]->SetName(name);
      hqB3.h2_DiscriHG[det-1]->SetTitle(name);	
      hqB3.h2_DiscriHG[det-1]->GetYaxis()->SetRangeUser(0,1.5);
    }
  }

  // === ================== === //
  // === LOOP OVER THE DATA === //
  // === ================== === //

  for(ULong64_t Entry=0; Entry<nentries; Entry++){
    
    cal.GetEntry(Entry);

    for(UShort_t m=0; m<cal.vPFN_CHINUlg_IsGammaToF->size(); m++)
      if(cal.vPFN_CHINUlg_IsGammaToF->at(m)) 
	hqCHINU.FillHistosLG(cal.vCHINUlg_det->at(m),cal.vCHINUlg_Q1->at(m),cal.vCHINUlg_Q2->at(m));

    for(UShort_t m=0; m<cal.vPFN_CHINUhg_IsGammaToF->size(); m++)
      if(cal.vPFN_CHINUhg_IsGammaToF->at(m)) 
	hqCHINU.FillHistosHG(cal.vCHINUhg_det->at(m),cal.vCHINUhg_Q1->at(m),cal.vCHINUhg_Q2->at(m));

    for(UShort_t m=0; m<cal.vPFN_B3lg_IsGammaToF->size(); m++)
      if(cal.vPFN_B3lg_IsGammaToF->at(m)) 
	hqB3.FillHistosLG(cal.vB3lg_det->at(m),cal.vB3lg_Q1->at(m),cal.vB3lg_Q2->at(m));

    for(UShort_t m=0; m<cal.vPFN_B3hg_IsGammaToF->size(); m++)
      if(cal.vPFN_B3hg_IsGammaToF->at(m)) 
	hqB3.FillHistosHG(cal.vB3hg_det->at(m),cal.vB3hg_Q1->at(m),cal.vB3hg_Q2->at(m));

  }// end of loop over the entries


  // === ================ === //
  // === SAVE HISTOTRAMMS === //
  // === ================ === //

  for(UShort_t det=0; det<CHINU_nDets; det++){
    histofile_CHINULG->cd(); hqCHINU.h2_DiscriLG[det]->Write();
    histofile_CHINUHG->cd(); hqCHINU.h2_DiscriHG[det]->Write();
  }
  for(UShort_t det=0; det<B3_nDets; det++){
    histofile_B3LG->cd(); hqB3.h2_DiscriLG[det]->Write();
    histofile_B3HG->cd(); hqB3.h2_DiscriHG[det]->Write();
  }
  histofile_CHINULG->ls(); histofile_CHINULG->Close();
  histofile_CHINUHG->ls(); histofile_CHINUHG->Close();
  histofile_B3LG->ls(); histofile_B3LG->Close();
  histofile_B3HG->ls(); histofile_B3HG->Close();
}

/////////////////////////////////////////
