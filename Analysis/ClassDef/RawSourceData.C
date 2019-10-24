#include "RawSourceData.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
RawSourceData::RawSourceData(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../data/Raw/Cs137/Run9_0001_Cs.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../../data/Raw/Cs137/Run9_0001_Cs.root");
      }
      f->GetObject("RawSourceData",tree);

   }
   Init(tree);
}

RawSourceData::~RawSourceData()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t RawSourceData::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t RawSourceData::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void RawSourceData::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   vNDETlg_label = 0;
   vNDEThg_label = 0;
   vNDETlg_det = 0;
   vNDEThg_det = 0;
   vNDETlg_clock = 0;
   vNDEThg_clock = 0;
   vNDETlg_tdc = 0;
   vNDEThg_tdc = 0;
   vNDETlg_time = 0;
   vNDEThg_time = 0;
   vNDETlg_Q1 = 0;
   vNDEThg_Q1 = 0;
   vNDETlg_Q2 = 0;
   vNDEThg_Q2 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Event", &Event, &b_Event);
   fChain->SetBranchAddress("Label", &Label, &b_Label);
   fChain->SetBranchAddress("Clock", &Clock, &b_Clock);
   fChain->SetBranchAddress("vNDETlg_label", &vNDETlg_label, &b_vNDETlg_label);
   fChain->SetBranchAddress("vNDEThg_label", &vNDEThg_label, &b_vNDEThg_label);
   fChain->SetBranchAddress("vNDETlg_det", &vNDETlg_det, &b_vNDETlg_det);
   fChain->SetBranchAddress("vNDEThg_det", &vNDEThg_det, &b_vNDEThg_det);
   fChain->SetBranchAddress("vNDETlg_clock", &vNDETlg_clock, &b_vNDETlg_clock);
   fChain->SetBranchAddress("vNDEThg_clock", &vNDEThg_clock, &b_vNDEThg_clock);
   fChain->SetBranchAddress("vNDETlg_tdc", &vNDETlg_tdc, &b_vNDETlg_tdc);
   fChain->SetBranchAddress("vNDEThg_tdc", &vNDEThg_tdc, &b_vNDEThg_tdc);
   fChain->SetBranchAddress("vNDETlg_time", &vNDETlg_time, &b_vNDETlg_time);
   fChain->SetBranchAddress("vNDEThg_time", &vNDEThg_time, &b_vNDEThg_time);
   fChain->SetBranchAddress("vNDETlg_Q1", &vNDETlg_Q1, &b_vNDETlg_Q1);
   fChain->SetBranchAddress("vNDEThg_Q1", &vNDEThg_Q1, &b_vNDEThg_Q1);
   fChain->SetBranchAddress("vNDETlg_Q2", &vNDETlg_Q2, &b_vNDETlg_Q2);
   fChain->SetBranchAddress("vNDEThg_Q2", &vNDEThg_Q2, &b_vNDEThg_Q2);
   Notify();
}

Bool_t RawSourceData::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void RawSourceData::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t RawSourceData::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
void RawSourceData::Loop()
{
//   In a ROOT session, you can do:
//      root> .L RawSourceData.C
//      root> RawSourceData t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}
