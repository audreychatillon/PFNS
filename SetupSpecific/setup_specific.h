#include "TString.h"

#ifndef SETUP_SPEC__H
#define SETUP_SPEC__H

// *** You should change the following define *** //
#define TRIGGER_MODE 1      //0: TRIGGER LESS 1: COINC
#define LABEL_GROUP 3000
#define FC_nQ     4
#define CHINU_nQ  2
#define B3_nQ     2
#define PULSER_nQ 1
#define HF_nQ     1
#define MACRO_nQ  1


// FISSION CHAMBER
#define FC_nAnodes 11

// CHI-NU DETECTORS 
#define CHINU_nDets 54
static int Side_ChiNu[CHINU_nDets] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
static int Ring_ChiNu[CHINU_nDets] = {1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3};
static int Rank_ChiNu[CHINU_nDets] = {1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9};
static float Distance_FC_ChiNu[CHINU_nDets] = {
  1., 1., 1., 1., 1., 1., 1., 1., 1., //L_I
  1., 1., 1., 1., 1., 1., 1., 1., 1., //L_II
  1., 1., 1., 1., 1., 1., 1., 1., 1., //L_III
  1., 1., 1., 1., 1., 1., 1., 1., 1., //R_I
  1., 1., 1., 1., 1., 1., 1., 1., 1., //R_II
  1., 1., 1., 1., 1., 1., 1., 1., 1.  //R_III
};

// B3 DETECTORS 
#define B3_nDets 4
static int Side_B3[B3_nDets] = {1,1,2,2};
static int Pos_B3[B3_nDets] = {1,2,1,2};
static float Distance_FC_B3[B3_nDets] = {
  1., 1., // LEFT
  1., 1.  // RIGHT
};

// FOR DETECTOR NAMEING PURPOSE
TString sideVal[2] = {"L","R"};
TString ringVal[3] = {"I","II","III"};
TString posVal[2] = {"BACK","FRONT"};


#endif //SETUP_SPEC__H
