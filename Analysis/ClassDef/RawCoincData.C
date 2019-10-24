#include "RawCoincData.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
RawCoincData::RawCoincData(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../data/Raw/Cf252/Run20_0001_Cf.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../../data/Raw/Cf252/Run20_0001_Cf.root");
      }
      f->GetObject("RawCoincData",tree);

   }
   Init(tree);
}

RawCoincData::~RawCoincData()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t RawCoincData::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t RawCoincData::LoadTree(Long64_t entry)
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

void RawCoincData::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   vB3lg_label = 0;
   vB3hg_label = 0;
   vB3lg_det = 0;
   vB3hg_det = 0;
   vB3lg_clock = 0;
   vB3hg_clock = 0;
   vB3lg_tdc = 0;
   vB3hg_tdc = 0;
   vB3lg_time = 0;
   vB3hg_time = 0;
   vB3lg_Q1 = 0;
   vB3hg_Q1 = 0;
   vB3lg_Q2 = 0;
   vB3hg_Q2 = 0;
   vB3lg_Q3 = 0;
   vB3hg_Q3 = 0;
   vB3lg_Q4 = 0;
   vB3hg_Q4 = 0;
   vFC_label = 0;
   vFC_anode = 0;
   vFC_clock = 0;
   vFC_tdc = 0;
   vFC_time = 0;
   vFC_Q1 = 0;
   vFC_Q2 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Event", &Event, &b_Event);
   fChain->SetBranchAddress("Label", &Label, &b_Label);
   fChain->SetBranchAddress("Clock", &Clock, &b_Clock);
   fChain->SetBranchAddress("B3lg_mult", B3lg_mult, &b_B3lg_mult);
   fChain->SetBranchAddress("B3hg_mult", B3hg_mult, &b_B3hg_mult);
   fChain->SetBranchAddress("vB3lg_label", &vB3lg_label, &b_vB3lg_label);
   fChain->SetBranchAddress("vB3hg_label", &vB3hg_label, &b_vB3hg_label);
   fChain->SetBranchAddress("vB3lg_det", &vB3lg_det, &b_vB3lg_det);
   fChain->SetBranchAddress("vB3hg_det", &vB3hg_det, &b_vB3hg_det);
   fChain->SetBranchAddress("vB3lg_clock", &vB3lg_clock, &b_vB3lg_clock);
   fChain->SetBranchAddress("vB3hg_clock", &vB3hg_clock, &b_vB3hg_clock);
   fChain->SetBranchAddress("vB3lg_tdc", &vB3lg_tdc, &b_vB3lg_tdc);
   fChain->SetBranchAddress("vB3hg_tdc", &vB3hg_tdc, &b_vB3hg_tdc);
   fChain->SetBranchAddress("vB3lg_time", &vB3lg_time, &b_vB3lg_time);
   fChain->SetBranchAddress("vB3hg_time", &vB3hg_time, &b_vB3hg_time);
   fChain->SetBranchAddress("vB3lg_Q1", &vB3lg_Q1, &b_vB3lg_Q1);
   fChain->SetBranchAddress("vB3hg_Q1", &vB3hg_Q1, &b_vB3hg_Q1);
   fChain->SetBranchAddress("vB3lg_Q2", &vB3lg_Q2, &b_vB3lg_Q2);
   fChain->SetBranchAddress("vB3hg_Q2", &vB3hg_Q2, &b_vB3hg_Q2);
   fChain->SetBranchAddress("vB3lg_Q3", &vB3lg_Q3, &b_vB3lg_Q3);
   fChain->SetBranchAddress("vB3hg_Q3", &vB3hg_Q3, &b_vB3hg_Q3);
   fChain->SetBranchAddress("vB3lg_Q4", &vB3lg_Q4, &b_vB3lg_Q4);
   fChain->SetBranchAddress("vB3hg_Q4", &vB3hg_Q4, &b_vB3hg_Q4);
   fChain->SetBranchAddress("FC_mult", FC_mult, &b_FC_mult);
   fChain->SetBranchAddress("vFC_label", &vFC_label, &b_vFC_label);
   fChain->SetBranchAddress("vFC_anode", &vFC_anode, &b_vFC_anode);
   fChain->SetBranchAddress("vFC_clock", &vFC_clock, &b_vFC_clock);
   fChain->SetBranchAddress("vFC_tdc", &vFC_tdc, &b_vFC_tdc);
   fChain->SetBranchAddress("vFC_time", &vFC_time, &b_vFC_time);
   fChain->SetBranchAddress("vFC_Q1", &vFC_Q1, &b_vFC_Q1);
   fChain->SetBranchAddress("vFC_Q2", &vFC_Q2, &b_vFC_Q2);
   Notify();
}

Bool_t RawCoincData::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void RawCoincData::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t RawCoincData::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
void RawCoincData::Loop()
{
//   In a ROOT session, you can do:
//      root> .L RawCoincData.C
//      root> RawCoincData t
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
