#ifndef _FC_RAWTOF__H_
#define _FC_RAWTOF__H_

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "../../SetupSpecific/setup_specific.h"

class FC_RawToFHistos{

 public :
  TH1F * h1_RawToF[FC_nAnodes];

  FC_RawToFHistos();
  ~FC_RawToFHistos();

  void DefineOneAnode(UShort_t);

  void FillHistos_FCmult1(vector<Double_t>*, UShort_t, Double_t);
  void FillHistos(vector<Double_t>*, vector<UShort_t>*, vector<Double_t>*);

  TCanvas * DrawOneAnode(UShort_t);
  TCanvas * DrawAllAnodes();

 private :
  UShort_t fFlag;
  Bool_t TestAnode(UShort_t, UShort_t);
};


#endif //_FC_RAWTOF__H_
