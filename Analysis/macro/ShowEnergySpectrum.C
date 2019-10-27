TChain* ch;

TCutG* cut_gamma;
TCutG* cut_neutron;

void LoadCut();

//////////////////////////////////////////
void ShowEnergySpectrum()
{
  LoadCut();

  ch = new TChain("CalData");
  ch->Add("../../data/root_cal/R23_to_R23.root");

  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  TCanvas* c2 = new TCanvas("c2","c2",600,600);

  //ch->Draw("vCHINUlg_Q2/vCHINUlg_Q1:vCHINUlg_Q1>>h(200,0,500000,200,0,2)","vCHINUlg_det==41 && vPFN_CHINUlg_IsGammaToF");
  //cut_gamma->Draw("same");

  c2->cd()->SetLogy();
  ch->Draw("vCHINUlg_Q1:vPFN_CHINUlg_Ene>>h2(400,0,20,1000,0,500000)","vCHINUlg_det==41 && !cut_gamma_41 && vPFN_CHINUlg_ToF>15 && FC_Q1>1700","colz");
  cut_neutron->Draw("same");

  c1->cd()->SetLogy();
  ch->Draw("vPFN_CHINUlg_Ene>>h3(400,0,20)","vCHINUlg_det==41 && !cut_gamma_41 && vPFN_CHINUlg_ToF>15 && FC_Q1>1700 && cut_neutron");

}

//////////////////////////////////////////
void LoadCut()
{
  TFile* File_cut_gamma = new TFile("cut/cut_gamma_41.root");
  cut_gamma = (TCutG*) File_cut_gamma->FindObjectAny("cut_gamma_41");
  cut_gamma->SetLineWidth(2);
  cut_gamma->SetLineColor(2);

  TFile* File_neutron = new TFile("cut/cut_neutron.root");
  cut_neutron = (TCutG*) File_neutron->FindObjectAny("cut_neutron");
  cut_neutron->SetLineWidth(2);
  cut_neutron->SetLineColor(2);
}
//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
