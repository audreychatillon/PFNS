#ifndef HFRawQHisto_h
#define HFRawQHisto_h

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "../../SetupSpecific/setup_specific.h"

class HF_RawQHisto{

 public :
  TH1I * h1_Q1;

  HF_RawQHisto();
  ~HF_RawQHisto();
  void DefineHisto();
  void FillHisto(vector<UInt_t> *);
  TCanvas * DrawHisto();

};

#endif //HFRawQHisto_h
