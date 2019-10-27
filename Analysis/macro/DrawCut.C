TChain* ch;

//////////////////////////////////////////
void DrawCut()
{

  TFile* histofile = new TFile("histo/histo_discri_gamma_lg.root");
  TFile* cutfile = new TFile("cut_discri_gamma_lg.root","update");

  TCanvas* c1 = new TCanvas("c1","c1",1200,1200);


  for(int det_number=1; det_number<55; det_number++){
    TH1F *h = (TH1F*)histofile->FindObjectAny(Form("histo_discri_gamma_%i",det_number));

    h->Draw("colz");

    TCutG* cutg;
    cutg = (TCutG*)gPad->WaitPrimitive("CUTG","CutG");
    cutg->SetName(Form("cut_discri_gamma_%i",det_number));

    cutfile->cd();
    cutg->Write();
  }
  histofile->Close();
  cutfile->Close();

}

/////////////////////////////////////////
