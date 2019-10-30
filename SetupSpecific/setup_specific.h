#include "TString.h"

#ifndef SETUP_SPEC__H
#define SETUP_SPEC__H

// *** ************************************** *** //
// *** You should change the following define *** //
// *** ************************************** *** //
#define TRIGGER_MODE 1      //0: TRIGGER LESS 1: COINC
#define LABEL_GROUP 3000
#define FC_nQ     3
#define CHINU_nQ  2
#define B3_nQ     2
#define PULSER_nQ 1
#define HF_nQ     1
#define MACRO_nQ  1


// FISSION CHAMBER
#define FC_nAnodes 11
#define Distance_FC_Target4 22 //m
#define ToFcutPFN 15 // 15ns = 24 MeV

//RUN 102
/*
static float GammaFlash_Mean[11] = {
  -7.99389e+02,
  0,
  0,
  -7.93712e+02,
  -7.94610e+02,
  -7.94183e+02,
  -7.92085e+02,
  -7.91687e+02,
  -7.95208e+02,
  -7.94937e+02,
  -7.93481e+02
};
*/

//RUN 103
static float GammaFlash_Mean[11] = 
  {
    -8.00392e+02,
    0,
    -7.95102e+02,
    -7.93581e+02,
    -7.94597e+02,
    -7.93983e+02,
    -7.91993e+02,
    -7.92007e+02,
    -7.95080e+02,
    -7.94383e+02,
    -7.93319e+02
  };

static float GammaFlash_Sigma[11] = 
  {
    3.61925e-01,
    0,
    4.10208e-01,
    1.50834e+00,
    5.21320e-01,
    1.62029e+00,
    1.86833e+00,
    8.81608e-01,
    1.67800e+00,
    1.52894e+00,
    1.49861e+00
  };







// CHI-NU DETECTORS 
#define CHINU_nDets 54
static int Side_ChiNu[CHINU_nDets] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
static int Ring_ChiNu[CHINU_nDets] = {1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3};
static int Rank_ChiNu[CHINU_nDets] = {1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9};
static double Distance_FC_ChiNu[CHINU_nDets] = {
  1., 1., 1., 1., 1., 1., 1., 1., 1., //L_I
  1., 1., 1., 1., 1., 1., 1., 1., 1., //L_II
  1., 1., 1., 1., 1., 1., 1., 1., 1., //L_III
  1., 1., 1., 1., 1., 1., 1., 1., 1., //R_I
  1., 1., 1., 1., 1., 1., 1., 1., 1., //R_II
  1., 1., 1., 1., 1., 1., 1., 1., 1.  //R_III
};
// CALIBRATED ToF GAMMA
#define CHINU_GammaToFmin 1.85
#define CHINU_GammaToFmax 4.8


// B3 DETECTORS 
#define B3_nDets 4
static int Side_B3[B3_nDets] = {1,1,2,2};
static int Pos_B3[B3_nDets] = {1,2,1,2};
static double Distance_FC_B3[B3_nDets] = {
  1.3, 1.3, // LEFT
  1.3, 1.3  // RIGHT
};
#define B3_GammaToFmin 2.85
#define B3_GammaToFmax 5.8



// FOR DETECTOR NAMEING PURPOSE
TString sideVal[2] = {"L","R"};
TString ringVal[3] = {"I","II","III"};
TString posVal[2] = {"BACK","FRONT"};


#endif //SETUP_SPEC__H
