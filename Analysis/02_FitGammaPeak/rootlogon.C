{
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(0.04,"xyz");
  gSystem->Load("../ClassDef/RawCoincData_C.so");
  gSystem->Load("../ClassDef/CHINU_RawToFHistos_C.so");
  gSystem->Load("../ClassDef/B3_RawToFHistos_C.so");
}
