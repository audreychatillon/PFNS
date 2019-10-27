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
  TH1I * h1_Q1LG[CHINU_nDets];
  TH1I * h1_Q2LG[CHINU_nDets];
  TH1I * h1_Q1HG[CHINU_nDets];
  TH1I * h1_Q2HG[CHINU_nDets];
  TH2F * h2_DiscriLG[CHINU_nDets];
  TH2F * h2_DiscriHG[CHINU_nDets];

  CHINU_RawQHistos();
  ~CHINU_RawQHistos();
  void Define1DLG(UShort_t,UShort_t,UShort_t);
  void Define1DHG(UShort_t,UShort_t,UShort_t);
  void DefineDiscriLG(UShort_t,UShort_t,UShort_t);
  void DefineDiscriHG(UShort_t,UShort_t,UShort_t);

  void FillHistosLG(vector<UShort_t> *, vector<UInt_t> *, vector<UInt_t> *);
  void FillHistosLG(UShort_t, UInt_t, UInt_t);
  void FillHistosHG(vector<UShort_t> *, vector<UInt_t> *, vector<UInt_t> *);
  void FillHistosHG(UShort_t, UInt_t, UInt_t);

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

 public :
  UShort_t fFlag1DLG[6];
  UShort_t fFlag1DHG[6];
  UShort_t fFlagDiscriLG[6];
  UShort_t fFlagDiscriHG[6];
  UShort_t GetDet(UShort_t,UShort_t,UShort_t);
  Bool_t   TestRank(UShort_t,ULong64_t);
};

#endif //CHINURawQHistos_h
