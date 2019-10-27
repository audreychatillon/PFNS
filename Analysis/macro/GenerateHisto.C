TChain* ch;

//////////////////////////////////////////
void GenerateHisto()
{

  ch = new TChain("CalData");
  ch->Add("../../data/Cal/Cf252/R2_to_R2.root");

  TFile* histofile = new TFile("histo/histo_discri_gamma_hg.root","update");

  TCanvas* c1 = new TCanvas("c1","c1",1200,1200);


  for(int det_number=1; det_number<55; det_number++){
    TString scond = Form("vPFN_CHINUhg_IsGammaToF && vCHINUhg_det==%i",det_number);

    ch->Draw("vCHINUhg_Q2/vCHINUhg_Q1:vCHINUhg_Q1>>h(500,0,1000000,200,0,2)",scond,"colz");
    TH1F *h = (TH1F*)gDirectory->FindObjectAny("h");
    TString histo_name = Form("histo_discri_gamma_hg_%i",det_number);
    h->SetName(histo_name);

    cout << "Histogram " << histo_name  << " has been generated." << endl; 

    h->Write();
  }
  histofile->Close();

}

/////////////////////////////////////////
