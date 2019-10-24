#ifndef _CHINU_RAWBKGTOF__H_
#define _CHINU_RAWBKGTOF__H_

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "../../SetupSpecific/setup_specific.h"

class CHINU_RawBkgToFHistos{

 public :
  TH1F * h1_RawToF_LG[CHINU_nDets];
  TH1F * h1_RawToF_HG[CHINU_nDets];

  CHINU_RawBkgToFHistos();
  ~CHINU_RawBkgToFHistos();

  void DefineOneDetLG(UShort_t, UShort_t,UShort_t);
  void DefineOneDetHG(UShort_t, UShort_t,UShort_t);

  void FillHistosLG_PULSERmult1(Double_t, vector<UShort_t>*, vector<Double_t>*, UShort_t*);
  void FillHistosHG_PULSERmult1(Double_t, vector<UShort_t>*, vector<Double_t>*, UShort_t*);

  TCanvas * DrawOneDetLG(UShort_t, UShort_t, UShort_t);
  TCanvas * DrawOneDetHG(UShort_t, UShort_t, UShort_t);
  TCanvas * DrawSideRingLG(UShort_t, UShort_t);
  TCanvas * DrawSideRingHG(UShort_t, UShort_t);
  TCanvas * DrawRankLG(UShort_t);
  TCanvas * DrawRankHG(UShort_t);

 public :
  UShort_t  GetDet(UShort_t, UShort_t, UShort_t);
  
 private :
  ULong64_t fFlagLG;
  ULong64_t fFlagHG;
  Bool_t    TestDet(UShort_t,ULong64_t);
};


#endif //_CHINU_RAWBKGTOF__H_
