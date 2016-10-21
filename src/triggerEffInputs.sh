#!/bin/bash

FISTFILE=$1
LASTFILE=$2

tar -xf workingArea.tar
cd CMSSW_7_4_2/src/
eval `scramv1 runtime -sh`

cd RA2bZinvEst/src

./triggerEffInputs $FISTFILE $LASTFILE

ls 

FILE=`ls *.root`

echo "copying ${FILE} to /store/user/awhitbe1/triggerEff/"
xrdcp --force $FILE root://cmseos.fnal.gov///store/user/awhitbe1/triggerEff/$FILE
rm $FILE