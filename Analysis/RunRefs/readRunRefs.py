#!/usr/bin/env python3
import re

# CHANGE THE FOLLOWING VARIABLES IF NECESSARY
NumberOfColumns=4

# OPEN INPUT FILE
inp = open('RunRefs.dat','r')
readinp = inp.readlines()

# LENGTH OF THE INPUT FILE
leninp = len(readinp)
print(str(leninp)+ ' lines in RunRefs.dat \n')

# READ THE FILE
Val_nRunsTot=0
Str_numRunTot='{'
Str_nFilesTot='{'
Str_T0refTot='{'
Val_nRunsU=0
Str_numRunU='{'
Str_nFilesU='{'
Str_T0refU='{'
Val_nRunsCf=0
Str_numRunCf='{'
Str_nFilesCf='{'
Str_T0refCf='{'
Val_nRunsBdF=0
Str_numRunBdF='{'
Str_nFilesBdF='{'
Val_nRunsCs=0
Str_numRunCs='{'
Str_nFilesCs='{'
for line in readinp:
    readline = re.split('\|',line.rstrip('\n'))
    print(readline)
    if(len(readline)==NumberOfColumns):
        Val_nRunsTot+=1

        # get the run number
        matchObj=re.match('\s*(\d+)\s*',readline[0],re.M|re.I)
        if matchObj:
            numRun = int(matchObj.group(1))
        Str_numRunTot+=str(numRun)+','
        # get the number of files per run
        matchObj=re.match('\s*(\d+)\s*',readline[1],re.M|re.I)
        if matchObj:
            nFiles = int(matchObj.group(1))
        Str_nFilesTot+=str(nFiles)+','
        # get the T0 reference
        matchObj=re.match('\s*(\d+)\s*',readline[3],re.M|re.I)
        if matchObj:
            T0ref = int(matchObj.group(1))        
        Str_T0refTot+=str(T0ref)+','
        # get the data type
        matchObj=re.match('\s*([UCfBdFCs]+)\s*',readline[2],re.M|re.I)
        if matchObj:
            dataType = matchObj.group(1)
            if dataType=='U':
                Val_nRunsU+=1
                Str_numRunU+=str(numRun)+','
                Str_nFilesU+=str(nFiles)+','
                Str_T0refU+=str(T0ref)+','
            if dataType=='Cf':
                Val_nRunsCf+=1
                Str_numRunCf+=str(numRun)+','
                Str_nFilesCf+=str(nFiles)+','
                Str_T0refCf+=str(T0ref)+','
            if dataType=='BdF':
                Val_nRunsBdF+=1
                Str_numRunBdF+=str(numRun)+','
                Str_nFilesBdF+=str(nFiles)+','
            if dataType=='Cs':
                Val_nRunsCs+=1
                Str_numRunCs+=str(numRun)+','
                Str_nFilesCs+=str(nFiles)+','
Str_numRunTot=Str_numRunTot[:-1]+'}'
Str_nFilesTot=Str_nFilesTot[:-1]+'}'
Str_T0refTot=Str_T0refTot[:-1]+'}'
Str_numRunU=Str_numRunU[:-1]+'}'
Str_nFilesU=Str_nFilesU[:-1]+'}'
Str_T0refU=Str_T0refU[:-1]+'}'
Str_numRunCf=Str_numRunCf[:-1]+'}'
Str_nFilesCf=Str_nFilesCf[:-1]+'}'
Str_T0refCf=Str_T0refCf[:-1]+'}'
Str_numRunBdF=Str_numRunBdF[:-1]+'}'
Str_nFilesBdF=Str_nFilesBdF[:-1]+'}'
Str_numRunCs=Str_numRunCs[:-1]+'}'
Str_nFilesCs=Str_nFilesCs[:-1]+'}'
inp.close()
    
# OPEN OUTPUT FILE
outp = open('RunRefs.h','w')
outp.write('#define TOT_NRUNS '+str(Val_nRunsTot)+'\n')
if (Val_nRunsTot>0):
    outp.write('UInt_t tot_numRun[TOT_NRUNS]='+Str_numRunTot+';\n')
    outp.write('UInt_t tot_nFiles[TOT_NRUNS]='+Str_nFilesTot+';\n')
    outp.write('UInt_t tot_T0ref[TOT_NRUNS]='+Str_T0refTot+';\n')
outp.write('\n#define U_NRUNS '+str(Val_nRunsU)+'\n')
if (Val_nRunsU>0):
    outp.write('UInt_t U_numRun[U_NRUNS]='+Str_numRunU+';\n')
    outp.write('UInt_t U_nFiles[U_NRUNS]='+Str_nFilesU+';\n')
    outp.write('UInt_t U_T0ref[U_NRUNS]='+Str_T0refU+';\n')
outp.write('\n#define Cf_NRUNS '+str(Val_nRunsCf)+'\n')
if (Val_nRunsCf>0):
    outp.write('UInt_t Cf_numRun[Cf_NRUNS]='+Str_numRunCf+';\n')
    outp.write('UInt_t Cf_nFiles[Cf_NRUNS]='+Str_nFilesCf+';\n')
    outp.write('UInt_t Cf_T0ref[Cf_NRUNS]='+Str_T0refCf+';\n')
outp.write('\n#define BdF_NRUNS '+str(Val_nRunsBdF)+'\n')
if (Val_nRunsBdF>0):
    outp.write('UInt_t BdF_numRun[BdF_NRUNS]='+Str_numRunBdF+';\n')
    outp.write('UInt_t BdF_nFiles[BdF_NRUNS]='+Str_nFilesBdF+';\n')
outp.write('\n#define Cs_NRUNS '+str(Val_nRunsCs)+'\n')
if (Val_nRunsCs>0):
    outp.write('UInt_t Cs_numRun[Cs_NRUNS]='+Str_numRunCs+';\n')
    outp.write('UInt_t Cs_nFiles[Cs_NRUNS]='+Str_nFilesCs+';\n')
outp.close()
