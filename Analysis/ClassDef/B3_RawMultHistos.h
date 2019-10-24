#ifndef B3RawMultHistos_h
#define B3RawMultHistos_h

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TFile.h"
#include "../../SetupSpecific/setup_specific.h"

class B3_RawMultHistos{

 public :
  TH1I * h1_LG;
  TH1I * h1_HG;
  TH1I * h1_LGpattern[2];
  TH1I * h1_HGpattern[2];

  B3_RawMultHistos();
  ~B3_RawMultHistos();
  void DefineHistos();
  void FillHistos(vector<UShort_t> *, vector<UShort_t> *);
  TCanvas * DrawHistos();
};

#endif //B3RawMultHistos_h
