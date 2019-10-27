#include "CalCoincData.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
CalCoincData::CalCoincData(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../data/Cal/Cf252/R2_to_R2.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../../data/Cal/Cf252/R2_to_R2.root");
      }
      f->GetObject("CalData",tree);

   }
   Init(tree);
}

CalCoincData::~CalCoincData()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t CalCoincData::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t CalCoincData::LoadTree(Long64_t entry)
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

void CalCoincData::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   vCHINUlg_det = 0;
   vCHINUhg_det = 0;
   vCHINUlg_Q1 = 0;
   vCHINUhg_Q1 = 0;
   vCHINUlg_Q2 = 0;
   vCHINUhg_Q2 = 0;
   vPFN_CHINUlg_ToF = 0;
   vPFN_CHINUhg_ToF = 0;
   vPFN_CHINUlg_IsGammaToF = 0;
   vPFN_CHINUhg_IsGammaToF = 0;
   vPFN_CHINUlg_IsGammaQdiscri = 0;
   vPFN_CHINUhg_IsGammaQdiscri = 0;
   vPFN_CHINUlg_Ene = 0;
   vPFN_CHINUhg_Ene = 0;
   vB3lg_det = 0;
   vB3hg_det = 0;
   vB3lg_Q1 = 0;
   vB3hg_Q1 = 0;
   vB3lg_Q2 = 0;
   vB3hg_Q2 = 0;
   vPFN_B3lg_ToF = 0;
   vPFN_B3hg_ToF = 0;
   vPFN_B3lg_IsGammaToF = 0;
   vPFN_B3hg_IsGammaToF = 0;
   vPFN_B3lg_IsGammaQdiscri = 0;
   vPFN_B3hg_IsGammaQdiscri = 0;
   vPFN_B3lg_Ene = 0;
   vPFN_B3hg_Ene = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Event", &Event, &b_Event);
   fChain->SetBranchAddress("FC_anode", &FC_anode, &b_FC_anode);
   fChain->SetBranchAddress("FC_Q1", &FC_Q1, &b_FC_Q1);
   fChain->SetBranchAddress("FC_Q2", &FC_Q2, &b_FC_Q2);
   fChain->SetBranchAddress("vCHINUlg_det", &vCHINUlg_det, &b_vCHINUlg_det);
   fChain->SetBranchAddress("vCHINUhg_det", &vCHINUhg_det, &b_vCHINUhg_det);
   fChain->SetBranchAddress("vCHINUlg_Q1", &vCHINUlg_Q1, &b_vCHINUlg_Q1);
   fChain->SetBranchAddress("vCHINUhg_Q1", &vCHINUhg_Q1, &b_vCHINUhg_Q1);
   fChain->SetBranchAddress("vCHINUlg_Q2", &vCHINUlg_Q2, &b_vCHINUlg_Q2);
   fChain->SetBranchAddress("vCHINUhg_Q2", &vCHINUhg_Q2, &b_vCHINUhg_Q2);
   fChain->SetBranchAddress("vPFN_CHINUlg_ToF", &vPFN_CHINUlg_ToF, &b_vPFN_CHINUlg_ToF);
   fChain->SetBranchAddress("vPFN_CHINUhg_ToF", &vPFN_CHINUhg_ToF, &b_vPFN_CHINUhg_ToF);
   fChain->SetBranchAddress("vPFN_CHINUlg_IsGammaToF", &vPFN_CHINUlg_IsGammaToF, &b_vPFN_CHINUlg_IsGammaToF);
   fChain->SetBranchAddress("vPFN_CHINUhg_IsGammaToF", &vPFN_CHINUhg_IsGammaToF, &b_vPFN_CHINUhg_IsGammaToF);
   fChain->SetBranchAddress("vPFN_CHINUlg_IsGammaQdiscri", &vPFN_CHINUlg_IsGammaQdiscri, &b_vPFN_CHINUlg_IsGammaQdiscri);
   fChain->SetBranchAddress("vPFN_CHINUhg_IsGammaQdiscri", &vPFN_CHINUhg_IsGammaQdiscri, &b_vPFN_CHINUhg_IsGammaQdiscri);
   fChain->SetBranchAddress("vPFN_CHINUlg_Ene", &vPFN_CHINUlg_Ene, &b_vPFN_CHINUlg_Ene);
   fChain->SetBranchAddress("vPFN_CHINUhg_Ene", &vPFN_CHINUhg_Ene, &b_vPFN_CHINUhg_Ene);
   fChain->SetBranchAddress("vB3lg_det", &vB3lg_det, &b_vB3lg_det);
   fChain->SetBranchAddress("vB3hg_det", &vB3hg_det, &b_vB3hg_det);
   fChain->SetBranchAddress("vB3lg_Q1", &vB3lg_Q1, &b_vB3lg_Q1);
   fChain->SetBranchAddress("vB3hg_Q1", &vB3hg_Q1, &b_vB3hg_Q1);
   fChain->SetBranchAddress("vB3lg_Q2", &vB3lg_Q2, &b_vB3lg_Q2);
   fChain->SetBranchAddress("vB3hg_Q2", &vB3hg_Q2, &b_vB3hg_Q2);
   fChain->SetBranchAddress("vPFN_B3lg_ToF", &vPFN_B3lg_ToF, &b_vPFN_B3lg_ToF);
   fChain->SetBranchAddress("vPFN_B3hg_ToF", &vPFN_B3hg_ToF, &b_vPFN_B3hg_ToF);
   fChain->SetBranchAddress("vPFN_B3lg_IsGammaToF", &vPFN_B3lg_IsGammaToF, &b_vPFN_B3lg_IsGammaToF);
   fChain->SetBranchAddress("vPFN_B3hg_IsGammaToF", &vPFN_B3hg_IsGammaToF, &b_vPFN_B3hg_IsGammaToF);
   fChain->SetBranchAddress("vPFN_B3lg_IsGammaQdiscri", &vPFN_B3lg_IsGammaQdiscri, &b_vPFN_B3lg_IsGammaQdiscri);
   fChain->SetBranchAddress("vPFN_B3hg_IsGammaQdiscri", &vPFN_B3hg_IsGammaQdiscri, &b_vPFN_B3hg_IsGammaQdiscri);
   fChain->SetBranchAddress("vPFN_B3lg_Ene", &vPFN_B3lg_Ene, &b_vPFN_B3lg_Ene);
   fChain->SetBranchAddress("vPFN_B3hg_Ene", &vPFN_B3hg_Ene, &b_vPFN_B3hg_Ene);
   Notify();
}

Bool_t CalCoincData::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void CalCoincData::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t CalCoincData::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
void CalCoincData::Loop()
{
//   In a ROOT session, you can do:
//      root> .L CalCoincData.C
//      root> CalCoincData t
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
