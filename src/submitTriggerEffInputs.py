from os import *

for i in range(0,5001,50):
    system("condor_submit arguments='{0} {1}' triggerEffInputs.jdl".format(i,i+50))
