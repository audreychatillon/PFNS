#ifndef B3RawQHistos_h
#define B3RawQHistos_h

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "../../SetupSpecific/setup_specific.h"

class B3_RawQHistos{

 public :
  TH1I * h1_Q1LG[4];
  TH1I * h1_Q2LG[4];
  TH1I * h1_Q1HG[4];
  TH1I * h1_Q2HG[4];
  TH2F * h2_DiscriLG[4];
  TH2F * h2_DiscriHG[4];

  B3_RawQHistos();
  ~B3_RawQHistos();
  void Define1DLG(UShort_t,UShort_t);
  void Define1DHG(UShort_t,UShort_t);
  void Define1DSideLG(UShort_t);
  void Define1DSideHG(UShort_t);
  void Define1DAllLG();
  void Define1DAllHG();
  
  void DefineDiscriLG(UShort_t,UShort_t);
  void DefineDiscriHG(UShort_t,UShort_t);
  void DefineDiscriSideLG(UShort_t);
  void DefineDiscriSideHG(UShort_t);
  void DefineDiscriAllLG();
  void DefineDiscriAllHG();
  
  void FillHistosLG(vector<UShort_t> *, vector<UInt_t> *, vector<UInt_t> *);
  void FillHistosHG(vector<UShort_t> *, vector<UInt_t> *, vector<UInt_t> *);
  
  TCanvas* DrawHistos1DLG(UShort_t, UShort_t);
  TCanvas* DrawHistos1DHG(UShort_t, UShort_t);
  TCanvas* DrawHistos1DSideLG(UShort_t);
  TCanvas* DrawHistos1DSideHG(UShort_t);
  TCanvas* DrawHistos1DAllLG();
  TCanvas* DrawHistos1DAllHG();

  TCanvas* DrawHistoDiscriLG(UShort_t, UShort_t);
  TCanvas* DrawHistoDiscriHG(UShort_t, UShort_t);
  TCanvas* DrawHistosDiscriSideLG(UShort_t);
  TCanvas* DrawHistosDiscriSideHG(UShort_t);
  TCanvas* DrawHistosDiscriAllLG();
  TCanvas* DrawHistosDiscriAllHG();

 private :
  UChar_t fFlag1DLG;
  UChar_t fFlag1DHG;
  UChar_t fFlagDiscriLG;
  UChar_t fFlagDiscriHG;
  UShort_t  GetDet(UShort_t,UShort_t);
  Bool_t    TestDet(UShort_t,UChar_t);
};

#endif //B3RawQHistos_h
