from os import *

for i in range(0,2801,50):
    system("condor_submit arguments='GJets 0 {0} {1}' plotMinPhotonDR.jdl".format(i,i+50))
for i in range(0,2151,50):
    system("condor_submit arguments='QCD 1 {0} {1}' plotMinPhotonDR.jdl".format(i,i+50))
