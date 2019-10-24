#!/usr/bin/env python3
import re

DetType_FC=1
DetType_NDET=2
DetType_PULSER=3
DetType_HF=4

DetSubType_NDET_LILG=111
DetSubType_NDET_LIHG=112
DetSubType_NDET_LIILG=121
DetSubType_NDET_LIIHG=122
DetSubType_NDET_LIIILG=131
DetSubType_NDET_LIIIHG=132
DetSubType_NDET_LB3LG=141
DetSubType_NDET_LB3HG=142

DetSubType_NDET_RILG=211
DetSubType_NDET_RIHG=212
DetSubType_NDET_RIILG=221
DetSubType_NDET_RIIHG=222
DetSubType_NDET_RIIILG=231
DetSubType_NDET_RIIIHG=232
DetSubType_NDET_RB3LG=241
DetSubType_NDET_RB3HG=242

# CHANGE THE FOLLOWING VARIABLES IF NECESSARY
NumberOfColumns=3

# OPEN INPUT FILE
inp = open('sample.pid','r')
readinp = inp.readlines()

# LENGTH OF THE INPUT FILE
leninp = len(readinp)
print(str(leninp)+ ' lines in sample.pid \n')

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
for i in range (0,11):
    Tab_LabelFC.append(0)
Tab_LabelLILG=[]
Tab_LabelLIHG=[]
Tab_LabelLIILG=[]
Tab_LabelLIIHG=[]
Tab_LabelLIIILG=[]
Tab_LabelLIIIHG=[]
for i in range (0,9):
    Tab_LabelLILG.append(0)
    Tab_LabelLIHG.append(0)
    Tab_LabelLIILG.append(0)
    Tab_LabelLIIHG.append(0)
    Tab_LabelLIIILG.append(0)
    Tab_LabelLIIIHG.append(0)
