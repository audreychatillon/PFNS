//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Oct 27 19:02:49 2019 by ROOT version 6.16/00
// from TTree CalData/Run 2 File 2 - Cal Data - ATTENTION ONLY EVENTS WITH MULT NDET>0
// found on file: ../../data/Cal/Cf252/R2_to_R2.root
//////////////////////////////////////////////////////////

#ifndef CalCoincData_h
#define CalCoincData_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "vector"

class CalCoincData {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          Event;
   UShort_t        FC_anode;
   UShort_t        FC_Q1;
   UShort_t        FC_Q2;
   vector<unsigned short> *vCHINUlg_det;
   vector<unsigned short> *vCHINUhg_det;
   vector<unsigned int> *vCHINUlg_Q1;
   vector<unsigned int> *vCHINUhg_Q1;
   vector<unsigned int> *vCHINUlg_Q2;
   vector<unsigned int> *vCHINUhg_Q2;
   vector<double>  *vPFN_CHINUlg_ToF;
   vector<double>  *vPFN_CHINUhg_ToF;
   vector<bool>    *vPFN_CHINUlg_IsGammaToF;
   vector<bool>    *vPFN_CHINUhg_IsGammaToF;
   vector<bool>    *vPFN_CHINUlg_IsGammaQdiscri;
   vector<bool>    *vPFN_CHINUhg_IsGammaQdiscri;
   vector<double>  *vPFN_CHINUlg_Ene;
   vector<double>  *vPFN_CHINUhg_Ene;
   vector<unsigned short> *vB3lg_det;
   vector<unsigned short> *vB3hg_det;
   vector<unsigned int> *vB3lg_Q1;
   vector<unsigned int> *vB3hg_Q1;
   vector<unsigned int> *vB3lg_Q2;
   vector<unsigned int> *vB3hg_Q2;
   vector<double>  *vPFN_B3lg_ToF;
   vector<double>  *vPFN_B3hg_ToF;
   vector<bool>    *vPFN_B3lg_IsGammaToF;
   vector<bool>    *vPFN_B3hg_IsGammaToF;
   vector<bool>    *vPFN_B3lg_IsGammaQdiscri;
   vector<bool>    *vPFN_B3hg_IsGammaQdiscri;
   vector<double>  *vPFN_B3lg_Ene;
   vector<double>  *vPFN_B3hg_Ene;

   // List of branches
   TBranch        *b_Event;   //!
   TBranch        *b_FC_anode;   //!
   TBranch        *b_FC_Q1;   //!
   TBranch        *b_FC_Q2;   //!
   TBranch        *b_vCHINUlg_det;   //!
   TBranch        *b_vCHINUhg_det;   //!
   TBranch        *b_vCHINUlg_Q1;   //!
   TBranch        *b_vCHINUhg_Q1;   //!
   TBranch        *b_vCHINUlg_Q2;   //!
   TBranch        *b_vCHINUhg_Q2;   //!
   TBranch        *b_vPFN_CHINUlg_ToF;   //!
   TBranch        *b_vPFN_CHINUhg_ToF;   //!
   TBranch        *b_vPFN_CHINUlg_IsGammaToF;   //!
   TBranch        *b_vPFN_CHINUhg_IsGammaToF;   //!
   TBranch        *b_vPFN_CHINUlg_IsGammaQdiscri;   //!
   TBranch        *b_vPFN_CHINUhg_IsGammaQdiscri;   //!
   TBranch        *b_vPFN_CHINUlg_Ene;   //!
   TBranch        *b_vPFN_CHINUhg_Ene;   //!
   TBranch        *b_vB3lg_det;   //!
   TBranch        *b_vB3hg_det;   //!
   TBranch        *b_vB3lg_Q1;   //!
   TBranch        *b_vB3hg_Q1;   //!
   TBranch        *b_vB3lg_Q2;   //!
   TBranch        *b_vB3hg_Q2;   //!
   TBranch        *b_vPFN_B3lg_ToF;   //!
   TBranch        *b_vPFN_B3hg_ToF;   //!
   TBranch        *b_vPFN_B3lg_IsGammaToF;   //!
   TBranch        *b_vPFN_B3hg_IsGammaToF;   //!
   TBranch        *b_vPFN_B3lg_IsGammaQdiscri;   //!
   TBranch        *b_vPFN_B3hg_IsGammaQdiscri;   //!
   TBranch        *b_vPFN_B3lg_Ene;   //!
   TBranch        *b_vPFN_B3hg_Ene;   //!

   CalCoincData(TTree *tree=0);
   virtual ~CalCoincData();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

