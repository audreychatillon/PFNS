#!/usr/bin/env python3
import re
import sys

# OPEN INPUT FILE
print(len(sys.argv))
if(len(sys.argv)==2):
    inp = open(sys.argv[1])
else:
    inp = open('sampleFake.pid','r')
readinp = inp.readlines()

# LENGTH OF THE INPUT FILE
leninp = len(readinp)


DetType_FC=1
DetType_NDET_ChiNu=2
DetType_NDET_B3=3
DetType_PULSER=4
DetType_HF=5
DetType_MACRO=6

DetSubType_NDET_LG = 21
DetSubType_NDET_HG = 22

# CHANGE THE FOLLOWING VARIABLES IF NECESSARY
NumberOfColumns=3


# DETERMINE THE MAXIMUM VALUE OF LABEL
Val_LabelMax=0
for line in readinp:
    readline = re.split('\:',line.rstrip('\n'))
    if(len(readline)==NumberOfColumns):
        print(readline)
        #label
        Val_label=int(readline[0])
        if Val_LabelMax<Val_label:
            Val_LabelMax=Val_label

# READ THE FILE
Tab_Label2Channel=[]
Tab_DetType=[]
Tab_DetSubType=[]
for i in range (0,Val_LabelMax):
    Tab_Label2Channel.append(0)
    Tab_DetType.append(0)
    Tab_DetSubType.append(0)
Tab_LabelFC=[]
Tab_LabelNDETLG=[]
Tab_LabelNDETHG=[]
Tab_LabelNDETB3LG=[]
Tab_LabelNDETB3HG=[]
Tab_LabelFC=[]
for i in range (0,11):
    Tab_LabelFC.append(0)
Tab_LabelNDETLG=[]
Tab_LabelNDETHG=[]
for i in range (0,54):
    Tab_LabelNDETLG.append(0)
    Tab_LabelNDETHG.append(0)
Tab_LabelNDETB3LG=[]
Tab_LabelNDETB3HG=[]
for i in range (0,4):
    Tab_LabelNDETB3LG.append(0)
    Tab_LabelNDETB3HG.append(0)
