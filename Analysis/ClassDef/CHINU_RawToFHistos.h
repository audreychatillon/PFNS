#ifndef _CHINU_RAWTOF__H_
#define _CHINU_RAWTOF__H_

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "../../SetupSpecific/setup_specific.h"

class CHINU_RawToFHistos{

 public :
  TH1F * h1_RawToF_LG[CHINU_nDets*FC_nAnodes];
  TH1F * h1_RawToF_HG[CHINU_nDets*FC_nAnodes];

  CHINU_RawToFHistos();
  ~CHINU_RawToFHistos();

  void DefineOneAnodeOneDetLG(UShort_t, UShort_t, UShort_t,UShort_t);
  void DefineOneAnodeOneDetHG(UShort_t, UShort_t, UShort_t,UShort_t);


  void FillHistosLG_FCmult1(UShort_t, Double_t, vector<UShort_t>*, vector<Double_t>*, UShort_t*);
  void FillHistosHG_FCmult1(UShort_t, Double_t, vector<UShort_t>*, vector<Double_t>*, UShort_t*);

  TCanvas * DrawOneAnodeOneDetLG(UShort_t, UShort_t, UShort_t, UShort_t);
  TCanvas * DrawOneAnodeOneDetHG(UShort_t, UShort_t, UShort_t, UShort_t);
  TCanvas * DrawOneAnodeSideRingLG(UShort_t, UShort_t, UShort_t);
  TCanvas * DrawOneAnodeSideRingHG(UShort_t, UShort_t, UShort_t);
  TCanvas * DrawOneAnodeRankLG(UShort_t, UShort_t);
  TCanvas * DrawOneAnodeRankHG(UShort_t, UShort_t);
  TCanvas * DrawAllAnodesOneDetLG(UShort_t, UShort_t, UShort_t);
  TCanvas * DrawAllAnodesOneDetHG(UShort_t, UShort_t, UShort_t);

 public :
  UShort_t  GetDet(UShort_t, UShort_t, UShort_t);
  
 private :
  UShort_t fFlagLG[6*FC_nAnodes];
  UShort_t fFlagHG[6*FC_nAnodes];
  Bool_t   TestRank(UShort_t,UShort_t);
};


#endif //_CHINU_RAWTOF__H_
