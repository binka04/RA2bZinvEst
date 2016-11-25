CC=g++
CFLAGS=
LIBS=`root-config --cflags --glibs` -L $(ROOTSYS)/lib
INCLUDES=-I$(CMSSW_BASE)/src/
EXECS=plotObs_baseline plotRzGamma triggerEffInputs plotPurityProperties
$(EXECS):RA2bTree.cc definitions.h skimSamples.cc plotterUtils.cc 
plotObs_GJetsCR_baseline:plotObs_GJetsCR_baseline.cc
plotObs_baseline:plotObs_baseline.cc
plotRzGamma:plotRzGamma.cc CMS_lumi.h CMS_lumi.cc
triggerEffInputs:triggerEffInputs.cc
plotPurityProperties:plotPurityProperties.cc

all:$(EXECS)

plotPurityProperties:
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) plotPurityProperties.cc -o ../bin/$@
triggerEffInputs:
	$(CC) -g $(CFLAGS) $(LIBS) $(INCLUDES) triggerEffInputs.cc -o ../bin/$@
plotRzGamma:
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) plotRzGamma.cc -o ../bin/$@
plotObs_baseline:
	$(CC) -g $(CFLAGS) $(LIBS) $(INCLUDES) plotObs_baseline.cc -o ../bin/$@

clean : 
	rm $(EXECS) 