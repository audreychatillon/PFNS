//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Oct 24 06:54:01 2019 by ROOT version 6.16/00
// from TTree RawCoincData/Run 3 File 1 - COINC MODE
// found on file: ../../data/Raw/Cf252/Run3_0001_Cf.root
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
   UShort_t        Label;
   ULong64_t       Clock;
   UShort_t        CHINUlg_mult[54];
   UShort_t        CHINUhg_mult[54];
   vector<unsigned short> *vCHINUlg_label;
   vector<unsigned short> *vCHINUhg_label;
   vector<unsigned short> *vCHINUlg_det;
   vector<unsigned short> *vCHINUhg_det;
   vector<ULong64_t> *vCHINUlg_clock;
   vector<ULong64_t> *vCHINUhg_clock;
   vector<double>  *vCHINUlg_tdc;
   vector<double>  *vCHINUhg_tdc;
   vector<double>  *vCHINUlg_time;
   vector<double>  *vCHINUhg_time;
   vector<unsigned int> *vCHINUlg_Q1;
   vector<unsigned int> *vCHINUhg_Q1;
   vector<unsigned int> *vCHINUlg_Q2;
   vector<unsigned int> *vCHINUhg_Q2;
   UShort_t        B3lg_mult[4];
   UShort_t        B3hg_mult[4];
   vector<unsigned short> *vB3lg_label;
   vector<unsigned short> *vB3hg_label;
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
   UShort_t        FC_mult[11];
   vector<unsigned short> *vFC_label;
   vector<unsigned short> *vFC_anode;
   vector<ULong64_t> *vFC_clock;
   vector<double>  *vFC_tdc;
   vector<double>  *vFC_time;
   vector<unsigned int> *vFC_Q1;
   vector<unsigned int> *vFC_Q2;
   vector<unsigned short> *vPULSER_label;
   vector<ULong64_t> *vPULSER_clock;
   vector<double>  *vPULSER_tdc;
   vector<double>  *vPULSER_time;
   vector<unsigned int> *vPULSER_Q1;
   vector<unsigned short> *vHF_label;
   vector<ULong64_t> *vHF_clock;
   vector<double>  *vHF_tdc;
   vector<double>  *vHF_time;
   vector<unsigned int> *vHF_Q1;
   vector<unsigned short> *vMACRO_label;
   vector<ULong64_t> *vMACRO_clock;
   vector<double>  *vMACRO_tdc;
   vector<double>  *vMACRO_time;
   vector<unsigned int> *vMACRO_Q1;

   // List of branches
   TBranch        *b_Event;   //!
   TBranch        *b_Label;   //!
   TBranch        *b_Clock;   //!
   TBranch        *b_CHINUlg_mult;   //!
   TBranch        *b_CHINUhg_mult;   //!
   TBranch        *b_vCHINUlg_label;   //!
   TBranch        *b_vCHINUhg_label;   //!
   TBranch        *b_vCHINUlg_det;   //!
   TBranch        *b_vCHINUhg_det;   //!
   TBranch        *b_vCHINUlg_clock;   //!
   TBranch        *b_vCHINUhg_clock;   //!
   TBranch        *b_vCHINUlg_tdc;   //!
   TBranch        *b_vCHINUhg_tdc;   //!
   TBranch        *b_vCHINUlg_time;   //!
   TBranch        *b_vCHINUhg_time;   //!
   TBranch        *b_vCHINUlg_Q1;   //!
   TBranch        *b_vCHINUhg_Q1;   //!
   TBranch        *b_vCHINUlg_Q2;   //!
   TBranch        *b_vCHINUhg_Q2;   //!
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
   TBranch        *b_FC_mult;   //!
   TBranch        *b_vFC_label;   //!
   TBranch        *b_vFC_anode;   //!
   TBranch        *b_vFC_clock;   //!
   TBranch        *b_vFC_tdc;   //!
   TBranch        *b_vFC_time;   //!
   TBranch        *b_vFC_Q1;   //!
   TBranch        *b_vFC_Q2;   //!
   TBranch        *b_vPULSER_label;   //!
   TBranch        *b_vPULSER_clock;   //!
   TBranch        *b_vPULSER_tdc;   //!
   TBranch        *b_vPULSER_time;   //!
   TBranch        *b_vPULSER_Q1;   //!
   TBranch        *b_vHF_label;   //!
   TBranch        *b_vHF_clock;   //!
   TBranch        *b_vHF_tdc;   //!
   TBranch        *b_vHF_time;   //!
   TBranch        *b_vHF_Q1;   //!
   TBranch        *b_vMACRO_label;   //!
   TBranch        *b_vMACRO_clock;   //!
   TBranch        *b_vMACRO_tdc;   //!
   TBranch        *b_vMACRO_time;   //!
   TBranch        *b_vMACRO_Q1;   //!

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

