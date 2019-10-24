#ifndef CHINURawQHistos_h
#define CHINURawQHistos_h

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "../../SetupSpecific/setup_specific.h"

class CHINU_RawQHistos{

 public :
  TH1I * h1_Q1LG[54];
  TH1I * h1_Q2LG[54];
  TH1I * h1_Q1HG[54];
  TH1I * h1_Q2HG[54];
  TH2F * h2_DiscriLG[54];
  TH2F * h2_DiscriHG[54];

  CHINU_RawQHistos();
  ~CHINU_RawQHistos();
  void Define1DLG(UShort_t,UShort_t,UShort_t);
  void Define1DHG(UShort_t,UShort_t,UShort_t);
  void Define1DSideRingLG(UShort_t,UShort_t);
  void Define1DSideRingHG(UShort_t,UShort_t);
  void Define1DRankLG(UShort_t);
  void Define1DRankHG(UShort_t);

  void DefineDiscriLG(UShort_t,UShort_t,UShort_t);
  void DefineDiscriHG(UShort_t,UShort_t,UShort_t);
  void DefineDiscriSideRingLG(UShort_t,UShort_t);
  void DefineDiscriSideRingHG(UShort_t,UShort_t);
  void DefineDiscriRankLG(UShort_t);
  void DefineDiscriRankHG(UShort_t);

  void FillHistosLG(vector<UShort_t> *, vector<UInt_t> *, vector<UInt_t> *);
  void FillHistosHG(vector<UShort_t> *, vector<UInt_t> *, vector<UInt_t> *);

  TCanvas* DrawHistos1DLG(UShort_t, UShort_t, UShort_t); 
  TCanvas* DrawHistos1DHG(UShort_t, UShort_t, UShort_t); 
  TCanvas* DrawHistos1DSideRingLG(UShort_t, UShort_t);
  TCanvas* DrawHistos1DSideRingHG(UShort_t, UShort_t);
  TCanvas* DrawHistos1DRankLG(UShort_t);
  TCanvas* DrawHistos1DRankHG(UShort_t);

  TCanvas* DrawHistoDiscriLG(UShort_t, UShort_t, UShort_t); 
  TCanvas* DrawHistoDiscriHG(UShort_t, UShort_t, UShort_t); 
  TCanvas* DrawHistosDiscriSideRingLG(UShort_t, UShort_t);
  TCanvas* DrawHistosDiscriSideRingHG(UShort_t, UShort_t);
  TCanvas* DrawHistosDiscriRankLG(UShort_t);
  TCanvas* DrawHistosDiscriRankHG(UShort_t);

 private :
  ULong64_t fFlag1DLG;
  ULong64_t fFlag1DHG;
  ULong64_t fFlagDiscriLG;
  ULong64_t fFlagDiscriHG;
  UShort_t  GetDet(UShort_t,UShort_t,UShort_t);
  Bool_t    TestDet(UShort_t,ULong64_t);
};

#endif //CHINURawQHistos_h
