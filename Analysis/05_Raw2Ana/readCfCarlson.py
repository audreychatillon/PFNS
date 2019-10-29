#!/usr/bin/env python3
import re

# OPEN INPUT FILE
inp = open('Cf_Carlson.txt','r')
readinp = inp.readlines()

# LENGTH OF THE INPUT FILE
leninp = len(readinp)
print(str(leninp)+ ' lines in Cf_Cf_Carlson \n')

Str_BinLowEdge='{'
Str_BinContent='{'
nbins=0;
for line in readinp:
    matchObj=re.match('#(\d*\D*)',line,re.M|re.I)
    if matchObj:
        print(line)
    else:
        readline = re.split('\s+',line.rstrip('\n'))
        Str_BinLowEdge += str(readline[1])+','
        Str_BinContent += str(readline[3])+','
        nbins+=1


Str_BinLowEdge+='20.}'
Str_BinContent=Str_BinContent[:-1]+'}'

# OPEN OUTPUT FILE
outp = open('Cf_Carlson.h','w')
outp.write('#ifndef CF_CARLSON__H\n')
outp.write('#define CF_CARLSON__H\n')
outp.write('\n')
outp.write('#define nBinsEne '+str(nbins)+'\n');
outp.write('\n')
outp.write('static float binEdges[nBinsEne+1] = '+Str_BinLowEdge+';\n')
outp.write('\n')
outp.write('static float binContent[nBinsEne] = '+Str_BinContent+';\n')
outp.write('\n')
outp.write('#endif //CF_CARLSON__H')
