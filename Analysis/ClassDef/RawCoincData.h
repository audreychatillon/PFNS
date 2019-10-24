//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Oct 23 18:57:46 2019 by ROOT version 6.16/00
// from TTree RawCoincData/Run 20 File 1 - COINC MODE
// found on file: ../../data/Raw/Cf252/Run20_0001_Cf.root
//////////////////////////////////////////////////////////

#ifndef RawCoincData_h
#define RawCoincData_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "vector"

class RawCoincData {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          Event;
   UInt_t          Label;
   ULong64_t       Clock;
   UShort_t        B3lg_mult[4];
   UShort_t        B3hg_mult[4];
   vector<unsigned int> *vB3lg_label;
   vector<unsigned int> *vB3hg_label;
   vector<unsigned short> *vB3lg_det;
   vector<unsigned short> *vB3hg_det;
   vector<ULong64_t> *vB3lg_clock;
   vector<ULong64_t> *vB3hg_clock;
   vector<double>  *vB3lg_tdc;
   vector<double>  *vB3hg_tdc;
   vector<double>  *vB3lg_time;
   vector<double>  *vB3hg_time;
   vector<unsigned int> *vB3lg_Q1;
   vector<unsigned int> *vB3hg_Q1;
   vector<unsigned int> *vB3lg_Q2;
   vector<unsigned int> *vB3hg_Q2;
   vector<unsigned int> *vB3lg_Q3;
   vector<unsigned int> *vB3hg_Q3;
   vector<unsigned int> *vB3lg_Q4;
   vector<unsigned int> *vB3hg_Q4;
   UShort_t        FC_mult[11];
   vector<unsigned int> *vFC_label;
   vector<unsigned short> *vFC_anode;
   vector<ULong64_t> *vFC_clock;
   vector<double>  *vFC_tdc;
   vector<double>  *vFC_time;
   vector<unsigned int> *vFC_Q1;
   vector<unsigned int> *vFC_Q2;

   // List of branches
   TBranch        *b_Event;   //!
   TBranch        *b_Label;   //!
   TBranch        *b_Clock;   //!
   TBranch        *b_B3lg_mult;   //!
   TBranch        *b_B3hg_mult;   //!
   TBranch        *b_vB3lg_label;   //!
   TBranch        *b_vB3hg_label;   //!
   TBranch        *b_vB3lg_det;   //!
   TBranch        *b_vB3hg_det;   //!
   TBranch        *b_vB3lg_clock;   //!
   TBranch        *b_vB3hg_clock;   //!
   TBranch        *b_vB3lg_tdc;   //!
   TBranch        *b_vB3hg_tdc;   //!
   TBranch        *b_vB3lg_time;   //!
   TBranch        *b_vB3hg_time;   //!
   TBranch        *b_vB3lg_Q1;   //!
   TBranch        *b_vB3hg_Q1;   //!
   TBranch        *b_vB3lg_Q2;   //!
   TBranch        *b_vB3hg_Q2;   //!
   TBranch        *b_vB3lg_Q3;   //!
   TBranch        *b_vB3hg_Q3;   //!
   TBranch        *b_vB3lg_Q4;   //!
   TBranch        *b_vB3hg_Q4;   //!
   TBranch        *b_FC_mult;   //!
   TBranch        *b_vFC_label;   //!
   TBranch        *b_vFC_anode;   //!
   TBranch        *b_vFC_clock;   //!
   TBranch        *b_vFC_tdc;   //!
   TBranch        *b_vFC_time;   //!
   TBranch        *b_vFC_Q1;   //!
   TBranch        *b_vFC_Q2;   //!

   RawCoincData(TTree *tree=0);
   virtual ~RawCoincData();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

