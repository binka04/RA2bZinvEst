from os import *

maxFileIndex=7400
nFilesPerJob=50

for i in range(0,maxFileIndex,nFilesPerJob):
    print "condor_submit arguments='{0} {1}' triggerEffInputs.jdl".format(i,i+nFilesPerJob)
    system("condor_submit arguments='{0} {1}' triggerEffInputs.jdl".format(i,i+nFilesPerJob))
