#ifndef PULSERRawMultHistos_h
#define PULSERRawMultHistos_h

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "../../SetupSpecific/setup_specific.h"

class PULSER_RawMultHistos{

 public :
  TH1I * h1_PULSER;
  TH1I * h1_PULSERpattern;

  PULSER_RawMultHistos();
  ~PULSER_RawMultHistos();
  void DefineHistos();
  void FillHistos(vector<UShort_t> *);
  TCanvas * DrawHistos();

};

#endif //PULSERRawMultHistos_h
