#ifndef FC_RAWQ__H
#define FC_RAWQ__H

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "../../SetupSpecific/setup_specific.h"

class FC_RawQHistos{

 public :
  TH1I * h1_Q1[FC_nAnodes];
  TH1I * h1_Q2[FC_nAnodes];
  TH2I * h2_Q2vsQ1[FC_nAnodes];

  FC_RawQHistos();
  ~FC_RawQHistos();
  void Define1D(UShort_t);
  void DefineDiscri(UShort_t);

  void FillHistos(vector<UShort_t>*, vector<UInt_t>*, vector<UInt_t>*);

  TCanvas * Draw1DHistos(UShort_t);
  TCanvas * Draw1DAllHistos();
  TCanvas * DrawDiscriHisto(UShort_t);
  TCanvas * DrawDiscriAllHistos();
  
 private :
  UShort_t fFlag1D;
  UShort_t fFlagDiscri;
  Bool_t   TestAnode(UShort_t,UShort_t);
};



#endif // FC_RAWQ__H