Tab_FCnQ=[]
Tab_LILGnQ=[]
Tab_LIHGnQ=[]
Tab_LIILGnQ=[]
Tab_LIIHGnQ=[]
Tab_LIIILGnQ=[]
Tab_LIIIHGnQ=[]
for line in readinp:
    readline = re.split('\:',line.rstrip('\n'))
    if(len(readline)==NumberOfColumns):
        #label
        Val_label=int(readline[0])
        # qdc data type
        matchObj=re.match('\s*QDC(\d)\s*',readline[1],re.M|re.I)
        if matchObj:
            Val_nQ = int(matchObj.group(1))
        # FC channel name
        matchObj=re.match('\s*FC\_(\d+)\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_FCnQ.append(Val_nQ)
            Val_Anode = int(matchObj.group(1))
            Tab_LabelFC[Val_Anode-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_Anode
            Tab_DetType[Val_label-1] = DetType_FC
        # L_I_i_LG
        matchObj=re.match('\s*L\_I_(\d+)_LG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_LILGnQ.append(Val_nQ)
            Val_LILG = int(matchObj.group(1))
            Tab_LabelLILG[Val_LILG-1] = Val_label
            Tab_Label2Channel[Val_label-1] = Val_LILG
            Tab_DetType[Val_label-1] = DetType_NDET
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LILG
        # L_I_i_HG
        matchObj=re.match('\s*L\_I_(\d+)_HG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_LIHGnQ.append(Val_nQ)
            Val_LIHG = int(matchObj.group(1))
            Tab_Label2Channel[Val_label-1] = Val_LIHG
            Tab_LabelLIHG[Val_LIHG-1] = Val_label
            Tab_DetType[Val_label-1] = DetType_NDET
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LIHG
        # L_II_i_LG
        matchObj=re.match('\s*L\_II_(\d+)_LG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_LIILGnQ.append(Val_nQ)
            Val_LIILG = int(matchObj.group(1))
            Tab_Label2Channel[Val_label-1] = Val_LIILG
            Tab_LabelLIILG[Val_LIILG-1] = Val_label
            Tab_DetType[Val_label-1] = DetType_NDET
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LIIHG
        # L_II_i_HG
        matchObj=re.match('\s*L\_II_(\d+)_HG\s*',readline[2],re.M|re.I)
        if matchObj:
            Tab_LIIHGnQ.append(Val_nQ)
            Val_LIIHG = int(matchObj.group(1))
            Tab_Label2Channel[Val_label-1] = Val_LIIHG
            Tab_LabelLIIHG[Val_LIIHG-1] = Val_label
            Tab_DetType[Val_label-1] = DetType_NDET
            Tab_DetSubType[Val_label-1] = DetSubType_NDET_LIIHG
inp.close()

# fission chamber
print('Tab_LabelFC: ')
print(Tab_LabelFC)
print('\n')
Val_FCnQ=Tab_FCnQ[0]
Str_LabelFC=str(Tab_LabelFC[0])+','
for i in range (1,len(Tab_FCnQ)):
    Str_LabelFC+= str(Tab_LabelFC[i])+','
    if(Tab_FCnQ[i]!=Val_FCnQ):
        print('ERROR FC has different data type for channel '+str(i+1))
Str_LabelFC=Str_LabelFC[:-1]

# L_I LG
print('Tab_LabelLILG: ')
print(Tab_LabelLILG)
print('\n')
Val_LILGnQ=Tab_LILGnQ[0]
Str_LabelLILG=str(Tab_LabelLILG[0])+','
for i in range (1,len(Tab_LILGnQ)):
    Str_LabelLILG+= str(Tab_LabelLILG[i])+','
    if(Tab_LILGnQ[i]!=Val_LILGnQ):
        print('ERROR LILG has different data type for channel '+str(i+1))
Str_LabelLILG=Str_LabelLILG[:-1]

# L_I HG
print('Tab_LabelLIHG: ')
print(Tab_LabelLIHG)
print('\n')
Val_LIHGnQ=Tab_LIHGnQ[0]
Str_LabelLIHG=str(Tab_LabelLIHG[0])+','
for i in range (1,len(Tab_LIHGnQ)):
    Str_LabelLIHG+= str(Tab_LabelLIHG[i])+','
    if(Tab_LIHGnQ[i]!=Val_LIHGnQ):
        print('ERROR LIHG has different data type for channel '+str(i+1))
Str_LabelLIHG=Str_LabelLIHG[:-1]

# L_II LG
print('Tab_LabelLIILG: ')
print(Tab_LabelLIILG)
print('\n')
Val_LIILGnQ=Tab_LIILGnQ[0]
Str_LabelLIILG=str(Tab_LabelLIILG[0])+','
for i in range (1,len(Tab_LIILGnQ)):
    Str_LabelLIILG+= str(Tab_LabelLIILG[i])+','
    if(Tab_LIILGnQ[i]!=Val_LIILGnQ):
        print('ERROR LIILG has different data type for channel '+str(i+1))
Str_LabelLIILG=Str_LabelLIILG[:-1]

# L_II HG
print('Tab_LabelLIIHG: ')
print(Tab_LabelLIIHG)
print('\n')
Val_LIIHGnQ=Tab_LIIHGnQ[0]
Str_LabelLIIHG=str(Tab_LabelLIIHG[0])+','
for i in range (1,len(Tab_LIIHGnQ)):
    Str_LabelLIIHG+= str(Tab_LabelLIIHG[i])+','
    if(Tab_LIIHGnQ[i]!=Val_LIIHGnQ):
        print('ERROR LIIHG has different data type for channel '+str(i+1))
Str_LabelLIIHG=Str_LabelLIIHG[:-1]

# DATA TYPE FOR NDET 
Val_NDETnQ=0
if (Val_LILGnQ==Val_LIHGnQ):
    Val_NDETnQ=Val_LILGnQ

# Tab with dimension LabelMax
print('Tab_Label2Channel: ')
print(Tab_Label2Channel)
print('\n')
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
outp = open('sample.h','w')
outp.write('// ========================================== //\n')
outp.write('// This file is generated by readSamplePid.py //\n')
outp.write('// ========================================== //\n\n\n')
outp.write('#ifndef __SAMPLE_H__\n')
outp.write('#define __SAMPLE_H__\n\n')
# fission chamber
outp.write('// ===  FISSION CHAMBER === //\n')
outp.write('#define FC_nQ '+str(Val_FCnQ)+'\n')
if(len(Tab_FCnQ)>0):
    outp.write('static int LabelFC['+str(len(Tab_FCnQ))+'] = {'+Str_LabelFC+'};\n\n')
# NDET
outp.write('// ===  NEUTRON DETECTORS === //\n')
outp.write('#define NDET_nQ '+str(Val_NDETnQ)+'\n')
if(len(Tab_LILGnQ)>0):
    outp.write('static int LabelLILG['+str(len(Tab_LILGnQ))+'] = {'+Str_LabelLILG+'};\n')
if(len(Tab_LIHGnQ)>0):
    outp.write('static int LabelLIHG['+str(len(Tab_LIHGnQ))+'] = {'+Str_LabelLIHG+'};\n')
if(len(Tab_LIILGnQ)>0):
    outp.write('static int LabelLIILG['+str(len(Tab_LIILGnQ))+'] = {'+Str_LabelLIILG+'};\n')
if(len(Tab_LIIHGnQ)>0):
    outp.write('static int LabelLIIHG['+str(len(Tab_LIIHGnQ))+'] = {'+Str_LabelLIIHG+'};\n')
# L_III LG
# L_III HG
# R_I LG
# R_I HG
# R_II LG
# R_II HG
# R_III LG
# R_III HG
# L_B3_LG
# R_B3_LG
# pulser
# HF
# Label --> Channel
outp.write('\n')
outp.write('static int Label2Ch['+str(len(Tab_Label2Channel))+'] = {'+Str_Label2Channel+'};\n')
outp.write('static int DetType['+str(len(Tab_DetType))+'] = {'+Str_DetType+'};\n')
outp.write('static int DetSubType['+str(len(Tab_DetSubType))+'] = {'+Str_DetSubType+'};\n')
outp.write('\n')
outp.write('#endif //__SAMPLE_H___')
outp.close()
