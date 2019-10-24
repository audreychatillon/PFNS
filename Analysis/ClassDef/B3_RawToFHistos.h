#ifndef _B3_RAWTOF__H_
#define _B3_RAWTOF__H_

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "../../SetupSpecific/setup_specific.h"

class B3_RawToFHistos{

 public :
  TH1F * h1_RawToF_LG[B3_nDets*FC_nAnodes];
  TH1F * h1_RawToF_HG[B3_nDets*FC_nAnodes];

  B3_RawToFHistos();
  ~B3_RawToFHistos();

  void DefineOneAnodeOneDetLG(UShort_t, UShort_t, UShort_t);
  void DefineOneAnodeOneDetHG(UShort_t, UShort_t, UShort_t);

  void FillHistosLG_FCmult1(UShort_t, Double_t, vector<UShort_t>*, vector<Double_t>*, UShort_t*);
  void FillHistosHG_FCmult1(UShort_t, Double_t, vector<UShort_t>*, vector<Double_t>*, UShort_t*);

  TCanvas * DrawOneAnodeOneDetLG(UShort_t, UShort_t, UShort_t);
  TCanvas * DrawOneAnodeOneDetHG(UShort_t, UShort_t, UShort_t);
  TCanvas * DrawAllAnodesOneDetLG(UShort_t, UShort_t);
  TCanvas * DrawAllAnodesOneDetHG(UShort_t, UShort_t);
  TCanvas * DrawOneAnodeAllDetsLG(UShort_t);
  TCanvas * DrawOneAnodeAllDetsHG(UShort_t);
  TCanvas * DrawAllAnodesAllDetsLG();
  TCanvas * DrawAllAnodesAllDetsHG();

 public :
  UShort_t  GetDet(UShort_t, UShort_t);
  
 private :
  UChar_t fFlagLG[FC_nAnodes];
  UChar_t fFlagHG[FC_nAnodes];
  Bool_t  TestDet(UShort_t,UChar_t);
};


#endif //_B3_RAWTOF__H_
