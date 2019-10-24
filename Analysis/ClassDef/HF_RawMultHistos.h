#ifndef HFRawMultHistos_h
#define HFRawMultHistos_h

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "../../SetupSpecific/setup_specific.h"

class HF_RawMultHistos{

 public :
  TH1I * h1_HF;
  TH1I * h1_HFpattern;

  HF_RawMultHistos();
  ~HF_RawMultHistos();
  void DefineHistos();
  void FillHistos(vector<UShort_t> *);
  TCanvas * DrawHistos();

};

#endif //HFRawMultHistos_h
