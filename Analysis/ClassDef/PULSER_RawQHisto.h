#ifndef PULSERRawQHisto_h
#define PULSERRawQHisto_h

#include <vector>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "../../SetupSpecific/setup_specific.h"

class PULSER_RawQHisto{

 public :
  TH1I * h1_Q1;

  PULSER_RawQHisto();
  ~PULSER_RawQHisto();
  void DefineHisto();
  void FillHisto(vector<UShort_t> *);
  TCanvas * DrawHisto();

};

#endif //PULSERRawQHisto_h
