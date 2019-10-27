{
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(0.04,"xyz");
  gSystem->Load("../ClassDef/RawCoincData_C.so");
  gSystem->Load("../ClassDef/CHINU_RawQHistos_C.so");
  gSystem->Load("../ClassDef/B3_RawQHistos_C.so");
}
