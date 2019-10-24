#ifndef CHINURawMultHistos_h
#define CHINURawMultHistos_h

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TFile.h"
#include "../../SetupSpecific/setup_specific.h"

class CHINU_RawMultHistos{

 public :
  TH1I * h1_LG;
  TH1I * h1_HG;
  TH1I * h1_LGpattern[6];
  TH1I * h1_HGpattern[6];

  CHINU_RawMultHistos();
  ~CHINU_RawMultHistos();
  void DefineHistos();
  void FillHistos(vector<UShort_t> *, vector<UShort_t> *);
  TCanvas * DrawHistos();
};

#endif //CHINURawMultHistos_h
