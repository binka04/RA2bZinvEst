#!/bin/bash

SAMPLENAME=$1
INDEX=$2
FISTFILE=$3
LASTFILE=$4

tar -xf workingArea.tar
cd CMSSW_7_4_2/src/
eval `scramv1 runtime -sh`

cd RA2bZinvEst/src

./plotMinPhotonDR $SAMPLENAME $INDEX $FISTFILE $LASTFILE

ls 

FILE=`ls *.root`

echo "copying ${FILE} to /store/user/awhitbe1/fragmentationStudies/"
xrdcp --force $FILE root://cmseos.fnal.gov///store/user/awhitbe1/fragmentationStudies/$FILE
rm $FILE