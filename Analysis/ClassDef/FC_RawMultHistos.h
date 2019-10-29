#ifndef FCRawMultHistos_h
#define FCRawMultHistos_h

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "../../SetupSpecific/setup_specific.h"

class FC_RawMultHistos{

 public :
  TH1I * h1_FC;
  TH1I * h1_FCpattern;
  TH1I * h1_multPerAnode[FC_nAnodes];
  TH2I * h2_MultTot_vs_MultAnode[FC_nAnodes];

  FC_RawMultHistos();
  ~FC_RawMultHistos();
  void DefineHistos();
  void FillHistos(vector<UShort_t> *,UShort_t*);
  TCanvas * Draw1DHistos();
  TCanvas * DrawMultPerAnode();
  TCanvas * Draw2DHistos();

};

#endif //FCRawMultHistos_h
