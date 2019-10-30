//  std includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../../SetupSpecific/setup_specific.h"


void run()
{

  Float_t BeamToFreal = Distance_FC_Target4 / SPEED_OF_LIGHT_MNS;
  Float_t OFFSET_ToFbeam[FC_nAnodes];

  for(UShort_t anode=1; anode<=FC_nAnodes; anode++)
    OFFSET_ToFbeam[anode-1] = BeamToFreal - GammaFlash_Mean[anode-1];

  ofstream OffsetToFbeam("../../SetupSpecific/BEAM_OffsetToF.h",ios::out | ios::trunc);
 
  OffsetToFbeam << "// File automatically generated from the array GammaFlash_Mean[FC_nAnodes] in setup_specific.h" << endl<<endl;
  OffsetToFbeam << "#ifndef _BEAM_OFFSET_TOF__H_" << endl;
  OffsetToFbeam << "#define _BEAM_OFFSET_TOF__H_" << endl;
  OffsetToFbeam << "Float_t BEAM_OffsetToF["<<FC_nAnodes<<"] = {" << endl;
  for(UShort_t anode=0; anode<FC_nAnodes-1; anode++)
    OffsetToFbeam << OFFSET_ToFbeam[anode] <<", " << endl;
  OffsetToFbeam << OFFSET_ToFbeam[FC_nAnodes-1] << endl;
  OffsetToFbeam << "};" << endl;
  OffsetToFbeam << "#endif " << endl;


}
