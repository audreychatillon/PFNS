//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Oct 19 19:08:04 2019 by ROOT version 6.16/00
// from TTree RawSourceData/Run 0 File 0 - TRIGGERLESS MODE: ONLY NEUTRON DETECTORS
// found on file: ../../data/Raw/Cs137/Run9_0001_Cs.root
//////////////////////////////////////////////////////////

#ifndef RawSourceData_h
#define RawSourceData_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "vector"

class RawSourceData {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          Event;
   UInt_t          Label;
   ULong64_t       Clock;
   vector<unsigned int> *vNDETlg_label;
   vector<unsigned int> *vNDEThg_label;
   vector<unsigned short> *vNDETlg_det;
   vector<unsigned short> *vNDEThg_det;
   vector<ULong64_t> *vNDETlg_clock;
   vector<ULong64_t> *vNDEThg_clock;
   vector<double>  *vNDETlg_tdc;
   vector<double>  *vNDEThg_tdc;
   vector<double>  *vNDETlg_time;
   vector<double>  *vNDEThg_time;
   vector<unsigned int> *vNDETlg_Q1;
   vector<unsigned int> *vNDEThg_Q1;
   vector<unsigned int> *vNDETlg_Q2;
   vector<unsigned int> *vNDEThg_Q2;

   // List of branches
   TBranch        *b_Event;   //!
   TBranch        *b_Label;   //!
   TBranch        *b_Clock;   //!
   TBranch        *b_vNDETlg_label;   //!
   TBranch        *b_vNDEThg_label;   //!
   TBranch        *b_vNDETlg_det;   //!
   TBranch        *b_vNDEThg_det;   //!
   TBranch        *b_vNDETlg_clock;   //!
   TBranch        *b_vNDEThg_clock;   //!
   TBranch        *b_vNDETlg_tdc;   //!
   TBranch        *b_vNDEThg_tdc;   //!
   TBranch        *b_vNDETlg_time;   //!
   TBranch        *b_vNDEThg_time;   //!
   TBranch        *b_vNDETlg_Q1;   //!
   TBranch        *b_vNDEThg_Q1;   //!
   TBranch        *b_vNDETlg_Q2;   //!
   TBranch        *b_vNDEThg_Q2;   //!

   RawSourceData(TTree *tree=0);
   virtual ~RawSourceData();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