Tab_FCnQ=[]
Tab_NDETnQ=[]
Tab_NDETB3nQ=[]
Val_FC=0
Val_NDET=0
Val_NDETB3=0
Val_PULSER=0
Val_HF=0
Val_MACRO=0
for line in readinp:
    readline = re.split('\:',line.rstrip('\n'))
    if(len(readline)==NumberOfColumns):
        # COLUMN 1 : label
        Val_label=int(readline[0])
        # COLUMN 2 : qdc data type
        matchObj=re.match('\s*QDC(\d)\s*',readline[1],re.M|re.I)
        if matchObj:
            Val_nQ = int(matchObj.group(1))
        # COLUMN 3 : if FC channel name
        matchObj=re.match('\s*FC_(\d+)\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_FCnQ.append(Val_nQ)
            Val_Anode = int(matchObj.group(1))
            Tab_LabelFC[Val_Anode-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_Anode
            Tab_DetType[Val_label-1] = DetType_FC
        # COLUMN 3 : if PULSER
        matchObj=re.match('\s*Pulser\s*',readline[2],re.M|re.I)
        if matchObj:
            Val_PULSER=1
            Val_PULSERlabel=Val_label
            Val_PULSERnQ=Val_nQ
            Tab_DetType[Val_label-1] = DetType_PULSER
        # COLUMN 3 : if HF BEAM
        matchObj=re.match('\s*HFBeam\s*',readline[2],re.M|re.I)
        if matchObj:
            Val_HF=1
            Val_HFlabel=Val_label
            Val_HFnQ=Val_nQ
            Tab_DetType[Val_label-1] = DetType_HF
        # COLUMN 3 : if MACRO PULSER
        matchObj=re.match('\s*Macro\s*',readline[2],re.M|re.I)
        if matchObj:
            Val_MACRO=1
            Val_MACROlabel=Val_label
            Val_MACROnQ=Val_nQ
            Tab_DetType[Val_label-1] = DetType_MACRO
        # COLUMN 3 : if L_I_i_LG
        matchObj=re.match('\s*L_I_(\d+)_LG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = int(matchObj.group(1))
            Tab_LabelNDETLG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LG
        # COLUMN 3 : if L_I_i_HG
        matchObj=re.match('\s*L_I_(\d+)_HG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = int(matchObj.group(1))
            Tab_LabelNDETHG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_HG
        # COLUMN 3 : if L_II_i_LG
        matchObj=re.match('\s*L_II_(\d+)_LG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = 9+int(matchObj.group(1))
            Tab_LabelNDETLG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LG
        # COLUMN 3 : if L_II_i_HG
        matchObj=re.match('\s*L_II_(\d+)_HG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = 9+int(matchObj.group(1))
            Tab_LabelNDETHG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_HG
        # COLUMN 3 : if L_III_i_LG
        matchObj=re.match('\s*L_III_(\d+)_LG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = 18+int(matchObj.group(1))
            Tab_LabelNDETLG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LG
        # COLUMN 3 : if L_III_i_HG
        matchObj=re.match('\s*L_III_(\d+)_HG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = 18+int(matchObj.group(1))
            Tab_LabelNDETHG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_HG
        # COLUMN 3 : if R_I_i_LG
        matchObj=re.match('\s*R_I_(\d+)_LG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = 27+int(matchObj.group(1))
            Tab_LabelNDETLG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LG
        # COLUMN 3 : if R_I_i_HG
        matchObj=re.match('\s*R_I_(\d+)_HG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = 27+int(matchObj.group(1))
            Tab_LabelNDETHG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_HG
        # COLUMN 3 : if R_II_i_LG
        matchObj=re.match('\s*R_II_(\d+)_LG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = 36+int(matchObj.group(1))
            Tab_LabelNDETLG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LG
        # COLUMN 3 : if R_II_i_HG
        matchObj=re.match('\s*R_II_(\d+)_HG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = 36+int(matchObj.group(1))
            Tab_LabelNDETHG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_HG
        # COLUMN 3 : if R_III_i_LG
        matchObj=re.match('\s*R_III_(\d+)_LG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = 45+int(matchObj.group(1))
            Tab_LabelNDETLG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LG
        # COLUMN 3 : if R_III_i_HG
        matchObj=re.match('\s*R_III_(\d+)_HG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETnQ.append(Val_nQ)
            Val_NDET = 45+int(matchObj.group(1))
            Tab_LabelNDETHG[Val_NDET-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDET
            Tab_DetType[Val_label-1] = DetType_NDET_ChiNu
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_HG
        # COLUMN 3 : if L_B3_i_LG
        matchObj=re.match('\s*L_B3_(\d+)_LG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETB3nQ.append(Val_nQ)
            Val_NDETB3 = int(matchObj.group(1))
            Tab_LabelNDETB3LG[Val_NDETB3-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDETB3
            Tab_DetType[Val_label-1] = DetType_NDET_B3
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LG
        # COLUMN 3 : if L_B3_i_HG
        matchObj=re.match('\s*L_B3_(\d+)_HG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETB3nQ.append(Val_nQ)
            Val_NDETB3 = int(matchObj.group(1))
            Tab_LabelNDETB3HG[Val_NDETB3-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDETB3
            Tab_DetType[Val_label-1] = DetType_NDET_B3
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_HG
        # COLUMN 3 : if R_B3_i_LG
        matchObj=re.match('\s*R_B3_(\d+)_LG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETB3nQ.append(Val_nQ)
            Val_NDETB3 = 2+int(matchObj.group(1))
            Tab_LabelNDETB3LG[Val_NDETB3-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDETB3
            Tab_DetType[Val_label-1] = DetType_NDET_B3
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LG
        # COLUMN 3 : if R_B3_i_HG
        matchObj=re.match('\s*R_B3_(\d+)_HG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_NDETB3nQ.append(Val_nQ)
            Val_NDETB3 = 2+int(matchObj.group(1))
            Tab_LabelNDETB3HG[Val_NDETB3-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_NDETB3
            Tab_DetType[Val_label-1] = DetType_NDET_B3
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_HG
inp.close()

# fission chamber
if(len(Tab_FCnQ)>0):
    Val_FC=1
    Val_FCnQ=Tab_FCnQ[0]
    for i in range (1,len(Tab_FCnQ)):
        if(Tab_FCnQ[i]!=Val_FCnQ):
            print('ERROR FC has different data type for channel '+str(i+1))
    Str_LabelFC=str(Tab_LabelFC[0])+','
    for i in range(1,len(Tab_LabelFC)):
        Str_LabelFC+= str(Tab_LabelFC[i])+','
    Str_LabelFC=Str_LabelFC[:-1]


# CHI-NU NDET
if(len(Tab_NDETnQ)>0):
    Val_NDET=1
    Val_NDETnQ=Tab_NDETnQ[0]
    for i in range (1,len(Tab_NDETnQ)):
        if(Tab_NDETnQ[i]!=Val_NDETnQ):
            print('ERROR CHI-NU NDET has different data type for index '+str(i))

    Str_LabelNDETLG=str(Tab_LabelNDETLG[0])+','
    for i in range (1,len(Tab_LabelNDETLG)):
        Str_LabelNDETLG+= str(Tab_LabelNDETLG[i])+','
    Str_LabelNDETLG=Str_LabelNDETLG[:-1]

    Str_LabelNDETHG=str(Tab_LabelNDETHG[0])+','
    for i in range (1,len(Tab_LabelNDETHG)):
        Str_LabelNDETHG+= str(Tab_LabelNDETHG[i])+','
    Str_LabelNDETHG=Str_LabelNDETHG[:-1]

# B3 NDET
if(len(Tab_NDETB3nQ)>0):
    Val_NDETB3=1
    Val_NDETB3nQ=Tab_NDETB3nQ[0]
    for i in range (1,len(Tab_NDETB3nQ)):
        if(Tab_NDETB3nQ[i]!=Val_NDETB3nQ):
            print('ERROR B3 NDET has different data type for index '+str(i))

    Str_LabelNDETB3LG=str(Tab_LabelNDETB3LG[0])+','
    for i in range (1,len(Tab_LabelNDETB3LG)):
        Str_LabelNDETB3LG+= str(Tab_LabelNDETB3LG[i])+','
    Str_LabelNDETB3LG=Str_LabelNDETB3LG[:-1]

    Str_LabelNDETB3HG=str(Tab_LabelNDETB3HG[0])+','
    for i in range (1,len(Tab_LabelNDETB3HG)):
        Str_LabelNDETB3HG+= str(Tab_LabelNDETB3HG[i])+','
    Str_LabelNDETB3HG=Str_LabelNDETB3HG[:-1]


# Tab with dimension LabelMax
Str_Label2Channel = str(Tab_Label2Channel[0])+','
Str_DetType = str(Tab_DetType[0])+','
Str_DetSubType = str(Tab_DetSubType[0])+','
for i in range (1,Val_LabelMax):
    Str_Label2Channel+=str(Tab_Label2Channel[i])+','
    Str_DetType+=str(Tab_DetType[i])+','
    Str_DetSubType+=str(Tab_DetSubType[i])+','
Str_Label2Channel=Str_Label2Channel[:-1]
Str_DetType=Str_DetType[:-1]
Str_DetSubType=Str_DetSubType[:-1]


# ### OPEN OUTPUT FILE ### #
outp = open('pid.h','w')
outp.write('// ========================================== //\n')
outp.write('// This file is generated by readSamplePid.py //\n')
outp.write('// ========================================== //\n\n\n')
outp.write('#ifndef __SAMPLE_H__\n')
outp.write('#define __SAMPLE_H__\n\n')
# fission chamber
outp.write('\n\n')
outp.write('// ===  =============== === //\n')
outp.write('// ===  FISSION CHAMBER === //\n')
outp.write('// ===  =============== === //\n\n')
outp.write('#define FC '+str(Val_FC)+'  // if 0: no FC defined in the DAQ\n\n')
if(len(Tab_FCnQ)>0):
    if(len(Tab_FCnQ)>0):
        outp.write('static int LabelFC['+str(len(Tab_LabelFC))+'] = {'+Str_LabelFC+'};\n\n')
# CHI-NU NDET
outp.write('\n\n')
outp.write('// ===  ======================== === //\n')
outp.write('// ===  CHI-NU NEUTRON DETECTORS === //\n')
outp.write('// ===  ======================== === //\n\n')
outp.write('#define CHINU '+str(Val_NDET)+'  // if 0: no Chi-Nu detector defined in the DAQ\n\n')
if(len(Tab_NDETnQ)>0):
    if(len(Tab_LabelNDETLG)>0):
        outp.write('static int Label_CHINU_LG['+str(len(Tab_LabelNDETLG))+'] = {'+Str_LabelNDETLG+'};\n\n')
    if(len(Tab_LabelNDETHG)>0):
        outp.write('static int Label_CHINU_HG['+str(len(Tab_LabelNDETHG))+'] = {'+Str_LabelNDETHG+'};\n\n')
# B3 NDET
outp.write('\n\n')
outp.write('// ===  ==================== === //\n')
outp.write('// ===  B3 NEUTRON DETECTORS === //\n')
outp.write('// ===  ==================== === //\n\n')
outp.write('#define B3 '+str(Val_NDETB3)+'  // if 0: no B3 detector defined in the DAQ\n\n')
if(len(Tab_NDETB3nQ)>0):
    if(len(Tab_LabelNDETB3LG)>0):
        outp.write('static int Label_B3_LG['+str(len(Tab_LabelNDETB3LG))+'] = {'+Str_LabelNDETB3LG+'};\n\n')
    if(len(Tab_LabelNDETB3HG)>0):
        outp.write('static int Label_B3_HG['+str(len(Tab_LabelNDETB3HG))+'] = {'+Str_LabelNDETB3HG+'};\n\n')
# pulser
outp.write('\n\n')
outp.write('// ===  ====== === //\n')
outp.write('// ===  PULSER === //\n')
outp.write('// ===  ====== === //\n\n')
outp.write('#define PULSER '+str(Val_PULSER)+'  // if 0: no pulser defined in the DAQ\n\n')
if (Val_PULSER>0):
    outp.write('#define Label_PULSER '+str(Val_PULSERlabel)+'\n\n')
# HF BEAM
outp.write('\n\n')
outp.write('// ===  == === //\n')
outp.write('// ===  HF === //\n')
outp.write('// ===  == === //\n\n')
outp.write('#define HF '+str(Val_HF)+'  // if 0: no HF defined in the DAQ\n\n')
if (Val_HF>0):
    outp.write('#define Label_HF '+str(Val_HFlabel)+'\n\n')
# MACRO PULSER
outp.write('\n\n')
outp.write('// ===  =========== === //\n')
outp.write('// ===  MACRO PULSE === //\n')
outp.write('// ===  =========== === //\n\n')
outp.write('#define MACRO '+str(Val_MACRO)+'  // if 0: no Macro Pulse defined in the DAQ\n\n')
if (Val_MACRO>0) :
    outp.write('#define Label_MACRO '+str(Val_MACROlabel)+'\n\n')
# Label --> Channel
outp.write('\n\n')
outp.write('// ===  =========================== === //\n')
outp.write('// ===  FOR GENERAL MAPPING PURPOSE === //\n')
outp.write('// ===  =========================== === //\n\n')
outp.write('static int Label2Ch['+str(len(Tab_Label2Channel))+'] = {'+Str_Label2Channel+'};\n\n')
outp.write('static int DetType['+str(len(Tab_DetType))+'] = {'+Str_DetType+'};\n\n')
outp.write('static int DetSubType['+str(len(Tab_DetSubType))+'] = {'+Str_DetSubType+'};\n\n')
outp.write('\n\n')
outp.write('#endif //__SAMPLE_H___')
outp.close()
